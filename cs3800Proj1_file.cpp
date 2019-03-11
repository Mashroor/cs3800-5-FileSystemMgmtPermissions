//file: cs3800Proj1_file.cpp
//author: Mashroor Rashid, CS3800 Sec B
//File class, definititons and code
#include "cs3800Proj1_file.h"
using namespace std;

file::file(string name){
            //set defaults for file. nothing special
            fileName = name;
            path = path + name + "/";
            setTimestamp();
            userName = "user";
            fileSize = 1024; //dummy variable. Only for printing
            permissions = "rwxrwxrwx";
}
void file::setTimestamp(){ //helper, to make timestamp, cut off \n char at end
    time_t result = time(nullptr);
    char *temp = ctime(&result);
    if (temp[strlen(temp)-1] == '\n'){
        temp[strlen(temp)-1] = '\0';
    }
    timestamp =temp;
}
void file::setPermissions(string permCode){
    string tempPermissions; //to be set at end
    string backupPermissions = permissions; //set so if we have invalid, can replace with prev
    for(int i = 0; i < permCode.length(); i++){ //check if valid ASCII values, range
       if(permCode[i] < 48 || permCode[i] > 55){
            cout << "chmod: Invalid file mode: " << permCode <<endl;
            return;
        }
    }
    for(int i = 0; i < permCode.length(); i++){ //iterative approach to concat string with perms
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