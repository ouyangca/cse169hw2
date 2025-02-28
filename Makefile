CC = g++

BREW = $(shell brew --prefix)

CFLAGS = -g -std=c++11 -Wno-deprecated-declarations -DGLM_ENABLE_EXPERIMENTAL
INCFLAGS = -Iinclude -I$(BREW)/include
LDFLAGS = -framework OpenGL -L$(BREW)/lib -lglfw -lGLEW

RM = /bin/rm -f
all: menv
menv: main.o Camera.o Cube.o Shader.o Tokenizer.o Window.o Skeleton.o Joint.o DOF.o Skin.o Vertex.o Triangle.o Keyframe.o Channel.o Animation.o Player.o Rig.o Cloth.o Particle.o SpringDamper.o Tri.o Ground.o
	$(CC) -o menv main.o Camera.o Cube.o Shader.o Tokenizer.o Skeleton.o Joint.o DOF.o Window.o Skin.o Vertex.o Triangle.o Keyframe.o Channel.o Animation.o Player.o Rig.o Cloth.o Particle.o SpringDamper.o Tri.o Ground.o $(LDFLAGS)
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
Triangle.o: src/Triangle.cpp include/Triangle.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Triangle.cpp
Vertex.o: src/Vertex.cpp include/Vertex.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Vertex.cpp
Skin.o: src/Skin.cpp include/Skin.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Skin.cpp
Keyframe.o: src/Keyframe.cpp include/Keyframe.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Keyframe.cpp
Channel.o: src/Channel.cpp include/Channel.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Channel.cpp
Animation.o: src/Animation.cpp include/Animation.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Animation.cpp
Player.o: src/Player.cpp include/Player.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Player.cpp
Rig.o: src/Rig.cpp include/Rig.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Rig.cpp
Cloth.o: src/Cloth.cpp include/Cloth.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Cloth.cpp
Particle.o: src/Particle.cpp include/Particle.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Particle.cpp
SpringDamper.o: src/SpringDamper.cpp include/SpringDamper.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/SpringDamper.cpp
Tri.o: src/Tri.cpp include/Tri.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Tri.cpp
Ground.o: src/Ground.cpp include/Ground.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Ground.cpp





clean: 
	$(RM) *.o menv
