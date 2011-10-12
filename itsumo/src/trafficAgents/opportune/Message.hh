
#ifndef MESSAGE_HH_
#define MESSAGE_HH_
#include <sstream>

using namespace std;

class Message
{
private:
	unsigned int _to_; 
	unsigned int _sender_;
	unsigned int _type_;
	unsigned int _content_;
	double _value_;	
	
public:	
	Message(unsigned int to, unsigned int sender, unsigned int type, unsigned int info, double value);
	virtual ~Message();
	unsigned int getSender();
	unsigned int getPerformative();
	unsigned int getContent();
	double getValue();	
	void setSender(unsigned int sender);
	void setPerformative(unsigned int type);
	void setContent(unsigned int info);
	void setValue(double value);	
	
	static const unsigned int UNKNOWN;
	static const unsigned int REQUEST_LAST_STATE_INFO; 
	static const unsigned int REQUEST_LAST_ACTION_INFO;  	 
	static const unsigned int INFORM_LAST_STATE_INFO;
	static const unsigned int INFORM_LAST_ACTION_INFO;
	static const unsigned int REQUEST_CURRENT_STATE_INFO; 
	static const unsigned int REQUEST_CURRENT_ACTION_INFO;  	 
	static const unsigned int INFORM_CURRENT_STATE_INFO;
	static const unsigned int INFORM_CURRENT_ACTION_INFO;
 	static const unsigned int PROPOSE_JOINT_ACTION;
 	static const unsigned int AGREE_JOINT_ACTION;
 	static const unsigned int REFUSE_ACTION_INFO;
 	static const unsigned int REFUSE_STATE_INFO;
 	static const unsigned int REFUSE_JOINT_ACTION;
	string toString();
};

#endif /*MESSAGE_HH_*/
