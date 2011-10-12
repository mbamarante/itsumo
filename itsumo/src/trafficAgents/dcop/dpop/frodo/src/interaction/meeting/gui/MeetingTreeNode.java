/*
 * Created on 17 mai 2005
 *
 */
package frodo.interaction.meeting.gui;

import java.io.BufferedReader;
import java.util.Enumeration;
import java.util.Random;
import java.util.Vector;

import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.MutableTreeNode;

import frodo.interaction.meeting.TimeGrid;


/**
 * @author George
 *
 */
public class MeetingTreeNode extends DefaultMutableTreeNode implements LabeledTreeNode {
	private String label = null;

	private TimeGrid grid = null;

	private int solutionIndex = -1;

	public MeetingTreeNode(String label) {
		super();
		this.label = label;
		grid = new TimeGrid();
	}

	public MeetingTreeNode(String label, TimeGrid tg) {
		super();
		this.label = label;
		grid = tg;
	}

	public MeetingTreeNode(String label, Vector participants) {
		super();
		this.label = label;
		for (int i = 0; i < participants.size(); i++) {
			MutableTreeNode child = (MutableTreeNode) participants.elementAt(i);
			this.add(child);
		}
	}

	public static MeetingTreeNode fromFile(BufferedReader reader) {
		MeetingTreeNode meeting = null;
		try {
			String line = reader.readLine();
			String[] parsedLine = line.split(",|\\(|\\)");
			String label = parsedLine[0].trim();
			int begin = Integer.parseInt(parsedLine[1].trim());
			int end = Integer.parseInt(parsedLine[2].trim());
			TimeGrid timeGrid = new TimeGrid(begin, end);
			meeting = new MeetingTreeNode(label, timeGrid);
			int numOfParticipants = Integer.parseInt(parsedLine[3].trim());
			for (int i = 0; i < numOfParticipants; i++) {
				ParticipantTreeNode participant = ParticipantTreeNode.fromString(reader.readLine(), timeGrid);
				if (meeting.canAddParticipant(participant.getLabel())) {
					meeting.add(participant);
				} else {
					System.err.println("error: can't add participant " + participant.getLabel() + " to the meeting " + meeting.getLabel());
				}

			}

		} catch (Exception e) {
			e.printStackTrace();
		}
		return meeting;
	}

	public String toString() {
		return label;
	}

	public TimeGrid getGrid() {
		return grid;
	}

	public String getLabel() {
		return label;
	}

	/**
	 * Returns true if this meeting contains a participant.
	 * @param p
	 * @return
	 */
	public boolean hasParticipant(ParticipantTreeNode p) {

		boolean found = false;
		Enumeration children = children();
		while (children.hasMoreElements() && !found) {
			if (((ParticipantTreeNode) children.nextElement()).getLabel().equals(p.getLabel())) {
				found = true;
			}
		}
		return found;
	}

	public ParticipantTreeNode getParticipant(String name) {
		ParticipantTreeNode participant = null;
		boolean found = false;
		Enumeration children = children();
		while (children.hasMoreElements() && !found) {
			ParticipantTreeNode child = (ParticipantTreeNode) children.nextElement();
			if (child.getLabel().equals(name)) {
				found = true;
				participant = child;
			}
		}
		return participant;
	}

	public ParticipantTreeNode[] getParticipants() {
		ParticipantTreeNode[] ps = new ParticipantTreeNode[getChildCount()];
		for (int i = 0; i < ps.length; i++) {
			ps[i] = (ParticipantTreeNode) getChildAt(i);
		}
		return ps;
	}

	public Comparable getRandomSolution() {
		int index = new Random().nextInt(grid.getNumberOfTimeSlots());
		return grid.getSlotAt(index);
	}

	public boolean canAddParticipant(String participantLabel) {
		boolean flag = true;
		Enumeration enum = children();
		while (flag && enum.hasMoreElements()) {
			if (((ParticipantTreeNode) enum.nextElement()).getLabel().equals(participantLabel)) {
				flag = false;
			}
		}
		return flag;
	}

	public String toStringSerialized() {
		String out = "";
		int numOfparticipants = getChildCount();
		out += getLabel() + "\t(" + getGrid().getStartSlot() + "," + getGrid().getEndSlot() + ")\t" + numOfparticipants + "\n";
		for (int i = 0; i < numOfparticipants; i++) {
			ParticipantTreeNode participant = (ParticipantTreeNode) getChildAt(i);
			out += participant.toStringMeetingView(getGrid()) + (i < numOfparticipants - 1 ? "\n" : "");
		}
		return out;
	}

}
