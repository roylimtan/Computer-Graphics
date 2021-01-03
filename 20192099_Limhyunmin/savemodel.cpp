#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
// �׸� �� �ִ� �� ���� ����
#define WIRE 0
#define SHADE 1
#define NUMBER_OF_POINT        10000  
/*#define _USE_MATH_DEFINES
#include <math.h>*/

float PI = 3.141592;

enum __shape    // �׸� ������ ������ �����ϴ� Enumerator
{
	POINT_1 = 1,
	LINES_1 = 8,
	ROTATEPOINT_Y30 = 2,
	ROTATEPOINT_Y10 = 3,
	ROTATEPOINT_Y60 = 4,
	ROTATEPOINT_X30 = 5,
	ROTATEPOINT_X10 = 6,
	ROTATEPOINT_X60 = 7
}          Shape;

#include<vector>
class xPoint3D
{
public:
	float x, y, z, w;
	xPoint3D() { x = y = z = 0; w = 1; };
};

class Face {
public:
	unsigned int ip[3];
};



xPoint3D newpt;

xPoint3D pt;

GLfloat myVertices[NUMBER_OF_POINT][2];            // �׷��� ���� ��ġ�� �����ϴ� �迭
GLfloat currentWidth = 800.0;      // ������ â ũ�⸦ �����ϴ� ����
GLfloat currentHeight = 600.0;
GLfloat initWidth = 800.0;           // ó�� â ũ�⸦ �����ϴ� ����
GLfloat initHeight = 600.0;
GLint count = 0;     // ��ü ���� ������ �����ϴ� ����
int i = 0;
int rot = 0;
float ox = 0;
float oy = 0;
float fRotAngle = 0;
float centerY = 0;
int pnum = 0;
int fnum = 0;
int sixearth = 0;
Face Mungji;

std::vector<xPoint3D>arInputPoints;
std::vector<xPoint3D>arRotPoints;
std::vector<Face> arFaces;

float radian = fRotAngle * (PI / 180.);

void SaveModel()
{
	FILE* fout;
	fopen_s(&fout, "c:\\data\\myModel.dat", "w");
	fprintf(fout, "VERTEX = %d\n", pnum+2);
	for (int i = 0; i < pnum; i++)
	{
		fprintf(fout, "%.1f %.1f %.1f\n", arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
	}
	fprintf(fout, "0.0 %.1f 0.0\n", arRotPoints[pnum].y);
	fprintf(fout, "0.0 %.1f 0.0\n", arRotPoints[pnum+count-1].y);

	

	fprintf(fout, "FACE = %d\n", fnum);
	for (int i = 0; i < fnum; i++)
	{
		fprintf(fout, "%d %d %d\n", arFaces[i].ip[0], arFaces[i].ip[1], arFaces[i].ip[2]);
	}
	fprintf(fout, "VERTEX = 1 \n");
	for (int i = 0; i < 1; i++)
	{
		if (i = count - 1)
			fprintf(fout, "%.1f\n", arRotPoints[i].y);

	}
	fclose(fout);
}

void MyDisplay()
{
	GLint index;
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINES);    //���۰� �� ���� ������ ��� ���� ����
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(500.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-500.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINES);    //���۰� �� ���� ������ ��� ���� ����
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 350.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, -350.0, 0.0);
	glEnd();

	glColor3f(0.f, 0.f, 1.f);  //�Ķ���
	glBegin(GL_LINE_LOOP);  //z�� �׸���
	glVertex3f(0.0, 0.0, -300.0); //������
	glVertex3f(0.0, 0.0, 300.0);  //����
	glEnd();

	glColor3f(1, 0.78823, 0.05490); // �׷����� ��ü�� ����, ���⼭�� R ���� 1�̹Ƿ� ������
	glPointSize(8.0f);  // �׷����� ���� ũ��
	glEnableClientState(GL_VERTEX_ARRAY);     // �迭�� ��� �����ϵ��� ��
	// myVertices �迭 �ȿ� ��ǥ�� �ϳ��� �� �ִٸ�


	if (count > 0)
	{
		switch (Shape)
		{
			// ���� ��´�
		case POINT_1:
			for (i = 0; i < count; i++)
			{
				glBegin(GL_POINTS);
				glVertex3f(arInputPoints[i].x, arInputPoints[i].y, arInputPoints[i].z);
			}
			break;
			// ���� �׸���
		case LINES_1:
			for (i = 0; i < count; i++)
			{
				glBegin(GL_LINES);
				glVertex3f(arInputPoints[i].x, arInputPoints[i].y, arInputPoints[i].z);
			}
			glEnd;
			break;
		case ROTATEPOINT_Y30:
			fRotAngle = 30;
			rot = fRotAngle;
			printf("30degrees\n");
			for (fRotAngle = 30; fRotAngle <= 360; fRotAngle += rot)

			{
				float radian = fRotAngle * (PI / 180.);
				for (int i = 0; i < arInputPoints.size(); i++)
				{
					newpt.x = arInputPoints[i].z * sin(radian) + arInputPoints[i].x * cos(radian);
					newpt.y = arInputPoints[i].y;
					newpt.z = arInputPoints[i].z * cos(radian) - arInputPoints[i].x * sin(radian);
					arRotPoints.push_back(newpt);
					glBegin(GL_POINTS);
					glVertex3f(arInputPoints[i].x, arInputPoints[i].y, arInputPoints[i].z);
					glVertex3f(newpt.x, newpt.y, newpt.z);
					pnum++;
					std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;

				}

			}

			newpt.x = 0; //���߽���
			newpt.y = arInputPoints[0].y;
			newpt.z = 0;
			arRotPoints.push_back(newpt);
			glBegin(GL_POINTS);
			glVertex3f(newpt.x, newpt.y, newpt.z);
			std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;
			
		
			newpt.x = 0; //�ٴ��߽���
			newpt.y = arInputPoints[0].y;
			newpt.z = 0;
			arRotPoints.push_back(newpt);
			glBegin(GL_POINTS);
			glVertex3f(newpt.x, newpt.y, newpt.z);
			std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;
		


			for (int i = 0; i < count - 1; i++) // ����
			{
				for (int k = 0; k < 360/rot - 1; k ++)
				{
					int j;
					j = count * k + i;
					Mungji.ip[0] = j;
					Mungji.ip[1] = j + 1;
					Mungji.ip[2] = j + count;
					fnum++;
					arFaces.push_back(Mungji);
					Mungji.ip[0] = j + 1;
					Mungji.ip[1] = j + count + 1;
					Mungji.ip[2] = j + count;
					fnum++;
					arFaces.push_back(Mungji);
				}
			}

								
			for (int i = 0; i < count - 1; i++) //������ ����
			{
				Mungji.ip[0] = count * 360 / rot - count + i;
				Mungji.ip[1] = count * 360 / rot - count + i + 1;
				Mungji.ip[2] = i;
				fnum++;
				arFaces.push_back(Mungji);
				Mungji.ip[0] = count * 360 / rot - count + i + 1;
				Mungji.ip[1] = i + 1;
				Mungji.ip[2] = i;
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 360 / rot - 1; i++) //����
			{
				Mungji.ip[0] = i * count;
				Mungji.ip[1] = count * (i+1);
				Mungji.ip[2] = count * (360/rot);
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 1; i++) //����
			{
				Mungji.ip[0] = count * 360 / rot - count;
				Mungji.ip[1] = i;
				Mungji.ip[2] = count * 360 / rot;
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 1; i < 360 / rot; i++) //�Ʒ���
			{
				Mungji.ip[0] = count * (i + 1) - 1;;
				Mungji.ip[1] = i * count - 1;
				Mungji.ip[2] = count * (360 / rot) + 1;
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 1; i++) //�Ʒ���
			{
				Mungji.ip[0] = count - 1;
				Mungji.ip[1] = count * 360 / rot - 1;
				Mungji.ip[2] = count * 360 / rot + 1;
				fnum++;
				arFaces.push_back(Mungji);
			}


			SaveModel();
			break;

		case ROTATEPOINT_Y10:
			fRotAngle = 10;
			rot = fRotAngle;
			printf("10degrees\n");
			for (fRotAngle = 10; fRotAngle <= 360; fRotAngle += rot)
			{
				float radian = fRotAngle * (PI / 180.);
				for (int i = 0; i < arInputPoints.size(); i++)
				{
					newpt.x = arInputPoints[i].z * sin(radian) + arInputPoints[i].x * cos(radian);
					newpt.y = arInputPoints[i].y;
					newpt.z = arInputPoints[i].z * cos(radian) - arInputPoints[i].x * sin(radian);
					arRotPoints.push_back(newpt);
					glBegin(GL_POINTS);
					glVertex3f(arInputPoints[i].x, arInputPoints[i].y, arInputPoints[i].z);
					glVertex3f(newpt.x, newpt.y, newpt.z);
					pnum++;
					std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;

				}
			}

			newpt.x = 0; //���߽���
			newpt.y = arInputPoints[0].y;
			newpt.z = 0;
			arRotPoints.push_back(newpt);
			glBegin(GL_POINTS);
			glVertex3f(newpt.x, newpt.y, newpt.z);
			std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;


			newpt.x = 0; //�ٴ��߽���
			newpt.y = arInputPoints[0].y;
			newpt.z = 0;
			arRotPoints.push_back(newpt);
			glBegin(GL_POINTS);
			glVertex3f(newpt.x, newpt.y, newpt.z);
			std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;


			for (int i = 0; i < count - 1; i++) // ����
			{
				for (int k = 0; k < 360 / rot - 1; k++)
				{
					int j;
					j = count * k + i;
					Mungji.ip[0] = j;
					Mungji.ip[1] = j + 1;
					Mungji.ip[2] = j + count;
					fnum++;
					arFaces.push_back(Mungji);
					Mungji.ip[0] = j + 1;
					Mungji.ip[1] = j + count + 1;
					Mungji.ip[2] = j + count;
					fnum++;
					arFaces.push_back(Mungji);
				}
			}


			for (int i = 0; i < count - 1; i++) //������ ����
			{
				Mungji.ip[0] = count * 360 / rot - count + i;
				Mungji.ip[1] = count * 360 / rot - count + i + 1;
				Mungji.ip[2] = i;
				fnum++;
				arFaces.push_back(Mungji);
				Mungji.ip[0] = count * 360 / rot - count + i + 1;
				Mungji.ip[1] = i + 1;
				Mungji.ip[2] = i;
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 360 / rot - 1; i++) //����
			{
				Mungji.ip[0] = i * count;
				Mungji.ip[1] = count * (i + 1);
				Mungji.ip[2] = count * (360 / rot);
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 1; i++) //����
			{
				Mungji.ip[0] = count * 360 / rot - count;
				Mungji.ip[1] = i;
				Mungji.ip[2] = count * 360 / rot;
				fnum++;
				arFaces.push_back(Mungji);
			}


			for (int i = 1; i < 360 / rot; i++) //�Ʒ���
			{
				Mungji.ip[0] = count * (i + 1) - 1;;
				Mungji.ip[1] = i * count - 1;
				Mungji.ip[2] = count * (360 / rot) + 1;
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 1; i++) //�Ʒ���
			{
				Mungji.ip[0] = count - 1;
				Mungji.ip[1] = count * 360 / rot - 1;
				Mungji.ip[2] = count * 360 / rot + 1;
				fnum++;
				arFaces.push_back(Mungji);
			}

			SaveModel();
			break;

		case ROTATEPOINT_Y60:
			fRotAngle = 60;
			rot = fRotAngle;
			printf("60degrees\n");
			for (fRotAngle = 60; fRotAngle <= 360; fRotAngle += rot)
			{
				float radian = fRotAngle * (PI / 180.);
				for (int i = 0; i < arInputPoints.size(); i++)
				{
					newpt.x = arInputPoints[i].z * sin(radian) + arInputPoints[i].x * cos(radian);
					newpt.y = arInputPoints[i].y;
					newpt.z = arInputPoints[i].z * cos(radian) - arInputPoints[i].x * sin(radian);
					arRotPoints.push_back(newpt);
					glBegin(GL_POINTS);
					glVertex3f(arInputPoints[i].x, arInputPoints[i].y, arInputPoints[i].z);
					glVertex3f(newpt.x, newpt.y, newpt.z);
					pnum++;
					std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;

				}
			}

			newpt.x = 0; //���߽���
			newpt.y = arInputPoints[0].y;
			newpt.z = 0;
			arRotPoints.push_back(newpt);
			glBegin(GL_POINTS);
			glVertex3f(newpt.x, newpt.y, newpt.z);
			std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;


			newpt.x = 0; //�ٴ��߽���
			newpt.y = arInputPoints[0].y;
			newpt.z = 0;
			arRotPoints.push_back(newpt);
			glBegin(GL_POINTS);
			glVertex3f(newpt.x, newpt.y, newpt.z);
			std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;

			for (int i = 0; i < count - 1; i++) // ����
			{
				for (int k = 0; k < 360 / rot - 1; k++)
				{
					int j;
					j = count * k + i;
					Mungji.ip[0] = j;
					Mungji.ip[1] = j + 1;
					Mungji.ip[2] = j + count;
					fnum++;
					arFaces.push_back(Mungji);
					Mungji.ip[0] = j + 1;
					Mungji.ip[1] = j + count + 1;
					Mungji.ip[2] = j + count;
					fnum++;
					arFaces.push_back(Mungji);
				}
			}


			for (int i = 0; i < count - 1; i++) //������ ����
			{
				Mungji.ip[0] = count * 360 / rot - count + i;
				Mungji.ip[1] = count * 360 / rot - count + i + 1;
				Mungji.ip[2] = i;
				fnum++;
				arFaces.push_back(Mungji);
				Mungji.ip[0] = count * 360 / rot - count + i + 1;
				Mungji.ip[1] = i + 1;
				Mungji.ip[2] = i;
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 360 / rot - 1; i++) //����
			{
				Mungji.ip[0] = i * count;
				Mungji.ip[1] = count * (i + 1);
				Mungji.ip[2] = count * (360 / rot);
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 1; i++) //����
			{
				Mungji.ip[0] = count * 360 / rot - count;
				Mungji.ip[1] = i;
				Mungji.ip[2] = count * 360 / rot;
				fnum++;
				arFaces.push_back(Mungji);
			}


			for (int i = 1; i < 360 / rot; i++) //�Ʒ���
			{
				Mungji.ip[0] = count * (i + 1) - 1;;
				Mungji.ip[1] = i * count - 1;
				Mungji.ip[2] = count * (360 / rot) + 1;
				fnum++;
				arFaces.push_back(Mungji);
			}

			for (int i = 0; i < 1; i++) //�Ʒ���
			{
				Mungji.ip[0] = count - 1;
				Mungji.ip[1] = count * 360 / rot - 1;
				Mungji.ip[2] = count * 360 / rot + 1;
				fnum++;
				arFaces.push_back(Mungji);
			}

			SaveModel();
			break;

		case ROTATEPOINT_X30:
			fRotAngle = 30;
			rot = fRotAngle;
			printf("30degrees\n");
			for (fRotAngle = 30; fRotAngle <= 360; fRotAngle += rot)
			{
				float radian = fRotAngle * (PI / 180.);
				for (int i = 0; i < arInputPoints.size(); i++)
				{
					newpt.x = arInputPoints[i].x;
					newpt.y = arInputPoints[i].y * cos(radian) - arInputPoints[i].z * sin(radian);
					newpt.z = arInputPoints[i].y * sin(radian) + arInputPoints[i].x * cos(radian);
					arRotPoints.push_back(newpt);
					glBegin(GL_POINTS);
					glVertex3f(arInputPoints[i].x, arInputPoints[i].y, arInputPoints[i].z);
					glVertex3f(newpt.x, newpt.y, newpt.z);
					pnum++;
					std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;
				}
			}


			SaveModel();
			break;

		case ROTATEPOINT_X10:
			fRotAngle = 10;
			rot = fRotAngle;
			printf("10degrees\n");
			for (fRotAngle = 10; fRotAngle < 360; fRotAngle += rot)
			{
				float radian = fRotAngle * (PI / 180.);
				for (int i = 0; i < arInputPoints.size(); i++)
				{
					newpt.x = arInputPoints[i].x;
					newpt.y = arInputPoints[i].y * cos(radian) - arInputPoints[i].z * sin(radian);
					newpt.z = arInputPoints[i].y * sin(radian) + arInputPoints[i].x * cos(radian);
					arRotPoints.push_back(newpt);
					glBegin(GL_POINTS);
					glVertex3f(arInputPoints[i].x, arInputPoints[i].y, arInputPoints[i].z);
					glVertex3f(newpt.x, newpt.y, newpt.z);
					pnum++;
					std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;

				}
			}
			SaveModel();
			break;

		case ROTATEPOINT_X60:
			fRotAngle = 60;
			rot = fRotAngle;
			printf("60degrees\n");
			for (fRotAngle = 60; fRotAngle < 360; fRotAngle += rot)
			{
				float radian = fRotAngle * (PI / 180.);
				for (int i = 0; i < arInputPoints.size(); i++)
				{
					newpt.x = arInputPoints[i].x;
					newpt.y = arInputPoints[i].y * cos(radian) - arInputPoints[i].z * sin(radian);
					newpt.z = arInputPoints[i].y * sin(radian) + arInputPoints[i].x * cos(radian);
					arRotPoints.push_back(newpt);
					glBegin(GL_POINTS);
					glVertex3f(arInputPoints[i].x, arInputPoints[i].y, arInputPoints[i].z);
					glVertex3f(newpt.x, newpt.y, newpt.z);
					pnum++;
					std::cout << newpt.x << "," << newpt.y << "," << newpt.z << std::endl;

				}
			}
			SaveModel();
			break;
		}
	}
	glEnd();
	glFlush();
}

// �޴� �̺�Ʈ Call Back �Լ�

void MyMainMenu(int entryID)
{
	if (entryID == 3)
	{
		exit(0);
	}
	glutPostRedisplay();
}
void MySubYMenu(int entryID)
{
	if (entryID == 1)
	{
		Shape = ROTATEPOINT_Y10;
	}
	else if (entryID == 2)
	{
		Shape = ROTATEPOINT_Y30;
	}
	else if (entryID == 3)
	{
		Shape = ROTATEPOINT_Y60;
	}
	glutPostRedisplay();
}
void MySubXMenu(int entryID)
{
	if (entryID == 1)
	{
		Shape = ROTATEPOINT_X10;
	}
	else if (entryID == 2)
	{
		Shape = ROTATEPOINT_X30;
	}
	else if (entryID == 3)
	{
		Shape = ROTATEPOINT_X60;
	}
	glutPostRedisplay();
}

// �ٽ� �׷��ִ� �Լ�

void MyReshape(int NewWidth, int NewHeight)
{
	// â ũ�Ⱑ �ٲ� �� ���ο� â�� ũ�⸦ ����
	currentWidth = NewWidth;
	currentHeight = NewHeight;
	// ����Ʈ ����
	glViewport(0, 0, NewWidth, NewHeight);
	glLoadIdentity();
	glOrtho(-1.0 * (currentWidth / 2), 1.0 * (currentWidth / 2), -1.0 * (currentHeight / 2), 1.0 * (currentHeight / 2), -300.0, 300.0);

}


void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y)
{
	// ���콺�� ���ʹ�ư�� Ŭ���Ǿ��� ��
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
	{
		// Ŭ���� ���콺�� ��ġ�� X, Y ��ǥ�� ����

		myVertices[count][0] = X / currentWidth;
		myVertices[count][1] = (currentHeight - Y) / currentHeight;

		pt.x = X - (currentWidth / 2);
		pt.y = (currentHeight / 2) - Y;
		pt.z = 0;
		arInputPoints.push_back(pt);
		arRotPoints.push_back(pt);
		printf("Point: (%.f,%.f) \n", pt.x, pt.y);
		count++;          // ��ü ��ǥ�� ���� ����
		printf("Count: %d \n", count);
		MyDisplay();       // ���콺�� Ŭ���� ������ �ٽ� �׷���
	}
}


int main(int argc, char** argv)
{
	Shape = POINT_1;
	// ���� ������ �ʱ�ȭ

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(currentWidth, currentHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("20192099_Limhyunmin");
	glClearColor(0.439215, 0.572549, 0.745098, 1.0);
	glMatrixMode(GL_PROJECTION);
	// Call Back �Լ� ���
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(MyMouseClick);
	// �޴� Call Back �Լ��� ����ϰ� �� �޴��� Label�� ��ȣ�� �Ҵ�
	GLint MySubYMenuID = glutCreateMenu(MySubYMenu);
	glutAddMenuEntry("10 Degrees", 1);
	glutAddMenuEntry("30 Degrees", 2);
	glutAddMenuEntry("60 Degrees", 3);
	GLint MySubXMenuID = glutCreateMenu(MySubXMenu);
	glutAddMenuEntry("10 Degrees", 1);
	glutAddMenuEntry("30 Degrees", 2);
	glutAddMenuEntry("60 Degrees", 3);
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("X-Rotation", MySubXMenuID);
	glutAddSubMenu("Y-Rotation", MySubYMenuID);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}