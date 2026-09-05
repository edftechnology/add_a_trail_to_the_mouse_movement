#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#endif

#include "Game.h"
#include "ResourceManager.h"
#include "Config.h"

#ifdef _WIN32
#include "WindowsOverlay.h"
#endif

#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
#if defined(__linux__)
void enable_click_through(GLFWwindow* window);
void set_desktop_overlay_size();
#endif

Game gameObject;

int main(int argc, char* argv[])
{
    // Initialize configuration system
    std::cout << "Cursor Trail - Customizable Version" << std::endl;
    
    // Try to load config from default file
    g_config.LoadFromFile("config.ini");
    
    // Parse command line arguments (override config file)
    if (g_config.ParseCommandLine(argc, argv) == false && argc > 1) {
        // If help was shown or there was an error, exit
        return 0;
    }
    
    // Print current configuration
    g_config.PrintConfig();
#ifdef _WIN32
    // Use Windows-specific overlay implementation for guaranteed top-level transparent overlay
    std::cout << "Starting Windows overlay mode for guaranteed transparency and top-level display..." << std::endl;
    
    WindowsOverlay overlay;
    if (!overlay.Initialize()) {
        std::cerr << "Failed to initialize Windows overlay. Falling back to OpenGL mode." << std::endl;
        // Fall through to OpenGL implementation
    } else {
        std::cout << "Windows overlay initialized successfully. Press Ctrl+C to exit." << std::endl;
        
        // Main loop for Windows overlay
        MSG msg = {};
        auto lastUpdate = GetTickCount64();
        
        while (true) {
            // Process Windows messages
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    goto cleanup;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            
            // Update and render at ~60fps
            auto currentTime = GetTickCount64();
            if (currentTime - lastUpdate >= 16) { // ~60fps
                overlay.Update();
                overlay.Render();
                lastUpdate = currentTime;
            }
            
            // Small sleep to prevent 100% CPU usage
            Sleep(1);
        }
        
    cleanup:
        overlay.Cleanup();
        return 0;
    }
#endif

    // OpenGL implementation (Windows fallback and other platforms)
    std::cout << "Starting OpenGL mode..." << std::endl;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Window hints for cursor trail overlay
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_FLOATING, true);

    // Improved Windows 11 compatibility - overlay should not take focus
    glfwWindowHint(GLFW_VISIBLE, true);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, false);  // Set to false for proper overlay behavior on Windows 11
    glfwWindowHint(GLFW_DECORATED, false);

#if defined(__linux__)
    set_desktop_overlay_size();
#else
    const GLFWvidmode* mode =  glfwGetVideoMode(glfwGetPrimaryMonitor());
    gameObject.Width = mode->width;
    gameObject.Height = mode->height;
#endif

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(gameObject.Width, gameObject.Height, "CursorTrail", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

#if defined(__linux__)
    glfwSetWindowPos(window, 0, 0);
    enable_click_through(window);
#endif
    
    glfwMakeContextCurrent(window);

    // Enable vsync to reduce CPU usage
    glfwSwapInterval(1);

    //glfwSetWindowOpacity(window, 0.7);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, gameObject.Width, gameObject.Height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set clear color to transparent for proper overlay transparency
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // initialize game
    // ---------------
    gameObject.Init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // update game state
        // -----------------
        gameObject.Update(window);

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT);
        gameObject.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

#if defined(__linux__)
void set_desktop_overlay_size()
{
    Display* display = glfwGetX11Display();
    if (display == nullptr) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        gameObject.Width = mode->width;
        gameObject.Height = mode->height;
        std::cerr << "Using primary monitor size: native X11 display unavailable." << std::endl;
        return;
    }

    int screen = DefaultScreen(display);
    gameObject.Width = static_cast<unsigned int>(DisplayWidth(display, screen));
    gameObject.Height = static_cast<unsigned int>(DisplayHeight(display, screen));
    std::cout << "Using X11 desktop size: " << gameObject.Width << "x" << gameObject.Height << std::endl;
}

void enable_click_through(GLFWwindow* window)
{
    Display* display = glfwGetX11Display();
    Window x11Window = glfwGetX11Window(window);

    if (display == nullptr || x11Window == 0) {
        std::cerr << "Could not enable X11 click-through: native window unavailable." << std::endl;
        return;
    }

    int shapeEventBase = 0;
    int shapeErrorBase = 0;
    if (XShapeQueryExtension(display, &shapeEventBase, &shapeErrorBase) == 0) {
        std::cerr << "Could not enable X11 click-through: XShape extension unavailable." << std::endl;
        return;
    }

    XShapeCombineRectangles(display, x11Window, ShapeInput, 0, 0, nullptr, 0, ShapeSet, YXBanded);
    XFlush(display);
    std::cout << "X11 click-through input region enabled." << std::endl;
}
#endif
