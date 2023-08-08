//-----------------------------------------------------------------------------
// File:			W_BufferManager.h
// Original Author:	Gordon Wood
//
// Class to manage Buffers. This isn't strictly needed in our use-cases as all
// it does is delegate through to the buffer classes themselves. However, it's
// provided as an intermediate step. In a future expansion, you may want to
// store a copy of all buffers created so you can later run through them and
// reload them if, say, your context is lost. This is left as an exercise.
//-----------------------------------------------------------------------------
#ifndef W_BUFFER_MANAGER_H
#define W_BUFFER_MANAGER_H

#include "W_Types.h"
#include "W_VertexBuffer.h"
#include "W_IndexBuffer.h"
#include "W_FrameBuffer.h"
#include "W_ShaderStorageBuffer.h"
#include <string>
#include <map>

namespace wolf
{
	class FrameBuffer;
	class BufferManager
	{
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		static VertexBuffer *CreateVertexBuffer(unsigned int length);
		static VertexBuffer *CreateVertexBuffer(const void *pData, unsigned int length);
		static IndexBuffer *CreateIndexBuffer(unsigned int length);
		static IndexBuffer *CreateIndexBuffer(const void *pData, unsigned int length);
		static FrameBuffer *CreateFrameBuffer(float width, float height);
		static ShaderStorageBuffer *CreateShaderStorageBuffer(unsigned int length);
		static ShaderStorageBuffer *CreateShaderStorageBuffer(const unsigned int *pData, unsigned int length);

		static void DestroyBuffer(Buffer *pBuf);
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE TYPES
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
	};

}

#endif
