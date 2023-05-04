#pragma once

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "player.h"

// Default camera values
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;
const bool  INVERT      = false;

class Camera
{
public:
    Camera(const float mouseSensitivity = SENSITIVITY, float fov = ZOOM, bool invertMouse = INVERT)
        : m_mouseSensitivity(mouseSensitivity)
        , m_fov(fov)
        , m_cursorEnabled(true)
        , m_invertMouse(invertMouse)
        , m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
    {
        updateCameraVectors(glm::vec2(0.0f, 0.0f));
    }

    ~Camera() {

    }

    void handleMouseMotion(SDL_MouseMotionEvent motion, glm::vec2& currentEulerAngles);
    void handleMouseButtonDown(SDL_MouseButtonEvent mouseButtonEvent);

    glm::mat4 getViewMatrix(glm::vec3 position);

    glm::vec3 getFront() const { return m_front; }
    glm::vec3 getUp() const { return m_up; }
    glm::vec3 getRight() const { return m_right; }

    float& getFov() { return m_fov; }


private:
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    const float m_mouseSensitivity;
    float m_fov;
    bool m_cursorEnabled;
    bool m_invertMouse;

    void updateCameraVectors(glm::vec2 eulerAngles);
};