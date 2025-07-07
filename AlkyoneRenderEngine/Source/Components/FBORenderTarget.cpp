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
	ColourAttachments = 0;
	DepthStencilAttachment = 0;
	Width = 0;
	Height = 0;

}

FBORenderTarget::FBORenderTarget(std::string Name,uint32 Width, uint32 Height, uint32 NumTargets) : RenderTarget(Name, Width, Height)
{
	ColourAttachments = TArray<RTexture *>(NumTargets);
	for (uint32 i = 0; i < NumTargets; i++)
	{
		ColourAttachments.PushBack(new RTexture(Width, Height, RTextureOptions::InitDefault()));
	}

	DepthStencilAttachment = new RTexture(Width, Height, RTextureOptions::InitDefaultDepth());
}

FBORenderTarget::~FBORenderTarget()
{
	ColourAttachments.Clear();

	delete DepthStencilAttachment;

	glDeleteFramebuffers(1, &ID);
}

void FBORenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,ID);
}

bool FBORenderTarget::Init(TArray<RTextureOptions> ColourAttachmentOptions, RTextureOptions DepthAttachmentOptions)
{
	bool bIsCompleted = false;

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	TArray<GLenum> Attachments = TArray<GLenum>(ColourAttachments.Size());
	for (uint32 i = 0; i < ColourAttachments.Size(); i++) {
		if (ColourAttachmentOptions.IsNotEmpty() && ColourAttachmentOptions.Size() == ColourAttachments.Size()) { // options is either empty or same number with attachments, nothing else will work.

			ColourAttachments[i]->TextureOptions = ColourAttachmentOptions[i];
		}
		ColourAttachments[i]->Generate();

		// attach to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
			ColourAttachments[i]->TextureID, 0);
		Attachments.PushBack(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(Attachments.Size(), &Attachments[0]);

	DepthStencilAttachment->TextureOptions = DepthAttachmentOptions;
	DepthStencilAttachment->Generate();

	// attach to framebuffer
	glNamedFramebufferTexture(ID, GL_DEPTH_ATTACHMENT, DepthStencilAttachment->TextureID, 0);

	GLenum Result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Result == GL_FRAMEBUFFER_COMPLETE) {
		bIsCompleted = true;
	}
	else {
		//TODO: error message if framebufer fails to initialize
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bIsCompleted;
}


GLFrameBufferObject::GLFrameBufferObject(uint32 fsaa)
{
}
