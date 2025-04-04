#pragma once
#include <Components/RenderTarget.h>




class FBORenderTarget : RenderTarget
{
public:
	FBORenderTarget();

	FBORenderTarget(uint32 Width, uint32 Height);

	~FBORenderTarget();

	void Bind();

	bool Init();

	inline uint32 GetTexture() const {
		return Texture;
	}

public: 
	uint32 Width;
	uint32 Height;
private:
	uint32 Texture;
	uint32 DepthStencilRBO;


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
