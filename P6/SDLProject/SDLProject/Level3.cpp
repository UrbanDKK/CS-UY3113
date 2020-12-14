#include "Level3.h"

#define LEVEL3_WIDTH  21
#define LEVEL3_HEIGHT 14

#define LEVEL3_ENEMY 2
#define GEMS 3


unsigned int level3_data[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level3::Initialize() {
    
    GLuint mapTextureID = Util::LoadTexture("Tiles.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    state.enemy_count=LEVEL3_ENEMY;
    state.gems_count= GEMS;
    state.nextScene = -1;
    state.currScene = 3;
    
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
    state.gems[0].position = glm::vec3(17.5f, -2.5f, 0);
    state.gems[1].position = glm::vec3(2.0f, -7.5f, 0);
    state.gems[2].position = glm::vec3(18.0f, -8.5f, 0);
    
    state.enemies = new Entity[LEVEL3_ENEMY];
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = ROTATE;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].position = glm::vec3(8.0f, -8.0f, 0);
    state.enemies[0].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[0].height = 7;
    state.enemies[0].width = 7;
    state.enemies[0].rotate = -90;
    state.enemies[0].enemy_data.push_back(-180);
    state.enemies[0].enemy_data.push_back(45);
    state.enemies[0].enemy_data.push_back(90); //ang
    state.enemies[0].enemy_data.push_back(3);
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = MOVE;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].position = glm::vec3(6.0f, 0.0f, 0);
    state.enemies[1].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[1].height = 7;
    state.enemies[1].width = 7;
    state.enemies[1].rotate = -90;
    state.enemies[1].speed = 4;
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].enemy_data.push_back(6);
    state.enemies[1].enemy_data.push_back(17);
    state.enemies[1].enemy_data.push_back(10);
    
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, state.map, state.enemies, LEVEL3_ENEMY, state.gems, GEMS);
    
    for(size_t i=0; i < LEVEL3_ENEMY; i++){
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL3_ENEMY, NULL,NULL);
    }
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Update(0, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    
    if(state.player->gems_collected == GEMS){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL3_ENEMY; i++) state.enemies[i].isActive = false;
        state.nextScene = state.currScene+1;
    }
    
    else if(state.player->destroy){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL3_ENEMY; i++) state.enemies[i].isActive = false;
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
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Render(program);
    }
    
    for(size_t i=0; i < LEVEL3_ENEMY; i++){
        state.enemies[i].Render(program);
    }
    
    
}





