/************************************************************************
                        rewardRecord.h.h - Copyright denise

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
The original location of this file is /home/gauss/denise/uml-generated-code/rewardRecord.h
**************************************************************************/


#ifndef REWARDRECORD_H
#define REWARDRECORD_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <gsl/gsl_statistics.h>
#include "SAComponent.h"
using namespace std;
/**
  * class rewardRecord
  */

class rewardRecord
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  rewardRecord ( );
  
  rewardRecord(vector<SAComponent*> st, double value );

  /**
   * Empty Destructor
   */
  virtual ~rewardRecord ( );

  // Static Public attributes
  //  

  // Public attributes
  //  

  vector<double> m_Values;
  double m_avgValue;
  vector<SAComponent*> m_state;

  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  


  /**
   * Set the value of m_avgValue
   * @param new_var the new value of m_avgValue
   */
  void addValue( double new_var );
  
  
   /**
    * Updates the value of m_avgValue and increments the
    *  number of hits.
    * @param new_var the new value of m_avgValue
    */
  void update( double new_var );

  /**
   * Get the value of m_avgValue
   * @return the value of m_avgValue
   */
  double getAvgValue ( );


  /**
   * Set the value of m_state
   * @param new_var the new value of m_state
   */
  void setState ( vector<SAComponent*> new_var );

  /**
   * Get the value of m_state
   * @return the value of m_state
   */
  vector<SAComponent*> getState ( );
  
  double getStdDeviation();
  
  double getCoefficientOfVariation();

  /**
   * Get the value of m_hits
   * @return the value of m_hits
   */
  unsigned int getHits ( );

  /**
   * test if the states are equal
   */
  bool equalState( vector<SAComponent*> state);
  
  string print();
private:


  void initAttributes ( ) ;
  static const unsigned int MAX_RECORDS; 


};

#endif // REWARDRECORD_H
