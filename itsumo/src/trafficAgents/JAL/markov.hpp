// Developed by Eduardo Wisnieski Basso - http://www.inf.ufrgs.br/~ewbasso and
// Bruno Castro da Silva - http://www.inf.ufrgs.br/~bcs
// Expanded by Denise de Oliveira  http://www.inf.ufrgs.br/~edenise
// NO WARRANTY is given over this source code. You are free to use or change
// this source code as you wish.
//------------------------------------------------------------------------------
#ifndef MARKOV_HPP
#define MARKOV_HPP

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// when initializing a new model based on a already existing, defines how
// much of the original confidence (count) will be copied
#define RATIO_OF_ORIGINAL_COUNT (9999999)


// Balance between deltaT and deltaM. balanceM=0.0 ignores deltaM.
#define BALANCE_SPACE   0.0
// Confidence: 0 for standard; 1 for standard non-zero;
//             2 to use factor; 3 to consider average count.
#define CONFIDENCE      1

#define USE_RESPONSIBILITY (0)



/// Implementation of learning algorighms in markov environments.
//------------------------------------------------------------------------------
namespace markov {
	
typedef double   number;

/// Basic reinforcement learning mechanism on Markov Decision Processes.
//------------------------------------------------------------------------------
class mdp {
	
public:
    /// Initializing constructor.
    mdp( ) {
        stateCount = 0;
        actionCount = 0;
    }

    /// Destructor. Do nothing.
    ~mdp() {};

    /// Get a code for this class.
    static int getType() { return 0; };

    /// Test if this class implements the specified class type.
    virtual bool testType( int _type ) { return _type == getType(); };

    /// Get total number of states.
    int getStateCount( ) { return stateCount; }
    
    /// Get number of possible actions.
    int getActionCount( ) { return actionCount; }

    /// Initialize the policy and the models.
    virtual bool initialize(markov::number ***_initialTransitionModel=NULL, 
			    markov::number **_initialRewardModel=NULL,
			    double **_initialCount=NULL) = 0;

    /// Decide what to do at the specified state.
    virtual int act( int _state, bool _greedy=false ) = 0;

    /// Adjust the decision process by observation.
    virtual bool observe( int _state, int _action,
                          int _newState, number _reward ) = 0;

    /// Get the action value in the specified state.
    virtual double getActionValue(int _state, int _action) = 0;

    virtual string print(){ return "";}
    
    virtual string printQTable(){ return "";}
    virtual string printSimpleQTable(){ return "";}
    
    virtual int getBestGreedyAction(int state){ return 0;}
    virtual float getGreedyQValue(int state){ return 0;}

protected:
    int         stateCount;     ///< Total number of states.
    int         actionCount;    ///< Number of possible actions.

};
//------------------------------------------------------------------------------


/// Mechanism that implements the Q-Learning algorithm.
//------------------------------------------------------------------------------
class qlearning : public mdp {
public:
    /// Initializing constructor.
    qlearning( ) {
        actionValue = NULL;
        stateCount = 0;
        actionCount = 0;
        learning = 0.0;
        discount = 0.0;
        curiosity = 0.05; //default value
        //totalExp = 0;
	maxTemp = 100;
	lastTemp = maxTemp;
	decayFactor = 0.9; //temperature will be multiplied by this factor at each action selection step
        //maxNumberOfExperiences = 1000;
    }

    /// Force destruction.
    ~qlearning( ) {
        destroy();
    }

    /// Get a code for this class.
    static int getType() { return 1; };

    /// Test if this class implements the specified class type.
    virtual bool testType( int _type ) {
        if ( mdp::testType( _type ) ) {
            return true;
        }
        return _type == getType();
    };

    /// Create the Q-Learning mechanism.
    bool create( int _stateCount, int _actionCount, number _discount = 0.9,
                 number _curiosity = 0.05, number _learning = 0.1 ) {
		cout << "_stateCount: " << _stateCount<< "_actionCount: " << _actionCount <<endl;
        if ( ( _stateCount <= 0 ) || ( _actionCount <= 0 ) ) {
            return false;
        }
        
        // Allocate action-values. The remember map for action-values map is
        // only allocate if necessary, at remember().
        actionValue = new number*[_stateCount];
        numActionValue = new number*[_stateCount];
         
        for ( int i = 0; i < _stateCount; i++ ) {
            actionValue[i] = new number[_actionCount];
            numActionValue[i] = new number[_actionCount];
        }

        stateCount = _stateCount;
        actionCount = _actionCount;
        
        // Set coefficients.
        learning = _learning;
        discount = _discount;
        curiosity = _curiosity;

        // Initialize the Q-Values.
        initialize();

        return true;
    }

    /// Destroy the mechanism.0.0;//
    void destroy( ) {

        if ( actionValue != NULL ) {
            for ( int i = 0; i < stateCount; i++ ) {
                if ( actionValue[i] != NULL ) {
                    delete [] actionValue[i];
                }
            }
            delete []actionValue;
            actionValue = NULL;
        }

        stateCount = 0;
        actionCount = 0;
    }

    /// Initialize the Q-values.
    bool initialize(markov::number ***_initialTransitionModel=NULL, markov::number **_initialRewardModel=NULL, 
		    double **_initialCount=NULL ) {
        if ( actionValue == NULL ) {
            return false;
        }
        for ( int i = 0; i < stateCount; i++ ) {
            for ( int j = 0; j < actionCount; j++ ) {
                actionValue[i][j] = 0.0;//double(rand())/RAND_MAX;
                numActionValue[i][j] = 0;                
            }
        }
        return true;
    }
       
    // first, the sum of the exp(Q/T) terms:
    double calculateSigma(int state, float QTEMPERATURE ){
     double sigma = 0;
     for ( int i = 0; i < actionCount; i++ ) {   
      sigma = sigma + exp(actionValue[state][i]/QTEMPERATURE);
     }
     return sigma;
    }

    // suggest an action using Bolzman
    int suggestBoltz (int state, float QTEMPERATURE ){
     double sigma = calculateSigma(state, QTEMPERATURE);
     float p = ((double)rand()/(double)RAND_MAX);    
     cout << "random nb: "<<p<<endl;
     float sum = 0.0;
     int i = -1;
     while ((sum < p)&&(i< actionCount-1)){
    	 i++;
    	 double prob = exp(actionValue[state][i]/QTEMPERATURE)/ sigma;
    	 cout<<"probability is "<<prob<<" for state"<<state<<" action"<<i<<endl;
    	 sum = sum + prob;    	 
     }
     // just hit p
     return i;
    }    
    
    float reasonableTemperature(){
    	double minTemp = 0.1;
    	//double hereWas_maxTemp = 0.5;
/*
    	if ( totalExp >= maxNumberOfExperiences){
    		return minTemp;
    	}
    	else{
    		float e = totalExp/maxNumberOfExperiences;
    		return (minTemp + (1-e)*(maxTemp-minTemp));      
    	}
    }
*/
		if (lastTemp > minTemp){
		    lastTemp = lastTemp * decayFactor;
		}
		cout << "temperature: " << lastTemp <<endl;
		return (lastTemp);
    }

    int getBestGreedyAction(int state){
    	double bestQ = 0.0;
    	int bestAction = -1;
    	for ( int i = 0; i < actionCount; i++ ) {        	
    		//cout<<"action value["<<_state<<"]["<<i<<"]:"<<actionValue[_state][i]<<endl;
    		if ( actionValue[state][i] > bestQ ) {            	
    			bestAction = i;
    			bestQ = actionValue[state][i];
    			//cout<<"best action"<<i<<endl;
    		}
    	}
    	if (bestAction == -1){ //all actions have q-value equal to zero
    		return rand() % actionCount; //choose random action
    	}    	
    	return bestAction;
    }
    
    
    float getGreedyQValue(int state){
    	float bestQ = 0.0;
    	int bestAction = 0;
    	for ( int i = 0; i < actionCount; i++ ) {        	
    		//cout<<"action value["<<state<<"]["<<i<<"]:"<<actionValue[state][i]<<endl;
    		if ( actionValue[state][i] > bestQ ) {            	
    			bestAction = i;
    			bestQ = actionValue[state][i];
    			//cout<<"best action"<<i<<endl;
    		}
    	}
    	return bestQ;
    }
    
    int getEpsilonGreedyAction(int state){        
        if ( rand() < ( RAND_MAX * curiosity) ) {                
            	//cout<<"select random action"<<endl;
            	//curiosity = 0.95*curiosity; // decay epsilon 5%
                return rand() % actionCount;
        }
        else{ 
        	return getBestGreedyAction(state);
        }         
    }
    
    /// Decide which action to do in the current state. Return -1 if the 
    /// mechanism is not created or if the current state is invalid.
    int act( int _state, bool _greedy=false ) {
        if ( actionValue == NULL ) {        	
            return -1;
        }
        if ( ( _state < 0 ) || ( _state >= stateCount ) ) {         	
            return -1;
        }
        int   bestAction = 0;    
        //bestAction = suggestBoltz(_state,reasonableTemperature());
    	bestAction = getEpsilonGreedyAction(_state);
         
        numActionValue[_state][bestAction]++;        
		
		//cout << "selected action: " << bestAction<<endl<<endl;
        return bestAction;
    }

    /// Adjust the Q-values by observing the environment. Return false if the
    /// mechanism is not created or if the current state, the action or the new
    /// state are invalid.
    bool observe( int _state, int _action, int _newState, number _reward ) {
        if ( actionValue == NULL ) {
            return false;
        }
        if ( ( _state < 0 ) || ( _state >= stateCount ) ||
             ( _newState < 0 ) || ( _newState >= stateCount ) ||
             ( _action < 0 ) || ( _action >= actionCount ) ) {
            return false;
        }

        // Get the Q-value for the best action at the new state.
        number bestQ = actionValue[_newState][0];
        for ( int i = 1; i < actionCount; i++ ) {
            if ( actionValue[_newState][i] > bestQ ) {
                bestQ = actionValue[_newState][i];
            }
        }

        // Adjust the Q-value for the current state and action.
        actionValue[_state][_action] += learning * ( _reward + discount * bestQ - actionValue[_state][_action] );

        return true;
    }

    /// Get action value.
    double getActionValue(int _state, int _action)
    {
        return actionValue[_state][_action];
    }

    string printQTable(){    	
   	    ostringstream ret;
    	for ( int i = 0; i < stateCount; i++ ) {
    		for ( int j = 0; j < actionCount; j++ ) {
    			ret<<"Q("<<i<<","<<j<<")="<<actionValue[i][j]<<"\n";                
    		}
    	}
    	return ret.str();
    }
    
    string printSimpleQTable(){    	
   	    ostringstream ret;
    	for ( int i = 0; i < stateCount; i++ ) {    		
    		for ( int j = 0; j < actionCount; j++ ) {//x y z
    			ret<<i<<"\t"<<j<<"\t"<<actionValue[i][j]<<"\n"; //"action qvalue "                
    		}    		
    	}
    	return ret.str();
    }

public:
	number    **actionValue;    ///< Map of action-values (Q).
	number    **numActionValue; 	///< Map of the number of times the action-values have being visited.
    number      learning;       	///< Learning rate.
    number      discount;       		///< Discount of future rewards.
    number      curiosity;      		///< Agent's curiosity rate.
    int      	totalExp; 				//total number of experiences
    number      lastTemp;            //last temperature (to be decreased in the Boltzman equation)
    number      maxTemp;             // max temperature for the Boltzman equation
    number      decayFactor;         // temperature will be multiplied by this factor at each action selection step
    //int 		maxNumberOfExperiences; //maximum number of experiences 
};
//------------------------------------------------------------------------------


}; // Namespace markov.
//------------------------------------------------------------------------------

#endif // MARKOV_HPP
