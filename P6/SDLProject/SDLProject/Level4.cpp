#include "Level4.h"

#define LEVEL4_WIDTH  25
#define LEVEL4_HEIGHT 18

#define LEVEL4_ENEMY 2
#define GEMS 4


unsigned int level4_data[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level4::Initialize() {
    
    GLuint mapTextureID = Util::LoadTexture("Tiles.png");
    state.map = new Map(LEVEL4_WIDTH, LEVEL4_HEIGHT, level4_data, mapTextureID, 1.0f, 4, 1);
    state.enemy_count=LEVEL4_ENEMY;
    state.gems_count= GEMS;
    state.nextScene = -1;
    state.currScene = 4;
    
    //player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2.0f, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("ice.png");
    state.player->height = 2;
    state.player->width = 2;
    
    
    state.gems = new Entity[GEMS];
    
    for(int i = 0; i < GEMS; i++){
        state.gems[i].entityType = GEM;
        state.gems[i].textureID = Util::LoadTexture("Gem.png");
        state.gems[i].height = 2;
        state.gems[i].width = 2;
    }
    state.gems[0].position = glm::vec3(2.5f, -14.5f, 0);
    state.gems[1].position = glm::vec3(7.5f, -9.0f, 0);
    state.gems[2].position = glm::vec3(16.5f, -7.0f, 0);
    state.gems[3].position = glm::vec3(21.5f, -2.0f, 0);
    
    state.enemies = new Entity[LEVEL4_ENEMY];
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = ROTATE;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].position = glm::vec3(10.0f, -5.0f, 0);
    state.enemies[0].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[0].height = 7;
    state.enemies[0].width = 7;
    state.enemies[0].rotate = 45;
    state.enemies[0].enemy_data.push_back(-60);
    state.enemies[0].enemy_data.push_back(90);
    state.enemies[0].enemy_data.push_back(90); //ang
    state.enemies[0].enemy_data.push_back(3);
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = MOVE;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].position = glm::vec3(5.0f, -17.0f, 0);
    state.enemies[1].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[1].height = 7;
    state.enemies[1].width = 7;
    state.enemies[1].rotate = 90;
    state.enemies[1].speed = 4;
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].enemy_data.push_back(3);
    state.enemies[1].enemy_data.push_back(21);
    state.enemies[1].enemy_data.push_back(20);
    
    
}

void Level4::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, state.map, state.enemies, LEVEL4_ENEMY, state.gems, GEMS);
    
    for(size_t i=0; i < LEVEL4_ENEMY; i++){
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL4_ENEMY, NULL, NULL);
    }
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Update(0, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    
    if(state.player->gems_collected == GEMS){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL4_ENEMY; i++) state.enemies[i].isActive = false;
        state.nextScene = state.currScene+1;
    }
    
    else if(state.player->destroy){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL4_ENEMY; i++) state.enemies[i].isActive = false;
        state.player->lives--;
        if(state.player->lives == 0){
            state.nextScene = -1;
        }
        else{
            state.nextScene = state.currScene;
        }
    }
}

void Level4::Render(ShaderProgram *program) {
    
    state.map->Render(program);
    state.player->Render(program);
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Render(program);
    }
    
    for(size_t i=0; i < LEVEL4_ENEMY; i++){
        state.enemies[i].Render(program);
    }
    
    
}





