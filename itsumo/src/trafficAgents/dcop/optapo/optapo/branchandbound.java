package optapo;

import java.util.Collection;
import java.util.Collections;
import java.util.Vector;

public class branchandbound {
	
	public Vector optimal;
	public Vector constraints;
	int globalOptimal = 0;
	Vector outSide;
	agent agent;
	
	public branchandbound(Vector vector, Vector constraints, int globalOptimal, Vector outSide, agent agent){
		this.agent = agent;
		this.outSide = outSide;
		this.globalOptimal = globalOptimal;
		this.constraints = constraints;
		this.optimal = (Vector)this.vectorOfAgentsClone(vector);
		
		this.bb((Vector)this.vectorOfAgentsClone(vector), this.optimal, new Vector());
		if (cost(this.optimal) == cost(vector)) this.optimal = (Vector)this.vectorOfAgentsClone(vector);
	}
	
	public void sortDomain(agent agent) {
		//System.out.println(agent+" - "+ agent.value+" - "+agent.domain);
		Vector vtemp =  (Vector)agent.domain.clone();
		Collections.shuffle(vtemp);
		for (int j = 0; j < vtemp.size(); j++) {
			if (((String)vtemp.elementAt(j)).equals(agent.value)) {
				//System.out.println(((String)vtemp.elementAt(j)));
				String temp = ((String)vtemp.elementAt(0));
				agent.domain.removeElementAt(0);
				agent.domain.insertElementAt(((String)vtemp.elementAt(j)), 0);
				agent.domain.removeElementAt(j);
				agent.domain.add(temp);
			}
		}
	}
	
	public boolean stopEarly(Vector vector){

		boolean costOutsideisOptimal = true;
		
		if (this.outSide != null) {
		
		for (int z = 0; z <  vector.size(); z++){
			for (int x = 0; x <  this.outSide.size(); x++){
				constraint temConstraint = null;
				if (!((agent)vector.elementAt(z)).name.equals(((agent)this.outSide.elementAt(x)).name) && !((agent)vector.elementAt(z)).name.equals(agent.name) && !((agent)this.outSide.elementAt(x)).name.equals(agent.name)) {
					if (getConstraint(((agent)vector.elementAt(z)),((agent)this.outSide.elementAt(x))) == null) {
						if (getConstraint(((agent)this.outSide.elementAt(x)), ((agent)vector.elementAt(z))) != null) {
							temConstraint = getConstraint(((agent)this.outSide.elementAt(x)), ((agent)vector.elementAt(z))); 
						}
					}
					else {
						temConstraint = getConstraint(((agent)vector.elementAt(z)),((agent)this.outSide.elementAt(x)));
					}
				}
				if (temConstraint != null) {
					if (temConstraint.getCost(((agent)vector.elementAt(z)).value,((agent)this.outSide.elementAt(x)).value) != bestRelationCost(((agent)vector.elementAt(z)),((agent)this.outSide.elementAt(x)))) costOutsideisOptimal = false;
				}
			}
		}
		if (cost(vector) != this.globalOptimal) costOutsideisOptimal = false;
		} 
		else costOutsideisOptimal = false;
		return costOutsideisOptimal;
	}
	
	public void bb(Vector tempGoodlist, Vector tempOptimal, Vector visitedQueue){
		if (!tempGoodlist.isEmpty()) {
			agent agent = (agent)((agent)tempGoodlist.elementAt(0)).clone();
			Vector temp = (Vector)tempGoodlist.clone();
			temp.removeElementAt(0);
			visitedQueue.add(agent);
			sortDomain(agent);

			for (int j = 0; j < agent.domain.size(); j++) {
				
				agent.value = ((String)agent.domain.elementAt(j));
				this.changeAgentDomain(tempOptimal, ((String)agent.domain.elementAt(j)), agent.name);
				
				Vector newTempOptimal = (Vector)this.vectorOfAgentsClone(this.optimal);
				this.changeAgentDomain(newTempOptimal, ((String)agent.domain.elementAt(j)), agent.name);
				
				if (cost(tempOptimal) <= cost(this.optimal)) {
					this.optimal = (Vector)this.vectorOfAgentsClone(tempOptimal);
				}
				if (cost(visitedQueue) <= cost(this.optimal) && !stopEarly(this.optimal)) {
			
					this.bb(temp, tempOptimal, visitedQueue);
				
				}
				if (j == agent.domain.size()-1) this.changeAgentDomain(tempOptimal, ((String)agent.domain.elementAt(0)), agent.name);
			}
			visitedQueue.remove(agent);
		}
	}
	
	public Vector changeAgentDomain(Vector list, String domain, String agentName){
		for (int i = 0; i < list.size(); i++) {
			if (((agent)list.elementAt(i)).name.equals(agentName)) {
				((agent)list.elementAt(i)).value = domain;
			}	
		}
		return list;
	}
	
	public int cost(Vector vector){
		Vector tempVector = (Vector)vector.clone();
		int tempCost = 0;
		for (int i = 0; i < vector.size(); i++) {
			for (int j = 0;	j < tempVector.size(); j++) {
				if (!((agent)vector.elementAt(i)).equals((agent)tempVector.elementAt(j))){
					constraint tempConstraint = this.getConstraint(((agent)vector.elementAt(i)),((agent)tempVector.elementAt(j)));
					if (tempConstraint != null) {
						tempCost += tempConstraint.getCost(((agent)vector.elementAt(i)).value, ((agent)tempVector.elementAt(j)).value);
					}
				}
			}
		}
		return tempCost;
	}
	
	public constraint getConstraint(agent a, agent b){
		for (int i = 0; i < constraints.size(); i++) {
			if ((((constraint)constraints.elementAt(i)).variable.name.equals(a.name) &&
				((constraint)constraints.elementAt(i)).otherVariable.name.equals(b.name))) {
					return (constraint)constraints.elementAt(i);
			}
		}
		return null;
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
	
	public Vector vectorOfAgentsClone(Vector temp) {
		Vector t = new Vector();
		for (int i = 0; i < temp.size(); i++) {
			t.addElement(((agent)temp.elementAt(i)).clone());		
		}
		return (Vector)t.clone();
	}
	
}
