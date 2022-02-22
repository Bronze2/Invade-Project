#include "pch.h"
#include "Collider3D.h"
#include "Transform.h"
#include "ConstantBuffer.h"
#include "Script.h"

#include "ResMgr.h"
#include "Device.h"
UINT CCollider3D::g_iColID = 0;
void CCollider3D::Update()
{

}

void CCollider3D::FinalUpdate()
{
	if (!IsActive())
		return;

	Vec3 vFinalPos = m_vOffsetPos;

	vFinalPos = vFinalPos / Transform()->GetWorldScale();
	Matrix matTranslation = XMMatrixTranslation(vFinalPos.x, vFinalPos.y, vFinalPos.z);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matColWorld = matScale * matTranslation;
	m_matColWorld *= Transform()->GetWorldMat();
}

void CCollider3D::Render()
{
	if (!IsActive())
		return;
	static CConstantBuffer* pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);
	g_transform.matWorld = m_matColWorld;
	CDevice::GetInst()->SetConstBufferToRegister(pCB, pCB->AddData(&g_transform));
	m_pColMtrl->UpdateData();
	m_pColMesh->Render();
	memset(&m_matColWorld, 0, sizeof(Matrix));
}

void CCollider3D::OnCollisionEnter(CCollider3D* _pOther)
{
	m_iCollisionCount += 1;
	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
		vecScripts[i]->OnCollision3DEnter(_pOther);
}

void CCollider3D::OnCollision(CCollider3D* _pOther)
{
	if (0 < m_iCollisionCount)
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl_1");
	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
		vecScripts[i]->OnCollision3D(_pOther);
}

void CCollider3D::OnCollisionExit(CCollider3D* _pOther)
{
	m_iCollisionCount -= 1;
	if (0 == m_iCollisionCount)
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl_0");

	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
		vecScripts[i]->OnCollision3DExit(_pOther);
}

void CCollider3D::SetCollider3DType(COLLIDER3D_TYPE _eType)
{
	m_eType = _eType;
	if (COLLIDER3D_TYPE::CUBE == m_eType)
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColCubeMesh");

}

void CCollider3D::SaveToScene(FILE* _pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_eType, sizeof(UINT), 1, _pFile);
}


void CCollider3D::LoadFromScene(FILE* _pFile)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);
	fread(&m_eType, sizeof(UINT), 1, _pFile);
	SetCollider3DType(m_eType);
}

void CCollider3D::operator=(const CCollider3D* _pOther)
{
	UINT iColID = m_iColID;
	*this = _pOther;

	m_iColID = iColID;
}

CCollider3D::CCollider3D():CComponent(COMPONENT_TYPE::COLLIDER3D)
,m_vOffsetPos(Vec3(0.f,0.f,0.f)),m_vOffsetScale(Vec3(1.f,1.f,1.f)),m_pColMtrl(nullptr),m_pColMesh(nullptr)
,m_iColID(g_iColID++),m_iCollisionCount(0)
{
	m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl_0");
}

CCollider3D::CCollider3D(const CCollider3D& _Other) : CComponent(COMPONENT_TYPE::COLLIDER3D)
,m_vOffsetPos(_Other.m_vOffsetPos), m_vOffsetScale(_Other.m_vOffsetScale), m_eType(_Other.m_eType),
m_pColMesh(_Other.m_pColMesh), m_pColMtrl(_Other.m_pColMtrl), m_iColID(g_iColID), m_iCollisionCount(0), m_matColWorld(_Other.m_matColWorld)
{
}

CCollider3D::~CCollider3D()
{
}
