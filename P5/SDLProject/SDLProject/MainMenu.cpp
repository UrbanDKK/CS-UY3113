#include "MainMenu.h"

void MainMenu::Initialize() {
    state.currScene = 0;
    state.nextScene = -1;
    state.background = new Entity();
    state.background->position = glm::vec3(0);
    state.background->entityType = BACKGROUND;
    state.background->textureID = Util::LoadTexture("Background.png");
}

void MainMenu::Update(float deltaTime) {
    state.background->Update(0, NULL, NULL, NULL, NULL);
}

void MainMenu::Render(ShaderProgram *program) {
    
    program->SetModelMatrix(state.background->modelMatrix);
    float vertices[]  = { -8, -6, 8, -6, 8, 6, -8, -6, 8, 6, -8, 6 };
    
    //float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, state.background->textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}





