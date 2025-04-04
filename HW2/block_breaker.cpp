#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
#include ".\include\GL\freeglut.h"
/*------------------------------------추가적으로 구현한 기능---------------------------*/
//1. SPACE바를 누르면 게임이 일시 정지
//2. 오른쪽 방향키를 누르면 공의 속도가 1씩 증가(최대 속도 : 8)
//3. 왼쪽 방향키를 누르면 공의 속도가 1씩 감소(최소 속도 : 2)

// 게임 영역 크기
const int WIDTH = 800;
const int HEIGHT = 600;

// 공 정보
float ballX = WIDTH / 2.0f;
float ballY = HEIGHT / 2.0f;
float ballRadius = 10.0f;
float ballDX = 4.0f;
float ballDY = 4.0f;

// 패들 정보
float paddleWidth = 100.0f;
float paddleHeight = 20.0f;
float paddleX = WIDTH / 2.0f - paddleWidth / 2;
float paddleY = 50.0f;

// 드래그 이전 패들 좌표
float prevX, prevY = 0;

// 블록 정보
struct Block
{
	float x, y;
	float w = 60.0f, h = 20.0f;
	bool destroyed = false;
};
std::vector<Block> blocks;

bool isGameOver = false;
bool isGameClear = false;
bool isDragging = false; // 마우스 드래그 상태
bool isPausing = false;

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

	// 공
	if (!isGameOver && !isGameClear)
	{
		glColor3f(1.0, 1.0, 1.0);
		DrawCircle(ballX, ballY, ballRadius);
	}

	// 패들
	glColor3f(0.2f, 0.6f, 1.0f);
	glRectf(paddleX, paddleY, paddleX + paddleWidth, paddleY + paddleHeight);

	// 블록
	isGameClear = true;
	for (const auto& b : blocks)
	{
		if (!b.destroyed)
		{
			isGameClear = false;
			glColor3f(1.0f, 0.4f, 0.2f);
			glRectf(b.x, b.y, b.x + b.w, b.y + b.h);
		}
	}

	// 메시지
	if (isGameOver)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		DrawText(WIDTH / 2 - 50, HEIGHT / 2, "Game Over!");
	}
	else if (isGameClear)
	{
		//클리어 시 공 정지
		ballDX = 0;
		ballDY = 0;
		glColor3f(0.0f, 1.0f, 0.0f);
		DrawText(WIDTH / 2 - 50, HEIGHT / 2, "Game Clear!");
	}
	if (isPausing) {
		glColor3f(0.0f, 1.0f, 1.0f);
		DrawText(WIDTH / 2 - 50, HEIGHT / 2, "Pausing...");
	}
	glutSwapBuffers();
}

void CheckCollision(){	
	//벽에 닿은 경우
	if (ballX + ballRadius >= WIDTH || ballX - ballRadius <= 0) {
		ballDX = -ballDX;
	}
	//천장에 닿은 경우
	if (ballY + ballRadius >= HEIGHT) {
		ballDY = -ballDY;
	}
	//패들에 닿은 경우 
	//공의 속도가 빨라지면 패들 상단 경계에 닿는 순간이 넘어갈 수 있기 때문에 부등호를 이용 
	//패들 하단부에 의한 반사를 방지하기 위해 일정 범위 설정
	if (ballY - ballRadius <= paddleY + paddleHeight && ballY >= paddleY - ballRadius + paddleHeight - 2){
		if(ballX >= paddleX && ballX <= paddleX + paddleWidth){
			ballDY = -ballDY;
		}
	}
	
	//블록에 닿은 경우
	for (auto& b : blocks) {
		//제거된 블록 처리
		if (b.destroyed) continue;
		//블록의 좌우에 닿은 경우
		if (ballX + ballRadius >= b.x && ballX - ballRadius <= b.x + b.w) {
			if (ballY >= b.y && ballY <= b.y + b.h) {
				ballDX = -ballDX;
				b.destroyed = true;
				break;
			}
		}
		//블록의 상하에 닿은 경우
		if(ballY + ballRadius >= b.y && ballY - ballRadius <= b.y + b.h){
			if (ballX <= b.x + b.w && ballX >= b.x) {
				ballDY = -ballDY;
				b.destroyed = true;
				break;
			}
		}
	}
	//바닥으로 떨어진 경우
	if (ballY + ballRadius <= 0) {
		isGameOver = true;
	}
}

void UpdateBall(){
	if (!isPausing) {
		ballX += ballDX;
		ballY += ballDY;
	}
}

void Timer(int value)
{
	UpdateBall();
	CheckCollision();
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);
}

// 마우스 버튼 상태 콜백
void MouseButton(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			if ((x <= paddleX + paddleWidth && x >= paddleX) && (y <= HEIGHT - paddleY && y >= HEIGHT - (paddleY + paddleHeight))) {
				isDragging = true;
				prevX = x;
				printf("드래깅중");
			}
		}
		else if (state == GLUT_UP) {
			isDragging = false;
		}
	}
}

// 마우스 드래그 콜백
void MouseDrag(int x, int y){

	//직전 드래그 위치만큼 패들 위치 이동
	if (isDragging) {
		float paddleDX = x - prevX;
		paddleX += paddleDX;
	}
	prevX = x;
}

void Reshape(int w, int h){
	glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int x, int y){
	if (key == 27) // ESC
		exit(0);
	//게임이 진행 중일 때
	if (!isGameClear && !isGameOver) {
		//SPACE를 누르는 경우
		if (key == 32 ){ // SPACE
			//게임 중지
			if (!isPausing) {
				isPausing = true;
			}
			//게임 재개
			else {
				isPausing = false;
			}
		}
	}
}
void SpecKey(int key, int x, int y) {
	//속도 증가
	if (key == GLUT_KEY_RIGHT) {
		if (abs(ballDX) < 8 && abs(ballDY) < 8) {
			ballDX > 0 ? ballDX += 1 : ballDX -= 1;
			ballDY > 0 ? ballDY += 1 : ballDY -= 1;
		}
	}
	//속도 감소
	if (key == GLUT_KEY_LEFT) {
		if (abs(ballDX) > 2 && abs(ballDY) > 2) {
			ballDX > 0 ? ballDX -= 1 : ballDX += 1;
			ballDY > 0 ? ballDY -= 1 : ballDY += 1;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("블록깨기 과제2");

	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);     // 버튼 상태 콜백
	glutMotionFunc(MouseDrag);      // 드래그 중 위치 콜백
	glutTimerFunc(16, Timer, 0);
	glutSpecialFunc(SpecKey);
	InitBlocks();
	glutMainLoop();
	return 0;
}
