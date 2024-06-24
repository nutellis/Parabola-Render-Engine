#pragma once

#include <HoneycombUIFramework/BaseWidget.h>
#include <Components/RenderWindow.h>
#include <Managers/ShaderManager.h>
#include <Components/Shader.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class  HCButtonWidget : public HCBaseWidget {
public:

    HCButtonWidget() {}

    HCButtonWidget(float inX, float inY, float inWidth, float inHeight, const std::string& inLabel)
        : HCBaseWidget(inX, inY, inWidth, inHeight), Label(inLabel) {
        InitButton();
    }

    void Draw() override {

        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));

        auto shader = SHADERMANAGER.GetShader("GUIShader");
        // Draw button rectangle
        glUseProgram(shader->ID);


        //shader->SetMat4("projection", projection);
        //shader->setVec3("textColor", color.x, color.y, color.z);

        // Bind VAO and draw the button
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Draw button label (requires text rendering setup)
    }

    void HandleInput(RenderWindow* window) override {
        // Handle button input
    }

    void InitButton() {
        
        SHADERMANAGER.CreateShader("GUIShader", "H:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/buttonVertexShader.vs",
            "H:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/ButtonFragShader.fs", nullptr);


        // Vertex data for a button
        float vertices[] = {
            X, Y,                   // Bottom-left
            X + Width, Y,           // Bottom-right
            X + Width, Y + Height,  // Top-right
            X, Y,                   // Bottom-left
            X + Width, Y + Height,  // Top-right
            X, Y + Height           // Top-left
        };

        // Generate and bind VAO and VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

private:
    std::string Label;
};