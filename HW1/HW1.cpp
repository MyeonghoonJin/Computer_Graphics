// HelloGL.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include ".\include\GL\freeglut.h"

// 전역 변수
int Width = 600, Height = 600;

// 콜백 함수 선언
void Render();
void Reshape(int w, int h);

int main(int argc, char **argv)
{
	// Freeglut 초기화
	glutInit(&argc, argv);

	// 윈도우 크기 지정
	glutInitWindowSize(Width, Height);

	// 칼라 버퍼의 속성 지정
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// 윈도우 생성
	glutCreateWindow("Hello OpenGL");

	// 콜백 함수 등록
	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);

	// 메시지 처리 루프 진입
	glutMainLoop();
	return 0;
}
void draw_coordinate_system()
{
	glColor3d(0.0, 0.0, 0.0);
	glLineWidth(2.0f);

	glBegin(GL_LINES);
	glVertex2d(-10.0, 0.0);
	glVertex2d(10.0, 0.0);
	glVertex2d(0.0, -10.0);
	glVertex2d(0.0, 10.0);
	glEnd();

	glColor3d(0.5, 0.5, 0.5);
	glLineWidth(1.0);
	glBegin(GL_LINES);
	for (double delta = -10; delta <= 10.0; delta += 1.0)
	{
		glVertex2d(-10.0, delta);
		glVertex2d(10.0, delta);

		glVertex2d(delta, 10.0);
		glVertex2d(delta, -10.0);
	}
	glEnd();
}
void draw_point(double x, double y)
{
	glColor3d(0.0, 0.0, 0.0);
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	glVertex2d(x, y);
	glEnd();
}
void draw_line(double x1, double y1, double x2, double y2)
{
	glColor3d(1.0, 0.0, 0.0);
	// glPointSize(5.0f);

	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}
void draw_triangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glColor3d(0.0, 1.0, 0.0);

	glBegin(GL_POLYGON);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();
}
void draw_ellipse(double cx, double cy, double sx, double sy, int snum)
{
	glColor3d(1.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < snum; i++)
	{
		double theta = i / (double)(snum - 1) * 2.0 * 3.141592;
		double x = cx + sx * cos(theta);
		double y = cy + sy * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}
void draw_sector(double cx, double cy, double r, double start_angle, double central_angle, int snum)
{
	glColor3d(0.0, 0.5, 1.0);

	// 각도를 라디안으로 변환
	double start_angle_rad = start_angle * 3.141592 / 180.0;
	double central_angle_rad = central_angle * 3.141592 / 180.0;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(cx, cy); // 중심점

	for (int i = 0; i <= snum; i++)
	{
		double theta = start_angle_rad + i / (double)(snum - 1) * central_angle_rad;
		double x = cx + r * cos(theta);
		double y = cy + r * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void Render()
{
	// 칼라 버퍼 초기화
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 관측 공간 지정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10.0, 10.0, -10.0, 10.0);

	// 모델링 좌표계 선택
	glMatrixMode(GL_MODELVIEW);

	// 좌표계 설정
	draw_coordinate_system();

	// 점 그리기
	draw_point(5.0, 3.0);

	// 선 그리기
	draw_line(2.0, 3.0, 0.0, -4.0);

	// 다각형 그리기
	draw_triangle(-4.0, 0.0, 0.0, 3.0, -6.0, 4.0);

	// 중심이 (-4,-4)이고 가로,세로 반지름이 4로 같은 타원 => 반지름이 4인 원 그리기
	draw_ellipse(-4.0, -4.0, 4.0, 4.0, 500);

	// 중심이 (7,-6)이고 가로 반지름이 1,세로 반지름이 2인 타원 그리기
	draw_ellipse(7.0, -6.0, 1.0, 2.0, 500);

	// 중심이 (-4,6)이고 반지름이 3, 45도에서 90도만큼 시계방향으로 펼쳐진 부채꼴 그리기
	draw_sector(-4.0, 6.0, 3.0, 45.0, 90.0, 500);
	// 칼라 버퍼 교환
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	// 관측 공간을 윈도우에 매핑
	glViewport(0, 0, w, h);
	Width = w;
	Height = h;
}
