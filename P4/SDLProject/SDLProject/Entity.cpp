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
            }
            else{
                isActive = false;
                destroy = true;
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
               position.y += penetrationY; velocity.y = 0;
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
           else if (velocity.x < 0) {
               position.x += penetrationX; velocity.x = 0;
           }
       }
   }
}

void Entity::Update(float deltaTime, Entity *player, Entity *platforms, int platformCount, Entity *enemies, int enemyCount)
{
    if(!isActive) return;
    
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
        CheckCollisionsY(enemies, enemyCount);
        CheckCollisionsY(platforms, platformCount);
        
    
        position.x += velocity.x * deltaTime;
        CheckCollisionsX(enemies, enemyCount);
        CheckCollisionsX(platforms, platformCount);
    }
    
    else if(entityType == ENEMY){
        if(!player->isActive) return;
        AI(player);
        velocity += acceleration * deltaTime;
        position.y += velocity.y * deltaTime;
        CheckCollisionsY(enemies, enemyCount);
        CheckCollisionsY(platforms, platformCount);
        
        position.x += velocity.x * deltaTime;
        CheckCollisionsX(enemies, enemyCount);
        CheckCollisionsX(platforms, platformCount);
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
            AI_Walker(player);
            break;
        case(JUMPER):
            AI_Jumper(player);
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

void Entity::AI_Walker(Entity *player){
    switch(aiState){
        case(IDLE):
            if(DetectDist(player, 5.0f)) aiState = WALKING;
            break;
        case(WALKING):
            if(velocity.x == 0) velocity.x = rand()%2 == 0 ? 1:-1;
            if(position.x < 3.5f) velocity.x = 1;
            else if (position.x > 6.5f) velocity.x = -1;
            if(!DetectDist(player, 5.0f)) aiState = IDLE;
            break;
    }
}

void Entity::AI_Jumper(Entity *player){
    acceleration.y = -9.81f;
    switch(aiState){
        case(IDLE):
            if(velocity.x == 0) velocity.x = rand()%2 == 0 ? 1:-1;
            if(position.x < 2.5f) velocity.x = 1;
            else if (position.x > 6.5f) velocity.x = -1;
            if(DetectDist(player, 5.0f)) aiState = JUMPING;
            break;
            
        case(JUMPING):
            if(velocity.x == 0) velocity.x = rand()%2 == 0 ? 1:-1;
            if(position.y <= -4.5f){
                velocity.y = rand()%5 + 4;
                if(position.x < 3.5f) velocity.x = 1;
                else if (position.x > 6.5f) velocity.x = -1;
            }
            if(!DetectDist(player, 5.0f)) aiState = IDLE;
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
            if(!DetectDist(player, 3.0f)) aiState = IDLE;
            break;
    }
}
