#pragma once

#include "Application.h"


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "game/ball.h"
#include "game/table.h"
#include "game/sound.h"

// ���캯��
Application::Application() {}
// ��������
Application::~Application() {}

// ��ʼ������
void Application::setupLighting(glm::vec3 lightPosition)
{
	// ���ù�Դλ��
	glm::vec3 lightPos = glm::vec3(lightPosition);
	shader->setVec3("lightPos", lightPos);
	// ���ù۲���λ��
	shader->setVec3("viewPos", viewPos);
	// ���ù�Դ��ɫ
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	shader->setVec3("lightColor", lightColor);
}

// ����͸��ͶӰ����ͼ����
void Application::setupPerspective()
{
	// ����͸��ͶӰ����
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	shader->setMat4("projection", projection);
	// ������ͼ����
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	shader->setMat4("view", view);
}

// ��ʼ�������
void Application::InitCamera()
{
	// ���������
	posCamX = 5.0f;
	posCamY = 1.0f;
	posCamZ = 3.0f;
	// �ӽ�����
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	// �Ϸ�������
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	// ������ٶ�
	CAMERA_SPEED = 0.0025;
	// ������ӽ�
	fovy = 45.0f;
	// �۲���λ��
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// �ƶ������
void Application::MoveCamera(float speed)
{
	// �������������
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

// ���������ƽ��
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

// �������ת
void Application::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

// ������ɫ��
void Application::Init()
{
	// ����������ͼ��ɫ��
	shader = new Shader("texturedCube.vert", "texturedCube.frag");
	shader->Init();
	shader2 = new Shader("vertexShader.vert", "fragmentShader.frag");
	shader2->Init();

	// ���������������������;��淴������
	cubeDiffTex = new Texture("crate_diffusemap.png");
	cubeDiffTex->Init();
	cubeSpecTex = new Texture("crate_specularmap.png");
	cubeSpecTex->Init();

	// ����������ʵ��
	cube = new Cube(shader, cubeDiffTex, cubeSpecTex);
	cube->Init();
	cube->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ�������
	InitCamera();

	// ����͸��ͶӰ����
	setupPerspective();

	// ���ù���
	glm::vec3 specificLightPos = glm::vec3(5.0f, 0.0f, -5.0f);
	setupLighting(specificLightPos);

	// ����ƽ��
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);
	BuildColoredPlane("marble.png");

	///////////////////////////////////////////////////////////////////////////////////////////////

	/*objek*/

	////mejabilliard
	// ��ʼ��̨��������
	billiardTex = new Texture("biliard.png");
	billiardTex->Init();
	// ����̨����ʵ��
	billiard = new Cube(shader, billiardTex, billiardTex);
	billiard->Init();
	// ����̨�������ű���
	billiard->SetScale(0.2f, 0.2f, 0.2f);

	////mejabilliard
	// ��ʼ��̨����������
	mejabilliardTex = new Texture("mejabilliard.png");
	mejabilliardTex->Init();
	// ����̨������ʵ��
	mejabilliard = new Cube(shader, mejabilliardTex, mejabilliardTex);
	mejabilliard->Init();
	// ����̨���������ű���
	mejabilliard->SetScale(0.2f, 0.2f, 0.2f);

	////kakibilliard
	// ��ʼ��̨����������
	kakibilliardTex = new Texture("kaki.jpeg");
	kakibilliardTex->Init();
	// ����̨������ʵ��
	kakibilliard = new Cube(shader, kakibilliardTex, kakibilliardTex);
	kakibilliard->Init();
	// ����̨���������ű���
	billiard->SetScale(0.2f, 0.2f, 0.2f);

	//sofa
	//// dudukan dan sandaransofa
	// ��ʼ��ɳ�����κͿ�������
	sofaTex = new Texture("sofa.jpeg");
	sofaTex->Init();
	// ����ɳ�����κͿ���ʵ��
	sofa = new Cube(shader, sofaTex, sofaTex);
	sofa->Init();
	// ����ɳ�����κͿ������ű���
	sofa->SetScale(0.2f, 0.2f, 0.2f);

	////kaki sofa
	// ��ʼ��ɳ��������
	kakisofaTex = new Texture("kaki.jpeg");
	kakisofaTex->Init();
	// ����ɳ����ʵ��
	kakisofa = new Cube(shader, kakisofaTex, kakisofaTex);
	kakisofa->Init();
	// ����ɳ�������ű���
	kakisofa->SetScale(0.2f, 0.2f, 0.2f);

	//pintu
	// ��ʼ��������
	pintuTex = new Texture("pintu.jpeg");
	pintuTex->Init();
	// ������ʵ��
	pintu = new Cube(shader, pintuTex, pintuTex);
	pintu->Init();
	// ���������ű���
	pintu->SetScale(0.2f, 0.2f, 0.2f);

	// vending machine
	// ��ʼ���Զ��ۻ�������
	vendingmachineTex = new Texture("vendingmachine.jpeg");
	vendingmachineTex->Init();
	// �����Զ��ۻ���ʵ��
	vendingmachine = new Cube(shader, vendingmachineTex, vendingmachineTex);
	vendingmachine->Init();
	// �����Զ��ۻ������ű���
	vendingmachine->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ���Զ��ۻ�����������
	kotakvendingmachineTex = new Texture("kotakvendingmachine.png");
	kotakvendingmachineTex->Init();
	// �����Զ��ۻ�������ʵ��
	kotakvendingmachine = new Cube(shader, kotakvendingmachineTex, kotakvendingmachineTex);
	kotakvendingmachine->Init();
	// �����Զ��ۻ����������ű���
	kotakvendingmachine->SetScale(0.2f, 0.2f, 0.2f);

	//resepsionis
	// ��ʼ���Ӵ�̨����
	resepsionisTex = new Texture("resepsionis.png");
	resepsionisTex->Init();
	// �����Ӵ�̨ʵ��
	resepsionis = new Cube(shader, resepsionisTex, resepsionisTex);
	resepsionis->Init();
	// ���ýӴ�̨���ű���
	resepsionis->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ���Ӵ�̨��������
	mejaresepsionisTex = new Texture("marmer.png");
	mejaresepsionisTex->Init();
	// �����Ӵ�̨����ʵ��
	mejaresepsionis = new Cube(shader, mejaresepsionisTex, mejaresepsionisTex);
	mejaresepsionis->Init();
	// ���ýӴ�̨�������ű���
	mejaresepsionis->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ���Ӵ�̨��������
	engselTex = new Texture("kaki.jpeg");
	engselTex->Init();
	// �����Ӵ�̨����ʵ��
	engsel = new Cube(shader, engselTex, engselTex);
	engsel->Init();
	// ���ýӴ�̨�������ű���
	engsel->SetScale(0.2f, 0.2f, 0.2f);

	//monitor
	// ��ʼ����ʾ����������
	monitorTex = new Texture("kaki.jpeg");
	monitorTex->Init();
	// ������ʾ������ʵ��
	monitor = new Cube(shader, monitorTex, monitorTex);
	monitor->Init();
	// ������ʾ���������ű���
	monitor->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ����ʾ����Ļ����
	lcdmonitorTex = new Texture("lcd.png");
	lcdmonitorTex->Init();
	// ������ʾ����Ļʵ��
	lcdmonitor = new Cube(shader, lcdmonitorTex, lcdmonitorTex);
	lcdmonitor->Init();
	// ������ʾ����Ļ���ű���
	lcdmonitor->SetScale(0.2f, 0.2f, 0.2f);

	//keyboard
	// ��ʼ����������
	keyboardTex = new Texture("keyboard.png");
	keyboardTex->Init();
	// ��������ʵ��
	keyboard = new Cube(shader, keyboardTex, keyboardTex);
	keyboard->Init();
	// ���ü������ű���
	keyboard->SetScale(0.2f, 0.2f, 0.2f);

	//mouse
	// ��ʼ���������
	mouseTex = new Texture("mouse.png");
	mouseTex->Init();
	// �������ʵ��
	mouse = new Cube(shader, mouseTex, mouseTex);
	mouse->Init();
	// ����������ű���
	mouse->SetScale(0.2f, 0.2f, 0.2f);


	//lukisan 1 dan 2
	// ��ʼ����һ����������
	lukisan1Tex = new Texture("lukisan1.png");
	lukisan1Tex->Init();
	// ������һ����ʵ��
	lukisan1 = new Cube(shader, lukisan1Tex, lukisan1Tex);
	lukisan1->Init();
	// ���õ�һ���������ű���
	lukisan1->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ���ڶ�����������
	lukisan2Tex = new Texture("lukisan2.png");
	lukisan2Tex->Init();
	// �����ڶ�����ʵ��
	lukisan2 = new Cube(shader, lukisan2Tex, lukisan2Tex);
	lukisan2->Init();
	// ���õڶ����������ű���
	lukisan2->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ����������������
	lukisan3Tex = new Texture("lukisan3.png");
	lukisan3Tex->Init();
	// ������������ʵ��
	lukisan3 = new Cube(shader, lukisan3Tex, lukisan3Tex);
	lukisan3->Init();
	// ���õ������������ű���
	lukisan3->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ�����ķ���������
	lukisan4Tex = new Texture("lukisan4.png");
	lukisan4Tex->Init();
	// �������ķ���ʵ��
	lukisan4 = new Cube(shader, lukisan4Tex, lukisan4Tex);
	lukisan4->Init();
	// ���õ��ķ��������ű���
	lukisan4->SetScale(0.2f, 0.2f, 0.2f);

	//jam
	// ��ʼ��ʱ������
	jamTex = new Texture("jam.jpeg");
	jamTex->Init();
	// ����ʱ��ʵ��
	jam = new Cube(shader, jamTex, jamTex);
	jam->Init();
	// ����ʱ�����ű���
	jam->SetScale(0.2f, 0.2f, 0.2f);

	//AC
	// ��ʼ���յ�����
	acTex = new Texture("AC.png");
	acTex->Init();
	// �����յ�ʵ��
	ac = new Cube(shader, acTex, acTex);
	ac->Init();
	// ���ÿյ����ű���
	ac->SetScale(0.2f, 0.2f, 0.2f);

	// ��ʼ���յ�ǰ�������
	depanacTex = new Texture("depanAC.png");
	depanacTex->Init();
	// �����յ�ǰ���ʵ��
	depanac = new Cube(shader, depanacTex, depanacTex);
	depanac->Init();
	// ���ÿյ�ǰ������ű���
	depanac->SetScale(0.2f, 0.2f, 0.2f);

	//nomor meja
	////1
	// ��ʼ������1������
	nomor1Tex = new Texture("1.jpeg");
	nomor1Tex->Init();
	// ��������1ʵ��
	nomor1 = new Cube(shader, nomor1Tex, nomor1Tex);
	nomor1->Init();
	// ��������1�����ű���
	nomor1->SetScale(0.2f, 0.2f, 0.2f);
	////2
	// ��ʼ������2������
	nomor2Tex = new Texture("2.jpeg");
	nomor2Tex->Init();
	// ��������2ʵ��
	nomor2 = new Cube(shader, nomor2Tex, nomor2Tex);
	nomor2->Init();
	// ��������2�����ű���
	nomor2->SetScale(0.2f, 0.2f, 0.2f);
	////3
	// ��ʼ������3������
	nomor3Tex = new Texture("3.jpeg");
	nomor3Tex->Init();
	// ��������3ʵ��
	nomor3 = new Cube(shader, nomor3Tex, nomor3Tex);
	nomor3->Init();
	// ��������3�����ű���
	nomor3->SetScale(0.2f, 0.2f, 0.2f);
	////4
	// ��ʼ������4������
	nomor4Tex = new Texture("4.jpeg");
	nomor4Tex->Init();
	// ��������4ʵ��
	nomor4 = new Cube(shader, nomor4Tex, nomor4Tex);
	nomor4->Init();
	// ��������4�����ű���
	nomor4->SetScale(0.2f, 0.2f, 0.2f);

	//lampu
	// ��ʼ���Ƶ�����
	lampuTex = new Texture("lampu.png");
	lampuTex->Init();
	// ������ʵ��
	lampu = new Cube(shader, lampuTex, lampuTex);
	lampu->Init();
	// ���õƵ����ű���
	lampu->SetScale(0.2f, 0.2f, 0.2f);

	////standlampu
	// ��ʼ���Ƽܵ�����
	standlampuTex = new Texture("standlampu.png");
	standlampuTex->Init();
	// �����Ƽ�ʵ��
	standlampu = new Cube(shader, standlampuTex, standlampuTex);
	standlampu->Init();
	// ���õƼܵ����ű���
	standlampu->SetScale(0.2f, 0.2f, 0.2f);

	/*Ruangan*/
	////dinding
	// ��ʼ��ǽ������
	dindingTex = new Texture("dinding.png");
	dinding = new Cube(shader, dindingTex, dindingTex);
	dinding->Init();
	// ����ǽ�����ű���
	dinding->SetScale(0.2f, 0.2f, 0.2f);
	////atap 
	// ��ʼ���컨������
	atapTex = new Texture("atap.png");
	atap = new Cube(shader, atapTex, atapTex);
	atap->Init();
	// �����컨�����ű���
	atap->SetScale(0.2f, 0.2f, 0.2f);


}
///////////////////////////////////////////////////////////////////////////////////////////////

// ��Դ�ͷ�
void Application::DeInit()
{
	/*objek*/
	// �ͷ����������
	delete cube;
	// �ͷ�̨��������
	delete billiard, mejabilliard, kakibilliard;
	// �ͷ�ɳ������
	delete sofa, kakisofa;
	// �ͷ��Ŷ���
	delete pintu;
	// �ͷ��Զ��ۻ�������
	delete vendingmachine, kotakvendingmachine;
	// �ͷŽӴ�̨����
	delete resepsionis, mejaresepsionis, engsel;
	// �ͷŻ�������
	delete lukisan1, lukisan2, lukisan3, lukisan4;
	// �ͷ�ʱ�Ӷ���
	delete jam;
	// �ͷ����Ŷ���
	delete nomor1, nomor2, nomor3, nomor4;
	// �ͷſյ�����
	delete ac, depanac;
	// �ͷ���ʾ������
	delete monitor, lcdmonitor, keyboard, mouse;
	// �ͷŵƶ���
	delete lampu, standlampu;

	/*ruangan*/
	// �ͷ�ǽ�ں��컨�����
	delete dinding, atap;

	/*Plane*/
	// ɾ�������������
	glDeleteVertexArrays(1, &VAO);
	// ɾ�����㻺�����
	glDeleteBuffers(1, &VBO);
	// ɾ��Ԫ�ػ������
	glDeleteBuffers(1, &EBO);
}

void Application::Update(double deltaTime)
{
	angle += (float)((deltaTime * 1.5f) / 1000);
	cube->SetRotation(angle, 0, 1, 0);
}

// ��Ⱦ����
void Application::Render()
{
	// �����ӿ�
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	// �����ɫ�������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ���������ɫΪ��ɫ
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ���ö����ģʽΪ���ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// ����͸��ͶӰ����
	setupPerspective();

	// ���Ʋ�ɫƽ��
	DrawColoredPlane();

	////////////////////////////////////////////////////////////////////////////////////////////////

	/*objek*/

	// ̨����
	//// ̨���� 1 (x=0.z=0)
	billiard->SetPosition(-0.105, -6555.0, -1.05);
	billiard->SetScale(6.5f, 0.0001f, 4.5f);
	billiard->Draw();
	//// ̨�����ײ� 1
	kakibilliard->SetPosition(-0.105, -2.6, -1.32);
	kakibilliard->SetScale(5.63f, 0.5f, 3.63f);
	kakibilliard->Draw();
	//// ̨�������� 1
	mejabilliard->SetPosition(-0.105, -2.14, -1.05);
	mejabilliard->SetScale(6.5f, 0.4f, 4.5f);
	mejabilliard->Draw();
	//// ̨������ 1
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
	//// ̨���� 2 (z)
	billiard->SetPosition(-0.105, -6555.0, -3.6);
	billiard->SetScale(6.5f, 0.0001f, 4.5f);
	billiard->Draw();
	//// ̨�����ײ� 2
	kakibilliard->SetPosition(-0.105, -2.6, -4.46);
	kakibilliard->SetScale(5.63f, 0.5f, 3.63f);
	kakibilliard->Draw();
	//// ̨�������� 2
	mejabilliard->SetPosition(-0.105, -2.14, -3.6);
	mejabilliard->SetScale(6.5f, 0.4f, 4.5f);
	mejabilliard->Draw();
	//// ̨������ 2
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
	//// ̨���� 3 (x)
	billiard->SetPosition(1.932, -6555.0, -1.05);
	billiard->SetScale(6.5f, 0.0001f, 4.5f);
	billiard->Draw();
	//// ̨�����ײ� 3
	kakibilliard->SetPosition(2.237, -2.6, -1.32);
	kakibilliard->SetScale(5.63f, 0.5f, 3.63f);
	kakibilliard->Draw();
	//// ̨�������� 3
	mejabilliard->SetPosition(1.932, -2.14, -1.05);
	mejabilliard->SetScale(6.5f, 0.4f, 4.5f);
	mejabilliard->Draw();
	//// ̨������ 3
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
	//// ̨���� 4 (x(̨����3),z(̨����2))
	billiard->SetPosition(1.932, -6555.0, -3.6);
	billiard->SetScale(6.5f, 0.0001f, 4.5f);
	billiard->Draw();
	//// ̨�����ײ� 4
	kakibilliard->SetPosition(2.237, -2.6, -4.46);
	kakibilliard->SetScale(5.63f, 0.5f, 3.63f);
	kakibilliard->Draw();
	//// ̨�������� 4
	mejabilliard->SetPosition(1.932, -2.14, -3.6);
	mejabilliard->SetScale(6.5f, 0.4f, 4.5f);
	mejabilliard->Draw();
	//// ̨������ 4
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

    // ɳ��
    //// ɳ������ 1
    sofa->SetPosition(-0.125, -2.9, -11.3);
    sofa->SetScale(5.0f, 0.5f, 2.0f);
    sofa->Draw();
    //// ɳ������ 1
    sofa->SetPosition(-0.125, -0.35, -46.71);
    sofa->SetScale(5.0f, 2.0f, 0.5f);
    sofa->Draw();
    //// ɳ������ 1
    kakisofa->SetPosition(-6.75, -0.75, -11.3);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// ɳ������ 1
    kakisofa->SetPosition(4.25, -0.75, -11.3);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// ɳ������ 2
    sofa->SetPosition(2.525, -2.9, -11.3);
    sofa->SetScale(5.0f, 0.5f, 2.0f);
    sofa->Draw();
    //// ɳ������ 2
    sofa->SetPosition(2.525, -0.35, -46.71);
    sofa->SetScale(5.0f, 2.0f, 0.5f);
    sofa->Draw();
    //// ɳ������ 2
    kakisofa->SetPosition(19.75, -0.75, -11.3);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// ɳ������ 2
    kakisofa->SetPosition(30.75, -0.75, -11.3);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// ɳ������ 3
    sofa->SetPosition(-0.125, -2.9, 0.8);
    sofa->SetScale(5.0f, 0.5f, 2.0f);
    sofa->Draw();
    //// ɳ������ 3
    sofa->SetPosition(-0.125, -0.35, 4.71);
    sofa->SetScale(5.0f, 2.0f, 0.5f);
    sofa->Draw();
    //// ɳ������ 3
    kakisofa->SetPosition(-6.75, -0.75, 0.8);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// ɳ������ 3
    kakisofa->SetPosition(4.25, -0.75, 0.8);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// ɳ������ 4
    sofa->SetPosition(2.525, -2.9, 0.8);
    sofa->SetScale(5.0f, 0.5f, 2.0f);
    sofa->Draw();
    //// ɳ������ 4
    sofa->SetPosition(2.525, -0.35, 4.71);
    sofa->SetScale(5.0f, 2.0f, 0.5f);
    sofa->Draw();
    //// ɳ������ 4
    kakisofa->SetPosition(19.75, -0.75, 0.8);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();
    //// ɳ������ 4
    kakisofa->SetPosition(30.75, -0.75, 0.8);
    kakisofa->SetScale(0.5f, 2.0f, 2.0f);
    kakisofa->Draw();

    // ��
    pintu->SetPosition(-44.7, 0.075, -2.120);
    pintu->SetScale(0.2f, 6.0f, 5.0f);
    pintu->Draw();

    // �Զ��ۻ���
    //// �ۻ�������
    kotakvendingmachine->SetPosition(1.5, 0.035, -6.55);
    kotakvendingmachine->SetScale(4.0f, 5.5, 3.6f);
    kotakvendingmachine->Draw();
    //// �ۻ���ǰ���
    vendingmachine->SetPosition(1.5, 0.035, -21779.55);
    vendingmachine->SetScale(4.0f, 5.5, 0.001f);
    vendingmachine->Draw();

    // �Ӵ�̨
    //// �Ӵ�̨�ײ�
    resepsionis->SetPosition(35.0, -0.5, -2.85);
    resepsionis->SetScale(0.5f, 2.5f, 3.7f);
    resepsionis->Draw();

    resepsionis->SetPosition(5.26, -0.5, -24.3);
    resepsionis->SetScale(3.625f, 2.5f, 0.5f);
    resepsionis->Draw();

    resepsionis->SetPosition(9.58, -0.5, -17.85);
    resepsionis->SetScale(1.9f, 2.5f, 0.5f);
    resepsionis->Draw();

    //// �Ӵ�̨����
    mejaresepsionis->SetPosition(23.15, 0.5, -3.9);
    mejaresepsionis->SetScale(0.75f, 0.25f, 2.7f);
    mejaresepsionis->Draw();

    mejaresepsionis->SetPosition(4.855, 0.5, -16.35);
    mejaresepsionis->SetScale(3.9f, 0.25f, 0.75f);
    mejaresepsionis->Draw();

    mejaresepsionis->SetPosition(8.59, 0.5, -11.74);
    mejaresepsionis->SetScale(2.1f, 0.25f, 0.75f);
    mejaresepsionis->Draw();

    //// �Ӵ�̨
    mejaresepsionis->SetPosition(24.15, -0.0, -3.9);
    mejaresepsionis->SetScale(0.75f, 0.25f, 2.7f);
    mejaresepsionis->Draw();

    //// �Ӵ�̨С��
    resepsionis->SetPosition(14.75, -0.5, -89.55);
    resepsionis->SetScale(1.35f, 1.75f, 0.1f);
    resepsionis->Draw();

    //// �Ӵ�̨�Ž���
    engsel->SetPosition(96.0, -4.0, -89.5);
    engsel->SetScale(0.2f, 0.1f, 0.1f);
    engsel->Draw();

    engsel->SetPosition(96.0, -14.0, -89.5);
    engsel->SetScale(0.2f, 0.1f, 0.1f);
    engsel->Draw();

    // ʱ��
    jam->SetPosition(20840.55, 5.0, -10.5);
    jam->SetScale(0.001f, 1.0f, 1.0f);
    jam->Draw();

    // ����
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

    // ����
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

    // ��ʾ��
    //// ��ʾ������
    monitor->SetPosition(34.8, 5.5, -21);
    monitor->SetScale(0.5f, 0.05f, 0.5f);
    monitor->Draw();
    //// ��ʾ��֧��
    monitor->SetPosition(116, 0.4, -35);
    monitor->SetScale(0.15f, 1.0f, 0.3f);
    monitor->Draw();
    //// ��ʾ����Ļ
    monitor->SetPosition(117, 1.0, -7);
    monitor->SetScale(0.15f, 1.0f, 1.5f);
    monitor->Draw();
    //// ��ʾ��LCD
    lcdmonitor->SetPosition(17630, 1.25, -8.075);
    lcdmonitor->SetScale(0.001f, 0.8f, 1.3f);
    lcdmonitor->Draw();

    // ����
    keyboard->SetPosition(36.3, 3.0, -10.25);
    keyboard->SetScale(0.5f, 0.05f, 1.0f);
    keyboard->Draw();

    // ���
    mouse->SetPosition(60.5, 3.0, -55.25);
    mouse->SetScale(0.3f, 0.05f, 0.2f);
    mouse->Draw();

    // ��
    //// ���� 1
    standlampu->SetPosition(-4.0, -9.6, -13.5);
    standlampu->SetScale(1.75f, 0.25f, 1.75f);
    standlampu->Draw();
    //// �Ƹ� 1
    standlampu->SetPosition(-28, -0.1, -94.5);
    standlampu->SetScale(0.25f, 3.75f, 0.25f);
    standlampu->Draw();
    //// �� 1
    lampu->SetPosition(-4.7, 1.5, -15.75);
    lampu->SetScale(1.5f, 1.5f, 1.5f);
    lampu->Draw();

    //// ���� 2
    standlampu->SetPosition(10.925, -9.6, -13.5);
    standlampu->SetScale(1.75f, 0.25f, 1.75f);
    standlampu->Draw();
    //// �Ƹ� 2
    standlampu->SetPosition(76.5, -0.1, -94.5);
    standlampu->SetScale(0.25f, 3.75f, 0.25f);
    standlampu->Draw();
    //// �� 2
    lampu->SetPosition(12.75, 1.5, -15.75);
    lampu->SetScale(1.5f, 1.5f, 1.5f);
    lampu->Draw();

    //// ���� 3
    standlampu->SetPosition(10.925, -9.6, 1.45);
    standlampu->SetScale(1.75f, 0.25f, 1.75f);
    standlampu->Draw();
    //// �Ƹ� 3
    standlampu->SetPosition(76.5, -0.1, 10.25);
    standlampu->SetScale(0.25f, 3.75f, 0.25f);
    standlampu->Draw();
    //// �� 3
    lampu->SetPosition(12.75, 1.5, 1.7);
    lampu->SetScale(1.5f, 1.5f, 1.5f);
    lampu->Draw();

    //// ���� 4
    standlampu->SetPosition(-4.0, -9.6, 1.45);
    standlampu->SetScale(1.75f, 0.25f, 1.75f);
    standlampu->Draw();
    //// �Ƹ� 4
    standlampu->SetPosition(-28, -0.1, 10.25);
    standlampu->SetScale(0.25f, 3.75f, 0.25f);
    standlampu->Draw();
    //// �� 4
    lampu->SetPosition(-4.7, 1.5, 1.7);
    lampu->SetScale(1.5f, 1.5f, 1.5f);
    lampu->Draw();

    // �յ�
    ac->SetPosition(-13.65, 5.8, -2.820);
    ac->SetScale(0.625f, 0.75f, 3.75f);
    ac->Draw();
    //// �յ�ǰ���
    depanac->SetPosition(-8215, 5.8, -2.820);
    depanac->SetScale(0.001f, 0.75f, 3.75f);
    depanac->Draw();

	// ��������ǽ��
	for (int i = 0; i < 4; i++) {
		if (i > 1) {
			// ����ǽ��λ�ú����ű���
			dinding->SetPosition(70 - (i - 2) * 100, 0.275, -0.35);
			dinding->SetScale(0.30f, 11.75f, 30.0f);
			// ����ǽ��
			dinding->Draw();
		}
		else {
			// ����ǽ��λ�ú����ű���
			dinding->SetPosition(0.2, 0.275, 15 - i * 100);
			dinding->SetScale(30.0f, 11.75f, 0.30f);
			// ����ǽ��
			dinding->Draw();
		}
	}

	// �����컨��λ�ú����ű���
	atap->SetPosition(0.2, 30, -0.35);
	atap->SetScale(30.0f, 0.30f, 30.0f);
	// �����컨��
	atap->Draw();

	///////////////////////////////////////////////////////////////////////////////////////////////

	// ������Ȳ���
	glDisable(GL_DEPTH_TEST);

}


//////////////////////////////////////////////

//int game(int argc, char* argv[])
//{
//	// initialise GLUT
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);//��ʾģʽ
//	glutInitWindowSize(1280, 720);//���ڴ�С	
//	glutCreateWindow("Billiard Ball game"); //��������
//
//	// register callbacks ע��һЩ�лص�����
//	glutKeyboardFunc(keyboard);//���̹���
//	glutSpecialFunc(special);//��Ӧ���̷�����Ƽ�
//	glutMouseFunc(mouse);//��깦��
//	glutPassiveMotionFunc(passive);//��Ӧ���û�б�����ȥʱ���Ƶ���������
//	glutDisplayFunc(display);//��ʾ��ǰ����
//	glutTimerFunc(10, timer, 1);
//	quadricObject = gluNewQuadric();
//	glutMainLoop();//��������¼�����ѭ�����ú���������main�����������һ����䡣
//	return 0;
//}

/////////////////////////////

/////////////////////////////////////////////////////


Table table;
int value = 2;
int timers;
double speed = 1;
double cameraAngle1 = -90;//ƽ��Ƕ�
double cameraAngle2 = 25;//�����Ƕ�

// handle key presses
void keyboardEvent(unsigned char key, int x, int y)//���̼����¼�
{
	switch (key)
	{
	case ' ':
		// reset game on Space key
		table.reset();
		break;
	case 50:
		// ����glut��ѭ��
		glutDestroyWindow(glutGetWindow());
		break;
	}
}

// handle special key presses
void special(int key, int x, int y)//�������ҷ��������������Ƕȣ�ʵ��λ�ñ任
{
	// rotate the camera
	switch (key)
	{
	case GLUT_KEY_LEFT:
		cameraAngle1 += 5;
		break;
	case GLUT_KEY_RIGHT:
		cameraAngle1 -= 5;
		break;
	case GLUT_KEY_UP:
		cameraAngle2 += 5;
		break;
	case GLUT_KEY_DOWN:
		cameraAngle2 -= 5;
	}

	// limit the angle
	if (cameraAngle2 < 10) cameraAngle2 = 10;//��ֹ�굽���׺ͳ����컨��
	if (cameraAngle2 > 80) cameraAngle2 = 80;
}
void selectMyFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}
void drawTextString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
		// Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

//void paintSpeed(double speed) {
//	glPushMatrix();
//	selectMyFont(24, ANSI_CHARSET, "Comic Sans MS");
//	//glClear(GL_COLOR_BUFFER_BIT);
//	//glColor3f(0.5f, 0.0f, 0.0f);
//	//glRasterPos3f(-4.5f, 0.2f, 1.0f);// �߶�
//	//glTranslated(-3, 0, 0);
//	char str1[100] = "Speed:";
//	char temp1[1000];
//	int percent = (int)(speed / 18 * 100);
//	printf("%d\n", percent);
//	sprintf_s(temp1, "%d", percent);
//	strcat_s(str1, temp1);
//	//glTranslated(0,0, 0);
//	drawTextString(str1);
//	glPopMatrix();
//
//}
void timer(int timerid)
{
	if (timers == 1 && value == 1)
	{
		speed += 0.3;
		//paintSpeed(speed);
	}
	else if (timers == 2 && value == 1)
	{
		if (!table.moving()) table.shoot();
		speed = 1;
	}

	if (timers == 1) value = 1;
	else if (timers == 2) value = 2;

	glutTimerFunc(10, timer, 1);//10 ms 
}

// handle mouse clicks
void mouseEvent(int button, int state, int x, int y)//�������¼�
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //����������������Ұ���,��ʼ��ʱ
	{
		timers = 1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		timers = 2;
		shootsound();
	}
}

// handle mouse motion
void passive(int x, int y)//����ƶ���û�а��£����Ƹ˵��ƶ�
{
	// adjust stick angle by left/right mouse movement
	int centre = glutGet(GLUT_WINDOW_WIDTH) / 2;
	table.setStickAngle((x - centre) * 0.2 - cameraAngle1);
}

// draw scene
void display(void)//��ʾ����
{
	// update the game to the current time
	int currentTime = glutGet(GLUT_ELAPSED_TIME);//�������ÿ���֡��
	table.update(currentTime);

	// clear window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// �����ɫ����������Ȼ�����
	glEnable(GL_DEPTH_TEST);//����֮��,����ʱ��飬��ǰ����ǰ���Ƿ��б�����أ����������ص��������������Ͳ�����ƣ�Ҳ����˵��OpenGL��ֻ������ǰ���һ�㡣

	// window sizes
	int width = glutGet(GLUT_WINDOW_WIDTH);//�õ����ڲ�������
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	// set perspective projection
	glMatrixMode(GL_PROJECTION); //����ǰ����ָ��ΪͶӰ����
	glLoadIdentity(); //Ȼ��Ѿ�����Ϊ��λ����
	//?????????????????????????????
	double fieldOfView = 50.0;  //ԽС�����ķ�ΧԽС
	double aspectRatio = (double)width / (double)height;//ʵ�ʴ��ڵ��ݺ��
	double nearPlane = 0.1;//�����Ĳ���
	double farPlane = 100.0;//Զ���Ĳ���
	gluPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);

	// set camera
	glMatrixMode(GL_MODELVIEW);//ģ���Ӿ�����
	glLoadIdentity();
	glTranslated(0, 0, -8); //��������(x, y, z)
	glRotated(cameraAngle2, 1, 0, 0);//��������Ƕȵ�������λ�� //ʹͼ����X����ת
	glRotated(cameraAngle1, 0, 1, 0);//ʹͼ����Y����ת

	// set light colour and position
	glEnable(GL_LIGHTING);//���ù���
	glEnable(GL_LIGHT0);//0�Ź���
	glEnable(GL_LIGHT1);//1�Ź���
	//���һ������Ϊ0��˵���Ƿ����Թ�Դ����0��Ϊλ���Թ�Դ ����̨�ƣ�����˥���� 
	GLfloat lightAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f }; //���ù�Դ�Ļ��������ԣ�Ĭ��ֵ(0, 0, 0, 1)��
	GLfloat lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//���ù�Դ��ɢ������ԣ�Ĭ��ֵ(1,1,1,1)
	GLfloat lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //���ù�Դ�ľ��淴������ԣ�Ĭ��ֵ(1, 1, 1, 1)
	GLfloat lightPosition[4] = { 0.0f, 4.0f, 1.0f, 1.0f };//���ù�Դ��λ�ã�Ĭ��ֵ(0,0,1,0)
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// normalise normals automatically
	glEnable(GL_NORMALIZE);//�Զ���λ��������

	// cause material colours to track current colour
	glEnable(GL_COLOR_MATERIAL);// ʹ����ɫ���� ������һ�л��ɻҰ�
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//������ʹ���ʵ�����ɫ�͵�ǰ��ɫ(��glColor*()����)һ�¡�
	//face����������GL_FRONT, GL_BACK, ��GL_FRONT_AND_BACK(ȱʡֵ)��
	//mode����������GL_AMBIENT, GL_DIFFUSE, GL_AMBIENT_AND_DIFFUSE(ȱʡֵ),
	//GL_SPECULAR, �� GL_EMISSION��face�����ĸ�����Щ����Ĳ�������ɫ��Ҫ
	//�ı�Ϊ��ǰ��ɫ��mode������Щ��������ɫ��Ҫ�ı䡣

	table.draw();

	// refresh window
	glutSwapBuffers();//��������������ָ��
	glutPostRedisplay();//�ػ�ͼ�񣬷���ͼ��ֻ����Ӧ���������Ϣʱ�Ż����ͼ��
	//�������ǽ��и��ӵĻ�ͼ����ʱ���������������Ե���˸������������Ĺؼ���
	//��ʹ���ƵĶ���ͬʱ��������Ļ�ϡ���ν˫���弼���� ��ָʹ�������������� ǰ̨
	//����ͺ�̨���塣ǰ̨���弴���ǿ�������Ļ����̨���������ڴ浱�У���������˵
	//�ǲ��ɼ��ġ�ÿ�ε����л�ͼ�������ں�̨�����н��У� ���������ʱ�� �ѻ��Ƶ�
	//���ս�����Ƶ���Ļ�ϣ� ������ ���ǿ�������GDIԪ��ͬʱ��������Ļ�ϣ��Ӷ���
	//����Ƶ��ˢ�µ��µĻ�����˸���⡣
}



////////////////////////////////////////////////////////////////////////////////////////




// ��������
void Application::ProcessInput(GLFWwindow* window)
{
	// �������ESC�����رմ���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		// �½�һ��glut����
		int argc = 1;
		char* argv[1] = { (char*)"Game" };
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ������ʾģʽ
		glutInitWindowSize(1280, 720); // ���ô��ڴ�С
		glutCreateWindow("Game"); // ��������

		glutKeyboardFunc(keyboardEvent);//���̹���
		glutSpecialFunc(special);//��Ӧ���̷�����Ƽ�
		glutMouseFunc(mouseEvent);//��깦��
		glutPassiveMotionFunc(passive);//��Ӧ���û�б�����ȥʱ���Ƶ���������

		glutDisplayFunc(display);//��ʾ��ǰ����
		glutTimerFunc(10, timer, 1);
		quadricObject = gluNewQuadric();
		
		glutMainLoop();
    }

	// ���������
	// -----------
	// �����������Ҽ��������ӽ�
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	// �����������������С�ӽ�
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// ����������ƶ�
	// -------------
	// �������W������ǰ�ƶ������
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	// �������S��������ƶ������
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	// �������A��������ƽ�������
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	// �������D��������ƽ�������
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// ���������Ctrl���������ƶ������
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		posCamY -= CAMERA_SPEED;
	}

	// ������¿ո���������ƶ������
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		posCamY += CAMERA_SPEED;
	}

	// �����������ת
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// ��ȡ���λ��
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// �������λ��
	glfwSetCursorPos(window, midX, midY);

	// ��ȡ�����ķ������ú���Ĳ����ٶ�
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// ֵԽ�������ת��Խ��
	viewCamY += angleZ * 2;

	// �����������x���ϵ���ת
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);
}

// ������ɫƽ��
void Application::BuildColoredPlane(const char* texturePath)
{
	// ���ز���������
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

	// ����������
	GLfloat vertices[] = {
		// λ�ú���������
		// �ײ�
		-0.5, -38.5, -0.5,  0,  0,
		0.5, -38.5, -0.5, 8,  0,
		0.5, -38.5,  0.5, 8, 8,
		-0.5, -38.5,  0.5,  0, 8,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// ��VAO
	glBindVertexArray(VAO);

	// ��VBO�����ö�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ��EBO��������������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// ���VAO
	glBindVertexArray(0);
}

// ���Ʋ�ɫƽ��
void Application::DrawColoredPlane()
{
	// ��VAO
	glBindVertexArray(VAO);

	// ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// ����Ԫ��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// ���VAO
	glBindVertexArray(0);
}


int main(int argc, char** argv) {
	// ����Applicationʵ��
	Application app = Application();
	// ����Ӧ�ó���
	app.Start("2024 CG Class Design",
		1920, 1080,
		false,
		false);
}