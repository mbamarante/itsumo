package optapo;

import java.util.*;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Timestamp;

public class simulator {

	Vector agents = new Vector();
	Vector domain = new Vector();
	Vector constraints = new Vector();
	
	public simulator() {
		super();
	}
	
	public static void main(String[] args) {
		Calendar cal = new GregorianCalendar();
		System.out.println(cal.get(Calendar.HOUR_OF_DAY)+" "+cal.get(Calendar.MINUTE)+" "+cal.get(Calendar.SECOND));

		if (args == null){
			System.out.println("programa arquivo metodo[x,o]");
		}
		else if (args.length <= 1) {
			System.out.println("programa arquivo metodo[x,o]");
		}
		else{
		String fileName = args[0];

		//************************************************************************
		if (args[1].equals("o")) {
			System.out.println("processando por mediacao cooperativa...");
			simulator simul = new simulator();

			simul.readScenarioFromFile(fileName);
			simul.execute();
			System.out.println("------------------------- fim");

			// TAVA AQUI PRA ESCREVER NO ARQUIVO			

			System.out.println("custo: "+simul.cost(simul.agents));
		//************************************************************************
		} else if (args[1].equals("x")) {
			simulator simul = new simulator();

			simul.readScenarioFromFile(fileName);

			System.out.println("processando por forca bruta...");
			cal = new GregorianCalendar();
			System.out.println(cal.get(Calendar.HOUR_OF_DAY)+" "+cal.get(Calendar.MINUTE)+" "+cal.get(Calendar.SECOND));
			
			geneticalgorithm bb = new geneticalgorithm(simul.agents, simul.constraints, 0);
			for (int i = 0; i < bb.optimal.size(); i++) {
				System.out.println(((agent)bb.optimal.elementAt(i)).name+" = "+((agent)bb.optimal.elementAt(i)).value);	
			}
			System.out.println("cost: "+simul.cost(bb.optimal));
			cal = new GregorianCalendar();
			System.out.println(cal.get(Calendar.HOUR_OF_DAY)+" "+cal.get(Calendar.MINUTE)+" "+cal.get(Calendar.SECOND));
			Vector temp = bb.optimal;

			System.out.println("optimal cost: "+simul.cost(temp));
			
		}
		//************************************************************************
		}
		
	}
	
	public void readScenarioFromFile(String fileName){
		Random x = new Random();
		BufferedReader config = openFile(fileName);
		String temp;
		try {
			int countVariable = 0;
			while ((temp = config.readLine())!= null){

				String[] tokens = temp.split(" ");
				if (tokens[0].equals("VARIABLE")){

					this.agents.addElement(new agent("ND"+tokens[1]));
					Vector domain = new Vector();

					for (int i = 0; i < Integer.parseInt(tokens[3]); i++){
						domain.addElement(Integer.toString(i));
					}
					((agent)this.agents.elementAt(this.agents.size()-1)).setDomain(domain);
					((agent)this.agents.elementAt(this.agents.size()-1)).value = (String)domain.elementAt(x.nextInt(domain.size()));//);
					System.out.println(((agent)this.agents.elementAt(this.agents.size()-1)).name);
					System.out.println(((agent)this.agents.elementAt(this.agents.size()-1)).domain);
					System.out.println(((agent)this.agents.elementAt(this.agents.size()-1)).value);

				} 
				else if (tokens[0].equals("CONSTRAINT")){

					this.constraints.addElement(new constraint(findAgentByName("ND"+tokens[1]), findAgentByName("ND"+tokens[2])));
					findAgentByName("ND"+tokens[1]).neighbors.add(findAgentByName("ND"+tokens[2]));
					findAgentByName("ND"+tokens[2]).neighbors.add(findAgentByName("ND"+tokens[1]));
					//
				}
				else if (tokens[0].equals("F")) {

					((constraint)this.constraints.elementAt(this.constraints.size()-1)).addConstraintCost(tokens[1],tokens[2],Integer.parseInt(tokens[3]));
				}
			}

			for (int i =0; i < this.agents.size(); i++){
				((agent)this.agents.elementAt(i)).startAgent(this.constraints);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public agent findAgentByName(String name){
		for (int i =0; i < this.agents.size(); i++){
			if (((agent)this.agents.elementAt(i)).name.equals(name)) return ((agent)this.agents.elementAt(i));
		}
		return null;
	}
	
	public void execute(){

		for (int i = 0; i < this.agents.size(); i++) {
			((agent)this.agents.elementAt(i)).sendInit();
		}
		for (int i = 0; i < this.agents.size(); i++) {
			System.out.println("Agent and good list"+((agent)this.agents.elementAt(i)).name+" = "+((agent)this.agents.elementAt(i)).goodList);
			for (int j = 0; j < ((agent)this.agents.elementAt(i)).goodList.size(); j++) {
				System.out.print("Agent and good list"+((agent)((agent)this.agents.elementAt(i)).goodList.elementAt(j)).name+" = "+ ((agent)((agent)this.agents.elementAt(i)).goodList.elementAt(j)).value+" | ");
			}
			System.out.println();
		}

		int y = 0;

		Calendar cal = new GregorianCalendar();
		int timeInit = cal.get(Calendar.SECOND);
		
		do {
			int x = 0;
				Collections.shuffle(agents);
				for (int i = 0; i < this.agents.size(); i++) {
					((agent)this.agents.elementAt(i)).checkAgentView();	
				}
				for (int i = 0; i < this.agents.size(); i++) {
					((agent)this.agents.elementAt(i)).continueCheckAgentView();	
				}
				for (int i = 0; i < this.agents.size(); i++) {
					((agent)this.agents.elementAt(i)).mediate();	
				}
				System.out.println("****************************************************************************************");
				x++;
				y++;
				System.out.println(" > "+y);
				for (int i = 0; i < this.agents.size(); i++) {
					System.out.println("Execute loop -> "+((agent)this.agents.elementAt(i)).name+" : "+((agent)this.agents.elementAt(i)).goodList+" +++ "+((agent)this.agents.elementAt(i)).mediate+" | "+((agent)this.agents.elementAt(i)).optimalLocalCost+" = "+((agent)this.agents.elementAt(i)).optimalGlobalCost);
				}
				cal = new GregorianCalendar();
				System.out.println(cal.get(Calendar.HOUR_OF_DAY)+" "+cal.get(Calendar.MINUTE)+" "+cal.get(Calendar.SECOND));
				System.out.println(((agent)this.agents.elementAt(0)).numMsg);
		} while(!this.testEnd());
		
		cal = new GregorianCalendar();
		int timeEnd = cal.get(Calendar.SECOND) - timeInit;

		System.out.println(y);

		int totalMsg = 0;
		int totalMsgSize = 0;

		for (int i = 0; i < this.agents.size(); i++) {
			System.out.println(((agent)this.agents.elementAt(i)).name+" = "+((agent)this.agents.elementAt(i)).value);
			totalMsg = totalMsg + (int)((agent)this.agents.elementAt(i)).numMsg;
                        totalMsgSize = totalMsgSize + (int)((agent)this.agents.elementAt(i)).msgSize;
		}

		try {
                	BufferedWriter out = new BufferedWriter(new FileWriter("optapo.txt", true));
			int numberOfMediations = 0;
			double avgSizeMediations = 0;
			for (int i = 0; i < this.agents.size(); i++) {
				out.write("VARIABLE="+((agent)this.agents.elementAt(i)).name.substring(2)+"\n");
				out.write("VALUE="+((agent)this.agents.elementAt(i)).value+"\n");

				numberOfMediations += ((agent)this.agents.elementAt(i)).mediations;

				int meio = 0;
				for (int tt = 0; tt < ((agent)this.agents.elementAt(i)).mediationSizes.size(); tt++)
				{
					meio += ((agent)this.agents.elementAt(i)).mediationSizes.elementAt(tt);
				}
				avgSizeMediations += meio / ((agent)this.agents.elementAt(i)).mediationSizes.size();
			}
			out.write("TOTALTIME="+timeEnd+"\n");
			int cycleCount = y*5 + 2; //y*5 = ((2)agentview(process/send and receive) + (3)mediate(process/send, receive and choose)) 
                                                  //+ 2 = (send init e receive init)
			out.write("CYCLES=" + cycleCount+"\n");
			out.write("MSGS="+totalMsg+"\n");
			double msgSize = totalMsgSize/totalMsg;
                        out.write("AVGMSGSIZE="+msgSize+"\n");

			double hi = numberOfMediations / this.agents.size();
			out.write("MEDS="+hi+"\n");
			double ho = avgSizeMediations / this.agents.size();
			out.write("MEDSSIZE="+ho+"\n");

			out.close();
                } catch (IOException e){}
	}
	
	public boolean testEnd(){

		for (int i = 0; i < this.agents.size(); i++) {

			if (((agent)this.agents.elementAt(i)).optimalLocalCost != ((agent)this.agents.elementAt(i)).optimalGlobalCost 
					&& ((agent)this.agents.elementAt(i)).optimalLocalCost!=0
					) return false;
		}
		return true;
	}
	
	public BufferedReader openFile(String fileName){
		try {
			FileReader fileConfig = new FileReader(fileName);
			BufferedReader config = new BufferedReader(fileConfig);

			return config;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public int cost(Vector vector){
		Vector tempVector = (Vector)vector.clone();
		int tempCost = 0;
		for (int i = 0; i < vector.size(); i++) {
			for (int j = 0;	j < tempVector.size(); j++) {
				if (!((agent)vector.elementAt(i)).equals((agent)tempVector.elementAt(j))){
					constraint tempConstraint = this.getConstraint(((agent)vector.elementAt(i)),((agent)tempVector.elementAt(j)));
					if (tempConstraint != null) {
						System.out.print(((agent)vector.elementAt(i))+" / "+((agent)tempVector.elementAt(j))+" > ");
						System.out.print(((agent)vector.elementAt(i)).value+" / "+((agent)tempVector.elementAt(j)).value+" = ");
						tempCost += tempConstraint.getCost(((agent)vector.elementAt(i)).value, ((agent)tempVector.elementAt(j)).value);
						System.out.print(tempConstraint.getCost(((agent)vector.elementAt(i)).value, ((agent)tempVector.elementAt(j)).value));
						System.out.println(" - "+bestRelationCost(((agent)vector.elementAt(i)), ((agent)tempVector.elementAt(j))));

					}
				}
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
	
	public constraint getConstraint(agent a, agent b){
		for (int i = 0; i < constraints.size(); i++) {
			if ((((constraint)constraints.elementAt(i)).variable.name.equals(a.name) &&
				((constraint)constraints.elementAt(i)).otherVariable.name.equals(b.name))) {
					return (constraint)constraints.elementAt(i);
			}
		}
		return null;
	}
	
	public Vector vectorOfAgentsClone(Vector temp) {
		Vector t = new Vector();
		for (int i = 0; i < temp.size(); i++) {
			t.addElement(((agent)temp.elementAt(i)).clone());		
		}
		return t;
	}

}
