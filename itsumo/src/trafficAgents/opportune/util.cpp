#include "util.h"


using namespace std;
int getNextInt(queue<string> *q){
	int ret = atoi(q->front().c_str());
	q->pop();
	return ret;   
}

double getNextDouble(queue<string> *q){
	double ret = atof(q->front().c_str());
	q->pop();
	return ret;   
}
/*
 * replace a char in "n" ocurrencies
 */ 
void replaceNTimes(string& modifyMe, string findMe, string newChars, int n){
  // Look in modifyMe for the "find string"
  // starting at position 0
  // Did we find the string to replace?
  	unsigned int i, count =0;
  	do{
  		i = modifyMe.find(findMe, 0);
    	// Replace the find string with newChars
    	if(i != string::npos){
    		modifyMe.replace(i,findMe.size(),newChars);
    		++count;
    	}
    	
	}while( ( i != string::npos)&&(count <n) );
    
}

void replaceChars(string& modifyMe, string findMe, string newChars){
  // Look in modifyMe for the "find string"
  // starting at position 0
  // Did we find the string to replace?
  	int i;
  	do{
  		i = modifyMe.find(findMe, 0);
    	// Replace the find string with newChars
    	if(i != string::npos)
    		modifyMe.replace(i,findMe.size(),newChars);
    	
	}while(i != string::npos);
    
}
