/************************************************************************
                        DQTable.h.cpp - Copyright denise

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
The original location of this file is /home/gauss/denise/uml-generated-code/DQTable.cpp
**************************************************************************/

#include "DQTable.h"

const unsigned int DQTable::GREEDY		= 0;
const unsigned int DQTable::E_GREEDY	= 1; 
const unsigned int DQTable::RANDOM		= 2;  	 

// Constructors/Destructors
//  

DQTable::DQTable() {
   initAttributes();
}


DQTable::DQTable(int id, int n_st, int n_act, double cur, double lr, double dis) {
	initAttributes(id, n_st,n_act,cur,lr,dis);
}


DQTable::~DQTable ( ) {
  	vector <stateRecord*>::iterator it;	
	for (it = (m_table).begin(); it != (m_table).end(); it++) 
		(*it)->~stateRecord();
}

//  
// Methods
//  




// Private attribute accessor methods
//  
/**
 * Set the value of m_table
 * @param new_var the new value of m_table
 */
void DQTable::setTable ( vector<stateRecord*> new_var ) {
  m_table = new_var;
}

void  DQTable::setEpsilon(double e){
	curiosity = e;
}


void  DQTable::setDecay(double d){
	decay = d;
}



/**
 * Get the value of m_table
 * @return the value of m_table
 */
vector<stateRecord*> DQTable::getTable ( ) {
  return m_table;
}

/**
 * Returns the number of entries of the table
 */
int DQTable::getSize(){
	vector<stateRecord*>::const_iterator cii;
	unsigned int sz = 0;  
	for(cii=m_table.begin(); cii!=m_table.end(); cii++){		  
		sz+=(*cii)->size();
	}
	return sz;
}
// Other methods
//  


/**
 * returns the Q value for a given state-action pair
 * @return double
 * @param  state
 * @param  action
 */
double DQTable::getValueForPair(vector<SAComponent*> state, vector<SAComponent*> action ) {
	   vector<stateRecord*>::const_iterator cii;  	
	   sort(state.begin(),state.end(),saCompare);  
	   for(cii=m_table.begin(); cii!=m_table.end(); ++cii){
		   if((*cii)->equalState(state)){ 		   	
	   			return (*cii)->getActionValue(action);
		   }
	   }	   	  
	   return stateRecord::NOVALUE;
}


/**
 * returns the best action (highest value) for a given state
 * @return vector<SAComponent*>
 * @param  state
 */
vector<SAComponent*> DQTable::getBestAction (vector<SAComponent*> state ) {
	   vector<stateRecord*>::const_iterator cii;  
	   double val = 0;
	   vector<SAComponent*> ac;
	   sort(state.begin(),state.end(),saCompare);
	   for(cii=m_table.begin(); cii!=m_table.end(); ++cii){
		   if((*cii)->equalState(state)){ 		   	
	   			return (*cii)->getBestAction();
		   }
	   }
	   cout<<myid<<" NAO ACHOU acao para "<<printSAVector(state)<<endl; 		  
	   return ac;
}


/**
 * returns the best single action (highest value) for a given state
 * @return vector<SAComponent*>
 * @param  state
 */
vector<SAComponent*> DQTable::getBestSingleAction (vector<SAComponent*> state ) {
	   vector<stateRecord*>::const_iterator cii;  
	   double val = 0;
	   vector<SAComponent*> ac;
	   sort(state.begin(),state.end(),saCompare);
	   for(cii=m_table.begin(); cii!=m_table.end(); ++cii){
		   if((*cii)->equalState(state)){ 		   	
	   			return (*cii)->getBestSingleAction();
		   }
	   }		  
	   return ac;
}




/**
 * Returns the best action value for a given state
 * @return double
 * @param  state
 */
double DQTable::getBestActionValue(vector<SAComponent*> state ) {
	   vector<stateRecord*>::const_iterator cii;  	 
	   for(cii=m_table.begin(); cii!=m_table.end(); ++cii){	   			 
		   if((*cii)->equalState(state)){		   	
	   			return (*cii)->getBestActionValue();
		   }
	   }
	   return stateRecord::NOVALUE;
}

/**
 * Returns the error of the best action for a given state
 * @return error (double)
 * @param  state (vector<SAComponent*>)
 */
double DQTable::getBestActionError(vector<SAComponent*> state ) {
	   vector<stateRecord*>::const_iterator cii;  	   	  
	   for(cii=m_table.begin(); cii!=m_table.end(); ++cii){	   			 
		   if((*cii)->equalState(state)){		   	
	   			return (*cii)->getBestActionError();
		   }
	   }
	   return stateRecord::NOVALUE;
}


/**
 * Returns the state with the minimum error in the best action considering 
 * all states that includes the given state in the table 
 * @return beststate (vector<SAComponent*>)
 * @param  state (vector<SAComponent*>)
 */
vector<SAComponent*> DQTable::getBestContainsState(vector<SAComponent*> state ) {
	   vector<stateRecord*>::const_iterator cii;  
	   vector<SAComponent*> beststate = state;
	   double val = stateRecord::NOVALUE;
	   double err = stateRecord::NOVALUE;	   
	   for(cii=m_table.begin(); cii!=m_table.end(); ++cii){	   			 
		   if((*cii)->containsState(state)){
		   		err = (*cii)->getBestActionError();	   	
	   			if ( err < val){
	   				val = err;
	   				beststate = (*cii)->getCState();
	   			}
		   }
	   }
	   return beststate;
}


/**
 * Returns the error of the state-action pair
 * @return error (double)
 * @param  state (vector<SAComponent*>)
 */
double DQTable::getStateActionError(vector<SAComponent*> state, vector<SAComponent*> action ) {
	   vector<stateRecord*>::const_iterator cii;  	   	   
	   for(cii=m_table.begin(); cii!=m_table.end(); ++cii){	   			 
		   if((*cii)->equalState(state)){		   	
	   			return (*cii)->getActionError(action);
		   }
	   }
	   return stateRecord::NOVALUE;
}

/** 
 * Returns the best action considering a probability (epsilon) 
 * to choose a random action.
 * 
 */
vector<SAComponent*> DQTable::getEpsilonGreedyAction(vector<SAComponent*> state){
	if ( rand() < ( RAND_MAX * curiosity) ) {              	     
		return getRandomAction(state);	    		
	}
	return getBestAction(state);	        
}

/**
 * returns a random action for a given state
 * @return vector<SAComponent*>
 * @param  state
 */
vector<SAComponent*> DQTable::getRandomAction(vector<SAComponent*> state){	
	   vector<stateRecord*>::const_iterator cii;
	   for(cii=m_table.begin(); cii!=m_table.end(); ++cii){
		   if((*cii)->equalState(state)){  
				return (*cii)->getRandomAction();
		   }
	   }
	   vector<SAComponent*> ret;
	   return ret;
}
/**
 * Returns an action according to a given policy, where type can be:
 *  DQTable::GREEDY		: for a greedy policy
 *  DQTable::E_GREEDY	: epsilon greedy
 *  DQTable::RANDOM		: random policy
 * 
 * Uses a greedy policy if the type is not recognized.
 *  
 */
vector<SAComponent*> DQTable::act(vector<SAComponent*> state, unsigned int type){
	switch(type){
		case(DQTable::E_GREEDY):	
			return getEpsilonGreedyAction(state);
		case(DQTable::GREEDY):
			return getBestAction(state);
		case(DQTable::RANDOM):
			return getRandomAction(state);
	}
	//
	return getBestAction(state);
}

bool DQTable::testConvergence(double threshold){
 	vector<stateRecord*>::const_iterator cii;  	 
   	for(cii=m_table.begin(); cii!=m_table.end(); cii++){	   
	   if ( !((*cii)->testConvergence(threshold)))
	   		return false;	   
	}
	return true;
}

/**
 * 
 */
void DQTable::update(vector<SAComponent*> state, vector<SAComponent*> action, vector<SAComponent*> newState, double reward){
	vector<SAComponent*>::iterator s;	 
	vector<SAComponent*>::iterator a;
	vector<stateRecord*>::iterator sr;
	vector<CQValue*>::iterator ac;	 
	vector<SAComponent*> stateComponent;
	SAComponent* myStateComponent;
	SAComponent* myActionComponent;
	vector<SAComponent*> actionComponent;
	double actionSize = (double)action.size();
	double oldValue = 0 ;
	double newValue = 0 ;	
	sort(newState.begin(),newState.end(),saCompare);
	myStateComponent = getComponentFromOwner(state, myid);
	myActionComponent = getComponentFromOwner(action, myid);
	if( myStateComponent != NULL ){ 
		stateComponent.push_back(myStateComponent);		
		state = removeComponent(state, myStateComponent);
		sort(state.begin(),state.end(),saCompare);
		if(myActionComponent != NULL) actionComponent.push_back(myActionComponent);
		if (action.size()>1)		
			action = removeComponent(action, myActionComponent);
		sort(action.begin(),action.end(),saCompare);
		//ajust single values
		sr = getStateEntry(stateComponent);						
		if (sr != m_table.end()){							
			ac = (*sr)->getEntry(actionComponent);								
			if (ac != (*sr)->getActionsEnd() ){ // just update when the action exists!										
				oldValue = (*ac)->getValue();									   			
				newValue = oldValue+( learning * (reward + discount * getBestActionValue(newState)-oldValue));
				(*ac)->update(newState, reward, newValue);									
			}					
		}					
		actionComponent.clear();
		//ajust composed values
		for(s= state.begin(); s!=state.end(); ++s) {    				 					
			stateComponent.push_back(*s);
			sort(stateComponent.begin(),stateComponent.end(),saCompare);
			sr = getStateEntry(stateComponent);						
			if (sr != m_table.end()){							
				for (a = action.begin(); a != action.end(); ++a){											
					actionComponent.push_back(*a);				
					sort(actionComponent.begin(),actionComponent.end(),saCompare);
					ac = (*sr)->getEntry(actionComponent);								
					if (ac != (*sr)->getActionsEnd() ){ // just update when the action exists!										
						oldValue = (*ac)->getValue();									   			
						newValue = oldValue+( learning * (reward + discount * getBestActionValue(newState)-oldValue));
						(*ac)->update(newState, reward, newValue);									
					}									
				}			
			}
		}	
	}
}
void DQTable::observe(vector<SAComponent*> state, vector<SAComponent*> action, vector<SAComponent*> newState, double reward){
	update(state, action, newState, reward);
}

/**
 * Inserts a new pair in the table
 * @return bool
 * @param  newEntry
 */
bool DQTable::addState(stateRecord* newEntry ){	
	vector<stateRecord*>::const_iterator it;	
	it = find(m_table.begin(), m_table.end(), newEntry);	
	if (it!=m_table.end()){ // value is already in the table!
		return false;
	}
	m_table.push_back(newEntry);		
	return true;
	
}

/**
 * Inserts a new action for a given state
 * @return bool: false if the action was already on the state or 
 * if the state do not exist
 * @param  state: state 
 * @param  action: action to be included
 * @param  value: initialization value
 */
bool DQTable::addActionToState(vector<SAComponent*> state, vector<SAComponent*> action, double value){
	   vector<stateRecord*>::iterator cii;	   
	   for(cii=m_table.begin(); cii!=m_table.end(); cii++){		  
		   if((*cii)->equalState(state)){		   		  		 
	   			return (*cii)->addAction(action, value);
		   }
	   }	
	   return false;
}
/**
 * Returns true if pair is already in the table. 
 */
bool DQTable::contains(vector<SAComponent*> state, vector<SAComponent*> action){
	vector<stateRecord*>::iterator cii;	   
	for(cii=m_table.begin(); cii!=m_table.end(); cii++){		  
	   if( (*cii)->equalState(state) ){
	   		if ( (*cii)->getEntry(action) != (*cii)->getActionsEnd()  ) {		   		  		 
					return true;	   		
	   		}
	   }
	}	  	   
	return false;
}

vector<CQValue*>::iterator DQTable::getEntry(vector<SAComponent*> state, vector<SAComponent*> action){
	vector<stateRecord*>::iterator cii;	   
	for(cii=m_table.begin(); cii!=m_table.end(); cii++){		  
		if((*cii)->equalState(state)){		   		  		 
	   		return ((*cii)->getEntry(action));	   		
		}
	}	  	   
	return  (*cii)->getActionsEnd();
}

vector<stateRecord*>::iterator DQTable::getStateEntry(vector<SAComponent*> state){	
   vector<stateRecord*>::iterator cii;	   
   for(cii=m_table.begin(); cii!=m_table.end(); cii++){		  
	   if((*cii)->equalState(state)){		   		  		 
   			return cii;	   		
	   }
   }	  	   
	return 	m_table.end();
}

string DQTable::print(){
 		ostringstream ret; 		
 		vector<stateRecord*>::const_iterator cii;  	 
   		for(cii=m_table.begin(); cii!=m_table.end(); cii++){	   
		   ret <<(*cii)->print()<<endl;
	   	}
    	return ret.str();
}


void DQTable::initAttributes(){	
}

void DQTable::initAttributes(int id, int n_st, int n_act, double cur, double lr, double dis){
	srand ( time(NULL) ); //init random seed
	int n_states	= n_st; 
    int n_actions	= n_act;
    myid			= id;
	curiosity		= cur;
	learning		= lr;
	discount		= dis;
	decay 			= 1;   
	for(int s=0;s<n_states;s++){
		vector<CQValue*> values;					
		vector<SAComponent*> v_s;
		v_s.push_back( new SAComponent(id,s));
		for(int a=0;a<n_actions;a++){				
			vector<SAComponent*> v_a;
			v_a.push_back(new SAComponent(id,a));
			CQValue *tmpcq = new CQValue(v_a);
			tmpcq->setValue(0.0);
			values.push_back(tmpcq);								
		}			
		m_table.push_back(new stateRecord(v_s,values));		
	}
				
}

/*
int main ( int argc, char **argv ){
	OpLearner* own = new OpLearner(0,1,5,2,true,0.7,0.9); 
	DQTable* teste = new DQTable(own);
	own->setLearning(teste);
	vector< vector<SAComponent*> > v;
	int sts[10][10];		
	for (int i =0;i<25;i++){				
		vector<SAComponent*> v0;
		v0.push_back(new SAComponent(own,i));
		v.push_back(v0);				
		r[i] = -0.01;
				
	}
	r[12] = 1;
	cout<<teste->print()<<endl;
	int s_atual=0;
	int s_n=0;									
	for (int i =0;i<100;i++){				
		vector<SAComponent*> ac = teste->act(v.at(s_atual),DQTable::E_GREEDY);	
		cout <<"estado atual "<< s_atual<<endl;		
		s_atual = s_n;
		if (ac.size()!=0){
			cout <<"acao "<< ac.at(0)->getInformation()<<endl;			
			if (ac.at(0)->getInformation()==1){			
				if (s_n>=1) s_n--;
				else s_n=4; 
			}
			else{ 			
				if (s_n<4) s_n++;
				else s_n=0;
			}		
		}
		cout <<"estado novo "<<s_n<<endl;
		teste->observe(v.at(s_atual), ac ,v.at(s_n), r[s_n]);				
		cout<<teste->print()<<endl;
	}
	
}*/
