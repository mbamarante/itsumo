/************************************************************************
                        stateRecord.h.cpp - Copyright denise

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

This file was generated on Thu Dec 11 2008 at 11:11:49
The original location of this file is /home/gauss/denise/uml-generated-code/stateRecord.cpp
**************************************************************************/

#include "stateRecord.h"

// Constructors/Destructors
//  
const double stateRecord::NOVALUE = std::numeric_limits<double>::min();

stateRecord::stateRecord ( ) {
initAttributes();
}

stateRecord::stateRecord (vector<SAComponent*> state , vector< CQValue * > values) {	
	setCState(state);
	setPossibleValues(values);
	initAttributes();
}

stateRecord::~stateRecord ( ) { 


  	vector <SAComponent*>::iterator it;	
	for (it = ( m_cState).begin(); it != (m_cState).end(); it++) 
		(*it)->~SAComponent();
		
    vector <CQValue*>::iterator it2;	
	for (it2 = ( m_possibleValues).begin(); it2 != ( m_possibleValues).end(); it2++) 
		(*it2)->~CQValue();
		
}

//  
// Methods
//  

unsigned int stateRecord::size(){
	return m_possibleValues.size();
}
/**
 * Set the value of m_possibleValues
 * @param new_var the new value of m_possibleValues
 */
void stateRecord::setPossibleValues ( vector< CQValue * > new_var ) {
	m_possibleValues = new_var;
}

/**
 * Get the value of m_possibleValues
 * @return the value of m_possibleValues
 */
vector< CQValue * > stateRecord::getPossibleValues ( ) {
  return m_possibleValues;
}

/**
 * Set the value of 
 * @param new_var the new value of m_cState
 */
void stateRecord::setCState( vector<SAComponent*> new_var ) {		
	sort(new_var.begin(),new_var.end(),saCompare);
	m_cState = new_var;
}

/**
 * Get the value of m_cState
 * @return the value of m_cState
 */
vector<SAComponent*> stateRecord::getCState( ) {	
	sort(m_cState.begin(),m_cState.end(),saCompare);
	return m_cState;
}

/**
 * @return int
 */
int stateRecord::numStates( ) {
	return m_cState.size();
}

double stateRecord::getActionValue(vector<SAComponent*> action){
   vector<CQValue*>::const_iterator cii = getEntry(action);
   if (cii!=m_possibleValues.end()){
   		return (*cii)->getValue();
   }
   return NOVALUE;
}


double stateRecord::getBestActionValue(){
   vector<CQValue*>::const_iterator cii;  
   double val = m_possibleValues[0]->getValue();
   for(cii=m_possibleValues.begin(); cii!=m_possibleValues.end(); ++cii){
	  	if ((*cii)->getValue()>val){ 
   				val = (*cii)->getValue();	   				
   		}	   
   }
   return val;
}

vector<SAComponent*> stateRecord::getBestAction(){
   vector<CQValue*>::const_iterator cii;  
   double val = 0;
   bool allzero = true;   
   vector<SAComponent*> ac;
   for(cii=m_possibleValues.begin(); cii!=m_possibleValues.end(); ++cii){
	  	if ((*cii)->getValue()>val){ 
   				val = (*cii)->getValue();
   				ac = (*cii)->getCAction();
   				allzero = false;	   				
   		}	   
   }
   if(allzero){ 
   		// if all actions have the same value returns a random action
   		return getRandomAction(); 
   }   
   return ac;
}

vector<SAComponent*> stateRecord::getBestSingleAction(){
   vector<CQValue*>::const_iterator cii;  
   double val = 0;
   bool allzero = true;   
   vector<SAComponent*> ac;
   for(cii=m_possibleValues.begin(); cii!=m_possibleValues.end(); cii++){
	  	if (((*cii)->isSingle())&&((*cii)->getValue()>val)){ 
   				val = (*cii)->getValue();
   				ac = (*cii)->getCAction();
   				allzero = false;	   				
   		}	   
   }
   if(allzero){ 
   		// if all actions have the same value returns a random action
   		return getRandomSingleAction(); 
   }
   return ac;
}



vector<SAComponent*> stateRecord::getRandomAction(){	
	  if (!m_possibleValues.empty())
	  		return m_possibleValues[rand()%m_possibleValues.size()]->getCAction();
}

vector<SAComponent*> stateRecord::getRandomSingleAction(){	
	  if (!m_possibleValues.empty()){
	  		unsigned int r =0;
	  		do{
	  			r = rand()%m_possibleValues.size();
	  		}while(!m_possibleValues[r]->isSingle());	  		
	  		return m_possibleValues[r]->getCAction();	  			  	
	  }
	  vector<SAComponent*> ac;
	  return ac;
}

vector<CQValue*>::iterator stateRecord::getEntry(vector<SAComponent*> action){	     
	   vector<CQValue*>::iterator cii; 	      
	   for(cii=m_possibleValues.begin(); cii!=m_possibleValues.end(); cii++){		  
		   if((*cii)->equals(action)){	   		  		 
	   			return cii;	   		
		   }
	   }	 		   	 
	   return m_possibleValues.end();
}


vector<CQValue*>::iterator stateRecord::getActionsEnd(){
	   return m_possibleValues.end();
}


/**
 * Returns the error for the best possible action 
 * (with the highest value) in a given state.
 * 
 */ 
double  stateRecord::getBestActionError(){
   vector<CQValue*>::const_iterator cii;  
   double val = stateRecord::NOVALUE;;
   bool allzero = true;   
   double error = stateRecord::NOVALUE;
   for(cii=m_possibleValues.begin(); cii!=m_possibleValues.end(); cii++){
	  	if ((*cii)->getValue()>val){ 
   				val = (*cii)->getValue();
   				error = (*cii)->getError();   					   				
   		}	   
   }
   return error; 
}


/**
 * 
 * Returns the error for the given action the state.
 * 
 */ 
double  stateRecord::getActionError(vector<SAComponent*> action){  
   vector<CQValue*>::const_iterator ac = getEntry(action);
   if (ac!=m_possibleValues.end()){
   		double r = (*ac)->getError();
   		//if(r>0) cout<<r<<endl;
   		return r;
   } 
   return NOVALUE; 
}

 
bool stateRecord::addAction(vector<SAComponent*> action, double value){  
   vector<CQValue*>::const_iterator ac = getEntry(action);
   if (ac==m_possibleValues.end()){
   		sort(action.begin(),action.end(),saCompare);
   		m_possibleValues.push_back( new CQValue(action,value) );
   		return true;
   } 
   return false; 
}

string stateRecord::print(){
	ostringstream ret;		 	
	string tmp;	
 	vector<SAComponent*>::const_iterator ci;
 	ret<<"[";  	 
   	for(ci=m_cState.begin(); ci!=m_cState.end(); ci++){   
	   ret<<(*ci)->print()<<",";
	}		
	tmp = ret.str().erase(ret.str().length()-1);	
	ret.str("");						 	
	ret<<tmp<<"] ";
	vector<CQValue*>::const_iterator cii; 		   	  
	for(cii=m_possibleValues.begin(); cii!=m_possibleValues.end(); cii++){		  
	   ret<<(*cii)->print()<<" ";
	}
	return ret.str();
}

bool stateRecord::operator==(const  stateRecord &cqval) const{	  
 	unsigned int size = m_cState.size();
 	if(size!=cqval.m_cState.size()) return false; 		 
   	for(int i=0; i<size;i++)
   		if (*(m_cState.at(i))== *(cqval.m_cState.at(i))) continue;
   		else return false;
   	return true;   	
}

vector<CQValue*>::iterator  stateRecord::getBestActionEntry(){
	return getEntry(getBestAction());
}

bool stateRecord::equals( stateRecord* cqval){
 	unsigned int size = m_cState.size(); 
 	if(size!=cqval->getCState().size()) return false; 			  	 
   	for(int i=0; i<size;i++)
   		if (*(m_cState.at(i))== *(cqval->getCState().at(i))) continue;
   		else return false;   	
    return true;
}


bool stateRecord::equalState( vector<SAComponent*> state){
 	unsigned int size = m_cState.size();  	
 	if(size!=state.size()) return false;
 	sort(state.begin(),state.end(),saCompare); //ensure the same order 	
   	for(int i=0; i<size;i++)
   		if (*(m_cState.at(i))== *(state.at(i))) continue;
   		else return false;   	
    return true;
}


bool stateRecord::containsState( vector<SAComponent*> state){	
 	sort(state.begin(),state.end(),saCompare); //ensure the same order
 	return includes(m_cState.begin(), m_cState.end(), state.begin(), state.end(), saCompare); 	   
}


bool stateRecord::testConvergence(double threshold){
	vector<CQValue*>::iterator cii; 
	for(cii=m_possibleValues.begin(); cii!=m_possibleValues.end(); cii++){		  
		if ( (*cii)->getLastDifference() > threshold){
			return false;
		}
	}
	return true;
}
// Other methods
//  

void stateRecord::initAttributes ( ) {
}

