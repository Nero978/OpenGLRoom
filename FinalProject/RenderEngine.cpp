#include "RenderEngine.h"

RenderEngine::RenderEngine() {
}


RenderEngine::~RenderEngine() {
	glfwDestroyWindow(window);
}



void RenderEngine::Start(const char* title, unsigned int width, unsigned int height, bool vsync, bool fullscreen) {

	// 设置应用程序配置
	this->screenHeight = height;
	this->screenWidth = width;

	// glfw: 初始化并配置
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw 窗口创建
	// --------------------
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(this->screenWidth, this->screenHeight, title, fullscreen ? monitor : NULL, NULL);
	if (window == NULL)
	{
		Err("Failed to create GLFW window");
	}

	// 设置窗口位置在屏幕中央
	// ---------------------------------------
	if (!fullscreen) {
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowPos(window, mode->width / 4, mode->height / 4);
	}

	// 设置 OpenGL 上下文
	// ------------------
	glfwMakeContextCurrent(window);

	// glad: 加载所有 OpenGL 函数指针
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Err("Failed to initialize GLAD");
	}

	// 设置垂直同步
	// ---------
	glfwSwapInterval(vsync ? 1 : 0);

	// 初始化函数
	// ---------------------
	Init();

	lastFrame = glfwGetTime() * 1000;

	// 渲染循环
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// 计算帧率和帧时间
		double deltaTime = GetDeltaTime();
		GetFPS();

		// 用户定义的处理输入、更新和渲染函数
		// ---------------------
		ProcessInput(window);
		Update(deltaTime);
		Render();

		// glfw: 交换缓冲区并轮询 IO 事件（按键按下/释放，鼠标移动等）
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		// 调试打印帧率
		PrintFrameRate();
	}

	// 用户定义的反初始化函数
	// ---------------------
	DeInit();

	// glfw: 终止，清除所有先前分配的 GLFW 资源。
	// ------------------------------------------------------------------
	glfwTerminate();
}

double RenderEngine::GetDeltaTime()
{
    // 获取当前时间（以毫秒为单位）
    double time = glfwGetTime() * 1000;
    // 计算与上一帧的时间差
    double delta = time - lastFrame;
    // 更新上一帧的时间
    lastFrame = time;

    return delta;
}

void RenderEngine::GetFPS()
{
    // 如果距离上次记录超过1秒
    if (glfwGetTime() * 1000 - last > 1000) {
        // 更新帧率
        fps = _fps;
        // 重置帧计数器
        _fps = 0;
        // 更新上次记录时间
        last += 1000;
    }
    // 增加帧计数器
    _fps++;
}


// 打印错误信息并退出游戏
void RenderEngine::Err(std::string errorString)
{
    std::cout << errorString << std::endl;
    glfwTerminate();
    exit(1);
}

static int frameCounter = 0;
void RenderEngine::PrintFrameRate() {
    // 每60帧打印一次帧率
    frameCounter++;
    if (frameCounter == 60) {
        std::cout << "FPS: " << fps << std::endl;
        frameCounter = 0;
    }
}

// 检查着色器错误
void RenderEngine::CheckShaderErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		// 检查着色器编译状态
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// 获取并打印编译错误信息
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			Err("| ERROR::::SHADER-COMPILATION-ERROR of type: " + type + "|\n" + infoLog + "\n| -- --------------------------------------------------- -- |");
		}
	}
	else
	{
		// 检查程序链接状态
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			// 获取并打印链接错误信息
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			Err("| ERROR::::PROGRAM-LINKING-ERROR of type: " + type + "|\n" + infoLog + "\n| -- --------------------------------------------------- -- |");
		}
	}
}

GLuint RenderEngine::BuildShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// 1. 从文件路径中获取顶点/片段着色器的源代码
	std::string vertexCode, fragmentCode, geometryCode;
	std::ifstream vShaderFile, fShaderFile, gShaderFile;

	// 确保 ifstream 对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// 读取文件缓冲区内容到流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();

		// 将流转换为字符串
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// 如果几何着色器路径存在，也加载几何着色器
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		Err("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2. 编译着色器
	GLuint vertex, fragment;
	// 顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckShaderErrors(vertex, "VERTEX");
	// 片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckShaderErrors(fragment, "FRAGMENT");
	// 如果几何着色器存在，编译几何着色器
	GLuint geometry;
	if (geometryPath != nullptr)
	{
		const GLchar* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		CheckShaderErrors(geometry, "GEOMETRY");
	}
	// 着色器程序
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	if (geometryPath != nullptr)
		glAttachShader(program, geometry);
	glLinkProgram(program);
	CheckShaderErrors(program, "PROGRAM");
	// 删除着色器，因为它们已经链接到我们的程序中，不再需要
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
	return program;
}

void RenderEngine::UseShader(GLuint program)
{
    // 使用当前着色器程序
    glUseProgram(program);
}



