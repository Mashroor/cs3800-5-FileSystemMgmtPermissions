Add functionality for the following:

  ## userdel -G <group> <username>  
    remove user from group  
  ## groups  
    list all groups in order. Not in tabulated form  
  ## groupadd <group>  
    create a new group  
  ## usermod -a -G <group>  
    add current user to group  
  ## useradd -G <group1,group2,...groupn> <user>   
    add specified user to all groups listed. Comma delineated  
  ## useradd <user>  
    create a new user. Should add user to default group  
  ## userdel <user>  
    delete the specified user  
  ## switchto <user>  
    change current user to the user specified  
  ## ./<file>  
    execute the program/file  
  ## chown <newOwner> <file>  
    change the owner of the file specified to the user specified  
  ## chgrp <bnewGroup> <file>  
    change the group of the file specified to the group specified  
