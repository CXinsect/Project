CC = gcc
XX = g++

FLAGS = -Wall -O  -g
TARGET = a.out

SrcFile = $(wildcard *.c main.cpp)
ObjFile = $(patsubst *.c,*.o,$(patsubst *.cpp,*.o,$(SrcFile)))

%o:%.c
	$(CC) $(FLAGS) -c $< -o $@ 
%.o:%.cpp
	$(XX) $(FLAGS) -c  $< -o $@
$(TARGET):$(ObjFile)
	$(XX) $(ObjFile) -o $(TARGET)
.PHONY:
	clean a.out
clean:
	rm -rf *.o a.out

