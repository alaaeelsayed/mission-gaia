//-----------------------------------------------------------------------------
// File:			W_ShaderStorageBuffer.cpp
// Original Author:	Hesham ElHamshary
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_ShaderStorageBuffer.h"

namespace wolf
{
	//----------------------------------------------------------
	// Constructors
	//----------------------------------------------------------
	ShaderStorageBuffer::ShaderStorageBuffer(unsigned int p_uiLength) : m_uiLength(p_uiLength)
	{
		glGenBuffers(1, &m_uiBuffer);
	}

	ShaderStorageBuffer::ShaderStorageBuffer(const void *p_pData, unsigned int p_uiLength) : m_uiLength(p_uiLength)
	{
		glGenBuffers(1, &m_uiBuffer);
		Write(p_pData, p_uiLength);
	}

	//----------------------------------------------------------
	// Destructor
	//----------------------------------------------------------
	ShaderStorageBuffer::~ShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_uiBuffer);
	}

	//----------------------------------------------------------
	// Fills this shader storage buffer with the given data
	//----------------------------------------------------------
	void ShaderStorageBuffer::Write(const void *p_pData, int p_iLength)
	{
		Bind();
		glBufferData(GL_SHADER_STORAGE_BUFFER, p_iLength == -1 ? m_uiLength : p_iLength, p_pData, GL_STATIC_DRAW);
	}

	//----------------------------------------------------------
	// Binds this buffer
	//----------------------------------------------------------
	void ShaderStorageBuffer::Bind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_uiBuffer);
	}

	void ShaderStorageBuffer::BindBase(unsigned int index)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_uiBuffer);
	}

}
