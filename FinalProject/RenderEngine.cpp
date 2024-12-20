#include "RenderEngine.h"

RenderEngine::RenderEngine() {
}


RenderEngine::~RenderEngine() {
	glfwDestroyWindow(window);
}



void RenderEngine::Start(const char* title, unsigned int width, unsigned int height, bool vsync, bool fullscreen) {

	// ����Ӧ�ó�������
	this->screenHeight = height;
	this->screenWidth = width;

	// glfw: ��ʼ��������
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw ���ڴ���
	// --------------------
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(this->screenWidth, this->screenHeight, title, fullscreen ? monitor : NULL, NULL);
	if (window == NULL)
	{
		Err("Failed to create GLFW window");
	}

	// ���ô���λ������Ļ����
	// ---------------------------------------
	if (!fullscreen) {
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowPos(window, mode->width / 4, mode->height / 4);
	}

	// ���� OpenGL ������
	// ------------------
	glfwMakeContextCurrent(window);

	// glad: �������� OpenGL ����ָ��
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Err("Failed to initialize GLAD");
	}

	// ���ô�ֱͬ��
	// ---------
	glfwSwapInterval(vsync ? 1 : 0);

	// ��ʼ������
	// ---------------------
	Init();

	lastFrame = glfwGetTime() * 1000;

	// ��Ⱦѭ��
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// ����֡�ʺ�֡ʱ��
		double deltaTime = GetDeltaTime();
		GetFPS();

		// �û�����Ĵ������롢���º���Ⱦ����
		// ---------------------
		ProcessInput(window);
		Update(deltaTime);
		Render();

		// glfw: ��������������ѯ IO �¼�����������/�ͷţ�����ƶ��ȣ�
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		// ���Դ�ӡ֡��
		PrintFrameRate();
	}

	// �û�����ķ���ʼ������
	// ---------------------
	DeInit();

	// glfw: ��ֹ�����������ǰ����� GLFW ��Դ��
	// ------------------------------------------------------------------
	glfwTerminate();
}

double RenderEngine::GetDeltaTime()
{
    // ��ȡ��ǰʱ�䣨�Ժ���Ϊ��λ��
    double time = glfwGetTime() * 1000;
    // ��������һ֡��ʱ���
    double delta = time - lastFrame;
    // ������һ֡��ʱ��
    lastFrame = time;

    return delta;
}

void RenderEngine::GetFPS()
{
    // ��������ϴμ�¼����1��
    if (glfwGetTime() * 1000 - last > 1000) {
        // ����֡��
        fps = _fps;
        // ����֡������
        _fps = 0;
        // �����ϴμ�¼ʱ��
        last += 1000;
    }
    // ����֡������
    _fps++;
}


// ��ӡ������Ϣ���˳���Ϸ
void RenderEngine::Err(std::string errorString)
{
    std::cout << errorString << std::endl;
    glfwTerminate();
    exit(1);
}

static int frameCounter = 0;
void RenderEngine::PrintFrameRate() {
    // ÿ60֡��ӡһ��֡��
    frameCounter++;
    if (frameCounter == 60) {
        std::cout << "FPS: " << fps << std::endl;
        frameCounter = 0;
    }
}

// �����ɫ������
void RenderEngine::CheckShaderErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		// �����ɫ������״̬
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// ��ȡ����ӡ���������Ϣ
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			Err("| ERROR::::SHADER-COMPILATION-ERROR of type: " + type + "|\n" + infoLog + "\n| -- --------------------------------------------------- -- |");
		}
	}
	else
	{
		// ����������״̬
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			// ��ȡ����ӡ���Ӵ�����Ϣ
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			Err("| ERROR::::PROGRAM-LINKING-ERROR of type: " + type + "|\n" + infoLog + "\n| -- --------------------------------------------------- -- |");
		}
	}
}

GLuint RenderEngine::BuildShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ����Դ����
	std::string vertexCode, fragmentCode, geometryCode;
	std::ifstream vShaderFile, fShaderFile, gShaderFile;

	// ȷ�� ifstream ��������׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// ��ȡ�ļ����������ݵ�����
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// �ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();

		// ����ת��Ϊ�ַ���
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// ���������ɫ��·�����ڣ�Ҳ���ؼ�����ɫ��
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

	// 2. ������ɫ��
	GLuint vertex, fragment;
	// ������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckShaderErrors(vertex, "VERTEX");
	// Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckShaderErrors(fragment, "FRAGMENT");
	// ���������ɫ�����ڣ����뼸����ɫ��
	GLuint geometry;
	if (geometryPath != nullptr)
	{
		const GLchar* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		CheckShaderErrors(geometry, "GEOMETRY");
	}
	// ��ɫ������
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	if (geometryPath != nullptr)
		glAttachShader(program, geometry);
	glLinkProgram(program);
	CheckShaderErrors(program, "PROGRAM");
	// ɾ����ɫ������Ϊ�����Ѿ����ӵ����ǵĳ����У�������Ҫ
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
	return program;
}

void RenderEngine::UseShader(GLuint program)
{
    // ʹ�õ�ǰ��ɫ������
    glUseProgram(program);
}



