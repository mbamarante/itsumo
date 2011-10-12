package adopt.problem;

import adopt.solver.*;
import adopt.common.*;


import java.io.*;
import java.util.*;
import java.net.*;

public  class dp2 
{
	

int Calculate(Variable dpnode, int dpvalue)
{
	
	int Threshold = 0;
	
	Vector cn = Simulator.pbm.getChildrenVar(dpnode);
	for(int i=0; i < cn.size() ; i++)
	{ 
		
		Variable nextChild = (Variable)(cn.get(i));
		int min1 = -1; 
		for(int j=0; j < nextChild.domainSize; j++)
		{
	
			int tempThresh = 0;	
				
			int currentValue = 0; 
			Value val = new NominalValue(j);
			
			
			if(nextChild.thresholdCalculated[j]==true)
				currentValue = nextChild.getInitThreshold(val);
			else
				currentValue = Calculate(nextChild, j);
						  	
		  	int varCode = 0;
			int var1 = nextChild.varIDof();
			int var2 = dpnode.varIDof();
			if(var1 > var2) varCode = 1000*var1 + var2; 
			else varCode = 1000*var2 + var1;
				
			MaxCOPConstraint valConstraint;		
			valConstraint = (MaxCOPConstraint)MaxCOPProblem.constraints.get(new Integer(varCode));
			Hashtable valHashTable = new Hashtable();
			
			if(valConstraint != null)
			{	
				
				
				valHashTable = valConstraint.nogoods;
			
			MaxCOPConstraint.nogood val12Cost;
			
			if (var2 > var1)
			val12Cost = ((MaxCOPConstraint.nogood)valHashTable.get(new Integer(1000*j + dpvalue))); 				
			else 
			val12Cost = ((MaxCOPConstraint.nogood)valHashTable.get(new Integer(1000*dpvalue+j))); 					
			tempThresh = currentValue + val12Cost.cost; 
			
				
			for(int k=0; k< nextChild.neighbors.size();k++) //for ancestors of nextChild except parent
			{   
				
			//childNeighbor = neighbor of nextChild
				Variable childNeighbor = ((Variable)nextChild.neighbors.elementAt(k));
				
				boolean isParent = false; 
				Variable childParent = (Simulator.pbm.pOrderTree.getParent(nextChild)).getRootVariable();
				
				if(childParent.varIDof() == childNeighbor.varIDof()) isParent = true;
				else isParent = false;
				
				if (!Simulator.pbm.pOrderTree.isDescendent(nextChild,childNeighbor) && 
					!isParent)
				{
					
					Variable childAncestor = childNeighbor;
					int min2 = -1;  			
					
					
					for(int l=0; l< childAncestor.domainSize;l++)
					{
						
						
							
							int varCode1 = 0;
							int var3 = childAncestor.varIDof();
							if(var1 > var3)
								varCode1 = 1000*var1 + var3; 
							else
								varCode1 = 1000*var3 + var1;
							MaxCOPConstraint valConstraint1;
							valConstraint1 = (MaxCOPConstraint)MaxCOPProblem.constraints.get(new Integer(varCode1));
							Hashtable valHashTable1 = new Hashtable();
							valHashTable1 = valConstraint1.nogoods;
							MaxCOPConstraint.nogood val23Cost;
						
							if(var1 > var3)					
							val23Cost = ((MaxCOPConstraint.nogood)valHashTable1.get(new Integer(1000*l + j))); 				
							else 
							val23Cost = ((MaxCOPConstraint.nogood)valHashTable1.get(new Integer(1000*j + l))); 				
							
							//	System.out.print(childNeighbor.varIDof()); 
							//	System.out.println ("Cost of " + k + ":" + l + "=" + val23Cost.cost);
								if (min2	== -1)
								min2 = val23Cost.cost;
								else if(min2 >val23Cost.cost)
							 	min2 = val23Cost.cost;				
						
					}
					
					tempThresh += min2; 
				}
			}
			
			if(min1 == -1) min1 = tempThresh; 
					else if(min1 > tempThresh) min1 = tempThresh; 
		}
		else
			{
			min1 = 0;
			System.out.println(" Node "+var1+" "+var2);
			}
		}
		
		Threshold += min1;	
	}

	 
	NominalValue val = new NominalValue(dpvalue);
	String inter = val.value;
	Integer stringToInt = new Integer(inter);
//	System.out.println("Variable="+dpnode.varIDof()+" Value = "+dpvalue+" Threshold = "+Threshold);
	dpnode.setInitThreshold(val,Threshold);
	return(Threshold);
			
}


	public void preprocess()
	{
	
		System.out.println("Executing DP2 ...");		
		Variable Root = Simulator.pbm.pOrderTree.getRootVariable();
		for(int i=0; i< Root.domainSize;i++) Calculate(Root,i); 
		//System.out.println("Root: "+ Root.varIDof() + " Value: "+i);
		
			
		Enumeration e ;
		e = MaxCOPProblem.constraints.elements();
		Enumeration nvars;
		nvars = Simulator.pbm.vars.elements();
		for(; nvars.hasMoreElements(); )  //for all nodes
		{
			int min = -1;
			Variable nextVariable = (Variable)nvars.nextElement();	
			    System.out.print("Variable :"+nextVariable.varIDof());
			for(int i=0; i < nextVariable.domainSize; i++)			
			{

				NominalValue setValue = new NominalValue(i);
				//System.out.println(i);
				int VarThresh = (int)(nextVariable.getInitThreshold(setValue));
				    System.out.print(" "+ i + " = " + VarThresh);
				if (min == -1) 
					min = VarThresh;
				else if (min > VarThresh) 
						min = VarThresh;
				
			}
			
			nextVariable.setInitThreshold(min);
			    System.out.println();
		
		}		 

	}
}



