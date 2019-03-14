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

using namespace std;

class group{
    private:
        //inner properties of user class
        string userName;
        string permission;
        vector<group> groupVect;

    public:

};

#endif