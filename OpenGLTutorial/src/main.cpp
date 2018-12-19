#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// http://docs.gl/

int windowHeight = 600;
int windowWidth  = 600;

int u_Color_location;
float r = 0.0f;

Shader* shader;
VertexArray* va;
VertexBuffer* vb;
IndexBuffer* ib;

void onInitialize(void)
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	GLCALL(std::cout << glGetString(GL_VERSION) << std::endl;)

	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	va = new VertexArray();
	vb = new VertexBuffer(positions, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	va->AddBuffer(*vb, layout);

	ib = new IndexBuffer(indices, 6);

	shader = new Shader("res/shaders/Basic.shader");
}

void onDisplay(void)
{
	GLCALL(glClear(GL_COLOR_BUFFER_BIT));

	shader->Bind();
	shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

	va->Bind();
	ib->Bind();

	GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

	glutSwapBuffers(); // exchange buffers for double buffering
}

void onIdle()
{
	static long last_time = 0;
	static long dt = 0;
	long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
	dt += time - last_time;
	last_time = time;

	if (dt > 100)
	{
		static float increment = 0.05f;
		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		dt = 0;
		glutPostRedisplay();					// redraw the scene
	}

}

int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(windowHeight, windowWidth);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Tutorial");

	onInitialize();

	glutIdleFunc(onIdle);
	glutDisplayFunc(onDisplay);
	glutMainLoop();

	delete ib;
	delete vb;
	delete va;
	delete shader;

	return 0;
}