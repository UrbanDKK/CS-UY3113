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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, playerMatrix, basketMatrix, sunMatrix, projectionMatrix;

float sun_scale = 1.0f;
float sun_rotate = 0.0f;
float player_x = -4.5f;
float player_y = 0.0f;
float player_vx = 1.0f;
float player_vy = 0.0f;
float rot_speed_ball = 45.0f;
float player_rotate = 0.0f;
float basket_y = 0.0f;
float basket_vy = 2.0f;
bool larger = true;

float lastTicks = 0.0f;

GLuint playerTextureID;
GLuint basketTextureID;
GLuint sunTextureID;

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
    displayWindow = SDL_CreateWindow("Basketball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    playerMatrix = glm::mat4(1.0f);
    basketMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.79f, 0.91f, 0.96f, 1.0f);
    glEnable(GL_BLEND);
    
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     
    
    playerTextureID = LoadTexture("ball.PNG");
    basketTextureID = LoadTexture("basket.PNG");
    sunTextureID = LoadTexture("sun.PNG");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void Scaling_sun(float dt){
    if(!larger){
        if(sun_scale > 0.75f){
            sun_scale *= 1.0f - 0.75f * dt;
            sunMatrix = glm::scale(sunMatrix, glm::vec3(sun_scale, sun_scale, 1.0f));
        }
        if(sun_scale <= 0.75f){
            larger = true;
        }
    }
    else{
        if(sun_scale < 1.5f){
            sun_scale *= 1.0f + 0.75f * dt;
            sunMatrix = glm::scale(sunMatrix, glm::vec3(sun_scale, sun_scale, 1.0f));
        }
        if(sun_scale >= 1.5f){
            larger = false;
        }
    }
}

float gravity = 9.8;

void Bounce(float dt){
    float a = gravity * dt;
    if(player_y <= -3.5f ){
        player_vy = -player_vy;
    }
    player_vy -= a;
    
    if(player_x >= 4.75f || player_x <= -4.75f){
        player_vx = -player_vx;
        rot_speed_ball = -rot_speed_ball;
    }
    
    player_x += player_vx * dt;
    player_y += player_vy * dt;
    if(player_y < -3.5f){
        player_y = -3.5f;
    }
    
    playerMatrix = glm::translate(playerMatrix, glm::vec3(player_x, player_y, 0.0f));
}

void rotate_ball(float dt){
    player_rotate -= rot_speed_ball * dt;
    playerMatrix = glm::rotate(playerMatrix, glm::radians(player_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
}

void basket_move(float dt){
    basket_y += basket_vy * dt;
    
    if(basket_y >= 3.25 || basket_y <= -3.25){
        basket_vy = -basket_vy;
    }
}

void Update() {
    //Delta time
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    //set player
    playerMatrix = glm::mat4(1.0f);
    
    Bounce(deltaTime);
    rotate_ball(deltaTime);
    
    //set basket
    basketMatrix = glm::mat4(1.0f);
    basket_move(deltaTime);
    basketMatrix = glm::translate(basketMatrix, glm::vec3(4.5f, basket_y, 0.0f));
    
    //set sun
    sunMatrix = glm::mat4(1.0f);
    sunMatrix = glm::translate(sunMatrix, glm::vec3(-3.5f, 2.75f, 0.0f));
    Scaling_sun(deltaTime);
    sun_rotate -= 45.0f * deltaTime;
    sunMatrix = glm::rotate(sunMatrix, glm::radians(sun_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    
}

void Render() {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    //draw ball
    program.SetModelMatrix(playerMatrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //draw basket
    program.SetModelMatrix(basketMatrix);
    glBindTexture(GL_TEXTURE_2D, basketTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //draw sun
    program.SetModelMatrix(sunMatrix);
    glBindTexture(GL_TEXTURE_2D, sunTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    
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
