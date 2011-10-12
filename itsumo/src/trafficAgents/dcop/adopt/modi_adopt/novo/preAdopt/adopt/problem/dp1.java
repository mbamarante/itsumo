package adopt.problem;

import adopt.solver.*;
import adopt.common.*;

import java.io.*;
import java.util.*;
import java.net.*;

public  class dp1 
{
		
	int Calculate(Variable dnode, int dpvalue)
	{
	Variable dpnode = dnode;
	int Threshold = 0;
	Vector cn = Simulator.pbm.getChildrenVar(dpnode);
	for(int i=0; i < cn.size() ; i++)
	{ 
		
		int min = -1; 
		Variable nextChild = (Variable)(cn.get(i));
		for(int j=0; j < nextChild.domainSize; j++)
		{
				
				int currentValue = 0;
				Value val = new NominalValue(j);
				if(nextChild.thresholdCalculated[j] == true)
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
				if(valConstraint != null)
				{	
					Hashtable valHashTable = new Hashtable();
					valHashTable = valConstraint.nogoods;
												
					MaxCOPConstraint.nogood val12Cost;
					if(var2 > var1)				
					val12Cost = ((MaxCOPConstraint.nogood)valHashTable.get(new Integer(1000*j+ dpvalue))); 				
					else 
					{   
					val12Cost = ((MaxCOPConstraint.nogood)valHashTable.get(new Integer(1000*dpvalue+ j))); 				
					}
					if (min	== -1)
						min = currentValue + val12Cost.cost;
					else if(min>(currentValue + val12Cost.cost))
					 	min = currentValue + val12Cost.cost;
					 	
			
				}
			
				else
				{
				min = 0;
				}
			}
			Threshold += min;

			
			
		
	}
		    //System.out.println("Variable: " + dpnode.varIDof() + " Value: "+ dpvalue + "threshold = "+Threshold);
			NominalValue val = new NominalValue(dpvalue);
			String inter = val.value;
			Integer stringToInt = new Integer(inter);
			dpnode.setInitThreshold(val,Threshold);
		//	System.out.println("HELLO");
			
			
			return(Threshold);
			
	}
	
	
		
				  
	public void preprocess()
	{
		
		System.out.println("Executing Dp1 ....");
		
		
		Variable Root = Simulator.pbm.pOrderTree.getRootVariable();
		for(int i=0; i< Root.domainSize;i++) 
			{
			//System.out.println("Root: "+ Root.varIDof() + " Value: "+i);
			Calculate(Root,i); 
			}
		
		Enumeration e ;
		e = MaxCOPProblem.constraints.elements();
		Enumeration nvars;
		nvars = Simulator.pbm.vars.elements();
		for(; nvars.hasMoreElements(); )  //for all nodes
			{
			int min = -1;
			Variable nextVariable = (Variable)nvars.nextElement();	
			for(int i=0; i < nextVariable.domainSize; i++)			
				{
				
				NominalValue setValue = new NominalValue(i);
				//System.out.println(i);
				int VarThresh = (int)(nextVariable.getInitThreshold(setValue));
				if (min == -1) 
					min = VarThresh;
				else if (min > VarThresh) 
						min = VarThresh;
				
				}
			
		nextVariable.setInitThreshold(min);
			System.out.println("Variable" + nextVariable.varIDof() + "threshold: " + min);
		}		 
				
		//	System.out.println("Final threshold = "+nextVariable.getInitThreshold()+"\n");
		//}
/*			
			
			
			int finalThreshold = 0;
			//dpnode = node for which threshold is calculated
			Variable dpnode =  (Variable)nvars.nextElement();
			Vector cn = Simulator.pbm.getChildrenVar(dpnode);
			
			//nchildren = list of children of dpnode
			Enumeration nchildren = cn.elements();
	//		System.out.print(dpnode.varIDof());
			
						
		
			for(; nchildren.hasMoreElements();)  //for the children of dpnode
			{ 	
		
			
				//System.out.print("  Ancestors of its children: ");
				//nextChild = The next child of dpnode  
				Variable nextChild = (Variable)nchildren.nextElement();
			
				//System.out.print(nextChild.varIDof());
		
				for(int i=0; i< nextChild.neighbors.size();i++) //for ancestors of nextChild
				{   
			
		  		//childNeighbor = neighbor of nextChild
				Variable childNeighbor = ((Variable)nextChild.neighbors.elementAt(i));
			
				if (!Simulator.pbm.pOrderTree.isDescendent(nextChild,childNeighbor))
				{
				Variable childAncestor = childNeighbor; 
				int min = -1;
			
					for (int k=0; k	< nextChild.domainSize ; k++)
						{
						for(int l=0; l < childAncestor.domainSize; l++)		
						{
					
						int varCode = 0;
						int var1 = nextChild.varIDof();
						int var2 = childAncestor.varIDof();
						if(var1 > var2)
						varCode = 1000*var1 + var2; 
						else
						varCode = 1000*var2 + var1;
						MaxCOPConstraint valConstraint;		
						valConstraint = (MaxCOPConstraint)MaxCOPProblem.constraints.get(new Integer(varCode));
						Hashtable valHashTable = new Hashtable();
						valHashTable = valConstraint.nogoods;
						MaxCOPConstraint.nogood val12Cost;
				
						val12Cost = ((MaxCOPConstraint.nogood)valHashTable.get(new Integer(1000*k + l))); 				
				//		System.out.print(childNeighbor.varIDof()); 
				//		System.out.println ("Cost of " + k + ":" + l + "=" + val12Cost.cost);
						if (min	== -1)
						min = val12Cost.cost;
						else if(min>val12Cost.cost)
					 	min = val12Cost.cost;				
						}
						}
					
				finalThreshold += min;
						
			}
		
		
			}
		
			}	
			System.out.print("Initial threshold = "+dpnode.getInitThreshold()+"  " );	
			dpnode.setInitThreshold(finalThreshold);
		
			System.out.println("Final threshold = "+dpnode.getInitThreshold()+"\n");
			
			
		}			
		
		
	for( ; e.hasMoreElements(); )
		{
		 	
		temp = (MaxCOPConstraint)( e.nextElement());
		Hashtable temp2 = new Hashtable();
		temp2 = temp.nogoods;
		nvar1 = temp.var1.varIDof();
		nvar2 = temp.var2.varIDof(); 
		System.out.println ("Variables " + nvar1 + " - " + nvar2); 
		Enumeration e2 = temp2.elements();
		
		for(;e2.hasMoreElements();) 
		{
			MaxCOPConstraint.nogood x;
			x = (MaxCOPConstraint.nogood)(e2.nextElement());
			
			System.out.print(x.val1.toString()+"-");
			System.out.print(x.val2.toString()+"=");
			System.out.println(x.cost);
			
			
			
		}	
	
		
		}
		
		
	*/	
		
		
		
	}
}

		
		
