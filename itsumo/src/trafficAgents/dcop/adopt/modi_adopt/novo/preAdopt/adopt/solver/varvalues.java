package adopt.solver;

import adopt.common.*;
import adopt.problem.*;

import java.lang.*;
import java.io.*;
import java.util.*;


public class varvalues{
	
	Vector Variables;
	Vector Values; 
	
	
	public varvalues()
	{
		Variables = new Vector; 
		Values = new Vector;
		
	}
		
	public Value getValue(Variable v)
	{
		int i;
	    i = indexOf(v);
	    if(i == -1){
	      return null;
	    }
	    else return (Value) Values.elementAt(i);
	}	
			
	
	private int indexOf(Variable v)
	{
	    int i=0;
	    for(i=0;i<Variables.size();i++){
	      Variable v2 = (Variable)Variables.elementAt(i);
	      if(v2.equalVar(v))
		return i;
	    }
	    return -1;
	  }
}