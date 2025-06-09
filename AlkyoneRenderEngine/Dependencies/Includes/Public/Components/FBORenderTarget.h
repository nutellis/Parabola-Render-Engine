#pragma once
#include <Components/RenderTarget.h>
#include <Components/Texture.h>


class FBORenderTarget : public RenderTarget
{
public:
	FBORenderTarget();

	FBORenderTarget(std::string Name, uint32 Width, uint32 Height, uint32 NumTargets = 1);

	~FBORenderTarget();

	void Bind();

	bool Init(
		TArray<RTextureOptions> ColourAttachmentOptions = TArray<RTextureOptions>(),
		RTextureOptions DepthAttachmentOptions = RTextureOptions::InitDefaultDepth());

	inline void AttachTextureToFrameBuffer(uint32 TextureRef, uint32 Channel) {
		glNamedFramebufferTexture(ID, Channel, TextureRef, 0);
	}

public:
	TArray<RTexture *> ColourAttachments;
	RTexture * DepthStencilAttachment;

private:
	uint32 NumColourTargets = 0;
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
