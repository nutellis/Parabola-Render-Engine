#include <GL/glew.h>

#include <Components/FBORenderTarget.h>
#include <Components/Texture.h>
#include <string>


/*
*
* We have to attach at least one buffer(color, depth or stencil buffer).
* There should be at least one color attachment.
* All attachments should be complete as well(reserved memory).
* Each buffer should have the same number of samples.
*
*/



FBORenderTarget::FBORenderTarget() : RenderTarget()
{
	ColourAttachment = 0;
	DepthStencilAttachment = 0;
	Width = 0;
	Height = 0;

}

FBORenderTarget::FBORenderTarget(std::string Name,uint32 Width, uint32 Height) : RenderTarget(Name, Width, Height)
{
	ColourAttachment = 0;
	DepthStencilAttachment = 0;
}

FBORenderTarget::~FBORenderTarget()
{

	glDeleteFramebuffers(1, &ID);
}

void FBORenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,ID);
}

bool FBORenderTarget::Init()
{
	bool bIsCompleted = false;

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	ColourAttachment = new Texture(Width, Height);
	ColourAttachment->GenerateColourTexture();

	// attach to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColourAttachment->TextureID, 0);

	DepthStencilAttachment = new Texture(Width, Height);
	DepthStencilAttachment->GenerateDepthTexture();
	
	// attach to framebuffer
	glNamedFramebufferTexture(ID, GL_DEPTH_ATTACHMENT, DepthStencilAttachment->TextureID, 0);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		bIsCompleted = true;
	}
	else {
		
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bIsCompleted;
}

uint32 FBORenderTarget::GetTexture() const
{
	return ColourAttachment->TextureID;

}
uint32 FBORenderTarget::GetDepth() const
{
	return DepthStencilAttachment->TextureID;

}


GLFrameBufferObject::GLFrameBufferObject(uint32 fsaa)
{
}
