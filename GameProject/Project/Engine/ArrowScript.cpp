#include "pch.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "PlayerScript.h"
#include "ParticleSystem.h"
#include "SceneMgr.h"
#include "SkillMgr.h"
#include "TrailRenderer.h"
#include <math.h>
#include "Network.h"
#include "TowerScript.h"
#include "CTowerColScript.h"
#include "PlayerColScript.h"
#include "BoxColScript.h"
#include "CameraScript.h"

void CArrowScript::SetSkill(SKILL* _pSkill)
{
    m_pSkill = new SKILL;
    m_pSkill->DotDamage = _pSkill->DotDamage;
    m_pSkill->Code = _pSkill->Code;
    m_pSkill->Name = _pSkill->Name;
    m_pSkill->eSkillType = _pSkill->eSkillType;
    m_pSkill->eElementType = _pSkill->eElementType;

    m_pSkill->fCoolTime = _pSkill->fCoolTime;
    m_pSkill->fDuration = _pSkill->fDuration;
    m_pSkill->fDamage = _pSkill->fDamage;
    m_pSkill->bUse = false;
    m_pSkill->bFinal = false;
    m_pSkill->Count = 0;
    m_pSkill->Sum = _pSkill->Sum;

}
void CArrowScript::Awake()
{
    m_pSound = SetSound2D(L"Sound\\Hit.wav");
    CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
    m_pParticle = new CGameObject;
    m_pParticle->SetName(L"Particle");
    m_pParticle->AddComponent(new CTransform);
    switch (m_iType)
    {
    case ELEMENT_TYPE::WATER:
        m_pParticle->AddComponent(new CParticleSystem);
        m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Bubble99"), L"ParticleUpdateMtrl");
        m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.4f, 0.4f, 0.8f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
        m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 1.f, 1.0f));
        m_pParticle->ParticleSystem()->SetStartScale(2.f);
        m_pParticle->ParticleSystem()->SetEndScale(5.f);


        break;
    case ELEMENT_TYPE::FIRE:
        m_pParticle->AddComponent(new CParticleSystem);
        m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"particle_00"));
        m_pParticle->ParticleSystem()->SetStartColor(Vec4(1.f, 1.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
        m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 0.f, 0.f, 1.0f));
        m_pParticle->ParticleSystem()->SetStartScale(2.f);
        m_pParticle->ParticleSystem()->SetEndScale(5.f);
        break;
    case ELEMENT_TYPE::DARK:
        m_pParticle->AddComponent(new CParticleSystem);
        m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"smokeparticle"), L"ParticleUpdateMtrl");
        m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
        m_pParticle->ParticleSystem()->SetEndColor(Vec4(0.f, 0.f, 0.f, 1.0f));
        m_pParticle->ParticleSystem()->SetStartScale(2.f);
        m_pParticle->ParticleSystem()->SetEndScale(5.f);
        break;
    case ELEMENT_TYPE::THUNDER:
        m_pParticle->AddComponent(new CParticleSystem);
        m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"smokeparticle"), L"ParticleUpdateMtrl");
        m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
        m_pParticle->ParticleSystem()->SetEndColor(Vec4(0.f, 0.f, 0.f, 1.0f));
        m_pParticle->ParticleSystem()->SetStartScale(2.f);
        m_pParticle->ParticleSystem()->SetEndScale(5.f);
        break;
    case ELEMENT_TYPE::WIND:
        m_pParticle->AddComponent(new CParticleSystem);
        m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"smokeparticle"), L"ParticleUpdateMtrl");
        break;


    }
    m_pParticle->SetActive(false);
    m_pParticle->FrustumCheck(false);
    m_pParticle->Transform()->SetLocalPos(Vec3(0.5f, 0.f, 0.f));
    GetObj()->AddChild(m_pParticle);


    m_eState = ARROW_STATE::IDLE;

    m_pTrail = new CGameObject;
    m_pTrail->SetName(L"ArrowTrail");
    m_pTrail->AddComponent(new CTransform);
    m_pTrail->AddComponent(new CMeshRender);
    m_pTrail->AddComponent(new CTrailRenderer);
    m_pTrail->TrailRenderer()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Sparks"));
    m_pTrail->TrailRenderer()->SetColor(Vec4(0.8f, 0.f, 0.f, 0.1f));   // 레드 0.8f, 0.f, 0.f, 0.1f // 블루 0.f, 0.f, 0.8f, 0.1f
    m_pTrail->TrailRenderer()->SetMaxWidth(3.f);
    m_pTrail->TrailRenderer()->SetMinWidth(3.f);
    m_pTrail->TrailRenderer()->SetEmit(false);
    m_pTrail->SetActive(true);
    m_pTrail->FrustumCheck(false);
    m_pTrail->MeshRender()->SetDynamicShadow(false);
    m_pTrail->TrailRenderer()->SetTargetObj(GetObj());
    m_pTrail->TrailRenderer()->SetCamera(pCurScene->FindLayer(L"Default")->GetParentObj()[1]->GetChild()[0]);
    pCurScene->FindLayer(L"Default")->AddGameObject(m_pTrail);

}


void CArrowScript::SkillCheck()
{
    if (nullptr == m_pParticle)return;
    if (nullptr != m_pSkill) {
        m_pParticle->SetActive(true);
    }
    else {
        m_pParticle->SetActive(false);
        return;
    }

}
void CArrowScript::Update()
{

    SkillCheck();

    Vec3 vPos = Transform()->GetLocalPos();
    Vec3 vRot = Transform()->GetLocalRot();

    // 추후에 카메라 위아래 움직임에 따라 발사 방향 바꿀 때 필요
    //CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
    //CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];

    switch (m_eState)
    {
    case ARROW_STATE::IDLE:
    {
        //Init();
        //GetObj()->SetActive(false);
    }
    break;
    case ARROW_STATE::ATTACK_READY:
    {
        if (m_isMain) {
            if (!m_bMaxCharged) {
                Vec3 vRestorePos = vPos;
                vPos.x += 30.f * DT;         // 15.f
            }
            Transform()->SetLocalPos(vPos);
        }
    }
    break;
    case ARROW_STATE::ATTACK:
    {

        if (m_isMain) {
            if (Transform()->GetWorldPos().y < 0.f)
            {
                cout << "DOWN -" << m_id << endl;

                m_pTrail->TrailRenderer()->SetEmit(false);      // 여기요 여기요 여기요

                m_eState = ARROW_STATE::IDLE;
                GetObj()->Transform()->SetLocalPos(Vec3(-1000, -1000, -1000));
                //GetObj()->MeshRender()->SetRender(false);
                m_fTime = 0;
                if (nullptr != m_pSkill)
                {
                    delete m_pSkill;
                    m_pSkill = nullptr;
                    m_pParticle->SetActive(false);
                }
                Network::GetInst()->send_collision_arrow(m_id, 999, PACKET_COLLTYPE::WALL, m_eCamp);
                //vPos.y = 5.f;
                //Transform()->SetLocalPos(vPos);
                //Transform()->SetLocalRot(vRot);
            }
            else {

                m_vRestorePos = vPos;
                Vec4 vDir = Vec4(m_vDir, 1.f);
                float a = vDir.x;
                vDir.x = vDir.z;
                vDir.z = a;
                m_pParticle->ParticleSystem()->SetDir(vDir);

                // 트레일
                m_pTrail->TrailRenderer()->SetEmit(true);

                // 화살 기존 코드

                m_vXZDir = Vec3(m_vDir.x, 0.f, m_vDir.z);
                m_vXZDir.Normalize();
                m_fAngle = acos(Dot(m_vDir, m_vXZDir));
                float fGravity;
                m_fTime += DT;


                // 바람스킬
                if (m_pSkill != nullptr && (UINT)SKILL_CODE::WIND_0 == m_pSkill->Code)
                {
                    vPos.x += m_vXZDir.x * m_fSpeed * m_fTime;
                    vPos.z += m_vXZDir.z * m_fSpeed * m_fTime;
                    Transform()->SetLocalPos(vPos);
                    CGameObject* pCamera = CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->GetParentObj()[1]->GetChild()[0];
                    pCamera->GetScript<CCameraScript>()->SetWind0Camera (false);
                    if (m_fVelocityY == 5000) {
                        m_fVelocityY += 1;
                    }
                    if (Vec3::Distance(vPos, m_vRestorePos) > 3000.f)
                    {
                        m_pTrail->TrailRenderer()->SetEmit(false);      // 여기요 여기요 여기요

                        if (nullptr != m_pSkill)
                        {
                            delete m_pSkill;
                            m_pSkill = nullptr;
                            m_pParticle->SetActive(false);
                        }
                        m_eState = ARROW_STATE::IDLE;
                        GetObj()->Transform()->SetLocalPos(Vec3(-1000, -1000, -1000));
                        //GetObj()->MeshRender()->SetRender(false);
                        m_fTime = 0;

                        Network::GetInst()->send_collision_arrow(m_id, 999, PACKET_COLLTYPE::WALL, m_eCamp);
                    }
                }
                else
                {
                    float fDegree = XMConvertToDegrees(m_fAngle);
                    if (fDegree <= 25.f) {
                        fDegree -= 20.f;
                        fGravity = GRAVITY * 200;
                    }
                    else if (fDegree > 25.f && fDegree <= 31.f) {
                        fDegree -= 20.f;
                        fGravity = GRAVITY * 600;
                    }
                    else {
                        fDegree -= 25.f;;
                        fGravity = GRAVITY * 1000;
                    }
                    m_fAngle = XMConvertToRadians(fDegree);

                    vPos.z = m_vStartPos.z + m_vXZDir.z * (m_fSpeed * m_fTime * cos(m_fAngle)) / 2;
                    vPos.x = m_vStartPos.x + m_vXZDir.x * (m_fSpeed * m_fTime * cos(m_fAngle)) / 2;
                    vPos.y = m_vStartPos.y + ((m_fSpeed * m_fTime * sin(m_fAngle)) - (0.5 * fGravity * m_fTime * m_fTime));
                    //처음 화살 Update
                    if (m_fVelocityY == 5000) {
                        //  현재 포물선 운동을 하고 있는 Y값
                        // 
                        m_fVelocityY = ((m_fSpeed * m_fTime * sin(m_fAngle)) - (0.5 * fGravity * m_fTime * m_fTime));

                        //최고점 높이 == velocity가 0이되는 지점. 
                        m_fHighest = (m_fSpeed * sin(m_fAngle)) * (m_fSpeed * sin(m_fAngle)) / (fGravity * 2);
                        m_fPerRotate = m_fHighest / m_fAngle;
                        Transform()->SetLocalPos(vPos);

                    }
                    else {
                        m_fVelocityY = ((m_fSpeed * m_fTime * sin(m_fAngle)) - (0.5 * fGravity * m_fTime * m_fTime));
                        float fHigh = m_fHighest - m_fVelocityY;
                        m_fRotateAngle = fHigh / m_fPerRotate;
                        if (m_fRotateAngle <= 0.005f && m_fDir == 1) {
                            m_fDir = -1;
                        }
                        m_qRot = Quaternion::CreateFromAxisAngle(m_vQtrnRotAxis, m_fRotateAngle * m_fDir);
                        Transform()->SetQuaternion(m_qRot);
                        Transform()->SetLocalPos(vPos);

                    }
                }

                // 번개스킬, 화염스킬
                if (nullptr != m_pSkill) {
                    if ((UINT)SKILL_CODE::THUNDER_1 == m_pSkill->Code) {
                        if (vPos.y <= 1.f) {

                            Vec3 vPos3 = GetObj()->Transform()->GetLocalPos();
                            vPos3.y = 1.f;
                            if (m_eCamp == CAMP_STATE::BLUE) {
                                cout << "BLUE ARROW" << endl;
                                cout << " ARROW ---" << (int)m_eCamp << endl;
                            }
                            else {
                                cout << " ARROW ---" << (int)m_eCamp << endl;
                            }
                            delete m_pSkill;
                            m_pSkill = nullptr;
                            CreateThunderObject(vPos3, (int)m_eCamp);
                            Network::GetInst()->send_arrow_create_skill(vPos3, PACKET_SKILL::Z_TUNDER);
                            Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
                        }
                    }
                    else if ((UINT)SKILL_CODE::_FIRE_1 == m_pSkill->Code) {
                        if (vPos.y <= 1.f) {
                            Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
                            vPos3.y = 1.f;
                            CreateBoomParticleObject(vPos3, L"smokeparticle");
                            Collision();
                            delete m_pSkill;
                            m_pSkill = nullptr;
                            Network::GetInst()->send_arrow_create_skill(vPos3, PACKET_SKILL::Z_FIRE);
                            Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
                        }
                    }
                }

                //화살 움직임 센드
                Network::GetInst()->send_update_arrow_move(m_id, vPos, m_qRot);

            }
        }
        else {
            LerpUpdate();
        }
    }
    break;
    }
}

void CArrowScript::StopTrail()
{
    m_pTrail->TrailRenderer()->SetEmit(false);
}

void CArrowScript::LerpUpdate()
{
    Vec3 vPos = Transform()->GetLocalPos();
    Vec3 vRot = Transform()->GetLocalRot();
    m_pTrail->TrailRenderer()->SetEmit(true);

    vPos = Vec3::Lerp(vPos, m_LerpPos, DT * 10.f);
    m_qRot = Vec4::Lerp(m_qRot, m_LerpQut, DT * 10.f);


    Transform()->SetLocalPos(vPos);
    Transform()->SetQuaternion(m_qRot);
}

void CArrowScript::EnterSkill(Vec3 vPos)
{
    if (nullptr != m_pSkill) {
        if ((UINT)SKILL_CODE::THUNDER_1 == m_pSkill->Code) {
            Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
            vPos.y = 1.f;

            CreateThunderObject(vPos, (int)m_eCamp);
            //Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
        }
        else if ((UINT)SKILL_CODE::_FIRE_1 == m_pSkill->Code) {
            Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
            vPos.y = 1.f;
            CreateBoomParticleObject(vPos, L"smokeparticle");
            //Collision();
            //Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
        }
    }

    SetState(ARROW_STATE::IDLE);

}

void CArrowScript::Init()
{
    if (nullptr == GetObj()->GetParent()) {
        m_pBow->AddChild(GetObj());
    }

    //m_pTrail->TrailRenderer()->SetEmit(false);

    m_pTrail->TrailRenderer()->SetEmit(false);
    m_bSetDotValue = false;
    m_fVelocityY = 5000;
    m_fFallSpeed = 0.f;
    m_bMaxCharged = false;
    m_fTime = 0.f;
    m_fDir = 1;
    m_fHighest = 0;
    m_fPerRotate = 1;
    Transform()->SetQuaternion(Vec4(0.f, 0.f, 0.f, 1.f));
    Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
    Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(80.f), XMConvertToRadians(0.f)));
    m_pTrail->TrailRenderer()->ClearPoint();

}
#include "Collider3D.h"
#include "MinionScript.h"
void CArrowScript::OnCollision3DEnter(CCollider3D* _pColldier)
{
    if (nullptr != _pColldier->GetObj()->GetScript<CPlayerColScript>() && m_eState == ARROW_STATE::ATTACK) {
        if (_pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->GetCamp() != m_eCamp) {

            if (m_pSkill == nullptr) {
                if (m_pPlayer->GetScript<CPlayerScript>()->GetIsMain())
                    m_pSound->PlaySoundOnce(100.f);
                cout << " Coll Other Camp Player Index - " << _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->m_GetId() << endl;
                m_eState = ARROW_STATE::IDLE;
                m_pTrail->TrailRenderer()->SetEmit(false);      // 여기요 여기요 여기요
                GetObj()->Transform()->SetLocalPos(Vec3(-1000, -1000, -1000));
                _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->GetDamage(500);
                Network::GetInst()->send_collision_arrow(m_id, _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->m_GetId(), PACKET_COLLTYPE::PLAYER, m_eCamp);

            }
            else {
                    if ((UINT)SKILL_CODE::THUNDER_1 == m_pSkill->Code) {
                            Vec3 vPos3 = GetObj()->Transform()->GetLocalPos();
                            vPos3.y = 1.f;
                            if (m_eCamp == CAMP_STATE::BLUE) {
                                cout << "BLUE ARROW" << endl;
                                cout << " ARROW ---" << (int)m_eCamp << endl;
                            }
                            else {
                                cout << " ARROW ---" << (int)m_eCamp << endl;
                            }
                            CreateThunderObject(vPos3, (int)m_eCamp);
                            delete m_pSkill;
                            m_pSkill = nullptr;
                            m_pParticle->SetActive(false);
                            Network::GetInst()->send_arrow_create_skill(vPos3, PACKET_SKILL::Z_TUNDER);
                            Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
                    }
                    else if ((UINT)SKILL_CODE::_FIRE_1 == m_pSkill->Code) {
                            Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
                            vPos3.y = 1.f;
                            CreateBoomParticleObject(vPos3, L"smokeparticle");

                            Collision();
                            Network::GetInst()->send_arrow_create_skill(vPos3, PACKET_SKILL::Z_FIRE);

                            delete m_pSkill;
                            m_pSkill = nullptr;
                            m_pParticle->SetActive(false);
                            Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
                    }
                    else if ((UINT)SKILL_CODE::FIRE_0 == m_pSkill->Code) {
                        _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->GetDamage(500);
                        _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
                        Network::GetInst()->send_arrow_skill(_pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->m_GetId(), PACKET_SKILL::E_FIRE);
                        Network::GetInst()->send_collision_arrow(m_id, _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->m_GetId(), PACKET_COLLTYPE::PLAYER, m_eCamp);

                        delete m_pSkill;
                        m_pSkill = nullptr;
                        m_pParticle->SetActive(false);
                        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
                    }
                    else if ((UINT)SKILL_CODE::THUNDER_0 == m_pSkill->Code) {
                        _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->GetDamage(500);
                        _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
                        Network::GetInst()->send_arrow_skill(_pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->m_GetId(), PACKET_SKILL::E_THUNDER);
                        Network::GetInst()->send_collision_arrow(m_id, _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->m_GetId(), PACKET_COLLTYPE::PLAYER, m_eCamp);

                        delete m_pSkill;
                        m_pSkill = nullptr;
                        m_pParticle->SetActive(false);
                        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
                    }

                    else if ((UINT)SKILL_CODE::DARK_0 == m_pSkill->Code) {
                        _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->GetDamage(500);
                        _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
                        Network::GetInst()->send_arrow_skill(_pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->m_GetId(), PACKET_SKILL::E_DARK);
                        Network::GetInst()->send_collision_arrow(m_id, _pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->m_GetId(), PACKET_COLLTYPE::PLAYER, m_eCamp);

                        delete m_pSkill;
                        m_pSkill = nullptr;
                        m_pParticle->SetActive(false);
                        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
                    }

            }
        }
    }
    else if (nullptr != _pColldier->GetObj()->GetScript<CMinionScript>() && m_eState == ARROW_STATE::ATTACK) {
        if (_pColldier->GetObj()->GetScript<CMinionScript>()->GetCamp() != m_eCamp) {
            if (m_pSkill == nullptr) {

                if (m_pPlayer->GetScript<CPlayerScript>()->GetIsMain())
                    m_pSound->PlaySoundOnce(100.f);
                cout << " Coll Other Camp Minion Index - " << _pColldier->GetObj()->GetScript<CMinionScript>()->m_GetId() << endl;
                m_eState = ARROW_STATE::IDLE;
                m_pTrail->TrailRenderer()->SetEmit(false);      // 여기요 여기요 여기요
                GetObj()->Transform()->SetLocalPos(Vec3(-1000, -1000, -1000));
                Network::GetInst()->send_collision_arrow(m_id, _pColldier->GetObj()->GetScript<CMinionScript>()->m_GetId(), PACKET_COLLTYPE::MONSTER, m_eCamp);
            }
            else {
                if ((UINT)SKILL_CODE::THUNDER_1 == m_pSkill->Code) {
                    Vec3 vPos3 = GetObj()->Transform()->GetLocalPos();
                    vPos3.y = 1.f;
                    if (m_eCamp == CAMP_STATE::BLUE) {
                        cout << "BLUE ARROW" << endl;
                        cout << " ARROW ---" << (int)m_eCamp << endl;
                    }
                    else {
                        cout << " ARROW ---" << (int)m_eCamp << endl;
                    }
                    CreateThunderObject(vPos3, (int)m_eCamp);
                    Network::GetInst()->send_arrow_create_skill(vPos3, PACKET_SKILL::Z_TUNDER);
                    delete m_pSkill;
                    m_pSkill = nullptr;
                    m_pParticle->SetActive(false);
                    Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
                }
                else if ((UINT)SKILL_CODE::_FIRE_1 == m_pSkill->Code) {
                    Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
                    vPos3.y = 1.f;
                    CreateBoomParticleObject(vPos3, L"smokeparticle");
                    Network::GetInst()->send_arrow_create_skill(vPos3, PACKET_SKILL::Z_FIRE);
                    Collision();

                    delete m_pSkill;
                    m_pSkill = nullptr;
                    m_pParticle->SetActive(false);
                    Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));

                }
            }
        }
    }
    else if (nullptr != _pColldier->GetObj()->GetScript<CTowerColScript>()) {
        if (m_pPlayer->GetScript<CPlayerScript>()->GetIsMain()) {
            m_pSound->PlaySoundOnce(100.f);
            cout << "tower collision on~!" << endl;
            Network::GetInst()->send_collision_arrow(m_id, _pColldier->GetObj()->GetScript<CTowerColScript>()->GetTower()->GetScript<CTowerScript>()->m_GetId(),
                PACKET_COLLTYPE::TOWER, m_eCamp);
        }
    }

    if (nullptr == m_pSkill) {
        if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
            if (nullptr != _pColldier->GetObj()->GetScript<CPlayerColScript>()) {
                cout << "_pColldier->GetObj()->GetScript<CPlayerColScript>()->GetPlayer()->GetScript<CPlayerScript>()->SetDamage(m_iDamage)" << endl;
            }
            else if (nullptr != _pColldier->GetObj()->GetScript<CTowerColScript>()) {
                cout << "_pColldier->GetObj()->GetScript<CTowerColScript>()->GetTower()->GetScript<CTowerScript>()->GetDamage(m_iDamage)" << endl;
            }
            else if (nullptr != _pColldier->GetObj()->GetScript<CMinionScript>()) {
                cout << "_pColldier->GetObj()->GetScript<CMinionScript>()->SetDamage(m_iDamage)" << endl;
            }
            else if (L"obstacle" == _pColldier->GetObj()->GetName()) {
                GetObj()->SetActive(false);
                m_pParticle->SetActive(false);

            }

        }
    }
}

CArrowScript::CArrowScript(ELEMENT_TYPE _iType) :CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT), m_iType(_iType), m_bMove(true)
{
}

CArrowScript::~CArrowScript()
{
    if (nullptr != m_pSkill)
    {
        delete m_pSkill;
        m_pSkill = nullptr;
    }

    if (nullptr != m_pSound) {
        delete m_pSound;
        m_pSound = nullptr;
    }

}

void CArrowScript::WaterSkill0(CCollider3D* _pCollider) {
    if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
        _pCollider->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    }

}

void CArrowScript::DarkSkill0(CCollider3D* _pCollider)
{
    if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
        _pCollider->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    }
}

void CArrowScript::ThunderSkill0(CCollider3D* _pCollider)
{
    if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
        _pCollider->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    }
}

void CArrowScript::ThunderSkill1(CCollider3D* _pCollider)
{
    Vec3 vPos = GetObj()->Transform()->GetWorldPos();
    vPos.y = 1.f;
    CreateThunderObject(vPos, m_iLayerIdx);
    if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
        _pCollider->GetObj()->GetScript<CPlayerScript>()->SetDamage(m_iDamage);
        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    }
    //else if (nullptr != _pCollider->GetObj()->GetScript<CMinionScript>()) {
    //   _pCollider->GetObj()->GetScript<CMinionScript>()->SetDamage(m_iDamage);
    //   Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    //}
    //else if (nullptr != _pCollider->GetObj()->GetScript<CTowerScript>()) {
    //   _pCollider->GetObj()->GetScript<CTowerScript>()->GetDamage(m_iDamage);
    //   Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    //}
}

void CArrowScript::FireSkill0(CCollider3D* _pCollider)
{
    if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
        _pCollider->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    }
}

void CArrowScript::FireSkill1(CCollider3D* _pCollider)
{
    if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
        Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
        CreateBoomParticleObject(vPos3, L"smokeparticle");
        //Collision();
        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    }

}

void CArrowScript::WindSkill0(CCollider3D* _pCollider)
{
    if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>())
        _pCollider->GetObj()->GetScript<CPlayerScript>()->SetDamage(m_pSkill->fDamage);
    else {
        Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
    }

}

void CArrowScript::WindSkill1(CCollider3D* _pCollider)
{
    //if (nullptr != _pCollider->GetObj()->GetScript<CMinionScript>()) {
    //   _pCollider->GetObj()->GetScript<CMinionScript>()->SetDamage(m_pSkill->fDamage);
    //}
    //else if (nullptr != _pCollider->GetObj()->GetScript<CTowerScript>()) {
    //   _pCollider->GetObj()->GetScript<CTowerScript>()->GetDamage(m_iDamage);

    //}
    //else if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
    //   _pCollider->GetObj()->GetScript<CPlayerScript>()->SetDamage(m_pSkill->fDamage);

    //}

}

// BoomBoomBoomBoomBoomBoomBoomBoomBommBoomBoom
void CArrowScript::Collision()
{
    CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

    //블루에 플레이어 있고 블루 레드 따로 미니언있음.

    //플레이어 충돌
    for (auto obj : pCurScene->FindLayer(L"Blue")->GetParentObj()) {
        if (obj->GetScript<CPlayerScript>() != nullptr) {
            if (obj->GetScript<CPlayerScript>()->GetCamp() != m_eCamp) {
                CGameObject* pObject1 = obj;
                CGameObject* pObject2 = GetObj();
                Vec3 vPos1 = pObject1->Transform()->GetWorldPos();
                Vec3 vPos2 = pObject2->Transform()->GetWorldPos();
                float Radius = sqrt(pow(vPos1.x - vPos2.x, 2) + pow(vPos1.z - vPos2.z, 2));
                if (Radius < 400.f)
                {
                    obj->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
                    Network::GetInst()->send_set_damage(obj->GetScript<CPlayerScript>()->m_GetId(), 300, PACKET_COLLTYPE::PLAYER, m_eCamp);
                }
         
            }
        }
        if (m_eCamp == CAMP_STATE::RED) {
            if (obj->GetScript<CMinionScript>() != nullptr) {
                if (obj->GetScript<CMinionScript>()->GetCamp() != m_eCamp) {
                    CGameObject* pObject1 = obj;
                    CGameObject* pObject2 = GetObj();
                    Vec3 vPos1 = pObject1->Transform()->GetWorldPos();
                    Vec3 vPos2 = pObject2->Transform()->GetWorldPos();
                    float Radius = sqrt(pow(vPos1.x - vPos2.x, 2) + pow(vPos1.z - vPos2.z, 2));
                    if (Radius < 400.f)
                    {
                        Network::GetInst()->send_set_damage(obj->GetScript<CMinionScript>()->m_GetId(), 350 ,PACKET_COLLTYPE::MONSTER, m_eCamp);

                    }
                }
            }
        }
    }


    if (m_eCamp == CAMP_STATE::BLUE) {
        for (auto obj : pCurScene->FindLayer(L"Red")->GetParentObj()) {
            if (obj->GetScript<CMinionScript>() != nullptr) {
                if (obj->GetScript<CMinionScript>()->GetCamp() != m_eCamp) {
                    CGameObject* pObject1 = obj;
                    CGameObject* pObject2 = GetObj();
                    Vec3 vPos1 = pObject1->Transform()->GetWorldPos();
                    Vec3 vPos2 = pObject2->Transform()->GetWorldPos();
                    float Radius = sqrt(pow(vPos1.x - vPos2.x, 2) + pow(vPos1.z - vPos2.z, 2));
                    if (Radius < 400.f)
                    {
                        Network::GetInst()->send_set_damage(obj->GetScript<CMinionScript>()->m_GetId(), 350, PACKET_COLLTYPE::MONSTER, m_eCamp);
                    }
                }
            }
        }
    }




}