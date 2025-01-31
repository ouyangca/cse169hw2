#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Cube* Window::cube;
Skeleton* Window::skeleton;
Skin* Window::skin;
bool Window::skel_found;
bool Window::skin_found;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects(bool skel_found, bool skin_found, const char* skelFile, const char* skinFile) {
    // Create a cube
    // cube = new Cube();

    skeleton = nullptr;

    Window::skel_found = skel_found;
    Window::skin_found = skin_found;
    if (skel_found){
        std::cout << "YAY Skeleton file found. " << std::endl;
    }
     if (skin_found){
        std::cout << "YAY Skin file found. " << std::endl;
     }

    if (skel_found && !skin_found){
        skeleton = new Skeleton();
        skeleton->load(skelFile);
        
    }
    
    else if (skel_found && skin_found){
        // bool binding = true;
    // std::cout << "seg2" << std::endl;
        skeleton = new Skeleton();
        skeleton->load(skelFile);
        skin = new Skin(true, skeleton);
        skin->Load(skinFile);

    }
    else if (!skel_found && skin_found){
    std::cout << "seg 1" << std::endl;

        skin = new Skin(false, skeleton);
        skin->Load(skinFile);
    }
    
    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete cube;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();
    
    
    if (skel_found && !skin_found){
        skeleton->update();
    }
    else if (skel_found && skin_found){
    std::cout << "seg 1" << std::endl;

        skeleton->update();
    std::cout << "seg 1" << std::endl;

       skin->Update(true);
    }
    else if (!skel_found && skin_found){
        
        skeleton->update();
    std::cout << "seg 1" << std::endl;

        skin->Update(false);
    }
    
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    // cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    // if (drawSkeleton){
    //     skeleton->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    // }
    // if (drawSkin){
    //     skin->Draw( false, Cam->GetViewProjectMtx(), Window::shaderProgram);
    // }
    if (skel_found && !skin_found){
        skeleton->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    else if (skel_found && skin_found){
    // std::cout << "draw" << std::endl;
        skeleton->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
        
       skin->DrawTriangle(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    else if (!skel_found && skin_found){
        // std::cout << "skin is found" << std::endl;
    std::cout << "triangle 1" << std::endl;

        skin->DrawTriangle(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            default:
                break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

    MouseX = (int)currX;
    MouseY = (int)currY;

    // Move camera
    // NOTE: this should really be part of Camera::Update()
    if (LeftDown) {
        const float rate = 1.0f;
        Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
        Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (RightDown) {
        const float rate = 0.005f;
        float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        Cam->SetDistance(dist);
    }
}