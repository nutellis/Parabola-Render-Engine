#pragma once
#include <Components/RenderTarget.h>


class Texture;


class FBORenderTarget : public RenderTarget
{
public:
	FBORenderTarget();

	FBORenderTarget(std::string Name, uint32 Width, uint32 Height);

	~FBORenderTarget();

	void Bind();

	bool Init(bool IsForShadowMapping = false);


	uint32 GetTexture() const;
	uint32 GetDepth() const;

	inline void AttachTextureToFrameBuffer(uint32 TextureRef, uint32 Channel) {
		glNamedFramebufferTexture(ID, Channel, TextureRef, 0);
	}

public:
	Texture * ColourAttachment;
	Texture * DepthStencilAttachment;

};

class GLFrameBufferObject
{
public:
	GLFrameBufferObject(uint32 fsaa);
	~GLFrameBufferObject();

	uint32 FBO;

	//bool bind(bool recreateIfNeeded);

	///** Swap buffers - only useful when using multisample buffers.
	//*/
	//void swapBuffers();

	///** This function acts very similar to @see GLFBORenderTexture::attachDepthBuffer
	//	The difference between D3D & OGL is that D3D setups the DepthBuffer before rendering,
	//	while OGL setups the DepthBuffer per FBO. So the DepthBuffer (RenderBuffer) needs to
	//	be attached for OGL.
	//*/
	////void attachDepthBuffer(DepthBuffer *depthBuffer);
	//void detachDepthBuffer();

private:
	//GLSurfaceDesc mMultisampleColourBuffer;

};
