/**********************************************************************
 * File Name: Context.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Aug 27 2001
 *
 * Data structure for holding a set of variable/value pairs 
 **********************************************************************/

package adopt.problem;

import adopt.common.*;

import java.lang.*;
import java.io.*;
import java.util.*;

public class Context{

  /* A Context holds a set of variable/value pairs */

  /* val[i] holds value of variable vars[i] */
  public Vector vars;
  public Vector vals;

  public Context(){
    vars = new Vector();
    vals = new Vector();
  }

  public Context(Context v){
    vars = (Vector) v.vars.clone();
    vals = (Vector) v.vals.clone();
  }

  /* return a new Context that is 'this' set intersect the variables
     in given Context. */
  public Context setIntersection(Context vw){
    Context result = new Context();
    for(int i=0;i<vars.size();i++){
      Variable v = (Variable) vars.elementAt(i);
      if(vw.valueOf(v) != null)
	result.addVarToContext(v, valueOf(v));
    }
    return result;
  }

  /* return a new Context that is 'this' set minus the variables
     in given Context. */
  public Context setMinus(Context vw){
    if(!vw.subsetOf(this)){
      System.out.println("Invalid call to Context.setMinus()!");
      System.out.println("  this: " + this.toString());
      System.out.println("  vw: " + vw.toString());
      System.exit(0);
    }
    
    Context result = new Context();
    for(int i=0;i<vars.size();i++){
      Variable v = (Variable) vars.elementAt(i);
      if(vw.valueOf(v) == null)
	result.addVarToContext(v, valueOf(v));
    }
    return result;
  }

  /* return a new Context that is the set union of 'this' and the 
     given Context. */
  public Context union(Context vw){
    Context result = new Context(vw);

    for(int i=0;i<vars.size();i++){
      Variable v = (Variable) vars.elementAt(i);
      /* if v is not already in result */
      if(result.valueOf(v) == null)
	{
	  result.vars.addElement(v);
	  result.vals.addElement(vals.elementAt(i));
	}
    }
    return result;
  }

  public int size(){
    return vars.size();
  }

  private int indexOf(Variable v)
  {
    int i=0;
    for(i=0;i<vars.size();i++){
      Variable v2 = (Variable)vars.elementAt(i);
      if(v2.equalVar(v))
	return i;
    }
    return -1;
  }

  /* is 'this' Context a subset of the given one? */
  public boolean subsetOf(Context vw){

    /* every variable in this Context must have same value in the
       given Context */
    for(int i=0;i<vars.size();i++){
      Variable v2 = (Variable) vars.elementAt(i);
      if( valueOf(v2) != vw.valueOf(v2))
	return false;
    }
    return true;
  }

  public boolean equals(Context vw){
    return (subsetOf(vw) && vw.subsetOf(this));
  }

  /* is 'this' Context compatible with the given one? */
  public boolean compatible(Context vw){
    /* every variable in this Context must have same value in the
       given Context or not exist. */
    for(int i=0;i<vars.size();i++){
      Variable v2 = (Variable) vars.elementAt(i);
      Value val2 = vw.valueOf(v2);
      if(val2 != null && !valueOf(v2).equal(val2)){
	return false;
      }
    }
    return true;
  }

  public void addVarToContext(Variable v, Value val){
    int i;
    i = indexOf(v);
    if (i == -1 ){
      vars.add(v);
      vals.add(val);
    }
    else{
      vals.setElementAt(val,i);
    }
  }

  public Variable getVariable(int idx){
    if(idx > vars.size())
      return null;
    return (Variable) vars.elementAt(idx);
  }

  public Value valueOf(Variable v){
    int i;
    i = indexOf(v);
    if(i == -1){
      //System.out.println("sumit & Rahul  " );
      return null;
    }
    else
      return (Value) vals.elementAt(i);
  }

  public String toString(){

    StringBuffer sb = new StringBuffer();
    sb.append("{ ");
    for(int i=0;i<vars.size();i++){
      Variable v = (Variable) vars.elementAt(i);
      Value val = valueOf(v);
      sb.append(v.uniqueNameOf()).append(" ").append(val.toString());
      if( i+1 < vars.size())
	sb.append(", ");
      else
	sb.append(" ");
    }
    sb.append("} ");

    return sb.toString();
  }

  public String toMsg(){
    StringBuffer sb = new StringBuffer();
    sb.append(" ");
    for(int i=0;i<vars.size();i++){
      Variable v = (Variable) vars.elementAt(i);
      Value val = valueOf(v);
      sb.append(v.uniqueNameOf()).append(" ").append(val.toString());
      if( i+1 < vars.size())
	sb.append(" ");
    }
    return sb.toString();
  }
    
//   public View contextToView(){

//     View result = new View();
//     result.vars = (Vector) vars.clone();
//     result.vals = (Vector) vals.clone();
//     result.delta_hat = 0;
//     return result;
//   }


  public static void main(String args[]){
    
  }    
}
  
