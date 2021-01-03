#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;
#define WIRE 0
#define SHADE 1
#define WCULL 2
#define SCULL 3
#define WUNCULL 4
#define SUNCULL 5
#define WORTHO 6
#define SORTHO 7
#define WUNORTHO 8
#define SUNORTHO 9

float PI = 3.141592;
GLuint  texture[1];


typedef struct {
	float x;
	float y;
	float z;
} Point;

class xPoint3D
{
public:
	float x, y, z, w;
	xPoint3D() { x = y = z = 0; w = 1; };
};

typedef struct {
	unsigned int ip[3];
} Face;

class TPoint {
public:
	float ip[3];
};

int pnum;
int fnum;
xPoint3D newpt;
xPoint3D pt;
TPoint tpoint;
Point* mpoint = NULL;
Face* mface = NULL;
GLfloat angle = 0; /* in degrees */
int moving;
int mousebegin;
int light_moving;
float scalefactor = 1.0;
int scaling = 0;
int status = 0; // WIRE or SHADE
float varx = 0.2;
float vary = 0.2;
float varz = 0.2;
float ground = 0.0;
float movex = 0.0;
float movey = 0.0;
float movez = 0.0;
float rotatex = 0.0;
float rotatey = 0.0;
float rotatez = 0.0;
float animatex = 0.0;
float fRotAngle = 0.0;
float backzero = 0.0;
float frontzero = 0.0;
float mscale = 1.0;
string fname = "./cup.dat";
GLfloat T = 0;
GLfloat xRotAngle = 0;
GLfloat yRotAngle = 0;
GLfloat xLocation = 0, yLocation = 0, zLocation = 0;

int trans;
int beginx, beginy;
GLint TexFilter = GL_LINEAR;
void glTexImage2D();


void DrawModel(float varx, float vary, float varz, float movex, float movey, float movez);

GLubyte* LoadBmp(const char* Path, int* Width, int* Height)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER* fh = NULL;
	BITMAPINFOHEADER* ih;
	BYTE* pRaster;
	hFile = CreateFileA(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER*)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);
	int len = FileSize - fh->bfOffBits;
	pRaster = (GLubyte*)malloc(len);
	memcpy(pRaster, (BYTE*)fh + fh->bfOffBits, len);
	// RGB로 순서를 바꾼다.
	for (BYTE* p = pRaster; p < pRaster + len - 3; p += 3) {
		BYTE b = *p;
		*p = *(p + 2);
		*(p + 2) = b;
	}
	ih = (BITMAPINFOHEADER*)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	*Width = ih->biWidth;
	*Height = ih->biHeight;
	free(fh);
	return pRaster;
}


void ReadModel()
{
	FILE* f1; char s[81]; int i;
	if (mpoint != NULL) delete mpoint;
	if (mface != NULL) delete mface;
	if ((f1 = fopen(fname.c_str(), "rt")) == NULL) { printf("No file\n"); exit(0); }
	fscanf(f1, "%s", s); printf("%s", s); fscanf(f1, "%s", s); printf("%s", s);
	fscanf(f1, "%d", &pnum); printf("%d\n", pnum);
	mpoint = new Point[pnum];
	for (i = 0; i < pnum; i++) {
		fscanf(f1, "%f", &mpoint[i].x); fscanf(f1, "%f", &mpoint[i].y); fscanf(f1, "%f", &mpoint[i].z);
		printf("%f %f %f\n", mpoint[i].x, mpoint[i].y, mpoint[i].z);
	}
	fscanf(f1, "%s", s); printf("%s", s);
	fscanf(f1, "%s", s); printf("%s", s);
	fscanf(f1, "%d", &fnum); printf("%d\n", fnum);
	mface = new Face[fnum];
	for (i = 0; i < fnum; i++) {
		fscanf(f1, "%d", &mface[i].ip[0]); fscanf(f1, "%d", &mface[i].ip[1]); fscanf(f1, "%d", &mface[i].ip[2]);
		printf("%d %d %d\n", mface[i].ip[0], mface[i].ip[1], mface[i].ip[2]);
	}
	fscanf(f1, "%s", s); printf("%s", s); fscanf(f1, "%s", s); printf("%s", s); fscanf(f1, "%f", s); printf("%f\n", s);
	fscanf(f1, "%f", &ground); printf("%f\n", ground); ///초기
	fclose(f1);
}

void DrawCull(void)
{
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
}

void UnDrawCull(void)
{
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
}

void Ortho(void)
{
	glEnable(GL_DEPTH_TEST);
	/* Setup the view and projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50, 50, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(100.0, 100.0, 100.0, // eye poisition
		0.0, 0.0, 0.0, // center is at (0,0,0)
		0.0, 1.0, 0.); // up is in positive Y direction
}

void UnOrtho(void)
{
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	gluPerspective(40.0, 1.0, 1.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(400.0, 400.0, 400.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.);
}

void DrawWire(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(1);
	glutSwapBuffers();
}

void DrawShade(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(1);
	glutSwapBuffers();
}

void WOrtho(void)
{
	Ortho();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(1);
	glutSwapBuffers();
	Ortho();
}

void WUnOrtho(void)
{
	UnOrtho();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(1);
	glutSwapBuffers();
}

void WDrawCull(void)
{
	DrawCull();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(1);
	glutSwapBuffers();

}

void WUnDrawCull(void)
{
	UnDrawCull();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(1);
	glutSwapBuffers();
}

void SOrtho(void)
{
	Ortho();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(1);
	glutSwapBuffers();
}

void SUnOrtho(void)
{
	UnOrtho();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(1);
	glutSwapBuffers();
}

void SDrawCull(void)
{
	DrawCull();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(1);
	glutSwapBuffers();
}

void SUnDrawCull(void)
{
	UnDrawCull();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(1);
	glutSwapBuffers();
}

void a_update()
{
	animatex = animatex + 10;
	backzero = 1.0;
	frontzero = 1.0;

}

void XRotate(void)
{
	rotatex = rotatex + 10;
}

void YRotate(void)
{
	rotatey = rotatey + 10;
}

void ZRotate(void)
{
	rotatez = rotatez + 10;
}

void InitLight() {
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	//GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat mat_shininess[] = { 30.0 };
	GLfloat light_specular[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { 500, 200, 200.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_COLOR_MATERIAL);
}


//Calculate Normal vector
Point cnormal(Point a, Point b, Point c) {
	Point p, q, r;
	double val;
	p.x = a.x - b.x; p.y = a.y - b.y; p.z = a.z - b.z;
	q.x = c.x - b.x; q.y = c.y - b.y; q.z = c.z - b.z;
	r.x = p.y * q.z - p.z * q.y;
	r.y = p.z * q.x - p.x * q.z;
	r.z = p.x * q.y - p.y * q.x;
	val = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
	r.x = r.x / val; r.y = r.y / val; r.z = r.z / val;
	return r;
}

GLint EnvMode = GL_REPLACE;

void GroundTexturing()
{
	GLubyte* data;
	int Width, Height; //for Texture
	glEnable(GL_TEXTURE_2D);
	data = LoadBmp("c:\\data\\bbbbb.bmp", &Width, &Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	// 텍스처 환경 설정
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glEnable(GL_REPLACE);

	// 텍스처 필터 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //얘는 맞음
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void BackTexturing()
{
	GLubyte* data;
	int Width, Height; //for Texture
	glEnable(GL_TEXTURE_2D);
	data = LoadBmp("c:\\data\\wood.bmp", &Width, &Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	// 텍스처 환경 설정
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glEnable(GL_REPLACE);

	// 텍스처 필터 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //얘는 맞음
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void RightTexturing()
{
	GLubyte* data;
	int Width, Height; //for Texture
	glEnable(GL_TEXTURE_2D);
	data = LoadBmp("c:\\data\\wood.bmp", &Width, &Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	// 텍스처 환경 설정
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glEnable(GL_REPLACE);

	// 텍스처 필터 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //얘는 맞음
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void LeftTexturing()
{
	GLubyte* data;
	int Width, Height; //for Texture
	glEnable(GL_TEXTURE_2D);
	data = LoadBmp("c:\\data\\wood.bmp", &Width, &Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	// 텍스처 환경 설정
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glEnable(GL_REPLACE);

	// 텍스처 필터 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //얘는 맞음
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void TopTexturing()
{
	GLubyte* data;
	int Width, Height; //for Texture
	glEnable(GL_TEXTURE_2D);
	data = LoadBmp("c:\\data\\wood.bmp", &Width, &Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	// 텍스처 환경 설정
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glEnable(GL_REPLACE);

	// 텍스처 필터 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //얘는 맞음
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void BottomTexturing()
{
	GLubyte* data;
	int Width, Height; //for Texture
	glEnable(GL_TEXTURE_2D);
	data = LoadBmp("c:\\data\\wood.bmp", &Width, &Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	// 텍스처 환경 설정
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glEnable(GL_REPLACE);

	// 텍스처 필터 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //얘는 맞음
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void DrawModel(float varx, float vary, float varz, float movex, float movey, float movez) {
	int i;
	glPushMatrix();
	glRotatef(rotatex, 0.0, 0.0, 1.0);
	glRotatef(rotatey + xRotAngle, 0.0, 1.0, 0.0);
	glRotatef(rotatez + yRotAngle, 1.0, 0.0, 0.0);
	glScalef(scalefactor, scalefactor, scalefactor);
	glColor3f(varx, vary, varz);
	float radian = animatex * (M_PI / 180);
	frontzero = cos(radian);
	for (i = 0; i < fnum; i++) {
		Point norm = cnormal(mpoint[mface[i].ip[2]], mpoint[mface[i].ip[1]], mpoint[mface[i].ip[0]]);
		glBegin(GL_TRIANGLES);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f((mpoint[mface[i].ip[0]].x * (cos(radian) / frontzero)) + (mpoint[mface[i].ip[0]].z * sin(radian) * backzero) + movex, mpoint[mface[i].ip[0]].y + movey, (mpoint[mface[i].ip[0]].z * (cos(radian) / frontzero) + (mpoint[mface[i].ip[0]].x * sin(radian) * backzero)) + movez);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f((mpoint[mface[i].ip[1]].x * (cos(radian) / frontzero)) + (mpoint[mface[i].ip[1]].z * sin(radian) * backzero) + movex, mpoint[mface[i].ip[1]].y + movey, (mpoint[mface[i].ip[1]].z * (cos(radian) / frontzero) + (mpoint[mface[i].ip[1]].x * sin(radian) * backzero)) + movez);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f((mpoint[mface[i].ip[2]].x * (cos(radian) / frontzero)) + (mpoint[mface[i].ip[2]].z * sin(radian) * backzero) + movex, mpoint[mface[i].ip[2]].y + movey, (mpoint[mface[i].ip[2]].z * (cos(radian) / frontzero) + (mpoint[mface[i].ip[2]].x * sin(radian) * backzero)) + movez);
		glEnd();
	}
	glPopMatrix();

}

void MakeGL_Model(float varx, float vary, float varz, float movex, float movey, float movez)
{
	int i;
	glShadeModel(GL_SMOOTH);
	if (glIsList(1)) glDeleteLists(1, 1);
	glNewList(1, GL_COMPILE);
	glPushMatrix();
	glTranslatef(xLocation, yLocation, zLocation);
	glRotatef(xRotAngle, 0.0, 1.0, 0.0); glRotatef(yRotAngle, 1.0, 0.0, 0.0);
	glScalef(scalefactor, scalefactor, scalefactor);

	glBegin(GL_LINES);
	for (int i = 0; i <= 500; i = i + 50)
	{
		if (i == 0) { glColor3f(.6, .3, .3); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(i, ground - 0.5, 1.0 * 500.0);
		glVertex3f(i, ground - 0.5, -1.0 * 500.0);
		if (i == 0) { glColor3f(.3, .3, .6); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(-i, ground - 0.5, -1.0 * 500);
		glVertex3f(-i, ground - 0.5, 1.0 * 500);
	};
	glEnd();

	glBegin(GL_LINES);
	for (int i = 0; i <= 500; i = i + 50)
	{
		if (i == 0) { glColor3f(.6, .3, .3); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(500.0, ground - 0.5, i);
		glVertex3f(-500.0, ground - 0.5, i);
		if (i == 0) { glColor3f(.3, .3, .6); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(500.0, ground - 0.5, -i);
		glVertex3f(-500.0, ground - 0.5, -i);
	};
	glEnd();

	glFlush();
	glPushMatrix();

	GroundTexturing();
	glBegin(GL_QUADS);   ////// 바닥
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(0.8f, 0.8f, 0.8f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(500.0, ground, 500.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(500.0, ground, -500.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-500.0, ground, -500.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-500.0, ground, 500.0);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	GroundTexturing();
	glBegin(GL_QUADS);   ////// 바닥
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(0.8f, 0.8f, 0.8f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(500.0, ground - 1, 500.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(500.0, ground - 1, -500.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-500.0, ground - 1, -500.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-500.0, ground - 1, 500.0);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	BackTexturing();
	glBegin(GL_QUADS);   ////// 뒷면
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(0.8f, 0.8f, 0.8f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(2000.0, 2000.0, -2000.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(2000.0, -2000.0, -2000.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-2000.0, -2000.0, -2000.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-2000.0, 2000.0, -2000.0);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	LeftTexturing();
	glBegin(GL_QUADS);   ////// 왼쪽
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(0.8f, 0.8f, 0.8f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-2000.0, 2000.0, -2000.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-2000.0, -2000.0, -2000.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-2000.0, -2000.0, 2000.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-2000.0, 2000.0, 2000.0);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	RightTexturing();
	glBegin(GL_QUADS);   ////// 오른쪽
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(0.8f, 0.8f, 0.8f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(2000.0, 2000.0, -2000.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(2000.0, -2000.0, -2000.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(2000.0, -2000.0, 2000.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(2000.0, 2000.0, 2000.0);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	TopTexturing();
	glBegin(GL_QUADS);   ////// 위
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(0.8f, 0.8f, 0.8f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(2000.0, 2000.0, -2000.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(2000.0, 2000.0, 2000.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-2000.0, 2000.0, 2000.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-2000.0, 2000.0, -2000.0);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();

	BottomTexturing();
	glBegin(GL_QUADS);   ////// 위
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(0.8f, 0.8f, 0.8f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(2000.0, -2000.0, -2000.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(2000.0, -2000.0, 2000.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-2000.0, -2000.0, 2000.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-2000.0, -2000.0, -2000.0);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	GLint index;

	glColor3f(1.f, 0.f, 0.f);  //빨간색
	glBegin(GL_LINE_LOOP);  //X축 그리기
	glVertex3f(-1.0 * 800, ground, 0.0); //시작점
	glVertex3f(1.0 * 800, ground, 0.0);  //끝점
	glEnd();

	glColor3f(0.f, 1.f, 0.f);  //초록색
	glBegin(GL_LINE_LOOP);  //y축 그리기
	glVertex3f(0.0, -1.0 * 800, 0.0); //시작점
	glVertex3f(0.0, 1.0 * 800, 0.0);  //끝점
	glEnd();

	glColor3f(0.f, 0.f, 1.f);  //파란색
	glBegin(GL_LINE_LOOP);  //z축 그리기
	glVertex3f(0.0, ground, -1.0 * 800); //시작점
	glVertex3f(0.0, ground, 1.0 * 800);  //끝점
	glEnd();
	glPopMatrix();

	DrawModel(varx, vary, varz, movex, movey, movez);
	glEndList();

}

void GLSetupRC(void)
{
	glEnable(GL_DEPTH_TEST);
	/* Setup the view and projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, // field of view in degree
		1.0, // aspect ratio
		1.0, // Z near
		2000.0); // Z far
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(600.0, 600.0, 600.0, // eye poisition
		0.0, 0.0, 0.0, // center is at (0,0,0)
		0.0, 1.0, 0.); // up is in positive Y direction
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	MakeGL_Model(varx, vary, varz, movex, movey, movez);
	if (status == WIRE)
		DrawWire();
	else if (status == WCULL)
		WDrawCull();
	else if (status == SCULL)
		SDrawCull();
	else if (status == WUNCULL)
		WUnDrawCull();
	else if (status == SUNCULL)
		SUnDrawCull();
	else if (status == WORTHO)
		WOrtho();
	else if (status == SORTHO)
		SOrtho();
	else if (status == WUNORTHO)
		WUnOrtho();
	else if (status == SUNORTHO)
		SUnOrtho();
	else
		DrawShade();
}

void keyboard(unsigned char key, int x, int y)
{
	printf("key %d\n", key);
	switch (key)
	{
	case 'w':
		status = WIRE; glutPostRedisplay(); break;
	case 's':
		status = SHADE; glutPostRedisplay(); break;
	case 'c':
		status = WCULL; glutPostRedisplay(); break;
	case 'C':
		status = SCULL; glutPostRedisplay(); break;
	case 'u':
		status = WUNCULL; glutPostRedisplay(); break;
	case 'U':
		status = SUNCULL; glutPostRedisplay(); break;
	case 'o':
		status = WORTHO; glutPostRedisplay(); break;
	case 'O':
		status = SORTHO; glutPostRedisplay(); break;
	case 'p':
		status = WUNORTHO; glutPostRedisplay(); break;
	case 'P':
		status = SUNORTHO; glutPostRedisplay(); break;
	case 'A':
		a_update();
		glutPostRedisplay();
		break;
	case 'X':
		rotatex = rotatex + 10;
		glutPostRedisplay(); break;
	case 'Y':
		rotatey = rotatey + 10;
		glutPostRedisplay(); break;
	case 'Z':
		rotatez = rotatez + 10;
		glutPostRedisplay(); break;

		/*
			case '+':
				if (mscale < 10.0) {
					mscale = mscale + 1.0;
					glutPostRedisplay(); break;
				}
				else {
					break;
				}

			case '-':
				if (mscale > 0.1) {
					mscale = mscale - 1.0;
					glutPostRedisplay(); break;
				}
				else {
					break;
				}
				*/
	case 'R':
		if (varx > 1.0) {
			break;
		}
		else {
			varx = varx + 0.2;
			glutPostRedisplay(); break;
		}
	case 'r':
		if (varx <= 0.1) {
			break;
		}
		else
		{
			varx = varx - 0.2;
			glutPostRedisplay(); break;

		}
	case 'G':
		if (vary > 1.0) {
			break;
		}
		else {
			vary = vary + 0.2;
			glutPostRedisplay(); break;
		}
	case 'g':
		if (vary <= 0.1) {
			break;
		}
		else
		{
			vary = vary - 0.2;
			glutPostRedisplay(); break;
		}
	case 'B':
		if (varz > 1.0) {
			break;
		}
		else {
			varz = varz + 0.2;
			glutPostRedisplay(); break;
		}
	case 'b':

		if (varz <= 0.1) {
			break;
		}
		else
		{
			varz = varz - 0.2;
			glutPostRedisplay(); break;
		}

	}
}

void MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			trans = 1;
		}
		else {
			moving = 1;
		}
		beginx = x; beginy = y;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		moving = 0; trans = 0;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		scaling = 1; beginx = x;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		scaling = 0;
	}
}

void MouseMotion(int x, int y)
{
	if (scaling) {
		scalefactor = scalefactor * (1.0 + (beginx - x) * 0.0001);
		glutPostRedisplay();
	}
	if (trans) {
		xLocation = xLocation + (x - beginx);
		beginx = x;
		yLocation = yLocation + (beginy - y);
		beginy = y;
		glutPostRedisplay();
	}
	if (moving) {
		xRotAngle = xRotAngle + (x - beginx);
		beginx = x;
		yRotAngle = yRotAngle + (y - beginy);
		beginy = y;
		glutPostRedisplay();
	}
}

void SpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			movez = movez + 10;
		}
		else {
			movex = movex - 10;
			printf("Left directional key \n");
			glutPostRedisplay();
			break;
		}
		printf("Left directional key \n");
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			movez = movez - 10;
		}
		else {
			movex = movex + 10;
			printf("Left directional key \n");
			glutPostRedisplay();
			break;
		}
		printf("Right directional key \n");
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		movey = movey + 10;
		printf("Up directional key \n");
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		movey = movey - 10;
		printf("Down directional key \n");
		glutPostRedisplay();
		break;
	}
}
void MyMainMenu(int entryID)
{
	if (entryID == 1)
	{
		fname = "./cup.dat";

		ReadModel(); glutPostRedisplay();
	}

	else if (entryID == 2) {
		fname = "./glasses.dat";

		ReadModel(); glutPostRedisplay();
	}
	else if (entryID == 3)
	{
		fname = "./apple.dat";
		ReadModel(); glutPostRedisplay();
	}


	else if (entryID == 4)
	{
		exit(0);
	}

}
void resize(int width, int height) {
	// we ignore the params and do:
	glutReshapeWindow(1000, 1000);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE |
		GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("20192099_Limhyunmin");
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	glutAddMenuEntry("cup", 1);
	glutAddMenuEntry("glasses", 2);
	glutAddMenuEntry("apple", 3);
	glutAddMenuEntry("EXIT", 4);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMotion);
	glutSpecialFunc(SpecialKeyboard);
	ReadModel();
	GLSetupRC();
	InitLight();
	glutReshapeFunc(resize);
	glutMainLoop();
	return 0;
}