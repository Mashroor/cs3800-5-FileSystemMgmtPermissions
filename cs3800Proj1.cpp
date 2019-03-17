//file: cs3800Proj1.cpp
//author: Mashroor Rashid, CS3800 Sec B
//main executable code, to be compiled
#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <cstring>
#include <string>
#include <cctype>

#define BOLDCYAN "\033[1m\033[36m"
#define RESET "\033[0m"

#include "cs3800Proj1_file.h"
#include "cs3800Proj1_directory.h"
#include "cs3800Proj1_user.h"
#include "cs3800Proj1_group.h"

int main(){
    //while loop control, used in exit
    bool control = true;
    //used for if searching through a vector for an obj
    bool found;
    string input;
    directory currDir; //create a new directory, as a base
    currDir.setName("/"); // This is the root, we cant move above this
    directory* currDirPtr = &currDir; //pointer, this will allow us to move between directories via pointing
    
    vector<user> userVect; //store all users in this. we can move around in this.
    user defaultUser("user"); //set a default user. of Group default.
    userVect.push_back(defaultUser); //throw this in to keep track of current user. start at default.
    user* currUserPtr = &defaultUser; //pointer to current user. we will be changing this
    
    vector<group> groupsVect; // To store groups, and add users to them later on
    groupsVect.push_back(currUserPtr->getGroupVector()[0]); //store the first group. when creating, add to this

    //parsing code for user
    while(control){ 
    cout << BOLDCYAN << currUserPtr->getUserName() << ":~" << RESET << currDirPtr->getPath() << "$ "; //to emulate linux, print this.
    getline(cin, input);
    istringstream ss(input);
	string token;

	vector<string> query;
	while(getline(ss, token, ' ')) {
		query.push_back(token);
	}
    //begin deciphering the query, or input
    if(!query.empty()){
            if(query[0] == "cd"){
                found = false;
                if(query[1] == ".."){ //if cd at root, do nothing, but don't display error message
                    if(currDirPtr->getParent() == nullptr){
                        found = true;
                    }
                    else{
                            if(currDirPtr->getParent() != nullptr){ //if inside a directory that isn't root
                                currDirPtr = currDirPtr->getParent();
                                found = true;
                            }
                    }
                }else{
                    for(int i = 0; i < currDirPtr->getSize(); i++){ //goes into a directory, down 1
                        if(query[1] == currDirPtr->getDirectoryVect()[i]->getDirectoryName()){
                            currDirPtr = currDirPtr->cd(query[1], currDirPtr);
                            found = true;
                        }
                    }
                }
                if(!found){ //directory not found message
                    cout << "-mash: cd: " 
                         << query[1] 
                         << ": No such directory" 
                         << endl;
                }
            }else{
                if(query[0] == "pwd"){
                    currDirPtr->pwd();
                }else{
                    if(query[0] == "ls"){
                        if(query.size()>1 && query[1] == "-l"){ //catches the -l as part of query, detailed list
                            currDirPtr->ls_l(*currUserPtr);
                        }else{
                            currDirPtr->ls(*currUserPtr); //basic ls, tabbed
                        }
                    }else{
                        if(query[0] == "mkdir"){ //allocates a new folder in memory, needs deleting in rmdir
                            found = false;
                            for (int i = 0; i < currDirPtr->getSize(); i++){
                                if( currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[1]){
                                    found = true;
                                }
                            }
                            for (int i = 0; i < currDirPtr->getFilesSize(); i++){
                                if(currDirPtr->getFilesVect()[i].getFileName() == query[1]){
                                    found = true;
                                }
                            }
                            if(found == true){ //error message if not found
                                cout << "-mash: mkdir: " 
                                     << query[1] 
                                     << ": file or directory exists" 
                                     << endl;
                            }else{
                                currDirPtr->mkdir(query[1], *currUserPtr);
                            }
                        }else{
                            if(query[0] == "touch"){ //create a file. dummy obj
                                found = false;
                                for (int i = 0; i < currDirPtr->getSize(); i++){
                                    if( currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[1]){
                                        found = true;
                                    }
                                }
                                if(found == true){ //error message if not found
                                    cout << "-mash: touch: " 
                                         << query[1] 
                                         << ": directory exists" 
                                         << endl;
                                }else{
                                    currDirPtr->touch(query[1], *currUserPtr);
                                }
                            }else{
                                if(query[0] == "rmdir"){ //remove a directory, needs to call destructor. avoid mem leak
                                    found = false; //search for if directory or file exists first, set false to jump out when found
                                    for (int i = 0; i < currDirPtr->getSize(); i++){
                                        if( currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[1]){
                                            currDirPtr->rmdir(query[1]);
                                            found = true;
                                        }
                                    }
                                    if(found == false){ //error message if not found
                                        cout << "-mash: rmdir: " 
                                             << query[1] 
                                             << ": No such directory" 
                                             << endl;
                                    }
                                }else{
                                    if(query[0] == "rm"){ //for files, nothing special. same search mech
                                        found = false;
                                        for (int i = 0; i < currDirPtr->getFilesSize(); i++){
                                            if(currDirPtr->getFilesVect()[i].getFileName() == query[1]){
                                                currDirPtr->rm(query[1]);
                                                found = true;
                                            }
                                        }
                                        if(found == false){
                                            cout << "-mash: rm: " 
                                                 << query[1] 
                                                 << ": No such file" 
                                                 << endl;
                                        }
                                    }else{
                                        if(query[0] == "chmod"){ //requires 3 items in query vect. check
                                            found = false;
                                            for (int i = 0; i < currDirPtr->getSize(); i++){
                                                if(query.size() == 3){ //make sure we have 3 things
                                                    if( currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[2]){
                                                        currDirPtr->chmod(query[1], query[2]);
                                                        found = true;
                                                    }
                                                }
                                            }
                                            for (int i = 0; i < currDirPtr->getFilesSize(); i++){
                                                if(query.size() == 3){ //make sure we have 3 things
                                                    if(currDirPtr->getFilesVect()[i].getFileName() == query[2]){
                                                        currDirPtr->chmod(query[1], query[2]);
                                                        found = true;
                                                    }
                                                }
                                            }
                                            if(found == false && query.size() == 3 ){ //if file/dir doesn't exist
                                                cout << "-mash: chmod:" 
                                                     << query[2] 
                                                     << ": No such file or directory" 
                                                     << endl;
                                            }else{
                                                if(query.size() < 3 || query.size() > 3){ //invalid definition of chmod
                                                    cout << "-mash: chmod"  
                                                        << ": No such definition of chmod" 
                                                        << endl;
                                                }
                                            }
                                        }else{
                                            if(query[0] == "exit" || query[0] == "quit"){
                                                control = false;
                                            }else{
                                                if(query[0][0] == '.' && query[0][1] == '/'){ // remove execute command
                                                    string execName = query[0];
                                                    for(int i = 0; i < 2; i++){
                                                        execName.erase(execName.begin());
                                                    }
                                                    found = false;
                                                    for (int i = 0; i < currDirPtr->getFilesSize(); i++){ //find the file here. Search.
                                                        if(currDirPtr->getFilesVect()[i].getFileName() == execName){
                                                            cout << execName
                                                                 << " has been executed."
                                                                 << endl;
                                                            found = true;
                                                        }
                                                    }
                                                    for (int i = 0; i < currDirPtr->getSize(); i++){ //make sure to check directories next, new error message
                                                        if( currDirPtr->getDirectoryVect()[i]->getDirectoryName() == execName){
                                                            cout << "-mash: " 
                                                                 << query[0] 
                                                                 << ": is a directory" 
                                                                 << endl;
                                                            found = true;
                                                        }
                                                    }
                                                    if(query[0].size() == 2){ //if empty command
                                                        cout << "-mash: " 
                                                             << query[0] 
                                                             << ": is a directory" 
                                                             << endl;
                                                    }
                                                    else{
                                                        if(found == false){ //no file to execute
                                                            cout << "-mash: " 
                                                                << query[0] 
                                                                << ": No such file or directory" 
                                                                << endl;
                                                        }
                                                    }
                                                }else{
                                                    if(query[0] == "whoami"){
                                                        cout << currUserPtr->getUserName() << '\n';
                                                    }else{
                                                        if(query[0] == "groups"){
                                                            for(int i = 0; i < currUserPtr->getGroupVector().size(); i++){
                                                                cout << currUserPtr->getGroupVector()[i].getGroupName() << " ";
                                                            }
                                                            cout << endl;
                                                        }else{
                                                            if(query[0] == "groupadd"){
                                                                found = false;
                                                                for(int i = 0; i < currUserPtr->getGroupVector().size(); i++){
                                                                    if(query[1] == currUserPtr->getGroupVector()[i].getGroupName()){
                                                                        found = true;
                                                                    }
                                                                }
                                                                if(found){
                                                                    cout << "groupadd: group '"
                                                                         << query[1]
                                                                         << "' already exists\n";
                                                                }else{
                                                                    if(query.size() != 2){
                                                                        cout << "-mash: groupadd"  
                                                                             << ": No such definition of groupadd" 
                                                                             << endl;
                                                                    }else{
                                                                        group newGroup(query[1]);
                                                                        groupsVect.push_back(newGroup);
                                                                    }
                                                                }
                                                            }else{
                                                                if(query[0] == "useradd"){
                                                                    found = false;
                                                                    for(int i = 0; i < userVect.size(); i++){
                                                                        if(query[1] == userVect[i].getUserName()){
                                                                            found = true;
                                                                        }
                                                                    }
                                                                    if(found && query.size() == 2){
                                                                        cout << "useradd: user '"
                                                                             << query[1]
                                                                             << "' already exists\n";
                                                                    }else{
                                                                        if(query.size() != 2){
                                                                            cout << "-mash: useradd"  
                                                                                 << ": No such definition of useradd" 
                                                                                 << endl;
                                                                        }else{
                                                                            user newUser(query[1]);
                                                                            userVect.push_back(newUser);
                                                                        }
                                                                    }
                                                                }else{
                                                                    if(query[0] == "userdel"){
                                                                        if(query[1] == "-G"){
                                                                            found = false;
                                                                            for(int i = 0; i < userVect.size(); i++){
                                                                                for(int j = 0; j < userVect[i].getGroupVector().size(); j++){
                                                                                    if(query[1] == userVect[i].getGroupVector()[j].getGroupName() && query[2] == userVect[i].getUserName()){
                                                                                        userVect[i].getGroupVector().erase(userVect[i].getGroupVector().begin()+j);
                                                                                        found = true;
                                                                                    }
                                                                                }
                                                                            }
                                                                        if(found == false && query.size() == 3){
                                                                            cout << "userdel -G: user '"
                                                                                 << query[1]
                                                                                 << "' is not a part of specified group\n";
                                                                        }else{
                                                                            if(query.size() != 3){
                                                                                cout << "-mash: userdel"  
                                                                                     << ": No such definition of userdel" 
                                                                                     << endl;
                                                                            }
                                                                        }
                                                                        }else{
                                                                            found = false;
                                                                            for(int i = 0; i < userVect.size(); i++){
                                                                                if(query[1] == userVect[i].getUserName()){
                                                                                    userVect.erase(userVect.begin()+i);
                                                                                    found = true;
                                                                                }
                                                                            }
                                                                            if(found == false && query.size() == 2){
                                                                                cout << "userdel: user '"
                                                                                    << query[1]
                                                                                    << "' does not exist\n";
                                                                            }else{
                                                                                if(query.size() != 2){
                                                                                    cout << "-mash: userdel"  
                                                                                        << ": No such definition of userdel" 
                                                                                        << endl;
                                                                                }
                                                                            }
                                                                        }
                                                                    }else{
                                                                        if(query[0] == "usermod" && query[1] == "-a" && query[2] == "-G" ){
                                                                            
                                                                            bool alreadyExists = false;
                                                                            bool foundGroup = false;
                                                                            bool inUserGroups = false;
                                                                            found = false;
                                                                            string groupToAdd;
                                                                            group newGroup; //to be added to current user

                                                                            for(int i = 0; i < groupsVect.size(); i++){ //check existence in group vector. bool true if it is
                                                                                if(query[3] == groupsVect[i].getGroupName()){
                                                                                    alreadyExists = true;
                                                                                }
                                                                            }
                                                                            for(int i = 0; i < groupsVect.size(); i++){ 
                                                                                if(query[3] == groupsVect[i].getGroupName() && alreadyExists){ //if it exist, we can create a new object
                                                                                    groupToAdd = groupsVect[i].getGroupName(); //set name
                                                                                    newGroup.setGroupName(groupToAdd); // change name of newgroup. We can use this now, to put in user groups 
                                                                                    foundGroup = true;  
                                                                                }
                                                                            }
                                                                            for(int i = 0; i < currUserPtr->getGroupVector().size();i++){ //check if its already in the user groups
                                                                                if(query[3] == currUserPtr->getGroupVector()[i].getGroupName()){
                                                                                    inUserGroups = true;
                                                                                }
                                                                            }
                                                                            if(foundGroup && !inUserGroups){ //if it is a real group, and the user does not have it, add it.
                                                                                currUserPtr->getGroupVector().push_back(newGroup);
                                                                                found = true;
                                                                            }
                                                                            if(!foundGroup && query.size() == 4){
                                                                                cout << "usermod: group '"
                                                                                        << query[3]
                                                                                        << "' does not exist\n";
                                                                            }else{
                                                                                if(!found && query.size() == 4){
                                                                                    cout << "usermod: User is already in group '"
                                                                                         << query[3]
                                                                                         << "'\n";
                                                                                }else{
                                                                                    if(query.size() != 4){
                                                                                        cout << "-mash: usermod"  
                                                                                            << ": No such definition of usermod" 
                                                                                            << endl;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }else{
                                                                            if(query[0] == "switchto"){
                                                                                found = false;
                                                                                for(int i = 0; i < userVect.size();i++){
                                                                                    if(userVect[i].getUserName() == query[1]){
                                                                                        currUserPtr = &userVect[i];
                                                                                        found = true;
                                                                                    }
                                                                                }
                                                                                if(!found && query.size() == 2){
                                                                                    cout << "switchto: user '"
                                                                                        << query[1]
                                                                                        << "' does not exist\n";
                                                                                }else{
                                                                                    if(query.size() != 2){
                                                                                        cout << "-mash: switchto"  
                                                                                            << ": No such definition of switchto" 
                                                                                            << endl;
                                                                                    }
                                                                                }

                                                                            }else{
                                                                                cout << "-mash: " 
                                                                                    << query[0]
                                                                                    << ": command not found\n";
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}