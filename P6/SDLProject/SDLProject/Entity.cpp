#include "Entity.h"
#include <iostream>
using namespace std;
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

bool arc_in_square(glm::vec3 point, glm::vec3 circle, glm::vec3 square, float width, float rotate){
    float angle_high = rotate + 30;
    float angle_low = rotate - 30;
    
    float angle = atan2(point.y-circle.y, point.x-circle.x) *180/3.1415926;
    if(point.x < circle.x && point.y>circle.y) angle += 180;
    else if(point.y < circle.y && point.x < circle.x) angle -= 180;
    //if(angle<-180) angle +=360;
    
    if(point.x<=square.x+width/2 && point.x>=square.x-width/2 && point.y<=square.y+width/2 && point.y>=square.y-width/2 && angle<=angle_high && angle >= angle_low){
        //cout << 'a';
        //cout << point.x <<' ' << point.y << ' ' << angle << circle.x << ' ' << circle.y << endl;
        return true;
    }
    else{
        ////cout << point.x <<' ' << point.y << ' ' << angle <<endl;
        return false;
    }
}

bool line_in_square(glm::vec3 circle, float angle, glm::vec3 square, float width){
    float a = tan(glm::radians(angle));
    float x1 = square.x- width/2;
    float y1 = a*(x1-circle.x)+circle.y;
    if(angle <= 90 && angle >= -90){
        if(x1 >= circle.x)
            if(y1 <= square.y+width/2 && y1 >= square.y - width/2 && ((y1-circle.y)*(y1-circle.y)+(x1-circle.x)*(x1-circle.x)<=49/4)) {
                ////cout << "z";
                return true;
            }
    }
    
    else{
        if(x1 <= circle.x){
            if(y1 <= square.y+width/2 && y1 >= square.y - width/2){
                //cout << "b";
                if((y1-circle.y)*(y1-circle.y)+(x1-circle.x)*(x1-circle.x)<=49/4) {
                    //cout << x1 << ' ' << y1 << ' ' << square.x << ' '<< square.y<< endl;
                    return true;
                }
            }
        }
    }
    
    x1 = square.x+width/2;
    y1 = a*(x1-circle.x)+circle.y;
    if(angle <= 90 && angle >= -90){
        if(x1 >= circle.x){
            // ////cout << angle;
            ////cout << x1 << ' ' << y1 << ' ' << angle <<endl;
            if(y1 <= square.y+width/2 && y1 >= square.y - width/2){
                ////cout <<"d";
                if((y1-circle.y)*(y1-circle.y)+(x1-circle.x)*(x1-circle.x)<=49/4){
                    ////cout << "c";
                    return true;
                }
            }
        }
    }
    else{
        if(x1 <= circle.x ){
            if(y1 <= square.y+width/2 && y1 >= square.y - width/2 && ((y1-circle.y)*(y1-circle.y)+(x1-circle.x)*(x1-circle.x)<=49/4)) {
                ////cout << "z";
                return true;
            }
        }
    }
    
    y1 = square.y+width/2;
    x1 = (y1-circle.y)/a +circle.x;
    if(angle <= 90 && angle >= -90){
        if(x1 >= circle.x){
            if(x1 <= square.x+width/2 && x1 >= square.x - width/2 && ((y1-circle.y)*(y1-circle.y)+(x1-circle.x)*(x1-circle.x)<=49/4)) {
                ////cout << "d";
                return true;
            }
        }
    }
    else{
        if(x1 <= circle.x){
            if(x1 <= square.x+width/2 && x1 >= square.x - width/2 && ((y1-circle.y)*(y1-circle.y)+(x1-circle.x)*(x1-circle.x)<=49/4)) {
                ////cout << "e";
                return true;
            }
        }
    }
    
    y1 = square.y-width/2;
    x1 = (y1-circle.y)/a +circle.x;
    if(angle <= 90 && angle >= -90){
        if(x1 >= circle.x)
            if(x1 <= square.x+width/2 && x1 >= square.x - width/2){
                //cout << "a";
                
                if((y1-circle.y)*(y1-circle.y)+(x1-circle.x)*(x1-circle.x)<=49/4) {
                    ////cout << angle;
                    //cout << x1 << ' ' << y1 << ' ' << square.x << ' '<< square.y<< endl;
                    return true;
                }
            }
    }
    else{
        if(x1 <= circle.x)
            if(x1 <= square.x+width/2 && x1 >= square.x - width/2 && ((y1-circle.y)*(y1-circle.y)+(x1-circle.x)*(x1-circle.x)<=49/4)) {
                //cout << "g";
                return true;
            }
    }
    
    return false;
    
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
        //lastCollision = other->entityType;
        if(other->entityType == GEM){
            
            other->destroy = true;
            //lastCollision = PLAYER;
            gems_collected++;
            goal = true;
            
            /*
            else{
                isActive = false;
                destroy = true;
                played_failure = true;
            }
             */
        }
        else if(other->entityType == ENEMY){
            float cx = other->position.x;
            float cy = other->position.y;
            float y_up = position.y + height/2-cy;
            float y_down = position.y - height/2-cy;
            float x_left = position.x - width/2-cx;
            float x_right = position.x + width/2-cx;
            
            
            //collide with arc
            glm::vec3 point = glm::vec3(x_left, -sqrt(49/4-(x_left*x_left)), 0)+other->position;
            if(arc_in_square(point, other->position, position, width, other->rotate) ) return true;
            point = glm::vec3(x_left, sqrt(49/4-x_left*x_left), 0)+other->position;
            if(arc_in_square(point, other->position, position, width, other->rotate)) return true;
            point = glm::vec3(x_right, -sqrt(49/4-x_right*x_right), 0)+other->position;
            if(arc_in_square(point, other->position, position, width, other->rotate)) return true;
            point = glm::vec3(x_right, sqrt(49/4-x_right*x_right), 0)+other->position;
            if(arc_in_square(point, other->position, position, width, other->rotate)) return true;
            point = glm::vec3(-sqrt(49/4-y_down*y_down), y_down, 0)+other->position;
            if(arc_in_square(point, other->position, position, width, other->rotate)) return true;
            point = glm::vec3(sqrt(49/4-y_down*y_down), y_down, 0)+other->position;
            if(arc_in_square(point, other->position, position, width, other->rotate)) return true;
            point = glm::vec3(-sqrt(49/4-y_up*y_up), y_up, 0)+other->position;
            if(arc_in_square(point, other->position, position, width, other->rotate)) return true;
            point = glm::vec3(sqrt(49/4-y_up*y_up), y_up, 0)+other->position;
            if(arc_in_square(point, other->position, position, width, other->rotate)) return true;
            
            
            
            float angle = other->rotate + 30;
            if(line_in_square(other->position, angle, position, width)){
                ////cout << 'b';
                return true;
            }
            angle = other->rotate - 30;
            if(line_in_square(other->position, angle, position, width)){
                ////cout << 'c';
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}

void Entity::Spotted(Entity *objects, int objectCount){
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];
        if(CheckCollision(object)){
            isActive = false;
            destroy = true;
            played_failure = true;
            return;
        }
    }
    
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

    if (map->Tile(top, &penetration_x, &penetration_y)==2 && movement.y > 0) {
        position.y -= penetration_y;
        movement.y = 0;
        collidedTop = true;
    }
    
    else if (map->Tile(top_left, &penetration_x, &penetration_y)==2 && movement.y > 0) {
        position.y -= penetration_y;
        movement.y = 0;
        collidedTop = true;
    }
    
    else if (map->Tile(top_right, &penetration_x, &penetration_y)==2 && movement.y > 0) {
        if(fabs(floor(position.x)-position.x+0.5)< 0){
            
            position.y -= penetration_y;
            movement.y = 0;
            collidedTop = true;
            
        }
    }
    
    if (map->Tile(bottom, &penetration_x, &penetration_y)==2 && movement.y < 0) {
        position.y += penetration_y;
        movement.y = 0;
        collidedBottom = true;
        
    }
    
    else if (map->Tile(bottom_left, &penetration_x, &penetration_y)==2 && movement.y < 0) {
        position.y += penetration_y;
        movement.y = 0;
        collidedBottom = true;
    }
    
    else if (map->Tile(bottom_right, &penetration_x, &penetration_y)==2 && movement.y < 0) {
        if(floor(position.x) != position.x-0.5f){
            position.y += penetration_y;
            movement.y = 0;
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

    if (map->Tile(left, &penetration_x, &penetration_y) == 2&& movement.x < 0) {
        
            position.x += penetration_x;
            movement.x = 0;
            collidedLeft = true;
        
    }
    
    else if(collidedTop == false){
        glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
        glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
        if (map->Tile(top_left, &penetration_x, &penetration_y)==2) {
            
            position.x += penetration_x;
            movement.x = 0;
            collidedLeft = true;
        }
        else if (map->Tile(top_right, &penetration_x, &penetration_y)==2) {
            
            //if(floor(position.x) != position.x-0.5){
                position.x -= penetration_x;
                movement.x = 0;
                collidedLeft = true;
            //}
        }
    }
     
    
    if (map->Tile(right, &penetration_x, &penetration_y)==2 && movement.x > 0) {
        //cout << "left" << endl;
        //if(ceil(position.y)!=position.y+0.5){
            position.x -= penetration_x;
            movement.x = 0;
            collidedRight = true;
        //}
        
    }
    
    
    else if(collidedBottom == false){
        glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
        glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
        
        
        if (map->Tile(bottom_left, &penetration_x, &penetration_y)==2) {
            if(ceil(position.y)!=position.y+0.5){
                position.x += penetration_x;
                movement.x = 0;
                collidedRight = true;
            }
        }
        else if (map->Tile(bottom_right, &penetration_x, &penetration_y)==2) {
            if(floor(position.x) != position.x-0.5 && ceil(position.y)!=position.y+0.5){
                position.x -= penetration_x;
                movement.x = 0;
                collidedRight = true;
            }
            
        }
    }
     
}

bool Entity::CheckWarm(Map *map){
    
    float x, y;
    glm::vec3 point;
    point = glm::vec3(position.x, position.y + (height / 2), position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    point = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    point = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    point = glm::vec3(position.x, position.y - (height / 2), position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    point = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    point = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    point = glm::vec3(position.x, position.y, position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    point = glm::vec3(position.x - (width / 2), position.y, position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    point = glm::vec3(position.x + (width / 2), position.y, position.z);
    if(map->Tile(point, &x, &y)==1) return true;
    return false;
}

void Entity::Update(float deltaTime, Entity *player, Map *map, Entity *enemies, int enemyCount, Entity *gems, int gemCount)
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
    
        
    
        position.y += movement.y * speed * deltaTime;
        CheckCollisionsY(map);
        CheckCollisionsY(gems, gemCount);
        
        
        position.x += movement.x * speed * deltaTime;
        CheckCollisionsX(map);
        CheckCollisionsX(gems, gemCount);
        
        Spotted(enemies, enemyCount);
        
        if(CheckWarm(map)){
            warm += deltaTime;
            if(warm > 3.0f){
                isActive = false;
                destroy = true;
                played_failure = true;
            }
        }
        else warm = 0;

    }
    
    else if(entityType == ENEMY){
        if(!player->isActive) return;
        AI(player);
        //velocity += acceleration * deltaTime;
        position += movement*speed * deltaTime;
        rotate += angular * deltaTime;
        
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
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
    
    float a = width/2;
    
    float vertices[]  = { -a, -a, a, -a, a, a, -a, -a, a, a, -a, a };
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
        case(ROTATE):
            AI_Rotate(player, enemy_data[0], enemy_data[1], enemy_data[2], enemy_data[3]);
            break;
        case(MOVE):
            AI_Move(player, enemy_data[0], enemy_data[1], enemy_data[2]);
            break;
    }
}

bool Entity::DetectDist(Entity *player, float d){
    float xdist = fabs(position.x-player->position.x) - (width + player->width)/2;
    float ydist = fabs(position.y-player->position.y) - (height + player->height)/2;
    
    return (xdist<d && ydist <d);
}

void Entity::AI_Rotate(Entity *player, float left, float right, float ang, float Dist){
    switch(aiState){
        case(IDLE):
            movement = glm::vec3(0);
            angular = 0;
            if(DetectDist(player, Dist)) aiState = ROTATING;
            break;
        case(ROTATING):
            if(angular == 0) angular = rand()%2 == 0 ? ang:-ang;
            if(rotate <= left) angular = -angular;
            else if(rotate >= right) angular = -angular;
            
            if(!DetectDist(player, Dist)) aiState = IDLE;
            break;
    }
}

void Entity::AI_Move(Entity *player, float left, float right, float Dist){
    
    switch(aiState){
        case(IDLE):
            movement = glm::vec3(0);
            if(DetectDist(player, Dist)) aiState = MOVING;
            break;
            
        case(MOVING):
            if(movement == glm::vec3(0)){
                if(int(rotate) % 180 == 0) movement.y = 1;
                else if(int(rotate) % 180 == 90 || int(rotate) % 180 == -90) movement.x = 1;
                movement *= rand()%2 == 0 ? 1:-1;
            }
            
            float p;
            if(int(rotate) % 180 == 0) p = position.y;
            else if(int(rotate) % 180 == 90 || int(rotate) % 180 == -90) p = position.x;
            
            if(p < left || p > right) movement = -movement;
            if(!DetectDist(player, Dist)) aiState = IDLE;
            break;
            
    }
}
