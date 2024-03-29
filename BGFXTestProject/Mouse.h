#pragma once

#include <GLFW/glfw3.h>

class Mouse {
	static void pos_callback(GLFWwindow*, double, double);
	static void btn_callback(GLFWwindow*, int, int, int);
	static void scr_callback(GLFWwindow*, double, double);

	static double x;
	static double y;

	static double scroll_x;
	static double scroll_y;
	static bool btn[];

	GLFWwindow* window;

	double sens;

	public:
	Mouse(GLFWwindow* window);

	void setPosition(double x, double y);
	void setSensitivity(double sens);

	double getSensitivity() const;

	double getX() const;
	double getY() const;

	double getScrollX() const;
	double getScrollY() const;

	bool pressed(int _btn);
};
