#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    destroy = false;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::JumpOn(Entity *other){
    return(fabs(position.x - other->position.x) < fabs(position.y - other->position.y)) && (position.y > other->position.y);
}

bool Entity::CheckCollision(Entity *other){
    if(other->destroy) return false;
    if(position == other->position) return false;
    
    float xdist = fabs(position.x-other->position.x) - (width + other->width)/2;
    float ydist = fabs(position.y-other->position.y) - (height + other->height)/2;
    
    if (xdist < 0 && ydist < 0){
        if(entityType!=PLAYER){
            return true;
        }
        lastCollision = other->entityType;
        if(other->entityType == ENEMY){
            if(JumpOn(other)){
                other->destroy = true;
                lastCollision = PLAYER;
                defeated_enemy++;
                goal = true;
            }
            else{
                isActive = false;
                destroy = true;
                played_failure = true;
            }
        }
        
        return true;
    }
    return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
   for (int i = 0; i < objectCount; i++)
   {
       Entity *object = &objects[i];
       if (CheckCollision(object)){
           float ydist = fabs(position.y - object->position.y);
           float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
           if (velocity.y > 0) {
               position.y -= penetrationY;
               velocity.y = 0;
               
           }
           else if (velocity.y < 0) {
               position.y += penetrationY;
               velocity.y = 0;
           }
       }
   }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
   for (int i = 0; i < objectCount; i++)
   {
       Entity *object = &objects[i];
       if (CheckCollision(object)){
           float xdist = fabs(position.x - object->position.x);
           float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
           
           if (velocity.x > 0) {
               position.x -= penetrationX;
               velocity.x = 0;
               
           }
           else if (velocity.x <= 0) {
               position.x += penetrationX;
               velocity.x = 0;
           }
       }
   }
}


void Entity::CheckCollisionsY(Map *map){
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        if(floor(position.x) != position.x){
            position.y -= penetration_y;
            velocity.y = 0;
            collidedTop = true;
        }
    }
    
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        if(floor(position.x) != position.x){
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
    }
}

void Entity::CheckCollisionsX(Map *map){
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }
    else if(collidedTop == false){
        glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
        glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
        if (map->IsSolid(top_left, &penetration_x, &penetration_y)) {
            position.x += penetration_x;
            velocity.x = 0;
            collidedLeft = true;
        }
        else if (map->IsSolid(top_right, &penetration_x, &penetration_y)) {
            
            if(floor(position.x) != position.x){
                position.x -= penetration_x;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
    
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
    else if(collidedBottom == false){
        glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
        glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
        
        if (map->IsSolid(bottom_left, &penetration_x, &penetration_y)) {
            position.x += penetration_x;
            velocity.x = 0;
            collidedRight = true;
        }
        else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y)) {
            position.x -= penetration_x;
            velocity.x = 0;
            collidedRight = true;
        }
    }
}

void Entity::Update(float deltaTime, Entity *player, Map *map, Entity *enemies, int enemyCount)
{
    if(!isActive) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if (entityType == PLAYER) {
        if (animIndices != NULL) {
            if (glm::length(movement) != 0) {
                animTime += deltaTime;

                if (animTime >= 0.25f)
                {
                    animTime = 0.0f;
                    animIndex++;
                    if (animIndex >= animFrames)
                    {
                        animIndex = 0;
                    }
                }
            }
            else {
                animIndex = 0;
            }
        }
    
        if(velocity.x < -3){
            acceleration.x += 12.0f;
        }
        else if(velocity.x > 3){
            acceleration.x -= 12.0f;
        }
        else{
            acceleration.x = 0;
            velocity.x = 0;
        }
        
        if(jump){
            jump = false;
            velocity.y = jumpPower;
        }
        
        velocity += movement.x * speed * deltaTime;
        velocity += acceleration * deltaTime;
    
        position.y += velocity.y * deltaTime;
        CheckCollisionsY(map);
        CheckCollisionsY(enemies, enemyCount);
        
        
        position.x += velocity.x * deltaTime;
        CheckCollisionsX(map);
        CheckCollisionsX(enemies, enemyCount);
        
    }
    
    else if(entityType == ENEMY){
        if(!player->isActive) return;
        AI(player);
        velocity += acceleration * deltaTime;
        position.y += velocity.y * deltaTime;
        CheckCollisionsY(enemies, enemyCount);
        CheckCollisionsY(map);
        
        
        position.x += velocity.x * deltaTime;
        CheckCollisionsX(enemies, enemyCount);
        CheckCollisionsX(map);
        
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if(destroy && entityType!=PLAYER) return;
    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::AI(Entity *player){
    switch(aiType){
        case(WALKER):
            AI_Walker(player, enemy_data[0], enemy_data[1], enemy_data[2]);
            break;
        case(JUMPER):
            AI_Jumper(player, enemy_data[0], enemy_data[1], enemy_data[2], enemy_data[3]);
            break;
        case(PATROLLER):
            AI_Patroller(player);
            break;
    }
}

bool Entity::DetectDist(Entity *player, float d){
    float xdist = fabs(position.x-player->position.x) - (width + player->width)/2;
    float ydist = fabs(position.y-player->position.y) - (height + player->height)/2;
    
    return (xdist<d && ydist <d);
}

void Entity::AI_Walker(Entity *player, float left, float right, float Dist){
    acceleration.y = -9.81f;
    switch(aiState){
        case(IDLE):
            velocity = glm::vec3(0);
            if(DetectDist(player, Dist)) aiState = WALKING;
            break;
        case(WALKING):
            if(velocity.x == 0) velocity.x = rand()%2 == 0 ? 2:-2;
            if(position.x < left) velocity.x = 2;
            else if (position.x > right) velocity.x = -2;
            if(!DetectDist(player, Dist)) aiState = IDLE;
            break;
    }
}

void Entity::AI_Jumper(Entity *player, float left, float right, float bottom, float Dist){
    acceleration.y = -9.81f;
    switch(aiState){
        case(IDLE):
            if(velocity.x == 0) velocity.x = rand()%2 == 0 ? 3:-3;
            if(position.x < left) velocity.x = 3;
            else if (position.x > right) velocity.x = -3;
            if(DetectDist(player, Dist)) aiState = JUMPING;
            break;
            
        case(JUMPING):
            if(velocity.x == 0) velocity.x = rand()%2 == 0 ? 3:-3;
            if(position.y <= bottom){
                velocity.y = rand()%4 + 4;
                if(position.x < left) velocity.x = 3;
                else if (position.x > right) velocity.x = -3;
            }
            //if(position.y >= -1.0f) position.y = -1.0f;
            if(!DetectDist(player, Dist)) aiState = IDLE;
            break;
            
    }
}

void Entity::AI_Patroller(Entity *player){
    switch(aiState){
        case(IDLE):
            if(DetectDist(player, 3.0f)) aiState = PATROLLING;
            break;
            
        case(PATROLLING):
            velocity = glm::normalize(player->position - position) * speed;
            //acceleration = glm::normalize(player->position - position);
            speed = 5.0f;
            if(!DetectDist(player, 12.0f)) aiState = IDLE;
            break;
    }
}
