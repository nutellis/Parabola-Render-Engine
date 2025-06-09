#include "Utilities/PrimitiveShapes.h"


PrimitiveShape::PrimitiveShape()
{
}

PrimitiveShape::~PrimitiveShape()
{
	Data.Clear();
}


CubeShape::CubeShape() : PrimitiveShape()
{
    Data = {
        // Near Plane (z = -1.0)
        Vector4f(-1.0f, -1.0f, -1.0f, 1.0f), // Bottom-left
        Vector4f(1.0f, -1.0f, -1.0f, 1.0f),  // Bottom-right
        Vector4f(-1.0f, 1.0f, -1.0f, 1.0f),  // Top-left
        Vector4f(1.0f, 1.0f, -1.0f, 1.0f),   // Top-right

        // Far Plane (z = 1.0)
        Vector4f(-1.0f, -1.0f, 1.0f, 1.0f),  // Bottom-left
        Vector4f(1.0f, -1.0f, 1.0f, 1.0f),   // Bottom-right
        Vector4f(-1.0f, 1.0f, 1.0f, 1.0f),   // Top-left
        Vector4f(1.0f, 1.0f, 1.0f, 1.0f)     // Top-right
    };
}

CubeShape::~CubeShape()
{
}

void CubeShape::Draw()
{
}

void CubeShape::Update()
{
}

ArrowShape::ArrowShape()
{
    Data = {
		// Arrow shaft
        Vector4f(0.0f, 0.0f, 0.0f, 1.0f), // Shaft start
        Vector4f(0.0f, 1.0f, 0.0f, 1.0f), // Shaft end
        Vector4f(-0.2f, 0.5f, 0.0f, 1.0f), // Left
        Vector4f(0.0f, 1.0f, 0.0f, 1.0f),  // Tip (for left line)
        Vector4f(0.2f, 0.5f, 0.0f, 1.0f),  // Right
        Vector4f(0.0f, 1.0f, 0.0f, 1.0f)   // Tip (for right line)
    };

}

void ArrowShape::Draw()
{
    GLuint arrowVAO, arrowVBO;
    glGenVertexArrays(1, &arrowVAO);
    glGenBuffers(1, &arrowVBO);

    // Bind and upload
    glBindVertexArray(arrowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, arrowVBO);
    glBufferData(GL_ARRAY_BUFFER, Data.SizeOf(), Data.Begin(), GL_STATIC_DRAW);

    // Vertex position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind to avoid accidental modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ArrowShape::Update()
{
	// Update logic for the arrow shape can be added here
}
