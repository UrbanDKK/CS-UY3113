#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other){
    float xdist = fabs(position.x-other->position.x) - (width + other->width)/2;
    float ydist = fabs(position.y-other->position.y) - (height + other->height)/2;
    
    if (xdist < 0 && ydist < 0){
        lastCollision = other->entityType;
        isActive = false;
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

void Entity::Update(float deltaTime, Entity *platforms, int platformCount, Entity *rocks, int rocksCount)
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
    
        //add a friction whose direction is opposite to veloocity.x
        if(velocity.x < 0){
            acceleration.x += 3.0f;
        }
        else if(velocity.x > 0){
            acceleration.x -= 3.0f;
        }
    
        velocity += acceleration * deltaTime;
        position.y += velocity.y * deltaTime;
        CheckCollisionsY(rocks, rocksCount);
        if(lastCollision != ROCK){
            CheckCollisionsY(platforms, platformCount);
        }
    
        position.x += velocity.x * deltaTime;
        CheckCollisionsX(rocks, rocksCount);
        if(lastCollision != ROCK){
            CheckCollisionsX(platforms, platformCount);
        }
    
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
