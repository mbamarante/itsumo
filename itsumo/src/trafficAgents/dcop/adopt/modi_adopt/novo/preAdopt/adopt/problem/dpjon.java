package adopt.problem;

import adopt.solver.*;
import adopt.common.*;

import java.io.*;
import java.util.*;
import java.net.*;

public  class dpjon 
{
	int Calculate(Variable dpnode){
	
		int Threshold = 0;
		Vector cn = Simulator.pbm.getChildrenVar(dpnode);
		for(int i=0; i < cn.size() ; i++){ 		//for children of dpnode
		
			Variable nextChild = (Variable)(cn.get(i));
			
			int childThreshold = Calculate(nextChild);
			
			for(int j=0; j< nextChild.neighbors.size();j++){ //for ancestors of nextChild
				
				//childNeighbor = neighbor of nextChild		
				Variable childNeighbor = ((Variable)nextChild.neighbors.elementAt(i));
				
				if (!Simulator.pbm.pOrderTree.isDescendent(nextChild,childNeighbor)){
					
					
					Variable childAncestor = childNeighbor; 
					int min = -1;
					for (int k=0; k	< nextChild.domainSize ; k++){
						
						//int min2 = -1;
						for(int l=0; l < childAncestor.domainSize; l++)	{
						
							int varCode = 0;
							int var1 = nextChild.varIDof();
							int var2 = childAncestor.varIDof();
							if(var1 > var2) varCode = 1000*var1 + var2; 
							else varCode = 1000*var2 + var1;
							
							MaxCOPConstraint valConstraint;		
							valConstraint = (MaxCOPConstraint)MaxCOPProblem.constraints.get(new Integer(varCode));
							if(valConstraint != null){	
					
								Hashtable valHashTable = new Hashtable();
								valHashTable = valConstraint.nogoods;
								MaxCOPConstraint.nogood val12Cost;
														
								if(var2 > var1)
								val12Cost = ((MaxCOPConstraint.nogood)valHashTable.get(new Integer(1000*k + l))); 				
								else
								val12Cost = ((MaxCOPConstraint.nogood)valHashTable.get(new Integer(1000*l + k))); 				
						
								if (min	== -1)
									min = val12Cost.cost;
								else if(min > val12Cost.cost)
							 		min = val12Cost.cost;				
							}
							else min = 0;
						}
				
					}
					
					
					Threshold += min;
					
				}			
			
			}
		
			Threshold += childThreshold;
			
		}	

		//System.out.print("Initial threshold = "+dpnode.getInitThreshold()+"  " );	
		for(int i = 0; i< dpnode.domainSize; i++)
			dpnode.setInitThreshold(((NominalValue)dpnode.domain[i]),Threshold);
				
		dpnode.setInitThreshold(Threshold);
		System.out.println("Final threshold = "+dpnode.getInitThreshold()+"\n");

		return Threshold;
	}
	
		
	public void preprocess()
	{
		
		System.out.println("Executing Experimental DP JON");
			
		Variable Root = Simulator.pbm.pOrderTree.getRootVariable();
		//System.out.println("Root: "+ Root.varIDof() + " Value: "+i);
		Calculate(Root); 
		
		
	
			
	}			
		
		
}	