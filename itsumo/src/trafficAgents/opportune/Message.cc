#include "Message.hh"

const unsigned int Message::UNKNOWN             		= 0;
const unsigned int Message::REQUEST_LAST_STATE_INFO  	= 1; 
const unsigned int Message::REQUEST_LAST_ACTION_INFO 	= 2;  	 
const unsigned int Message::INFORM_LAST_STATE_INFO		= 3;
const unsigned int Message::INFORM_LAST_ACTION_INFO  	= 4;
const unsigned int Message::REQUEST_CURRENT_STATE_INFO  = 5; 
const unsigned int Message::REQUEST_CURRENT_ACTION_INFO = 6;  	 
const unsigned int Message::INFORM_CURRENT_STATE_INFO	= 7;
const unsigned int Message::INFORM_CURRENT_ACTION_INFO  = 8;
const unsigned int Message::PROPOSE_JOINT_ACTION		= 9;
const unsigned int Message::AGREE_JOINT_ACTION  		= 10;
const unsigned int Message::REFUSE_ACTION_INFO  		= 11;
const unsigned int Message::REFUSE_STATE_INFO   		= 12;
const unsigned int Message::REFUSE_JOINT_ACTION 		= 13;


Message::Message(unsigned int to, unsigned int sender, unsigned int type, unsigned int info, double value){
	_to_ 		= to;
	_sender_	= sender;
	_type_ 		= type;
	_content_ 	= info;
	_value_ 	= value;			
}

Message::~Message(){
	
}
/**
 * Returns the id of the sender agent. 
 */
unsigned int Message::getSender(){
	return _sender_;
}

/**
 * Returns the message Performative. Denotes the 
 * type of the communicative act of the message.
 */
unsigned int Message::getPerformative(){
	return _type_;
}

/**
 * Returns the message content
 */
unsigned int Message::getContent(){
	return _content_;
}

/**
 * Returns the message value
 */
double Message::getValue(){
	return _value_;
}	

/**
 * Defines the message sender
 */
void Message::setSender(unsigned int sender){
	_sender_ = sender;
}

/**
 * Defines the message performative according to its type:
  
 * REQUEST_STATE_INFO  : request state information
 * REQUEST_ACTION_INFO : request action information
 * INFORM_STATE_INFO   : sends state information
 * INFORM_ACTION_INFO  : sends action information
 * PROPOSE_JOINT_ACTION: propose joint action
 * AGREE_JOINT_ACTION  : accept the joint action
 * REFUSE_ACTION_INFO  : refuses action information
 * REFUSE_STATE_INFO   : refuses state information
 * REFUSE_JOINT_ACTION : declines the proposed joint-action
 * UNKNOWN			   : unknown/invalid message 
 * 
 */
void Message::setPerformative(unsigned int type){
	_type_ = type;
}

/**
 * Defines the message content. Implemented as an 
 * integer number that represents an action or a
 * state (depending on the message type).
 */
void Message::setContent(unsigned int info){
	_content_ = info;
}	

/**
 * Defines the message value.
 */
void Message::setValue(double value){
	_value_ = value;
}	

/**
 * Converts to a string with values separated by white spaces:
 * _to_ _sender_ _type_ _content_ _value_ 
 */
string Message::toString(){
	ostringstream ret; 		 	  	 	   
	ret <<";" <<_to_<<","<<_sender_<<","<<_type_<<","<<_content_<<","<<_value_<<";";	 	
	return ret.str();    
}
