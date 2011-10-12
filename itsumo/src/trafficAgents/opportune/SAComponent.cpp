/************************************************************************
                        SAComponent.h.cpp - Copyright denise

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
The original location of this file is /home/gauss/denise/uml-generated-code/SAComponent.cpp
**************************************************************************/

#include "SAComponent.h"

// Constructors/Destructors
//  
SAComponent::SAComponent ( ) {
initAttributes();
}



SAComponent::SAComponent(int own, int info) {
	initAttributes(own, info);
}

SAComponent::~SAComponent ( ) { }


// Private attribute accessor methods
//  


/**
 * Set the value of m_owner
 * @param new_var the new value of m_owner
 */
void SAComponent::setOwner ( int new_var ) {
  m_owner = new_var;
}

/**
 * Get the id of the owner
 * @return the value of m_owner
 */
int SAComponent::getOwner( ) {
  return m_owner;
}

/**
 * Set the value of m_information
 * @param new_var the new value of m_information
 */
void SAComponent::setInformation ( int new_var ) {
  m_information = new_var;
}

/**
 * Get the value of m_information
 * @return the value of m_information
 */
int SAComponent::getInformation ( ) {
  return m_information;
}

string SAComponent::print(){
	ostringstream ret;
	ret<<"("<< m_owner<<","<< m_information<<")";
	return ret.str();
}

// Other methods
//  

void SAComponent::initAttributes ( ) {
  m_information = -1;  
  m_owner = -1;
}

void SAComponent::initAttributes(int own, int info ) {
  m_information = info;
  m_owner = own;
}

/*
 * Tests if the value is in vector. 
 */
bool SAComponent::ownerIsIn( vector<SAComponent*> vec ){
	vector<SAComponent*>::const_iterator it;	
	for (it = vec.begin(); it!= vec.end(); it++){
		if ( this->m_owner == (*it)->getOwner() )
			return true; // value is in vector !
	}	
	return false;
}




bool SAComponent::equals(SAComponent* sa2){
 	if(m_information != sa2->getInformation()) return false;
 	if(m_owner != sa2->getOwner()) return false;   
	return true;
}

bool SAComponent::operator==(const SAComponent& b){
   if( m_information != b.m_information) return false;
   if(m_owner != b.m_owner) return false;   
   return true;
}
//commom functions

bool SAComponent::operator<(const SAComponent& b) {
    if (m_owner==b.m_owner) return (m_information<b.m_information);
    return (m_owner<b.m_owner);
}

bool SAComponent::operator>(const SAComponent& b) {
     if (m_owner==b.m_owner) return (m_information>b.m_information);
     return (m_owner>b.m_owner);
}

bool saCompare(SAComponent* i,SAComponent* j) { 
	return ((*i)<(*j)); 
}

bool saEquals(SAComponent* i,SAComponent* j) { 
	return ((*i)==(*j)); 
}

bool equalSA( vector<SAComponent*> a, vector<SAComponent*>  b) {
	unsigned int sa = a.size();
	if (sa!=b.size()) return false;
	for (unsigned int i=0; i<sa; i++){
		if ( !((*(a[i])) == (*(b[i]))) )return false;
	}	
	return true;
}
vector<SAComponent*> removeComponent( vector<SAComponent*> a, SAComponent*  b) {	
	vector<SAComponent*>::iterator it;	
	for (it = a.begin(); it!= a.end(); it++){	
		if ( (**(it)) == (*(b)) ){ 
			a.erase(it);
			return a;
		}
	}	
	return a;
}

vector<SAComponent*> vector_unique_union( vector<SAComponent*> a, vector<SAComponent*>  b) {
	int sa = a.size();
	int sb = b.size();	
	sort(a.begin(), a.end(),saCompare);
	sort(b.begin(), b.end(),saCompare);
	vector<SAComponent*> nv(sa+sb);
	vector<SAComponent*>::const_iterator ia;
	ia = set_union(a.begin(),a.end(),b.begin(),b.end(),nv.begin(),saCompare);
	nv.resize( ia - nv.begin() );   
	return nv;
}



string printSAVector( vector<SAComponent*> vec ) { 
	string ret="";
	vector<SAComponent*>::const_iterator it;	
	for (it = vec.begin(); it!= vec.end(); it++){
		ret+=(*it)->print();	
		
	}		
	return ret; 
}

int getInfoFromOwner(vector<SAComponent*> vec, int ownerid ) {
	vector<SAComponent*>::const_iterator it;	
	for (it = vec.begin(); it!= vec.end(); it++){
		if ( ownerid ==(*it)->getOwner()){
			return (*it)->getInformation();
		}			
	}
	return 	-1;//invalid id
}

SAComponent* getComponentFromOwner(vector<SAComponent*> vec, int ownerid ) {
	vector<SAComponent*>::const_iterator it;	
	for (it = vec.begin(); it!= vec.end(); it++){
		if ( ownerid ==(*it)->getOwner()){
			return (*it) ;
		}			
	}
//	printf("owner %d not found! in %s \n", ownerid, printSAVector(vec).c_str());
	return NULL;//owner not found
}
