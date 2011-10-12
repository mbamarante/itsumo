#ifndef UTIL_H_
#define UTIL_H_
#include <algorithm>
#include <sstream>
#include <cstring>
#include <iostream>
#include <queue>

using namespace std;

int getNextInt(queue<string> *q);
double getNextDouble(queue<string> *q);
void replaceNTimes(string& modifyMe, string findMe, string newChars, int n);
void replaceChars(string& modifyMe, string findMe, string newChars);

#endif /*UTIL_H_*/
