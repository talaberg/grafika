#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// http://docs.gl/

int windowHeight = 600;
int windowWidth  = 600;

unsigned int buffer;
unsigned int vao;
unsigned int ibo;
unsigned int shader;
int u_Color_location;
float r = 0.0f;

VertexBuffer* vb;
IndexBuffer* ib;

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return{ ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	GLCALL(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCALL(glShaderSource(id, 1, &src, nullptr));
	GLCALL(glCompileShader(id));

	int result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char * message = (char*)alloca(length * sizeof(char));
		GLCALL(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCALL(glDeleteShader(id));
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCALL(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));
	GLCALL(glLinkProgram(program));
	GLCALL(glValidateProgram(program));

	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return program;
}

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

	GLCALL(glGenVertexArrays(1, &vao));
	GLCALL(glBindVertexArray(vao));

	vb = new VertexBuffer(positions, 4 * 2 * sizeof(float));

	// Specify vertex attribute
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 2, 0));

	ib = new IndexBuffer(indices, 6);

	// Specify vertex attribute
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCALL(glUseProgram(shader));

	GLCALL(u_Color_location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(u_Color_location != -1);
	GLCALL(glUniform4f(u_Color_location, r, 0.3f, 0.8f, 1.0f));

	GLCALL(glBindVertexArray(0));
	GLCALL(glUseProgram(0));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void onDisplay(void)
{
	GLCALL(glClear(GL_COLOR_BUFFER_BIT));

	GLCALL(glUseProgram(shader));
	GLCALL(glUniform4f(u_Color_location, r, 0.3f, 0.8f, 1.0f));

	GLCALL(glBindVertexArray(vao));

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

	delete vb;
	delete ib;
	glDeleteProgram(shader);

	return 0;
}