#include <Components/VertexBuffers.h>


#include <VertexFormat.h>
#include <Managers/LogManager.h>


/*	some wisdom

With glBufferData (...), you can call that command multiple times on the same object
and it will orphan the old memory and allocate new storage. 
With glBufferStorage (...), the buffer's size is set for the lifetime of the object (immutable)
and it is an error (GL_INVALID_OPERATION) to call glBufferStorage (...) again
once it has been allocated immutably.

*/
/* some more wisdom
/glBindVertexBuffer/ and /glVertexArrayVertexBuffer/ bind the buffer named 'buffer'
to the vertex buffer binding point whose index is given by 'bindingindex'. 
/glBindVertexBuffer/ modifies the binding of the currently bound vertex array object, 
whereas /glVertexArrayVertexBuffer/ allows the caller to specify ID of the vertex array object(VAO)
with an argument named 'vaobj'.
*/

/*
glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(vertex_t));

is the named equivalent to

glBindVertexArray(vao);
glBindVertexBuffer(0, vbo, 0, sizeof(vertex_t));

*/

//VERTEX ARRAY OBJECT

VertexArrayObject::VertexArrayObject() : bIsCreated(false)
{}


VertexArrayObject::~VertexArrayObject()
{}

void VertexArrayObject::Bind()
{
	glBindVertexArray(ID);
}

void VertexArrayObject::CreateArray(PVertexComponentCount InComponentCount)
{
	if (bIsCreated == false) {
		glCreateVertexArrays(1, &ID);

		ComponentCount = InComponentCount;
		bIsCreated = true;
	}
}

void VertexArrayObject::SetupAttribute(uint32 BufferIndex, uint32 Attribute, uint32 ComponentCount, uint32 RelativeOffset)
{

	// Enable the attribute
	glEnableVertexArrayAttrib(ID, Attribute	);

	// Tell OpenGL what the format of the attribute is
	glVertexArrayAttribFormat(ID, Attribute, ComponentCount, GL_FLOAT, GL_FALSE,RelativeOffset);

	// Tell OpenGL which vertex buffer binding to use for this attribute. 
	glVertexArrayAttribBinding(ID, Attribute, BufferIndex);

}

void VertexArrayObject::AttachVertexBuffer(uint32 Buffer,uint32 Index,uint32 Stride)
{
	glVertexArrayVertexBuffer(ID, Index, Buffer, 0, Stride);
}

void VertexArrayObject::AttachElementBuffer(uint32 ElementBuffer)
{
	glVertexArrayElementBuffer(ID, ElementBuffer);
}

uint32 VertexArrayObject::GetID()
{
	return ID;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

// VERTEX BUFFER OBJECT

VertexBufferObject::VertexBufferObject() : bIsCreated(false)
{}

VertexBufferObject::~VertexBufferObject()
{}

void VertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER,ID);
}

//Maybe check the size? nuuuh
void  VertexBufferObject::CreateBuffer(size_t Size , const void *Data = nullptr)
{
	// Create the buffer
	if (bIsCreated == false) {
		//glCreateBuffers(1, &ID);

		glGenBuffers(1, &ID);   // Generate the buffer

		glBindBuffer(GL_ARRAY_BUFFER, ID);

		Index = 0;
		if (Data != nullptr) {
			//glBufferStorage(GL_ARRAY_BUFFER, Size, Data, GL_DYNAMIC_STORAGE_BIT);
			//glNamedBufferStorage(ID, Size, Data, GL_DYNAMIC_STORAGE_BIT);
			glBufferData(GL_ARRAY_BUFFER, Size, Data, GL_STATIC_DRAW);
		}
		bIsCreated = true;
	}
	else
	{
		//LOG(WARNING, "Buffer Already Created");
	}
	
}
void VertexBufferObject::WriteBuffer(void * Data)
{
	//glNamedBufferSubData(ID, 0, sizeof(Data), Data);

	glNamedBufferData(ID, sizeof(Data), Data, GL_STATIC_DRAW);

	//void *BufferPtr = glMapNamedBufferRange(ID, 0, sizeof(Data), GL_MAP_WRITE_BIT); // glMapNamedBuffer(ID, GL_WRITE_ONLY); 

	//memcpy(BufferPtr, Data, sizeof(Data));

	//unmap only before buffer deletetion.
	//glUnmapNamedBuffer(ID);

}

void VertexBufferObject::CopyBuffer(uint32 ReadBufferID, ptr ReadOffset)
{
	//glCopyNamedBufferSubData(ReadBufferID, ID, ReadOffset, 0, sizeof(VertexBufferObject));
}

uint32 VertexBufferObject::GetID()
{
	return ID;
}

uint32 VertexBufferObject::GetIndex()
{
	return Index;
}

/* p145 c 5.1.2
If the data you want to put into a buffer is a
constant value, it is probably much more efficient to call
glClearBufferSubData()
or glClearNamedBufferSubData()
*/


/* p146 c 5.1.2
Once your data has been sent to the GPU, it’s entirely possible you may want to share
that data between buffers or copy the results from one buffer into another.OpenGL
provides an easy - to - use way of doing that.glCopyBufferSubData() and
glCopyNamedBufferSubData() let you specify which buffers are involved as
well as the size and offsets to use.
*/


/* p154 c 5.1.3
After the vertex format information has been set up with calls to
glVertexArrayAttribFormat(), you can change the vertex buffers that are bound
with further calls to glVertexArrayAttribBinding(). If you want to render a lot of
geometry stored in different buffers but with similar vertex formats, simply call
glVertexArrayAttribBinding() to switch buffers and start drawing from them.
*/


