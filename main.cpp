#include "Window.h"
#include "core.h"

void error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window) {
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);

    // Set the key callback.
    glfwSetKeyCallback(window, Window::keyCallback);

    // Set the mouse and cursor callbacks
    glfwSetMouseButtonCallback(window, Window::mouse_callback);
    glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions() {
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
              << std::endl;

    // If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(int argc, char *argv[]) {

    const char* skelFile = nullptr;
    const char* skinFile = nullptr;
    const char* animFile = nullptr;
    bool skel_found = false;
    bool skin_found = false;
    bool anim_found = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.find(".skel") != std::string::npos) {
            skelFile = argv[i];
            skel_found = true;
        } else if (arg.find(".skin") != std::string::npos) {
            skinFile = argv[i];
            skin_found = true;
        } else if (arg.find(".anim") != std::string::npos) {
            animFile = argv[i];
            anim_found = true;
        }
    }

    if (skel_found) {
        std::cout << "YAY Skeleton file found: " << skelFile << std::endl;
    }
    if (skin_found) {
        std::cout << "YAY Skin file found: " << skinFile << std::endl;
    }
    if (anim_found) {
        std::cout << "YAY Animation file found: " << animFile << std::endl;
    }

    // Now skelFile and skinFile can be used accordingly

    // Create the GLFW window.
    GLFWwindow* window = Window::createWindow(800, 600);
    if (!window) exit(EXIT_FAILURE);

    // Print OpenGL and GLSL versions.
    print_versions();
    // Setup callbacks.
    setup_callbacks(window);
    // Setup OpenGL settings.
    setup_opengl_settings();

    // Initialize the shader program; exit if initialization fails.
    if (!Window::initializeProgram()) exit(EXIT_FAILURE);
    // Initialize objects/pointers for rendering; exit if initialization fails.
    

     if (!skelFile) {
            std::cout << "TAT SKELETON FILE NOT FOUND" << std::endl;
    }


    if (!skinFile) {
        std::cout << "TAT SKIN FILE NOT FOUND" << std::endl;
        
    }

    if (!animFile) {
        std::cout << "TAT anim FILE NOT FOUND" << std::endl;
        
    }



    if (!Window::initializeObjects(skel_found, skin_found, skelFile, skinFile, animFile )) exit(EXIT_FAILURE);



    // Loop while GLFW window should stay open.
    while (!glfwWindowShouldClose(window)) {
        // Main render display callback. Rendering of objects is done here.

       
        Window::displayCallback(window);
        // std::cout << "check call back" << std::endl;

        


        // Idle callback. Updating objects, etc. can be done here.
        Window::idleCallback();
    }

    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}