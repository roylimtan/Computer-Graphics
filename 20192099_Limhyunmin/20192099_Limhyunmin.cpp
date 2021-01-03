#include <GL/freeglut.h>
#include<vector>
#include<stdlib.h>
#include<iostream>
#include<string>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

#define M_PI 3.141592654

GLint x, y, z;

class xPoint3D
{
public:
	float x, y, z, w;
	xPoint3D() { x = y = z = 0; w = 1; };
};

vector<xPoint3D>arPoints;
vector<xPoint3D>arRotPoints;
xPoint3D pt;


GLboolean aaRotAngle = false;
GLboolean bbRotAngle = false;
GLboolean ccRotAngle = false;

float aRotAngle = 30;
float bRotAngle = 60;
float cRotAngle = 90;

float aradian = aRotAngle * (M_PI / 180);
float bradian = bRotAngle * (M_PI / 180);
float cradian = cRotAngle * (M_PI / 180);


GLsizei winWidth = 1000, winHeight = 700;


void init(void) { //배경 색
	glClearColor(0.745098, 0.745098, 0.745098, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-500, 500, -350, 350, -500, 500);
}

void drawAxis() { //축그리기
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINES);    //시작과 끝 점을 포함한 모든 점을 연결
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(500.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-500.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINES);    //시작과 끝 점을 포함한 모든 점을 연결
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 350.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, -350.0, 0.0);
	glEnd();
}


void Drawpoints(GLint button, GLint action, GLint xMouse, GLint yMouse) { //마우스 클릭할때 점 찍기

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{

		x = xMouse - 500;
		y = winHeight - yMouse - 350;
		z = 0;

		pt.x = x;
		pt.y = y;
		pt.z = z;

		arPoints.push_back(pt);

		glPointSize(10.0);
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_POINTS);
		glVertex3f(x, y, z);
		glEnd();

		std::cout << x << "," << y << "," << z << std::endl;


	}
	glFlush();
}

void aRotation(void) { //X축 회전
	xPoint3D newpt;
	if (aaRotAngle == true) //30도
	{
		for (int k = 1; k < (360 / aRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].x;
				newpt.y = arPoints[i].y * cos(aradian * k) - arPoints[i].z * sin(aradian * k);
				newpt.z = arPoints[i].y * sin(aradian * k) + arPoints[i].z * cos(aradian * k);
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}

	else if (bbRotAngle == true) //60도
	{
		for (int k = 1; k < (360 / bRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].x;
				newpt.y = arPoints[i].y * cos(bradian * k) - arPoints[i].z * sin(bradian * k);
				newpt.z = arPoints[i].y * sin(bradian * k) + arPoints[i].z * cos(bradian * k);
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}

	else if (ccRotAngle == true) //90도
	{
		for (int k = 1; k < (360 / cRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].x;
				newpt.y = arPoints[i].y * cos(cradian * k) - arPoints[i].z * sin(cradian * k);
				newpt.z = arPoints[i].y * sin(cradian * k) + arPoints[i].z * cos(cradian * k);
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}
	cout << arPoints.size() << endl;
}

void bRotation(void) { //Y축 회전
	xPoint3D newpt;
	if (aaRotAngle == true) //30도
	{
		for (int k = 1; k < (360 / aRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].z * sin(aradian * k) + arPoints[i].x * cos(aradian * k);
				newpt.y = arPoints[i].y;
				newpt.z = arPoints[i].z * cos(aradian * k) - arPoints[i].x * sin(aradian * k);
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}

	else if (bbRotAngle == true) //60도
	{
		for (int k = 1; k < (360 / bRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].z * sin(bradian * k) + arPoints[i].x * cos(bradian * k);
				newpt.y = arPoints[i].y;
				newpt.z = arPoints[i].z * cos(bradian * k) - arPoints[i].x * sin(bradian * k);
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}

	else if (ccRotAngle == true) //90도
	{
		for (int k = 1; k < (360 / cRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].z * sin(cradian * k) + arPoints[i].x * cos(cradian * k);
				newpt.y = arPoints[i].y;
				newpt.z = arPoints[i].z * cos(cradian * k) - arPoints[i].x * sin(cradian * k);
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}
	cout << arPoints.size() << endl;
}

void cRotation(void) { //z축 회전
	xPoint3D newpt;
	if (aaRotAngle == true) //30도
	{
		for (int k = 1; k < (360 / aRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].x * cos(aradian * k) - arPoints[i].y * sin(aradian * k);
				newpt.y = arPoints[i].x * sin(aradian * k) + arPoints[i].y * cos(aradian * k);
				newpt.z = arPoints[i].z;
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}

	else if (bbRotAngle == true) //60도
	{
		for (int k = 1; k < (360 / bRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].x * cos(bradian * k) - arPoints[i].y * sin(bradian * k);
				newpt.y = arPoints[i].x * sin(bradian * k) + arPoints[i].y * cos(bradian * k);
				newpt.z = arPoints[i].z;
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}

	else if (ccRotAngle == true) //90도
	{
		for (int k = 1; k < (360 / cRotAngle); k++) {
			for (int i = 0; i < arPoints.size(); i++) {
				xPoint3D newpt;
				newpt.x = arPoints[i].x * cos(cradian * k) - arPoints[i].y * sin(cradian * k);
				newpt.y = arPoints[i].x * sin(cradian * k) + arPoints[i].y * cos(cradian * k);
				newpt.z = arPoints[i].z;
				arRotPoints.push_back(newpt);

				glPointSize(10.0);
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(newpt.x, newpt.y, newpt.z);
				glEnd();
				std::cout << x << "," << y << "," << z << std::endl;
			}
		}
	}
	cout << arPoints.size() << endl;
}

void displayFcn() {
	glClearColor(0.745098, 0.745098, 0.745098, 1);

	drawAxis();

	glFlush();
}


void MyMainMenu(int entryID) {// 프로그램 종료
	if (entryID == 1)
		exit(0);


	glutPostRedisplay();
}

void MySubMenu_X(int entryID) { //X축 메뉴
	if (entryID == 1) {
		aaRotAngle = true;
		aRotation();
	}
	else if (entryID == 2) {
		bbRotAngle = true;
		aRotation();
	}

	else if (entryID == 3) {
		ccRotAngle = true;
		aRotation();
	}

	glutPostRedisplay();
}

void MySubMenu_Y(int entryID) {  //Y축 메뉴

	if (entryID == 1) {
		aaRotAngle = true;
		bRotation();
	}
	else if (entryID == 2) {
		bbRotAngle = true;
		bRotation();
	}
	else if (entryID == 3) {
		ccRotAngle = true;
		bRotation();
	}
	glutPostRedisplay();
}

void MySubMenu_Z(int entryID) {  //Z축 메뉴

	if (entryID == 1) {
		aaRotAngle = true;
		cRotation();
	}
	else if (entryID == 2) {
		bbRotAngle = true;
		cRotation();
	}
	else if (entryID == 3) {
		ccRotAngle = true;
		cRotation();
	}
	glutPostRedisplay();
}

typedef struct {
	float x;
	float y;
	float z;
} Point;

typedef struct {
	unsigned int ip[3];
} Face;

int pnum;
int fnum;
Point* mpoint = NULL;
Face* mface = NULL;


void SaveModel()
{
	FILE* fout;
	fopen_s(&fout, "c:\\data\\myModel.dat", "w");
	fprintf(fout, "VERTEX = %d\n", pnum);
	for (int i = 0; i < pnum; i++)
	{
		fprintf(fout, "%.1f %.1f %.1f\n", arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
	}

}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("20192099_Limhyunmin");

	init();
	glutDisplayFunc(displayFcn);
	glutMouseFunc(Drawpoints);

	GLint MySubMenuID_X = glutCreateMenu(MySubMenu_X);
	glutAddMenuEntry("30", 1);
	glutAddMenuEntry("60", 2);
	glutAddMenuEntry("90", 3);

	GLint MySubMenuID_Y = glutCreateMenu(MySubMenu_Y);
	glutAddMenuEntry("30", 1);
	glutAddMenuEntry("60", 2);
	glutAddMenuEntry("90", 3);

	GLint MySubMenuID_Z = glutCreateMenu(MySubMenu_Z);
	glutAddMenuEntry("30", 1);
	glutAddMenuEntry("60", 2);
	glutAddMenuEntry("90", 3);

	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("X", MySubMenuID_X);
	glutAddSubMenu("Y", MySubMenuID_Y);
	glutAddSubMenu("Z", MySubMenuID_Z);
	glutAddMenuEntry("Exit", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}