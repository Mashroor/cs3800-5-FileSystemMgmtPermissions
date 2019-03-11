//file: cs3800Proj1_file.h
//author: Mashroor Rashid, CS3800 Sec B
//Directory class; this class intends to be a dummy file. Stores a few parameters to be manipulated
#ifndef CS3800PROJ1_FILE_H
#define CS3800PROJ1_FILE_H

#include<iostream>
#include<cstring>
#include<string>
#include<ctime>

using namespace std;

class file{
    private:
        //inner properties of file class
        string permissions;
        string userName;
        int fileSize;
        string timestamp;
        string path;
        string fileName;
    public:
        //constructor, only used in touch, no need for destructor since new mem not allocated
        file(string name);
        //getters, returns any values in the private section
        string getPermissions() const{return permissions;}
        string getUserName() const{return userName;}
        int getFileSize() const{return fileSize;}
        string getTimestamp() const{return timestamp;}
        string getFileName() const{return fileName;}
        
        //Func: setTimestamp()
        //Pre: A file must be acted upon.
        //Post: Timestamp variable will be reset
        void setTimestamp();
        //Func: setPermissions()
        //Pre: A file must be the calling object, the permission must be a length 3 string, of only numbers
        //Post: The permission variable will be set to a permission string
        void setPermissions(string PermCode);
        //this obj does not need any extra functions, since this is only here as a dummy obj
};

#endif