#pragma warning(disable : 4099)

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#define BX_CONFIG_DEBUG 1
#define GLFW_DLL

#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <GLFW/glfw3.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Mouse.h"
#include "Player.h"
#include "Keyboard.h"
#include "ChunkManager.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define RESET_FLAGS (BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X4)

#define CHUNK_SIZE 10

const float FOV = 90.0f;

bgfx::ShaderHandle loadShader(const std::string FILENAME) {
	std::string shaderPath = "???";

	switch (bgfx::getRendererType()) {
		case bgfx::RendererType::Noop:
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
		case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
	}

	std::string filePath = shaderPath + FILENAME;

	std::ifstream file(filePath, std::ios::binary);
	int fileSize = std::filesystem::file_size(filePath);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	file.read((char*) mem->data, fileSize);
	mem->data[mem->size - 1] = '\0';

	file.close();

	return bgfx::createShader(mem);
}

int main(int argc, const char** argv) {
	glfwSetErrorCallback([](int error, const char* descr) {
		std::cerr << "GLFW Runtime error!" << "\n\tCode: " << error << "\n\tError: " << descr << std::endl;
	});

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	/*//Fulscreen mode
	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);*/
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello, bgfx!", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Window creation error!\n";
		glfwTerminate();
		return -1;
	}

	Keyboard keyboard(window);
	Mouse mouse(window);
	mouse.setSensitivity(0.15f);
	mouse.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	bool advanced_debug = false,
		wireframe_debug = false,
		rotation = false;

	//platform staff
	bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	init.platformData.ndt = glfwGetX11Display();
	init.platformData.nwh = (void*) (uintptr_t) glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
	init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
	init.platformData.nwh = glfwGetWin32Window(window);
#endif

	init.type = bgfx::RendererType::Vulkan;
	init.resolution.reset = RESET_FLAGS;

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	init.resolution.width = width;
	init.resolution.height = height;

	if (!bgfx::init(init)) {
		std::cout << "bgfx initialization error!" << std::endl;
		glfwTerminate();
		return -1;
	}

	bgfx::ViewId view_id = 0;
	bgfx::setViewClear(view_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF);
	bgfx::setViewRect(view_id, 0, 0, width, height);

	bgfx::ShaderHandle vert = loadShader("vs_cubes.bin");
	bgfx::ShaderHandle frag = loadShader("fs_cubes.bin");
	bgfx::ProgramHandle program = bgfx::createProgram(vert, frag, true);

	glm::mat4 projection = glm::perspectiveFov(FOV, (float) width, (float) height, 0.01f, 100.0f);

	Player player1(glm::vec3(0.f, 5.f, 0.f), .05f);
	//Camera camera(glm::vec3(0.0f, 0.5f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	ChunkManager manager(1, CHUNK_SIZE);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		player1.look(mouse);
		player1.move(keyboard);

		manager.update(player1.getPosition(), player1.getFrontVec());

		//<--------------------------keyboard event start-------------------------->
		if (keyboard.keyPressedOnce(Keyboard::KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, true);
		}

		if (keyboard.keyPressedOnce(Keyboard::KEY_F1)) {
			advanced_debug = !advanced_debug;
			wireframe_debug = false;
		}

		if (keyboard.keyPressedOnce(Keyboard::KEY_F2)) {
			wireframe_debug = !wireframe_debug;
			advanced_debug = false;
		}
		//<--------------------------keyboard event end-------------------------->

		//window resize.
		int oldWidth = width, oldHeight = height;
		glfwGetWindowSize(window, &width, &height);
		if (width != oldWidth || height != oldHeight) {
			bgfx::reset(width, height, RESET_FLAGS);
			bgfx::setViewRect(view_id, 0, 0, bgfx::BackbufferRatio::Equal);
			projection = glm::perspectiveFov(FOV, (float) width, (float) height, 0.01f, 100.0f);
		}

		bgfx::touch(view_id);

		//<--------------------------render start-------------------------->

		glm::mat4 view = player1.getViewMatrix();
		bgfx::setViewTransform(view_id, &view[0][0], &projection[0][0]);

		manager.draw(view_id, program);

		bgfx::dbgTextClear();
		const bgfx::Stats* stats = bgfx::getStats();
		bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 for advanced debug mode. Press F2 for wireframe mode");
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

		if (!wireframe_debug) {
			bgfx::setDebug(advanced_debug ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
		} else {
			bgfx::setDebug(BGFX_DEBUG_WIREFRAME);
		}

		//<--------------------------render end-------------------------->

		bgfx::frame();
	}

	bgfx::destroy(program);
	bgfx::shutdown();

	glfwTerminate();

	return 0;
}