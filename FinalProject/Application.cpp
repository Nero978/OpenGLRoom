#include "Application.h"

// 构造函数
Application::Application() {}
// 析构函数
Application::~Application() {}

// 初始化光照
void Application::setupLighting(glm::vec3 lightPosition)
{
	// 设置光源位置
	glm::vec3 lightPos = glm::vec3(lightPosition);
	shader->setVec3("lightPos", lightPos);
	// 设置观察者位置
	shader->setVec3("viewPos", viewPos);
	// 设置光源颜色
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	shader->setVec3("lightColor", lightColor);
}

// 设置透视投影和视图矩阵
void Application::setupPerspective()
{
	// 设置透视投影矩阵
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	shader->setMat4("projection", projection);
	// 设置视图矩阵
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	shader->setMat4("view", view);
}

// 初始化摄像机
void Application::InitCamera()
{
	// 摄像机坐标
	posCamX = 5.0f;
	posCamY = 1.0f;
	posCamZ = 3.0f;
	// 视角坐标
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	// 上方向坐标
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	// 摄像机速度
	CAMERA_SPEED = 0.0025;
	// 摄像机视角
	fovy = 45.0f;
	// 观察者位置
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// 移动摄像机
void Application::MoveCamera(float speed)
{
	// 计算摄像机坐标
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

// 摄像机左右平移
void Application::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

// 摄像机旋转
void Application::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

// 创建着色器
void Application::Init()
{
	// 构建光照贴图着色器
	shader = new Shader("texturedCube.vert", "texturedCube.frag");
	shader->Init();
	shader2 = new Shader("vertexShader.vert", "fragmentShader.frag");
	shader2->Init();

	// 创建立方体的漫反射纹理和镜面反射纹理
	cubeDiffTex = new Texture("crate_diffusemap.png");
	cubeDiffTex->Init();
	cubeSpecTex = new Texture("crate_specularmap.png");
	cubeSpecTex->Init();

	// 创建立方体实例
	cube = new Cube(shader, cubeDiffTex, cubeSpecTex);
	cube->Init();
	cube->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化摄像机
	InitCamera();

	// 设置透视投影矩阵
	setupPerspective();

	// 设置光照
	glm::vec3 specificLightPos = glm::vec3(5.0f, 0.0f, -5.0f);
	setupLighting(specificLightPos);

	// 设置平面
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);
	BuildColoredPlane("marble.png");

	///////////////////////////////////////////////////////////////////////////////////////////////

	/*objek*/

	////mejabilliard
	// 初始化台球桌纹理
	billiardTex = new Texture("biliard.png");
	billiardTex->Init();
	// 创建台球桌实例
	billiard = new Cube(shader, billiardTex, billiardTex);
	billiard->Init();
	// 设置台球桌缩放比例
	billiard->SetScale(0.2f, 0.2f, 0.2f);

	////mejabilliard
	// 初始化台球桌面纹理
	mejabilliardTex = new Texture("mejabilliard.png");
	mejabilliardTex->Init();
	// 创建台球桌面实例
	mejabilliard = new Cube(shader, mejabilliardTex, mejabilliardTex);
	mejabilliard->Init();
	// 设置台球桌面缩放比例
	mejabilliard->SetScale(0.2f, 0.2f, 0.2f);

	////kakibilliard
	// 初始化台球桌腿纹理
	kakibilliardTex = new Texture("kaki.jpeg");
	kakibilliardTex->Init();
	// 创建台球桌腿实例
	kakibilliard = new Cube(shader, kakibilliardTex, kakibilliardTex);
	kakibilliard->Init();
	// 设置台球桌腿缩放比例
	billiard->SetScale(0.2f, 0.2f, 0.2f);

	//sofa
	//// dudukan dan sandaransofa
	// 初始化沙发座椅和靠背纹理
	sofaTex = new Texture("sofa.jpeg");
	sofaTex->Init();
	// 创建沙发座椅和靠背实例
	sofa = new Cube(shader, sofaTex, sofaTex);
	sofa->Init();
	// 设置沙发座椅和靠背缩放比例
	sofa->SetScale(0.2f, 0.2f, 0.2f);

	////kaki sofa
	// 初始化沙发腿纹理
	kakisofaTex = new Texture("kaki.jpeg");
	kakisofaTex->Init();
	// 创建沙发腿实例
	kakisofa = new Cube(shader, kakisofaTex, kakisofaTex);
	kakisofa->Init();
	// 设置沙发腿缩放比例
	kakisofa->SetScale(0.2f, 0.2f, 0.2f);

	//pintu
	// 初始化门纹理
	pintuTex = new Texture("pintu.jpeg");
	pintuTex->Init();
	// 创建门实例
	pintu = new Cube(shader, pintuTex, pintuTex);
	pintu->Init();
	// 设置门缩放比例
	pintu->SetScale(0.2f, 0.2f, 0.2f);

	// vending machine
	// 初始化自动售货机纹理
	vendingmachineTex = new Texture("vendingmachine.jpeg");
	vendingmachineTex->Init();
	// 创建自动售货机实例
	vendingmachine = new Cube(shader, vendingmachineTex, vendingmachineTex);
	vendingmachine->Init();
	// 设置自动售货机缩放比例
	vendingmachine->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化自动售货机箱体纹理
	kotakvendingmachineTex = new Texture("kotakvendingmachine.png");
	kotakvendingmachineTex->Init();
	// 创建自动售货机箱体实例
	kotakvendingmachine = new Cube(shader, kotakvendingmachineTex, kotakvendingmachineTex);
	kotakvendingmachine->Init();
	// 设置自动售货机箱体缩放比例
	kotakvendingmachine->SetScale(0.2f, 0.2f, 0.2f);

	//resepsionis
	// 初始化接待台纹理
	resepsionisTex = new Texture("resepsionis.png");
	resepsionisTex->Init();
	// 创建接待台实例
	resepsionis = new Cube(shader, resepsionisTex, resepsionisTex);
	resepsionis->Init();
	// 设置接待台缩放比例
	resepsionis->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化接待台桌面纹理
	mejaresepsionisTex = new Texture("marmer.png");
	mejaresepsionisTex->Init();
	// 创建接待台桌面实例
	mejaresepsionis = new Cube(shader, mejaresepsionisTex, mejaresepsionisTex);
	mejaresepsionis->Init();
	// 设置接待台桌面缩放比例
	mejaresepsionis->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化接待台铰链纹理
	engselTex = new Texture("kaki.jpeg");
	engselTex->Init();
	// 创建接待台铰链实例
	engsel = new Cube(shader, engselTex, engselTex);
	engsel->Init();
	// 设置接待台铰链缩放比例
	engsel->SetScale(0.2f, 0.2f, 0.2f);

	//monitor
	// 初始化显示器底座纹理
	monitorTex = new Texture("kaki.jpeg");
	monitorTex->Init();
	// 创建显示器底座实例
	monitor = new Cube(shader, monitorTex, monitorTex);
	monitor->Init();
	// 设置显示器底座缩放比例
	monitor->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化显示器屏幕纹理
	lcdmonitorTex = new Texture("lcd.png");
	lcdmonitorTex->Init();
	// 创建显示器屏幕实例
	lcdmonitor = new Cube(shader, lcdmonitorTex, lcdmonitorTex);
	lcdmonitor->Init();
	// 设置显示器屏幕缩放比例
	lcdmonitor->SetScale(0.2f, 0.2f, 0.2f);

	//keyboard
	// 初始化键盘纹理
	keyboardTex = new Texture("keyboard.png");
	keyboardTex->Init();
	// 创建键盘实例
	keyboard = new Cube(shader, keyboardTex, keyboardTex);
	keyboard->Init();
	// 设置键盘缩放比例
	keyboard->SetScale(0.2f, 0.2f, 0.2f);

	//mouse
	// 初始化鼠标纹理
	mouseTex = new Texture("mouse.png");
	mouseTex->Init();
	// 创建鼠标实例
	mouse = new Cube(shader, mouseTex, mouseTex);
	mouse->Init();
	// 设置鼠标缩放比例
	mouse->SetScale(0.2f, 0.2f, 0.2f);


	//lukisan 1 dan 2
	// 初始化第一幅画的纹理
	lukisan1Tex = new Texture("lukisan1.png");
	lukisan1Tex->Init();
	// 创建第一幅画实例
	lukisan1 = new Cube(shader, lukisan1Tex, lukisan1Tex);
	lukisan1->Init();
	// 设置第一幅画的缩放比例
	lukisan1->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化第二幅画的纹理
	lukisan2Tex = new Texture("lukisan2.png");
	lukisan2Tex->Init();
	// 创建第二幅画实例
	lukisan2 = new Cube(shader, lukisan2Tex, lukisan2Tex);
	lukisan2->Init();
	// 设置第二幅画的缩放比例
	lukisan2->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化第三幅画的纹理
	lukisan3Tex = new Texture("lukisan3.png");
	lukisan3Tex->Init();
	// 创建第三幅画实例
	lukisan3 = new Cube(shader, lukisan3Tex, lukisan3Tex);
	lukisan3->Init();
	// 设置第三幅画的缩放比例
	lukisan3->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化第四幅画的纹理
	lukisan4Tex = new Texture("lukisan4.png");
	lukisan4Tex->Init();
	// 创建第四幅画实例
	lukisan4 = new Cube(shader, lukisan4Tex, lukisan4Tex);
	lukisan4->Init();
	// 设置第四幅画的缩放比例
	lukisan4->SetScale(0.2f, 0.2f, 0.2f);

	//jam
	// 初始化时钟纹理
	jamTex = new Texture("jam.jpeg");
	jamTex->Init();
	// 创建时钟实例
	jam = new Cube(shader, jamTex, jamTex);
	jam->Init();
	// 设置时钟缩放比例
	jam->SetScale(0.2f, 0.2f, 0.2f);

	//AC
	// 初始化空调纹理
	acTex = new Texture("AC.png");
	acTex->Init();
	// 创建空调实例
	ac = new Cube(shader, acTex, acTex);
	ac->Init();
	// 设置空调缩放比例
	ac->SetScale(0.2f, 0.2f, 0.2f);

	// 初始化空调前面板纹理
	depanacTex = new Texture("depanAC.png");
	depanacTex->Init();
	// 创建空调前面板实例
	depanac = new Cube(shader, depanacTex, depanacTex);
	depanac->Init();
	// 设置空调前面板缩放比例
	depanac->SetScale(0.2f, 0.2f, 0.2f);

	//nomor meja
	////1
	// 初始化桌号1的纹理
	nomor1Tex = new Texture("1.jpeg");
	nomor1Tex->Init();
	// 创建桌号1实例
	nomor1 = new Cube(shader, nomor1Tex, nomor1Tex);
	nomor1->Init();
	// 设置桌号1的缩放比例
	nomor1->SetScale(0.2f, 0.2f, 0.2f);
	////2
	// 初始化桌号2的纹理
	nomor2Tex = new Texture("2.jpeg");
	nomor2Tex->Init();
	// 创建桌号2实例
	nomor2 = new Cube(shader, nomor2Tex, nomor2Tex);
	nomor2->Init();
	// 设置桌号2的缩放比例
	nomor2->SetScale(0.2f, 0.2f, 0.2f);
	////3
	// 初始化桌号3的纹理
	nomor3Tex = new Texture("3.jpeg");
	nomor3Tex->Init();
	// 创建桌号3实例
	nomor3 = new Cube(shader, nomor3Tex, nomor3Tex);
	nomor3->Init();
	// 设置桌号3的缩放比例
	nomor3->SetScale(0.2f, 0.2f, 0.2f);
	////4
	// 初始化桌号4的纹理
	nomor4Tex = new Texture("4.jpeg");
	nomor4Tex->Init();
	// 创建桌号4实例
	nomor4 = new Cube(shader, nomor4Tex, nomor4Tex);
	nomor4->Init();
	// 设置桌号4的缩放比例
	nomor4->SetScale(0.2f, 0.2f, 0.2f);

	//lampu
	// 初始化灯的纹理
	lampuTex = new Texture("lampu.png");
	lampuTex->Init();
	// 创建灯实例
	lampu = new Cube(shader, lampuTex, lampuTex);
	lampu->Init();
	// 设置灯的缩放比例
	lampu->SetScale(0.2f, 0.2f, 0.2f);

	////standlampu
	// 初始化灯架的纹理
	standlampuTex = new Texture("standlampu.png");
	standlampuTex->Init();
	// 创建灯架实例
	standlampu = new Cube(shader, standlampuTex, standlampuTex);
	standlampu->Init();
	// 设置灯架的缩放比例
	standlampu->SetScale(0.2f, 0.2f, 0.2f);

	/*Ruangan*/
	////dinding
	// 初始化墙壁纹理
	dindingTex = new Texture("dinding.png");
	dinding = new Cube(shader, dindingTex, dindingTex);
	dinding->Init();
	// 设置墙壁缩放比例
	dinding->SetScale(0.2f, 0.2f, 0.2f);
	////atap 
	// 初始化天花板纹理
	atapTex = new Texture("atap.png");
	atap = new Cube(shader, atapTex, atapTex);
	atap->Init();
	// 设置天花板缩放比例
	atap->SetScale(0.2f, 0.2f, 0.2f);


}
///////////////////////////////////////////////////////////////////////////////////////////////

// 资源释放
void Application::DeInit()
{
	/*objek*/
	// 释放立方体对象
	delete cube;
	// 释放台球桌对象
	delete billiard, mejabilliard, kakibilliard;
	// 释放沙发对象
	delete sofa, kakisofa;
	// 释放门对象
	delete pintu;
	// 释放自动售货机对象
	delete vendingmachine, kotakvendingmachine;
	// 释放接待台对象
	delete resepsionis, mejaresepsionis, engsel;
	// 释放画作对象
	delete lukisan1, lukisan2, lukisan3, lukisan4;
	// 释放时钟对象
	delete jam;
	// 释放桌号对象
	delete nomor1, nomor2, nomor3, nomor4;
	// 释放空调对象
	delete ac, depanac;
	// 释放显示器对象
	delete monitor, lcdmonitor, keyboard, mouse;
	// 释放灯对象
	delete lampu, standlampu;

	/*ruangan*/
	// 释放墙壁和天花板对象
	delete dinding, atap;

	/*Plane*/
	// 删除顶点数组对象
	glDeleteVertexArrays(1, &VAO);
	// 删除顶点缓冲对象
	glDeleteBuffers(1, &VBO);
	// 删除元素缓冲对象
	glDeleteBuffers(1, &EBO);
}

void Application::Update(double deltaTime)
{
	angle += (float)((deltaTime * 1.5f) / 1000);
	cube->SetRotation(angle, 0, 1, 0);
}

// 渲染场景
void Application::Render()
{
	// 设置视口
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	// 清除颜色缓冲和深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 设置清除颜色为白色
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 设置多边形模式为填充模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);

	// 设置透视投影矩阵
	setupPerspective();

	// 绘制彩色平面
	DrawColoredPlane();

	////////////////////////////////////////////////////////////////////////////////////////////////

	/*objek*/

	// 台球桌
	//// 台球桌 1 (x=0.z=0)
	billiard->SetPosition(-0.105, -6555.0, -1.05);
	billiard->SetScale(6.5f, 0.0001f, 4.5f);
	billiard->Draw();
	//// 台球桌底部 1
	kakibilliard->SetPosition(-0.105, -2.6, -1.32);
	kakibilliard->SetScale(5.63f, 0.5f, 3.63f);
	kakibilliard->Draw();
	//// 台球桌顶部 1
	mejabilliard->SetPosition(-0.105, -2.14, -1.05);
	mejabilliard->SetScale(6.5f, 0.4f, 4.5f);
	mejabilliard->Draw();
	//// 台球桌腿 1
	for (int i = 0; i < 4; i++) {
		if (i > 1) {
			kakibilliard->SetPosition(-8.0 + (i - 2) * 13, -1.2, -16);
			kakibilliard->SetScale(0.4f, 1.5f, 0.4f);
			kakibilliard->Draw();
		}
		else {
			kakibilliard->SetPosition(-8.0 + i * 13, -1.2, -8);
			kakibilliard->SetScale(0.4f, 1.5f, 0.4f);
			kakibilliard->Draw();
		}
	}
	//// 台球桌 2 (z)
	billiard->SetPosition(-0.105, -6555.0, -3.6);
	billiard->SetScale(6.5f, 0.0001f, 4.5f);
	billiard->Draw();
	//// 台球桌底部 2
	kakibilliard->SetPosition(-0.105, -2.6, -4.46);
	kakibilliard->SetScale(5.63f, 0.5f, 3.63f);
	kakibilliard->Draw();
	//// 台球桌顶部 2
	mejabilliard->SetPosition(-0.105, -2.14, -3.6);
	mejabilliard->SetScale(6.5f, 0.4f, 4.5f);
	mejabilliard->Draw();
	//// 台球桌腿 2
	for (int i = 0; i < 4; i++) {
		if (i > 1) {
			kakibilliard->SetPosition(-8.0 + (i - 2) * 13, -1.2, -44.5);
			kakibilliard->SetScale(0.4f, 1.5f, 0.4f);
			kakibilliard->Draw();
		}
		else {
			kakibilliard->SetPosition(-8.0 + i * 13, -1.2, -36.5);
			kakibilliard->SetScale(0.4f, 1.5f, 0.4f);
			kakibilliard->Draw();
		}
	}
	//// 台球桌 3 (x)
	billiard->SetPosition(1.932, -6555.0, -1.05);
	billiard->SetScale(6.5f, 0.0001f, 4.5f);
	billiard->Draw();
	//// 台球桌底部 3
	kakibilliard->SetPosition(2.237, -2.6, -1.32);
	kakibilliard->SetScale(5.63f, 0.5f, 3.63f);
	kakibilliard->Draw();
	//// 台球桌顶部 3
	mejabilliard->SetPosition(1.932, -2.14, -1.05);
	mejabilliard->SetScale(6.5f, 0.4f, 4.5f);
	mejabilliard->Draw();
	//// 台球桌腿 3
	for (int i = 0; i < 4; i++) {
		if (i > 1) {
			kakibilliard->SetPosition(25.0 + (i - 2) * 13, -1.2, -16);
			kakibilliard->SetScale(0.4f, 1.5f, 0.4f);
			kakibilliard->Draw();
		}
		else {
			kakibilliard->SetPosition(25.0 + i * 13, -1.2, -8);
			kakibilliard->SetScale(0.4f, 1.5f, 0.4f);
			kakibilliard->Draw();
		}
	}
	//// 台球桌 4 (x(台球桌3),z(台球桌2))
	billiard->SetPosition(1.932, -6555.0, -3.6);
	billiard->SetScale(6.5f, 0.0001f, 4.5f);
	billiard->Draw();
	//// 台球桌底部 4
	kakibilliard->SetPosition(2.237, -2.6, -4.46);
	kakibilliard->SetScale(5.63f, 0.5f, 3.63f);
	kakibilliard->Draw();
	//// 台球桌顶部 4
	mejabilliard->SetPosition(1.932, -2.14, -3.6);
	mejabilliard->SetScale(6.5f, 0.4f, 4.5f);
	mejabilliard->Draw();
	//// 台球桌腿 4
	for (int i = 0; i < 4; i++) {
		if (i > 1) {
			kakibilliard->SetPosition(25.0 + (i - 2) * 13, -1.2, -44.5);
			kakibilliard->SetScale(0.4f, 1.5f, 0.4f);
			kakibilliard->Draw();
		}
		else {
			kakibilliard->SetPosition(25.0 + i * 13, -1.2, -36.5);
			kakibilliard->SetScale(0.4f, 1.5f, 0.4f);
			kakibilliard->Draw();
		}
	}

    // 沙发
    //// 沙发座椅 1
    sofa->SetPosition(-0.125, -2.9, -11.3);
    sofa->SetScale(5.0f, 0.5f, 2.0f);
    sofa->Draw();
    //// 沙发靠背 1
    sofa->SetPosition(-0.125, -0.35, -46.71);
    sofa->SetScale(5.0f, 2.0f, 0.5f);
    sofa->Draw();
    //// 沙发左腿 1
    kakisofa->SetPosition(-6.75, -0.75, -11.3);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// 沙发右腿 1
    kakisofa->SetPosition(4.25, -0.75, -11.3);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// 沙发座椅 2
    sofa->SetPosition(2.525, -2.9, -11.3);
    sofa->SetScale(5.0f, 0.5f, 2.0f);
    sofa->Draw();
    //// 沙发靠背 2
    sofa->SetPosition(2.525, -0.35, -46.71);
    sofa->SetScale(5.0f, 2.0f, 0.5f);
    sofa->Draw();
    //// 沙发左腿 2
    kakisofa->SetPosition(19.75, -0.75, -11.3);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// 沙发右腿 2
    kakisofa->SetPosition(30.75, -0.75, -11.3);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// 沙发座椅 3
    sofa->SetPosition(-0.125, -2.9, 0.8);
    sofa->SetScale(5.0f, 0.5f, 2.0f);
    sofa->Draw();
    //// 沙发靠背 3
    sofa->SetPosition(-0.125, -0.35, 4.71);
    sofa->SetScale(5.0f, 2.0f, 0.5f);
    sofa->Draw();
    //// 沙发左腿 3
    kakisofa->SetPosition(-6.75, -0.75, 0.8);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// 沙发右腿 3
    kakisofa->SetPosition(4.25, -0.75, 0.8);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// 沙发座椅 4
    sofa->SetPosition(2.525, -2.9, 0.8);
    sofa->SetScale(5.0f, 0.5f, 2.0f);
    sofa->Draw();
    //// 沙发靠背 4
    sofa->SetPosition(2.525, -0.35, 4.71);
    sofa->SetScale(5.0f, 2.0f, 0.5f);
    sofa->Draw();
    //// 沙发左腿 4
    kakisofa->SetPosition(19.75, -0.75, 0.8);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// 沙发右腿 4
    kakisofa->SetPosition(30.75, -0.75, 0.8);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();

    // 门
    pintu->SetPosition(-44.7, 0.075, -2.120);
    pintu->SetScale(0.2f, 6.0f, 5.0f);
    pintu->Draw();

    // 自动售货机
    //// 售货机箱体
    kotakvendingmachine->SetPosition(1.5, 0.035, -6.55);
    kotakvendingmachine->SetScale(4.0f, 5.5, 3.6f);
    kotakvendingmachine->Draw();
    //// 售货机前面板
    vendingmachine->SetPosition(1.5, 0.035, -21779.55);
    vendingmachine->SetScale(4.0f, 5.5, 0.001f);
    vendingmachine->Draw();

    // 接待台
    //// 接待台底部
    resepsionis->SetPosition(35.0, -0.5, -2.85);
    resepsionis->SetScale(0.5f, 2.5f, 3.7f);
    resepsionis->Draw();

    resepsionis->SetPosition(5.26, -0.5, -24.3);
    resepsionis->SetScale(3.625f, 2.5f, 0.5f);
    resepsionis->Draw();

    resepsionis->SetPosition(9.58, -0.5, -17.85);
    resepsionis->SetScale(1.9f, 2.5f, 0.5f);
    resepsionis->Draw();

    //// 接待台桌面
    mejaresepsionis->SetPosition(23.15, 0.5, -3.9);
    mejaresepsionis->SetScale(0.75f, 0.25f, 2.7f);
    mejaresepsionis->Draw();

    mejaresepsionis->SetPosition(4.855, 0.5, -16.35);
    mejaresepsionis->SetScale(3.9f, 0.25f, 0.75f);
    mejaresepsionis->Draw();

    mejaresepsionis->SetPosition(8.59, 0.5, -11.74);
    mejaresepsionis->SetScale(2.1f, 0.25f, 0.75f);
    mejaresepsionis->Draw();

    //// 接待台
    mejaresepsionis->SetPosition(24.15, -0.0, -3.9);
    mejaresepsionis->SetScale(0.75f, 0.25f, 2.7f);
    mejaresepsionis->Draw();

    //// 接待台小门
    resepsionis->SetPosition(14.75, -0.5, -89.55);
    resepsionis->SetScale(1.35f, 1.75f, 0.1f);
    resepsionis->Draw();

    //// 接待台门铰链
    engsel->SetPosition(96.0, -4.0, -89.5);
    engsel->SetScale(0.2f, 0.1f, 0.1f);
    engsel->Draw();

    engsel->SetPosition(96.0, -14.0, -89.5);
    engsel->SetScale(0.2f, 0.1f, 0.1f);
    engsel->Draw();

    // 时钟
    jam->SetPosition(20840.55, 5.0, -10.5);
    jam->SetScale(0.001f, 1.0f, 1.0f);
    jam->Draw();

    // 画作
    lukisan1->SetPosition(20840.55, 0.635, -4.0);
    lukisan1->SetScale(0.001f, 5.5, 4.0f);
    lukisan1->Draw();

    lukisan2->SetPosition(20840.55, 0.635, -1.2);
    lukisan2->SetScale(0.001f, 5.5, 4.0f);
    lukisan2->Draw();

    lukisan3->SetPosition(-8850, 0.635, -4.0);
    lukisan3->SetScale(0.001f, 5.5, 4.0f);
    lukisan3->Draw();

    lukisan4->SetPosition(-8850, 0.635, -1.2);
    lukisan4->SetScale(0.001f, 5.5, 4.0f);
    lukisan4->Draw();

    // 桌号
    //// 1
    nomor1->SetPosition(-0.5, 3.0, -25350.0);
    nomor1->SetScale(1.0f, 1.5f, 0.001f);
    nomor1->Draw();
    //// 2
    nomor2->SetPosition(13.0, 3.0, -25350.0);
    nomor2->SetScale(1.0f, 1.5f, 0.001f);
    nomor2->Draw();
    //// 3
    nomor3->SetPosition(13.0, 3.0, 4350.0);
    nomor3->SetScale(1.0f, 1.5f, 0.001f);
    nomor3->Draw();
    //// 4
    nomor4->SetPosition(-0.5, 3.0, 4350.0);
    nomor4->SetScale(1.0f, 1.5f, 0.001f);
    nomor4->Draw();

    // 显示器
    //// 显示器底座
    monitor->SetPosition(34.8, 5.5, -21);
    monitor->SetScale(0.5f, 0.05f, 0.5f);
    monitor->Draw();
    //// 显示器支架
    monitor->SetPosition(116, 0.4, -35);
    monitor->SetScale(0.15f, 1.0f, 0.3f);
    monitor->Draw();
    //// 显示器屏幕
    monitor->SetPosition(117, 1.0, -7);
    monitor->SetScale(0.15f, 1.0f, 1.5f);
    monitor->Draw();
    //// 显示器LCD
    lcdmonitor->SetPosition(17630, 1.25, -8.075);
    lcdmonitor->SetScale(0.001f, 0.8f, 1.3f);
    lcdmonitor->Draw();

    // 键盘
    keyboard->SetPosition(36.3, 3.0, -10.25);
    keyboard->SetScale(0.5f, 0.05f, 1.0f);
    keyboard->Draw();

    // 鼠标
    mouse->SetPosition(60.5, 3.0, -55.25);
    mouse->SetScale(0.3f, 0.05f, 0.2f);
    mouse->Draw();

    // 灯
    //// 灯座 1
    standlampu->SetPosition(-4.0, -9.6, -13.5);
    standlampu->SetScale(1.75f, 0.25f, 1.75f);
    standlampu->Draw();
    //// 灯杆 1
    standlampu->SetPosition(-28, -0.1, -94.5);
    standlampu->SetScale(0.25f, 3.75f, 0.25f);
    standlampu->Draw();
    //// 灯 1
    lampu->SetPosition(-4.7, 1.5, -15.75);
    lampu->SetScale(1.5f, 1.5f, 1.5f);
    lampu->Draw();

    //// 灯座 2
    standlampu->SetPosition(10.925, -9.6, -13.5);
    standlampu->SetScale(1.75f, 0.25f, 1.75f);
    standlampu->Draw();
    //// 灯杆 2
    standlampu->SetPosition(76.5, -0.1, -94.5);
    standlampu->SetScale(0.25f, 3.75f, 0.25f);
    standlampu->Draw();
    //// 灯 2
    lampu->SetPosition(12.75, 1.5, -15.75);
    lampu->SetScale(1.5f, 1.5f, 1.5f);
    lampu->Draw();

    //// 灯座 3
    standlampu->SetPosition(10.925, -9.6, 1.45);
    standlampu->SetScale(1.75f, 0.25f, 1.75f);
    standlampu->Draw();
    //// 灯杆 3
    standlampu->SetPosition(76.5, -0.1, 10.25);
    standlampu->SetScale(0.25f, 3.75f, 0.25f);
    standlampu->Draw();
    //// 灯 3
    lampu->SetPosition(12.75, 1.5, 1.7);
    lampu->SetScale(1.5f, 1.5f, 1.5f);
    lampu->Draw();

    //// 灯座 4
    standlampu->SetPosition(-4.0, -9.6, 1.45);
    standlampu->SetScale(1.75f, 0.25f, 1.75f);
    standlampu->Draw();
    //// 灯杆 4
    standlampu->SetPosition(-28, -0.1, 10.25);
    standlampu->SetScale(0.25f, 3.75f, 0.25f);
    standlampu->Draw();
    //// 灯 4
    lampu->SetPosition(-4.7, 1.5, 1.7);
    lampu->SetScale(1.5f, 1.5f, 1.5f);
    lampu->Draw();

    // 空调
    ac->SetPosition(-13.65, 5.8, -2.820);
    ac->SetScale(0.625f, 0.75f, 3.75f);
    ac->Draw();
    //// 空调前面板
    depanac->SetPosition(-8215, 5.8, -2.820);
    depanac->SetScale(0.001f, 0.75f, 3.75f);
    depanac->Draw();

	// 绘制四面墙壁
	for (int i = 0; i < 4; i++) {
		if (i > 1) {
			// 设置墙壁位置和缩放比例
			dinding->SetPosition(70 - (i - 2) * 100, 0.275, -0.35);
			dinding->SetScale(0.30f, 11.75f, 30.0f);
			// 绘制墙壁
			dinding->Draw();
		}
		else {
			// 设置墙壁位置和缩放比例
			dinding->SetPosition(0.2, 0.275, 15 - i * 100);
			dinding->SetScale(30.0f, 11.75f, 0.30f);
			// 绘制墙壁
			dinding->Draw();
		}
	}

	// 设置天花板位置和缩放比例
	atap->SetPosition(0.2, 30, -0.35);
	atap->SetScale(30.0f, 0.30f, 30.0f);
	// 绘制天花板
	atap->Draw();

	///////////////////////////////////////////////////////////////////////////////////////////////

	// 禁用深度测试
	glDisable(GL_DEPTH_TEST);

}

// 处理输入
void Application::ProcessInput(GLFWwindow* window)
{
	// 如果按下ESC键，关闭窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// 缩放摄像机
	// -----------
	// 如果按下鼠标右键，增加视角
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	// 如果按下鼠标左键，减小视角
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// 更新摄像机移动
	// -------------
	// 如果按下W键，向前移动摄像机
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	// 如果按下S键，向后移动摄像机
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	// 如果按下A键，向左平移摄像机
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	// 如果按下D键，向右平移摄像机
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// 如果按下左Ctrl键，向下移动摄像机
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		posCamY -= CAMERA_SPEED;
	}

	// 如果按下空格键，向上移动摄像机
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		posCamY += CAMERA_SPEED;
	}

	// 更新摄像机旋转
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// 获取鼠标位置
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// 设置鼠标位置
	glfwSetCursorPos(window, midX, midY);

	// 获取鼠标光标的方向，设置合理的操作速度
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// 值越大，摄像机转动越快
	viewCamY += angleZ * 2;

	// 限制摄像机在x轴上的旋转
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);
}

// 构建彩色平面
void Application::BuildColoredPlane(const char* texturePath)
{
	// 加载并创建纹理
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 构建几何体
	GLfloat vertices[] = {
		// 位置和纹理坐标
		// 底部
		-0.5, -38.5, -0.5,  0,  0,
		0.5, -38.5, -0.5, 8,  0,
		0.5, -38.5,  0.5, 8, 8,
		-0.5, -38.5,  0.5,  0, 8,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// 绑定VAO
	glBindVertexArray(VAO);

	// 绑定VBO并设置顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 绑定EBO并设置索引数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 解绑VAO
	glBindVertexArray(0);
}

// 绘制彩色平面
void Application::DrawColoredPlane()
{
	// 绑定VAO
	glBindVertexArray(VAO);

	// 绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// 绘制元素
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// 解绑VAO
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	// 创建Application实例
	Application app = Application();
	// 启动应用程序
	app.Start("2024 CG Class Design",
		1920, 1080,
		false,
		false);
}