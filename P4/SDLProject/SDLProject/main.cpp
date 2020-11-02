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
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 72
#define ENEMY_COUNT 3
using namespace std;


struct GameState{
    Entity *player;
    Entity *platforms;
    Entity *enemies;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, PlayerMatrix, projectionMatrix;

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
bool moving = true;

GLuint fontTextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n"; assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Devils Out", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    PlayerMatrix = glm::mat4(1.0f);
    
    projectionMatrix = glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.95f, 0.91f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     
    //player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-6.0f, 2.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 3.0f;
    state.player->jumpPower = 5.0f;
    state.player->textureID = LoadTexture("Player.png");
    state.player->width = 1.0f;
    
    
    //platform
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("Tile.png");
    GLuint wallTextureID = LoadTexture("Tile2.png");
    for(int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].entityType = PLATFORM;
    }
    for(int i = 0; i < 16; i++){
        state.platforms[i].position = glm::vec3(-6.5f+i, -5.5f, 0);
    }
    state.platforms[16].position = glm::vec3(3.5f, 0.0f, 0);
    state.platforms[17].position = glm::vec3(-6.5f, 1.0f, 0);
    state.platforms[18].position = glm::vec3(-5.5f, 1.0f, 0);
    state.platforms[19].position = glm::vec3(-3.0f, -2.0f, 0);
    state.platforms[20].position = glm::vec3(-2.0f, -2.0f, 0);
    state.platforms[21].position = glm::vec3(-1.0f, -2.0f, 0);
    state.platforms[22].position = glm::vec3(0.0f, -2.0f, 0);
    state.platforms[23].position = glm::vec3(1.0f, -2.0f, 0);
    state.platforms[24].position = glm::vec3(4.5f, 0.0f, 0);
    state.platforms[25].position = glm::vec3(5.5f, 0.0f, 0);
    state.platforms[26].position = glm::vec3(6.5f, 0.0f, 0);
    state.platforms[27].position = glm::vec3(-1.0f, 3.0f, 0);
    state.platforms[28].position = glm::vec3(0.0f, 3.0f, 0);
    state.platforms[29].position = glm::vec3(1.0f, 3.0f, 0);
    state.platforms[30].position = glm::vec3(2.0f, 3.0f, 0);
    state.platforms[31].position = glm::vec3(3.5f, 0.0f, 0);
    
    for(int i = 0; i < 32; i++){
        state.platforms[i].textureID = platformTextureID;
    }
    
    for(int i = 0; i < 16; i++){
        state.platforms[i+32].position = glm::vec3(-7.5f+i, 5.5f, 0);
        state.platforms[i+32].textureID = wallTextureID;
    }
    for(int i = 0; i < 12; i++){
        state.platforms[i+48].position = glm::vec3(-7.5f, -5.5f+i, 0);
        state.platforms[i+48].textureID = wallTextureID;
    }
    for(int i = 0; i < 12; i++){
        state.platforms[i+60].position = glm::vec3(7.5f, -5.5f+i, 0);
        state.platforms[i+60].textureID = wallTextureID;
    }
    
    //enemy
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("Enemy.png");

    state.enemies[0].position = glm::vec3(4.5f, 1.0f, 0);
    state.enemies[1].position = glm::vec3(4.5f, -4.5f, 0);
    state.enemies[2].position = glm::vec3(-4.0f, -4.5f, 0);
    state.enemies[2].speed = 3.0f;
    state.enemies[0].aiType = WALKER;
    state.enemies[1].aiType = JUMPER;
    state.enemies[2].aiType = PATROLLER;
    
    for(size_t i=0; i < ENEMY_COUNT; i++){
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].aiState = IDLE;
        state.enemies[i].width = 1.0f;
    }
    state.enemies[2].textureID = LoadTexture("Enemy_flying.png");
    
    //Text
    fontTextureID = LoadTexture("font1.png");
}

void ProcessInput() {
    
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 10.0f;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
    
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                    case SDLK_SPACE:
                        state.player->jump = true;
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if(moving){
        if (keys[SDL_SCANCODE_LEFT]) {
            state.player->velocity.x = -5.0f;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            state.player->velocity.x = +5.0f;
        }
        
    }
}

void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text,float size, float spacing, glm::vec3 position){
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    std::vector<float> vertices;
    std::vector<float> texCoords;
    for(int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        
        vertices.insert(vertices.end(), {
        offset + (-0.5f * size), 0.5f * size, offset + (-0.5f * size), -0.5f * size, offset + (0.5f * size), 0.5f * size, offset + (0.5f * size), -0.5f * size, offset + (0.5f * size), 0.5f * size, offset + (-0.5f * size), -0.5f * size,
        });
        texCoords.insert(texCoords.end(), { u, v,
                u, v + height,
                u + width, v,
                u + width, v + height,
                u + width, v,
                u, v + height,
            });
    }
    
    glm::mat4 modelMatrix = glm::mat4(1.0f); modelMatrix = glm::translate(modelMatrix, position); program->SetModelMatrix(modelMatrix);
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data()); glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data()); glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTextureID); glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}


void Update() {
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        
        state.player->Update(FIXED_TIMESTEP, NULL, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        
        for(size_t i=0; i < PLATFORM_COUNT; i++){
            state.platforms[i].Update(0, NULL, NULL, 0, NULL, 0);
        }
        
        for(size_t i=0; i < ENEMY_COUNT; i++){
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
    
}

void Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    for(size_t i=0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Render(&program);
    }
    
    for(size_t i=0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(&program);
    }
    
    state.player->Render(&program);
    
    DrawText(&program, fontTextureID, "Jumping(space) on the devils to kill them" , 0.5f, -0.25f, glm::vec3(-7.5f, 5.5f, 0));
    
    if(state.player->defeated_enemy == ENEMY_COUNT){
        DrawText(&program, fontTextureID, "You Win" , 1.0f, -0.25f, glm::vec3(-2.0f, 1.0f, 0));
        state.player->isActive = false;
        for(int i = 0; i < ENEMY_COUNT; i++) state.enemies[i].isActive = false;
    }
    else if(state.player->destroy){
        DrawText(&program, fontTextureID, "You Lose" , 1.0f, -0.25f, glm::vec3(-3.0f, 1.0f, 0));
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
