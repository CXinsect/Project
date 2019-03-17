SrcFile=$(wildcard *.cpp)
ObjFile=$(patsubst *.cpp,%.o,$(SrcFile))
app:$(ObjFile)
	g++  -o $@ -I ./include main.cpp  
%cpp:%.o
	g++ -c $< -I ./include $(ObjFile)

.PHONY:
	all clean
test:
	echo $(SrcFile)
	echo $(ObjFile)
clean:
	rm  -rf  *.o
