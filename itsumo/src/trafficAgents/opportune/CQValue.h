/************************************************************************
                        CQValue.h.h - Copyright denise

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
The original location of this file is /home/gauss/denise/uml-generated-code/CQValue.h
**************************************************************************/


#ifndef CQVALUE_H
#define CQVALUE_H

#include <algorithm>
#include <sstream>
#include <math.h>


#include "rewardHistory.h"

using namespace std;
/**
  * class CQValue
  * Represents a state-action pair and the respective value.
  */

class  CQValue
{
public:

  static const unsigned int MINIMUM_UPDATES;

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  CQValue();
  
  CQValue(vector<SAComponent*> action);
  
  CQValue(vector<SAComponent*> action, double value );
  
  /**
   * Empty Destructor
   */
  virtual ~CQValue();



  /**
   * @return vector<SAComponent*>
   */
  vector<SAComponent*> getAction();


  /**
   * @return int
   */
  int numActions();

  /**
   * updates the error value
   */
  void updateError();


  /**
   * Set the value of m_cAction
   * @param new_var the new value of m_cAction
   */
  void setCAction( vector<SAComponent*> new_var );

  /**
   * Get the value of m_cAction
   * @return the value of m_cAction
   */
  vector<SAComponent*> getCAction();


  /**
   * Set the value of m_value
   * @param new_var the new value of m_value
   */
  void setValue( double new_var );

  /**
   * Get the value of m_value
   * @return the value of m_value
   */
  double getValue();


  /**
   * Set the value of m_updates
   * @param new_var the new value of m_updates
   */
  void setUpdates( int new_var );

  /**
   * Get the value of m_updates
   * @return the value of m_updates
   */
  int getUpdates();


  /**
   * Set the value of m_error
   * @param new_var the new value of m_error
   */
  void setError( double new_var );

  /**
   * Get the value of m_error
   * @return the value of m_error
   */
  double getError();

  void updateReward(vector<SAComponent*> state, double value);
  
  void update(vector<SAComponent*> state, double rewardValue, double newValue);

  
	/**
	 * Get the value of m_avgReward
	 * @return the value of m_avgReward
	 */
	rewardHistory getAvgReward();
  
  /**
   * Returns true if the states or the actions is empty
   */
  bool empty();

  /**
   * Returns true if there is only one action component
   */
  bool isSingle();

  
  string print();
  
  bool equals(vector<SAComponent*> ac);
  
  bool equals(CQValue* cqval);
  
  double getLastDifference();
    
  friend bool operator==(const CQValue &a,const CQValue &b);


  bool less(CQValue& cqval);
  
  bool operator<(const CQValue& cqval) const;
  
private:
  // Private attributes
  //  
  vector<SAComponent*> m_cAction;
  double m_value;
  double old_value;
  int m_updates;
  double m_error;
  rewardHistory m_avgReward;

  void initAttributes() ;

};

#endif // CQVALUE_H
