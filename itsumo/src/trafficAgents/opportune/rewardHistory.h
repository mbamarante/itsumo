/************************************************************************
                        rewardHistory.h.h - Copyright denise

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
The original location of this file is /home/gauss/den
// Protected static attribute accessor methods
//  


// Protected attribute accessor methods
//  


// Private static attribute accessor methods
//  


// Private attribute accessor methods
//  
ise/uml-generated-code/rewardHistory.h
**************************************************************************/


#ifndef REWARDHISTORY_H
#define REWARDHISTORY_H

#include <string>
#include <vector>
#include "rewardRecord.h"

/**
  * class rewardHistory
  */

class rewardHistory
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  rewardHistory ( );

  /**
   * Empty Destructor
   */
  virtual ~rewardHistory ( );

  // Static Public attributes
  //  

  // Public attributes
  //  

  vector<rewardRecord*> m_history;

  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  

  /**
   * @return bool
   * @param  state
   */
  bool addRecord (vector<SAComponent*> state );

  /**
   * Set the value of m_history
   * @param new_var the new value of m_history
   */
  void setHistory ( vector<rewardRecord*> new_var );

  /**
   * Get the value of m_history
   * @return the value of m_history
   */
  vector<rewardRecord*> getHistory ( );



  /**
   * @return rewardRecord* 
   */
  rewardRecord* getMaxValueRecord ( );
  
  double getMaxValue( ); 

  double getMaxDeviation();
  
  double getMaxError();

  /** 
   * @param  state
   */
  void addRecord (vector<SAComponent*> state, double value );


  /**
   * @return bool
   * @param  state
   */
  bool updateRecord (vector<SAComponent*> state, double value);


  // Private attribute accessor methods
  //  

private:



  /**
   * @return bool
   * @param  state
   */
  bool stateHasValue (vector<SAComponent*> state );

  void initAttributes ( ) ;

};

#endif // REWARDHISTORY_H
