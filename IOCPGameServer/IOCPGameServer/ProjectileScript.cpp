#include "pch.h"
#include "ProjectileScript.h"
#include "GameObject.h"
#include "Collider3D.h"
#include "MinionScript.h"
#include "TimeMgr.h"
#include "Server.h"
#include "SceneMgr.h"
#include "TowerScript.h"
#include "PlayerScript.h"

void CProjectileScript::Update()
{
	if (m_pObject->IsDead()) {
		DeleteObject(GetObj());
		return;
	}
	if (DeleteTime < std::chrono::high_resolution_clock::now()) {
		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
		DeleteObject(GetObj());
	}

	Vec3 vLocalPos = Transform()->GetLocalPos();
	Vec3 vRestoreLocalPos = vLocalPos;
	
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vWorldDir = m_pObject->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vLocalDir = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);




	if (m_pObject != nullptr) {
		if (m_pObject->GetScript<CMinionScript>() != nullptr) {
			if (m_pObject->GetScript<CMinionScript>()->GetState() == MINION_STATE::WALK) {
				CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
				DeleteObject(GetObj());
			}
		}
	}
	else {
		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
		DeleteObject(GetObj());
	}

	if (vLocalPos.y < 0) {
		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
		DeleteObject(GetObj());
	}

	if (m_eCamp == CAMP_STATE::RED) {
		for (auto& obj : CSceneMgr::GetInst()->GetCurScene(index)->FindLayer(L"Blue")->GetParentObj())
		{
			if (GetObj() != nullptr) {
				if (obj->GetScript<CTowerScript>() != nullptr) {
					if (obj->GetScript<CTowerScript>()->GetCamp() != m_eCamp) {
						if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 120) {
							obj->GetScript<CTowerScript>()->GetDamage(m_uiDamage);
							CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
							DeleteObject(GetObj());
							break;
						}
					}
				}

				if (obj->GetScript<CMinionScript>() != nullptr) {
					if (obj->GetScript<CMinionScript>()->GetCamp() != m_eCamp) {
						if (m_eProjectileType == PROJECTILE_TYPE::TOWER) {
							if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 20) {
								obj->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
								CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
								DeleteObject(GetObj());
								break;
							}
						}
						else if (m_eProjectileType == PROJECTILE_TYPE::MINION) {
							if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 70) {
								obj->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
								CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
								DeleteObject(GetObj());
								break;
							}
						}
					}
				}

				if (obj->GetScript<CPlayerScript>() != nullptr) {
					if (obj->GetScript<CPlayerScript>()->GetCamp() != m_eCamp) {

						if (m_eProjectileType == PROJECTILE_TYPE::TOWER) {
							if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 50) {
								obj->GetScript<CPlayerScript>()->GetDamage(m_uiDamage);
								CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
								DeleteObject(GetObj());
								break;
							}
						}
						else if (m_eProjectileType == PROJECTILE_TYPE::MINION) {
							if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 120) {
								obj->GetScript<CPlayerScript>()->GetDamage(m_uiDamage);
								CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
								DeleteObject(GetObj());
								break;
							}
						}

					}
				}
			}
		}
	}

	if (m_eCamp == CAMP_STATE::BLUE) {
		for (auto& obj : CSceneMgr::GetInst()->GetCurScene(index)->FindLayer(L"Red")->GetParentObj())
		{
			if (GetObj() != nullptr) {
				if (obj->GetScript<CTowerScript>() != nullptr) {
					if (obj->GetScript<CTowerScript>()->GetCamp() != m_eCamp) {
						if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 120) {
							obj->GetScript<CTowerScript>()->GetDamage(m_uiDamage);
							CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
							DeleteObject(GetObj());
							break;
						}
					}
				}

				if (obj->GetScript<CMinionScript>() != nullptr) {
					if (obj->GetScript<CMinionScript>()->GetCamp() != m_eCamp) {
						if (m_eProjectileType == PROJECTILE_TYPE::TOWER) {
							if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 20) {
								obj->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
								CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
								DeleteObject(GetObj());
								break;
							}
						}
						else if (m_eProjectileType == PROJECTILE_TYPE::MINION) {
							if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 70) {
								obj->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
								CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
								DeleteObject(GetObj());
								break;
							}
						}
					}
				}

				if (obj->GetScript<CPlayerScript>() != nullptr) {
					if (obj->GetScript<CPlayerScript>()->GetCamp() != m_eCamp) {

						if (m_eProjectileType == PROJECTILE_TYPE::TOWER) {
							if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 20) {
								obj->GetScript<CPlayerScript>()->GetDamage(m_uiDamage);
								CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
								DeleteObject(GetObj());
								break;
							}
						}
						else if (m_eProjectileType == PROJECTILE_TYPE::MINION) {
							if (Vec3::Distance(obj->Transform()->GetLocalPos(), vLocalPos) < 120) {
								obj->GetScript<CPlayerScript>()->GetDamage(m_uiDamage);
								CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
								DeleteObject(GetObj());
								break;
							}
						}

					}
				}
			}
		}
	}




	switch (m_eProjectileType)
	{
	case PROJECTILE_TYPE::MINION:
	{
		//tMTBone* p = const_cast<tMTBone*>(m_pObject->MeshRender()->GetMesh()->GetBone(m_iBone));
		//int d = m_pObject->Animator3D()->GetFrameIdx();
		//if (p->vecKeyFrame.size() < d) {
		//	return;
		//}

		if (!m_bRotate) {
			Vec3 vRot = Transform()->GetLocalRot();
			float angle = atan2(vLocalPos.x - m_vTargetPos.x, vLocalPos.z - m_vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;

			Transform()->SetLocalRot(vRot);
			m_bRotate = true;
		}

		if (vLocalPos.z != 0.f && vLocalPos.x != 0.f) {
			Vec3 vvalue = vLocalPos - m_vTargetPos;
			vvalue.Normalize();
			int b = 0;
		}
		int a = 0;
		float xvalue = m_vDir.x * 100.f * DT;

		vLocalPos += m_vDir * 500.f * DT;
		//vLocalPos.y = vRestoreLocalPos.y;
		
	}

	break;
	case PROJECTILE_TYPE::TOWER:
	{

		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vRot = Transform()->GetLocalRot();

		//cout <<"["<<m_GetId()<<"]"  <<m_pTarget->GetScript<CMinionScript>()->m_GetId() << endl;
		float angle = atan2(vPos.y - m_vTargetPos.y, vPos.z - m_vTargetPos.z) * (180 / PI);
		float rotate = angle * 0.0174532925f;
		vRot.x = rotate;
		Transform()->SetLocalRot(vRot);
		//SetDir(vRot);

		if (!m_bRotate) {
			Vec3 vRot = Transform()->GetLocalRot();
			float angle = atan2(vPos.x - m_vTargetPos.x, vPos.z - m_vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;

			Transform()->SetLocalRot(vRot);
			m_bRotate = true;
		}

		if (vPos.z != 0.f && vPos.x != 0.f) {
			Vec3 vvalue = vPos - m_vTargetPos;
			vvalue.Normalize();
			int b = 0;
		}
		int a = 0;
		float xvalue = m_vDir.x * 100.f * DT;

		vLocalPos += m_vDir * 100;
	}
	break;
	default:
		break;
	}

	



	//if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr) {
	//	if (nullptr != _pOther->GetObj()) {
	//		if (m_pObject->GetScript<CMinionScript>() != nullptr) {
	//			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() != m_eCamp)
	//			{
	//				_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
	//				CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//				DeleteObject(GetObj());
	//			}
	//		}
	//		// 타워 -> 미니언
	//		if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr && m_eProjectileType == PROJECTILE_TYPE::TOWER) {
	//			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() != m_eCamp) {
	//				_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
	//				CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//				DeleteObject(GetObj());
	//			}
	//		}
	//	}

	//}

	//// 미니언 -> 타워
	//if (_pOther->GetObj()->GetScript<CTowerScript>() != nullptr && m_eProjectileType == PROJECTILE_TYPE::MINION) {
	//	if (_pOther->GetObj()->GetScript<CTowerScript>()->GetCamp() != m_eCamp) {
	//		_pOther->GetObj()->GetScript<CTowerScript>()->GetDamage(m_uiDamage);
	//		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//		DeleteObject(GetObj());
	//	}
	//}

	//// 타워 -> 플레이어
	//if (_pOther->GetObj()->GetScript<CPlayerScript>() != nullptr && m_eProjectileType == PROJECTILE_TYPE::TOWER) {
	//	if (_pOther->GetObj()->GetScript<CPlayerScript>()->GetCamp() != m_eCamp) {
	//		_pOther->GetObj()->GetScript<CPlayerScript>()->GetDamage(m_uiDamage);
	//		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//		DeleteObject(GetObj());
	//	}
	//}

	//// 미니언 -> 플레이어
	//if (_pOther->GetObj()->GetScript<CPlayerScript>() != nullptr && m_eProjectileType == PROJECTILE_TYPE::MINION) {
	//	cout << "플레이어 충돌" << endl;
	//	if (_pOther->GetObj()->GetScript<CPlayerScript>()->GetCamp() != m_eCamp) {
	//		_pOther->GetObj()->GetScript<CPlayerScript>()->GetDamage(m_uiDamage);
	//		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//		DeleteObject(GetObj());
	//	}
	//}


















	Transform()->SetLocalPos(vLocalPos);
	SHARED_DATA::g_bullet[m_GetId()].x = vLocalPos.x;
	SHARED_DATA::g_bullet[m_GetId()].y = vLocalPos.y;
	SHARED_DATA::g_bullet[m_GetId()].z = vLocalPos.z;

	CServer::GetInst()->send_projectile_packet(m_GetId(), 1);

}


void CProjectileScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	//if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr) {
	//	if (nullptr != _pOther->GetObj()) {
	//		if (m_pObject->GetScript<CMinionScript>() != nullptr) {
	//			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() != m_eCamp)
	//			{
	//				_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
	//				CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//				DeleteObject(GetObj());
	//			}
	//		}

	//		// 타워 -> 미니언
	//		if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr && m_eProjectileType == PROJECTILE_TYPE::TOWER) {
	//			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() != m_eCamp) {
	//				_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
	//				CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//				DeleteObject(GetObj());
	//			}
	//		}
	//	}

	//}



	//// 미니언 -> 타워
	//if (_pOther->GetObj()->GetScript<CTowerScript>() != nullptr && m_eProjectileType == PROJECTILE_TYPE::MINION) {
	//	if (_pOther->GetObj()->GetScript<CTowerScript>()->GetCamp() != m_eCamp) {
	//		_pOther->GetObj()->GetScript<CTowerScript>()->GetDamage(m_uiDamage);
	//		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//		DeleteObject(GetObj());
	//	}
	//}

	//// 타워 -> 플레이어
	//if (_pOther->GetObj()->GetScript<CPlayerScript>() != nullptr && m_eProjectileType == PROJECTILE_TYPE::TOWER) {
	//	if (_pOther->GetObj()->GetScript<CPlayerScript>()->GetCamp() != m_eCamp) {
	//		_pOther->GetObj()->GetScript<CPlayerScript>()->GetDamage(m_uiDamage);
	//		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//		DeleteObject(GetObj());
	//	}
	//}

	//// 미니언 -> 플레이어
	//if (_pOther->GetObj()->GetScript<CPlayerScript>() != nullptr && m_eProjectileType == PROJECTILE_TYPE::MINION) {
	//	cout << "플레이어 충돌" << endl;
	//	if (_pOther->GetObj()->GetScript<CPlayerScript>()->GetCamp() != m_eCamp) {
	//		_pOther->GetObj()->GetScript<CPlayerScript>()->GetDamage(m_uiDamage);
	//		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
	//		DeleteObject(GetObj());
	//	}
	//}
}

void CProjectileScript::Init()
{
	m_uiDamage = 100;
	m_SetId(SHARED_DATA::g_bulletindex);
	SHARED_DATA::g_bullet[m_GetId()] = Transform()->GetLocalPos();
	CServer::GetInst()->send_projectile_packet(m_GetId(), 0);

	if (m_eProjectileType == PROJECTILE_TYPE::TOWER) {
		cout << "Projectile - " << m_GetId() << "Create - Pos ["
			<< SHARED_DATA::g_bullet[m_GetId()].x << ","
			<< SHARED_DATA::g_bullet[m_GetId()].y << ","
			<< SHARED_DATA::g_bullet[m_GetId()].z << "]" << endl;
	}
	SHARED_DATA::g_bulletindex++;

	DeleteTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(3000);

}

CProjectileScript::CProjectileScript() :CScript((UINT)SCRIPT_TYPE::PROJECTILESCRIPT), m_pObject(nullptr), m_fSpeed(), m_uiDamage(), m_fAlpha(0.f)
, m_bRotate(false), m_eProjectileType(PROJECTILE_TYPE::MINION)
{
}

CProjectileScript::~CProjectileScript()
{
}