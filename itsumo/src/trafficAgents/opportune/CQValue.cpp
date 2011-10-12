#include "CQValue.h"

const unsigned int CQValue::MINIMUM_UPDATES	= 10;

// Constructors/Destructors
//  

CQValue::CQValue(){
	initAttributes();
}

/** 
 * @param  action
 */
CQValue::CQValue(vector<SAComponent*> action ) {	 
	setCAction(action);	  
	initAttributes();
}

/**
 * Creates a new action with a specific new value
 * @param  action
 * @param  value
 */
CQValue::CQValue(vector<SAComponent*> action, double value ) {	 
	setCAction(action);	  	
	initAttributes();
	setValue(value);
}


CQValue::~CQValue(){

	vector <SAComponent*>::iterator it;	
	for (it = ( m_cAction).begin(); it != ( m_cAction).end(); it++) 
		(*it)->~SAComponent();
		
	m_avgReward.rewardHistory::~rewardHistory();
}

//  
// Methods
//  




// Private attribute accessor methods
//  

/**
 * Set the value of m_cAction
 * @param new_var the new value of m_cAction
 */
void CQValue::setCAction( vector<SAComponent*> new_var ) {
	sort(new_var.begin(),new_var.end(),saCompare);	
  	m_cAction = new_var;
}

/**
 * Get the value of m_cAction
 * @return the value of m_cAction
 */
vector<SAComponent*> CQValue::getCAction( ) {
  return m_cAction;
}

/**
 * Set the value of m_value
 * @param new_var the new value of m_value
 */
void CQValue::setValue( double new_var ) {
	old_value = m_value; //always saves the last value
  	m_value = new_var;
}

/**
 * Get the value of m_value
 * @return the value of m_value
 */
double CQValue::getValue( ) {
  return m_value;
}

/**
 * Get the differentece betweeen 
 * m_value and old_value
 * @return difference (absolute value)
 */
double CQValue::getLastDifference(){
  return fabs(old_value-m_value);
}

/**
 * Set the value of m_updates
 * @param new_var the new value of m_updates
 */
void CQValue::setUpdates( int new_var ) {
  m_updates = new_var;
}

/**
 * Get the value of m_updates
 * @return the value of m_updates
 */
int CQValue::getUpdates( ) {
  return m_updates;
}

/**
 * Set the value of m_error
 * @param new_var the new value of m_error
 */
void CQValue::setError( double new_var ) {
  m_error = new_var;
}

/**
 * Get the value of m_error
 * @return the value of m_error
 */
double CQValue::getError( ) {  
	updateError();	//calculates the new error	
	return m_error;
}

/**
 * updates the reward record
 * @param state: state
 * @param value: value of the current reward
 */
void CQValue::updateReward(vector<SAComponent*> state, double value) {
	vector<SAComponent*> s;
	s.push_back( new SAComponent(0,0));//TESTE
	m_avgReward.updateRecord(s, value);
}

void CQValue::update(vector<SAComponent*> state, double rewardValue, double newValue){	
	setValue(newValue);					//set the new Value
	updateReward(state, rewardValue); 	//updates the reward record	
	m_updates++;						//increment the updates number
}
/**
 * Get the value of m_avgReward
 * @return the value of m_avgReward
 */
rewardHistory CQValue::getAvgReward( ) {
  return m_avgReward;
}

bool  CQValue::empty(){
	if(m_cAction.empty()) return true;
	return false;
}

/**
 * Returns true if there is only one action component
 */
bool CQValue::isSingle(){
	if(m_cAction.size()==1) return true;
	return false;
}


string CQValue::print(){
	ostringstream ret;	 	
	string tmp;	
	vector<SAComponent*>::const_iterator cii;
	ret<<"[";			  
   	for(cii=m_cAction.begin(); cii!=m_cAction.end(); cii++){		   
	   ret<< (*cii)->print()<<",";	
	}
	tmp = ret.str().erase(ret.str().length()-1);
	ret.str("");	
	ret<<tmp<<"]"<<" ("<<m_error<<"): "<< m_value;	
    return ret.str();	 
}

// Other methods
//  


/**
 * @return vector<SAComponent*>
 */
vector<SAComponent*> CQValue::getAction( ) {
	return m_cAction;
}





/**
 * @return int
 */
int CQValue::numActions( ) {
	return m_cAction.size();
}

/**
 * Updates the error value when the pair has ben visited for 
 * a given number of times (CQValue::MINIMUM_UPDATES) using 
 * the new error value from the m_avgReward.getMaxError()
 */
void CQValue::updateError() {
	if(m_updates>CQValue::MINIMUM_UPDATES){
		m_error = m_avgReward.getMaxError();	
	}	
}

void CQValue::initAttributes( ) {
  	m_value = 0;
  	old_value = 0;
	m_updates = 0;
	m_error = 0;
}


bool operator==(const CQValue &a, const CQValue &b){	  
	int size = a.m_cAction.size();
 	if(size!=b.m_cAction.size()) return false; 		
   	for(int i=0; i<size;i++){
   		if (*(a.m_cAction.at(i))== *(b.m_cAction.at(i))) continue;
   		else return false;    	
   	}
   return true;
}

bool CQValue::equals(vector<SAComponent*> ac){ 			 	 	  	 	
	int size = m_cAction.size();
	if( size!= ac.size() ) return false; 
   	for(int i=0; i<size;i++){   		   		
   		//if ( (m_cAction.at(i))->equals(ac.at(i)) ){   	   
   		if(saEquals(m_cAction.at(i),ac.at(i))){ 			
   			continue;
   		}
   		else{    	   			
   			return false;
   		}
   	}   	
    return true;
}

bool CQValue::equals(CQValue* cqval){ 	
	int size = m_cAction.size();	
 	if(size!=cqval->getCAction().size()) return false;
 	vector<SAComponent*> ac = cqval->getCAction(); 	 	 
   	for(int i=0; i<size;i++){
   		if (*(m_cAction.at(i))== *(ac.at(i))) continue;
   		else return false;
   	}
    return true;
}


bool CQValue::less(CQValue& cqval){
	return (m_value < cqval.m_value);
}

bool CQValue::operator<(const CQValue& cqval) const{
	return (m_value < cqval.m_value);
}


