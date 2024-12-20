#pragma once

#include "RenderEngine.h"
#include "Cube.h"
#include "Shader.h"
#include "Texture.h"


class Application :
    public RenderEngine
{
public:
    Application();
    ~Application();

private:
	glm::vec3 viewPos;
	Shader* shader, *shader2;
	GLuint shaderProgram, VBO, VAO, EBO, texture;
	
	Texture 
		//objek
		*cubeDiffTex, *cubeSpecTex, 
		*billiardTex, *mejabilliardTex,*kakibilliardTex,
		*sofaTex, *kakisofaTex, 
		*pintuTex,
		*vendingmachineTex, *kotakvendingmachineTex,
		*resepsionisTex, *mejaresepsionisTex, *engselTex,
		*lukisan1Tex,*lukisan2Tex, *lukisan3Tex, * lukisan4Tex,
		*jamTex,
		*nomor1Tex, *nomor2Tex, *nomor3Tex, *nomor4Tex,
		*acTex, *depanacTex,
		*monitorTex, *lcdmonitorTex, *keyboardTex, *mouseTex,
		*lampuTex, *standlampuTex,

		//Ruangan
		*dindingTex, *atapTex;

	Cube 
		//objek
		*cube, 
		*billiard, *mejabilliard, *kakibilliard,
		*sofa, *kakisofa, 
		*pintu,
		*vendingmachine, * kotakvendingmachine, 
		*resepsionis,*mejaresepsionis, *engsel,
		*lukisan1, *lukisan2, *lukisan3, *lukisan4,
		*jam,
		*nomor1, *nomor2, *nomor3, *nomor4,
		*ac,*depanac,
		*monitor, *lcdmonitor, *keyboard, *mouse,
		*lampu, *standlampu,

		//Ruangan
		*dinding, *atap;

	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	float angle = 0;

	void BuildColoredPlane(const char* texturePath);
	void DrawColoredPlane();

	void setupPerspective();
	void setupLighting(glm::vec3 lightPosition);

	void InitCamera();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);

	virtual void Init();
	virtual void DeInit(); 
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow* window);
};

