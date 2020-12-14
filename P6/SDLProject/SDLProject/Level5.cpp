#include "Level5.h"

#define LEVEL5_WIDTH  31
#define LEVEL5_HEIGHT 23

#define LEVEL5_ENEMY 4
#define GEMS 7


unsigned int level5_data[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2,
        2, 0, 0, 0, 2, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2,
        2, 0, 0, 0, 2, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2,
        2, 0, 0, 0, 2, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2,
        2, 0, 0, 0, 2, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2,
        2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level5::Initialize() {
    
    GLuint mapTextureID = Util::LoadTexture("Tiles.png");
    state.map = new Map(LEVEL5_WIDTH, LEVEL5_HEIGHT, level5_data, mapTextureID, 1.0f, 4, 1);
    state.enemy_count=LEVEL5_ENEMY;
    state.gems_count= GEMS;
    state.nextScene = -1;
    state.currScene = 5;
    
    //player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2.0f, -20, 0);
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
    state.gems[0].position = glm::vec3(2, -2, 0);
    state.gems[1].position = glm::vec3(6, -6, 0);
    state.gems[2].position = glm::vec3(19, -2, 0);
    state.gems[3].position = glm::vec3(6, -10.5, 0);
    state.gems[4].position = glm::vec3(21.5, -6, 0);
    state.gems[5].position = glm::vec3(15, -10.5, 0);
    state.gems[6].position = glm::vec3(25, -20, 0);
    
    
    state.enemies = new Entity[LEVEL5_ENEMY];
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = ROTATE;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].position = glm::vec3(0, -4, 0);
    state.enemies[0].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[0].height = 7;
    state.enemies[0].width = 7;
    state.enemies[0].rotate = 0;
    state.enemies[0].enemy_data.push_back(-90);
    state.enemies[0].enemy_data.push_back(90);
    state.enemies[0].enemy_data.push_back(90); //ang
    state.enemies[0].enemy_data.push_back(3);
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = MOVE;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].position = glm::vec3(13, -10, 0);
    state.enemies[1].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[1].height = 7;
    state.enemies[1].width = 7;
    state.enemies[1].rotate = 180;
    state.enemies[1].speed = 4;
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].enemy_data.push_back(-18);
    state.enemies[1].enemy_data.push_back(-4);
    state.enemies[1].enemy_data.push_back(20);
    
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].aiType = MOVE;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].position = glm::vec3(16, -8, 0);
    state.enemies[2].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[2].height = 7;
    state.enemies[2].width = 7;
    state.enemies[2].rotate = 90;
    state.enemies[2].speed = 4;
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].enemy_data.push_back(16);
    state.enemies[2].enemy_data.push_back(26);
    state.enemies[2].enemy_data.push_back(20);
    
    state.enemies[3].entityType = ENEMY;
    state.enemies[3].aiType = ROTATE;
    state.enemies[3].aiState = IDLE;
    state.enemies[3].position = glm::vec3(26, -13, 0);
    state.enemies[3].textureID = Util::LoadTexture("Enemy.png");
    state.enemies[3].height = 7;
    state.enemies[3].width = 7;
    state.enemies[3].rotate = 0;
    state.enemies[3].enemy_data.push_back(-60);
    state.enemies[3].enemy_data.push_back(60);
    state.enemies[3].enemy_data.push_back(90); //ang
    state.enemies[3].enemy_data.push_back(9);
    
}

void Level5::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, state.map, state.enemies, LEVEL5_ENEMY, state.gems, GEMS);
    
    for(size_t i=0; i < LEVEL5_ENEMY; i++){
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL5_ENEMY, NULL, NULL);
    }
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Update(0, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    
    if(state.player->gems_collected == GEMS){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL5_ENEMY; i++) state.enemies[i].isActive = false;
        state.nextScene = state.currScene+1;
    }
    
    else if(state.player->destroy){
        state.player->isActive = false;
        for(int i = 0; i < LEVEL5_ENEMY; i++) state.enemies[i].isActive = false;
        state.player->lives--;
        if(state.player->lives == 0){
            state.nextScene = -1;
        }
        else{
            state.nextScene = state.currScene;
        }
    }
}

void Level5::Render(ShaderProgram *program) {
    
    state.map->Render(program);
    state.player->Render(program);
    
    for(size_t i=0; i < GEMS; i++){
        state.gems[i].Render(program);
    }
    
    for(size_t i=0; i < LEVEL5_ENEMY; i++){
        state.enemies[i].Render(program);
    }
    
    
}





