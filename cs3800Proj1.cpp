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
#include<algorithm>

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
    userVect.reserve(50); //to avoid screwing with the userVect. had issues with moving the vector entirely
    user defaultUser("user"); //set a default user. of Group default.
    userVect.push_back(defaultUser); //throw this in to keep track of current user. start at default.
    user* currUserPtr = &userVect[0]; //pointer to current user in vector. we will be changing this
    
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
                    string tempDirectoryGroup;
                    for(int i = 0; i < currDirPtr->getDirectoryVect().size(); i++){ //search for a temporary Directory group
                        for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                            if(currDirPtr->getDirectoryVect()[i]->getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                tempDirectoryGroup = currUserPtr->getGroupVector()[j].getGroupName();
                            }
                        }
                    }
                    for(int i = 0; i < currDirPtr->getSize(); i++){ //goes into a directory, down 1
                        if(query[1] == currDirPtr->getDirectoryVect()[i]->getDirectoryName()){
                                found = true;
                            if((currDirPtr->getDirectoryVect()[i]->getUserName() == currUserPtr->getUserName() && currDirPtr->getDirectoryVect()[i]->getPermissions()[2] == 'x') //if user has permission to access
                                || (currDirPtr->getDirectoryVect()[i]->getGroupName() == tempDirectoryGroup && currDirPtr->getDirectoryVect()[i]->getPermissions()[5] == 'x')  //if group has permission
                                || (currDirPtr->getDirectoryVect()[i]->getPermissions()[8] == 'x')){ // if other has permission to cd
                                currDirPtr = currDirPtr->cd(query[1], currDirPtr);
                            }
                            else{
                                cout << "-mash: Permission denied.\n";
                            }
                        }
                    }
                }
                if(!found){ //directory not found message
                    cout << "-mash: cd: " << query[1] << ": No such directory" << endl;
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
                            for (int i = 0; i < currDirPtr->getSize(); i++){ //finds if dir exists
                                if( currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[1]){
                                    found = true;
                                }
                            }
                            for (int i = 0; i < currDirPtr->getFilesSize(); i++){ //search if file exists
                                if(currDirPtr->getFilesVect()[i].getFileName() == query[1]){
                                    found = true;
                                }
                            }
                            string tempDirectoryGroup;
                            for(int i = 0; i < currDirPtr->getDirectoryVect().size(); i++){ //search for a temporary Directory group
                                for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                                    if(currDirPtr->getDirectoryVect()[i]->getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                        tempDirectoryGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                    }
                                }
                            }
                            if(found == true){ //error message if not found
                                cout << "-mash: mkdir: " << query[1] << ": file or directory exists" << endl;
                            }else{
                                if((currDirPtr->getUserName() == currUserPtr->getUserName() && currDirPtr->getPermissions()[1] == 'w') //if user has permission to access
                                || (currDirPtr->getGroupName() == tempDirectoryGroup && currDirPtr->getPermissions()[4] == 'w')  //if group has permission
                                || (currDirPtr->getPermissions()[7] == 'w')){
                                    currDirPtr->mkdir(query[1], *currUserPtr); 
                                }else{
                                    cout << "-mash: Permission denied.\n";
                                }
                            }
                        }else{
                            if(query[0] == "touch"){ //create a file. dummy obj
                                found = false;
                                for (int i = 0; i < currDirPtr->getSize(); i++){
                                    if(currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[1]){
                                        found = true;
                                    }
                                }
                                if(found){ //error message if not found
                                    currDirPtr->touch(query[1], *currUserPtr);
                                }else{
                                    currDirPtr->touch(query[1], *currUserPtr);
                                }
                            }else{
                                if(query[0] == "rmdir"){ //remove a directory, needs to call destructor. avoid mem leak
                                    found = false; //search for if directory or file exists first, set false to jump out when found
                                    for(int i = 0; i < currDirPtr->getSize(); i++){
                                        found = true;
                                    }

                                    string tempDirectoryGroup;
                                    for(int i = 0; i < currDirPtr->getDirectoryVect().size(); i++){ //search for a temporary Directory group
                                        for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                                            if(currDirPtr->getDirectoryVect()[i]->getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                tempDirectoryGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                            }
                                        }
                                    }
                                    for (int i = 0; i < currDirPtr->getSize(); i++){
                                        if( currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[1]){
                                            found = true;
                                            if((currDirPtr->getUserName() == currUserPtr->getUserName() && currDirPtr->getDirectoryVect()[i]->getPermissions()[1] == 'w') //if user has permission to access
                                                || (currDirPtr->getGroupName() == tempDirectoryGroup && currDirPtr->getDirectoryVect()[i]->getPermissions()[4] == 'w')  //if group has permission
                                                || (currDirPtr->getPermissions()[7] == 'w')){
                                                currDirPtr->rmdir(query[1]);
                                            }else{
                                                cout << "-mash: Permission denied.\n";
                                            }
                                        }
                                    }
                                    if(found == false){ //error message if not found
                                        cout << "-mash: rmdir: " << query[1] << ": No such directory" << endl;
                                    }
                                }else{
                                    if(query[0] == "rm"){ //for files, nothing special. same search mech
                                        found = false;

                                        string tempDirectoryGroup;
                                        for(int i = 0; i < currDirPtr->getDirectoryVect().size(); i++){ //search for a temporary Directory group
                                            for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                                                if(currDirPtr->getDirectoryVect()[i]->getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                    tempDirectoryGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                                }
                                            }
                                        }
                                        for (int i = 0; i < currDirPtr->getFilesSize(); i++){
                                            if(currDirPtr->getFilesVect()[i].getFileName() == query[1]){
                                                found = true;
                                                if((currDirPtr->getUserName() == currUserPtr->getUserName() && currDirPtr->getFilesVect()[i].getPermissions()[1] == 'w') //if user has permission to access
                                                || (currDirPtr->getGroupName() == tempDirectoryGroup && currDirPtr->getFilesVect()[i].getPermissions()[4] == 'w')  //if group has permission
                                                || (currDirPtr->getPermissions()[7] == 'w')){
                                                    currDirPtr->rm(query[1]);
                                                }else{
                                                    cout << "-mash: Permission denied.\n";

                                                }
                                            }
                                        }
                                        if(found == false){
                                            cout << "-mash: rm: " << query[1] << ": No such file" << endl;
                                        }
                                    }else{
                                        if(query[0] == "chmod"){ //requires 3 items in query vect. check
                                            found = false;
                                            string tempDirectoryGroup;
                                            for(int i = 0; i < currDirPtr->getDirectoryVect().size(); i++){ //search for a temporary Directory group
                                                for(int j = 0; j < currUserPtr->getGroupVector().size();j++){
                                                    if(currDirPtr->getDirectoryVect()[i]->getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                        tempDirectoryGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                                    }
                                                }
                                            }
                                            string tempFileGroup;
                                            for(int i = 0; i <currDirPtr->getFilesVect().size(); i++){ //search for temp file group
                                                for(int j = 0; j < currUserPtr->getGroupVector().size();j++){
                                                    if(currDirPtr->getFilesVect()[i].getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                        tempFileGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                                    }
                                                }
                                            }
                                            for (int i = 0; i < currDirPtr->getSize(); i++){
                                                if(query.size() == 3){ //make sure we have 3 things
                                                    if(currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[2]){
                                                        if((currUserPtr->getUserName() == currDirPtr->getDirectoryVect()[i]->getUserName() &&currDirPtr->getDirectoryVect()[i]->getPermissions()[1] == 'w')
                                                        ||(currDirPtr->getDirectoryVect()[i]->getGroupName() == tempFileGroup && currDirPtr->getDirectoryVect()[i]->getPermissions()[4] == 'w')
                                                        ||(currDirPtr->getDirectoryVect()[i]->getPermissions()[7] == 'w')){
                                                            currDirPtr->chmod(query[1], query[2]);
                                                        }else{
                                                            cout << "-mash: Permission denied.\n";
                                                        }
                                                        found = true;
                                                    }
                                                }
                                            }
                                            for (int i = 0; i < currDirPtr->getFilesSize(); i++){
                                                if(query.size() == 3){ //make sure we have 3 things
                                                    if(currDirPtr->getFilesVect()[i].getFileName() == query[2]){
                                                        if((currUserPtr->getUserName() == currDirPtr->getFilesVect()[i].getUserName() && currDirPtr->getFilesVect()[i].getPermissions()[1] == 'w') //if user has permission to access
                                                        || (currDirPtr->getFilesVect()[i].getGroupName() == tempFileGroup && currDirPtr->getFilesVect()[i].getPermissions()[4] == 'w')  //if group has permission
                                                        || (currDirPtr->getFilesVect()[i].getPermissions()[7] == 'w')){
                                                            currDirPtr->chmod(query[1], query[2]);
                                                        }else{
                                                            cout << "-mash: Permission denied.\n";
                                                        }
                                                        found = true;
                                                    }
                                                }
                                            }
                                            if(found == false && query.size() == 3 ){ //if file/dir doesn't exist
                                                cout << "-mash: chmod:" << query[2] << ": No such file or directory" << endl;
                                            }else{
                                                if(query.size() != 3 && query[2].size() != 3){ //invalid definition of chmod
                                                    cout << "-mash: chmod: No such definition of chmod" << endl;
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
                                                    bool perm = true;
                                                    string tempFileGroup;
                                                    for(int i = 0; i < currDirPtr->getDirectoryVect().size(); i++){
                                                        for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                                                            if(currDirPtr->getDirectoryVect()[i]->getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                                tempFileGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                                            }
                                                        }
                                                    }
                                                    for (int i = 0; i < currDirPtr->getFilesSize(); i++){ //find the file here. Search.
                                                        if(currDirPtr->getFilesVect()[i].getFileName() == execName){ //if file is found
                                                            if((currDirPtr->getFilesVect()[i].getUserName() == currUserPtr->getUserName() && currDirPtr->getFilesVect()[i].getPermissions()[2] == 'x') //if user has permission
                                                            || (currDirPtr->getFilesVect()[i].getGroupName() == tempFileGroup && currDirPtr->getFilesVect()[i].getPermissions()[5] == 'x')  //if group has permission
                                                            || (currDirPtr->getFilesVect()[i].getPermissions()[8] == 'x')){ //if other has permission
                                                                cout << execName
                                                                    << " has been executed."
                                                                    << endl;
                                                                found = true;
                                                            }else{
                                                                perm = false;
                                                            }
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
                                                        cout << "-mash: " << query[0] << ": is a directory" << endl;
                                                    }
                                                    else{
                                                        if(found == false && perm){ //no file to execute
                                                            cout << "-mash: " << query[0] << ": No such file or directory" << endl;
                                                        }else{
                                                            if(!found && !perm){
                                                                cout << "-mash: Permission denied.\n";
                                                            }
                                                        }
                                                    }
                                                }else{
                                                    if(query[0] == "whoami"){ //for testing. extra
                                                        cout << currUserPtr->getUserName() << '\n';
                                                    }else{
                                                        if(query[0] == "groups"){ //spits out groups
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
                                                                    cout << "groupadd: group '" << query[1] << "' already exists\n";
                                                                }else{
                                                                    if(query.size() != 2){
                                                                        cout << "-mash: groupadd" << ": No such definition of groupadd" << endl;
                                                                    }else{
                                                                        group newGroup(query[1]);
                                                                        groupsVect.push_back(newGroup);
                                                                    }
                                                                }
                                                            }else{
                                                                if(query[0] == "useradd"){
                                                                    found = false;
                                                                    bool existingGroup = false;
                                                                    int counter = 0;
                                                                    if(query[1] == "-G"){
                                                                        
                                                                        vector<string> listToAdd;
                                                                        istringstream groupString(query[2]); //start splitting csv string here. same as spaced string before
                                                                        string temp;
                                                                        while(getline(groupString, temp, ',')){
                                                                            listToAdd.push_back(temp);
                                                                        }
                                                                        for(int i = 0; i < userVect.size(); i++){ //check to see if user already exits
                                                                            if(query[3] == userVect[i].getUserName()){
                                                                                found = true;  //found a user, set bool to create.
                                                                            }
                                                                        }

                                                                        for(int i = 0; i < listToAdd.size(); i++){ //check if the groups exist in the system
                                                                            for(int j = 0; j < groupsVect.size(); j++){
                                                                                if(listToAdd[i] == groupsVect[j].getGroupName()){
                                                                                    counter++;
                                                                                }
                                                                            }
                                                                        }
                                                                        if(found && query.size() == 4){
                                                                            cout << "useradd: Invalid user creation, groups do not exist" << endl;
                                                                        }else{
                                                                            if(query.size() != 4){
                                                                                cout << "-mash: useradd"  << ": No such definition of useradd" << endl;
                                                                            }else{
                                                                                if(counter == listToAdd.size()){ //if the number found is equal to all to add,
                                                                                    user newUser(query[3]);
                                                                                    for(int i = 0; i < listToAdd.size(); i++){
                                                                                        newUser.getGroupVector().push_back(listToAdd[i]);
                                                                                        groupsVect.push_back(listToAdd[i]);
                                                                                    }
                                                                                    userVect.push_back(newUser); //add new piece
                                                                                }else{
                                                                                    cout << "useradd: Invalid user creation, groups do not exist" << endl;
                                                                                }

                                                                            }
                                                                        }
                                                                    }else{
                                                                        for(int i = 0; i < userVect.size(); i++){
                                                                            if(query[1] == userVect[i].getUserName()){
                                                                                found = true;
                                                                            }
                                                                        }
                                                                        if(found && query.size() == 2){
                                                                            cout << "useradd: user '" << query[1] << "' already exists" << endl;;
                                                                        }else{
                                                                            if(query.size() != 2){
                                                                                cout << "-mash: useradd"  << ": No such definition of useradd" << endl;
                                                                            }else{
                                                                                user newUser(query[1]);
                                                                                userVect.push_back(newUser); //add new piece
                                                                            }
                                                                        }
                                                                    }
                                                                }else{
                                                                    if(query[0] == "userdel"){
                                                                        bool defaultDelete = false;
                                                                        if(query[1] == "-G"){
                                                                            for(int i = 0; i < userVect.size(); i++){
                                                                                if(query[3] == userVect[i].getUserName()){
                                                                                    for(int j = 0; j < userVect[i].getGroupVector().size(); j++){
                                                                                        if(query[2] == userVect[i].getGroupVector()[j].getGroupName()){
                                                                                            if(userVect[i].getGroupVector()[j].getGroupName() == "users"){
                                                                                                cout << "-mash: userdel -G: Cannot delete default group" << endl;;
                                                                                                found = true;
                                                                                                defaultDelete = true;
                                                                                            }
                                                                                            if(userVect[i].getGroupVector()[j].getGroupName() == currUserPtr->getGroupVector()[j].getGroupName() && !defaultDelete){
                                                                                                currUserPtr->getGroupVector().erase(userVect[i].getGroupVector().begin()+j);
                                                                                            }else{
                                                                                                if(!defaultDelete){
                                                                                                    userVect[i].getGroupVector().erase(userVect[i].getGroupVector().begin()+j);
                                                                                                }
                                                                                            }
                                                                                            found = true;
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                            if(found == false && query.size() == 4){
                                                                                cout << "userdel -G: user '" << query[3] << "' is not a part of specified group" << endl;
                                                                            }else{
                                                                                if(query.size() != 4){
                                                                                    cout << "-mash: userdel" << ": No such definition of userdel" << endl;
                                                                                }
                                                                            }
                                                                        }else{
                                                                            found = false;
                                                                            for(int i = 0; i < userVect.size(); i++){
                                                                                if(query[1] == userVect[i].getUserName()){
                                                                                    found = true;
                                                                                    if(query[1] != currUserPtr->getUserName()){
                                                                                        userVect.erase(userVect.begin()+i);
                                                                                    }else{
                                                                                        cout << "-mash: userdel: cannot delete current user" << endl;
                                                                                    }
                                                                                }
                                                                            }
                                                                            if(found == false && query.size() == 2){
                                                                                cout << "userdel: user '" << query[1] << "' does not exist\n";
                                                                            }else{
                                                                                if(query.size() != 2){
                                                                                    cout << "-mash: userdel" << ": No such definition of userdel" << endl;
                                                                                }
                                                                            }
                                                                        }
                                                                    }else{
                                                                        if(query[0] == "usermod" && query[1] == "-a" && query[2] == "-G" ){
                                                                            
                                                                            bool foundGroup = false;
                                                                            bool inUserGroups = false;
                                                                            found = false;
                                                                            string groupToAdd;
                                                                            group newGroup; //to be added to current user

                                                                            for(int i = 0; i < groupsVect.size(); i++){ //search for group in groupVect. Make if found.
                                                                                if(query[3] == groupsVect[i].getGroupName()){
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
                                                                                for(int i = 0; i < userVect.size(); i++){
                                                                                    if(currUserPtr->getUserName() == userVect[i].getUserName()){

                                                                                         if(userVect[i].getUserName() == currUserPtr->getUserName() && !inUserGroups){
                                                                                                currUserPtr->getGroupVector().push_back(newGroup);
                                                                                            }else if(!inUserGroups){
                                                                                                    userVect[i].getGroupVector().push_back(newGroup);  //add the group
                                                                                            }
                                                                                        found = true;
                                                                                    }
                                                                                }
                                                                            }
                                                                            if(!foundGroup && query.size() == 4){
                                                                                cout << "usermod: group '" << query[3] << "' does not exist\n";
                                                                            }else{
                                                                                if(!found && query.size() == 4){
                                                                                    cout << "usermod: User is already in group '" << query[3] << "'\n";
                                                                                }else{
                                                                                    if(query.size() != 4){
                                                                                        cout << "-mash: usermod" << ": No such definition of usermod" << endl;
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
                                                                                    cout << "switchto: user '" << query[1]<< "' does not exist\n";
                                                                                }else{
                                                                                    if(query.size() != 2){
                                                                                        cout << "-mash: switchto"  << ": No such definition of switchto" << endl;
                                                                                    }
                                                                                }
                                                                            }else{
                                                                                if(query[0] == "chown"){
                                                                                    bool dirFound = false;
                                                                                    bool fileFound = false;
                                                                                    bool userFound = false;
                                                                                    found = false;
                                                                                    for(int i = 0; i < userVect.size(); i++){ //search if user exists
                                                                                        if(query[1] == userVect[i].getUserName()){
                                                                                            userFound = true;
                                                                                        }
                                                                                    }
                                                                                    string tempDirectoryGroup;
                                                                                    for(int i = 0; i < currDirPtr->getDirectoryVect().size(); i++){ //search for a temporary Directory group
                                                                                        for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                                                                                            if(currDirPtr->getDirectoryVect()[i]->getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                                                                tempDirectoryGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                    string tempFileGroup;
                                                                                    for(int i = 0; i <currDirPtr->getFilesVect().size(); i++){ //search for temporary File group
                                                                                        for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                                                                                            if(currDirPtr->getFilesVect()[i].getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                                                                tempFileGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                    for (int i = 0; i < currDirPtr->getSize(); i++){ //change user of directory
                                                                                        if( currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[2]){
                                                                                            if((currUserPtr->getUserName() == currDirPtr->getDirectoryVect()[i]->getUserName() && currDirPtr->getDirectoryVect()[i]->getPermissions()[1] == 'w') //if user has permission to access
                                                                                            || (currDirPtr->getDirectoryVect()[i]->getGroupName() == tempDirectoryGroup && currDirPtr->getDirectoryVect()[i]->getPermissions()[4] == 'w')  //if group has permission
                                                                                            || (currDirPtr->getDirectoryVect()[i]->getPermissions()[7] == 'w')){
                                                                                                    if(userFound){
                                                                                                        currDirPtr->getDirectoryVect()[i]->setUserName(query[1]);
                                                                                                    }
                                                                                                }
                                                                                                else{
                                                                                                    cout << "-mash: Permission denied.\n";
                                                                                                }
                                                                                            dirFound = true;
                                                                                        }
                                                                                    }
                                                                                    for (int i = 0; i < currDirPtr->getFilesSize(); i++){ //change owner of file
                                                                                        if(currDirPtr->getFilesVect()[i].getFileName() == query[2]){
                                                                                            if((currUserPtr->getUserName() == currDirPtr->getFilesVect()[i].getUserName() && currDirPtr->getFilesVect()[i].getPermissions()[1] == 'w') //if user has permission to access
                                                                                            || (currDirPtr->getFilesVect()[i].getGroupName() == tempFileGroup && currDirPtr->getFilesVect()[i].getPermissions()[4] == 'w')  //if group has permission
                                                                                            || (currDirPtr->getFilesVect()[i].getPermissions()[7] == 'w')){
                                                                                                    if(userFound){
                                                                                                        currDirPtr->getFilesVect()[i].setUserName(query[1]);
                                                                                                    }
                                                                                                }else{
                                                                                                    cout << "-mash: Permission denied.\n";
                                                                                                }
                                                                                            fileFound = true;
                                                                                        }
                                                                                    }
                                                                                    if(!userFound){
                                                                                            cout << "-mash: " << query[2] << ": no such user on system\n";
                                                                                        }else if(!fileFound && !dirFound){
                                                                                            cout << "-mash: " << query[2] << ": no such file or directory\n";
                                                                                        }
                                                                                    if(query.size() != 3){
                                                                                        cout << "-mash: chown" << ": No such definition of chown" << endl;
                                                                                    }
                                                                                }else{
                                                                                    if(query[0] == "chgrp"){
                                                                                        bool dirFound = false;
                                                                                        bool fileFound = false;
                                                                                        bool groupFound = false;
                                                                                        found = false;
                                                                                        for(int i = 0; i < groupsVect.size(); i++){
                                                                                            if(query[1] == groupsVect[i].getGroupName()){
                                                                                                    groupFound = true;
                                                                                            }
                                                                                        }
                                                                                        string tempDirectoryGroup;
                                                                                        for(int i = 0; i < currDirPtr->getDirectoryVect().size(); i++){ //search for a temporary Directory group
                                                                                            for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                                                                                                if(currDirPtr->getDirectoryVect()[i]->getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                                                                    tempDirectoryGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                        string tempFileGroup;
                                                                                        for(int i = 0; i <currDirPtr->getFilesVect().size(); i++){
                                                                                            for(int j = 0;j < currUserPtr->getGroupVector().size();j++){
                                                                                                if(currDirPtr->getFilesVect()[i].getGroupName() == currUserPtr->getGroupVector()[j].getGroupName()){
                                                                                                    tempFileGroup = currUserPtr->getGroupVector()[j].getGroupName();
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                        for (int i = 0; i < currDirPtr->getSize(); i++){ //change user of directory
                                                                                            if(currDirPtr->getDirectoryVect()[i]->getDirectoryName() == query[2]){
                                                                                                if(((currUserPtr->getUserName() == currDirPtr->getDirectoryVect()[i]->getUserName() && currDirPtr->getDirectoryVect()[i]->getPermissions()[1] == 'w') //if user has permission to access
                                                                                                || (currDirPtr->getDirectoryVect()[i]->getGroupName() == tempDirectoryGroup && currDirPtr->getDirectoryVect()[i]->getPermissions()[4] == 'w')  //if group has permission
                                                                                                || (currDirPtr->getDirectoryVect()[i]->getPermissions()[7] == 'w'))){
                                                                                                    if(groupFound){
                                                                                                        currDirPtr->getDirectoryVect()[i]->setGroupName(query[1]);
                                                                                                    }
                                                                                                }else{
                                                                                                    cout << "-mash: Permission denied.\n";
                                                                                                }
                                                                                                dirFound = true;
                                                                                            }
                                                                                        }
                                                                                        for (int i = 0; i < currDirPtr->getFilesSize(); i++){ //change owner of file
                                                                                            if(currDirPtr->getFilesVect()[i].getFileName() == query[2]){
                                                                                                if(((currUserPtr->getUserName() == currDirPtr->getFilesVect()[i].getUserName() && currDirPtr->getFilesVect()[i].getPermissions()[1] == 'w') //if user has permission to access
                                                                                                || (currDirPtr->getFilesVect()[i].getGroupName() == tempFileGroup && currDirPtr->getFilesVect()[i].getPermissions()[4] == 'w')  //if group has permission
                                                                                                || (currDirPtr->getFilesVect()[i].getPermissions()[7] == 'w'))){
                                                                                                    if(groupFound){
                                                                                                        currDirPtr->getFilesVect()[i].setGroupName(query[1]);
                                                                                                    }
                                                                                                }else{
                                                                                                    cout << "-mash: Permission denied.\n";
                                                                                                }
                                                                                                fileFound = true;
                                                                                            }
                                                                                        }
                                                                                        if(!groupFound){
                                                                                            cout << "-mash: " << query[2] << ": no such group on system\n";
                                                                                        }else if(!fileFound && !dirFound){
                                                                                            cout << "-mash: " << query[2] << ": no such file or directory\n";
                                                                                        }
                                                                                        if(query.size() != 3){
                                                                                            cout << "-mash: chgrp" << ": No such definition of chmod" << endl;
                                                                                            }
                                                                                    }else{
                                                                                        cout << "-mash: " << query[0] << ": command not found\n";
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
        }
    }
    return 0;
}