#ifndef SACOMPONENT_H
#define SACOMPONENT_H

#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

/**
  * class SAComponent
  * The SAComponent represents an action or a state. The owner is the point to the
  * agent responsable of perceiving the state or performing the action the
  * information is an integer number representing a state/action of the agent.
  */

class SAComponent
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SAComponent( );
  
  SAComponent(int own, int info);

  /**
   * Empty Destructor
   */
  virtual ~SAComponent( );


  // Public attribute accessor methods
  //  

  /**
   * @return int
   */
  int getInfo( );


  // Private attribute accessor methods
  //  


  /**
   * Set the value of m_owner
   * @param new_var the new value of m_owner
   */
  void setOwner(int new_var );

  /**
   * Get the value of m_owner
   * @return the value of m_owner
   */
  int getOwner( );


  /**
   * Set the value of m_information
   * @param new_var the new value of m_information
   */
  void setInformation ( int new_var );

  /**
   * Get the value of m_information
   * @return the value of m_information
   */
  int getInformation( );
  
  string print();
  bool equals(SAComponent* sa2);
	bool operator==(const SAComponent &b);

	bool operator<(const SAComponent &b );
	bool operator>(const SAComponent &b );

   bool ownerIsIn( vector<SAComponent*> vec );	
   	
	
	
	
private:

  // Private attributes
  //  
  int m_owner;
  int m_information;

  void initAttributes( ) ;
  void initAttributes(int own, int info );
 

};
bool saCompare( SAComponent* i, SAComponent* j );
bool saEquals( SAComponent* i, SAComponent* j );
bool equalSA( vector<SAComponent*> a, vector<SAComponent*>  b);
vector<SAComponent*> removeComponent( vector<SAComponent*> a, SAComponent*  b);
vector<SAComponent*> vector_unique_union( vector<SAComponent*> a, vector<SAComponent*>  b); 
string printSAVector( vector<SAComponent*> vec );
int getInfoFromOwner(vector<SAComponent*> vec, int ownerid );
SAComponent* getComponentFromOwner(vector<SAComponent*> vec, int ownerid );
 
#endif // SACOMPONENT_H


