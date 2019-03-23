//file: cs3800Proj1_user.h
//author: Mashroor Rashid, CS3800 Sec B
//User class; This class intends to model a user in UNIX. Should contain permissions to be checked.
#ifndef CS3800PROJ1_USER_H
#define CS3800PROJ1_USER_H

#include<iostream>
#include<cstring>
#include<string>
#include<ctime>
#include<vector>
#include "cs3800Proj1_group.h"

using namespace std;

class user{
    private:
        //inner properties of user class
        string userName;
        vector<group> groupVect;

    public:
        user(string name); //constructor

        string getUserName() const{return userName;} //getters
        vector<group>& getGroupVector() {return groupVect;}

        void addGroup(string groupName); //function to add groups, just pushes to vector
};

#endif