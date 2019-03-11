//file: cs3800Proj1_directory.cpp
//author: Mashroor Rashid, CS3800 Sec B
//Directory class, definititons and code
#include "cs3800Proj1_directory.h"
using namespace std;

directory::directory(string name, directory &newParent){
            directoryName = name;
            parent = &newParent;
            if (parent != nullptr){
                path = parent->getPath() + '/' + directoryName;
            }
            else{ //this is only really used for the root. everything else made should have a parent
                path = directoryName;
            }
            setTimestamp();
            userName = "user"; //arbitrary value, not really important other than printing
            fileSize = 1024; //arbitrary value, not really important other than printing
            permissions = "rwxrwxrwx";
}
directory::~directory(){ //required since we need to allocate in mkdir()
    for(int i = 0; i < innerDirectories.size(); i++){
        delete innerDirectories[i];
    }
}
void directory::setTimestamp(){ //helper, sets time without a \n from the time_t obj
    time_t result = time(nullptr);
    char *temp = ctime(&result);
    if (temp[strlen(temp)-1] == '\n'){
        temp[strlen(temp)-1] = '\0';
    }
    timestamp = temp;
}
void directory::setPermissions(string permCode){
    string tempPermissions;
    string backupPermissions = permissions;
    for(int i = 0; i < permCode.length(); i++){ //check if valid code, parse in range
       if(permCode[i] < 48 || permCode[i] > 55){
            cout << "chmod: Invalid file mode: " << permCode <<endl;
            return;
        }
    }
    for(int i = 0; i < permCode.length(); i++){
            if(permCode[i] == '0'){
                tempPermissions += "---";
            }
            if(permCode[i] == '1'){
                tempPermissions += "--x";
            }
            if(permCode[i] == '2'){
                tempPermissions += "-w-";
            }
            if(permCode[i] == '3'){
                tempPermissions += "-wx";
            }
            if(permCode[i] == '4'){
                tempPermissions += "r--";
            }
            if(permCode[i] == '5'){
                tempPermissions += "r-x";
            }
            if(permCode[i] == '6'){
                tempPermissions += "rw-";
            }
            if(permCode[i] == '7'){
                tempPermissions += "rwx";
            }
    }
    permissions = tempPermissions;
}
directory* directory::cd(string objName, directory* newParent){
    for(int i = 0; i < innerDirectories.size(); i++){
        if(innerDirectories[i]->getDirectoryName() == objName){
            return innerDirectories[i]; //returns a pointer, since most work for cd is in main
        }
    }
}
void directory::pwd(){ //kind of redundant, but it spits out a path
    cout << getPath() + '/' << endl;
}
void directory::ls(){ //this function is just a pretty print
    for (int i = 0; i < innerDirectories.size(); i++){
        cout << BOLDCYAN << innerDirectories[i]->getDirectoryName() << RESET <<  '/' << "\t";
        }
    for (int i = 0; i < innerFiles.size(); i++){
        cout << innerFiles[i].getFileName() << "\t";
        }
    cout << endl;
}
void directory::ls_l(){ //a detailed pretty print. Not special
    for (int i = 0; i < innerDirectories.size(); i++){
        cout << 'd'
             << innerDirectories[i]->getPermissions() << "\t"
             << innerDirectories[i]->getUserName() << "\t"
             << innerDirectories[i]->getFileSize() << "\t"
             << innerDirectories[i]->getTimestamp() << "\t"
             << BOLDCYAN << innerDirectories[i]->getDirectoryName() << RESET << '/' << "\n";
        }
    for (int i = 0; i < innerFiles.size(); i++){
        cout << '-'
             << innerFiles[i].getPermissions() << "\t"
             << innerFiles[i].getUserName() << "\t"
             << innerFiles[i].getFileSize() << "\t"
             << innerFiles[i].getTimestamp() << "\t"
             << innerFiles[i].getFileName() << "\n";
        }
}
void directory::chmod(string permCode, string dirName){
    for(int i = 0; i < innerDirectories.size(); i++){
        if(innerDirectories[i]->getDirectoryName() == dirName){
            innerDirectories[i]->setPermissions(permCode);
            innerDirectories[i]->setTimestamp();
        }
    }
    for(int i = 0; i < innerFiles.size(); i++){
        if(innerFiles[i].getFileName() == dirName){
            innerFiles[i].setPermissions(permCode);
            innerFiles[i].setTimestamp();
        }
    }    
}
void directory::mkdir(string newDirName){
    for(int i = 0; i < innerDirectories.size(); i++){ //check existance first
        if(innerDirectories[i]->getDirectoryName() == newDirName){
            cout << "mkdir: " << newDirName << ": File exists\n"; 
            return;
        }
    }    
    directory* newDir = new directory(newDirName, *this); //ALLOCATING DATA HERE. MAKE SURE TO DELETE WHEN DEREF
    innerDirectories.push_back(newDir);
}
void directory::touch(string newFileName){
    for(int i = 0; i < innerFiles.size(); i++){ //update timestamp if existing
        if(innerFiles[i].getFileName() == newFileName){
            innerFiles[i].setTimestamp();
            return;
        }
    }
    file newFile(newFileName); //create a new file
    innerFiles.push_back(newFile);
    return;
}
void directory::rmdir(string dirToDel){ //removes a directory. destructor is called when out of scope
    for(int i = 0; i < innerDirectories.size(); i++){
        if(innerDirectories[i]->getDirectoryName() == dirToDel){
            delete innerDirectories[i];
            innerDirectories.erase(innerDirectories.begin()+ i);
        }
    }
}
void directory::rm(string fileToDel){ //removes a file. default destructor called, since no data allocated
    for(int i = 0; i < innerFiles.size(); i++){
        if(innerFiles[i].getFileName() == fileToDel){
            innerFiles.erase(innerFiles.begin()+ i);
        }
    }
}

