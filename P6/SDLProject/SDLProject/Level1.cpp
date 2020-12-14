#include "Level1.h"

#define LEVEL1_WIDTH  18
#define LEVEL1_HEIGHT 12

#define LEVEL1_ENEMY 0
#define GEMS 1


unsigned int level1_data[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {
    
    GLuint mapTextureID = Util::LoadTexture("Tiles.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.enemy_count=LEVEL1_ENEMY;
    state.gems_count= GEMS;
    state.nextScene = -1;
    state.currScene = 1;
    
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
    state.gems[0].position = glm::vec3(15.0f, -4, 0);
    state.gems[0].textureID = Util::LoadTexture("Gem.png");
    state.gems[0].height = 2;
    state.gems[0].width = 2;
    
    
    
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, state.map, state.enemies, LEVEL1_ENEMY, state.gems, GEMS);
    
    for(size_t i=0; i < LEVEL1_ENEMY; i++){
        //state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY);
    }
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Update(0, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    
    if(state.player->gems_collected == GEMS){
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
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Render(program);
    }
    
    for(size_t i=0; i < LEVEL1_ENEMY; i++){
        state.enemies[i].Render(program);
    }
    
    
}





