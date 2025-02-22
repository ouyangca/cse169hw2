#pragma once

#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "core.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Animation.h"
#include "Player.h"
#include "Rig.h"

class Window {
public:
    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;
    static bool skel_found;
    static bool skin_found;
    static int selectedJoint;  // Global or static variable to track the selected joint
    static int selectedDOF;

    static Skeleton* skeleton;
    static Skin* skin;
    static Rig* rig;
    static Animation* clip;
    static Player* player;

    // Objects to render
    static Cube* cube;

    // Shader Program
    static GLuint shaderProgram;

    // Act as Constructors and desctructors
    static bool initializeProgram();
    static bool initializeObjects(bool skel_found, bool skin_found, 
                const char* skelFile, const char* skinFile, const char* animFile);
    static void cleanUp();
    static void printDOF();



    // for the Window
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    // update and draw functions
    static void idleCallback();
    static void displayCallback(GLFWwindow*);

    // helper to reset the camera
    static void resetCamera();

    // callbacks - for interaction
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);
};