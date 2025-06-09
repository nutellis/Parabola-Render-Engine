#pragma once

#ifndef RENDERHELPER
#define RENDERHELPER

#include <Core/RenderCore.h>

#include <VertexFormat.h>
#include <Components/VertexBuffers.h>
#include <Components/RenderActor.h>

namespace RenderHelper {

	inline RQuadRenderActor& GetQuadActor() {
		static RQuadRenderActor Instance;
		return Instance;
	}

    inline void SetupBuffers(VertexArrayObject & VAO, VertexBufferObject & VBO, VertexArray Vertices, PVertexComponentCount ComponentCount, uint32 DrawType)
	{
		VAO.CreateArray(ComponentCount);

		VBO.CreateBuffer(DrawType, Vertices.SizeOf(), Vertices.Begin());

		VAO.SetupAttribute(VBO.GetIndex(), 0, VAO.ComponentCount.Position, offsetof(VertexFormat, VertexFormat::Position));

		VAO.SetupAttribute(VBO.GetIndex(), 1, VAO.ComponentCount.Normal, offsetof(VertexFormat, VertexFormat::Normal));

		VAO.SetupAttribute(VBO.GetIndex(), 2, VAO.ComponentCount.UVs, offsetof(VertexFormat, VertexFormat::UVs));

		VAO.AttachVertexBuffer(VBO.GetID(), VBO.GetIndex(), sizeof(VertexFormat));

	}
};


#endif // !RENDERHELPER
