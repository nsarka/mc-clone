#include "camera.h"


void Camera::handleMouseMotion(SDL_MouseMotionEvent motion, glm::vec2& currentEulerAngles)
{
    // Dont do anything with mouse look if the cursor is enabled
    if (m_cursorEnabled)
        return;

    int mouseX = motion.xrel;
    int mouseY = motion.yrel;

    float xOffset = (mouseX)*m_mouseSensitivity;
    float yOffset = (mouseY)*m_mouseSensitivity;

    currentEulerAngles.x += xOffset; // camera x is yaw
    currentEulerAngles.y += yOffset * (m_invertMouse ? 1.0f : -1.0f); // camera y is pitch

    if (currentEulerAngles.y < -89.0f) // pitch
        currentEulerAngles.y = -89.0f;

    if (currentEulerAngles.y > 89.0f)
        currentEulerAngles.y = 89.0f;

    updateCameraVectors(currentEulerAngles);
}

void Camera::handleMouseButtonDown(SDL_MouseButtonEvent mouseButtonEvent)
{
    switch (mouseButtonEvent.button)
    {
    case SDL_BUTTON_LEFT:
        break;
    case SDL_BUTTON_RIGHT:
        m_cursorEnabled = !m_cursorEnabled;
        SDL_ShowCursor(m_cursorEnabled ? SDL_ENABLE : SDL_DISABLE);
        SDL_SetRelativeMouseMode(m_cursorEnabled ? SDL_FALSE : SDL_TRUE);
        break;
    default:
        //SDL_ShowSimpleMessageBox(0, "Mouse", "Some unhandled mouse button was pressed", window);
        break;
    }
}

void Camera::updateCameraVectors(glm::vec2 eulerAngles)
{
    // calculate the new Front vector
    glm::vec3 front;

    front.x = cos(glm::radians(eulerAngles.x)) * cos(glm::radians(eulerAngles.y));
    front.y = sin(glm::radians(eulerAngles.y));
    front.z = sin(glm::radians(eulerAngles.x)) * cos(glm::radians(eulerAngles.y));
    m_front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 Camera::getViewMatrix(glm::vec3 position)
{
    return glm::lookAt(position, position + m_front, m_up);
}