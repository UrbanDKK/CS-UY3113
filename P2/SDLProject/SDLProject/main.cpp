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
glm::mat4 viewMatrix, LeftPaddleMatrix, BallMatrix, RightPaddleMatrix, projectionMatrix;


glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0, 0, 0);
glm::vec3 lp_position = glm::vec3(-4.75f, 0, 0); //left paddle
glm::vec3 lp_movement = glm::vec3(0, 0, 0);
glm::vec3 rp_position = glm::vec3(4.75f, 0, 0); //right paddle
glm::vec3 rp_movement = glm::vec3(0, 0, 0);
float paddle_speed = 3.0f;
float ball_speed = 2.5f;
float ball_acceleration = 0.2f; //makes game harder along time
float paddle_acceleration = 0.1f;
float lastTicks = 0.0f;
bool moving = true;

GLuint BallTextureID;
GLuint LeftPaddleTextureID;
GLuint RightPaddleTextureID;

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
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    BallMatrix = glm::mat4(1.0f);
    LeftPaddleMatrix = glm::mat4(1.0f);
    RightPaddleMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(1.0f, 0.91f, 0.96f, 1.0f);
    glEnable(GL_BLEND);
    
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     
    
    BallTextureID = LoadTexture("Ball.png");
    LeftPaddleTextureID = LoadTexture("Red.png");
    RightPaddleTextureID = LoadTexture("Blue.png");
}

void ProcessInput() {
    
    lp_movement =glm::vec3(0, 0, 0);
    rp_movement =glm::vec3(0, 0, 0);
    
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
        //left paddle
        if (keys[SDL_SCANCODE_W]) {
            lp_movement.y += 1;
        }
        else if (keys[SDL_SCANCODE_S]) {
            lp_movement.y -= 1;
        }
        
        //right paddle
        if (keys[SDL_SCANCODE_UP]) {
            rp_movement.y += 1;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            rp_movement.y -= 1;
        }
    }
}

bool collide_with_lp(){
    return (fabs(lp_position.x-ball_position.x) < 0.5f) && (fabs(lp_position.y-ball_position.y) < 1.25f);
}

bool collide_with_rp(){
    return (fabs(rp_position.x-ball_position.x) < 0.5f) && (fabs(rp_position.y-ball_position.y) < 1.25f);
}

void Ball_Bounce(float dt){
    if(!moving) return;
    
    //start game
    if(ball_movement == glm::vec3(0, 0, 0)){
        //random start direction
        int dir = rand()%4;
        ball_movement.x = dir < 2 ? 1.0f:-1.0f;
        ball_movement.y = dir%2 == 1 ? 1.0f:-1.0f;
        ball_movement = glm::normalize(ball_movement);
    }
    
    //touch top or bottom--change y direction
    if(3.75f - ball_position.y <= 0.25f || ball_position.y + 3.75f <= 0.25f){
        ball_movement.y = -ball_movement.y;
    }
    
    //touch either paddle -- change x direction
    if(collide_with_lp() || collide_with_rp()){
        ball_movement.x = -ball_movement.x;
        
        //to fix the overlap if speed is too fast that the ball crush into the paddle
        if(collide_with_lp()) ball_position.x = -4.25f;
        else if(collide_with_rp()) ball_position.x = 4.25f;
    }
    
    ball_position += ball_movement * ball_speed * dt;
    BallMatrix = glm::mat4(1.0f);
    BallMatrix = glm::translate(BallMatrix, ball_position);
}

void Update() {
    //game ends
    if(ball_position.x > 4.35f || ball_position.x < -4.35f){
        moving = false;
        ball_movement = glm::vec3(0);
        lp_movement = glm::vec3(0);
        rp_movement = glm::vec3(0);
        return;
    }
    
    //Delta time
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    //Ball
    Ball_Bounce(deltaTime);
    ball_speed += ball_acceleration * deltaTime;
    
    
    //Left Paddle
    lp_position += lp_movement * paddle_speed * deltaTime;
    //set boudary
    if(lp_position.y > 2.75) lp_position.y = 2.75;
    else if(lp_position.y < -2.75) lp_position.y = -2.75;
    LeftPaddleMatrix = glm::mat4(1.0f);
    LeftPaddleMatrix = glm::translate(LeftPaddleMatrix, lp_position);
    
    //Right Paddle
    rp_position += rp_movement * paddle_speed * deltaTime;
    //set boudary
    if(rp_position.y > 2.75) rp_position.y = 2.75;
    else if(rp_position.y < -2.75) rp_position.y = -2.75;
    RightPaddleMatrix = glm::mat4(1.0f);
    RightPaddleMatrix = glm::translate(RightPaddleMatrix, rp_position);
    
    paddle_speed += paddle_acceleration * deltaTime;
    
    
}

void Render() {
    float vertices_Ball[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    float texCoords_Ball[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    float vertices_Paddle[] = { -0.25, -1, 0.25, -1, 0.25, 1, -0.25, -1, 0.25, 1, -0.25, 1 };
    float texCoords_Paddle[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    //draw ball
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_Ball);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords_Ball);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(BallMatrix);
    glBindTexture(GL_TEXTURE_2D, BallTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    //draw paddles
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_Paddle);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords_Paddle);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    //left
    program.SetModelMatrix(LeftPaddleMatrix);
    glBindTexture(GL_TEXTURE_2D, LeftPaddleTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //right
    program.SetModelMatrix(RightPaddleMatrix);
    glBindTexture(GL_TEXTURE_2D, RightPaddleTextureID);
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
