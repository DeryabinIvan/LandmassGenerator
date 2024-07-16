#include "Color.h"

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	_abgr = (alpha << 8) | blue;
	_abgr = (_abgr << 8) | green;
	_abgr = (_abgr << 8) | red;
}

void Color::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	_abgr = (alpha << 8) | blue;
	_abgr = (_abgr << 8) | green;
	_abgr = (_abgr << 8) | red;
}

uint8_t Color::red() {
	return _abgr;
}

uint8_t Color::green() {
	return (_abgr & 0x0000ff00) >> 8;
}

uint8_t Color::blue() {
	return (_abgr & 0x00ff0000) >> (8 * 2);
}

uint8_t Color::alpha() {
	return (_abgr & 0xff000000) >> (8 * 3);
}

uint32_t Color::abgr() {
	return _abgr;
}
