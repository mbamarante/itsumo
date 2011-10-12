package frodo.dbreakout.CSP;

import java.util.Vector;

import frodo.CSP.CSPNodeInterface;
import frodo.CSP.CSPVariable;
import frodo.CSP.ComplexValue;
import frodo.CSP.ComplexVariable;
import frodo.utils.Logger;


/**
 * @author apetcu
 * 
 * To change this generated comment edit the template variable "typecomment": Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */
public class CSPNodeDB implements CSPNodeInterface {

	private Logger myLogger;

	private int id;

	Vector variables;

	Vector constraints;

	Vector neighbours;

	Vector improvements;

	Vector violatedConstraints;

	String name;

	//    int myImprove;
	int current_evaluation;

	int term_cnt;

	ComplexVariable myComplexVariable;

	//--------------------------------------------------------------------------------------------------//
	public CSPNodeDB(int id, String name, Logger myl) {
		this(id, name, myl, new Vector(), new Vector(), new Vector());
	}

	//--------------------------------------------------------------------------------------------------//
	public CSPNodeDB(int id, String name, Logger myl, Vector vars, Vector constraints, Vector neighbs) {
		this.id = id;
		this.variables = vars;
		this.name = name;
		this.constraints = constraints;
		this.neighbours = neighbs;
		this.improvements = new Vector();
		this.violatedConstraints = new Vector();
		this.myLogger = myl;
	}

	//--------------------------------------------------------------------------------------------------//
	public void setupLocalConstraints() {
		int ls = getLocalSize();
		for (int i = 0; i < ls - 1; i++) {
			for (int j = i + 1; j < ls; j++) {
				BC_ID_DifferentWithWeight bc = new BC_ID_DifferentWithWeight(getVariable(i), getVariable(j));
				this.addConstraint(bc);
			}
		}
	}

	//--------------------------------------------------------------------------------------------------//
	/*
	 * public void setupExternalConstraints(){ int lcnt = getLocalSize(); int ncnt = getNeighboursCount(); for(int i=0;i <ncnt;i++){ CSPNodeDB ngh =
	 * (CSPNodeDB)neighbours.elementAt(i); int remoteCnt = ngh.getLocalSize(); for(int j=0;j <lcnt;j++){ for(int k=0;k <remoteCnt;k++){
	 * BinaryConstraint bc = new BC_ID_Different(getVariable(j), ngh.getVariable(k)); ngh.addConstraint(bc); } } } }
	 */
	//--------------------------------------------------------------------------------------------------//
	public void setupExternalConstraints(int localVar, int extNode, int extVar) {
		int ncnt = getNumberOfNeighbours();
		boolean found = false;
		int i = 0;
		while (i < ncnt && !found) {
			CSPNodeDB ngh = (CSPNodeDB) neighbours.elementAt(i);
			if (ngh.getId() == extNode) {
				found = true;
				BC_ID_DifferentWithWeight bc = new BC_ID_DifferentWithWeight(getVariable(localVar), ngh.getVariable(extVar));
				ngh.addConstraint(bc);
			}
			i++;
		}
		if (!found)
			myLogger.println("EXTERNAL NODE NOT FOUND!!!!");
	}

	//--------------------------------------------------------------------------------------------------//
	public int setRandomInitial() {
		myLogger.println("//------------------------------// Setting \"random\" initial values...");
		for (int i = 0; i < getLocalSize(); i++) {
			CSPVariableDBA v = getVariable(i);
			v.setValue((Integer) (v.getDomain().elementAt(i)));
		}
		return this.computeCurrentEval();
	}

	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	//this doesn't actually compute the values, this happens in preprocess.. here we only put them first
	public void computeNI() {
		myLogger.println("//------------------------------// Putting NI values first...");
		for (int i = 0; i < getLocalSize(); i++) {
			CSPVariableDBA localVar = getVariable(i);
			Vector NIDomainLocal = localVar.getNIDomain(); //this was setup in preprocess
			if (NIDomainLocal != null) {
				putTheseValuesFirst(localVar.getDomain(), NIDomainLocal);
			}
		}
	}

	//--------------------------------------------------------------------------------------------------//
	public void updateVariables(Vector updates) {
		for (int i = 0; i < updates.size(); i++) {
			CSPVariableDBA tvar = (CSPVariableDBA) updates.elementAt(i);
			int nodeId = tvar.getId().getAgentID();
			int varId = tvar.getId().getLocalID();
			boolean found = false;
			int cnt = 0;
			while (!found && cnt < getNumberOfNeighbours()) {
				CSPNodeDB ngh = (CSPNodeDB) neighbours.elementAt(cnt);
				if (ngh.getId() == nodeId) {
					found = true;
					ngh.getVariable(varId).setValue(tvar.getValue());
				}
				cnt++;
			}
			if (found == false)
				myLogger.println("^^^^^^^^^^^^^^^^^^^^^^^^Problem while updating variables.");
		}
	}

	//--------------------------------------------------------------------------------------------------//
	/**
	 * Method updateNeighbourDomains.
	 * 
	 * @param doms
	 */
	public void updateNeighbourDomains(Vector doms) { //set the domains for the variables from neighbouring nodes
		myLogger.println("UPDATING DOMAIN FOR: " + doms);
		CSPVariableDBA extvar = null;
		CSPNodeDB ngh = null;
		for (int i = 0; i < doms.size(); i++) { //for each incoming domain
			extvar = (CSPVariableDBA) doms.elementAt(i);
			// get the respective variable
			int nodeId = extvar.getId().getAgentID();
			//trick: if the vector contains variables from multiple nodes---->>> trouble!!
			int varId = extvar.getId().getLocalID();
			Vector extDomain = extvar.getDomain();
			ngh = getNeighbour(nodeId);
			CSPVariableDBA externalVar = ngh.getVariable(varId);
			externalVar.setDomain(extDomain);
			//I'm setting it anyway, since I don't know yet if it's connected or not... it might be!!
			for (int j = 0; j < getLocalSize(); j++) {
				getVariable(j).updateDomainCounters(extDomain);
			}
		}
	}

	//--------------------------------------------------------------------------------------------------//
	public void preprocess() { //here I setup the external constraints, compute the neighbourhood interchangeabilities and eliminate fake neighbours
		Vector fakeNeighbours = new Vector();
		for (int cntNodeExt = 0; cntNodeExt < getNumberOfNeighbours(); cntNodeExt++) {
			CSPNodeDB nodeExt = (CSPNodeDB) neighbours.elementAt(cntNodeExt);
			myLogger.println("Preprocessing for " + nodeExt);
			boolean isNodeConnected = false;
			for (int cntVarExt = 0; cntVarExt < nodeExt.getLocalSize(); cntVarExt++) {
				CSPVariableDBA varExt = nodeExt.getVariable(cntVarExt);
				Vector domainExt = varExt.getDomain();
				for (int cntVarLocal = 0; cntVarLocal < getLocalSize(); cntVarLocal++) {
					boolean isVarConnected = false;
					//one of the external variables is connected with an internal one
					CSPVariableDBA varLocal = getVariable(cntVarLocal);
					Vector domainLocal = varLocal.getDomain();
					Vector NIDomainLocal = varLocal.getNIDomain();
					if (NIDomainLocal == null) {
						NIDomainLocal = new Vector(domainLocal);
					}
					//		    NIDomainLocal.addAll(domainLocal);//I suppose that all values for the current var are NI, and remove later the ones that are
					// not.
					for (int cntDomainExt = 0; cntDomainExt < domainExt.size(); cntDomainExt++) {
						Integer extValue = (Integer) domainExt.elementAt(cntDomainExt);
						for (int cntDomainLocal = 0; cntDomainLocal < domainLocal.size(); cntDomainLocal++) {
							Integer localValue = (Integer) domainLocal.elementAt(cntDomainLocal);
							if (extValue.equals(localValue)) {
								//                                myLogger.println(" -->Common: " + extValue );
								isNodeConnected = true;
								isVarConnected = true;
								NIDomainLocal.remove(localValue);
								//it's not NI anymore!!
							}
						}
					}
					varLocal.setNIDomain(NIDomainLocal);
					//                    myLogger.println("Variable " + varLocal + " has NIDomain=" + NIDomainLocal);
					if (isVarConnected) {
						BC_ID_DifferentWithWeight bc = new BC_ID_DifferentWithWeight(varLocal, varExt);
						nodeExt.addConstraint(bc);
					}
				}
			}
			if (!isNodeConnected) {
				myLogger.println("XXXXX Removed node " + cntNodeExt + " from neighbours.");
				fakeNeighbours.add(nodeExt);
			}
		}
		neighbours.removeAll(fakeNeighbours);
	}

	//--------------------------------------------------------------------------------------------------//
	public Vector getNPIDomain(int myVarId, Vector idsRef) {
		CSPVariableDBA myVar = this.getVariable(myVarId);
		Vector NPID = new Vector(myVar.getDomain());
		for (int i = 0; i < idsRef.size(); i++) {
			CSPVariableDBA tmpVar = (CSPVariableDBA) idsRef.elementAt(i);
			Vector tmpDom = tmpVar.getDomain();
			NPID.removeAll(tmpDom);
		}
		return NPID;
	}

	//--------------------------------------------------------------------------------------------------//
	public Vector putTheseValuesFirst(Vector origDom, Vector toBeFirst) {
		origDom.removeAll(toBeFirst);
		origDom.addAll(0, toBeFirst);
		return origDom;
	}

	//--------------------------------------------------------------------------------------------------//
	public void preprocess_NPI() {
		myLogger.println("->>>>>>>>>>>>>NPI preprocessing");
		Vector NPIReferenceSet = new Vector();
		for (int i = 0; i < this.violatedConstraints.size(); i++) {
			BC_ID_DifferentWithWeight bc = (BC_ID_DifferentWithWeight) this.violatedConstraints.elementAt(i);
			CSPVariable tmpVar = bc.getTo();
			NPIReferenceSet.add(tmpVar);
		}
		myLogger.println("->>>>>>>>>>>>>NPI preprocessing: reference set=" + NPIReferenceSet);
		for (int i = 0; i < this.getLocalSize(); i++) {
			CSPVariableDBA tmp = getVariable(i);
			Vector NPIDom = this.getNPIDomain(tmp.getId().getLocalID(), NPIReferenceSet);
			myLogger.println("->>>>>>>>>>>>>NPI preprocessing found NPI:" + NPIDom);
			this.putTheseValuesFirst(tmp.getDomain(), NPIDom);
		}
	}

	//--------------------------------------------------------------------------------------------------//
	public void computeWeightedNI() {
		myLogger.println("\nXXXXXXXXXXXXXXXXXX sorting for weak-NI XXXXXXXXXXXXXXXXXXX \n");
		for (int i = 0; i < getLocalSize(); i++) {
			//			myLogger.println("\nXXXXXXXXXXXXXXXXXX BEFORE sorted domain for " + getVariable(i) + " = " + getVariable(i).getDomain());
			//			myLogger.println("XXXXXXXXXXXXXXXXXX BEFORE sorted counter domain for " + getVariable(i) + " = " + getVariable(i).getDomainCounters());
			getVariable(i).sortDomain_Counters();
			//			myLogger.println("\nXXXXXXXXXXXXXXXXXX sorted domain for " + getVariable(i) + " = " + getVariable(i).getDomain());
			//			myLogger.println("XXXXXXXXXXXXXXXXXX sorted counter domain for " + getVariable(i) + " = " + getVariable(i).getDomainCounters());
		}
	}

	//--------------------------------------------------------------------------------------------------//
	public int evaluateLocalConstraints(int varId, Integer testValue) {
		int eval = 0;
		for (int j = 0; j < getNumberOfConstraints(); j++) { //testing all constraints
			BC_ID_DifferentWithWeight bc = (BC_ID_DifferentWithWeight) getConstraint(j);
			CSPVariable from = bc.getFrom();
			CSPVariable to = bc.getTo();
			int fromId = from.getId().getLocalID();
			int toId = to.getId().getLocalID();
			if (fromId == varId) {
				if ((to.getValue() != null) && !bc.satisfies(testValue, to.getValue())) {
					eval = Integer.MAX_VALUE - 10000;
					//                    eval++;
				}
			} else if (toId == varId) {
				if ((from.getValue() != null) && !bc.satisfies(testValue, from.getValue())) {
					//                    eval++;
					eval = Integer.MAX_VALUE - 10000;
				}
			} else if (!bc.isSatisfied())
				eval += bc.getWeight();
		}
		return eval;
	}

	//--------------------------------------------------------------------------------------------------//
	public int evaluateExternalConstraints(int varId, Integer testValue) {
		int eval = 0;
		for (int i = 0; i < getNumberOfNeighbours(); i++) { //testing all neighbours
			CSPNodeDB neigh = (CSPNodeDB) neighbours.elementAt(i);
			for (int j = 0; j < neigh.getNumberOfConstraints(); j++) { //testing all constraints/neighbour
				BC_ID_DifferentWithWeight bc = (BC_ID_DifferentWithWeight) neigh.getConstraint(j);
				CSPVariableDBA from = (CSPVariableDBA) bc.getFrom();
				CSPVariableDBA to = (CSPVariableDBA) bc.getTo();
				int fromId = from.getId().getLocalID();
				int toId = to.getId().getLocalID();
				if (fromId == varId) {
					if ((to.getValue() != null) && !bc.satisfies(testValue, to.getValue())) {
						eval += bc.getWeight();
					}
				}
				/*
				 * this does not apply, because external constraints are always local var->external var else if(toId==varId){ if(
				 * (from.getValue()!=null) && !bc.satisfies(testValue, from.getValue())){ eval++; } }
				 */
				else if (!bc.isSatisfied())
					eval += bc.getWeight();
			}
		}
		return eval;
	}

	//--------------------------------------------------------------------------------------------------//
	public int computeCurrentEval() {
		int eval = 0;
		improvements.clear();
		violatedConstraints.clear();
		//local ones
		for (int i = 0; i < constraints.size(); i++) {
			BC_ID_DifferentWithWeight constr = (BC_ID_DifferentWithWeight) constraints.elementAt(i);
			//            myLogger.print(constr.toString());
			if (!constr.isSatisfied()) {
				//		myLogger.println("->FALSE");
				eval = eval + constr.getWeight();
				violatedConstraints.add(constr);
			}
			//    	    else
			//		myLogger.println("->TRUE");
		}
		//external ones
		for (int i = 0; i < neighbours.size(); i++) {
			CSPNodeDB ngh = (CSPNodeDB) neighbours.elementAt(i);
			for (int j = 0; j < ngh.getNumberOfConstraints(); j++) {
				BC_ID_DifferentWithWeight constr = (BC_ID_DifferentWithWeight) ngh.getConstraint(j);
				if (!constr.isSatisfied()) {
					//                    myLogger.println(constr.toString() + " -> FALSE");
					eval = eval + constr.getWeight();
					violatedConstraints.add(constr);
				}
			}
		}
		myLogger.println("\n\t EVALUATION:" + eval);
		myLogger.println("\n\t VIOLATIONS:" + this.violatedConstraints);
		this.current_evaluation = eval;
		return eval;
	}

	//--------------------------------------------------------------------------------------------------//
	/*
	 * public int computeEvaluation(Vector myLocalVars){ int eval=0; //local constraints //PLEASE PAY ATTENTION THAT THE INCOMING VECTOR HAS TO
	 * CONTAIN ALL THE LOCAL VARIABLES IN ORDER!!! myLogger.println("Computing evaluation for " + myLocalVars); for(int i=0;i
	 * <constraints.size();i++){ BC_ID_Different constr =(BC_ID_Different) constraints.elementAt(i); CSPAbstractVariable from = constr.getFrom(); int
	 * idFrom = from.getID().getLocalID(); CSPAbstractVariable to = constr.getFrom(); int idTo = from.getID().getLocalID();
	 * 
	 * if(!constr.satisfies( ((CSPAbstractVariable)myLocalVars.elementAt(idFrom)).getValue(),
	 * ((CSPAbstractVariable)myLocalVars.elementAt(idTo)).getValue())){ eval=eval + constr.getWeight(); } } //external ones for(int i=0;i
	 * <neighbours.size();i++){ CSPNodeDB ngh = (CSPNodeDB)neighbours.elementAt(i); for(int j=0;j <ngh.getNumberOfConstraints();j++){ BC_ID_Different
	 * constr =(BC_ID_Different) ngh.getConstraint(j); CSPAbstractVariable from = constr.getFrom(); int idFrom = from.getID().getLocalID();
	 * CSPAbstractVariable to = constr.getFrom(); if(!constr.satisfies( ((CSPAbstractVariable)myLocalVars.elementAt(idFrom)).getValue(),
	 * to.getValue())){ eval=eval + constr.getWeight(); } } } return eval; }
	 */
	//--------------------------------------------------------------------------------------------------//
	public int computeEvaluation(int varId, Integer testValue) {
		//        violatedConstraints.clear();
		int local = evaluateLocalConstraints(varId, testValue);
		//        myLogger.println("\t\t/////////////////// Evaluated local constraints for V" + varId + "=" + testValue + " -> " + local);
		int ext = evaluateExternalConstraints(varId, testValue);
		//        myLogger.println("\t\t/////////////////// Evaluated external constraints for V" + varId + "=" + testValue + " -> " + ext);
		return local + ext;
	}

	//--------------------------------------------------------------------------------------------------//
	public int computeImprovement() {
		int myImprove = 0;
		int tmp_eval = current_evaluation;
		int eval = 0;
		improvements.clear();
		if (current_evaluation == 0)
			return 0;
		CSPVariableDBA tmpvar = null;
		for (int i = 0; i < getLocalSize(); i++) {
			CSPVariableDBA var = getVariable(i);
			//            myLogger.println("[[[[[[[[[[[[[[[[[[[[[[[ " + var.toString() + " " + var.getDomain());
			Vector dom = var.getDomain();
			Object tmp_val = var.getValue();
			for (int j = 0; j < dom.size(); j++) {
				Integer testValue = (Integer) dom.elementAt(j);
				if (!testValue.equals(var.getValue())) {
					eval = computeEvaluation(i, testValue);
					//                    myLogger.println("[[[[[[[[[[[[[[[[[[[[[[[ eval(i=" + i + ", testValue=" + testValue + ")=" +eval);
					//                    myLogger.println("[[[[[[[[[[[[[[[[[[[[[[[ myImprove=" + myImprove + " tmp_eval=" + tmp_eval + " eval=" + eval);
					if (myImprove < (tmp_eval - eval)) {
						tmpvar = new CSPVariableDBA(var.getId(), null);
						tmpvar.setValue(testValue);
						improvements.clear();
						improvements.add(tmpvar);
						myImprove = tmp_eval - eval;
						myLogger.println("DDDDDDDDDDDDDDDDDDDD improved: myImprove=" + myImprove + " eval=" + eval + " improvements=" + improvements);
					}
				}
			}
			var.setValue((Integer) tmp_val);
		}
		current_evaluation = tmp_eval;
		//	myLogger.println("[[[[[[[[[[[[[[[[[[[[[[[ " + var.toString() + " " + var.getDomain());
		myLogger.println("DDDDDDDDDDDDDDDDDDDDDDDDD FINAL improve: myImprove=" + myImprove + " eval=" + eval + " improvements=" + improvements + "    }}}tmpVar=" + tmpvar);
		return myImprove;
	}

	//--------------------------------------------------------------------------------------------------//
	public void executeImprovement() {
		for (int j = 0; j < improvements.size(); j++) {
			CSPVariableDBA tmpvar = (CSPVariableDBA) improvements.elementAt(j);
			CSPVariableDBA realvar = this.getVariable(tmpvar.getId().getLocalID());
			realvar.setValue(tmpvar.getValue());
		}
		improvements.clear();
	}

	//--------------------------------------------------------------------------------------------------//
	public void increaseWeights() {
		myLogger.println("Increasing weights for " + violatedConstraints.toString());
		for (int i = 0; i < violatedConstraints.size(); i++) {
			BC_ID_DifferentWithWeight constr = (BC_ID_DifferentWithWeight) violatedConstraints.elementAt(i);
			constr.increaseWeight(1);
		}
	}

	//--------------------------------------------------------------------------------------------------//
	public Vector getRandomInitial() {
		setRandomInitial();
		return variables;
	}

	/**
	 * 
	 * @uml.property name="improvements"
	 */
	//--------------------------------------------------------------------------------------------------//
	public Vector getImprovements() {
		return this.improvements;
	}

	//--------------------------------------------------------------------------------------------------//
	public int numberOfViolationsForVariable(CSPVariableDBA var) {
		int eval = 0;
		for (int i = 0; i < violatedConstraints.size(); i++) {
			BC_ID_DifferentWithWeight constr = (BC_ID_DifferentWithWeight) violatedConstraints.elementAt(i);
			CSPVariableDBA from = (CSPVariableDBA) constr.getFrom();
			if (from.getId().getLocalID() == var.getId().getLocalID())
				eval++;
			else {
				CSPVariableDBA to = (CSPVariableDBA) constr.getTo();
				if (from.getId().getLocalID() == var.getId().getLocalID())
					eval++;
			}
		}
		return eval;
	}

	//--------------------------------------------------------------------------------------------------//
	/*
	 * public int getImprove(){ return this.myImprove; }
	 */
	//--------------------------------------------------------------------------------------------------//
	public int getLocalSize() {
		return variables.size();
	}

	public int getNumberOfNeighbours() {
		return neighbours.size();
	}

	/**
	 * 
	 * @uml.property name="id"
	 */
	public void setId(int id) {
		this.id = id;
	}

	/**
	 * 
	 * @uml.property name="id"
	 */
	public int getId() {
		return id;
	}

	/**
	 * 
	 * @uml.property name="variables"
	 */
	public void setVariables(Vector v) {
		this.variables = v;
	}

	/**
	 * 
	 * @uml.property name="variables"
	 */
	public Vector getVariables() {
		return variables;
	}

	//only for compatibility with the one from CSPNodeAdopt -> used in the display
	public ComplexVariable getComplexVariable() {
		return this.myComplexVariable;
	}

	public void addVariable(Object v) {
		variables.add(v);
	}

	public int getNumberOfVariables() {
		return variables.size();
	}

	//------------------------------------------------------------------------------//
	public boolean deleteVariable(Object v) {
		return variables.removeElement(v);
	}

	public CSPVariableDBA getVariable(int i) {
		return (CSPVariableDBA) variables.elementAt(i);
	}

	public void addNeighbour(Object n) {
		neighbours.add(n);
	}

	public boolean deleteNeighbour(Object n) {
		return neighbours.removeElement(n);
	}

	/**
	 * 
	 * @uml.property name="neighbours"
	 */
	public void setNeighbours(Vector v) {
		this.neighbours = v;
	}

	//------------------------------------------------------------------------------//
	public CSPNodeDB getNeighbour(int id) {
		int cnt = 0;
		while (cnt < getNumberOfNeighbours()) {
			// now searching through the known variables (external ones) for the incoming one
			CSPNodeDB ngh = (CSPNodeDB) neighbours.elementAt(cnt);
			if (ngh.getId() == id)
				return ngh;
			cnt++;
		}
		return null;
	}

	/**
	 * 
	 * @uml.property name="neighbours"
	 */
	//------------------------------------------------------------------------------//
	public Vector getNeighbours() {
		return neighbours;
	}

	//------------------------------------------------------------------------------//
	public void addConstraint(Object bc) {
		constraints.add(bc);
	}

	/**
	 * 
	 * @uml.property name="constraints"
	 */
	//------------------------------------------------------------------------------//
	public void setConstraints(Vector constr) {
		constraints = constr;
	}

	//------------------------------------------------------------------------------//
	public boolean deleteConstraint(Object bc) {
		return constraints.removeElement(bc);
	}

	//------------------------------------------------------------------------------//
	public BC_ID_DifferentWithWeight getConstraint(int i) {
		return (BC_ID_DifferentWithWeight) constraints.elementAt(i);
	}

	//------------------------------------------------------------------------------//
	public int getNumberOfConstraints() {
		return constraints.size();
	}

	/**
	 * 
	 * @uml.property name="name"
	 */
	//------------------------------------------------------------------------------//
	public String getName() {
		return name;
	}

	/**
	 * 
	 * @uml.property name="name"
	 */
	//------------------------------------------------------------------------------//
	public void setName(String n) {
		name = n;
	}

	//------------------------------------------------------------------------------//
	public String getAllDomainsString() {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < getLocalSize(); i++) {
			sb.append(getVariable(i).getDomainString() + " ");
		}
		return sb.toString();
	}

	//--------------------------------------------------------------------------------------------------//
	public String toString() {
		String rez = new String(name + "=" + variables.toString() + "\n");
		for (int i = 0; i < getNumberOfNeighbours(); i++)
			rez = rez + "\t" + neighbours.elementAt(i).toString();
		//	rez +="\n";
		return rez;
	}

	//--------------------------------------------------------------------------------------------------//
	/*
	 * public void updateNeighbourDomains(Vector doms){ myLogger.println("UPDATING DOMAIN FOR: " +doms); CSPAbstractVariable extvar=null; boolean
	 * isNodeConnected=false; CSPNodeDB ngh=null; for(int i=0;i <doms.size();i++){//for each incoming domain extvar = (CSPAbstractVariable)
	 * doms.elementAt(i); // get the respective variable int nodeId = extvar.getID().getAgentID(); //trick: if the vector contains variables from
	 * multiple nodes---->>> trouble!! int varId = extvar.getID().getLocalID(); Vector extDomain = extvar.getDomain(); ngh = getNeighbour(nodeId);
	 * CSPAbstractVariable externalVar = ngh.getVariable(varId); externalVar.setDomain(extDomain);//I'm setting it anyway, since I don't know yet if
	 * it's connected or not... it might be!! for(int k=0;k <getLocalSize();k++){ Vector localDomain = getVariable(k).getDomain();
	 * myLogger.print("CHECKING LOCAL_DOMAIN: " +localDomain + " AGAINST EXTERN: " + extDomain); boolean isVarConnected=false; int l=0; while(l
	 * <localDomain.size() && !isVarConnected){ Integer localVal = (Integer) localDomain.elementAt(l); int j=0; while(j <extDomain.size() &&
	 * !isVarConnected){//now I am trying to determine whether I am connected to this node through constraints or not. If none of his values is in one
	 * of my domains, then I am not. Integer extVal = (Integer)extDomain.elementAt(j); if(localVal.equals(extVal)){ isNodeConnected=true;
	 * isVarConnected=true; setupExternalConstraints(k, nodeId, varId); myLogger.print(" -->Common: " + extVal ); } j++; } l++; } myLogger.println("
	 * ==> COMMON VALUES " +(isVarConnected? "yes":"no"));
	 *  } } if(!isNodeConnected){ neighbours.removeElement(ngh); myLogger.println("^^^^^^^^^^^^^^^^^^^^^^^^ Node " + ngh.getId() + " deleted from
	 * neighbours."); } }
	 */
	//--------------------------------------------------------------------------------------------------//
	/*
	 * public int computeImprovementBT(Vector valsSoFar){ int tmp_eval = current_evaluation; int eval; int currentVarID = valsSoFar.size();
	 * myLogger.println("Entered improve BT with " + valsSoFar); if(currentVarID==0){ myImprove=0; } if(currentVarID==getLocalSize()){ eval =
	 * computeEvaluation(valsSoFar); if(current_evaluation-eval>myImprove){ myImprove=current_evaluation-eval; myLogger.println("FOUND A BETTER
	 * SOLUTION:" + valsSoFar+ " ->" + myImprove); improvements=valsSoFar; } return myImprove; } else { CSPAbstractVariable currVar =
	 * getVariable(currentVarID); Vector domain = currVar.getDomain(); for(int i=0;i <domain.size();i++){
	 * currVar.setValue((Integer)domain.elementAt(i)); valsSoFar.add(currVar); computeImprovementBT(valsSoFar); valsSoFar.remove(currVar); } }
	 */
	/*
	 * CSPAbstractVariable var = getVariable(i); CSPAbstractVariable tmpvar = new CSPAbstractVariable(var.getID(),null); Vector dom = var.getDomain();
	 * Object tmp_val = var.getValue(); for(int j=0;j <dom.size();j++){ tmpvar.setValue((Integer)dom.elementAt(j));
	 * var.setValue((Integer)dom.elementAt(j)); eval=computeCurrentEval(); myLogger.println("Var " + tmpvar.toString() + " =>" + eval); if(myImprove <
	 * (tmp_eval-eval)){ improvements.clear(); improvements.add(tmpvar); myImprove=tmp_eval-eval; } } var.setValue((Integer)tmp_val); }
	 * current_evaluation = tmp_eval;
	 */
	//        return myImprove;
	//    }
	//--------------------------------------------------------------------------------------------------//
	/**
	 * @return Returns the constraints.
	 * 
	 * @uml.property name="constraints"
	 */
	public Vector getConstraints() {
		return constraints;
	}

	/* (non-Javadoc)
	 * @see testbed.CSP.CSPNodeInterface#getComplexValue()
	 */
	public ComplexValue getComplexValue() {
		// TODO Auto-generated method stub
		return myComplexVariable.getValue();
	}

}