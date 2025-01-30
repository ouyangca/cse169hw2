CC = g++

BREW = $(shell brew --prefix)

CFLAGS = -g -std=c++11 -Wno-deprecated-declarations -DGLM_ENABLE_EXPERIMENTAL
INCFLAGS = -Iinclude -I$(BREW)/include
LDFLAGS = -framework OpenGL -L$(BREW)/lib -lglfw

RM = /bin/rm -f
all: menv
menv: main.o Camera.o Cube.o Shader.o Tokenizer.o Window.o Skeleton.o Joint.o DOF.o
	$(CC) -o menv main.o Camera.o Cube.o Shader.o Tokenizer.o Skeleton.o Joint.o DOF.o Window.o $(LDFLAGS)
main.o: main.cpp include/Window.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
Camera.o: src/Camera.cpp include/Camera.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Camera.cpp
Cube.o: src/Cube.cpp include/Cube.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Cube.cpp
Shader.o: src/Shader.cpp include/Shader.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Shader.cpp
Tokenizer.o: src/Tokenizer.cpp include/Tokenizer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Tokenizer.cpp
DOF.o: src/DOF.cpp include/DOF.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/DOF.cpp
Joint.o: src/Joint.cpp include/Joint.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Joint.cpp
Skeleton.o: src/Skeleton.cpp include/Skeleton.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Skeleton.cpp
Window.o: src/Window.cpp include/Window.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Window.cpp


clean: 
	$(RM) *.o menv
