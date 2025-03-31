#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
#include ".\include\GL\freeglut.h"

// ���� ���� ũ��
const int WIDTH = 800;
const int HEIGHT = 600;

// �� ����
float ballX = WIDTH / 2.0f;
float ballY = HEIGHT / 2.0f;
float ballRadius = 10.0f;
float ballDX = 4.0f;
float ballDY = 4.0f;

// �е� ����
float paddleWidth = 100.0f;
float paddleHeight = 20.0f;
float paddleX = WIDTH / 2.0f - paddleWidth / 2;
float paddleY = 50.0f;

// ��� ����
struct Block
{
	float x, y;
	float w = 60.0f, h = 20.0f;
	bool destroyed = false;
};
std::vector<Block> blocks;

bool isGameOver = false;
bool isGameClear = false;
bool isDragging = false; // ���콺 �巡�� ����

void InitBlocks()
{
	blocks.clear();
	for (int row = 0; row < 5; ++row)
	{
		for (int col = 0; col < 10; ++col)
		{
			Block b;
			b.x = 60 + col * 70;
			b.y = HEIGHT - 50 - row * 30;
			blocks.push_back(b);
		}
	}
}

void DrawCircle(float cx, float cy, float r)
{
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 30; ++i)
	{
		float theta = 2.0f * 3.14159f * i / 30;
		glVertex2f(cx + r * cos(theta), cy + r * sin(theta));
	}
	glEnd();
}

void DrawText(float x, float y, const char* text)
{
	glRasterPos2f(x, y);
	while (*text)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
		text++;
	}
}

void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ��
	if (!isGameOver && !isGameClear)
	{
		glColor3f(1.0, 1.0, 1.0);
		DrawCircle(ballX, ballY, ballRadius);
	}

	// �е�
	glColor3f(0.2f, 0.6f, 1.0f);
	glRectf(paddleX, paddleY, paddleX + paddleWidth, paddleY + paddleHeight);

	// ���
	for (const auto& b : blocks)
	{
		if (!b.destroyed)
		{
			glColor3f(1.0f, 0.4f, 0.2f);
			glRectf(b.x, b.y, b.x + b.w, b.y + b.h);
		}
	}

	// �޽���
	if (isGameOver)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		DrawText(WIDTH / 2 - 50, HEIGHT / 2, "Game Over!");
	}
	else if (isGameClear)
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		DrawText(WIDTH / 2 - 50, HEIGHT / 2, "Game Clear!");
	}

	glutSwapBuffers();
}

void CheckCollision()
{	
}

void UpdateBall()
{
}

void Timer(int value)
{
	UpdateBall();
	CheckCollision();

	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);
}

// ���콺 ��ư ���� �ݹ�
void MouseButton(int button, int state, int x, int y)
{
}

// ���콺 �巡�� �ݹ�
void MouseDrag(int x, int y)
{
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27) // ESC
		exit(0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("��ϱ��� ����2");

	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);     // ��ư ���� �ݹ�
	glutMotionFunc(MouseDrag);      // �巡�� �� ��ġ �ݹ�
	glutTimerFunc(16, Timer, 0);

	InitBlocks();
	glutMainLoop();
	return 0;
}
