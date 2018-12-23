#include "framework.h"

class ModelObject
{
public:
    ModelObject(){}
    ~ModelObject(){}

    void SetModel(mat4 m) { model = m; }
    mat4 GetModel() { return model; }
private:
    mat4 model;

};