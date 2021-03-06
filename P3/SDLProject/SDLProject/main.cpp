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

#define PLATFORM_COUNT 4
#define ROCK_COUNT 27
using namespace std;


struct GameState{
    Entity *player;
    Entity *platforms;
    Entity *rocks;
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
    displayWindow = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    PlayerMatrix = glm::mat4(1.0f);
    
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
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
    state.player->position = glm::vec3(0, 3.75f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0.75f, 0);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("Player.png");
    
    
    //platform
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("Safe.png");

    state.platforms[0].position = glm::vec3(1, -3.75f, 0);
    state.platforms[1].position = glm::vec3(2, -3.75f, 0);
    state.platforms[2].position = glm::vec3(0, -3.75f, 0);
    state.platforms[3].position = glm::vec3(-1.0f, -3.75f, 0);
    
    for(size_t i=0; i < PLATFORM_COUNT; i++){
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].entityType = PLATFORM;
    }
    
    //rock
    state.rocks = new Entity[ROCK_COUNT];
    GLuint rockTextureID = LoadTexture("Rock.png");
    
    state.rocks[0].position = glm::vec3(-3.0f, -3.75f, 0);
    state.rocks[1].position = glm::vec3(-4.0f, -3.75f, 0);
    state.rocks[2].position = glm::vec3(-5.0f, -3.75f, 0);
    state.rocks[3].position = glm::vec3(-3.0f, -2.75f, 0);
    state.rocks[4].position = glm::vec3(-2.0f, -3.75f, 0);
    state.rocks[5].position = glm::vec3(3.0f, -3.75f, 0);
    state.rocks[6].position = glm::vec3(4.0f, -3.75f, 0);
    state.rocks[7].position = glm::vec3(5.0f, -3.75f, 0);
    state.rocks[8].position = glm::vec3(-5.0f, -2.75f, 0);
    state.rocks[9].position = glm::vec3(-5.0f, -1.75f, 0);
    state.rocks[10].position = glm::vec3(-5.0f, -0.75f, 0);
    state.rocks[11].position = glm::vec3(-5.0f, 0.25f, 0);
    state.rocks[12].position = glm::vec3(-5.0f, 1.25f, 0);
    state.rocks[13].position = glm::vec3(-5.0f, 2.25f, 0);
    state.rocks[14].position = glm::vec3(-5.0f, 3.25f, 0);
    state.rocks[15].position = glm::vec3(5.0f, -2.75f, 0);
    state.rocks[16].position = glm::vec3(5.0f, -1.75f, 0);
    state.rocks[17].position = glm::vec3(5.0f, -0.75f, 0);
    state.rocks[18].position = glm::vec3(5.0f, 0.25f, 0);
    state.rocks[19].position = glm::vec3(5.0f, 1.25f, 0);
    state.rocks[20].position = glm::vec3(5.0f, 2.25f, 0);
    state.rocks[21].position = glm::vec3(5.0f, 3.25f, 0);
    state.rocks[22].position = glm::vec3(2.5f, 0.25f, 0);
    state.rocks[23].position = glm::vec3(1.5f, 0.25f, 0);
    state.rocks[24].position = glm::vec3(0.5, 0.25f, 0);
    state.rocks[25].position = glm::vec3(-0.5f, 0.25f, 0);
    state.rocks[26].position = glm::vec3(-4.0f, -2.75f, 0);
    
    for(size_t i=0; i < ROCK_COUNT; i++){
        state.rocks[i].textureID = rockTextureID;
        state.rocks[i].entityType = ROCK;
    }
    
    //Text
    fontTextureID = LoadTexture("font1.png");
}

void ProcessInput() {
    
    state.player->acceleration = glm::vec3(0, -0.5f, 0);
    
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
                        // Some sort of action
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if(moving){
        if (keys[SDL_SCANCODE_LEFT]) {
            state.player->acceleration.x = -6.0f;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            state.player->acceleration.x = 6.0f;
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
        
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT, state.rocks, ROCK_COUNT);
        
        for(size_t i=0; i < PLATFORM_COUNT; i++){
            state.platforms[i].Update(0, NULL, 0, NULL, 0);
        }
        
        for(size_t i=0; i < ROCK_COUNT; i++){
            state.rocks[i].Update(0, NULL, 0, NULL, 0);
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
    
    for(size_t i=0; i < ROCK_COUNT; i++){
        state.rocks[i].Render(&program);
    }
    
    state.player->Render(&program);
    
    if(state.player->lastCollision == PLATFORM){
        DrawText(&program, fontTextureID, "Mission Successful" , 0.5f, -0.25f, glm::vec3(-2.0f, 1.0f, 0));
    }
    else if(state.player->lastCollision == ROCK){
        DrawText(&program, fontTextureID, "Mission Failed" , 0.5f, -0.25f, glm::vec3(-2.0f, 1.0f, 0));
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
