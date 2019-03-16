//file: cs3800Proj1_user.cpp
//author: Mashroor Rashid, CS3800 Sec B
//Group class, definititons and code
#include "cs3800Proj1_user.h"
using namespace std;

user::user(string name){
    userName = name;
    group defaultGroup("default"); //add to default group
    groupVect.push_back(defaultGroup);
}

void user::addGroup(string groupName){
    group newGroup(groupName);
    groupVect.push_back(newGroup);
}