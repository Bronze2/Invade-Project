#include "pch.h"
#include "MinionScript.h"
#include "TimeMgr.h"
#include "Sensor.h"
#include "Server.h"

void CMinionScript::Init()
{
	m_eState = MINION_STATE::WALK;
	switch (m_eAttackType)
	{
	case MINION_ATTACK_TYPE::MELEE:{
		SetAttackRange(50);
		m_uiMaxHp = 450; m_uiAttackDamage = 30;
		}
		break;

	case MINION_ATTACK_TYPE::RANGE: {
		SetAttackRange(150);
		m_uiMaxHp = 300; m_uiAttackDamage = 20;
	}
	 break;

	case MINION_ATTACK_TYPE::CANON: {
		SetAttackRange(150);
		m_uiMaxHp = 550; m_uiAttackDamage = 60;
	}
	 break;

	}
	m_iCurHp = m_uiMaxHp;
	m_pTarget = m_pNexus;

}


void CMinionScript::Update()
{
	CheckHp();
	if (m_eState == MINION_STATE::DIE) {
		return;
	}
	FindNearObject(m_arrEnemy);
	CheckRange();
	Vec3 vPos = Transform()->GetWorldPos(); 
	Vec3 vTargetPos;
	Vec3 vRot = Transform()->GetLocalRot();
	if (nullptr != m_pTarget&&!m_bAllienceCol) {
		Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
		float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
		float rotate = angle * 0.0174532925f; //0.0174532925f
		vRot.y = rotate;

	}
	if (m_bAllienceCol&&!m_bSeparate) {
		if (m_bRotate) {
			cout << "Rot M" << endl;
			if (m_eCamp == CAMP_STATE::RED) {
				vRot.y -= PI / 2;
			}
			else {
				vRot.y += PI / 2;
			}
			m_bRotate = false;
		}
	}

	Vec3 vLocalPos = Transform()->GetLocalPos();
	//

	switch (m_eState)
	{
	case MINION_STATE::WALK: {
		Vec3 vWorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vLocalPos.x -= vWorldDir.x * 5;
		vLocalPos.z -= vWorldDir.z * 5;
		Transform()->SetLocalPos(vLocalPos);
		Transform()->SetLocalRot(vRot);
	}
		break;
	case MINION_STATE::ATTACK:
	{
		if (m_ePrevState != m_eState) {
			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();

			//cout <<"["<<m_GetId()<<"]"  <<m_pTarget->GetScript<CMinionScript>()->m_GetId() << endl;
			float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;
		}
	}
		break;
	case MINION_STATE::DIE:
		break;
	default:
		break;
	}

	Transform()->SetLocalPos(vLocalPos);
	Transform()->SetLocalRot(vRot);

	/*if (m_id == 0 || m_id == 1)
		cout << "Minion ID:" << m_id << "Pos:" << Transform()->GetLocalPos().x << "," << Transform()->GetLocalPos().y << "," << Transform()->GetLocalPos().z << endl << endl;*/


	//미니언 정보 update
	//SHARED_DATA::g_minion[m_GetId()].m_cLock.lock();
	SHARED_DATA::g_minion[m_GetId()].Pos.x = vLocalPos.x;
	SHARED_DATA::g_minion[m_GetId()].Pos.y = vLocalPos.y;
	SHARED_DATA::g_minion[m_GetId()].Pos.z = vLocalPos.z;
	SHARED_DATA::g_minion[m_GetId()].Rot.x = vRot.x;
	SHARED_DATA::g_minion[m_GetId()].Rot.y = vRot.y;
	SHARED_DATA::g_minion[m_GetId()].Rot.z = vRot.z;
	SHARED_DATA::g_minion[m_GetId()].State = m_eState;

	//SHARED_DATA::g_minion[m_GetId()].m_cLock.unlock();
	//std::cout << GetObj()->GetId() << endl;
	CServer::GetInst()->send_move_minion_packet(m_GetId());
	//

}



void CMinionScript::OnDetectionEnter(CGameObject* _pOther)
{
	m_arrEnemy.push_back(_pOther);
	m_bFindNear = true;
}

void CMinionScript::OnDetection(CGameObject* _pOther)
{
	
}

void CMinionScript::OnDetectionExit(CGameObject* _pOther)
{
	vector<CGameObject*>::iterator iter=m_arrEnemy.begin();
	for (int i=0; iter != m_arrEnemy.end(); ++iter,++i) {
		if (m_arrEnemy[i] == _pOther) {
			m_arrEnemy.erase(iter);
			if (Sensor()->GetDetectionCount() == 0) {
				m_bFindNear = false;
				m_pTarget = m_pNexus;
			}
			else {
				FindNearObject(m_arrEnemy);
			}
			return;
		}
		
	}

}

void CMinionScript::AddObject(CGameObject* _pObject)
{

	m_arrEnemy.push_back(_pObject);

}

void CMinionScript::CheckHp()
{
	if (m_iCurHp <= 0.f&&!GetObj()->IsFallDown()) {
		m_eState = MINION_STATE::DIE;
		GetObj()->SetFallDown();
	}
}

void CMinionScript::CheckRange()
{
	if (SHARED_DATA::g_minion[m_GetId()].m_during_attack) {
		SHARED_DATA::g_minion[m_GetId()].m_attack_current_time += 1;
	}

	if (m_pTarget == nullptr)return;
	Vec3 vTargetPos=m_pTarget->Transform()->GetWorldPos();
	Vec3 vPos = Transform()->GetWorldPos();
	float length = sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));
	if (m_fAttackRange >= length) {
		if (!SHARED_DATA::g_minion[m_GetId()].m_during_attack) {
			cout << "[" << m_GetId() << "] 공격하라~" << endl;
			m_eState = MINION_STATE::ATTACK;
			SHARED_DATA::g_minion[m_GetId()].State = m_eState;

			switch (m_eAttackType)
			{
			case MINION_ATTACK_TYPE::MELEE: {

				if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
					m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
				}
				//else if (m_pTarget->GetScript<CTowerScript>() != nullptr) {
				//	m_pTarget->GetScript<CTowerScript>()->GetDamage(m_uiAttackDamage);
				//}
			}
			break;

			case MINION_ATTACK_TYPE::RANGE: {

				if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
					m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
				}
				//if(m_eCamp == CAMP_STATE::BLUE)
				//	CreateProjectile(L"Blue");					
				//else
				//	CreateProjectile(L"Red");
			}
			break;

			case MINION_ATTACK_TYPE::CANON: {

				if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
					m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
				}
				//if (m_eCamp == CAMP_STATE::BLUE)
				//	CreateProjectile(L"Blue");
				//else
				//	CreateProjectile(L"Red");
			}
			break;

			}
			
			CServer::GetInst()->send_anim_minion_packet(m_GetId());

			SHARED_DATA::g_minion[m_GetId()].m_during_attack = true;
			SHARED_DATA::g_minion[m_GetId()].m_attack_current_time ++;


		}
		else if (SHARED_DATA::g_minion[m_GetId()].m_attack_current_time >= SHARED_DATA::g_minion[m_GetId()].m_attack_max_time) {

			SHARED_DATA::g_minion[m_GetId()].m_during_attack = false;
			SHARED_DATA::g_minion[m_GetId()].m_attack_current_time = 0;
			m_eState = MINION_STATE::WALK;

		}
		//if (m_GetId() == 0) cout << "DT :" << m_attack_current_time << endl;

	}
	else {
		if (SHARED_DATA::g_minion[m_GetId()].m_attack_current_time >= SHARED_DATA::g_minion[m_GetId()].m_attack_max_time) {
			
			SHARED_DATA::g_minion[m_GetId()].m_during_attack = false;
			SHARED_DATA::g_minion[m_GetId()].m_attack_current_time = 0;
			m_eState = MINION_STATE::WALK;
				
		}
		
	}
}

void CMinionScript::FindNearObject(const vector<CGameObject*>& _pObject)
{
	if (m_arrEnemy.size() == 0) {
		m_pTarget = m_pNexus;
	}

	if (0 == _pObject.size()||!m_bFindNear)return;
	for (int i = 0; i < _pObject.size(); ++i) {
		if (i == 0) {
			m_pTarget = _pObject[i];
		}
		else {
			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
			Vec3 vPos = Transform()->GetWorldPos();
			float length1= sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));

			Vec3 vTargetPos2 = _pObject[i]->Transform()->GetWorldPos();
			float length2 = sqrt(pow(vTargetPos2.x - vPos.x, 2) + pow(vTargetPos2.z - vPos.z, 2));
			if (length1 > length2) {
				m_pTarget = _pObject[i];
			}
		}
	}



}



void CMinionScript::m_FFind()
{
}

CMinionScript::CMinionScript():CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_eState(MINION_STATE::WALK)
{
}

CMinionScript::CMinionScript(float _fSpeed, float _fRange, MINION_STATE _eState, CAMP_STATE _eCamp)
	: CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_fSpeed(_fSpeed),m_fRange(_fRange),m_eState(_eState),m_eCamp(_eCamp), m_bAllienceCol(false)
	,m_bRotate(false),m_bSeparate(false)
{
}

CMinionScript::~CMinionScript()
{
}

#include "Collider3D.h"

static bool iSeparate=true;

void CMinionScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	cout << " Coll " << endl;

	if (_pOther->GetObj()->GetScript<CMinionScript>() == nullptr) {
	
	}
	else {
		if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() == m_eCamp&&m_eState==MINION_STATE::WALK) {
			cout <<" Coll in "<<endl;
			m_bAllienceCol = true;
			m_bRotate = true;
			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetState() == MINION_STATE::WALK) {
				float Value=_pOther->Transform()->GetLocalPos().x - Transform()->GetLocalPos().x;
				if (Value >= 0) {
					_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
					GetObj()->GetScript<CMinionScript>()->SetSeparate(true);
				}
				else {
					GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
					_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(true);
				}
			
				}
			}
			m_iAllienceCol += 1;
		}
}




void CMinionScript::OnCollision3D(CCollider3D* _pOther)
{
}

void CMinionScript::OnCollision3DExit(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CMinionScript>() == nullptr) {

	}
	else {
		if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() == m_eCamp && m_eState == MINION_STATE::WALK) {
			
		
		}
		m_iAllienceCol -= 1;
		if (m_iAllienceCol <= 0) {
			m_iAllienceCol = 0;
		}
		if (m_iAllienceCol == 0) {
			m_bAllienceCol = false;
		}
	}

}

#include "ProjectileScript.h"
#include "PlayerScript.h"
void CMinionScript::CreateProjectile(const wstring& _Layer)
{
	if (nullptr == m_pTarget)
		return;
	if (m_pTarget->IsDead())
		return;
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CProjectileScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->GetScript<CProjectileScript>()->SetObject(GetObj());
	pObject->GetScript<CProjectileScript>()->SetDamage(m_uiAttackDamage);
	pObject->GetScript<CProjectileScript>()->SetProjectileType(PROJECTILE_TYPE::MINION);
	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().z, m_pTarget->Transform()->GetWorldPos().z));
	}
	else {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().y, m_pTarget->Transform()->GetWorldPos().z));
	}

	
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vProjectilePos = vPos;
	vProjectilePos.y += 10;
	pObject->Transform()->SetLocalPos(vProjectilePos);

	pObject->GetScript<CProjectileScript>()->SetStartPos(vProjectilePos);
	pObject->GetScript<CProjectileScript>()->SetTarget(m_pTarget);
	//		GetObj()->AddChild(pObject);
	//		pObject->Transform()->SetLocalPos(Vec3(0.f, 50.f, -100.f));
	pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(0.1f, 0.1f, 0.1f));

	pObject->GetScript<CProjectileScript>()->Init();
	pObject->GetScript<CProjectileScript>()->SetSpeed(500.f);
	m_pProjectile = pObject;
	CreateObject(pObject, _Layer);

	SHARED_DATA::g_bullet[pObject->GetScript<CProjectileScript>()->m_GetId()] = vProjectilePos;
	CServer::GetInst()->send_projectile_packet(pObject->GetScript<CProjectileScript>()->m_GetId(), 0);
	SHARED_DATA::g_bulletindex++;
}