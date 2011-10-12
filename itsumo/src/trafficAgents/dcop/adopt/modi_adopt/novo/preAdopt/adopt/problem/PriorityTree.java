/**********************************************************************
 * File Name: PriorityTree.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Apr 18 2002
 *
 * Implements a tree data structure.
 * Used for a tree priority ordering of variables.
 **********************************************************************/

package adopt.problem;

import adopt.common.*;

import java.lang.*;
import java.io.*;
import java.util.*;

public class PriorityTree{
  /* root node */
  PriorityNode r;
  /* children of r */
  public Vector children;

  /* Class constructor */
  public PriorityTree(Variable v1){
    r = new PriorityNode(v1, 0);
    children = new Vector();
  }

  public PriorityTree(Variable v1, int cost1){
    r = new PriorityNode(v1, cost1);
    children = new Vector();
  }

  public PriorityTree(PriorityNode root){
    r = root;
    children = new Vector();
  }

  public Variable getRootVariable(){
    return r.v;
  }

  /* Traverse the tree until the given variable is found.  Return a
     pointer to the subtree whose root is the parent of the given
     variable */
  public PriorityTree getParent(Variable v){
    PriorityTree result = null;

    for(int i=0;i<children.size();i++){
      PriorityTree st = (PriorityTree) children.elementAt(i);
      if(st.r.v.equalVar(v)){
	result = this;
	break;
      }
      else{
	PriorityTree sst = st.getParent(v);
	if(sst != null){
	  result = sst;
	  break;
	}
      }
    }
    return result;
  }

  /* Traverse the tree until the given variable is found, return its
     associated integer. */
  public int getCost(Variable v){
    int result = -1;
    if (v.equalVar(r.v))
      result = r.num;
    else{
      for(int i=0;i<children.size();i++){
	PriorityTree st = (PriorityTree) children.elementAt(i);
	int c = st.getCost(v);
	if(c >= 0){
	  result = c;
	  break;
	}
      }
    }
    return result;
  }

  public int size(){
    int result = 0;
    /* count root */
    result++;
    /* count num descendents */
    for(int i=0;i<children.size();i++){
      PriorityTree st = (PriorityTree) children.elementAt(i);
      result += st.size();
    }
    return result;
  }

  /* what is the depth of this tree?
   'depth' is the longest path from root to leaf */
  public int depth(){
    int maxd = 0;
    for(int i=0;i<children.size();i++){
      PriorityTree st = (PriorityTree) children.elementAt(i);
      int d = st.depth();
      if(d > maxd)
	maxd = d;
    }
    return maxd+1;
  }

  /* return a pointer to the subtree rooted at the given variable */
  public PriorityTree getSubTree(Variable v){
    if(v.equalVar(r.v))
      return this;
    for(int i=0;i<children.size();i++){
      PriorityTree st = (PriorityTree) children.elementAt(i);
      if(v.equalVar(st.r.v)){
	return st;
      }
      else{
	PriorityTree stt = st.getSubTree(v);
	if(stt != null)
	  return stt;
      }
    }
    return null;
  }
  /* add a branch to 'this' node. */
  public void addSubTree(PriorityTree st){
    if(!consistencyCheck(st))
      {
	System.out.println(" PriorityTree.addSubTree(): Error in Consistency Check!!");
	System.out.println(" " + this.toString());
	System.out.println(" " + st.toString());
	System.exit(0);
      }
    children.add(st);
  }

  public PriorityTree copy() {
	  PriorityTree st = new PriorityTree(r);
	  st.children = (Vector)children.clone();
	  return st;
  }

  /* is the given tree equal to this one?
     Equal: has the same variable/values with the same parent/child relationships.
     (note: costs may be different.)
     */
  public boolean equal(PriorityTree st){
    /* are the roots equal ? */
    if(!r.v.equalVar(st.r.v) ){
      return false;
    }
    if(children.size() != st.children.size())
      return false;
    for(int i=0;i<children.size();i++){
      PriorityTree sst = (PriorityTree) children.elementAt(i);
      int flag = 0;
      for(int j=0;j<st.children.size();j++){
	PriorityTree sst2 = (PriorityTree) st.children.elementAt(j);
	if(sst.equal(sst2)){
	  flag = 1;
	  break;
	}
      }
      if(flag == 0)
	return false;
    }
    return true;
  }

  /* is v in this tree? */
  public boolean isInTree(Variable v){
    if(v.equalVar(r.v)) return true;
    for(int i=0;i<children.size();i++){
      PriorityTree st = (PriorityTree) children.elementAt(i);
      if(st.isInTree(v)) return true;
    }
    return false;
  }

  /* another consistency check method */
  /* is v1 a descendent of v2? */
  public boolean isDescendent(Variable v1, Variable v2){
    boolean result = false;

    if(r.v.equalVar(v1)){
      for(int i=0;i<children.size();i++){
	PriorityTree st = (PriorityTree) children.elementAt(i);
	if(st.isInTree(v2)) result = true;
      }
    }
    else{
      for(int i=0;i<children.size();i++){
	PriorityTree st = (PriorityTree) children.elementAt(i);
	boolean rc = st.isDescendent(v1, v2);
	if(rc == true){
	  result = true;
	  break;
	}
      }
    }
    return result;
  }

  /* is any variable in st already in this tree? */
  public boolean consistencyCheck(PriorityTree st){
    if(isInTree(st.r.v)) return false;
    for(int i=0;i<st.children.size();i++){
      PriorityTree sst = (PriorityTree) st.children.elementAt(i);
      boolean rc = consistencyCheck(sst);
      if(!rc) return false;
    }
    return true;
  }

  public String toString(){
    StringBuffer sb = new StringBuffer();
    sb.append("(" + r.toString());
    for(int i=0;i<children.size();i++){
      PriorityTree st = (PriorityTree) children.elementAt(i);
      sb.append(", " + st.toString());
    }
    sb.append(")");
    return sb.toString();
  }


  public String toDFSString(){
    StringBuffer sb = new StringBuffer();
    sb.append(r.v.varID + "[" + r.v.getInitThreshold() + "]");
    for(int i=0;i<children.size();i++){
      PriorityTree st = (PriorityTree) children.elementAt(i);
      sb.append(", " + st.toDFSString());
    }
    return sb.toString();
  }


  public static PriorityTree parsePriorityTree(String str, Problem pbm){
    PriorityTree result = null ;
    char[] s = str.toCharArray();
    int i = 0;
    while(s[i++] != '(');
    i--;
    int j = findClosingParen(s, i);
    result = parseHelper(s, i, j, pbm);
    return result;
  }

  /* s[idx] should be an open paren.
     returns i such that s[i] is the matching closing paren
     */
  public static int findClosingParen(char[] s, int idx){
    int numopen = 1;
    int numclosed = 0;

    int i = idx+1;
    while(i < s.length){
      char c = s[i++];
      if(c == ')')
	{
	  numclosed++;
	  if (numopen == numclosed)
	    return i-1;
	}
      if(c == '(')
	numopen++;
    }
    return i;
  }


  /* return the PriorityTree from s[idx] to s[idx2]
     s[idx] should be the opening paren of a PriorityTree.
     s[idx2] should be the closing paren of a PriorityTree.
   */
  public static PriorityTree parseHelper(char[] s, int idx, int idx2, Problem pbm){
    PriorityTree result = null ;
    int i = idx;
    int j = idx2;

    Utility.Dprint("Processing from " + idx + " to " + j , Utility.TRACE_RIDICULOUS_LEVEL);
    /* start of Prioritytree should be ( */
    if(s[i++] != '('){
      System.out.println("PriorityTree.parseString(): Parse error!!");
      System.exit(0);
    }else{
      /* start of root node should be ( */
      if(s[i++] != '('){
	System.out.println("PriorityTree.parseString(): Parse error!!");
	System.exit(0);
      }else{
	char[] ss = new char[100];
	int ii = 0;
	while(s[i] != ')'){
	  ss[ii] = s[i];
	  ii++;i++;
	}
	ss[ii] = '\0';

	PriorityNode root = PriorityNode.parseString(new String(ss), pbm);
	result = new PriorityTree(root);
	i++;
	/* we are done */
	if(s[i] == ')'){
	  Utility.Dprint(" Returning: " + result.toString() , Utility.TRACE_RIDICULOUS_LEVEL);
	  return result;
	}
	else{
	  /* get to next open paren */
	  while(s[i++] != '(');
	  i--;
	  while(i < j){
	    int jj = PriorityTree.findClosingParen(s, i);
	    PriorityTree st = PriorityTree.parseHelper(s, i, jj, pbm);
	    result.addSubTree(st);
	    i = jj;
	    i++;
	    /* we are done */
	    if(s[i] == ')'){
	      Utility.Dprint(" Returning: " + result.toString() , Utility.TRACE_RIDICULOUS_LEVEL);
	      return result;
	    }
	    else{
	      /* get to next open paren */
	      while(s[i++] != '(');
	      i--;
	    }
	  }
	}
      }
    }
    Utility.Dprint(" Returning: " + result.toString() , Utility.TRACE_RIDICULOUS_LEVEL);
    return result;
  }

  public static void main(String args[]){

  }

}

