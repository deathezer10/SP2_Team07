#include "Object.h"
#include "Assignment03.h"



void Object::render() {
    _scene->modelStack.PushMatrix();
    _scene->modelStack.Translate(position.x, position.y, position.z);
    _scene->modelStack.Rotate(rotationX, 1, 0, 0);
    _scene->modelStack.Rotate(rotationY, 0, 1, 0);
    _scene->modelStack.Rotate(rotationZ, 0, 0, 1);
    _scene->modelStack.Scale(scale, scale, scale);
    _scene->RenderMesh(_scene->meshList[type], true);
    _scene->modelStack.PopMatrix();
}