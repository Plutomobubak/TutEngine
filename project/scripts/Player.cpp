#include "../../engine/core/tutLib.h"  // your common interface header

class PlayerSc : public TutScript {
private:
    const float SPEED = 1.0f;
    const float sensitivity = 0.001f;

    double lastX = 0.0;
    double lastY = 0.0;
    bool firstMouse = true;

    Entity player;
    Entity cam;

public:
    void Init() override {
        std::cout << "Init Player" << "\n";
        auto& reg = Registry::instance();
        player = GetEntity();
        std::cout << "Player script initialized for entity " << GetEntity() << "\n";
    }

    void Update(float deltaT) override {
        auto& reg = Registry::instance();
        if (!cam)
          cam = reg.find("Cam");
        Move(reg, deltaT);
        if (InputManager::IsMousePressed(0)) {
            InputManager::LockMouse(true);
        } else if (InputManager::IsKeyDown(GLFW_KEY_ESCAPE)) {
            InputManager::LockMouse(false);
        }
        if (InputManager::IsMouseLocked()) LookAround(reg, deltaT);
    }

    void LookAround(Registry& reg, float deltaT) {
        double mouseX, mouseY;
        InputManager::GetMousePosition(mouseX, mouseY);

        if (firstMouse) {
            lastX = mouseX;
            lastY = mouseY;
            firstMouse = false;
        }

        double xoffset = lastX - mouseX;
        double yoffset = lastY - mouseY;

        lastX = mouseX;
        lastY = mouseY;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        glm::vec3& protation = reg.get<Transform>(player).rotation;
        glm::vec3& crotation = reg.get<Transform>(cam).rotation;

        protation.y += (float)xoffset;
        crotation.x += (float)yoffset;

        // Clamp pitch to avoid flipping
        if (crotation.x > glm::radians(89.0f)) crotation.x = glm::radians(89.0f);
        if (crotation.x < glm::radians(-89.0f)) crotation.x = glm::radians(-89.0f);

        // Wrap yaw angle
        if (protation.y > glm::two_pi<float>()) protation.y -= glm::two_pi<float>();
        else if (protation.y < 0) protation.y += glm::two_pi<float>();

    }

    void Move(Registry& reg, float deltaT) {
        float fspeed = SPEED * deltaT;
        glm::vec3 euler = reg.get<Transform>(player).rotation;

        glm::mat4 rotMatrix = glm::yawPitchRoll(euler.y, euler.x, euler.z);

        glm::vec3 fwd = glm::vec3(rotMatrix * glm::vec4(0, 0, -1, 0)) * fspeed;
        glm::vec3 rgt = glm::vec3(rotMatrix * glm::vec4(1, 0, 0, 0)) * fspeed;
        glm::vec3 up = glm::vec3(rotMatrix * glm::vec4(0, 1, 0, 0)) * fspeed;

        if (InputManager::IsKeyDown(GLFW_KEY_W)) {
            reg.get<Transform>(player).position += fwd;
        }
        if (InputManager::IsKeyDown(GLFW_KEY_S)) {
            reg.get<Transform>(player).position -= fwd;
        }
        if (InputManager::IsKeyDown(GLFW_KEY_A)) {
            reg.get<Transform>(player).position -= rgt;
        }
        if (InputManager::IsKeyDown(GLFW_KEY_D)) {
            reg.get<Transform>(player).position += rgt;
        }
        if (InputManager::IsKeyDown(GLFW_KEY_SPACE)) {
            reg.get<Transform>(player).position += up;
        }
        if (InputManager::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
            reg.get<Transform>(player).position -= up;
        }
    }
};

// Entry point expected by the engine
extern "C" TutScript* CreateScript() {
    return new PlayerSc();
}
