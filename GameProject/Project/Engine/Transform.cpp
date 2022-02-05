#include "pch.h"
#include "Transform.h"

#include "ConstantBuffer.h"
#include "Device.h"

tTransform	g_transform;
Vec3 vAsis[3] = { Vec3::Right,Vec3::Up,Vec3::Front };

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vLocalPos(Vec3(0.f, 0.f, 0.f))
	, m_vLocalRot(Vec3(0.f, 0.f, 0.f))
	, m_vLocalScale(Vec3(1.f, 1.f, 1.f))
{
}

CTransform::~CTransform()
{
}

void CTransform::FinalUpdate()
{
	Matrix matTranslation = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);
	Matrix matScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	Matrix matRot = XMMatrixRotationX(m_vLocalRot.x);
	matRot *= XMMatrixRotationY(m_vLocalRot.y);
	matRot *= XMMatrixRotationZ(m_vLocalRot.z);

	static Vec3 arrDefault[(UINT)DIR_TYPE::END] = { Vec3::Right, Vec3::Up, Vec3::Front };

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(arrDefault[i], matRot);
	}

	// ���� x (ũ�� x ȸ�� x �̵�)(�������)
	m_matWorld = matScale * matRot * matTranslation;

	if (GetObj()->GetParent())
	{
		m_matWorld *= GetObj()->GetParent()->Transform()->GetWorldMat();

		for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(arrDefault[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}
	else
	{
		memcpy(m_vWorldDir, m_vLocalDir, sizeof(Vec3) * 3);
	}

	// ����� ���
	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::UpdateData()
{
	static CConstantBuffer* pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	UINT iOffsetPos = pCB->AddData(&g_transform);
	CDevice::GetInst()->SetConstBufferToRegister(pCB, iOffsetPos);
}

Vec3 CTransform::GetWorldScale()
{
	CGameObject* pParent = GetObj()->GetParent();
	Vec3 vWorldScale = m_vLocalScale;
	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetLocalScale();

		pParent = pParent->GetParent();
	}
	return vWorldScale;
}

void CTransform::LookAt(const Vec3& _vLook)
{
	Vec3 vFront = _vLook;
	vFront.Normalize();

	Vec3 vRight = Vec3::Up.Cross(_vLook);
	vRight.Normalize();

	Vec3 vUp = vFront.Cross(vRight);
	vUp.Normalize();

	Matrix matRot = XMMatrixIdentity();

	matRot.Right(vRight);
	matRot.Up(vUp);
	matRot.Front(vFront);

	m_vLocalRot = DeComposeRotMat(matRot);

	// ���⺤��(��, ��, ��) �����ϱ�	
	Matrix matRotate = XMMatrixRotationX(m_vLocalRot.x);
	matRotate *= XMMatrixRotationY(m_vLocalRot.y);
	matRotate *= XMMatrixRotationZ(m_vLocalRot.z);

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(vAsis[i], matRotate);
		m_vLocalDir[i].Normalize();
		m_vWorldDir[i] = m_vLocalDir[i];
	}
}

bool CTransform::IsCasting(const Vec3& _vPos)
{
	Vec3 vWorldPos = GetWorldPos();
	Vec3 vWorldScale = GetWorldScale();
	if (vWorldPos.x - vWorldScale.x / 2.f < _vPos.x && _vPos.x < vWorldPos.x + vWorldScale.x / 2.f
		&& vWorldPos.y - vWorldScale.y / 2.f < _vPos.y && _vPos.y < vWorldPos.y + vWorldScale.y / 2.f)
	{
		return true;
	}
	return false;
}

float CTransform::GetMaxScale()
{
	Vec3 vWorldScale = GetWorldScale();
	float fMax = max(vWorldScale.x, vWorldScale.y);
	fMax = max(fMax, vWorldScale.z);
	return fMax;
}

void CTransform::SaveToScene(FILE* _pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_vLocalPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vLocalScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vLocalRot, sizeof(Vec3), 1, _pFile);
}

void CTransform::LoadFromScene(FILE* _pFile)
{
	fread(&m_vLocalPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vLocalScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vLocalRot, sizeof(Vec3), 1, _pFile);
}