#makefile
#1 executable required for assignment
cs3800Proj1Executable: cs3800Proj1.o cs3800Proj1_directory.o cs3800Proj1_file.o cs3800Proj1_group.o cs3800Proj1_user.o
#linking of compiled files
	g++ cs3800Proj1.o cs3800Proj1_directory.o cs3800Proj1_file.o cs3800Proj1_group.o cs3800Proj1_user.o -o program

#------------------------------------------------------------------------------

cs3800Proj1.o : cs3800Proj1.cpp
	g++ -c -std=c++11 cs3800Proj1.cpp

cs3800Proj1_directory.o: cs3800Proj1_directory.cpp cs3800Proj1_directory.h
	g++ -c -std=c++11 cs3800Proj1_directory.cpp

cs3800Proj1_file.o: cs3800Proj1_file.cpp cs3800Proj1_file.h
	g++ -c -std=c++11 cs3800Proj1_file.cpp

cs3800Proj1_group.o: cs3800Proj1_group.cpp cs3800Proj1_group.h
	g++ -c -std=c++11 cs3800Proj1_group.cpp

cs3800Proj1_user.o: cs3800Proj1_user.cpp cs3800Proj1_user.h
	g++ -c -std=c++11 cs3800Proj1_user.cpp

#remove extraneous files
clean:
	 rm cs3800Proj1.o cs3800Proj1_directory.o cs3800Proj1_file.o cs3800Proj1_group.o cs3800Proj1_user.o