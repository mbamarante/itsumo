/************************************************************************
                        rewardRecord.h.cpp - Copyright denise

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
The original location of this file is /home/gauss/denise/uml-generated-code/rewardRecord.cpp
**************************************************************************/

#include "rewardRecord.h"

// Constructors/Destructors
//  
const unsigned int rewardRecord::MAX_RECORDS	= 1000; 

rewardRecord::rewardRecord ( ) {
initAttributes();
}

rewardRecord::rewardRecord (vector<SAComponent*> st, double value ) {
	setState(st);
	m_Values.push_back(value);
	m_avgValue = value;
}

rewardRecord::~rewardRecord ( ) { 
	vector<SAComponent*>::iterator it;	
	for (it =(m_state).begin(); it != (m_state).end(); it++) 
		(*it)->~SAComponent();
		
	m_Values.vector::~vector();
}

//  
// Methods
//  

/**
* Updates the value of m_avgValue and increments the 
* number of hits.
* @param new_var the new value of m_avgValue
*/
void rewardRecord::update( double new_var ){
	unsigned int size = m_Values.size();
	if ( size == MAX_RECORDS){		
		m_Values.erase(m_Values.begin());	
	}		
	m_Values.push_back(new_var);
	++size;	 
	if (size>1){
		m_avgValue  = gsl_stats_mean(&m_Values[0], 1, size);	
	}	
	m_avgValue = new_var;
	//cout<<"mean: "<<m_avgValue<<" stdev: "<<getStdDeviation()<<" cv: "<<getCoefficientOfVariation()<<endl;	
}

/**
 * Returns the average value
 * @return the value of m_avgValue
 */
double rewardRecord::getAvgValue ( ) {
  return m_avgValue;  
}


/**
 * Set the value of m_state
 * @param new_var the new value of m_state
 */
void rewardRecord::setState ( vector<SAComponent*> new_var ) {
	sort(new_var.begin(),new_var.end(),saCompare);
	m_state = new_var;

}

/**
 * Get the value of m_state
 * @return the value of m_state
 */
vector<SAComponent*> rewardRecord::getState ( ) {
  return m_state;
}



/**
 * Get the value of m_hits
 * @return the value of m_hits
 */
unsigned int rewardRecord::getHits ( ) {
  return m_Values.size();
}

double rewardRecord::getStdDeviation(){
	unsigned int size = m_Values.size();
	if ( size > 0){		
		return gsl_stats_sd_m(&m_Values[0], 1, size, m_avgValue);
	}	
	return 0.0;
}

double rewardRecord::getCoefficientOfVariation(){
	if (m_avgValue!=0){
		return getStdDeviation()/m_avgValue;
	}
	return 0.0;
}

/**
 * test if the states are equal
 * @return true if they are equal
 */
bool rewardRecord::equalState( vector<SAComponent*> state){
 	unsigned int size = m_state.size();  	
 	if(size!=state.size()) return false;
 	sort(state.begin(),state.end(),saCompare); 			  	 
   	for(unsigned int i=0; i<size;i++)
   		if (*(m_state.at(i))== *(state.at(i))) continue;
   		else return false;   	
    return true;
}

string rewardRecord::print(){
	vector<double>::const_iterator v;
	ostringstream ret;	 		
	for(v=m_Values.begin();v!=m_Values.end();v++){
		ret<<*v<<" ";
	}
	return ret.str();
}
// Other methods
//  

void rewardRecord::initAttributes( ){
  m_avgValue = 0.0;
 
}

