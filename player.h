#pragma once

#include "camera.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;

class Player
{
public:
	Player()
		: m_position(glm::vec3(0.0f, 0.0f, 3.0f))
		, m_velocity(glm::vec3(0.0f, 0.0f, 0.0f))
		, m_eulerAngles(glm::vec2(YAW, PITCH)) // x: yaw, y: pitch
	{}

	~Player() {}

	glm::vec3& getPosition() { return m_position; }
	glm::vec3& getVelocity() { return m_velocity; }
	glm::vec2& getEulerAngles() { return m_eulerAngles; }

	/*
	void setPosition(glm::vec3 pos) { m_position = pos; }
	void setVelocity(glm::vec3 velocity) { m_velocity = velocity; }
	void setEulerAngles(glm::vec2 angles) { m_eulerAngles = angles; }
	*/

private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec2 m_eulerAngles; // x: yaw, y: pitch
};