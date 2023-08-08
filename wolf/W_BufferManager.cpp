//-----------------------------------------------------------------------------
// File:			W_BufferManager.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_BufferManager.h"

namespace wolf
{

	//----------------------------------------------------------
	// Creates a new Vertex Buffer
	//----------------------------------------------------------
	VertexBuffer *BufferManager::CreateVertexBuffer(unsigned int length)
	{
		return new VertexBuffer(length);
	}

	//----------------------------------------------------------
	// Creates a new Vertex Buffer
	//----------------------------------------------------------
	VertexBuffer *BufferManager::CreateVertexBuffer(const void *pData, unsigned int length)
	{
		return new VertexBuffer(pData, length);
	}

	//----------------------------------------------------------
	// Creates a new Index Buffer
	//----------------------------------------------------------
	IndexBuffer *BufferManager::CreateIndexBuffer(unsigned int length)
	{
		return new IndexBuffer(length);
	}

	//----------------------------------------------------------
	// Creates a new Index Buffer
	//----------------------------------------------------------
	IndexBuffer *BufferManager::CreateIndexBuffer(const void *pData, unsigned int length)
	{
		IndexBuffer *pRet = new IndexBuffer(length);
		pRet->Write(pData);
		return pRet;
	}

	//----------------------------------------------------------
	// Creates a new Frame Buffer
	//----------------------------------------------------------
	FrameBuffer *BufferManager::CreateFrameBuffer(float width, float height)
	{
		FrameBuffer *pRet = new FrameBuffer(width, height);
		return pRet;
	}

	//----------------------------------------------------------
	// Creates a new Shader Storage Buffer
	//----------------------------------------------------------
	ShaderStorageBuffer *BufferManager::CreateShaderStorageBuffer(unsigned int length)
	{
		return new ShaderStorageBuffer(length);
	}

	//----------------------------------------------------------
	// Creates a new Shader Storage Buffer
	//----------------------------------------------------------
	ShaderStorageBuffer *BufferManager::CreateShaderStorageBuffer(const unsigned int *pData, unsigned int length)
	{
		return new ShaderStorageBuffer(pData, length);
	}

	//----------------------------------------------------------
	// Destroys a buffer.
	//----------------------------------------------------------
	void BufferManager::DestroyBuffer(Buffer *pBuf)
	{
		if (!pBuf)
			return;
		delete pBuf;
	}

}
