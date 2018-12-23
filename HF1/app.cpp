#include "framework.h"
#include "mymath.h"

#include "Renderer.h"
#include "DrawEngine.hpp"

#include <memory>


const unsigned POINTS_PER_CPS = 4;

class Track : public ModelObject
{
    CatmullRom curve;
    std::vector<vec4> vertexCoords;
    std::vector<unsigned int> indices;

    void SaturatePoints()
    {
        for (size_t i = 0; i < vertexCoords.size(); i++)
        {
            vertexCoords[i].x = (vertexCoords[i].x > 1.0f ? 1.0f : vertexCoords[i].x);
            vertexCoords[i].y = (vertexCoords[i].y > 1.0f ? 1.0f : vertexCoords[i].y);
            vertexCoords[i].x = (vertexCoords[i].x < -1.0f ? -1.0f : vertexCoords[i].x);
            vertexCoords[i].y = (vertexCoords[i].y < -1.0f ? -1.0f : vertexCoords[i].y);
        }
    }

    void CalcVertexCoords()
    {
        vertexCoords.resize(curve.cps.size() * POINTS_PER_CPS);
        indices.resize(curve.cps.size() * POINTS_PER_CPS);

        const float step = 0.25f;
        float t = 0;
        for (unsigned i = 0; i < curve.cps.size() * POINTS_PER_CPS; i++)
        {
            vec3 r = curve.r(t);
            vertexCoords[i] = { r.x, r.y, 0.0f, 1.0f };
            t += step;
            indices[i] = i;
        }

        SaturatePoints();

        vertexCoords.insert(vertexCoords.begin(), { -1.0f, vertexCoords[0].y, 0.0f, 1.0f });
        vertexCoords.insert(vertexCoords.begin(), { -1.0f, -1.0f, 0.0f, 1.0f });
        vertexCoords.push_back({ 1.0f, vertexCoords[vertexCoords.size() - 1].y, 0.0f, 1.0f });
        vertexCoords.push_back({ 1.0f, -1.0f, 0.0f, 1.0f });

        indices.push_back(indices.size());
        indices.push_back(indices.size());
        indices.push_back(indices.size());
        indices.push_back(indices.size());
        indices.push_back(0);
    }



public:
    Track()
    {
        SetModel(ScaleMatrix({ 1.0f, 1.0f, 1.0f }));
    }

    void Addpoint(float x, float y)
    {
        curve.AddControlPoint(vec3(x, y, 0), (float)(curve.cps.size() + 1));
    }

    void Create(DrawEngine &engine)
    {
        CalcVertexCoords();

        engine.RegisterObject(this, vertexCoords, indices, { 1.0f, 0.3f, 0.3f, 1.0f });
    }
};

std::unique_ptr<DrawEngine> drawEngine;
std::unique_ptr<ModelObject> mo;
Track track;

// Initialization, create an OpenGL context
void onInitialization()
{
    drawEngine = std::make_unique<DrawEngine>();
}

// Window has become invalid: Redraw
void onDisplay()
{
    drawEngine->Draw();

    glutSwapBuffers(); // exchange buffers for double buffering
}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY) {
    if (key == 'd') glutPostRedisplay();         // if d, invalidate display, i.e. redraw
    else if (key == ' ')
    {
        track.Create(*drawEngine);
        glutPostRedisplay();
    }
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

    track.Addpoint(cX, cY);
}

// Idle event indicating that some time elapsed: do animation here
void onIdle() {
    long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program

    float sec = time / 1000.0f;                // convert msec to sec

}
