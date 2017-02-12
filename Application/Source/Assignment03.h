#ifndef ASSIGNMENT03_H
#define ASSIGNMENT03_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CharacterManager.h"
#include "UIManager.h"
#include "ObjectBuilder.h"
#include "ObjectInteractor.h"

#include <vector>

class Assignment03 : public Scene {

    friend class Object;
    friend class ObjectBuilder;
    friend class ObjectInteractor;
    friend class UIManager;
    friend class Bullet;

public:
    enum GEOMETRY_TYPE {

        GEO_NONE,

        GEO_AXES,
        GEO_FLOOR,

        GEO_LIGHTBALL,
        GEO_LIGHTBALL2,

        GEO_LEFT,
        GEO_RIGHT,
        GEO_TOP,
        GEO_BOTTOM,
        GEO_FRONT,
        GEO_BACK,

        GEO_TEXT,

        // Scene Models
        GEO_BUILDING,
        GEO_DOOR,
        GEO_CRATE,
        GEO_LAMP,
        GEO_TABLE,
        GEO_CHAIR,
        GEO_SPACESHIP,
        GEO_SLIME,
        GEO_ROCK1,

        GEO_BULLET,

        NUM_GEOMETRY
    };

    enum UNIFORM_TYPE {

        U_MVP = 0,
        U_MODELVIEW,
        U_MODELVIEW_INVERSE_TRANSPOSE,
        U_MATERIAL_AMBIENT,
        U_MATERIAL_DIFFUSE,
        U_MATERIAL_SPECULAR,
        U_MATERIAL_SHININESS,

        U_LIGHT0_POSITION,
        U_LIGHT0_COLOR,
        U_LIGHT0_POWER,
        U_LIGHT0_KC,
        U_LIGHT0_KL,
        U_LIGHT0_KQ,
        U_LIGHT0_TYPE,
        U_LIGHT0_SPOTDIRECTION,
        U_LIGHT0_COSCUTOFF,
        U_LIGHT0_COSINNER,
        U_LIGHT0_EXPONENT,

        U_LIGHT1_POSITION,
        U_LIGHT1_COLOR,
        U_LIGHT1_POWER,
        U_LIGHT1_KC,
        U_LIGHT1_KL,
        U_LIGHT1_KQ,
        U_LIGHT1_TYPE,
        U_LIGHT1_SPOTDIRECTION,
        U_LIGHT1_COSCUTOFF,
        U_LIGHT1_COSINNER,
        U_LIGHT1_EXPONENT,

        U_LIGHTENABLED,
        U_NUMLIGHTS,

        U_COLOR_TEXTURE_ENABLED,
        U_COLOR_TEXTURE,

        U_TEXT_ENABLED,
        U_TEXT_COLOR,

        U_TOTAL,

    };


    Assignment03(Application* app);
    ~Assignment03();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    Application* _app;
    CharacterManager charManager;
    UIManager textManager;
    ObjectBuilder objBuilder;
    Camera3 camera;
    float _dt;
    float _elapsedTime = 0;

private:
    Light light[2];

    MS modelStack, viewStack, projectionStack;

    Mesh* meshList[NUM_GEOMETRY];

    unsigned m_parameters[U_TOTAL];
    unsigned m_vertexArrayID;
    unsigned m_programID;

    void RenderCharacter();

    void RenderMesh(Mesh *mesh, bool enableLight);
    void RenderMeshOnScreen(Mesh* mesh, int x, int y, float sizex, float sizey);
    void RenderSkybox();

    const float shootCooldown = 0.05f; // Cooldown between each bullet shot

    bool pendingReset = false;
    float nextShootTime = 0;

};

#endif