#include <GL/glew.h>

#include <Components/FBORenderTarget.h>


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
	Texture = 0;
	DepthStencilRBO = 0;

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

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// attach to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);

	glGenRenderbuffers(1, &DepthStencilRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720); //TODO: Fix dimensions
	
	// attach to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		bIsCompleted = true;
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bIsCompleted;
}

GLFrameBufferObject::GLFrameBufferObject(uint32 fsaa)
{
}
