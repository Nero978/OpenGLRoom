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