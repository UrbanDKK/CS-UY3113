#include "Level2.h"

#define LEVEL2_WIDTH  16
#define LEVEL2_HEIGHT 12

#define LEVEL2_ENEMY 2
#define GEMS 1


unsigned int level2_data[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level2::Initialize() {
    
    GLuint mapTextureID = Util::LoadTexture("Tiles.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    state.enemy_count=LEVEL2_ENEMY;
    state.gems_count= GEMS;
    state.nextScene = -1;
    state.currScene = 2;
    
    //player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2.0f, -4, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("ice.png");
    state.player->height = 2;
    state.player->width = 2;
    
    
    state.gems = new Entity[GEMS];
    state.gems[0].entityType = GEM;
    state.gems[0].position = glm::vec3(13.5f, -5, 0);
    state.gems[0].textureID = Util::LoadTexture("Gem.png");
    state.gems[0].height = 2;
    state.gems[0].width = 2;
    
    state.enemies = new Entity[LEVEL2_ENEMY];
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = ROTATE;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].position = glm::vec3(8.0f, -6, 0);
    state.enemies[0].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[0].height = 7;
    state.enemies[0].width = 7;
    state.enemies[0].enemy_data.push_back(-120);
    state.enemies[0].enemy_data.push_back(60);
    state.enemies[0].enemy_data.push_back(60); //ang
    state.enemies[0].enemy_data.push_back(3);
    
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = MOVE;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].position = glm::vec3(15.0f, -6, 0);
    state.enemies[1].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[1].height = 7;
    state.enemies[1].width = 7;
    state.enemies[1].rotate = 180;
    state.enemies[1].speed = 4;
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].enemy_data.push_back(-9);
    state.enemies[1].enemy_data.push_back(-6);
    state.enemies[1].enemy_data.push_back(4);
    
    
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, state.map, state.enemies, LEVEL2_ENEMY, state.gems, GEMS);
    
    for(size_t i=0; i < LEVEL2_ENEMY; i++){
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY, NULL, NULL);
    }
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Update(0, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    
    if(state.player->gems_collected == GEMS){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL2_ENEMY; i++) state.enemies[i].isActive = false;
        state.nextScene = state.currScene+1;
    }
    
    else if(state.player->destroy){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL2_ENEMY; i++) state.enemies[i].isActive = false;
        state.player->lives--;
        if(state.player->lives == 0){
            state.nextScene = -1;
        }
        else{
            state.nextScene = state.currScene;
        }
    }
}

void Level2::Render(ShaderProgram *program) {
    
    state.map->Render(program);
    state.player->Render(program);
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Render(program);
    }
    
    for(size_t i=0; i < LEVEL2_ENEMY; i++){
        state.enemies[i].Render(program);
    }
    
    
}





