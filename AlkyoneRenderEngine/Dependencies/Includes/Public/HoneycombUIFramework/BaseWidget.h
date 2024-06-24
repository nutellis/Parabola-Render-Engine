#pragma once


class RenderWindow;

class HCBaseWidget { // : PObject {

public:
    HCBaseWidget(float InX, float InY, float InWidth, float InHeight)
        : X(InX), Y(InY), Width(InWidth), Height(InHeight) {}

    virtual void Draw() = 0;
    virtual void HandleInput(RenderWindow* window) = 0;

protected:
    float X, Y, Width, Height;

    GLuint VAO, VBO;
    GLuint ShaderProgram;
public:

    HCBaseWidget() = default;
};