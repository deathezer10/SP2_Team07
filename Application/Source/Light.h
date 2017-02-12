#ifndef LIGHT_H
#define LIGHT_H

#include "MeshBuilder.h"

struct Light
{
    //add these after existing parameters 
    enum LIGHT_TYPE
    {
        LIGHT_POINT = 0,
        LIGHT_DIRECTIONAL,
        LIGHT_SPOT,
    };

    LIGHT_TYPE type;
    Vector3 spotDirection;

    float cosCutoff;
    float cosInner;
    float exponent;

    Position position;
    Color color;

    float power;
    float kC, kL, kQ;

    Light(){
        position.Set(0, 0, 0);
        color.Set(1, 1, 1);
        power = 1;
        kC = 1.f;
        kL = 0.01f;
        kQ = 0.001f;
    }
};
#endif