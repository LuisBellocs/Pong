//Example2_4.cpp : A bouncing ball 

//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/glut.h> //the glut file for windows operations
					 // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>

#define PI 3.1415926535898 

double xpos, ypos, ydir, xdir;         // x and y position for house to be drawn
double sx, sy, squash;          // xy scale factors
double rot, rdir;             // rotation 
int SPEED = 50;        // speed of timer call back in msecs
GLfloat T1[16] = { 1.,0.,0.,0.,\
				  0.,1.,0.,0.,\
				  0.,0.,1.,0.,\
				  0.,0.,0.,1. };
GLfloat S[16] = { 1.,0.,0.,0.,\
				 0.,1.,0.,0.,\
				 0.,0.,1.,0.,\
				 0.,0.,0.,1. };
GLfloat T[16] = { 1.,0.,0.,0.,\
				 0., 1., 0., 0.,\
				 0.,0.,1.,0.,\
				 0.,0.,0.,1. };

GLint circle_points = 100;
void MyCircle2f(GLfloat centerx, GLfloat centery, GLfloat radius) {
	GLint i;
	GLdouble angle;
	glBegin(GL_POLYGON);
	for (i = 0; i < circle_points; i++) {
		angle = 2 * PI * i / circle_points;
		glVertex2f(centerx + radius * cos(angle), centery + radius * sin(angle));
	}
	glEnd();
}

GLfloat RadiusOfBall = 15.;
// Draw the ball, centered at the origin
void draw_ball() {
	glColor3f(0.0, 0.0, 0.0);
	MyCircle2f(0., 0., RadiusOfBall);

}

// Paddle
GLfloat paddle1X = 140.;
GLfloat paddle1Y = 50.;
GLfloat paddle2X = 10.;
GLfloat paddle2Y = 50.;
GLfloat paddleWidth = 10.;
GLfloat paddleHeight = 40.;
GLfloat paddleSpeed = 5.;

void draw_paddle(GLfloat x, GLfloat y) {
	glColor3f(0.0, 0.0, 0.0); // Azul
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + paddleWidth, y);
	glVertex2f(x + paddleWidth, y + paddleHeight);
	glVertex2f(x, y + paddleHeight);
	glEnd();
}


// Controller
// Move at x axis paddle 2 using s key and w key
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': case 'W':
		if (paddle2Y + paddleHeight < 120) paddle2Y += paddleSpeed;
		break;
	case 's': case 'S':
		if (paddle2Y > 0) paddle2Y -= paddleSpeed;
		break;
	}
	glutPostRedisplay();
}

// Second Controller
// Move at x axis paddle1 using up_arrow and down_arrow
void keyboard2(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if (paddle1Y + paddleHeight < 120) paddle1Y += paddleSpeed;
		break;
	case GLUT_KEY_DOWN:
		if (paddle1Y > 0) paddle1Y -= paddleSpeed;
		break;
	}
	glutPostRedisplay();
}

void update(int value) {
	// Collision with floor
	if (ypos == RadiusOfBall && ydir == -1) {
		sy = sy * squash;

		if (sy < 0.8)
			squash = 1.1;
		else if (sy > 1.) {
			sy = 1.;
			squash = 0.9;
			ydir = 1;
		}
		sx = 1. / sy;
	}
	else {
		xpos = xpos + xdir * 1.5;
		ypos = ypos + ydir * 1.5 - (1. - sy) * RadiusOfBall;

		// right paddle collision 
		if (xpos >= paddle1X - RadiusOfBall && xpos <= paddle1X + paddleWidth) {
			if (ypos >= paddle1Y && ypos <= paddle1Y + paddleHeight) {
				xdir = -1;
			}
		}
		// lefy paddle collision
		if (xpos <= paddle2X + paddleWidth + RadiusOfBall && xpos >= paddle2X) {
			if (ypos >= paddle2Y && ypos <= paddle2Y + paddleHeight) {
				xdir = 1;
			}
		}
		// reset of ball when it goes out  
		if (xpos <= 0 - RadiusOfBall) {
			xpos = 80;
			ypos = 60;
		}

		if (xpos >= 160 + RadiusOfBall) {
			xpos = 80;
			ypos = 60;
		}
		// up and down collisions
		if (ypos == 120 - RadiusOfBall)
			ydir = -1;
		else if (ypos < RadiusOfBall)
			ydir = 1;
	}

	glutTimerFunc(SPEED, update, 0);
	glutPostRedisplay();
}	

void Display(void)
{
	// swap the buffers
	glutSwapBuffers();

	//clear all pixels with the specified clear color
	glClear(GL_COLOR_BUFFER_BIT);
	// 160 is max X value in our world
	// Define X position of the ball to be at center of window
	// xpos = 80.;

	//reset transformation state 
	glLoadIdentity();

	draw_paddle(paddle1X, paddle1Y);  // right paddle
	draw_paddle(paddle2X, paddle2Y);  // left paddle

	//Translate the bouncing ball to its new position
	T[12] = xpos;
	T[13] = ypos;
	glLoadMatrixf(T);

	T1[13] = -RadiusOfBall;
	
	glMultMatrixf(T1);
	S[0] = sx;
	S[5] = sy;
	// Scale the ball about its bottom
	glMultMatrixf(S);
	T1[13] = RadiusOfBall;
	// Translate ball up so bottom is at the origin
	glMultMatrixf(T1);

	draw_ball();
}


void reshape(int w, int h)
{
	// on reshape and on startup, keep the viewport to be the entire size of the window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// keep our logical coordinate system constant
	gluOrtho2D(0.0, 160.0, 0.0, 120.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void init(void) {
	//set the clear color to be white
	glClearColor(1.0, 1.0, 1.0, 0.0);
	// initial position set to 0,0
	xpos = 80; ypos = 60; xdir = 1; ydir = 1;  // Start from center
	sx = 1.; sy = 1.; squash = 0.9;
	rot = 0;

}


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(320, 240);
	glutCreateWindow("Bouncing Ball");
	init();
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard2);

	// Add timer callback to animate the ball
	glutTimerFunc(SPEED, update, 0);

	glutMainLoop();

	return 1;
}