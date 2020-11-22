#include "Level1.h"

#define LEVEL1_WIDTH  22
#define LEVEL1_HEIGHT 12

#define LEVEL1_ENEMY 1

unsigned int level1_data[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.enemy_count=LEVEL1_ENEMY;
    state.nextScene = -1;
    state.currScene = 1;
    
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
    state.enemies = new Entity[LEVEL1_ENEMY];
    GLuint enemyTextureID = Util::LoadTexture("Enemy.png");

    state.enemies[0].position = glm::vec3(12.0f, -5.0f, 0);
    state.enemies[0].aiType = WALKER;
    state.enemies[0].enemy_data = {11.0f, 17.0f, 6.0f};
    
    for(size_t i=0; i < LEVEL1_ENEMY; i++){
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].aiState = IDLE;
        state.enemies[i].width = 1.0f;
    }
    
    
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, state.map, state.enemies, LEVEL1_ENEMY);
    
    for(size_t i=0; i < LEVEL1_ENEMY; i++){
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY);
    }
    
    if(state.player->defeated_enemy == LEVEL1_ENEMY){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL1_ENEMY; i++) state.enemies[i].isActive = false;
        state.nextScene = state.currScene+1;
    }
    
    else if(state.player->destroy){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL1_ENEMY; i++) state.enemies[i].isActive = false;
        state.player->lives--;
        if(state.player->lives == 0){
            state.nextScene = -1;
        }
        else{
            state.nextScene = state.currScene;
        }
    }
}

void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    
    
    for(size_t i=0; i < LEVEL1_ENEMY; i++){
        state.enemies[i].Render(program);
    }
    
    state.map -> Render(program);
}





