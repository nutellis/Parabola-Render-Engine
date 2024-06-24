#include <GL/glew.h>

#include <RenderEngine/Components/FBORenderTarget.h>


/*
*
* We have to attach at least one buffer(color, depth or stencil buffer).
* There should be at least one color attachment.
* All attachments should be complete as well(reserved memory).
* Each buffer should have the same number of samples.
*
*/



FBORenderTarget::FBORenderTarget()
{
	glGenFramebuffers(1, &ID);

	//glBindFramebuffer(GL_FRAMEBUFFER, ID);

}

FBORenderTarget::~FBORenderTarget()
{

	glDeleteFramebuffers(1, &ID);
}

void FBORenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,ID);
}

GLFrameBufferObject::GLFrameBufferObject(uint32 fsaa)
{
}
