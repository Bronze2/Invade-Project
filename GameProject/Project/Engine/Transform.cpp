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
	, m_vQuaternion(Vec4(0.f,0.f,0.f,1.f))
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

	auto rotMat = XMMatrixRotationQuaternion(XMLoadFloat4(&this->m_vQuaternion));



	static Vec3 arrDefault[(UINT)DIR_TYPE::END] = { Vec3::Right, Vec3::Up, Vec3::Front };

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(arrDefault[i], matRot);
	}

	// 로컬 x (크기 x 회전 x 이동)(월드행렬)
	m_matWorld = matScale * matRot*rotMat * matTranslation;


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

	// 역행렬 계산
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

	// 방향벡터(우, 상, 전) 갱신하기	
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

Matrix CTransform::LookAt(const Vec3& Eye, const Vec3& target, const Vec3& _Up)
{
	Vec3 forward = target - Eye;
	forward.Normalize();
	Vec3 S = Vec3::Up.Cross(forward);
	S.Normalize();
	Vec3 u = forward.Cross(S);
	Matrix  result;
	result.m[0][0] = S.x;
	result.m[0][1] = S.y;
	result.m[0][2] = S.z;
	result.m[1][0] = u.x;
	result.m[1][1] = u.y;
	result.m[1][2] =u.z;
	result.m[2][0] = forward.x;
	result.m[2][1] = forward.y;
	result.m[2][2] = forward.z;
	result.m[0][3] = -S.Dot(Eye);
	result.m[1][3] = -u.Dot(Eye);
	result.m[2][3] = -forward.Dot(Eye);





	return result;
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