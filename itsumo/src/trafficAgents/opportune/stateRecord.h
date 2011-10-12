/************************************************************************
                        stateRecord.h - Copyright denise

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

This file was generated on Thu Dec 11 2008 at 11:11:49
The original location of this file is /home/gauss/denise/uml-generated-code/stateRecord.h
**************************************************************************/


#ifndef STATERECORD_H
#define STATERECORD_H

#include<limits>
#include "CQValue.h"

/**
  * class stateRecord
  */

class stateRecord
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  stateRecord ( );

  stateRecord (vector<SAComponent*> state , vector< CQValue * > values);

  /**
   * Empty Destructor
   */
  virtual ~stateRecord ( );

  // Static Public attributes
  // 


private:

  // Static Private attributes
  //  

  // Private attributes
  //  

  vector< CQValue * > m_possibleValues;
  vector<SAComponent*> m_cState;

public:


  // Private attribute accessor methods
  //  


  /**
   * Set the value of m_possibleValues
   * @param new_var the new value of m_possibleValues
   */
  void setPossibleValues ( vector< CQValue * > new_var );

  /**
   * Get the value of m_possibleValues
   * @return the value of m_possibleValues
   */
  vector< CQValue * > getPossibleValues ( );
  
  double getActionValue(vector<SAComponent*> action);
  
  double getBestActionValue();
  
  vector<SAComponent*> getBestAction();
  
  vector<SAComponent*> getBestSingleAction();
  
  vector<CQValue*>::iterator  getBestActionEntry();
  
  double getBestActionError();
  
  bool addAction(vector<SAComponent*> action, double value);
  
  double getActionError(vector<SAComponent*> action);
  
  vector<SAComponent*> getRandomAction();
  
  vector<SAComponent*> getRandomSingleAction();
  
  /**
   * Set the value of m_cState
   * @param new_var the new value of m_cState
   */
  void setCState ( vector<SAComponent*> new_var );

  /**
   * Get the value of m_cState
   * @return the value of m_cState
   */
  vector<SAComponent*> getCState ( );
  
  int numStates( );
  vector<CQValue*>::iterator getEntry(vector<SAComponent*> action);
  vector<CQValue*>::iterator getActionsEnd();
  
  bool operator==(const  stateRecord &cqval) const;
  bool equals( stateRecord* cqval);
  bool equalState( vector<SAComponent*> state);
  bool containsState( vector<SAComponent*> state);
  string print();
  unsigned int size();
  bool testConvergence(double threshold);
  
  static const double NOVALUE;
  
private:
  void initAttributes ( ) ;  
  
};

#endif // STATERECORD_H
