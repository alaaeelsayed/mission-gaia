//-----------------------------------------------------------------------------
// File:			W_ShaderStorageBuffer.h
// Original Author:	Hesham ElHamshary
//
// OpenGL 4.3 Introduced Shader Storage Buffers for direct access of data
// from shaders.
//-----------------------------------------------------------------------------
#ifndef W_SHADERSTORAGEBUFFER_H
#define W_SHADERSTORAGEBUFFER_H

#include "W_Types.h"
#include "W_Buffer.h"
#include "W_Texture.h"

namespace wolf
{

	struct Vertex
	{
		GLfloat x, y, z;	// Position
		GLfloat u, v;		// Texture Coordinates
		GLfloat nX, nY, nZ; // Normals
	};

	class ShaderStorageBuffer : public Buffer
	{
		friend class BufferManager;

	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		virtual void Bind();
		void BindBase(unsigned int index);
		virtual void Write(const void *pData, int p_iLength = -1);
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// Made private to enforce creation and deletion via BufferManager
		ShaderStorageBuffer(unsigned int p_uiLength);
		ShaderStorageBuffer(const void *p_pData, unsigned int p_uiLength);

		virtual ~ShaderStorageBuffer();
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		unsigned int m_uiLength;
		unsigned int m_uiMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		GLuint m_uiBuffer;
		//-------------------------------------------------------------------------
	};

}

#endif
