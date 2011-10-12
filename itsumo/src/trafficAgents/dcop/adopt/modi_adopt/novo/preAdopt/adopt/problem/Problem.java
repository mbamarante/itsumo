/*
 *    Problem.java
 *    Author: Jay Modi
 *    Date: Aug 28 2001
 *
 *    Specifies a DCO Problem.
 *    Methods for computing local cost (delta), creating different
 *    priority orderings.
 */

package adopt.problem;
import adopt.common.*;

import java.io.*;
import java.util.*;
import java.net.*;

public abstract class Problem {

  /*** Class variables ***/

  String problemName;
  /* list of variables */
  public Vector vars;
  /* list of agents names*/
  public Vector agents;

  /* adjacency matrix for the given constraint graph */
  public int[][] adj;

  /* Represents the maximum cost possible */
  public static int MAX_VALUE = 100000000;
  /* 0: linear ordering using heuristic.
     1: tree ordering using heuristic.
     2: linear ordering based on depth-first traversal of tree from [1].
     3: random linear ordering.
     */
  int _orderSwitch;
  public static int CHAIN = 0;
  public static int TREE = 1;
  public static int DFSCHAIN = 2;
  public static int RANDOMCHAIN = 3;

  public static int sub;
  public static int div;

  /* Chain priority ordering
     -----------------------
     pOrderChain[i] holds the priority of variable i (var.varID == i)
     Note: varIDs must be [0...n]
     if pOrderChain[i] < pOrderChain[j], then j is higher priority than i
     */
  int[] pOrderChain = null;
  /* Tree priority ordering
     ----------------------
     pOrder.getCost() holds the priority of pOrder.root.
   */
  public PriorityTree pOrderTree = null;
  
	//*********List of variables that are not connected to the main root
  public int[] notConnectedVars;
  //********** Maximum cost in DSN domain (Used for calculating accuracy)
  public static int DSNmaxCost;
  /*** End class variables ***/

  /*** Class Methods ***/

  /* Constructor */
  public Problem(String name){
    problemName = name;
    vars = new Vector();
    agents = new Vector();
  }
  /** Abstract Methods **/
  /* evaluate unary constraints */
  public abstract int evaluate(Variable v1, Value val1);
  /* evaluate binary constraints */
  public abstract int evaluate(Variable v1, Value val1, Variable v2, Value val2);
  /* evaluate n-ary constraints */
  public abstract int evaluate(Variable v1, Value val1, Context vw);
  /* return true if priority(v1) > priority(v2), false otherwise */
  public abstract boolean comparePriority(Variable v1,Variable v2);
  /** End abstract methods **/

  /** Static Methods **/
  /* returns a negative integer, zero, or a positive integer as d1
     is less than, equal to, or greater d2.*/
  public static int compareDeltas(int d1, int d2){

    if(d1 == Problem.MAX_VALUE &&
       d2 != Problem.MAX_VALUE)
      /* d1 > d2 */
      return 1;
    else if(d1 != Problem.MAX_VALUE &&
	    d2 == Problem.MAX_VALUE)
      /* d2 < d1 */
      return -1;
    else if(d1 == d2)
      return 0;
    else{
      if(d1 > d2)
	return 1;
      else  /* d2 > d1 */
	return -1;
    }
  }

  /* return d1 - d2 */
  public static int subDeltas(int d1, int d2){
    /* inf - inf = 0 */
    if(d1 == Problem.MAX_VALUE &&
       d2 == Problem.MAX_VALUE)
      return 0;
    /* inf - n = inf */
    if(d1 == Problem.MAX_VALUE ||
       d2 == Problem.MAX_VALUE)
      return Problem.MAX_VALUE;
    else
      return d1 - d2;
  }

  /* return d1 + d2 */
  public static int sumDeltas(int d1, int d2){
    if(d1 == Problem.MAX_VALUE ||
       d2 == Problem.MAX_VALUE)
      return Problem.MAX_VALUE;
    else
      return d1+d2;
  }
  /** End static methods **/


  /* DELTA(): Computes local cost delta(v1) with respect to all
     variables in given context */
  public int delta(Variable v1, Context vw){
    /*debug*/Utility.Dprint("Entering Problem.delta():....", Utility.TRACE_RIDICULOUS_LEVEL);
    int delta = 0;
    /*debug*/    Utility.Dprint("    Variable: " + v1.uniqueNameOf(), Utility.TRACE_RIDICULOUS_LEVEL);
    /*debug*/    Utility.Dprint("    Context: " + vw.toString(), Utility.TRACE_RIDICULOUS_LEVEL);
    Value val1 = vw.valueOf(v1);
    if(val1 == null){
      Utility.Dprint("Problem.delta(): Unknown variable", Utility.DEBUG);
      System.exit(0);
    }
    /* evaluate unary constraint*/
    delta = sumDeltas(delta, evaluate(v1,val1));

    /* evaluate binary constraints*/
    Variable v2;
    for(int i=0;i<vars.size();i++){
      v2 = (Variable) vars.elementAt(i);
      Value val2 = vw.valueOf(v2);
      if(val2 != null){
	if(connected(v1, v2)){
	  int rc = evaluate(v1, val1, v2, val2);
	  delta = sumDeltas(delta, rc);
	}
      }
    }
    /* evaluate nary constraints */
    delta = sumDeltas(delta, evaluate(v1,val1, vw));
    /*debug*/    Utility.Dprint("...leaving Problem.delta() " + delta,
				Utility.TRACE_RIDICULOUS_LEVEL);
    return delta;
  }


  /* set agents field */
  public void setAgents(Vector agnts){
    agents = agnts;
  }

  /*** Accessor methods ***/
  public int numAgents(){
    return agents.size();
  }
  public int numVars(){
    return vars.size();
  }
  /* return variable in this problem with given uniqueName */
  public Variable getVariableFromUniqueVarName(String uname){

    for(int i=0;i<vars.size();i++) {
      Variable v2 = (Variable) vars.elementAt(i);
      if(v2.isUniqueNameOf(uname))
	return v2;
    }

    /* debug */ Utility.Dprint("Problem.getVariableFromUniqueVarName(): Unknown variable: " + uname,
			       Utility.TRACE_RIDICULOUS_LEVEL);
    return null;
  }
  /* are the given variables connected by a constraint? */
  public boolean connected(Variable q1, Variable q2){
    return (q1.isNeighbor(q2) && q2.isNeighbor(q1));
  }
  /* return the variables that are connected to the given one*/
  public Vector getLinks(Variable v){
    Utility.Dprint("Entering getLinks()...", Utility.TRACE_RIDICULOUS_LEVEL);
    Utility.Dprint(" v = " + v.uniqueNameOf(), Utility.TRACE_RIDICULOUS_LEVEL);
    Vector result = new Vector();
    /* find neighboring variables */
    for(int j=0;j<vars.size();j++){
      Variable v2 = (Variable) vars.elementAt(j);
      if(connected(v,v2)){
	/* record the variable */
	result.add(v2);
      }
    }
    for(int i=0;i<result.size();i++){
      Variable v2 = (Variable) result.elementAt(i);
      Utility.Dprint(" v2 = " + v2.uniqueNameOf(), Utility.TRACE_RIDICULOUS_LEVEL);
    }
    Utility.Dprint("...leaving getLinks()", Utility.TRACE_RIDICULOUS_LEVEL);
    return result;
  }
  /* return variable in this problem with given varID */
  public Variable getVariableFromID(int v1){
    for(int j=0;j<vars.size();j++){
      Variable v = (Variable) vars.elementAt(j);
      if(v.varID == v1){
        return v;
      }
    }
    return null;
  }
  /* return variables owned to given agent */
  public Vector getVariablesFromAgentID(int agentID){
    /* debug */ Utility.Dprint("Entering getVariablesFromAgentID("+agentID+")...",
			       Utility.TRACE_RIDICULOUS_LEVEL);
    /* variables owned by given agent */
    Vector myVars = new Vector();
    for(int i=0;i<vars.size();i++){
      Variable v = (Variable) vars.elementAt(i);
      if(v.agentIDof() == agentID)
	myVars.add(v);
    }
    /* debug */ Utility.Dprint("...leaving getVariablesFromAgentName()  "+myVars.size(),
			       Utility.TRACE_RIDICULOUS_LEVEL);
    return myVars;
  }
  /* return all children of given variable */
  public Vector getChildrenVar(Variable v){
    Vector result = new Vector();
    if(isChainOrder()){
      Variable vv = nextVariable(v);
      if(vv != null)
	result.add(vv);
    }
    else{
      PriorityTree t = pOrderTree.getSubTree(v);
      if(t != null){
	for(int i=0;i<t.children.size();i++){
	  PriorityTree st = (PriorityTree) t.children.elementAt(i);
	  Variable vv = st.getRootVariable();
	  result.add(vv);
	}
      }
    }
    return result;
  }
  /*** End accessor methods ***/


  /*** Methods for creating/accessing variable ordering ***/
  public boolean isTreeOrder(){
    if(_orderSwitch == TREE){
      return true;
    }
    return false;
  }
  public boolean isChainOrder(){
    if(_orderSwitch == CHAIN ||
       _orderSwitch == DFSCHAIN ||
       _orderSwitch == RANDOMCHAIN
       ){ return true; }
    else if(_orderSwitch == TREE)
      return false;
    else{
      System.out.println("Error in Problem.isChainOrder()..." +
			 "unknown _orderSwitch!!");
      System.exit(0);
    }
    return false;
  }

  /* if a tree-order, return the depth of the tree */
  public int orderDepth(){
    if(_orderSwitch != TREE){
      System.out.println("Error in Problem.treeDepth()..." +
			 "current order is not a tree!!");
      System.exit(0);
    }
    return pOrderTree.depth();
  }

  /* return the priority number of given variable */
  public int priorityOf(Variable v){
    if(_orderSwitch == TREE){
      return pOrderTree.getCost(v);
    }
    else if(isChainOrder()){
      return pOrderChain[v.varID];
    }
    else{
      System.out.println("Problem(): Unknown _orderSwitch: " + _orderSwitch);
      System.exit(0);
    }
    return -1;
  }
  /* write the priority ordering to a log file */
  public void logPriorityOrder(Logger nLog){

    nLog.printToLogln("\nPriority Order");
    nLog.printToLogln("--------------------");

    if(isTreeOrder()){
      nLog.printToLogln(pOrderTree.toString());
      nLog.printToLogln("depth = " + orderDepth());
    }
    for(int j=0;j<vars.size();j++){
      Variable v1 = (Variable) vars.elementAt(j);
      nLog.printToLogln(v1.uniqueNameOf() + " " + priorityOf(v1));
    }
  }

  /* return the variable that is immediately previous (higher) in
     priority order to the given one -- i.e., its parent.  */
  public Variable previousVariable(Variable v){
    if(_orderSwitch == TREE){
      PriorityTree st = pOrderTree.getParent(v);
      if(st == null)
	return null;
      else{
	Variable result = st.getRootVariable();
	return result;
      }
    }
    else{
      Variable minPrtyVar = null;
      for(int i=0;i<vars.size();i++){
	Variable vvar = (Variable) vars.elementAt(i);
	/* we are trying to find the highest lower priority variable */
	if(comparePriority(vvar, v) && (minPrtyVar == null ||
					comparePriority(minPrtyVar, vvar))){
	  minPrtyVar = vvar;
	}
      }
      return minPrtyVar;
    }
  }

  /* if a chain order, return the variable that is next (lower) in
     priority order to the given one */
  public Variable nextVariable(Variable v){
    if(!isChainOrder()){
      return null;
    }
    Variable maxPrtyVar = null;
    for(int i=0;i<vars.size();i++){
      Variable vvar = (Variable) vars.elementAt(i);
      if(comparePriority(v, vvar) && (maxPrtyVar == null ||
				      comparePriority(vvar, maxPrtyVar))){
	maxPrtyVar = vvar;
      }
    }
    return maxPrtyVar;
  }

  /* Convert a tree-order into a chain-order through a depth-first traversal of the tree */
  public void convertToChain(){
    if(_orderSwitch != TREE){
      System.out.println("Error in Problem.convertToChain()..." +
			 "converting non-tree to chain!!");
      System.exit(0);
    }
    else{
      pOrderChain = convertToChainHelper(pOrderTree);
      _orderSwitch = DFSCHAIN;
      pOrderTree = null;
      Utility.Dprint("  Priorities: ", Utility.MSG_LEVEL2);
      for(int j=0;j<vars.size();j++){
	Variable v1 = (Variable) vars.elementAt(j);
	Utility.Dprint(v1.varID + " " + priorityOf(v1), Utility.MSG_LEVEL2);
      }
    }
  }
  public int[] convertToChainHelper(PriorityTree st){
    int[] map2 = new int[vars.size()];
    int i=vars.size()-1;
    PriorityTree tOrder = st;
    String s = tOrder.toDFSString();
    StringTokenizer stok = new StringTokenizer(s, ",");
    while(stok.hasMoreTokens()){
      String ss = stok.nextToken().trim();
      int varID = (new Integer(ss)).intValue();
      map2[varID] = i--;
    }
    return map2;
  }

  public void calculateVarThresholds(PriorityTree st,int[][] constraintValues) {
	  for(int i = 0;i < constraintValues.length;i++) {
		  for(int j = i+1;j < constraintValues.length;j++) {
			  if(constraintValues[i][j] != 0) {
				  Variable var1 = (Variable)vars.elementAt(i);
				  Variable var2 = (Variable)vars.elementAt(j);
				  if(st.isDescendent(var2,var1)) {
					  var1.setInitThreshold(var1.getInitThreshold() + constraintValues[i][j]);
				  }
				  else if(st.isDescendent(var1,var2)) {
					  var2.setInitThreshold(var2.getInitThreshold() + constraintValues[i][j]);
				  }
			  }
		  }
	  }

	  for(int i = 0;i < vars.size();i++) {
		  System.out.println("Var " + i + " = " + ((Variable)vars.elementAt(i)).getInitThreshold());
	  }
  }

  public int calculateThresholds(PriorityTree st) {
	  //System.out.println(st.toString());
	  if(st.children.size() == 0)
	  {
		  return st.getRootVariable().getInitThreshold();
	  }
	  else
	  {
		  int total = st.getRootVariable().getInitThreshold();
		  for(int i = 0;i < st.children.size();i++)
		  {
			  total += calculateThresholds((PriorityTree)st.children.elementAt(i));
		  }
		  st.getRootVariable().setInitThreshold(total);
		  return total;
	  }
  }

  public void getAdjacencyMatrix() {
	  adj = new int[vars.size()][vars.size()];
	  for(int i = 0;i < vars.size();i++)
	  {
		  Variable var1 = (Variable)vars.elementAt(i);
		for(int j = 0;j < vars.size();j++)
		{
			Variable var2 = (Variable)vars.elementAt(j);
			if(i == j)
				adj[i][j] = 0;
			else if(connected(var1,var2)) {
				adj[i][j] = 1;
			}
		}
	  }
  }

  /*public PriorityTree priorityTree31() {
	getAdjacencyMatrix();
	int v[] = new int[vars.size()];
	int minDepth = 1000;
	PriorityTree[] minDepthTrees = new PriorityTree[100];
	int z = 0;

	for(int i = 0;i < vars.size();i++) {
		for(int j = 0;j < vars.size();j++)
			v[j] = 1;
	//Vector trees = getTrees(getMidLongestShortestPath(v),v);
	Vector trees = getTrees(i,v);
		for(int k = 0;k < trees.size();k++) {
			PriorityTree pOrderTree = (PriorityTree)trees.elementAt(k);
			if(pOrderTree.depth() <= minDepth) {
				minDepth = pOrderTree.depth();
				minDepthTrees[z] = pOrderTree;
				z++;
			}
		}
	//}
	System.out.println("Minimum depth is " + minDepth);
	Random rand = new Random();
	int random = rand.nextInt(z);
	while(minDepthTrees[random].depth() != minDepth) {
		random = rand.nextInt(z);
	}
	return new PriorityTree((Variable)vars.elementAt(0));
  }*/


  public PriorityTree priorityTree() {
	getAdjacencyMatrix();
	int v[] = new int[vars.size()];
	int z = 0;

	for(int j = 0;j < vars.size();j++)
			v[j] = 1;
	System.out.println("Midpoint is " + getMidLongestShortestPath(v));
	Vector trees = getTrees(getMidLongestShortestPath(v),v);

	PriorityTree order = (PriorityTree)trees.elementAt(0);
	System.out.println("Minimum depth is " + order.depth());
	return order;
  }

  private int removeZeroes(int[][] w, int noOfPartitions) {
	  for(int j = 0;j <= noOfPartitions;j++) {
		  int counter = 0;
		  for(int i = 0;i < vars.size();i++) {
			if(w[i][j] == 1) {
				counter = 1;
				break;
			}
		  }
		  if(counter == 0) {
			for(int k = j;k < noOfPartitions;k++) {
		  		for(int i = 0;i < vars.size();i++) {
					w[i][k] = w[i][k+1];
		  		}
			}
		  	j--;
		  	noOfPartitions--;
		  }
	  }
	  return noOfPartitions;
  }

  public int findCluster(int var,int[][] w,int noOfPartitions) {
	  Variable v = (Variable)vars.elementAt(var);
	  int howMany = -1; //first matched cluster
	  int newNoOfPartitions = noOfPartitions;

	  for(int j = 0;j <= noOfPartitions;j++) {
		  for(int i = 0;i < vars.size();i++) {
			  if(w[i][j] == 1) {
				  Variable varClus = (Variable)vars.elementAt(i);
				  if(connected(varClus,v) && howMany == -1 && i != var) {
					  w[var][j] = 1;
					  howMany = j;
					  break;
				  }
				  else if(connected(varClus,v) && howMany != -1 && i != var) {
					  for(int k = 0;k < vars.size();k++) {
						  if(w[k][j] == 1)
						  {
							w[k][howMany] = 1;
							w[k][j] = 0;
						  }
					  }
					  newNoOfPartitions--;
					  break;
				  }
			  }
		  }
	  }
	  removeZeroes(w,noOfPartitions);


	  if(howMany == -1) {
		  w[var][++newNoOfPartitions] = 1;
	  }
	  return newNoOfPartitions;
  }

  private void printArray(int v[]) {
	  for(int i = 0;i < v.length;i++) {
		  System.out.print(i + "--> " + v[i] + ";");
	  }
	  System.out.println("");
  }

  private void printDoubleArray(int w[][]) {
	  System.out.println("Printing w");
	  for(int i = 0;i < w.length;i++) {
		  printArray(w[i]);
	  }
	  System.out.println("");
  }

  public int getPartitions(int v[],int[][] w) {
	  int noOfPartitions = -1;
	  for(int i = 0;i < v.length;i++) {
		  if(v[i] != 0) {
			  noOfPartitions = findCluster(i,w,noOfPartitions);
		  }
	  }
	  return noOfPartitions;
  }

  public void getCurrentV(int[][] w,int partNo,int[] v) {
	  for(int i = 0;i < v.length;i++) {
		  v[i] = w[i][partNo];
	  }
  }

  public int sum(int[] v) {
	  int sum = 0;
	  for(int i = 0;i < v.length;i++) {
		  sum += v[i];
	  }
	  return sum;
  }

  public int getIndex(int[] v) {
	  for(int i = 0;i < v.length;i++) {
		  if(v[i] == 1)
		  	   return i;
	  }
	  return -1;
  }

  public void getChildren(int parent,int[] v,int[] children) {
	  int k = 0;
	  for(int i = 0;i < v.length;i++)
	  	children[i] = -1;
	  int mid = getMidLongestShortestPath(v);
	  int maxConstrained = 0;
	  int maxConstraints = 0;
	  if(connected((Variable)vars.elementAt(parent), (Variable)vars.elementAt(mid)) && v[mid] == 1) {
	  	children[0] = mid;
	  	return;
	  }
	  int dist = 0;
	  int shortestDist = 99999;

	  for(int i = 0;i < v.length;i++) {
/*		  if(v[i] == 1 && connected((Variable)vars.elementAt(parent), (Variable)vars.elementAt(i))) {
			  int constraints = getLinks((Variable)vars.elementAt(i)).size();
			  if(constraints > maxConstraints) {
				  maxConstraints = constraints;
				  maxConstrained = i;
			  }
		  }*/
		  int[] visited = new int[vars.size()];
		  int[] path = new int[vars.size()];
		  for(int s = 0;s < vars.size();s++)
		  {
		  	visited[s] = 0;
		  	path[s] = -1;
		  }
		  visited[mid] = 1;
		  if(connected((Variable)vars.elementAt(parent), (Variable)vars.elementAt(i))) {
		  	dist = getShortestPath(mid,i,v,path,1,visited);
		  	if(dist < shortestDist) {
		  		shortestDist = dist;
				children[0] = i;
		  	}
		}
	  }
	  //children[0] = maxConstrained;
  }


  public void getChildren1(int parent,int[] v,int[] children) {
  	  int k = 0;
  	  for(int i = 0;i < v.length;i++)
  	  	children[i] = -1;
  	  for(int i = 0;i < v.length;i++) {
  		  if(v[i] == 1 && connected((Variable)vars.elementAt(parent), (Variable)vars.elementAt(i))) {
  			  children[k] = i;
  			  k++;
  		  }
  	  }
  }

  public void printVector(Vector trees) {
	  for(int i = 0;i < trees.size();i++)
	  	System.out.println(trees.elementAt(i).toString() + "**");
  }

  public Vector getTrees(int parent,int v[]) {
	  Vector trees = new Vector();
	  v[parent] = 0;
	  int[][] w = new int[vars.size()][vars.size()];
	  int noOfPartitions = getPartitions(v,w);

	  PriorityTree root = new PriorityTree((Variable)vars.elementAt(parent),vars.size()-1);
	  trees.add(root);

	  for(int i = 0;i <= noOfPartitions;i++) {
		  Vector dupTrees = new Vector();
		  int[] va = new int[vars.size()];
		  getCurrentV(w,i,va);

		  if(sum(v) == 1) {
			  for(int z = 0;z < trees.size();z++) {
				PriorityTree root1 = ((PriorityTree)trees.elementAt(z)).copy();
				PriorityTree t = new PriorityTree((Variable)vars.elementAt(getIndex(v)),vars.size()-1);
				root1.addSubTree(t);
			    dupTrees.add(root1);
			  }
		  }
		  else {
			  int[] children = new int[va.length];
			  getChildren(parent,va,children);
			  for(int k = 0;children[k] != -1 && k < children.length;k++) {
				  Vector childTrees = new Vector();
				  childTrees = getTrees(children[k],va);
				  for(int z = 0;z < trees.size();z++) {
					  for(int r = 0;r < childTrees.size();r++) {
						PriorityTree root1 = ((PriorityTree)trees.elementAt(z)).copy();
						PriorityTree t = (PriorityTree)childTrees.elementAt(r);
					  	root1.addSubTree(t);
					  	dupTrees.add(root1);
				      }
				  }
				  va[children[k]] = 1;
			  }
		  }
		  trees = (Vector)dupTrees.clone();
	  }
	  return trees;
  }

  public Vector getTrees1(int parent,int v[]) {
	  Vector trees = new Vector();
	  v[parent] = 0;
	  int[][] w = new int[vars.size()][vars.size()];
	  int noOfPartitions = getPartitions(v,w);

	  PriorityTree root = new PriorityTree((Variable)vars.elementAt(parent),vars.size()-1);
	  trees.add(root);

	  for(int i = 0;i <= noOfPartitions;i++) {
		  Vector dupTrees = new Vector();
		  int[] va = new int[vars.size()];
		  getCurrentV(w,i,va);

		  if(sum(v) == 1) {
			  for(int z = 0;z < trees.size();z++) {
				PriorityTree root1 = ((PriorityTree)trees.elementAt(z)).copy();
				PriorityTree t = new PriorityTree((Variable)vars.elementAt(getIndex(v)),vars.size()-1);
				root1.addSubTree(t);
			    dupTrees.add(root1);
			  }
		  }
		  else {
			  int[] children = new int[va.length];
			  getChildren1(parent,va,children);
			  for(int k = 0;children[k] != -1 && k < children.length;k++) {
				  Vector childTrees = new Vector();
				  childTrees = getTrees1(children[k],va);
				  for(int z = 0;z < trees.size();z++) {
					  for(int r = 0;r < childTrees.size();r++) {
						PriorityTree root1 = ((PriorityTree)trees.elementAt(z)).copy();
						PriorityTree t = (PriorityTree)childTrees.elementAt(r);
					  	root1.addSubTree(t);
					  	dupTrees.add(root1);
				      }
				  }
				  va[children[k]] = 1;
			  }
		  }
		  trees = (Vector)dupTrees.clone();
	  }
	  return trees;
  }

  /* This function is to get the shortest path between given two nodes in a graph */
  public int getShortestPath(int i,int j, int[] v,int[] path,int pathSize,int[] visited) {
  	if(i == j)
  		return 0;
  	int z = 1000;
  	int[] path1 = new int[vars.size()];
  	for(int k = 0;k < vars.size();k++)
  		path1[k] = path[k];
  	for(int k = 0;k < vars.size();k++)
  	{
  		if(v[k] == 1 && adj[i][k] == 1 && visited[k] != 1)
  		{
  			path[pathSize] = k;
  			visited[k] = 1;
  			int curr = 1+getShortestPath(k,j,v,path,pathSize+1,visited);

  			if(curr < z) {
  				z = curr;
  				for(int s = 0;s < vars.size();s++)
  				{
  					path1[s] = path[s];
  				}
  			}
  			visited[k] = 0;
  			for(int q = pathSize;q < vars.size();q++)
  				path[q] = -1;
   		}
  	}
  	for(int s = 0;s < vars.size();s++) {
  		path[s] = path1[s];
  	}

  	return z;
  }
/**
 * Initialise the original estimates from nodes and edges.
for (int i=0; i<nodes; i++)
{
    D[i][i] = 0.0f;
}

for (Edge* e = edges->begin(); e != edges->end(); e++)
{
    D[e->start][e->end] = e->length;
}

for (int k=0; k<nodes.size(); k++)
{
    for (int i=0; i<nodes.size(); i++)
    {
        for (int j=0; j<nodes.size(); j++)
        {
            D[i][j] = min( D[i][j], D[i][k] + D[k][j] );
        }
    }
} */
public int getMidLongestShortestPath(int[] v) {
	int longestShortestPath = 0;
	int node1 = -1;
	int node2 = -1;
	int D[][] = new int[vars.size()][vars.size()];
	for(int i = 0;i < vars.size();i++)
	{
		for(int j = 0;j < vars.size();j++)
		{
			if(adj[i][j] == 0)
				D[i][j] = 999999;
			else
				D[i][j] = adj[i][j];
		}
	}

 	for(int k = 0;k < vars.size();k++)
 	{
		 for(int i = 0;i < vars.size();i++)
		 {
			 if(v[i] == 1)
			 {
			 	for(int j = 0;j < vars.size();j++)
			 	{
					if(v[j] == 1)
					{
						D[i][j] = minimum (D[i][j], D[i][k] + D[k][j]);
					}
			 	}
			 }
		 }
 	}

 	for(int i = 0;i < vars.size();i++)
	{
		for(int j = 0;j < vars.size();j++)
		{
			if(D[i][j] > longestShortestPath && v[i] == 1 && v[j] == 1)
			{
				longestShortestPath = D[i][j];
				node1 = i;
				node2 = j;
			}
		}
	}
	int[] visited = new int[vars.size()];
	int[] path = new int[vars.size()];


	for(int s = 0;s < vars.size();s++)
	{
		visited[s] = 0;
		path[s] = -1;
	}

	visited[node1] = 1;
	path[0] = node1;
	getShortestPath(node1,node2,v,path,1,visited);

  	int i = 0;
  	for( i = vars.size()-1;path[i] == -1;i--)
  	{
  	}
  	return path[i/2];
}

public int minimum (int k, int l)
{
	if(k < l)
		return k;
	return l;
}

public PriorityTree priorityTree31() {
	getAdjacencyMatrix();
	int v[] = new int[vars.size()];
	int minDepth = 1000;

	for (int i = 0;i < vars.size();i++)
	{
		for(int j = 0;j < vars.size();j++)
			v[j] = 1;
		Vector trees = getTrees1(i,v);

		for(int k = 0;k < trees.size();k++) {
			PriorityTree pOrderTree = (PriorityTree)trees.elementAt(k);
			if(pOrderTree.depth() < minDepth) {
				minDepth = pOrderTree.depth();
				System.out.println(pOrderTree);
			}
		}
	}

	System.out.println("Minimum depth is " + minDepth);
	return new PriorityTree((Variable)vars.elementAt(1),0);
  }


  /** Methods for Tree Priority Order **/

  /* create a tree ordering  according to following heuristic:
     choose variable that has the most links with already chosen vars.
     In case of tie, it should have the most links with unchosen vars.
     */
  public PriorityTree priorityTree21(){
    
    notConnectedVars = new int[vars.size()];
    for(int i=0; i< vars.size(); i++) notConnectedVars[i] = -1; 
    
    /* new tree order */
    PriorityTree order;
    /* map2 will keep track of already chosen variables */
    int[] map2 = new int[vars.size()];
    for(int i=0;i<vars.size();i++){
      map2[i] = -1;
    }
	getAdjacencyMatrix();
    Variable root = findNextPriority(null, map2);
    order = new PriorityTree(root, vars.size()-1);
    map2[root.varID] = 1;
    PriorityTree currRoot = order;

    /* while we havent ordered all variables */
    while(order.size() < vars.size() ){
      int q = order.getCost(currRoot.getRootVariable()) - 1;
      Variable v = findNextPriority(currRoot.getRootVariable(), map2);
      /* order as many variables as we can until we hit a leaf*/
      while(v != null){
    	PriorityTree s = new PriorityTree(v, q);
	map2[v.varID] = 1;
	currRoot.addSubTree(s);
	currRoot = s;
	q--;
	v = findNextPriority(currRoot.getRootVariable(), map2);
      }

      /* back up one node before restarting loop */
      currRoot = order.getParent(currRoot.getRootVariable());
      if(currRoot == null &&
	 order.size() != vars.size()){
	root = findNextPriority(null, map2);
	PriorityTree st = new PriorityTree(root, vars.size()-2);
	map2[root.varID] = 1;
	order.addSubTree(st);
	currRoot = st;
	
	
	//********Changes made for storing variables that are not connected to main root
	int r = 0;
	while(r<vars.size() && notConnectedVars[r]!=-1) r++;
	notConnectedVars[r] = root.varIDof();
	
    
      }
    }

    /* error check */
    for(int i=0;i<vars.size();i++){
      Variable v1 = (Variable) vars.elementAt(i);
      for(int j=0;j<vars.size();j++){
	Variable v2 = (Variable) vars.elementAt(j);
	if(connected(v1, v2) &&
	   !order.isDescendent(v1, v2) &&
	   !order.isDescendent(v2, v1)){
	  System.out.println("Error in ordering!!!");
	  System.out.println(v1.varID + " is connected to " + v2.varID
			     + " but are in different subtrees!");
	}
      }
    }
    System.out.println("Depth is " + order.depth());
    return order;
  }
  /* for a tree ordering, is v1 higher priority than v2? */
  public boolean comparePriorityTree(Variable v1,  Variable v2){
    if(pOrderTree.isDescendent(v1,v2))
      return true;
    else
      return false;
  }


  /** Methods for Random Chain priority ordering **/

  /* prioritize variables based on ID */
  public int[] priorityRandom(){
    int[] map = new int[vars.size()];

    /* initialize */
    for(int i=0;i<vars.size();i++){
      map[i] = -1;
    }
    /* current priority to be assigned */
    int p = vars.size()-1;
    /* while there are more vars to be assigned priority */
    while(p >= 0){
      /* nextVar to assign priority to */
      Variable nextVar = null;
      for(int i=0;i<vars.size();i++){
	Variable v = (Variable) vars.elementAt(i);
	/* v has not been assigned a priority yet */
	if(map[v.varID] < 0){
	  /* does v have higher ID than current nextvar? */
	  if(nextVar == null ||
	     compareVarID(v,nextVar)){
	    nextVar = v;
	  }
	}
      }
      /* set priority */
      map[nextVar.varID] = p--;
    }
    return map;
  }
  /* helper function for priorityRandom method */
  private boolean compareVarID(Variable v1, Variable v2){
    if(v1.varID < v2.varID)
      return true;
    /* same varID?, use agent id */
    else if(v1.varID == v2.varID)
      if(v1.agentID < v2.agentID)
	return true;
    return false;
  }

  /** Methods for Chain Ordering **/

  /* prioritize variables according to following heuristic:
     choose a variable as top priority.
     choose next variable that has most links with already chosen
     variables.
     --in case of tie, choose variable with smallest domain.
     */
  public int[] priorityChain(){
    int[] map = new int[vars.size()];
    /* initialize */
    for(int i=0;i<vars.size();i++){
      map[i] = -1;
    }
    /* current priority to be assigned */
    int p = vars.size()-1;
    /* while there are more vars to be assigned priority */
    while(p >= 0){
      /* We choose a nextVar sucht that it has
	 the most links with already prioritized variables */
      Variable nextVar = findNextPriority(null, map);
      if(nextVar != null)
	/* set priority */
	map[nextVar.varID] = p--;
      else{
	System.out.println("Problem.priorityChain(): ");
	System.out.println("  findNextPriority returned null, but i still have" +
			   "  variables to prioritize!");
	System.exit(0);
      }
    }
    return map;
  }


  /* for a chain ordering, is v1 higher priority than v2? */
  public boolean comparePriorityChain(Variable v1,  Variable v2){
    if(pOrderChain[v1.varID] > pOrderChain[v2.varID])
      return true;
    if(pOrderChain[v1.varID] == pOrderChain[v2.varID] && v1.varID != v2.varID){
      System.out.println("Error: Equal priorities!! (" + v1.varID + " " + v2.varID+")");
      Utility.Dprint("  Priorities: ", Utility.MSG_LEVEL2);
      for(int i=0;i<vars.size();i++){
	Variable v = (Variable) vars.elementAt(i);
	Utility.Dprint(v.varID + " " + pOrderChain[v.varID], Utility.MSG_LEVEL2);
      }
    }
    return false;
  }


  /** Helper functions for creating priority orderings **/

  /* how many links does the given variable have with unchosen variables? */
  public int numLinksUnChosen(Variable v, int[] l){
    int result = 0;
    for(int i=0;i<vars.size();i++){
      Variable v1 = (Variable) vars.elementAt(i);
      if(connected(v1, v) && l[v1.varID] == -1 )
	result++;
    }
    return result;
  }
  /* how many links does the given variable have with Chosen variables? */
  public int numLinksChosen(Variable v, int[] l){
    int result = 0;
    for(int i=0;i<vars.size();i++){
      Variable v1 = (Variable) vars.elementAt(i);
      if(connected(v1, v) && l[v1.varID] != -1 )
	result++;
    }
    return result;
  }
  /* find a variable t such that l[t.varID] == -1 (it is unchosen),
     and it has the most links with already chosen vars.
     In case of tie, it should have the most links with unchosen vars.
     the chosen variable must be connected to the given variable v.
     */
  public Variable findNextPriority(Variable v, int[] l){
    Variable nextVar = null;
    for(int i=0;i<vars.size();i++){
      Variable vvar = (Variable) vars.elementAt(i);
      /* is this variable currently unchosen?*/
      if(l[vvar.varID] == -1){
	/* is it connected to v? */
	if(v == null || connected(v,vvar)){
	  if(nextVar == null)
	    nextVar = vvar;
	  else{
	    int vvarNumLinks = numLinksChosen(vvar, l);
	    int nextVarNumLinks = numLinksChosen(nextVar, l);
	    if(vvarNumLinks > nextVarNumLinks)
	      nextVar = vvar;
	    else if(vvarNumLinks == nextVarNumLinks){
	      if(numLinksUnChosen(vvar, l) > numLinksUnChosen(nextVar, l)){
		nextVar = vvar;
	      }
	    }
	  }
	}
      }
    }
    return nextVar;
  }

}


