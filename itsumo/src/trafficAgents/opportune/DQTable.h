/************************************************************************
                        DQTable.h - Copyright denise

Here you can write a license for your code, some comments or any other
information you want to have in your generated code. To to this simply
configure the "headings" directory in uml to point to a directory
where you have your heading files.

or you can just replace the contents of this file with your own.
If you want to do this, this file is located at

/usr/share/apps/umbrello/headings/heading.h

-->Code Generators searches for heading files based on the file extension
   i.e. it will look for a file name ending in ".h" to include in C++ header
   files, and for a file name ending in ".java" to include in all generated
   java code.
   If you name the file "heading.<extension>", Code Generator will always
   choose this file even if there are other files with the same extension in the
   directory. If you name the file something else, it must be the only one with that
   extension in the directory to guarantee that Code Generator will choose it.

you can use variables in your heading files which are replaced at generation
time. possible variables are : author, date, time, filename and filepath.
just write %variable_name%

This file was generated on Tue Dec 2 2008 at 13:16:09
The original location of this file is /home/gausDQTable::initAttributes(*OpLearner own, int n_states, int n_actions)s/denise/uml-generated-code/DQTable.h
**************************************************************************/


#ifndef DQTABLE_H
#define DQTABLE_H



#include "stateRecord.h"




/**
  * class DQTable
  * Class of the dynamic Q-table
  */

class DQTable{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  DQTable();
  
  DQTable(int id, int n_st, int n_act, double cur, double lr, double dis);

  /**
   * Empty Destructor
   */
  virtual ~DQTable();

 

  /**
   * returns the Q value for a given state-action pair
   * @return double
   * @param  state
   * @param  action
   */
  double getValueForPair (vector<SAComponent*> state, vector<SAComponent*> action );


  /**
   * returns the best action (highest value) for a given state
   * @return vector<SAComponent*>
   * @param  state
   */
  vector<SAComponent*> getBestAction (vector<SAComponent*> state );

  vector<SAComponent*> getBestSingleAction (vector<SAComponent*> state );

  /**
   * @return double
   * @param  state
   */
  double getBestActionValue (vector<SAComponent*> state );
  
  
  double getBestActionError(vector<SAComponent*> state );
 
  vector<SAComponent*> getBestContainsState(vector<SAComponent*> state );
  
  double getStateActionError(vector<SAComponent*> state, vector<SAComponent*> action );
  
  /**
   * returns a random action with the probability "epsilon" (curiosity), otherwise
   * returns the best action (highest value) for a given state
   * @return vector<SAComponent*> action
   * @param  state
   */
  vector<SAComponent*> getEpsilonGreedyAction(vector<SAComponent*> state);

  
  
  vector<SAComponent*> act(vector<SAComponent*> state, unsigned int type);

  /**
   * returns a random action for a given state
   * @return vector<SAComponent*>
   * @param  state
   */
  vector<SAComponent*> getRandomAction (vector<SAComponent*> state );

  void update(vector<SAComponent*> state, vector<SAComponent*> action, vector<SAComponent*> newState, double reward);
  void observe(vector<SAComponent*> state, vector<SAComponent*> action, vector<SAComponent*> newState, double reward);


  /**
   * Inserts a new pair in the table
   * @return bool
   * @param  newEntry
   */
  bool addState(stateRecord* newEntry );
  bool addActionToState(vector<SAComponent*> state, vector<SAComponent*> action, double value);
  bool contains(vector<SAComponent*> state, vector<SAComponent*> action);
  
  vector<CQValue*>::iterator getEntry(vector<SAComponent*> state, vector<SAComponent*> action);
  vector<stateRecord*>::iterator getStateEntry(vector<SAComponent*> state);  
  int getSize();

  void setEpsilon(double e);
  void setDecay(double d);

  bool testConvergence(double threshold);

private:

  // Static Private attributes
  //  

  // Private attributes
  //  

  vector<stateRecord*> m_table;
  double decay;
  double curiosity; //epsilon
  double learning; //epsilon
  double discount; //epsilon
  int    myid; //id of the learning agent
 
public:


  // Private attribute accessor methods
  //  


  /**
   * Set the value of m_table
   * @param new_var the new value of m_table
   */
  void setTable ( vector<stateRecord*> new_var );

  /**
   * Get the value of m_table
   * @return the value of m_table
   */
  vector<stateRecord*> getTable ( );
  
  string print();
  
  	static const unsigned int GREEDY;
	static const unsigned int E_GREEDY;
 	static const unsigned int RANDOM;
  

private:


  void initAttributes();
  void initAttributes(int id, int n_st, int n_act, double cur, double lr, double dis);

};

#endif // DQTABLE_H
