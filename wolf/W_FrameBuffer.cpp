//-----------------------------------------------------------------------------
// File:			W_FrameBuffer.cpp
// Original Author:	Alaa Emad
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_FrameBuffer.h"

namespace wolf
{
	//----------------------------------------------------------
	// Constructor
	//----------------------------------------------------------
	FrameBuffer::FrameBuffer(float width, float height) : m_fWidth(width), m_fHeight(height)
	{
		GLfloat quadVertices[] = {
			// x     y     u     v
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f};

		m_pVBO = BufferManager::CreateVertexBuffer(quadVertices, sizeof(GLfloat) * 4 * 6);

		m_pDecl = new wolf::VertexDeclaration();
		m_pDecl->Begin();
		m_pDecl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
		m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
		m_pDecl->SetVertexBuffer(m_pVBO);
		m_pDecl->End();

		m_pProgram = ProgramManager::CreateProgram("data/shaders/screen.vsh", "data/shaders/screen.fsh");
		glGenFramebuffers(1, &m_uiBuffer);
		Bind();
		_createTexture();
		glGenRenderbuffers(1, &m_uiRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_uiRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_fWidth, m_fHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiRenderBuffer);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//----------------------------------------------------------
	// Destructor
	//----------------------------------------------------------
	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_uiBuffer);
		glDeleteRenderbuffers(1, &m_uiRenderBuffer);
		glDeleteTextures(1, &m_uiTexture);
	}

	void FrameBuffer::_createTexture()
	{
		glGenTextures(1, &m_uiTexture);
		glBindTexture(GL_TEXTURE_2D, m_uiTexture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_fWidth, m_fHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTexture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//----------------------------------------------------------
	// Binds this buffer
	//----------------------------------------------------------
	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_uiBuffer);
	}

	void FrameBuffer::Render()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_uiTexture);
		m_pProgram->SetUniform("u_texture", 0);
		m_pProgram->Bind();

		m_pDecl->Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

}
