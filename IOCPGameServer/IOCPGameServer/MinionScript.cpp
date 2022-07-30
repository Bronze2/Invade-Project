#include "pch.h"
#include "MinionScript.h"
#include "TimeMgr.h"
#include "Sensor.h"
#include "Server.h"
#include "SceneMgr.h"
#include "Collider3D.h"
#include "PlayerScript.h"

void CMinionScript::Init()
{
	m_eState = MINION_STATE::WALK;
	switch (m_eAttackType)
	{
	case MINION_ATTACK_TYPE::MELEE:
	{
		SetAttackRange(200);
		m_uiMaxHp = 950; m_uiAttackDamage = 50;
	}
		break;

	case MINION_ATTACK_TYPE::RANGE: {
		SetAttackRange(350);
		m_uiMaxHp = 900; m_uiAttackDamage = 50;
	}
	 break;

	case MINION_ATTACK_TYPE::CANON: {
		SetAttackRange(350);
		m_uiMaxHp = 1550; m_uiAttackDamage = 100;
	}
	 break;

	}
	m_iCurHp = m_uiMaxHp;
	m_pTarget = m_pNexus;

	std::chrono::high_resolution_clock::time_point AttackTime = std::chrono::high_resolution_clock::now();
}



//void CMinionScript::Update()
//{
//	CheckHp();
//	if (m_eState == MINION_STATE::DIE) {
//		return;
//	}
//
//	FindNearObject(m_arrEnemy);
//	CheckRange();
//	Vec3 vPos = Transform()->GetWorldPos();
//	Vec3 vTargetPos;
//	Vec3 vRot = Transform()->GetLocalRot();
//	if (nullptr != m_pTarget && !m_bAllienceCol) {
//		Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
//		float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
//		float rotate = angle * 0.0174532925f; //0.0174532925f
//		vRot.y = rotate;
//
//	}
//	if (m_bAllienceCol && !m_bSeparate && !SHARED_DATA::g_minion[m_GetId()].m_during_attack) {
//		if (m_bRotate) {
//			if (m_eCamp == CAMP_STATE::RED) {
//				vRot.y -= PI / 2;
//			}
//			else {
//				vRot.y += PI / 2;
//			}
//			m_bRotate = false;
//		}
//	}
//
//	Vec3 vLocalPos = Transform()->GetLocalPos();
//	//
//
//	switch (m_eState)
//	{
//	case MINION_STATE::IDLE:
//	{
//	}
//	break;
//	case MINION_STATE::WALK: {
//		Vec3 vWorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
//		vLocalPos.x -= vWorldDir.x * 5;
//		vLocalPos.z -= vWorldDir.z * 5;
//		Transform()->SetLocalPos(vLocalPos);
//		Transform()->SetLocalRot(vRot);
//	}
//						   break;
//	case MINION_STATE::ATTACK:
//	{
//		if (m_ePrevState != m_eState) {
//			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
//
//			//cout <<"["<<m_GetId()<<"]"  <<m_pTarget->GetScript<CMinionScript>()->m_GetId() << endl;
//			float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
//			float rotate = angle * 0.0174532925f;
//			vRot.y = rotate;
//		}
//	}
//	break;
//	case MINION_STATE::DIE:
//		break;
//	default:
//		break;
//	}
//
//	Transform()->SetLocalPos(vLocalPos);
//	Transform()->SetLocalRot(vRot);
//
//	/*if (m_id == 0 || m_id == 1)
//	   cout << "Minion ID:" << m_id << "Pos:" << Transform()->GetLocalPos().x << "," << Transform()->GetLocalPos().y << "," << Transform()->GetLocalPos().z << endl << endl;*/
//
//
//	   //미니언 정보 update
//	   //SHARED_DATA::g_minion[m_GetId()].m_cLock.lock();
//	SHARED_DATA::g_minion[m_GetId()].Pos.x = vLocalPos.x;
//	SHARED_DATA::g_minion[m_GetId()].Pos.y = vLocalPos.y;
//	SHARED_DATA::g_minion[m_GetId()].Pos.z = vLocalPos.z;
//	SHARED_DATA::g_minion[m_GetId()].Rot.x = vRot.x;
//	SHARED_DATA::g_minion[m_GetId()].Rot.y = vRot.y;
//	SHARED_DATA::g_minion[m_GetId()].Rot.z = vRot.z;
//	SHARED_DATA::g_minion[m_GetId()].State = m_eState;
//
//	//SHARED_DATA::g_minion[m_GetId()].m_cLock.unlock();
//	//std::cout << GetObj()->GetId() << endl;
//	CServer::GetInst()->send_move_minion_packet(m_GetId(),index);
//	//
//
//}

void CMinionScript::SetRect(int w, int h)
{
	Vec3 Pos = Transform()->GetLocalPos();
	bound.left = Pos.x - w;
	bound.right = Pos.x + w;
	bound.top = Pos.z - h;
	bound.bottom = Pos.z + h;

}

void CMinionScript::SetPrevRect(int w, int h , Vec3 Pos)
{
	bound.left = Pos.x - w;
	bound.right = Pos.x + w;
	bound.top = Pos.z - h;
	bound.bottom = Pos.z + h;

}

void CMinionScript::Update()
{
	CheckHp();
	//CheckObstacle();
	if (m_eState == MINION_STATE::DIE) {
		return;
	}

	Vec3 vLocalPos = Transform()->GetLocalPos();
	if (m_eCamp == CAMP_STATE::BLUE) {
		if(vLocalPos.z > 9500)		//9500
			FindNearObject(m_arrEnemy);
		else
			m_pTarget = m_pNexus;

	}
	else if (m_eCamp == CAMP_STATE::RED) {
		if (vLocalPos.z < 10100)
			FindNearObject(m_arrEnemy);
		else
			m_pTarget = m_pNexus;

	}
	//m_pTarget = m_pNexus;
	//FindNearObject(m_arrEnemy);
	CheckRange();


	Vec3 vPos = Transform()->GetWorldPos(); 
	Vec3 vTargetPos;
	Vec3 vRot = Transform()->GetLocalRot();

	switch (m_eState)
	{
	case MINION_STATE::IDLE:
	{
	}
	break;
	case MINION_STATE::WALK: {


		bool Down = false;
		bool Right = false;
		bool Left = false;

		int Dir = 1;
		if (m_eCamp == CAMP_STATE::BLUE) Dir = -1;
		
		int Prioty = 1;
		if (movePrioty == 0) Prioty = -1;
		
		Vec3 vTargetPos = m_pTarget->Transform()->GetLocalPos();
		vTargetPos.y = 0;

		vLocalPos.y = 0;
		Vec3 vTargetDir = vTargetPos - vLocalPos;
		vTargetDir.Normalize();


		Vec3 vWorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		

		vector<CGameObject*> temp;
		if (m_eCamp == CAMP_STATE::RED) {
			for (auto min : CSceneMgr::GetInst()->GetCurScene(index)->FindLayer(L"Red")->GetParentObj())
			{
				if (min->GetScript<CMinionScript>() != nullptr && 
					Vec3::Distance(vLocalPos, min->Transform()->GetLocalPos()) < 300 &&
					min->GetScript<CMinionScript>()->m_GetId() != m_GetId()) {
					temp.push_back(min);
					
				}
			}
		}
		else if (m_eCamp == CAMP_STATE::BLUE) {
			for (auto min : CSceneMgr::GetInst()->GetCurScene(index)->FindLayer(L"Blue")->GetParentObj())
			{
				if (min->GetScript<CMinionScript>() != nullptr &&
					Vec3::Distance(vLocalPos, min->Transform()->GetLocalPos()) < 300 && 
					min->GetScript<CMinionScript>()->m_GetId() != m_GetId()) {
					temp.push_back(min);
				}
			}
		}
		/// //////////////////////////////////////////////////////////////////////////////////////////
		RECT tempRect;
		Vec3 RightDir  = VectorRotate(vTargetDir, Vec3(0,1,0), PI / 2);
		Vec3 LeftDir  = VectorRotate(vTargetDir, Vec3(0, 1, 0), -PI / 2);
		vector<Vec3> CurrentDir{ vTargetDir ,-vTargetDir ,RightDir , LeftDir };
		map<int, Vec3> nearDir;

		//4방향 검사를 진행
		for (auto& dir : CurrentDir) {
			// 방향마다 몬스터 검사.
			Vec3 Pos = Transform()->GetLocalPos() + (dir * 10);
			SetPrevRect(100,100,Pos);	//충돌체 업데이트
			for (auto& other : temp) {
				if (other->GetScript<CMinionScript>()->GetState() == MINION_STATE::ATTACK ||
					other->GetScript<CMinionScript>()->GetState() == MINION_STATE::IDLE) {
					//상대 몬스터의 충돌체 정보를 가져옴
					RECT otherbound = other->GetScript<CMinionScript>()->GetRect();
					//충돌 검사
					if (IntersectRect(&tempRect, &bound, &otherbound)) {
						continue;
					}
					else {
						nearDir.try_emplace(Vec3::Distance(Pos, m_pTarget->Transform()->GetLocalPos()), dir);
						cout << "[" << Vec3::Distance(Pos, m_pTarget->Transform()->GetLocalPos()) << "]" << endl;
					}
				}
			}
		}
			Vec3 FinalDir = vTargetDir;
			int dis = 999;
			for (auto dir : nearDir) {
				if (dir.first < dis) {
					dis = dir.first;
					FinalDir = dir.second;
				}
			}
			vLocalPos.z += 10 * FinalDir.z;
			vLocalPos.x += 10 * FinalDir.x;

			SetPrevRect(100, 100, vLocalPos);

		/////////////////////////////////////////////////////////////////////////////////////////////
		////아래
		//vLocalPos.z += 10 * vTargetDir.z;
		//vLocalPos.x += 10 * vTargetDir.x;

		//vLocalPos.y = 0;


		//for (auto other : temp)
		//{
		//	Vec3 DisPos = other->Transform()->GetLocalPos();

		//	if (other->GetScript<CMinionScript>()->GetState() == MINION_STATE::ATTACK ||
		//		other->GetScript<CMinionScript>()->GetState() == MINION_STATE::IDLE) {
		//		//if (Vec3::Distance(vLocalPos, DisPos) < 150) {
		//		RECT otherbound = other->GetScript<CMinionScript>()->GetRect();
		//		if (IntersectRect(&tempRect, &bound, &otherbound)) {
		//			vLocalPos.z -= 10 * vTargetDir.z;
		//			vLocalPos.x -= 10 * vTargetDir.x;
		//			vTargetDir = VectorRotate(vTargetDir, Transform()->GetWorldDir(DIR_TYPE::UP), PI / 2 * Prioty);
		//			vLocalPos.z += 10 * vTargetDir.z;
		//			vLocalPos.x += 10 * vTargetDir.x;
		//			Down = true;
		//			break;
		//		}
		//	}
		//}
		////오른쪽 가보기
		//if (Down) {
		//	for (auto other : temp)
		//	{
		//		Vec3 DisPos = other->Transform()->GetLocalPos();

		//		if (other->GetScript<CMinionScript>()->GetState() == MINION_STATE::ATTACK ||
		//			other->GetScript<CMinionScript>()->GetState() == MINION_STATE::IDLE) {
		//			//if (Vec3::Distance(vLocalPos, DisPos) < 120) {
		//			RECT otherbound = other->GetScript<CMinionScript>()->GetRect();
		//				if (IntersectRect(&tempRect, &bound, &otherbound)) {
		//				vLocalPos.z += 10 * vTargetDir.z;
		//				vLocalPos.x += 10 * vTargetDir.x;
		//				Right = true;
		//				break;
		//			}
		//		}
		//	}
		//}


		//왼쪽 가보기
		//if (Right) {
		//	for (auto other : temp)
		//	{
		//		Vec3 DisPos = other->Transform()->GetLocalPos();
		//		DisPos.y = 0;
		//		if (other->GetScript<CMinionScript>()->GetState() == MINION_STATE::ATTACK ||
		//			other->GetScript<CMinionScript>()->GetState() == MINION_STATE::IDLE) {
		//			if (Vec3::Distance(vLocalPos, DisPos) < 100) {
		//				vLocalPos.x += 0;
		//				vLocalPos.z += 20 * Dir;
		//				Left = true;
		//				m_eState = MINION_STATE::IDLE;
		//				cout << " IDLE ON " << m_id << endl;
		//				break;
		//			}
		//		}
		//	}
		//}

		Transform()->SetLocalPos(vLocalPos);
		Transform()->SetLocalRot(vRot);

		//if (m_bAllienceCol && !m_bSeparate && MINION_STATE::ATTACK != m_eState) {
		//


		//}
		//else {
		//	Vec3 vWorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		//	vLocalPos.x -= vWorldDir.x * 5;
		//	vLocalPos.z -= vWorldDir.z * 5;
		//	Transform()->SetLocalPos(vLocalPos);
		//	Transform()->SetLocalRot(vRot);
		//}
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
		Transform()->SetLocalPos(vLocalPos);
	}
		break;
	case MINION_STATE::DIE:
		break;
	default:
		break;
	}

	//
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
	//SHARED_DATA::g_minion[m_GetId()].State = m_eState;

	//SHARED_DATA::g_minion[m_GetId()].m_cLock.unlock();
	//std::cout << GetObj()->GetId() << endl;
	CServer::GetInst()->send_move_minion_packet(m_GetId(),index);
	//

}


void CMinionScript::InterSectsObject(CCollider3D* _pCollider)
{
	Vec3 vWorldPos = Transform()->GetWorldPos();
	vWorldPos.y += Collider3D()->GetOffsetPos().y;

	Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);


	if (nullptr == m_pTarget)
		return;
	if (nullptr == m_pTarget->GetScript<CPlayerScript>())
		return;


	Vec3 target = m_pTarget->Transform()->GetWorldPos();
	target.y += m_pTarget->Collider3D()->GetOffsetPos().z;
	m_pRay->position = vWorldPos;
	m_pRay->direction = vDir;
	float min = INFINITE;
	if (_pCollider->GetObj()->GetName() != L"Obstacle")
	{

	}
	if (m_pRay->Intersects(_pCollider->GetBox(), min)) {
		Vec3 target = m_pTarget->Transform()->GetWorldPos();

		target.y += m_pTarget->Collider3D()->GetOffsetPos().z;
		float distance = Vec3::Distance(target, vWorldPos);
		Vec3 vPos2 = Vec3();

		float distance2 = Vec3::Distance(_pCollider->GetObj()->Transform()->GetWorldPos(), vWorldPos);
		if (min > distance)
			return;
		if (_pCollider->GetObj()->GetName() == L"obstacle") {
			m_InterSectObject = _pCollider->GetObj();

			m_eFindState = FIND_STATE::RAY_FIRST;

			m_fPrevXValue = vWorldPos.x;

		}

	}
}

void CMinionScript::CheckObstacle()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene(index);

	RayCollision(pCurScene->FindLayer(L"Obstacle"));
}

void CMinionScript::RayCollision(const CLayer* _pLayer)
{
	const vector<CGameObject*>& vecObj = _pLayer->GetObjects();

	map<DWORD_PTR, bool>::iterator iter;
	for (size_t i = 0; i < vecObj.size(); ++i) {
		CCollider3D* pCollider = vecObj[i]->Collider3D();
		if (nullptr == pCollider)
			continue;
		InterSectsObject(pCollider);
	}



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
void CMinionScript::GetDamage(const UINT& _uiDamage) 
{ 
	m_iCurHp -= _uiDamage; 

	if (m_iCurHp <= 0.f && !GetObj()->IsFallDown()) {
		m_eState = MINION_STATE::DIE;
		CServer::GetInst()->send_delete_minion(m_GetId());
		DeleteObject(GetObj());
	}
	else {
		CServer::GetInst()->send_damage_minion(m_GetId() , m_iCurHp , m_eCamp);
	}
}
void CMinionScript::CheckHp()
{
	//if (m_iCurHp <= 0.f&&!GetObj()->IsFallDown()) {
	//	m_eState = MINION_STATE::DIE;
	//	CServer::GetInst()->send_delete_minion(m_GetId());
	//	GetObj()->SetFallDown();
	//}
}
#include "TowerScript.h"
void CMinionScript::CheckRange()
{
	if (m_pTarget == nullptr)return;
	Vec3 vTargetPos=m_pTarget->Transform()->GetWorldPos();
	Vec3 vPos = Transform()->GetWorldPos();
	float length = sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));
	if (m_fAttackRange >= length) {
		if (AttackTime < std::chrono::high_resolution_clock::now()) {
			AttackTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(2000);	//2초간격
			m_eState = MINION_STATE::ATTACK;
			//SHARED_DATA::g_minion[m_GetId()].State = m_eState;
			SHARED_DATA::g_minion[m_GetId()].State = m_eState;
			CServer::GetInst()->send_anim_minion_packet(m_GetId(), index);
			switch (m_eAttackType)
			{
			case MINION_ATTACK_TYPE::MELEE: {

				if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
					m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
				}
				else if (m_pTarget->GetScript<CTowerScript>() != nullptr) {
					m_pTarget->GetScript<CTowerScript>()->GetDamage(m_uiAttackDamage);
				}
			}
			break;

			case MINION_ATTACK_TYPE::RANGE: {

//				if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
//					m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
//				}
				if(m_eCamp == CAMP_STATE::BLUE)
					CreateProjectile(L"Blue");					
				else
					CreateProjectile(L"Red");
			}
			break;

			case MINION_ATTACK_TYPE::CANON: {

				if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
					m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
				}
	/*			else if (m_pTarget->GetScript<CTowerScript>() != nullptr) {
					m_pTarget->GetScript<CTowerScript>()->GetDamage(m_uiAttackDamage);
				}*/
				//if (m_eCamp == CAMP_STATE::BLUE)
				//	CreateProjectile(L"Blue");
				//else
				//	CreateProjectile(L"Red");

			}
			break;

			}

		}
		else{
			m_eState = MINION_STATE::IDLE;

		}

	}
	else {
		m_eState = MINION_STATE::WALK;
		SHARED_DATA::g_minion[m_GetId()].State = m_eState;
		CServer::GetInst()->send_anim_minion_packet(m_GetId(), index);
	}
}

void CMinionScript::FindNearObject(const vector<CGameObject*>& _pObject)
{
	if (m_arrEnemy.size() == 0) {
		if (!m_pFirstTower->IsDead())
		{
			m_pTarget = m_pFirstTower;

		}
		else if (!m_pSecondTower->IsDead()) {

			m_pTarget = m_pSecondTower;

		}
		else
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
	if (_pOther->GetObj()->GetScript<CMinionScript>() == nullptr) {
	
	}
	else {
		//if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() == m_eCamp /*&& m_eState == MINION_STATE::WALK*/) {
		//	Vec3 vPos = _pOther->GetObj()->Transform()->GetLocalPos();
		//	if (_pOther->GetObj()->GetScript<CMinionScript>()->m_GetId()) {
		//		m_bAllienceCol = true;
		//		m_bRotate = true;
		//	}



		//	//if (_pOther->GetObj()->GetScript<CMinionScript>()->GetState() == MINION_STATE::WALK) {
		//	//	float Value=_pOther->Transform()->GetLocalPos().x - Transform()->GetLocalPos().x;
		//	//	if (Value >= 0) {
		//	//		_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
		//	//		GetObj()->GetScript<CMinionScript>()->SetSeparate(true);
		//	//	}
		//	//	else {
		//	//		GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
		//	//		_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(true);
		//	//	}
		//	//
		//	//	}
		//	}
		//	m_iAllienceCol += 1;
		//}
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

	if (_Layer == L"Red") {
		wcout << m_pTarget->GetName() << endl;
	}
	if (nullptr == m_pTarget)
		return;
	if (m_pTarget->IsDead())
		return;
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CProjectileScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(500.f, 500.f, 500.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->GetScript<CProjectileScript>()->SetObject(GetObj());
	pObject->GetScript<CProjectileScript>()->SetDamage(m_uiAttackDamage);
	pObject->GetScript<CProjectileScript>()->SetProjectileType(PROJECTILE_TYPE::MINION);
	pObject->GetScript<CProjectileScript>()->SetCamp(m_eCamp);
	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().z, m_pTarget->Transform()->GetWorldPos().z));
	}
	else {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().y, m_pTarget->Transform()->GetWorldPos().z));
	}

	pObject->Transform()->SetLocalPos(Vec3(GetObj()->Transform()->GetWorldPos().x, GetObj()->Transform()->GetWorldPos().y + GetObj()->Collider3D()->GetOffsetPos().y, GetObj()->Transform()->GetWorldPos().z));
	Vec3 vTargetPos = m_pTarget->Transform()->GetLocalPos() + Vec3(0.f, m_pTarget->Collider3D()->GetOffsetScale().y, 0.f);
	Vec3 vPos = GetObj()->Transform()->GetLocalPos() + Vec3(0.f, GetObj()->Transform()->Collider3D()->GetOffsetScale().y, 0.f);
	Vec3 vDir = vTargetPos - vPos;
	vDir = vDir.Normalize();
	pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.0f, 1.f));
	pObject->GetScript<CProjectileScript>()->SetDir(vDir);
	//근데 미니언이 공격을하고 회전을하면
	// 공격때 만든 Front는 사실 뒤로날리는거다. 
	// 
	pObject->GetScript<CProjectileScript>()->Init();


	CreateObject(index ,pObject, _Layer);

}