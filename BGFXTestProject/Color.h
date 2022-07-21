#pragma once

#include "glm/glm.hpp"

class Color {
	private:
	uint32_t _abgr;

	public:
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xff);

	void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xff);

	uint8_t red();
	uint8_t green();
	uint8_t blue();
	uint8_t alpha();

	uint32_t abgr();
};

