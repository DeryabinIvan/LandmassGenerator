#pragma once

#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"

#include "glm/glm.hpp"

class Player: public Camera {
	float speed = 0;

	double lx = 400, ly = 300;
	double yaw = -90, pitch = 0;

	public:
	Player(glm::vec3 posistion, float speed);
	Player(float x, float y, float z, float speed);

	void setSpeed(float speed);

	void move(const Keyboard& k);
	void look(const Mouse& m);
};

