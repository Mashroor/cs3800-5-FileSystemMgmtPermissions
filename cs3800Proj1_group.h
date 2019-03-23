//file: cs3800Proj1_group.h
//author: Mashroor Rashid, CS3800 Sec B
//Group class; This class intends to simulate a group, and its permission set.
#ifndef CS3800PROJ1_GROUP_H
#define CS3800PROJ1_GROUP_H

#include<iostream>
#include<cstring>
#include<string>
#include<ctime>
#include<vector>

using namespace std;

class group{
    private:
        //inner properties of group class
        string groupName;

    public:
        group(): groupName("null") {}; //constructors
        group( const group& source) : groupName(source.groupName){}
        group(string name);
        string getGroupName() const{return groupName;} //setters and getters. nothing special here
        void setGroupName(string newName){groupName = newName;}
};

#endif