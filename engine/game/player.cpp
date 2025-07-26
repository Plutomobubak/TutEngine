#include "player.h"


Entity Player::player;

const float SPEED = 1;
const float sensitivity = 0.001f;

double Player::lastX = 0.0;
double Player::lastY = 0.0;
bool Player::firstMouse = true;

void Player::Init(){
    auto& reg = Registry::instance();
    player = reg.create("PlayerR");
    reg.get<Transform>(player).position = glm::vec3(0,0,5);
    reg.add<Camera>(player,Camera{});
}

void Player::Update(float deltaT){
    auto& reg = Registry::instance();
    Player::Move(reg, deltaT);
    Player::LookAround(reg,deltaT);
}

void Player::LookAround(Registry& reg, float deltat){
    double mouseX, mouseY;
    InputManager::GetMousePosition(mouseX, mouseY);

    if (firstMouse) {
        lastX = mouseX;
        lastY = mouseY;
        firstMouse = false;
    }

    double xoffset = lastX - mouseX;
    double yoffset = lastY - mouseY;  // reversed y-axis for typical FPS

    lastX = mouseX;
    lastY = mouseY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    glm::vec3& rotation = reg.get<Transform>(player).rotation;

    rotation.y += (float)xoffset;  // yaw
    rotation.x += (float)yoffset;  // pitch

    // Clamp pitch to avoid flipping
    if (rotation.x > glm::radians(89.0f)) rotation.x = glm::radians(89.0f);
    if (rotation.x < glm::radians(-89.0f)) rotation.x = glm::radians(-89.0f);

    // Optionally, keep yaw in [0, 2pi] range:
    if (rotation.y > glm::two_pi<float>()) rotation.y -= glm::two_pi<float>();
    else if (rotation.y < 0) rotation.y += glm::two_pi<float>();


    if (InputManager::IsMousePressed(0)){
      InputManager::LockMouse(true);
    }
    else if (InputManager::IsKeyDown(GLFW_KEY_ESCAPE)){
      InputManager::LockMouse(false);
    }
}


void Player::Move(Registry& reg, float deltaT){
      float fspeed = SPEED * deltaT;
    glm::vec3 euler = reg.get<Transform>(player).rotation;

    // Create a rotation matrix
    glm::mat4 rotMatrix = glm::yawPitchRoll(euler.y, euler.x, euler.z);

    // Extract local basis vectors
    glm::vec3 fwd = glm::vec3(rotMatrix * glm::vec4(0, 0, -1, 0)) * fspeed; // OpenGL forward
    glm::vec3 rgt   = glm::vec3(rotMatrix * glm::vec4(1, 0,  0, 0)) * fspeed; // Right
    glm::vec3 up      = glm::vec3(rotMatrix * glm::vec4(0, 1,  0, 0)) * fspeed; // Up

      if(InputManager::IsKeyDown(GLFW_KEY_W)){
        reg.get<Transform>(player).position += fwd;
      }
      if(InputManager::IsKeyDown(GLFW_KEY_S)){
        reg.get<Transform>(player).position -= fwd;
      }
      

      if(InputManager::IsKeyDown(GLFW_KEY_A)){
        reg.get<Transform>(player).position -= rgt;
      }
      if(InputManager::IsKeyDown(GLFW_KEY_D)){
        reg.get<Transform>(player).position += rgt;
      }
      
      if(InputManager::IsKeyDown(GLFW_KEY_SPACE)){
        reg.get<Transform>(player).position += up;
      }
      if(InputManager::IsKeyDown(GLFW_KEY_LEFT_SHIFT)){
        reg.get<Transform>(player).position -= up;
      }
}

