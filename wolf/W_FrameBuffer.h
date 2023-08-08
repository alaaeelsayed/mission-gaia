//-----------------------------------------------------------------------------
// File:			W_FrameBuffer.h
// Original Author:	Alaa Emad
//
//-----------------------------------------------------------------------------
#ifndef W_FRAMEBUFFER_H
#define W_FRAMEBUFFER_H

#include "W_Types.h"
#include "W_Buffer.h"
#include "W_Texture.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_BufferManager.h"

namespace wolf
{
	class VertexBuffer;
	class VertexDeclaration;
	class FrameBuffer : public Buffer
	{
		friend class BufferManager;

	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		virtual void Bind();
		void Render();
		virtual void Write(const void *p_pData, int p_iLength = -1){};
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// Made private to enforce creation and deletion via BufferManager
		FrameBuffer(float width, float height);
		virtual ~FrameBuffer();
		void _createTexture();
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		Program *m_pProgram;
		VertexBuffer *m_pVBO;
		VertexDeclaration *m_pDecl;
		GLuint m_uiBuffer;
		GLuint m_uiTexture;
		GLuint m_uiRenderBuffer;
		float m_fWidth, m_fHeight;
		//-------------------------------------------------------------------------
	};

}

#endif
