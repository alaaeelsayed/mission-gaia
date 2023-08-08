//-----------------------------------------------------------------------------
// File:			W_ProgramManager.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_ProgramManager.h"

namespace wolf
{

	std::map<std::string, ProgramManager::Entry *> ProgramManager::m_programs;

	//----------------------------------------------------------
	// Creates a new Program or returns an existing copy if already
	// loaded previously
	//----------------------------------------------------------
	Program *ProgramManager::CreateProgram(const std::string &vsFile, const std::string &fsFile)
	{
		std::map<std::string, Entry *>::iterator iter = m_programs.find(vsFile + fsFile);

		if (iter != m_programs.end())
		{
			iter->second->m_iRefCount++;
			return iter->second->m_pProg;
		}

		Program *pProg = new Program(vsFile, fsFile);
		Entry *pEntry = new Entry(pProg);
		m_programs[vsFile + fsFile] = pEntry;

		return pProg;
	}

	//----------------------------------------------------------
	// Creates a new Program or returns an existing copy if already
	// loaded previously
	//----------------------------------------------------------
	Program *ProgramManager::CreateProgram(const std::string &sFile, GLenum shaderType)
	{
		std::map<std::string, Entry *>::iterator iter = m_programs.find(sFile);

		if (iter != m_programs.end())
		{
			iter->second->m_iRefCount++;
			return iter->second->m_pProg;
		}

		Program *pProg = new Program(sFile, shaderType);
		Entry *pEntry = new Entry(pProg);
		m_programs[sFile] = pEntry;
		return pProg;
	}

	//----------------------------------------------------------
	// Destroys a program. Only actually deletes it if the refcount
	// is down to 0.
	//----------------------------------------------------------
	void ProgramManager::DestroyProgram(Program *pProg)
	{
		if (!pProg)
			return;

		std::map<std::string, Entry *>::iterator iter;

		for (iter = m_programs.begin(); iter != m_programs.end(); iter++)
		{
			if (iter->second->m_pProg == pProg)
			{
				iter->second->m_iRefCount--;
				if (iter->second->m_iRefCount == 0)
				{
					delete iter->second->m_pProg;
					m_programs.erase(iter);
				}
				return;
			}
		}

		// Should never get here!
		assert(false);
	}

}
