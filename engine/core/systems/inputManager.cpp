#include "inputManager.h"

std::unordered_map<int, KeyState> InputManager::keyStates;
std::unordered_map<int, KeyState> InputManager::mouseButtonStates;
double InputManager::mouseX = 0.0;
double InputManager::mouseY = 0.0;
bool InputManager::mouseLocked = false;

void InputManager::Init(GLFWwindow* window) {
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    InputManager::window = window;
}

void InputManager::Update() {
    for (auto& [key, state] : keyStates) {
        if (state == KeyState::Pressed)
            state = KeyState::Held;
        else if (state == KeyState::Released)
            state = KeyState::None;
    }

    for (auto& [btn, state] : mouseButtonStates) {
        if (state == KeyState::Pressed)
            state = KeyState::Held;
        else if (state == KeyState::Released)
            state = KeyState::None;
    }
}

bool InputManager::IsKeyDown(int key) {
    return keyStates[key] == KeyState::Pressed || keyStates[key] == KeyState::Held;
}

bool InputManager::IsKeyPressed(int key) {
    return keyStates[key] == KeyState::Pressed;
}

bool InputManager::IsKeyReleased(int key) {
    return keyStates[key] == KeyState::Released;
}

bool InputManager::IsMouseDown(int button) {
    return mouseButtonStates[button] == KeyState::Pressed || mouseButtonStates[button] == KeyState::Held;
}

bool InputManager::IsMousePressed(int button) {
    return mouseButtonStates[button] == KeyState::Pressed;
}

bool InputManager::IsMouseReleased(int button) {
    return mouseButtonStates[button] == KeyState::Released;
}

void InputManager::GetMousePosition(double& x, double& y) {
    x = mouseX;
    y = mouseY;
}

// Callbacks

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {    
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (action == GLFW_PRESS){
        keyStates[key] = KeyState::Pressed;
 for (const auto& [k, state] : keyStates) {
        std::cout << "Key: " << k << " State: " << static_cast<int>(state) << std::endl;
    }
    }
    else if (action == GLFW_RELEASE)
        keyStates[key] = KeyState::Released;
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {    
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods); // Forward to ImGui
    if (action == GLFW_PRESS)
        mouseButtonStates[button] = KeyState::Pressed;
    else if (action == GLFW_RELEASE)
        mouseButtonStates[button] = KeyState::Released;
}

void InputManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;    

    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
}


void InputManager::LockMouse(bool lock){
    lock? glfwSetInputMode(InputManager::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED): glfwSetInputMode(InputManager::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    mouseLocked = lock;
}

bool InputManager::IsMouseLocked(){
  return mouseLocked;
}
