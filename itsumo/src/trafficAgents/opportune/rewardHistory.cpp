/************************************************************************
                        rewardHistory.h.cpp - Copyright denise

Here you can write a license for your code, some comments or any other
information you want to have in your generated code. To to this simply
configure the "headings" directory in uml to point to a directory
where you have your heading files.

or you can just replace the contents of this file with your own.
If you want to do this, this file is located at

/usr/share/apps/umbrello/headings/heading.cpp

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
The original location of this file is /home/gauss/denise/uml-generated-code/rewardHistory.cpp
**************************************************************************/

#include "rewardHistory.h"

// Constructors/Destructors
//  

rewardHistory::rewardHistory ( ) {
initAttributes();
}

rewardHistory::~rewardHistory ( ) {
	vector <rewardRecord*>::iterator it;	
	for (it =(m_history).begin(); it != (m_history).end(); it++) 
		(*it)->~rewardRecord();
}

//  
// Methods
//  


// Public attribute accessor methods
//  


/**
 * Set the value of m_history
 * @param new_var the new value of m_history
 */
void rewardHistory::setHistory ( vector<rewardRecord*> new_var ) {
  m_history = new_var;
}

/**
 * Get the value of m_history
 * @return the value of m_history
 */
vector<rewardRecord*> rewardHistory::getHistory ( ) {
  return m_history;
}


// Other methods
//  


/**
 * @return rewardRecord
 */
rewardRecord* rewardHistory::getMaxValueRecord ( ) {
   vector<rewardRecord*>::const_iterator cii;
   rewardRecord* temp;
   double val = -1;
   for(cii=m_history.begin(); cii!=m_history.end(); cii++){
   		if ((*cii)->getAvgValue()>val){
   			temp = *cii;
   		}   		      
   }
   return temp;
}

/**
 * Returns the maximum mean reward received for this action
 * @return maximum value
 */
double rewardHistory::getMaxValue( ) {
   return getMaxValueRecord()->getAvgValue();
}

double rewardHistory::getMaxDeviation(){
   vector<rewardRecord*>::const_iterator cii;   
   double val = -1.0; 
   double dev = -1.0;
   for(cii=m_history.begin(); cii!=m_history.end(); ++cii){
   		dev = (*cii)->getStdDeviation();
   		if(dev>val){
   			val = dev; 
   		}   		      
   }
   return val;
}
double rewardHistory::getMaxError(){
   vector<rewardRecord*>::const_iterator cii;   
   double val = 0.0;
   double err = -1;   
   for(cii=m_history.begin(); cii!=m_history.end(); cii++){
   		err = (*cii)->getCoefficientOfVariation();
   		if(err>val){
   			val = err; 
   		}   		      
   }
   return val;
}

/**
 * Creates a new record for the given state 
 * @param  state
 */
void rewardHistory::addRecord(vector<SAComponent*> state, double value) {
	m_history.push_back(new rewardRecord(state,value));
}


/**
 * @return bool
 * @param  state
 */
bool rewardHistory::stateHasValue(vector<SAComponent*> state ) {
   vector<rewardRecord*>::const_iterator cii;   
   for(cii=m_history.begin(); cii!=m_history.end(); cii++){
   		if ((*cii)->equalState(state)){   			
   			return true;   			
   		}   		      
   }
   return false;
}

/**
 * 
 * @param  state
 */
bool rewardHistory::updateRecord(vector<SAComponent*> state, double value){	
	vector<rewardRecord*>::const_iterator cii;   
	for(cii=m_history.begin(); cii!=m_history.end(); cii++){
		if ((*cii)->equalState(state)){
				(*cii)->update(value);
	  			return true;   			
	  		}   		      
	}
	addRecord(state, value);
	return true;
}


void rewardHistory::initAttributes ( ) {
	 vector<rewardRecord*> m_history;
}

