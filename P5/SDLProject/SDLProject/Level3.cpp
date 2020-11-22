#include "Level3.h"

#define LEVEL3_WIDTH 30
#define LEVEL3_HEIGHT 12

#define ENEMY_COUNT 3

unsigned int level3_data[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 1, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
        2, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
        2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2

};

void Level3::Initialize() {
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    state.enemy_count = ENEMY_COUNT;
    state.currScene = 3;
    state.nextScene = -1;
    
    //player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1.0f, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 3.0f;
    state.player->jumpPower = 5.0f;
    state.player->textureID = Util::LoadTexture("Player.png");
    
    
    //enemy
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("Enemy.png");

    state.enemies[0].position = glm::vec3(2.0f, -10.0f, 0);
    state.enemies[0].aiType = WALKER;
    state.enemies[0].enemy_data = {2.0f, 10.0f, 6.0f};
    state.enemies[1].position = glm::vec3(10.0f, -8.0f, 0);
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].enemy_data = {10.0f, 17.0f, -8.0f, 6.0f};
    state.enemies[2].position = glm::vec3(22.0f, -6.0f, 0);
    state.enemies[2].aiType = PATROLLER;
    //state.enemies[2].enemy_data = {11.0f, 17.0f, 6.0f};
    
    for(size_t i=0; i < ENEMY_COUNT; i++){
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].aiState = IDLE;
        state.enemies[i].width = 1.0f;
    }
    
    state.enemies[2].textureID = Util::LoadTexture("Enemy_flying.png");
    
    
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, state.map, state.enemies, ENEMY_COUNT);
    
    for(size_t i=0; i < ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, ENEMY_COUNT);
    }
    
    if(state.player->defeated_enemy == ENEMY_COUNT){
        state.player->isActive = false;
        for(int i = 0; i < ENEMY_COUNT; i++) state.enemies[i].isActive = false;
        state.nextScene = state.currScene+1;
    }
    
    else if(state.player->destroy){
        state.player->isActive = false;
        for(int i = 0; i < ENEMY_COUNT; i++) state.enemies[i].isActive = false;
        state.player->lives--;
        if(state.player->lives == 0){
            state.nextScene = -1;
        }
        else{
            state.nextScene = state.currScene;
        }
        
    }
}

void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    
    
    for(size_t i=0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    
    state.map -> Render(program);
}





