#include "pch.h"
#include "Resource.h"


void CResource::SaveToScene(FILE* _pFile)
{
	SaveWString(_pFile, GetName());
	SaveWString(_pFile, m_strPath);
}

CResource::CResource(RES_TYPE _eType)
	:m_iRefCount(0),m_eType(_eType),m_iCloneCount(0)
{
}

CResource::CResource(const CResource& _res):m_iRefCount(0),m_iCloneCount(0),m_eType(_res.m_eType)
{
}

CResource::~CResource()
{
}
