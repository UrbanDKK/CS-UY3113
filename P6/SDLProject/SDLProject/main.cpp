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
#include "Level4.h"
#include "Level5.h"

#include "Entity.h"
#include "Map.h"
#include "Util.h"

using namespace std;

Scene *currentScene;
Scene *sceneList[6];

Mix_Music *music;
//Mix_Chunk *jump;
Mix_Chunk *Gem_Collected;
Mix_Chunk *dead;
Mix_Chunk *lose;
Mix_Chunk *warming;
Mix_Chunk *win;


bool warm_play = false;
bool check_warm = true;
void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

int lives = 5;


SDL_Window* displayWindow;
bool gameIsRunning = true;
bool map = false;
bool play_end = false;

ShaderProgram program;
glm::mat4 viewMatrix, PlayerMatrix, projectionMatrix;
float view_X;
float view_Y;

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
    music = Mix_LoadMUS("Itty.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    
    lose = Mix_LoadWAV("GameOver.wav");
    dead = Mix_LoadWAV("Nope.wav");
    Gem_Collected = Mix_LoadWAV("getGem.wav");
    warming = Mix_LoadWAV("3sec.wav");
    win = Mix_LoadWAV("Victory.wav");
    
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
    sceneList[4] = new Level4();
    sceneList[5] = new Level5();
    SwitchToScene(sceneList[0]);
    
    if(currentScene->state.currScene!= 0)  currentScene->state.player->lives = lives;
    
    //Text
    fontTextureID = Util::LoadTexture("Font.png");
}

void ProcessInput() {
    if(currentScene->state.currScene != 0){
        currentScene->state.player->movement = glm::vec3(0);
        currentScene->state.player->speed = 6.0f;
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
                    
                    case SDLK_m:
                        
                        break;
                        
                    case SDLK_RETURN:
                        if(currentScene->state.currScene == 0){
                            currentScene->state.nextScene = 1;
                            Mix_PlayChannel(-1, Gem_Collected, 0);
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_M]) {
        if(currentScene->state.currScene != 0) map = true;
    }
    else map = false;
    
    if(currentScene->state.currScene != 0 && moving &&!map){
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->movement.x = -1.0f;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->movement.x = +1.0f;
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            currentScene->state.player->movement.y = -1.0f;
        }
        else if (keys[SDL_SCANCODE_UP]) {
            currentScene->state.player->movement.y = +1.0f;
        }
        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }
    }
}




void Update() {
    if(currentScene->state.currScene == 0){
        currentScene->Update(0);
        return;
    }
    else{
        moving = true;
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
    
    if(!map){
        projectionMatrix = glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, -1.0f, 1.0f);
        if(currentScene->state.player->position.x < 7.5f) view_X = -7.5f;
        else if(currentScene->state.player->position.x > currentScene->state.map->width - 8.5f) view_X = -(currentScene->state.map->width - 8.5f);
        else view_X = -currentScene->state.player->position.x;
        
        if(currentScene->state.player->position.y > -5.5f) view_Y = 5.5f;
        else if(currentScene->state.player->position.y < -currentScene->state.map->height
                +6.5f) view_Y = currentScene->state.map->height - 6.5f;
        else view_Y = -currentScene->state.player->position.y;
        
        
        viewMatrix = glm::translate(viewMatrix,glm::vec3(view_X, view_Y, 0));
    }
    
    else{
        projectionMatrix = glm::ortho(-currentScene->state.map->width/2.0f, currentScene->state.map->width/2.0f,-currentScene->state.map->width*3.0f/8.0f, currentScene->state.map->width*3.0f/8.0f, -1.0f, 1.0f);
        viewMatrix = glm::translate(viewMatrix,glm::vec3(-currentScene->state.map->width/2.0f +0.5f, currentScene->state.map->height/2.0f -0.5f, 0));
        map = false;
    }
    
}

void Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    if(currentScene->state.currScene!=0){
        if(lives<0) lives = 0;
        Util::DrawText(&program, fontTextureID, "Press M for map" , 0.27f, 0, glm::vec3(-view_X -7.5f, -view_Y+5.5f-1, 0));
        Util::DrawText(&program, fontTextureID, "Lives: " + to_string(lives), 0.3f, 0, glm::vec3(-view_X -7.5f, -view_Y+5.5f-0, 0));
        Util::DrawText(&program, fontTextureID, "Need to collect " + to_string(currentScene->state.gems_count-currentScene->state.player->gems_collected) + " more gem(s)", 0.3f, 0, glm::vec3(-view_X -7.5f, -view_Y+5.5f-0.5f, 0));
        Util::DrawText(&program, fontTextureID, "Level " + to_string(currentScene->state.currScene), 0.3f, 0, glm::vec3(-view_X -7.5f, -view_Y-5.5f, 0));
        
        if(currentScene->state.currScene == 1)
            Util::DrawText(&program, fontTextureID, "Rule 1: Don't stand on the warm zone for more than 3 sec" , 0.27f, 0, glm::vec3(-view_X -7.5f, -view_Y+5.5f-1.5f, 0));
        
        
        else if(currentScene->state.currScene == 2)
            Util::DrawText(&program, fontTextureID, "Rule 2: Don't get spotted by enemies" , 0.27f, 0, glm::vec3(-view_X -7.5f, -view_Y+5.5f-1.5f, 0));
        
        
        
        lives = currentScene->state.player->lives;
        
        
        
        if(currentScene->state.player->goal){
            currentScene->state.player->goal = false;
            Mix_PlayChannel(-1, Gem_Collected, 0);
        }
        else if(currentScene->state.player->played_failure){
            if(warm_play) Mix_HaltChannel(-1);
            warm_play = false;
            check_warm = false;
            currentScene->state.player->played_failure = false;
            Mix_PlayChannel(-1, dead, 0);
        }
        
        
        if(currentScene->state.nextScene == 6){
            Mix_HaltMusic();
            //Mix_HaltChannel(-1);
            Util::DrawText(&program, fontTextureID, "You Win" , 1.0f, 0, glm::vec3(-view_X -2.0f,-view_Y+5.5f -6.0f, 0));
            moving = false;
            if(!play_end){
                Mix_HaltChannel(-1);
                Mix_PlayChannel(-1, win, 0);
                play_end = true;
            }
            
        }
        else if(lives <= 0){
            Mix_HaltMusic();
            //Mix_HaltChannel(-1);
            currentScene->state.player->isActive = false;
            for(int i = 0; i < currentScene->state.enemy_count; i++) currentScene->state.enemies[i].isActive = false;
            moving = false;
            Util::DrawText(&program, fontTextureID, "You Lose" , 1.0f, 0, glm::vec3( -view_X -3.0f, -view_Y+5.5f-6.0f, 0));
            if(!play_end){
                Mix_HaltChannel(-1);
                Mix_PlayChannel(-1, lose, 0);
                play_end = true;
            }
            
        }
        else{
            if(currentScene->state.player->warm){
                if(!warm_play && check_warm) Mix_PlayChannel(-1, warming, 0);
                warm_play = true;
            }
            else{
                if(warm_play && check_warm) Mix_HaltChannel(-1);
                else check_warm = true;
                warm_play = false;
            }
        }
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
        if (currentScene->state.nextScene >= 0 && currentScene->state.nextScene < 6 && lives>0) {
            if(currentScene->state.currScene != 0) lives = currentScene->state.player->lives;
            SwitchToScene(sceneList[currentScene->state.nextScene]);
            currentScene->state.player->lives = lives;
            moving = false;
        }
    }
    
    Shutdown();
    return 0;
}
