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

enum EntityType { PLAYER, PLATFORM, ENEMY};
enum AIType {WALKER, JUMPER, PATROLLER};
enum AIState {IDLE, WALKING, JUMPING, PATROLLING};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    
    float width = 1.0f;
    float height = 1.0f;
    
    float speed;
    
    bool isActive = true;
    EntityType lastCollision = PLAYER;
    int defeated_enemy = 0;
    GLuint textureID;
    bool destroy = false;
    
    bool jump = false;
    float jumpPower = 0;
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
    
    Entity();
    
    bool JumpOn(Entity *other);
    bool CheckCollision(Entity *other);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void Update(float deltaTime, Entity *player, Entity *platforms, int platformCount, Entity *rocks, int rocksCount);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    void AI(Entity *player);
    void AI_Walker(Entity *player);
    void AI_Jumper(Entity *player);
    void AI_Patroller(Entity *player);
    bool DetectDist(Entity *player, float d);
};
