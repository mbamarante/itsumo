/*
 * Created on 30 mai 2005
 *
 */
package frodo.interaction.meeting.roundtrip;

import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Vector;

import frodo.COP.ValuedConstraint;
import frodo.CSP.CSPVariable;
import frodo.CSP.VariableId;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.OCOPBasicProblem;
import frodo.OCOP.DPOP.Dimension;
import frodo.OCOP.DPOP.DPOPNode;
import frodo.interaction.meeting.graph.Heuristics;
import frodo.interaction.meeting.graph.PseudoTreeEditor;
import frodo.interaction.meeting.gui.MeetingTreeNode;
import frodo.interaction.meeting.gui.ParticipantTreeNode;
import frodo.interaction.meeting.gui.RootTreeNode;
import frodo.monitoring.interfaces.OCOPModelEvent;
import frodo.monitoring.interfaces.OCOPModelListener;
import frodo.monitoring.interfaces.OCOPSelectionEvent;
import frodo.utils.HelperUtilities;


/**
 * @author gushakov
 *
 */
public class DPOPMaker {
	private FileWriter fileWiter = null;

	RootTreeNode root = null;

	MeetingTreeNode[] meetings = null;

	ParticipantTreeNode[] participations = null;

	HashMap variables = null;

	OCOPBasicProblem ocop = null;

	OCOPModelListener viewer;

	public static final int SCHEDULER = 0;

	public static final int VIEWER = 1;

	/**
	 * Constructor takes the root of the meeting tree.
	 */
	public DPOPMaker(RootTreeNode root) {
		this.root = root;
		update(root);
	}

	public OCOPBasicProblem getOCOP() {
		return ocop;
	}

	public void update(RootTreeNode newRoot) {
		if (newRoot != null) {
			makeMeetingsAndParticipations(newRoot);
			variables = mapParticipationsToVariables();
			buildAll();
		} else {
			ocop = new OCOPBasicProblem();
		}

		// Clear the current heuristics pointers.
		Heuristics.CURRENT_DFS_ROOT_ID = 0;

		// Notify the viewer of the changes to the model.
		if (viewer != null) {
			viewer.ocopModelChanged(new OCOPModelEvent(OCOPModelEvent.SET_NEW_PROBLEM, ocop));
		}
	}

	/**
	 * Returns the number of variables which is equal to the total number of
	 * participations.
	 */
	public int getNumOfVars() {
		return participations.length;
	}

	/**
	 * Fills the vectors of meetings and participations.
	 */
	private void makeMeetingsAndParticipations(RootTreeNode newRoot) {
		root = newRoot;
		meetings = new MeetingTreeNode[root.getChildCount()];
		participations = new ParticipantTreeNode[root.getParticipationsCount()];

		Enumeration children = root.children();
		int i = 0;
		int j = 0;
		while (children.hasMoreElements()) {
			MeetingTreeNode meeting = (MeetingTreeNode) children.nextElement();
			meetings[i] = meeting;
			Enumeration e = meeting.children();
			while (e.hasMoreElements()) {
				ParticipantTreeNode participant = (ParticipantTreeNode) e.nextElement();
				participations[j] = participant;
				j++;
			}
			i++;
		}
	}

	private HashMap mapParticipationsToVariables() {
		//Reset the total number of variables.
		HashMap map = new HashMap();
		for (int i = 0; i < participations.length; i++) {
			map.put(getMapKey(participations[i]), makeCSPVariable(participations[i], i));
		}
		return map;
	}

	private String getMapKey(ParticipantTreeNode p) {
		MeetingTreeNode m = (MeetingTreeNode) p.getParent();
		return p.getLabel() + "#" + m.getLabel();
	}

	/**
	 * Returns a CSPVariable representing this participation.
	 * 
	 * @param participant
	 *            A node in the tree of meetings and participations.
	 */
	private CSPVariable makeCSPVariable(ParticipantTreeNode participant, int id) {
		CSPVariable var = null;
		String pLabel = participant.getLabel();
		String mLabel = ((MeetingTreeNode) participant.getParent()).getLabel();

		VariableId varId = new VariableId(id, 0);

		//Add an identifier label for this variable
		varId.setLabel(pLabel + "/" + mLabel);

		/*
		 * Get the domain of the variable, which is the time slots in the time
		 * grid of the meeting of this participant.
		 */
		MeetingTreeNode meeting = (MeetingTreeNode) participant.getParent();
		Vector domain = meeting.getGrid().getTimeSlots();

		var = new CSPVariable(varId, domain);

		return var;
	}

	/**
	 * Constructs a unary valued constraint with the utility values taken from
	 * the scheduling preferences of the specified participant.
	 */
	private ValuedConstraint makeUnaryConstraint(ParticipantTreeNode participant) {
		CSPVariable var = (CSPVariable) variables.get(getMapKey(participant));
		Vector domain = var.getDomain();
		ValuedConstraint unary = new ValuedConstraint(var);
		int[][] valuations = new int[domain.size()][1];
		for (int i = 0; i < domain.size(); i++) {
			Integer timeSlot = (Integer) domain.get(i);
			valuations[i][0] = ((Integer) participant.getSchedule().get(timeSlot)).intValue();
		}
		unary.setValuations(valuations);
		return unary;
	}

	/**
	 * Constructs all of the binary (equality) constraints between the pairs of
	 * participants in the meeting. Note, there are several ways to connect
	 * the participants of the same meeting: "to chain" them with the equality
	 * constraints or to connect them in a "bush" formation, in the latter case
	 * it also matters which participant is at the head of "the bush".
	 * 
	 * @param meeting
	 *            A MeetingTreeNode representing a meeting.
	 */
	private Vector makeSameMeetingConstraints(MeetingTreeNode meeting) {
		Vector constraints = new Vector();
		int numOfparticipants = meeting.getChildCount();
		for (int i = 0; i < numOfparticipants; i++) {
			ValuedConstraint vc = null;
			ParticipantTreeNode pFrom = (ParticipantTreeNode) meeting.getChildAt(i);
			if (Heuristics.CURRENT_EQUALITY_CONNECTION == Heuristics.CONNECT_TO_FIRST) {
				vc = makeOneSameMeetingConstraint(pFrom, meeting, 0);
			} else if (Heuristics.CURRENT_EQUALITY_CONNECTION == Heuristics.CHAIN) {
				vc = makeOneSameMeetingConstraint(pFrom, meeting, (i == 0) ? 0 : i - 1);
			}
			if (vc != null) {
				constraints.add(vc);
			}
		}
		return constraints;
	}

	private int[][] makeSameMeetingValuations(Vector domFrom, Vector domTo, String nameFrom, String nameTo, MeetingTreeNode meeting) {

		int numFrom = domFrom.size();
		int numTo = domTo.size();

		int[][] valuations = new int[numFrom][numTo];
		for (int i = 0; i < numFrom; i++) {
			for (int j = 0; j < numTo; j++) {
				if (domFrom.elementAt(i).equals(domTo.elementAt(j))) {
					valuations[i][j] = RootTreeNode.DEFAULT_POSITIVE_UTILITY;
				} else {
					valuations[i][j] = RootTreeNode.DEFAULT_NEGATIVE_UTILITY;
				}
			}
		}
		return valuations;
	}

	private ValuedConstraint[] toArray(Vector constraints) {
		ValuedConstraint[] arrayConstraints = new ValuedConstraint[constraints.size()];
		for (int i = 0; i < constraints.size(); i++) {
			ValuedConstraint next = (ValuedConstraint) constraints.elementAt(i);
			arrayConstraints[i] = next;
		}

		return arrayConstraints;
	}

	/**
	 * Construct binary (inequality) constraints between any two attendances
	 * of the same particiapant in a different meetings.
	 */
	private Vector makeSameParticipantConstraints() {
		Vector constraints = new Vector();

		for (int i = 0; i < participations.length - 1; i++) {
			for (int j = i + 1; j < participations.length; j++) {
				if (participations[i].getLabel().equals(participations[j].getLabel())) {
					/*
					 * For each pair of meetings with the same participant,
					 * create a binary, inequality constraint, because at one
					 * time slot only one meeting can take place.
					 */
					CSPVariable varFrom = (CSPVariable) variables.get(getMapKey(participations[i]));
					CSPVariable varTo = (CSPVariable) variables.get(getMapKey(participations[j]));
					MeetingTreeNode mFrom = (MeetingTreeNode) participations[i].getParent();
					MeetingTreeNode mTo = (MeetingTreeNode) participations[j].getParent();
					int[][] valuations = makeSameParticipantValuations(varFrom.getDomain(), varTo.getDomain(), participations[i].getLabel(), mFrom, mTo);
					ValuedConstraint vc = new ValuedConstraint(varFrom, varTo);
					vc.setValuations(valuations);

					// This is an inequality constraint.
					vc.setType('#');
					constraints.add(vc);
				}
			}

		}

		return constraints;
	}

	private int[][] makeSameParticipantValuations(Vector domFrom, Vector domTo, String pName, MeetingTreeNode mFrom, MeetingTreeNode mTo) {

		int numFrom = domFrom.size();
		int numTo = domTo.size();

		int[][] valuations = new int[numFrom][numTo];
		for (int i = 0; i < numFrom; i++) {
			for (int j = 0; j < numTo; j++) {
				if (domFrom.elementAt(i).equals(domTo.elementAt(j))) {
					valuations[i][j] = RootTreeNode.DEFAULT_NEGATIVE_UTILITY;
				} else {
					valuations[i][j] = RootTreeNode.DEFAULT_POSITIVE_UTILITY;
				}
			}
		}
		return valuations;
	}

	/**
	 * Constructs a centralized version of the csp problem corresponding to the
	 * meetings tree registered with this object.
	 * 
	 * @return An instance of the OCOPProblem.
	 */
	public void buildAll() {
		ocop = new OCOPBasicProblem();
		int numOfVars = getNumOfVars();
		// Make problem nodes;
		for (int i = 0; i < numOfVars; i++) {
			CSPVariable var = (CSPVariable) variables.get(getMapKey(participations[i]));
			DPOPNode node = new DPOPNode(null, var.getId().getAgentID(), null);
			ocop.addNode(node);
			node.addVariable(var);
			node.setDimension(new Dimension(node.getId(), var.getDomainSize()));
			node.setLabel('X');
			ocop.addVariable(var);
		}
		// Make constraints
		ValuedConstraint[] constraints = makeAllConstraints();
		for (int i = 0; i < constraints.length; i++) {
			ValuedConstraint vc = constraints[i];
			setOneConstraint(vc, ocop);
		}
	}

	private void setOneConstraint(ValuedConstraint vc, OCOPAbstractProblem problem) {
		//TODO: set owner agent?
		CSPVariable varFrom = (CSPVariable) vc.getFrom();
		CSPVariable varTo = (CSPVariable) vc.getTo();
		VariableId fromId = varFrom.getId();
		VariableId toId = varTo.getId();

		varFrom.addConstraint(vc);
		vc.setId(problem.getConstraints().size());
		problem.addConstraint(vc);
		problem.getNodeById(fromId.getAgentID()).addConstraint(vc);

		if (fromId.equals(toId)) {

		} else {
			//this is a binary constraint
			//building the mirror constraint, to add it to the destination
			// variable
			ValuedConstraint constrTo = new ValuedConstraint(varTo, varFrom);
			//TODO: set owner agent?
			constrTo.setValuations(HelperUtilities.intArrayTranspose(vc.getValuations()));
			varTo.addConstraint(constrTo);
			OCOPAbstractNode fromNode = problem.getNodeById(fromId.getAgentID());
			//add toNode as neighbor to fromNode
			if (fromNode.getNeighborById(toId.getAgentID()) == null) {
				fromNode.addNeighbour(problem.getNodeById(toId.getAgentID()));
			}
			OCOPAbstractNode toNode = problem.getNodeById(toId.getAgentID());
			//add toNode as neighbor to fromNode
			toNode.addConstraint(constrTo);
			if (toNode.getNeighborById(fromId.getAgentID()) == null) {
				toNode.addNeighbour(problem.getNodeById(fromId.getAgentID()));
			}
		}

	}

	private int encodePair(int x, int y) {
		return (int) (x * x + 2 * x * y + y * y + 3 * x + y) / 2;
	}

	private ValuedConstraint[] makeAllConstraints() {
		Vector constraints = new Vector();
		// Unary
		for (int i = 0; i < participations.length; i++) {
			ValuedConstraint vc = makeUnaryConstraint(participations[i]);
			constraints.add(vc);
		}
		// Constraints, binary, same meeting attendants
		for (int i = 0; i < meetings.length; i++) {
			Vector vcs = makeSameMeetingConstraints(meetings[i]);
			for (int j = 0; j < vcs.size(); j++) {
				constraints.add(vcs.elementAt(j));
			}
		}
		//Constraints, binary, same participant
		if (meetings.length >= 2) {
			Vector vcs = makeSameParticipantConstraints();
			for (int i = 0; i < vcs.size(); i++) {
				constraints.add(vcs.elementAt(i));
			}
		}
		return toArray(constraints);
	}

	public void writeOCOPToFile(String fileName) {
		try {
			PrintWriter fw = new PrintWriter(new FileOutputStream(fileName));
			fw.print(ocop.toStringSerialize());
			fw.flush();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Returns the value assigned to the variable
	 * represented by the first participant of the specified
	 * meeting.
	 */
	public Integer getMeetingSolution(MeetingTreeNode meeting) {
		ParticipantTreeNode first = (ParticipantTreeNode) meeting.getChildAt(0);
		CSPVariable variable = (CSPVariable) variables.get(getMapKey(first));
		Integer value = variable.getAssignment().getValue();
		return value;
	}

	/*
	 * Register an OCOP model listener.
	 */
	public void addOCOPModelListener(OCOPModelListener listener, int type) {
	}

	public void displayOCOP() {
		OCOPModelEvent event = new OCOPModelEvent(OCOPModelEvent.SHOW_PSEUDO_TREE, ocop);
		if (viewer != null) {
			viewer.ocopModelChanged(event);
		} else {
			viewer = (OCOPModelListener) new PseudoTreeEditor(ocop);
		}
	}

	//------------------------------------------------------------------------------------------------------------
	/*
	 * The methods below are for addition of new nodes/constraints without
	 * rebuilding the OCOP problem.
	 */

	public void addNewParticipation(ParticipantTreeNode newParticipation, MeetingTreeNode meeting, RootTreeNode newRoot) {

		if (ocop == null) {
			buildAll();
		}

		//Update helper arrays, meetings and participations.
		//appendOneToMeetingsHelper(meeting);
		//appendOneToParticipationsHelper(newParticipation);
		makeMeetingsAndParticipations(newRoot);

		//Make new variable.
		CSPVariable var = makeCSPVariable(newParticipation, getNumOfVars() - 1);
		variables.put(getMapKey(newParticipation), var);

		//Build and register a new node with the problem.
		DPOPNode node = new DPOPNode(null, var.getId().getAgentID(), null);
		ocop.addNode(node);
		node.addVariable(var);
		node.setDimension(new Dimension(node.getId(), var.getDomainSize()));
		node.setLabel('X');
		ocop.addVariable(var);

		//Build and register the new constraints.
		Vector vcs = makeSameParticipantConstraintsForOne(newParticipation);
		ValuedConstraint vc = makeUnaryConstraint(newParticipation);
		vcs.add(vc);
		/*
		 * Connect this participant with an equality constraint
		 * to one other participant in the same meeting according to
		 * the heuristic used.
		 */
		if (Heuristics.CURRENT_EQUALITY_CONNECTION == Heuristics.CONNECT_TO_FIRST) {
			vc = makeOneSameMeetingConstraint(newParticipation, meeting, 0);

		} else if (Heuristics.CURRENT_EQUALITY_CONNECTION == Heuristics.CHAIN) {
			vc = makeOneSameMeetingConstraint(newParticipation, meeting, meeting.getChildCount() - 2);
		}
		if (vc != null) {
			vcs.add(vc);
		}
		for (int i = 0; i < vcs.size(); i++) {
			setOneConstraint((ValuedConstraint) vcs.elementAt(i), ocop);
		}

		if (viewer != null) {
			//Dispatch the model change event to the viewer
			viewer.ocopModelChanged(new OCOPModelEvent(OCOPModelEvent.NEW_NODE_ADDED, ocop));
		}
	}

	private void appendOneToMeetingsHelper(MeetingTreeNode last) {
		MeetingTreeNode[] newMeetings = new MeetingTreeNode[meetings.length + 1];
		for (int i = 0; i < meetings.length; i++) {
			newMeetings[i] = meetings[i];
		}
		newMeetings[meetings.length] = last;
		meetings = newMeetings;
	}

	private void appendOneToParticipationsHelper(ParticipantTreeNode last) {
		ParticipantTreeNode[] newParticipations = new ParticipantTreeNode[participations.length + 1];
		for (int i = 0; i < participations.length; i++) {
			newParticipations[i] = participations[i];
		}
		newParticipations[participations.length] = last;
		participations = newParticipations;
	}

	private Vector makeSameParticipantConstraintsForOne(ParticipantTreeNode participant) {
		Vector constraints = new Vector();

		for (int i = 0; i < participations.length; i++) {
			if (participations[i].getLabel().equals(participant.getLabel()) && !(participations[i].equals(participant))) {
				/*
				 * For each pair of meetings with the same participant, create a
				 * binary, inequality constraint, because at one time slot only
				 * one meeting can take place. Do not make a constraint with the
				 * (identical) participation in the same meeting.
				 */
				CSPVariable varFrom = (CSPVariable) variables.get(getMapKey(participations[i]));
				CSPVariable varTo = (CSPVariable) variables.get(getMapKey(participant));
				MeetingTreeNode mFrom = (MeetingTreeNode) participations[i].getParent();
				MeetingTreeNode mTo = (MeetingTreeNode) participant.getParent();
				int[][] valuations = makeSameParticipantValuations(varFrom.getDomain(), varTo.getDomain(), participations[i].getLabel(), mFrom, mTo);
				ValuedConstraint vc = new ValuedConstraint(varFrom, varTo);
				vc.setValuations(valuations);

				// This is an inequality constraint.
				vc.setType('#');
				constraints.add(vc);
			}
		}

		return constraints;

	}

	/**
	 * Returns one equality constraint, which relates the specified participant
	 * to one other participant of the specified meeting.
	 * If this is the first participant of the meeting, do not make any constraints.
	 * @param participant new participant of the meeting
	 * @param meeting
	 * @param index the index of the participation to which the new participant 
	 * 			will be related by the constraint.
	 * @return a valued equality constraint
	 */
	private ValuedConstraint makeOneSameMeetingConstraint(ParticipantTreeNode participant, MeetingTreeNode meeting, int index) {
		ValuedConstraint vc = null;
		if (meeting.getChildCount() > 1) {
			ParticipantTreeNode pFrom = participant;
			ParticipantTreeNode pTo = (ParticipantTreeNode) meeting.getChildAt(index);
			if (!pFrom.equals(pTo)) {
				CSPVariable varFrom = (CSPVariable) variables.get(getMapKey(pFrom));
				CSPVariable varTo = (CSPVariable) variables.get(getMapKey(pTo));
				int[][] valuations = makeSameMeetingValuations(varFrom.getDomain(), varTo.getDomain(), pFrom.getLabel(), pTo.getLabel(), meeting);
				vc = new ValuedConstraint(varFrom, varTo);
				vc.setValuations(valuations);

				// This is an equality constraint.
				vc.setType('=');
			}
		}
		return vc;
	}

	public void changeUnaryConstraint(ParticipantTreeNode participation, Integer timeSlot) {
		int value = ((Integer) participation.getPreferenceForTimeSlot(timeSlot)).intValue();
		CSPVariable var = (CSPVariable) variables.get(getMapKey(participation));
		int varId = var.getId().getAgentID();
		//TODO: see if this makes sense.
		DPOPNode node = (DPOPNode) ocop.getNodeById(varId);
		//We only have one unary constraint for each node.
		ValuedConstraint vc = (ValuedConstraint) node.getUnaryConstraints().elementAt(0);
		vc.setValuation(timeSlot, value);
	}

	//	------------------------------------------------------------------------------------------------------------

	public void clearAllHighlights() {
		if (viewer != null) {
			viewer.highlightChanged(new OCOPSelectionEvent(OCOPSelectionEvent.SELECTION_CLEARED, null));
		}
	}

	public void highlightAllAttendants(MeetingTreeNode meeting) {
		int[] ids = new int[meeting.getChildCount()];
		Enumeration enum = meeting.children();
		int i = 0;
		while (enum.hasMoreElements()) {
			ParticipantTreeNode participant = (ParticipantTreeNode) enum.nextElement();
			CSPVariable var = (CSPVariable) variables.get(getMapKey(participant));
			ids[i] = var.getId().getAgentID();
			i++;
		}
		if (viewer != null) {
			viewer.highlightChanged(new OCOPSelectionEvent(OCOPSelectionEvent.NEW_SELECTION_MEETING, ids));
		}
	}

	public void highlightAllParticipations(ParticipantTreeNode participant) {
		int[] ids = null;
		Vector idsVec = new Vector();
		for (int i = 0; i < participations.length; i++) {
			if (participations[i].getLabel().equals(participant.getLabel())) {
				CSPVariable var = (CSPVariable) variables.get(getMapKey(participations[i]));
				idsVec.add(new Integer(var.getId().getAgentID()));
			}
		}
		ids = new int[idsVec.size()];
		for (int i = 0; i < ids.length; i++) {
			ids[i] = ((Integer) idsVec.elementAt(i)).intValue();
		}
		if (viewer != null) {
			viewer.highlightChanged(new OCOPSelectionEvent(OCOPSelectionEvent.NEW_SELECTION_PARTICIPANT, ids));
		}
	}
	/*
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 */
}
