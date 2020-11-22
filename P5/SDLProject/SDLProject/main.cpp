#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>

#include "Scene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

#include "Entity.h"
#include "Map.h"
#include "Util.h"

using namespace std;

Scene *currentScene;
Scene *sceneList[4];

Mix_Music *music;
Mix_Chunk *jump;
Mix_Chunk *Enemy_killed;
Mix_Chunk *failure;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

int lives = 3;


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, PlayerMatrix, projectionMatrix;
float view_X;

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
bool moving = true;

GLuint fontTextureID;

void Initialize() {
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Devils Out", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    //Audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    jump = Mix_LoadWAV("bounce.wav");
    failure = Mix_LoadWAV("failure.wav");
    Enemy_killed = Mix_LoadWAV("gameover.wav");
    
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
    
    sceneList[0] = new MainMenu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);
    
    if(currentScene->state.currScene!= 0)  currentScene->state.player->lives = lives;
    
    //Text
    fontTextureID = Util::LoadTexture("font1.png");
}

void ProcessInput() {
    if(currentScene->state.currScene != 0){
        currentScene->state.player->acceleration = glm::vec3(0, -9.81f, 0);
        currentScene->state.player->movement = glm::vec3(0);
        currentScene->state.player->speed = 10.0f;
    }
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
                        if(currentScene->state.currScene != 0){
                            currentScene->state.player->jump = true;
                            Mix_PlayChannel(-1, jump, 0);
                        }
                        break;
                    case SDLK_RETURN:
                        if(currentScene->state.currScene == 0){
                            currentScene->state.nextScene = 1;
                            Mix_PlayChannel(-1, Enemy_killed, 0);
                        }
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if(currentScene->state.currScene != 0 && moving){
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->velocity.x = -5.0f;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->velocity.x = +5.0f;
        }
        
    }
}




void Update() {
    if(currentScene->state.currScene == 0){
        currentScene->Update(0);
        return;
    }
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    lives = currentScene->state.player->lives;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    
    while (deltaTime >= FIXED_TIMESTEP) {
        
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    
    if(currentScene->state.player->position.x < 7.5f) view_X = -7.5f;
    else if(currentScene->state.player->position.x > currentScene->state.map->width - 8.5f) view_X = -(currentScene->state.map->width - 8.5f);
    else view_X = -currentScene->state.player->position.x;
    
    viewMatrix = glm::translate(viewMatrix,glm::vec3(view_X, 5.5, 0));

    
}

void Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    if(currentScene->state.currScene!=0){
        if(lives<0) lives = 0;
        
        Util::DrawText(&program, fontTextureID, "Lives: " + to_string(lives), 0.5f, -0.25f, glm::vec3(-view_X -7.5f, -1, 0));
        
        Util::DrawText(&program, fontTextureID, "Jumping(space) on the devils to kill them" , 0.5f, -0.25f, glm::vec3(-view_X -7.5f, 0, 0));
        
        lives = currentScene->state.player->lives;
        if(currentScene->state.player->goal){
            currentScene->state.player->goal = false;
            Mix_PlayChannel(-1, Enemy_killed, 0);
        }
        else if(currentScene->state.player->played_failure){
            currentScene->state.player->played_failure = false;
            Mix_PlayChannel(-1, failure, 0);
        }
        
        if(currentScene->state.nextScene == 4){
            Mix_HaltMusic();
            Util::DrawText(&program, fontTextureID, "You Win" , 1.0f, -0.25f, glm::vec3(-view_X -2.0f, -6.0f, 0));
        }
        else if(lives <= 0){
            Mix_HaltMusic();
            currentScene->state.player->isActive = false;
            for(int i = 0; i < currentScene->state.enemy_count; i++) currentScene->state.enemies[i].isActive = false;
            Util::DrawText(&program, fontTextureID, "You Lose" , 1.0f, -0.25f, glm::vec3( -view_X -3.0f, -6.0f, 0));
        }
    }
    
    else{
        Util::DrawText(&program, fontTextureID, "Press Enter to Start" , 0.65f, -0.35f, glm::vec3(-3.0f, -2.0f, 0));
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
        if (currentScene->state.nextScene >= 0 && currentScene->state.nextScene < 4 && lives>0) {
            if(currentScene->state.currScene != 0) lives = currentScene->state.player->lives;
            SwitchToScene(sceneList[currentScene->state.nextScene]);
            currentScene->state.player->lives = lives;
        }
    }
    
    Shutdown();
    return 0;
}
