#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"

enum class KeyState {
    None,
    Pressed,
    Held,
    Released
};

class InputManager {
public:
    static void Init(GLFWwindow* window);
    static void Update(); // Call every frame
    static bool IsKeyDown(int key);
    static bool IsKeyPressed(int key);
    static bool IsKeyReleased(int key);
    
    static bool IsMouseDown(int button);
    static bool IsMousePressed(int button);
    static bool IsMouseReleased(int button);
    static void GetMousePosition(double& x, double& y);
    static void LockMouse(bool lock);
    static bool IsMouseLocked();

private:
    static inline GLFWwindow* window = nullptr; 
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static std::unordered_map<int, KeyState> keyStates;
    static std::unordered_map<int, KeyState> mouseButtonStates;
    static double mouseX, mouseY;
    static bool mouseLocked;
};
