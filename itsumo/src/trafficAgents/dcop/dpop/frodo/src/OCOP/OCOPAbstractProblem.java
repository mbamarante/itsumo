/*
 * Created on Jul 21, 2003
 *
 */

package frodo.OCOP;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Vector;

import frodo.COP.OCOPProblemFactory;
import frodo.COP.ValuedConstraint;
import frodo.CSP.Assignment;
import frodo.CSP.BinaryConstraint;
import frodo.CSP.CSPVariable;
import frodo.CSP.ComplexValue;
import frodo.CSP.VariableId;
import frodo.utils.Logger;

/**
 * @author apetcu To change the template for this generated type comment go to Window>Preferences>Java>Code Generation>Code and Comments
 */
public abstract class OCOPAbstractProblem implements OCOPProblemInterface, Cloneable {

	protected Vector nodes;

	protected Vector variables;

	protected Vector constraints;

	//	protected double lowerBound;

	protected Logger logger;

	protected String name;

	protected boolean multipleVarsPerNode;

	//-----------------------------------------------------------------------------------------------------------------//
	public OCOPAbstractProblem() {
		variables = new Vector();
		constraints = new Vector();
		nodes = new Vector();
		//		lowerBound = Integer.MAX_VALUE;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------//
	//	public static abstract OCOPAbstractProblem loadFromTextFile(String fileName);
	//------------------------------------------------------------------------------------------------------------------------------------//
	public static OCOPAbstractProblem loadFromFile(String fileName) {
		OCOPAbstractProblem prob = null;
		try {
			FileInputStream fis = new FileInputStream(fileName);
			ObjectInputStream oos = new ObjectInputStream(fis);
			prob = (OCOPAbstractProblem) oos.readObject();
			fis.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return prob;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------//
	//-----VARIABLES--------------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------//
	public CSPVariable findVariable(VariableId vid) {
		int cnt = 0;
		while (cnt < variables.size()) {
			CSPVariable curVar = (CSPVariable) variables.elementAt(cnt);
			if (curVar.getId().equals(vid))
				return curVar;
			cnt++;
		}
		return null;
	}

	//---------------------------------------------------------------------------------//
	public void addVariable(Object var) {
		variables.add(var);
	}

	//----------------------------------------------------------------------------------------------------//
	public boolean deleteVariable(Object v) {
		CSPVariable var = (CSPVariable) v;
		System.out.println("Deleting variable" + var.getId());
		int i = 0;
		while (i < constraints.size()) {
			BinaryConstraint bc = getConstraint(i);
			CSPVariable toVar = bc.getTo();
			CSPVariable fromVar = bc.getFrom();

			if (fromVar.getId().equals(var.getId())) {
				//adica daca trebuie sa sterg fromVar, caut constrainul corespunzator in toVar
				BinaryConstraint toDel = toVar.getConstraint(toVar.findConstraint(fromVar.getId()));
				toVar.deleteConstraint(toDel);
				constraints.removeElementAt(i);
			} else {
				if (toVar.getId().equals(var.getId())) {
					//adica daca trebuie sa sterg toVar, caut constrainul corespunzator in fromVar
					BinaryConstraint toDel = fromVar.getConstraint(fromVar.findConstraint(toVar.getId()));
					fromVar.deleteConstraint(toDel);
					constraints.removeElementAt(i);
				} else
					i++;
			}
		}
		return variables.removeElement(v);
	}

	//----------------------------------------------------------------------------------------------------//
	public int getNumberOfVariables() {
		return variables.size();
	}

	//----------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------//
	public String currentAssignmentsAsString() {
		return getCurrentAssignments().toString();
	}

	//----------------------------------------------------------------------------------------------------//
	//----constraints-------------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------//
	public void addConstraint(Object bc) {
		constraints.add(bc);
	}

	public BinaryConstraint getConstraint(int id) {
		return (BinaryConstraint) constraints.elementAt(id);
	}

	public boolean deleteConstraint(Object bc) {
		return constraints.removeElement(bc);
	}

	public Vector getConstraints() {
		return constraints;
	}

	public void setConstraints(Vector constraints) {
		this.constraints = constraints;
	}

	public int getNumberOfConstraints() {
		return constraints.size();
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	public ComplexValue getCurrentAssignments() {

		ComplexValue assig = new ComplexValue(new Vector());
		for (int i = 0; i < nodes.size(); i++) {
			assig.addValue(getNodeByIndex(i).getVariable().getAssignment());
		}
		return assig;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	public void setCurrentAssignments(ComplexValue toSet) {
		for (int i = 0; i < toSet.size(); i++) {
			Assignment assig = toSet.getITHValue(i);
			getVariable(assig.getVariableId()).setValue(assig.getValue());
			//            assig.addValue(getVariable(i).getAssignment());
		}
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//	public double getLowerBound() {
	//		return lowerBound;
	//	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	public void addNode(Object node) {
		nodes.add(node);
	}

	public OCOPAbstractNode getNodeByIndex(int index) {
		OCOPAbstractNode node = (OCOPAbstractNode) nodes.elementAt(index);
		return node;
	}

	public OCOPAbstractNode getNodeById(int id) {
		OCOPAbstractNode nodeToReturn = null;
		for (int i = 0; i < nodes.size(); i++) {
			OCOPAbstractNode node = (OCOPAbstractNode) nodes.elementAt(i);
			if (node.getId() == id)
				nodeToReturn = node;
		}
		return nodeToReturn;
	}

	public Vector getNodes() {
		return nodes;
	}

	public boolean deleteNode(Object n) {
		OCOPAbstractNode nd = (OCOPAbstractNode) n;
		return deleteNodeByID(nd.getId());
	}

	public boolean deleteNodeByID(int todel) {
		System.out.println("Deleting node " + todel);
		OCOPAbstractNode nd = (OCOPAbstractNode) getNodeById(todel);
		//		OCOPNode nd = (OCOPNode) n;
		for (int i = 0; i < nd.getNumberOfVariables(); i++) {
			CSPVariable var = nd.getVariable(i);
			deleteVariable(var);
		}
		for (int i = 0; i < nd.getNumberOfNeighbours(); i++) {
			OCOPAbstractNode ngh1 = nd.getNeighborByIndex(i);
			OCOPAbstractNode ngh2 = getNodeById(ngh1.getId());
			ngh2.deleteNeighbourById(todel);
		}
		return nodes.removeElement(nd);
	}

	public void setNodes(Vector nodes) {
		this.nodes = nodes;
	}

	public int getNumberOfNodes() {
		return nodes.size();
	}

	//	------------------------------------------------------------------------------------------------------------------------------------//
	//	------------------------------------------------------------------------------------------------------------------------------------//
	public double getTotalEvaluation() {
		double totalUtility = 0;
		for (int i = 0; i < nodes.size(); i++) {//for all nodes
			OCOPAbstractNode curn = getNodeByIndex(i);
			//			System.out.println("Unary for X" + curn.getId());
			totalUtility += curn.getEvaluationUnaryConstraints();
			for (int j = 0; j < curn.getNumberOfConstraints(); j++) {//for all the constraints
				ValuedConstraint vc = curn.getConstraint(j);
				if (vc.getArity() > 1) {
					//					OCOPAbstractNode ngh = curn.getNeighbourByIndex(j);
					if (vc.getSourceId().compareTo(vc.getDestinationId()) > 0) {//this is to avoid counting the same constraint twice.
						totalUtility += vc.getValuation();
					}
				}
			}
		}
		return totalUtility;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	public CSPVariable getVariable(int i) {
		return (CSPVariable) variables.elementAt(i);
	}

	//	------------------------------------------------------------------------------------------------------------------------------------//
	//	------------------------------------------------------------------------------------------------------------------------------------//
	public CSPVariable getVariable(VariableId vid) {
		OCOPAbstractNode nd = getNodeById(vid.getAgentID());
		return nd.getVariable(vid.getLocalID());
		//		return (CSPVariable) variables.elementAt(i);
	}

	//	------------------------------------------------------------------------------------------------------------------------------------//
	//	------------------------------------------------------------------------------------------------------------------------------------//
	public Vector getVariables() {
		return variables;
	}

	//	------------------------------------------------------------------------------------------------------------------------------------//
	public boolean saveToFile(String fileName) {
		try {
			FileOutputStream fos = new FileOutputStream(fileName);
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(this);
			fos.close();
		} catch (Exception e) {
			System.out.println("Exception while saving problem to file:");
			e.printStackTrace();
			return false;
		}
		return true;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	public boolean saveToTextFile(String fileName) {
		try {
			FileWriter fw = new FileWriter(fileName);
			fw.write(toStringSerialize());
			fw.close();
		} catch (Exception e) {
			System.out.println("Exception while saving problem to file:");
			e.printStackTrace();
			return false;
		}
		return true;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	public void setVariables(Vector vector) {
		variables = vector;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	public String toString() {
		String line = variables.size() + " variables and " + constraints.size() + " constraints.\n";
		for (int i = 0; i < variables.size(); i++) {
			line += ((CSPVariable) variables.elementAt(i)).toStringSerialize() + "\n";
		}
		return line;
	}

	//	------------------------------------------------------------------------------------------------------------------------------------//
	public String toStringNodes() {
		String line = nodes.size() + " nodes in the problem\n";
		for (int i = 0; i < nodes.size(); i++) {
			line += getNodeByIndex(i).toString() + "\n";
		}
		return line;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	public String toStringSerialize() {
		//this is supposed to be vars / agents. In case there are multiple vars/agent, they are different
		StringBuffer line = new StringBuffer(variables.size() + "\t" + getNumberOfNodes() + "\n");

		for (int i = 0; i < getNumberOfNodes(); i++) {
			String sss = getNodeByIndex(i).toStringSerialize();
			if (sss.length() > 0)
				line.append(sss + "\n");
		}
		/*
		 * for (int i = 0; i < variables.size(); i++) { line += ((CSPVariable) variables.elementAt(i)).toStringWithConstraints() + "\n"; }
		 */
		int constrSz = getNumberOfConstraints();
		for (int i = 0; i < constrSz; i++) {
			line.append(getConstraint(i).toStringSerialize() + "\n");
			//			System.out.println("Type: " + getConstraint(i).getType());
		}
		return line.toString();
	}

	//------------------------------------------------------------------------------------------------------------------------------------//

	public Logger getLogger() {
		return logger;
	}

	public void setLogger(Logger logger) {
		this.logger = logger;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	public int getMaximalDomainSize() {
		int s = 0;
		for (int i = 0; i < getVariables().size(); i++) {
			CSPVariable ocv = getVariable(i);
			if (s < ocv.getDomainSize())
				s = ocv.getDomainSize();
		}
		return s;
	}

	//------------------------------------------------------------------------------------------------------------------------------------//
	public boolean isConnected() {
		boolean rez = false;
		boolean visited[] = new boolean[getVariables().size()];
		visited[0] = true;
		int visitcnt = 1;
		boolean togo = true;
		while (visitcnt < getVariables().size() && togo) {
			togo = false;
			for (int i = 0; i < getVariables().size(); i++) {
				if (visited[i]) {
					OCOPAbstractNode oan = getNodeByIndex(i);
					for (int j = 0; j < oan.getNumberOfNeighbours(); j++) {
						OCOPAbstractNode ngh = oan.getNeighborByIndex(j);
						CSPVariable oavngh = ngh.getVariable();
						if (!visited[oavngh.getId().getAgentID()]) {
							visited[oavngh.getId().getAgentID()] = true;
							togo = true;
							visitcnt++;
						}
					}
				} else {
				}
			}
		}
		if (visitcnt == getVariables().size())
			rez = true;
		else {
			System.out.println("The problem is disconnected. Not reached nodes: ");
			for (int i = 0; i < getVariables().size(); i++) {
				if (!visited[i])
					System.out.println(i + " ");
			}
		}
		return rez;
	}

	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------//
	public boolean hasMultipleVarsPerNode() {
		return multipleVarsPerNode;
	}

	public void setMultipleVarsPerNode(boolean multipleVarsPerNode) {
		this.multipleVarsPerNode = multipleVarsPerNode;
	}

	//-------------------------------------------------------------------------------------------------------------//
	public Object clone() {
		return OCOPProblemFactory.loadFromString(toStringSerialize(), getNodeByIndex(0));
	}
}
//------------------------------------------------------------------------------------------------------------------------------------//
