/*
 */
package optapo;

import java.util.Hashtable;
import java.util.Random;
import java.util.Vector;

public class agent implements Cloneable{

	public String value;
	public String name;
	int optimalGlobalCost;
	int optimalLocalCost;
	int priority;
	int mediation = 0;
	int mediate = 1;
	public Vector goodList = new Vector();
	Vector initList = new Vector();
	public Vector domain = new Vector();
	Vector neighbors = new Vector();
	Vector agentView = new Vector();
	Hashtable agentViewPath = new Hashtable();
	Vector constraints = new Vector();
	Vector listShare = new Vector();
	Vector tempListShare = new Vector();
	int tempMediate = 0;
	int counter = 0;
	int mediations = 0;
	Vector mediationSizes = new Vector();
	boolean flagMediate = false;
	boolean flagInit = false;
	//boolean flagMediate = false;
	Hashtable labeled = new Hashtable();
	Vector preferences = new Vector();
	public int numMsg = 0;
	public double msgSize = 0;

	public agent(String name) {
		super();
		this.name = name;
		Random x = new Random();
		this.optimalGlobalCost = 0;
		this.mediation = 0;
		this.mediate = 0;
		this.goodList.add(this);
	}

	public int bestRelationCost(agent a){
		int tempCost = 999999999;
		constraint tempConstraint = getConstraint(this, a);
		if (tempConstraint == null) tempConstraint = getConstraint(a, this);
		if (tempConstraint != null) {
			for (int i = 0; i < tempConstraint.costVector.size(); i++) {
				if (((constraintCost)tempConstraint.costVector.elementAt(i)).cost < tempCost)
					tempCost = ((constraintCost)tempConstraint.costVector.elementAt(i)).cost;
			}
		}
		return tempCost;
	}

	public int bestRelationCost(agent a, agent b){
		int tempCost = 999999999;
		constraint tempConstraint = getConstraint(a, b);
		if (tempConstraint == null) tempConstraint = getConstraint(b, a);
		if (tempConstraint != null) {
			for (int i = 0; i < tempConstraint.costVector.size(); i++) {
				if (((constraintCost)tempConstraint.costVector.elementAt(i)).cost < tempCost)
					tempCost = ((constraintCost)tempConstraint.costVector.elementAt(i)).cost;
			}
		}
		return tempCost;
	}

	public int getRelationCost(agent b){
		constraint tempConstraint = getConstraint(this, b);
		if (tempConstraint != null)	return tempConstraint.getCost(this.value, b.value);
		else {
			tempConstraint = getConstraint(b, this);
			if (tempConstraint != null)	return tempConstraint.getCost(b.value, this.value);
		}
		return 999999999;
	}

	public int getRelationCost(agent a, agent b){
		constraint tempConstraint = getConstraint(a, b);
		if (tempConstraint != null)	return tempConstraint.getCost(a.value, b.value);
		else {
			tempConstraint = getConstraint(b, a);
			if (tempConstraint != null)	return tempConstraint.getCost(b.value, a.value);
		}
		return 999999999;
	}

	public void optimalLocalCost(){
		Vector tempGoodList = (Vector)this.goodList.clone();
		System.out.print("Optimal local cost > ");
		for (int j = 0;	j < tempGoodList.size(); j++) {
        	System.out.print((agent)tempGoodList.elementAt(j)+"="+((agent)tempGoodList.elementAt(j)).value+" ");
        }

		int tempCost = 0;
		for (int i = 0; i < this.goodList.size(); i++) {
			for (int j = 0;
			j < tempGoodList.size(); j++) {
				if (!((agent)this.goodList.elementAt(i)).equals((agent)tempGoodList.elementAt(j))){
					constraint tempConstraint = this.getConstraint(((agent)this.goodList.elementAt(i)),((agent)tempGoodList.elementAt(j)));
					if (tempConstraint != null) {
						tempCost += tempConstraint.getCost(((agent)this.goodList.elementAt(i)).value, ((agent)tempGoodList.elementAt(j)).value);
					}
				}
			}
		}
		System.out.println(" - "+tempCost);
		this.optimalLocalCost = tempCost;
	}



	public int optimalLocalCost(Vector goodList){
		Vector tempGoodList = (Vector)goodList.clone();
		int tempCost = 0;
		for (int i = 0; i < goodList.size(); i++) {
			for (int j = 0; j < tempGoodList.size(); j++) {
				if (!((agent)goodList.elementAt(i)).equals((agent)tempGoodList.elementAt(j))){
					constraint tempConstraint = this.getConstraint(((agent)goodList.elementAt(i)),((agent)tempGoodList.elementAt(j)));
					if (tempConstraint != null) {
						tempCost += tempConstraint.getCost(((agent)goodList.elementAt(i)).value, ((agent)tempGoodList.elementAt(j)).value);
					}
				}
			}
		}
		return tempCost;
	}

	public constraint getConstraint(agent a, agent b){
		for (int i = 0; i < this.constraints.size(); i++) {
			if ((((constraint)this.constraints.elementAt(i)).variable.name.equals(a.name) &&
				((constraint)this.constraints.elementAt(i)).otherVariable.name.equals(b.name))) {
					return (constraint)this.constraints.elementAt(i);
			}
		}
		return null;
	}

	public boolean existsSuboptimalRelationLowerPriorityAgent(){
		boolean exists = false;
		Vector tempGoodList = (Vector)goodList.clone();
		//int tempCost = 0;
		for (int i = 0; i < goodList.size(); i++) {
			for (int j = 0; j < tempGoodList.size(); j++) {
				if (this.getRelationCost(((agent)this.goodList.elementAt(i)), ((agent)tempGoodList.elementAt(j))) != 999999999 &&
					this.bestRelationCost(((agent)this.goodList.elementAt(i)), ((agent)tempGoodList.elementAt(j))) != 999999999 ) {
					
					if (this.getRelationCost(((agent)this.goodList.elementAt(i)), ((agent)tempGoodList.elementAt(j))) > this.bestRelationCost(((agent)this.goodList.elementAt(i)), ((agent)tempGoodList.elementAt(j))) &&
						(((agent)this).priority >= ((agent)this.goodList.elementAt(i)).priority)) {
								if ((((agent)this).priority == ((agent)this.goodList.elementAt(i)).priority)) {
									if ((this.name).compareTo(((agent)this.goodList.elementAt(i)).name) >= 0) exists = true;
								}
								else exists = true;
					}
				}

			}
		}
		return exists;
	}

	public void createTempListShare(){
		this.tempListShare.clear();
		
		for (int i = 0; i < this.agentView.size(); i++) {
			
				if (this.getRelationCost(((agent)this.agentView.elementAt(i))) > this.bestRelationCost(((agent)this.agentView.elementAt(i)))) {
					this.tempListShare.add(((agent)this.agentView.elementAt(i)));
				}
		}
	}

	public void checkAgentView() {
		if ((this.initList.size() != 0) || (this.mediation != 0)) return;
		this.tempMediate = 0;
		this.flagMediate = false;
		this.createTempListShare();
		this.optimalLocalCost();

		if ((this.optimalLocalCost > this.optimalGlobalCost) &&
			(existsSuboptimalRelationLowerPriorityAgent())) {
			tempMediate = 1;
		}
		else if (this.optimalLocalCost > this.optimalGlobalCost) {
			tempMediate = 2;
		}

		System.out.println(this.name+" - "+tempMediate+" - "+this.priority);
	}


	public void continueCheckAgentView() {

		if (tempMediate == 1 && !existsActiveGraterPriority(this)) {
			String newValue = domainOptimalGlobalCostLowerPriorityNeighbors();
			if (newValue != null) {
				this.value = newValue;
				this.mediate = 0;
				this.createTempListShare();
				System.out.println(this.name+" - Active, the neighbors have lower priority");
				this.numMsg+=this.agentView.size();
				this.msgSize+=10*this.agentView.size();
			}
			else {
				this.flagMediate = true;
				System.out.println(this.name+" - Active, the neighbors DO NOT have lower priority");
			}
		}
		else if (tempMediate == 2){
			this.flagMediate = true;
		}
		else if(tempMediate != this.mediate || (this.mediate == 0 && !tempListShare.containsAll(listShare))){

			this.mediate = tempMediate;
			listShare = (Vector)tempListShare.clone();
			this.numMsg+=this.agentView.size();
			this.msgSize+=10*this.agentView.size();

		}
		else if (this.mediate==0) {
			Vector tempAgentView = (Vector)this.agentView.clone();
			
			for (int i = 0; i < tempAgentView.size(); i++) {

				for (int j = 0; j < ((agent)tempAgentView.elementAt(i)).listShare.size(); j++) {
					if (!this.pertences(this.goodList,(agent)((agent)tempAgentView.elementAt(i)).listShare.elementAt(j)) &&
							((agent)((agent)tempAgentView.elementAt(i))).mediate == 0) {
						Vector tempAgentInPath = new Vector(); 
						if ((Vector)((agent)tempAgentView.elementAt(i)).agentViewPath.get(((agent)((agent)tempAgentView.elementAt(i)).listShare.elementAt(j)).name) != null) 
							tempAgentInPath.addAll((Vector)((agent)tempAgentView.elementAt(i)).agentViewPath.get(((agent)((agent)tempAgentView.elementAt(i)).listShare.elementAt(j)).name));
						tempAgentInPath.addElement(((agent)((agent)tempAgentView.elementAt(i)).listShare.elementAt(j)));

						for (int k = 0; k < tempAgentInPath.size(); k++) {
							if (!this.pertences(tempAgentView,((agent)tempAgentInPath.elementAt(k)))) {

								this.initList.add(((agent)tempAgentInPath.elementAt(k)));
								this.numMsg++;
								this.msgSize+=10;
								((agent)tempAgentInPath.elementAt(k)).init(this, tempAgentInPath);
							}
						}
						
					}
				}

			}
		}
		listShare = (Vector)tempListShare.clone();
		this.mediate = this.tempMediate;

	}

	

	public boolean existsActiveGraterPriority(agent a){
		boolean exists = false;
		for (int i = 0; i < this.goodList.size(); i++) {

			if (!((agent)this.goodList.elementAt(i)).name.equals(a.name)) {

				if ((((agent)this.goodList.elementAt(i)).mediate == 1) &&
					((agent)a).priority <= ((agent)this.goodList.elementAt(i)).priority) {
						if (((agent)a).priority == ((agent)this.goodList.elementAt(i)).priority) {
							
							if ((a.name).compareTo(((agent)this.goodList.elementAt(i)).name) >= 0) exists = true;  
						}
						else exists = true;
				}
			}
		}
		return exists;
	}

	
	public boolean existsNeighborGraterPriority(){
		boolean existis = false;
		for (int i = 0; i < this.neighbors.size(); i++) {
			if (((agent)this).priority <= ((agent)this.agentView.elementAt(i)).priority) {
				if (((agent)this).priority == ((agent)this.goodList.elementAt(i)).priority) {
					if ((this.name).compareTo(((agent)this.goodList.elementAt(i)).name) >= 0) existis = true;  
				}
				else existis = true;
			}
		}
		return existis;
	}
	
	public String domainOptimalGlobalCostLowerPriorityNeighbors(){
		for(int k = 0; k < this.domain.size(); k++)
			if (!((String)this.domain.elementAt(k)).equals(this.value) ) {
				agent tempAgent = ((agent)this.clone());
				tempAgent.value = ((String)this.domain.elementAt(k));
				Vector tempGoodList = ((Vector)this.agentView.clone());
				tempGoodList.remove(this);
				tempGoodList.add(tempAgent);
				if (optimalLocalCost(tempGoodList) == this.optimalGlobalCost && !this.existsNeighborGraterPriority()) {
					return tempAgent.value;
				}
			}
		return null;
	}
	
	
	public void mediate() {
		if (this.flagMediate) {
			System.out.println(this.name+" decide mediar: " +tempMediate+" - "+this.goodList);
			this.mediations++;
			this.mediationSizes.add(this.goodList.size()-1);
			this.mediation = tempMediate;
			this.counter = this.goodList.size()-1;
			Vector tempGoodList = (Vector)this.goodList.clone();
			for (int i = 0; i < tempGoodList.size(); i++) {
				this.numMsg++;
				this.msgSize+=5;
				if (((agent)tempGoodList.elementAt(i)) != this) ((agent)tempGoodList.elementAt(i)).receiveEvaluate(this);
			}
		}
	}

	public void receiveEvaluate(agent agent) {

		if ((this.mediation != 0 || existsActiveGraterPriority(agent)) && agent.mediate == 1) {
			agent.mediatorWait(this);
		}
		else {
			if (this.mediation != 1) this.mediation = this.tempMediate;
			for (int j = 0; j < this.domain.size(); j++) {
				agent tempAgent = (agent)this.clone();
				tempAgent.value = ((String)this.domain.elementAt(j));
				Vector tempVector = new Vector();
				for (int i = 0; i < this.goodList.size(); i++) {

					if (this.getRelationCost(tempAgent, ((agent)this.goodList.elementAt(i))) > this.bestRelationCost(((agent)this.goodList.elementAt(i)))) {
						tempVector.addElement(((agent)this.goodList.elementAt(i)));
					}

				}
				this.labeled.put(((String)this.domain.elementAt(j)), tempVector);
			}
			agent.mediatorEvaluate(this);
		}
		this.numMsg++;
	}
	
	public void mediatorWait(agent a){
		this.counter--;
		if (this.counter == 0){
			this.chooseSolution();
		}	
	}
	
	public void mediatorEvaluate(agent a){
		if (!pertences(this.preferences,a)) this.preferences.addElement(a);
		this.counter--;
		if (this.counter == 0){
			this.chooseSolution();
		}	
	}
	
	public void chooseSolution(){
		System.out.println(this.name+" choose solution > "+ this.optimalLocalCost);
		if (!this.preferences.isEmpty()) {
			Vector bb = this.branchandbound();
			System.out.print("resultado B&B "+bb+" - ");
			for (int i = 0; i < bb.size(); i++) {
					System.out.print(((agent)bb.elementAt(i)).value+" - ");
			}
			System.out.print("\n");

			this.optimalGlobalCost = this.optimalLocalCost(bb);

			Vector temp = (Vector)this.vectorOfAgentsClone(this.goodList).clone();
			for (int i = 0; i < this.preferences.size(); i++) {
				for (int j = 0; j < temp.size(); j++) {
					if (((agent)temp.elementAt(j)).name.equals(((agent)this.preferences.elementAt(i)).name)) {
						((agent)temp.elementAt(j)).changeAgentDomain(bb);
					}
				}
			}
			for (int j = 0; j < temp.size(); j++) {
				if (((agent)temp.elementAt(j)).name.equals(this.name)) {
					((agent)temp.elementAt(j)).changeAgentDomain(bb);
						break;
				}
			}
		
			int newOptimalGlobalCost = this.optimalLocalCost(temp);//F'i

			int actualOptimalGlobalCost = this.optimalLocalCost(goodList);//F's

			if (this.mediation == 1 && newOptimalGlobalCost <= actualOptimalGlobalCost){
				this.changeAgentDomain(bb);

			}


			Vector tempAgentView = (Vector)this.agentView.clone();

			for (int i = 0; i < tempAgentView.size(); i++) {
				if (this.pertences(this.preferences, ((agent)tempAgentView.elementAt(i)))) {
					agent tempAgent = this.getAgentIn(bb,((agent)tempAgentView.elementAt(i)).name);

					Vector tempVector = (Vector)(getLabeled(tempAgent.name).get(tempAgent.value));

					Vector tempAgentInPath = new Vector();
					tempAgentInPath.addElement(((agent)tempAgentView.elementAt(i)));
					if (tempVector != null)
					for (int k = 0; k < tempVector.size(); k++) {
						if (!this.pertences(tempAgentView, ((agent)tempVector.elementAt(k))) &&
							!((agent)tempVector.elementAt(k)).name.equals(this.name)) {

							this.initList.addElement(((agent)tempVector.elementAt(k)));
							((agent)tempVector.elementAt(k)).init(this, tempAgentInPath);
							this.numMsg++;
							this.msgSize+=10;
						}
					}
					if (this.mediation == 1 && newOptimalGlobalCost <= actualOptimalGlobalCost){
						((agent)tempAgentView.elementAt(i)).receiveAccept(tempAgent.value);
						this.numMsg++;
						this.msgSize+=5;
					}
					else if (this.mediation == 1 && newOptimalGlobalCost > actualOptimalGlobalCost) {
						((agent)tempAgentView.elementAt(i)).receiveAccept(((agent)tempAgentView.elementAt(i)).value);
						this.numMsg++;
						this.msgSize+=5;
					}
				}
				else {
					if (this.mediation == 1) {
						this.numMsg++;
						this.msgSize+=5;
					}
				}

			}
		}
		this.preferences.clear();
		this.mediation = 0;
		
	}
	
	public void receiveAccept(String domain){
		this.value = domain;
		this.mediation = 0;
	}
	
	
	
	
	
	public void startAgent(Vector constraints, Vector domain) {
		this.domain = domain;
		this.constraints = constraints;
		this.priority = this.neighbors.size();
		this.initList = (Vector)this.neighbors.clone();

	}
	
	public void startAgent(Vector constraints) {
		this.constraints = constraints;
		this.priority = this.neighbors.size();
		this.initList = (Vector)this.vectorOfAgentsClone(neighbors).clone();
		this.listShare = (Vector)this.vectorOfAgentsClone(neighbors).clone();
	}
	
	public void setDomain(Vector domain){
		this.domain = domain;
	}

	public Vector branchandbound(){
		Vector temp = new Vector();
		//System.out.println(this.preferences);
		for (int i = 0; i < this.goodList.size(); i++) {
			if (!pertences(this.preferences, ((agent)this.goodList.elementAt(i)))) {
				temp.addElement(((agent)this.goodList.elementAt(i)));
			}
		}
		branchandbound bb = new branchandbound((Vector)this.vectorOfAgentsClone(this.goodList), this.constraints, this.optimalGlobalCost, temp, this);
		return bb.optimal;
	}
	
	public Vector geneticalgorithm(){
			Vector temp = new Vector();
			geneticalgorithm ga = new geneticalgorithm((Vector)this.vectorOfAgentsClone(this.goodList), this.constraints, this.optimalLocalCost);
			return ga.optimal;
		
	}
	
	
	public Vector changeAgentDomain(Vector list, String domain, String agentName){
		for (int i = 0; i < list.size(); i++) {
			if (((agent)list.elementAt(i)).name.equals(agentName)) {
				((agent)list.elementAt(i)).value = domain;
			}	
		}
		return list;
	}
	
	
	public void changeAgentDomain(Vector list){
		for (int i = 0; i < list.size(); i++) {
			if (((agent)list.elementAt(i)).name.equals(this.name)) {
				this.value = ((agent)list.elementAt(i)).value;
			}	
		}
	}
	
	public boolean pertences(Vector list, agent agent){
		for (int i = 0; i < list.size(); i++) {
			if (((agent)list.elementAt(i)).name.equals(agent.name)) {
				return true;
			}	
		}
		return false;
	}
	
	public agent getAgentIn(Vector list, String name) {
		for (int i = 0; i < list.size(); i++) {
			if (((agent)list.elementAt(i)).name.equals(name)) {
				return ((agent)list.elementAt(i));
			}
		}
		return null;
	}
	
	public Hashtable getLabeled(String name) {
		for (int i = 0; i < preferences.size(); i++) {
			if (((agent)preferences.elementAt(i)).name.equals(name)) {
				return ((agent)preferences.elementAt(i)).labeled; 
			}
		}
		return null;
	}
	
	public void checkAgent() {
		for (int i = 0; i < this.agentView.size(); i++) {
			((agent)this.agentView.elementAt(i)).checkAgentView();
		}
		for (int i = 0; i < this.agentView.size(); i++) {
			((agent)this.agentView.elementAt(i)).continueCheckAgentView();
		}
	}

	
	
	public boolean bIsMyNeighbor(agent b) {
		for (int i = 0; i < this.neighbors.size(); i++) {
			if (((agent)this.neighbors.elementAt(i)).name.equals(b.name)) return true;
		}
		return false;
	}
	
	
//  ***********************************************************************************
//	***********************************************************************************
//	***********************************************************************************
//	***********************************************************************************
	
	public void sendInit() {
		System.out.println(this.name +" --- "+this.neighbors);
		for (int i = 0; i < this.neighbors.size(); i++) {
			this.numMsg++;
			this.msgSize+=10;
			((agent)this.neighbors.elementAt(i)).init(this, null);
		}
	}
	
	public void init(agent agent, Vector path) {
		this.agentView.add(agent);
		if (path != null) this.agentViewPath.put(agent.name,path);
		Vector tempGoodList = (Vector)this.goodList.clone();
		boolean insert = false;
		for (int i = 0; i < tempGoodList.size(); i++) {
			if (bIsNeighborOfa(((agent)tempGoodList.elementAt(i)), agent)) {
				insert = true;
			}
		}
		if (insert)	{
			if (!pertences(this.goodList, agent)) this.goodList.add(agent);
			for (int j = 0; j < this.agentView.size(); j++) {
			 	if (!isInGoodList((agent)this.agentView.elementAt(j)) &&
			 		(this.isRelatedSomeoneInGoodList((agent)this.agentView.elementAt(j))))
			 		this.goodList.add((agent)this.agentView.elementAt(j));
			}
			this.priority = this.goodList.size();
		}
		if (this.isInInitList(agent))
			this.removeInitList(agent);
		else {
			agent.init(this, null);
			this.numMsg++;
			this.msgSize += 10;
		}
		
	}
	
	public boolean bIsNeighborOfa(agent a, agent b) {
		for (int i = 0; i < a.agentView.size(); i++) {
			if (((agent)a.agentView.elementAt(i)).name.equals(b.name)) return true;
		}
		return false;
	}
	
	public boolean isInGoodList(agent a) {
		for (int k = 0; k < this.goodList.size(); k++) {
			if ((((agent)this.goodList.elementAt(k)).name).equals(a.name)) return true;
		}
		return false;
	}
	
	public boolean isInInitList(agent a) {
		for (int k = 0; k < this.initList.size(); k++) {
			if ((((agent)this.initList.elementAt(k)).name).equals(a.name)) return true;
		}
		return false;
	}
	
	public void removeInitList(agent a) {
		for (int k = 0; k < this.initList.size(); k++) {
			if ((((agent)this.initList.elementAt(k)).name).equals(a.name))
				this.initList.removeElementAt(k);
		}
	}
	
	public boolean isRelatedSomeoneInGoodList(agent a) {
		for (int k = 0; k < this.goodList.size(); k++) {
			for (int i = 0; i < ((agent)this.goodList.elementAt(k)).listShare.size(); i++) {
				if ((((agent)((agent)this.goodList.elementAt(k)).listShare.elementAt(i)).name).equals(a.name)) return true;				
			}
		}
		return false;
	}


	public String toString(){
		return this.name;
	}
	
	public Object clone() {
		agent agentClone = new agent(this.name);
		agentClone.constraints = (Vector)this.constraints.clone();
		agentClone.value = this.value;
		agentClone.optimalGlobalCost = this.optimalGlobalCost;
		agentClone.optimalLocalCost = this.optimalLocalCost;
		agentClone.priority = this.priority;
		agentClone.mediation= this.mediation;
		agentClone.mediate = this.mediate;
		agentClone.goodList = (Vector)this.goodList.clone();
		agentClone.initList = (Vector)this.initList.clone();
		agentClone.neighbors = (Vector)this.neighbors.clone();
		agentClone.agentView = (Vector)this.agentView.clone();
		agentClone.listShare = (Vector)this.listShare.clone();
		agentClone.tempListShare = (Vector)this.tempListShare.clone();
		agentClone.domain = (Vector)this.domain.clone();
		return agentClone;
	}
		
	public Vector vectorOfAgentsClone(Vector temp) {
		Vector t = new Vector();
		for (int i = 0; i < temp.size(); i++) {
			t.addElement(((agent)temp.elementAt(i)).clone());		
		}
		return t;
	}

}
