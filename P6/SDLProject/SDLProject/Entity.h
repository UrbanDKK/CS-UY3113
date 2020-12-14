#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include <vector>
#include <math.h> 

enum EntityType { PLAYER, PLATFORM, ENEMY, BACKGROUND, GEM};
enum AIType {ROTATE, MOVE};
enum AIState {IDLE, ROTATING, MOVING};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    std::vector<float> enemy_data;
    
    bool goal = false;
    bool played_failure = false;
    float width = 1.0f;
    float height = 1.0f;
    
    float speed;
    
    bool isActive = true;
    //EntityType lastCollision = PLAYER;
    int gems_collected = 0;
    GLuint textureID;
    bool destroy = false;
    //bool warm = false;
    float warm = 0;
    
    int lives;
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    
    float rotate = 0;;
    float angular = 0;
    bool collidedTop, collidedBottom, collidedLeft, collidedRight;
    
    
    Entity();
    
    bool JumpOn(Entity *other);
    void Spotted(Entity *objects, int objectCount);
    bool CheckCollision(Entity *other);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
    bool CheckWarm(Map *map);
    void Update(float deltaTime, Entity *player, Map *map, Entity *enemy, int enemyCount, Entity *gems, int gemCount);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    void AI(Entity *player);
    void AI_Rotate(Entity *player, float left, float right,float ang, float Dist);
    void AI_Move(Entity *player, float left, float right, float Dist);
    bool DetectDist(Entity *player, float d);
};
