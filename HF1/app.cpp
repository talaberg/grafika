#include "framework.h"
#include "mymath.h"

#include "Renderer.h"
#include "DrawEngine.hpp"

#include <memory>


const unsigned POINTS_PER_CPS = 4;

class Track
{
    CatmullRom curve;
    vector<float> vertexCoords;
    unsigned int vbo;        // vertex buffer object

    void CalcVertexCoords()
    {
        vertexCoords.resize(curve.cps.size() * POINTS_PER_CPS * 2);

        const float step = 0.25f;
        float t = 0;
        for (unsigned i = 0; i < curve.cps.size() * POINTS_PER_CPS; i++)
        {
            vec3 r = curve.r(t);
            vertexCoords[2 * i] = r.x;
            vertexCoords[2 * i + 1] = r.y;
            t += step;
        }
    }

public:
    void Addpoint(float x, float y)
    {
        curve.AddControlPoint(vec3(x, y, 0), (float)(curve.cps.size() + 1));
    }

    void Create(unsigned int &vao)
    {
        glGenVertexArrays(1, &vao);    // create 1 vertex array object
        glBindVertexArray(vao);        // make it active

        glGenBuffers(1, &vbo);    // Generate a vertex buffer object

        glBindBuffer(GL_ARRAY_BUFFER, vbo); // make it active, it is an array

        // Map Attribute Array 0 to the current bound vertex buffer (vbo[0])
        glEnableVertexAttribArray(0);
        // Data organization of Attribute Array 0
        glVertexAttribPointer(0,            // Attribute Array 0
            2, GL_FLOAT,  // components/attribute, component type
            GL_FALSE,        // not in fixed point format, do not normalized
            0, NULL);     // stride and offset: it is tightly packed

        glVertexAttribPointer(0,       // vbo -> AttribArray 0
            2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
            0, NULL);              // stride, offset: tightly packed
    }

    void Draw(const unsigned int vao, GPUProgram gpuProgram)
    {

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Geometry with 24 bytes (6 floats or 3 x 2 coordinates)

        glBufferData(GL_ARRAY_BUFFER,     // Copy to GPU target
            sizeof(float)* vertexCoords.size(),  // # bytes
            vertexCoords.data(),              // address
            GL_DYNAMIC_DRAW);

        glClearColor(0, 0, 0, 0);     // background color
        glClear(GL_COLOR_BUFFER_BIT); // clear frame buffer
        // Set color
        int location = glGetUniformLocation(gpuProgram.getId(), "color");
        glUniform3f(location, 1.0f, 1.0f, 0.0f); // 3 floats

        float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix,
            0, 1, 0, 0,    // row-major!
            0, 0, 1, 0,
            0, 0, 0, 1 };

        location = glGetUniformLocation(gpuProgram.getId(), "MVP");    // Get the GPU location of uniform variable MVP
        glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);    // Load a 4x4 row-major float matrix to the specified location

        glBindVertexArray(vao);  // Draw call
        glDrawArrays(GL_LINES, 0 /*startIdx*/, 1 /*# Elements*/);

        glutSwapBuffers(); // exchange buffers for double buffering
    }
};

std::unique_ptr<DrawEngine> drawEngine;
std::unique_ptr<ModelObject> mo;

// Initialization, create an OpenGL context
void onInitialization() {
   /* glViewport(0, 0, windowWidth, windowHeight);

    glGenVertexArrays(1, &vao);    // get 1 vao id
    glBindVertexArray(vao);        // make it active

    unsigned int vbo;        // vertex buffer object
    glGenBuffers(1, &vbo);    // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Geometry with 24 bytes (6 floats or 3 x 2 coordinates)
    float vertices[] = { -0.8f, -0.8f, -0.6f, 1.0f, 0.8f, -0.2f };
    glBufferData(GL_ARRAY_BUFFER,     // Copy to GPU target
        sizeof(vertices),  // # bytes
        vertices,              // address
        GL_STATIC_DRAW);    // we do not change later

    glEnableVertexAttribArray(0);  // AttribArray 0
    glVertexAttribPointer(0,       // vbo -> AttribArray 0
        2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
        0, NULL);              // stride, offset: tightly packed

    track.Create(vaoTrack);

    // create program for the GPU
    gpuProgram.Create(vertexSource, fragmentSource, "outColor");*/

    drawEngine = std::make_unique<DrawEngine>();

    mo = std::make_unique<ModelObject>();

    mo->SetModel(ScaleMatrix({ 1.0f, 1.0f, 1.0f }));

    std::vector<vec4> positions = {
        vec4(-0.5f, -0.5f, 0.0f, 1.0f),
        vec4( 0.5f, -0.5f, 0.0f, 1.0f),
        vec4( 0.5f,  0.5f, 0.0f, 1.0f),
        vec4(-0.5f,  0.5f, 0.0f, 1.0f)
    };
    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    std::vector<vec4> positions2;
    drawEngine->RegisterObject(mo.get(), positions, indices, { 0.1f, 0.1f, 0.8f, 1.0f });
    for (size_t i = 0; i < positions.size(); i++)
    {
        positions[i] = positions[i] * TranslateMatrix({ 0.2f, 0.3f, 0.0f });
    }
    drawEngine->RegisterObject(mo.get(), positions, indices, { 0.5f, 0.1f, 0.4f, 1.0f });
    for (size_t i = 0; i < positions.size(); i++)
    {
        positions[i] = positions[i] * TranslateMatrix({ -0.4f, -0.2f, 0.0f });
        positions[i] = positions[i] * RotationMatrix(45.0f, { -1.0f, 1.0f, 1.0f });
    }
    drawEngine->RegisterObject(mo.get(), positions, indices, { 0.7f, 0.7f, 0.2f, 1.0f });

}

// Window has become invalid: Redraw
void onDisplay() {
    /*
    // Set color to (0, 1, 0) = green
    int location = glGetUniformLocation(gpuProgram.getId(), "color");
    glUniform3f(location, 0.0f, 1.0f, 0.0f); // 3 floats

    float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix,
                              0, 1, 0, 0,    // row-major!
                              0, 0, 1, 0,
                              0, 0, 0, 1 };

    location = glGetUniformLocation(gpuProgram.getId(), "MVP");    // Get the GPU location of uniform variable MVP
    glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);    // Load a 4x4 row-major float matrix to the specified location

    glBindVertexArray(vao);*/  // Draw call
    //glDrawArrays(GL_TRIANGLES, 0 /*startIdx*/, 3 /*# Elements*/);

    //track.Draw(vaoTrack, gpuProgram);

    drawEngine->Draw();

    glutSwapBuffers(); // exchange buffers for double buffering


}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY) {
    if (key == 'd') glutPostRedisplay();         // if d, invalidate display, i.e. redraw
}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY) {
}

// Move mouse with key pressed
void onMouseMotion(int pX, int pY) {    // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
    // Convert to normalized device space
    float cX = 2.0f * pX / windowWidth - 1;    // flip y axis
    float cY = 1.0f - 2.0f * pY / windowHeight;
    printf("Mouse moved to (%3.2f, %3.2f)\n", cX, cY);
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
    // Convert to normalized device space
    float cX = 2.0f * pX / windowWidth - 1;    // flip y axis
    float cY = 1.0f - 2.0f * pY / windowHeight;

    char * buttonStat;
    switch (state) {
    case GLUT_DOWN: buttonStat = "pressed"; break;
    case GLUT_UP:   buttonStat = "released"; break;
    }

    switch (button) {
    case GLUT_LEFT_BUTTON:   printf("Left button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);   break;
    case GLUT_MIDDLE_BUTTON: printf("Middle button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY); break;
    case GLUT_RIGHT_BUTTON:  printf("Right button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);  break;
    }

//    track.Addpoint(cX, cY);
}

// Idle event indicating that some time elapsed: do animation here
void onIdle() {
    long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program

    float sec = time / 1000.0f;                // convert msec to sec

}
