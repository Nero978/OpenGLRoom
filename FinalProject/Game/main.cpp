#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "ball.h"
#include "table.h"
#include "sound.h"



//int main(int argc, char* argv[])
//{
//	// initialise GLUT
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);//显示模式
//	glutInitWindowSize(1280, 720);//窗口大小	
//	glutCreateWindow("Billiard Ball game"); //窗口名字
//
//	// register callbacks 注册一些列回调函数
//	glutKeyboardFunc(keyboard);//键盘功能
//	glutSpecialFunc(special);//响应键盘方向控制键
//	glutMouseFunc(mouse);//鼠标功能
//	glutPassiveMotionFunc(passive);//相应鼠标没有被按下去时，移到鼠标的情形
//	glutDisplayFunc(display);//显示当前窗口
//	glutTimerFunc(10, timer, 1);
//	quadricObject = gluNewQuadric();
//	glutMainLoop();//程序进入事件处理循环。该函数必须是main主函数的最后一条语句。
//	return 0;
//}