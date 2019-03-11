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

int main(){
    //while loop control, used in exit
    bool control = true;
    //used for if searching through a vector for an obj
    bool found;
    string input;
    directory currDir; //create a new directory, as a base
    currDir.setName("/"); // This is the root, we cant move above this
    directory* currDirPtr = &currDir; //pointer, this will allow us to move between directories via pointing
    
    //parsing code for user
    while(control){ 
    cout << BOLDCYAN << "user:~" << RESET << currDirPtr->getPath() << "$ "; //to emulate linux, print this. user is not an actual obj or anything
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
                            currDirPtr->ls_l();
                        }else{
                            currDirPtr->ls(); //basic ls, tabbed
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
                                currDirPtr->mkdir(query[1]);
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
                                    currDirPtr->touch(query[1]);
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
                                                cout << "-mash: chmod: " 
                                                     << query[2] 
                                                     << ": No such file or directory" 
                                                     << endl;
                                            }else{
                                                if(query.size() < 3 || query.size() > 3){ //invalid definition of chmod
                                                    cout << "-mash: chmod "  
                                                        << ": No such definition of chmod" 
                                                        << endl;
                                                }
                                            }
                                        }else{
                                            if(query[0] == "exit" || query[0] == "quit"){
                                                control = false;
                                            }else{ //command not found message. last resort
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
    return 0;
}