/**********************************************************************
 * File Name: PriorityNode.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Apr 18 2002
 *
 * Implements a node for the PriorityTree data structure.
 **********************************************************************/

package adopt.problem;

import adopt.common.*;

import java.lang.*;
import java.io.*;
import java.util.*;

/* each node has a Variable and associated Integer */
public class PriorityNode{
  Variable v;
  int num;

  public PriorityNode(Variable v1, int num1){
    v = v1;
    num = num1;
  }

  public String toString(){
    StringBuffer sb = new StringBuffer();
    sb.append("(" + v.uniqueNameOf() + ",");
    sb.append(" " + v.getInitThreshold() + ")");
    return sb.toString();
  }

  public static PriorityNode parseString(String str, Problem pbm){
    StringTokenizer stok = new StringTokenizer(str, ",");
    String s = stok.nextToken();
    Variable v1 = pbm.getVariableFromUniqueVarName(s);
    s = stok.nextToken().trim();
    int num = (new Integer(s)).intValue();
    PriorityNode result = new PriorityNode(v1,num);
    return result;
  }
}

