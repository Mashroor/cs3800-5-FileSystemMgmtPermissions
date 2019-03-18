//file: cs3800Proj1_directory.h
//author: Mashroor Rashid, CS3800 Sec B
//Directory class; this class intends to emulate the directories in linux. This class acts on the base layer for the program.
#ifndef CS3800PROJ1_DIRECTORY_H
#define CS3800PROJ1_DIRECTORY_H
#define BOLDCYAN "\033[1m\033[36m"
#define RESET "\033[0m"
#include <vector>
#include <cstring>
#include <string>
#include <ctime>
#include <iostream>
#include "cs3800Proj1_file.h"
#include "cs3800Proj1_user.h"

using namespace std;

class directory{
    private:
        //tracking of inner objects
        vector<directory*> innerDirectories;
        vector<file> innerFiles;

        directory* parent;
        //inner properties of directory class
        string permissions;
        string userName;
        string groupName;
        int fileSize;
        string timestamp;
        string path;
        string directoryName;

    public:
        directory(): parent(nullptr) {}; //used in mkdir(), to allocate a new directory obj
        directory(string name, directory &newParent, user &dirOwner); //used for any time a diretory is made
        ~directory(); //since making a new directory, need a way to deref, avoid mem leaking
        
        //getters, these will return any values in private section
        string getPermissions() const{return permissions;}
        string getUserName() const{return userName;}
        string getGroupName() const{return groupName;}
        int getFileSize()const{return fileSize;}
        string getTimestamp() const{return timestamp;}
        string getPath() const{return path;}
        string getDirectoryName() const{return directoryName;}
        int getSize() const{return innerDirectories.size();}
        int getFilesSize() const{return innerFiles.size();}
        directory* getParent() const{return parent;}
        vector<directory*> getDirectoryVect() const{return innerDirectories;}
        vector<file> getFilesVect() const{return innerFiles;}
        
        //Func: setTimestamp()
        //Pre: A directory obj must be acted upon. 
        //Post: Sets the timestamp varialbe with a string.
        void setTimestamp();
        //Func: setName(string)
        //Pre: A directory must call the function. Must take a string of any length as the parameter.
        //Post: Will set the name variable of the object.
        void setName(string name){directoryName = name;}
        //Func: setPermissions(string)
        //Pre: A directory must be the calling object. a length 3, string of a number should be passed in.
        //Post: Permission will be set according the the string passed in.
        void setPermissions(string permCode);
        //
        //
        //
        void setUserName(string newName){userName = newName;}
        //
        //
        //
        void setGroupName(string newName){groupName = newName;}
        //Func: cd(string, directory*)
        //Pre: A directory should take in a name string along with a pointer to a new parent
        //Post: parent will be set for the old object, passed name will be the new currDirPtr
        directory* cd(string objName, directory* newParent);
        //Func: pwd()
        //Pre: Directory should call the function
        //Post: Display the path variable to the screen
        void pwd();        
        //Func: ls()
        //Pre: Directory object should be initialized
        //Post: Print the current directory contents to the screen
        void ls(user owner);
        //Func: ls_l()
        //Pre: Directory object should be initialized
        //Post: Print the current directory contents and details to screen
        void ls_l(user owner);
        //Func: chmod (string0, string1)
        //Pre: Directory should take in a permission code, and directory name to act on
        //Post: Set permission variable using setPermission()
        void chmod(string permCode, string dirName);
        //Func:mkdir(string)
        //Pre: Directory should take in the name of the directory to be created
        //Post: Creates a new directory and stores it in the directory vector
        void mkdir(string newDirName, user dirOwner);
        //Func: touch(string)
        //Pre: Directory should take in the name of a file to be created, that is not already created
        //Post: File should be created and stored in the files vector
        void touch(string newFileName, user fileOwner);
        //Func: rmdir(string)
        //Pre: Directory should have other directories within its directory vector
        //Post: Directory of name <dirToDel> will be removed and de-allocated from memory
        void rmdir(string dirToDel);
        //Func: rm(string)
        //Pre: Directory should have files within the files vector
        //Post: a file of name <fileToDel> will be removed from the Files directory
        void rm(string fileToDel);

};

#endif