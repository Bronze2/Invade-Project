﻿#include "pch.h"
#include "PlayerScript.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "Camera.h"
#include"CameraScript.h"
#include "Animator3D.h"
#include "Network.h"
#include "Collider3D.h"

#include "ProjectileScript.h"
#include "ParticleSystem.h"
#include "SkillMgr.h"

#include "BowScript.h"
//
//void CPlayerScript::m_FAnimation()
//{
//	// 로비씬 RUN과 JUMP 순서 바꿈
//	if (m_ePrevState != m_eState) {
//		// Blend  ʿ 
//		switch (m_eState)
//		{
//		case PLAYER_STATE::IDLE:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::IDLE);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::IDLE, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::IDLE;
//			}
//		}
//		break;
//		case PLAYER_STATE::WALK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::WALK);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::WALK, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::WALK;
//			}
//		}
//		break;
//		case PLAYER_STATE::JUMP:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::JUMP);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::JUMP, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::JUMP;
//			}
//		}
//		break;
//		case PLAYER_STATE::RUN:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::RUN);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::RUN, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::RUN;
//			}
//		}
//		break;
//		case PLAYER_STATE::ATTACK_READY:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::ATTACK_READY;
//				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
//			}
//		}
//		break;
//		case PLAYER_STATE::ATTACK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::ATTACK;
//				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK);
//			}
//		}
//		break;
//		case PLAYER_STATE::ATTACK_READY_HIGH:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY_HIGH);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY_HIGH, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::ATTACK_READY_HIGH;
//				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
//			}
//		}
//		break;
//		case PLAYER_STATE::DEMAGED:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::DEMAGED);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::DEMAGED, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::DEMAGED;
//			}
//		}
//		break;
//		case PLAYER_STATE::DIE:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
//				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
//				GetObj()->Animator3D()->SetBlendState(true);
//				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::DIE);
//				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::DIE, 0.f);
//				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
//				m_ePrevState = PLAYER_STATE::DIE;
//			}
//		}
//		break;
//
//		default:
//			break;
//		}
//	}
//	else {
//		switch (m_eState)
//		{
//		case PLAYER_STATE::IDLE:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
//
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
//						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::IDLE);
//						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
//						GetObj()->Animator3D()->SetCurTime(0.f);
//						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
//					}
//				}
//
//			}
//		}
//		break;
//		case PLAYER_STATE::WALK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
//
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
//						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::WALK);
//						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
//						GetObj()->Animator3D()->SetCurTime(0.f);
//						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
//					}
//				}
//
//			}
//		}
//		break;
//		case PLAYER_STATE::JUMP:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP");
//
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
//						m_eState = PLAYER_STATE::IDLE;
//					}
//				}
//
//			}
//		}
//		break;
//		case PLAYER_STATE::RUN:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN");
//
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
//						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::RUN);
//						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
//						GetObj()->Animator3D()->SetCurTime(0.f);
//						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
//					}
//				}
//
//			}
//		}
//		break;
//		case PLAYER_STATE::ATTACK_READY:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
//						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
//					}
//				}
//			}
//		}
//		break;
//		case PLAYER_STATE::ATTACK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
//						m_eState = PLAYER_STATE::IDLE;
//						GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::IDLE);
//					}
//				}
//			}
//		}
//		break;
//		case PLAYER_STATE::ATTACK_READY_HIGH:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH");
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
//						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
//					}
//				}
//			}
//		}
//		break;
//		case PLAYER_STATE::DEMAGED:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED");
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
//						m_eState = PLAYER_STATE::IDLE;
//					}
//				}
//
//			}
//		}
//		break;
//		case PLAYER_STATE::DIE:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
//				if (!GetObj()->Animator3D()->GetBlendState()) {
//					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
//					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
//						m_eState = PLAYER_STATE::IDLE;
//					}
//				}
//			}
//		}
//		break;
//
//		default:
//			break;
//		}
//	}
//}

void CPlayerScript::m_FAnimation()
{
	// 로비씬 RUN과 JUMP 순서 바꿈
	if (m_ePrevState != m_eState) {
		// Blend  ʿ 
		switch (m_eState)
		{
		case PLAYER_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::IDLE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::IDLE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::IDLE;
			}
		}
		break;
		case PLAYER_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::WALK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::WALK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::WALK;
			}
		}
		break;
		case PLAYER_STATE::JUMP:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::JUMP);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::JUMP, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::JUMP;
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK_READY;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
			}
		}
		break;
		case PLAYER_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK);
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_1:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_1")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_1");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY_HIGH_1);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY_HIGH_1, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK_READY_HIGH_1;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_2:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_2")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_2");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY_HIGH_2);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY_HIGH_2, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK_READY_HIGH_2;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_3:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_3")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_3");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY_HIGH_3);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY_HIGH_3, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK_READY_HIGH_3;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_4:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_4")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_4");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY_HIGH_4);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY_HIGH_4, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK_READY_HIGH_4;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_5:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_5")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_5");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY_HIGH_5);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY_HIGH_5, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK_READY_HIGH_5;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
			}
		}
		break;
		case PLAYER_STATE::DEMAGED:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::DEMAGED);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::DEMAGED, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::DEMAGED;
			}
		}
		break;
		case PLAYER_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::DIE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::DIE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::DIE;
			}
		}
		break;
		case PLAYER_STATE::RUN:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::RUN);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::RUN, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::RUN;
			}
		}
		break;
		default:
			break;
		}
	}
	else {
		switch (m_eState)
		{
		case PLAYER_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::IDLE);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					}
				}

			}
		}
		break;
		case PLAYER_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::WALK);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					}
				}

			}
		}
		break;
		case PLAYER_STATE::JUMP:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						m_eState = PLAYER_STATE::IDLE;
					}
				}

			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
				if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
				if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
					m_eState = PLAYER_STATE::IDLE;
					GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::IDLE);
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_1:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_1")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_1");
				if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_2:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_2")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_2");
				if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_3:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_3")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_3");
				if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_4:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_4")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_4");
				if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY_HIGH_5:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_5")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY_HIGH_5");
				if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
				}
			}
		}
		break;
		case PLAYER_STATE::DEMAGED:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						m_eState = PLAYER_STATE::IDLE;
					}
				}

			}
		}
		break;
		case PLAYER_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						// 누워있는 상태로 멈춰이아어리ㅏ어리어라ㅓ이리ㅏㅓ
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
						//m_eState = PLAYER_STATE::IDLE;
					}
				}
			}
		}
		break;
		case PLAYER_STATE::RUN:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::RUN);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					}
				}

			}
		}
		break;
		default:
			break;
		}
	}
}
void CPlayerScript::Init()
{
	m_eState = PLAYER_STATE::IDLE;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
		m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
		GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::IDLE);
		GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
		m_eState = PLAYER_STATE::IDLE;
		m_ePrevState = PLAYER_STATE::IDLE;
	}
}

void CPlayerScript::Awake()
{
	cout << "PlayerAwake - [" << m_id << "]" << endl;
	m_fMoveSpeed = 500.f;

	m_pHealParticle = new CGameObject;
	m_pHealParticle->AddComponent(new CTransform);
	m_pHealParticle->AddComponent(new CParticleSystem);
	m_pHealParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"HardRain2"), L"ParticleUpdate4Mtrl");
	m_pHealParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.5f, 0.5f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	m_pHealParticle->ParticleSystem()->SetEndColor(Vec4(0.f, 0.8f, 1.f, 1.0f));
	m_pHealParticle->ParticleSystem()->SetStartScale(10.f);
	m_pHealParticle->ParticleSystem()->SetEndScale(15.f);
	m_pHealParticle->FrustumCheck(false);
	m_pHealParticle->SetActive(false);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pHealParticle);
	GetObj()->AddChild(m_pHealParticle);


	m_pFlameParticle = new CGameObject;
	m_pFlameParticle->AddComponent(new CTransform);
	m_pFlameParticle->AddComponent(new CParticleSystem);
	m_pFlameParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Flame"), L"ParticleUpdate4Mtrl");
	m_pFlameParticle->ParticleSystem()->SetStartColor(Vec4(0.5f, 0.5f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	m_pFlameParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 0.f, 0.f, 1.0f));
	m_pFlameParticle->ParticleSystem()->SetStartScale(10.f);
	m_pFlameParticle->ParticleSystem()->SetEndScale(0.1f);
	m_pFlameParticle->FrustumCheck(false);
	m_pFlameParticle->SetActive(false);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pFlameParticle);
	GetObj()->AddChild(m_pFlameParticle);


	m_pThunderParticle = new CGameObject;
	m_pThunderParticle->AddComponent(new CTransform);
	m_pThunderParticle->AddComponent(new CParticleSystem);
	m_pThunderParticle->Transform()->SetLocalPos(Vec3(0.f, 0.f, 150.f));
	m_pThunderParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Thunder"), L"ParticleUpdate3Mtrl");
	m_pThunderParticle->ParticleSystem()->SetStartColor(Vec4(0.5f, 0.5f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	m_pThunderParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 0.f, 1.0f));
	m_pThunderParticle->ParticleSystem()->SetStartScale(10.f);
	m_pThunderParticle->ParticleSystem()->SetEndScale(10.f);
	m_pThunderParticle->FrustumCheck(false);
	m_pThunderParticle->SetActive(false);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pThunderParticle);
	GetObj()->AddChild(m_pThunderParticle);


	m_fMoveSpeed = 300.f;


	Ptr<CMeshData> pHelmetMesh;

	if (m_eCamp == CAMP_STATE::BLUE) {
		switch (m_iType) {
		case ELEMENT_TYPE::WATER:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\head_vertex.fbx");
			
			break;
		case ELEMENT_TYPE::DARK:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_02_Blue.fbx");
			break;
		case ELEMENT_TYPE::WIND:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_03_Blue.fbx");
			break;
		case ELEMENT_TYPE::THUNDER:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_04_Blue.fbx");
			break;
		case ELEMENT_TYPE::FIRE:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_05_Blue.fbx");
			break;
		}
	}
	else if (m_eCamp == CAMP_STATE::RED) {
		switch (m_iType) {
		case ELEMENT_TYPE::WATER:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_01_Red.fbx");
			break;
		case ELEMENT_TYPE::DARK:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_02_Red.fbx");
			break;
		case ELEMENT_TYPE::WIND:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_03_Red.fbx");
			break;
		case ELEMENT_TYPE::THUNDER:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_04_Red.fbx");
			break;
		case ELEMENT_TYPE::FIRE:
			pHelmetMesh = CResMgr::GetInst()->LoadFBX(L"FBX\\helmet_05_Red.fbx");
			break;
		}
	}
	m_pHelmetObject = pHelmetMesh->Instantiate();
	m_pHelmetObject->MeshRender()->SetDynamicShadow(false);
	m_pHelmetObject->FrustumCheck(false);
	m_pHelmetObject->SetName(L"Helmet");
	m_pHelmetObject->Transform()->SetLocalScale(Vec3(0.1f, 0.09f, 0.1f));
	GetObj()->AddChild(m_pHelmetObject);
}


void CPlayerScript::Update()
{

// Z-up To Y-up
	
	AttachHelmet();
	if (CSceneMgr::GetInst()->GetCurScene()->GetCurScene() == SCENE_TYPE::INGAME) {	
		SkillCoolTimeCheck();
		Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
		Transform()->SetWorldDir(DIR_TYPE::UP, vDirFront);
		Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CGameObject* pEmptyObject = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1]);
		CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];
		CCameraScript* pCameraScript = pCamera->GetScript<CCameraScript>();
		CGameObject* pBow = GetObj()->GetChild()[0];

		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vPos2 = Transform()->GetLocalPos();
		Vec3 vPos3 = Transform()->GetWorldPos();
		Vec3 vRot = Transform()->GetLocalRot();
		Vec3 vRestoreRot;
		int rotydegree = XMConvertToDegrees(vRot.y);
		int reminder = rotydegree % 360 + 90;
		Update_LerpPos();

		if (isMain) {
			if (0 <= reminder && reminder <= 180) {
				m_bCheckDegree = true;
			}
			else {
				m_bCheckDegree = false;
			}
			if (KEY_HOLD(KEY_TYPE::KEY_W) && KEY_HOLD(KEY_TYPE::KEY_A)) {
				int a = 0;
			}

			if ((KEY_TAB(KEY_TYPE::KEY_W) || KEY_TAB(KEY_TYPE::KEY_S) || KEY_TAB(KEY_TYPE::KEY_A) || KEY_TAB(KEY_TYPE::KEY_D)) && KEY_NONE(KEY_TYPE::KEY_LBTN)) {
				m_fFactor = 0.f;
				m_fTurnDegree = 0.f;
				m_bTurn = true;

				m_vRestoreRot = vRot;

			}
			if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_D))) {

				Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
				Vec3 vCollisonCheckPos = vPos;
				vCollisonCheckPos.x += vFront.x * m_fMoveSpeed * DT;
				vCollisonCheckPos.z += vFront.z * m_fMoveSpeed * DT;
				if (MoveCheck(vCollisonCheckPos)) {
					SetLerpPos(vPos);
				}
				else {
					Vec3 vTurnUpFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
					restorePos = Transform()->GetLocalPos();
					Network::GetInst()->send_key_up_packet(0, vFront.x, vFront.y, vFront.z, 1);

				}
				//Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
				//Network::GetInst()->send_key_up_packet(0, vFront.x, vFront.y, vFront.z, 1);
			}

			if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_D)) && KEY_NONE(KEY_TYPE::KEY_LBTN)) {


				m_eState = PLAYER_STATE::IDLE;
				m_fTurnDegree = 0.f;
				m_fFactor = 0.f;
				m_bTurn = true;
				m_vRestoreRot = vRot;

			}

			if ((KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_D)) && KEY_NONE(KEY_TYPE::KEY_LBTN)) {
				m_eState = PLAYER_STATE::WALK;
				if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				{
					m_eState = PLAYER_STATE::RUN;
				}


				Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

				// m_fRotateDegree = 현재 정상적으로 플레이어가 있어야 할 회전값 (W기준 여기로 가야 함)
				m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;

				m_fCurDegree = 0.f;
				m_iKeyHoldCnt = 0;

				if (KEY_HOLD(KEY_TYPE::KEY_W)) {
					m_fCurDegree += 0.f;
					m_iKeyHoldCnt++;
				}
				if (KEY_HOLD(KEY_TYPE::KEY_S)) {
					m_fCurDegree += 180.f;
					m_iKeyHoldCnt++;
				}
				if (KEY_HOLD(KEY_TYPE::KEY_A)) {
					if (KEY_HOLD(KEY_TYPE::KEY_S)) {
						m_fCurDegree += 270.f;
					}
					else {
						m_fCurDegree -= 90.f;
					}
					m_iKeyHoldCnt++;
				}
				if (KEY_HOLD(KEY_TYPE::KEY_D)) {
					m_fCurDegree += 90.f;
					m_iKeyHoldCnt++;
				}

				m_fCurDegree /= (float)m_iKeyHoldCnt;

				if ((KEY_HOLD(KEY_TYPE::KEY_W) && KEY_HOLD(KEY_TYPE::KEY_S)) || (KEY_HOLD(KEY_TYPE::KEY_A) && KEY_HOLD(KEY_TYPE::KEY_D))) {
					m_fCurDegree = 0.f;
				}

				m_fTurnDegree = m_fRotateDegree + m_fCurDegree;

				float fRotDegree = XMConvertToDegrees(m_vRestoreRot.y);
				if (fRotDegree > 360.f) {
					fRotDegree -= 360.f;
				}

				// 180도 이상 회전하는 경우 없도록
				if (abs(fRotDegree - m_fTurnDegree) > 180.f) {
					if (abs(fRotDegree - ((m_fTurnDegree + 360.f) - fRotDegree)) > abs(fRotDegree - ((m_fTurnDegree - 360.f) - fRotDegree))) {
						m_fTurnDegree -= 360.f;
					}
					else {
						m_fTurnDegree += 360.f;
					}
				}
				// 0 -> 270보다는 0 -> -90되는게 이쁘니까
				vRot.y = XMConvertToRadians(fRotDegree * (1.f - m_fFactor) + m_fTurnDegree * m_fFactor);
				if (m_fFactor < 1.f) {
					// 회전 속도 빠르게 할 때 m_fFactor 빠르게 해주면 됨
					m_fFactor += 10.f * DT;
				}
				else {
					m_fFactor = 1.f;
					if ((KEY_HOLD(KEY_TYPE::KEY_W) && KEY_HOLD(KEY_TYPE::KEY_S)) || (KEY_HOLD(KEY_TYPE::KEY_A) && KEY_HOLD(KEY_TYPE::KEY_D))) {
						m_bTurn = true;
					}
					else {
						m_bTurn = false;
					}
				}



				if (!m_bTurn && CTimeMgr::GetInst()->GetPlayerMoveFPS()) {


					Vec3 vCollisonCheckPos = vPos;
					vCollisonCheckPos.x += vFront.x * m_fMoveSpeed * DT;
					vCollisonCheckPos.z += vFront.z * m_fMoveSpeed * DT;
					if (MoveCheck(vCollisonCheckPos)) {
						SetPrevLerpPos(vPos);
						m_bColCheck = true;
					}
					else {
						Vec3 vTurnUpFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
						restorePos = Transform()->GetLocalPos();
						Network::GetInst()->send_rotation_packet(vRot);
						Network::GetInst()->send_key_down_packet(0, vTurnUpFront.x, vTurnUpFront.y, vTurnUpFront.z, 0);
					}

					//m_bColCheck = false;
				}

			}


			Vec3 vCamRot = pCamera->Transform()->GetLocalRot();
			float fCamRotDegree = XMConvertToDegrees(vCamRot.x);

			//if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
			//	m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;
			//	// 공격 시 무조건 카메라가 바라보는 방향으로 플레이어 회전시키기 (화살 개발 이후 주석 풀기)
			//	vRot.y = XMConvertToRadians(m_fRotateDegree + 10.f);   // 5.f 더 회전시킬건지?

			//	if (fCamRotDegree <= -3.f) {
			//		m_eState = PLAYER_STATE::ATTACK_READY_HIGH;
			//		//Network::GetInst()->send_attack_ready_packet(m_GetId(), (int)PLAYER_STATE::ATTACK_READY_HIGH);
			//	}
			//	else {
			//		m_eState = PLAYER_STATE::ATTACK_READY;
			//		Network::GetInst()->send_attack_ready_packet(m_GetId(), (int)PLAYER_STATE::ATTACK_READY);
			//	}
			//	Network::GetInst()->send_rotation_packet(vRot);

			//}

			if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
				m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;
				// 공격 시 무조건 카메라가 바라보는 방향으로 플레이어 회전시키기 (화살 개발 이후 주석 풀기)
				vRot.y = XMConvertToRadians(m_fRotateDegree + 10.f);   // 5.f 더 회전시킬건지?

				// 화살 위아래 애니메이션
				if (fCamRotDegree <= -3.f) {
					m_eState = PLAYER_STATE::ATTACK_READY_HIGH_5;
					Network::GetInst()->send_attack_ready_packet(m_GetId(), (int)PLAYER_STATE::ATTACK_READY_HIGH_5);

				}
				else if (fCamRotDegree <= -1.f) {
					m_eState = PLAYER_STATE::ATTACK_READY_HIGH_4;
					Network::GetInst()->send_attack_ready_packet(m_GetId(), (int)PLAYER_STATE::ATTACK_READY_HIGH_4);

				}
				else if (fCamRotDegree <= 1.f) {
					m_eState = PLAYER_STATE::ATTACK_READY_HIGH_3;
					Network::GetInst()->send_attack_ready_packet(m_GetId(), (int)PLAYER_STATE::ATTACK_READY_HIGH_3);

				}
				else if (fCamRotDegree <= 3.f) {
					m_eState = PLAYER_STATE::ATTACK_READY_HIGH_2;
					Network::GetInst()->send_attack_ready_packet(m_GetId(), (int)PLAYER_STATE::ATTACK_READY_HIGH_2);

				}
				else if (fCamRotDegree <= 5.f) {
					m_eState = PLAYER_STATE::ATTACK_READY_HIGH_1;
					Network::GetInst()->send_attack_ready_packet(m_GetId(), (int)PLAYER_STATE::ATTACK_READY_HIGH_1);

				}
				else {
					m_eState = PLAYER_STATE::ATTACK_READY;
				}
				Network::GetInst()->send_rotation_packet(vRot);
			}
			//if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
			//	m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;
			//	// 공격 시 무조건 카메라가 바라보는 방향으로 플레이어 회전시키기 (화살 개발 이후 주석 풀기)
			//	vRot.y = XMConvertToRadians(m_fRotateDegree);	// 5.f 더 회전시킬건지?
			//	m_eState = PLAYER_STATE::ATTACK_READY;
			//	Network::GetInst()->send_attack_ready_packet(m_GetId(),(int)PLAYER_STATE::ATTACK_READY);
			//	Network::GetInst()->send_rotation_packet(vRot);

			//}

			if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
				m_eState = PLAYER_STATE::ATTACK;

			}


			if (KEY_TAB(KEY_TYPE::KEY_NUM4))
			{
				m_eState = PLAYER_STATE::JUMP;
			}

			if (KEY_TAB(KEY_TYPE::KEY_NUM5))
			{
				m_eState = PLAYER_STATE::DIE;
			}
			Transform()->SetLocalRot(vRot);
			UseSkill();
			GetDamage();
		}
		StatusCheck();

		Update_LerpPos();
	}
	m_FAnimation();
}

#include "InGameScene.h"
#include "CollisionMgr.h"

bool CPlayerScript::MoveCheck(const Vec3& _vPos)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Vec3 vFinalPos = Collider3D()->GetOffsetPos();
	vFinalPos = vFinalPos / Transform()->GetWorldScale();
	Matrix matColTranslation = XMMatrixTranslation(vFinalPos.x, vFinalPos.y, vFinalPos.z);
	Matrix matColScale = XMMatrixScaling(Collider3D()->GetOffsetScale().x, Collider3D()->GetOffsetScale().y, Collider3D()->GetOffsetScale().z);
	Matrix MatColWorld = matColScale * matColTranslation;
	Matrix matTranslation = XMMatrixTranslation(_vPos.x, _vPos.y, _vPos.z);
	Matrix matScale = XMMatrixScaling(Transform()->GetLocalScale().x, Transform()->GetLocalScale().y, Transform()->GetLocalScale().z);
	Matrix matRot = XMMatrixRotationX(Transform()->GetLocalRot().x);
	matRot *= XMMatrixRotationY(Transform()->GetLocalRot().y);
	matRot *= XMMatrixRotationZ(Transform()->GetLocalRot().z);
	Matrix matWorld = matScale * matRot * matTranslation;
	MatColWorld *= matWorld;
	bool bTrue = false;
	for (int i = (UINT)INGAME_LAYER::DEFAULT + 1; i < (UINT)INGAME_LAYER::OBSTACLE + 1; ++i) {
		const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();
		if (i == 6)
			continue;
		for (int j = 0; j < vecObj.size(); ++j) {
			if (vecObj[j] == GetObj())
				continue;
			if (vecObj[j]->IsDead())
				continue;
			if (!vecObj[j]->Collider3D())
				continue;
			if (L"Projectile" == vecObj[j]->GetName())
				continue;
			if (L"Minion" == vecObj[j]->GetName())
				continue;
			if (L"Arrow" == vecObj[j]->GetName())
				continue;

			bTrue = CCollisionMgr::GetInst()->CollisionCubeMatrix(MatColWorld, vecObj[j]->Collider3D()->GetColliderWorldMatNotify());
			if (bTrue) {
				wcout << vecObj[j]->GetName() << endl;
				return bTrue;
			}
		}

	}


	return bTrue;

}

//
//
void CPlayerScript::SetState(int state)
{
	//enum class PLAYER_STATE {
	//	IDLE,
	//	WALK,
	//	RUN,
	//	JUMP,
	//	ATTACK_READY,
	//	ATTACK,
	//	DEMAGED,
	//	DIE,
	//	END,
	//};

	m_eState = (PLAYER_STATE)state;
}

void CPlayerScript::SetType(ELEMENT_TYPE _iType)
{
	m_iType = _iType;

	//스킬 설정 
	switch (m_iType)
	{
	case ELEMENT_TYPE::WATER:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(0);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(1);
	}
	break;
	case ELEMENT_TYPE::DARK:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(4);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(5);
	}
	break;
	case ELEMENT_TYPE::WIND:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(2);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(3);
	}
	break;
	case ELEMENT_TYPE::THUNDER:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(6);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(7);
	}
	break;
	case ELEMENT_TYPE::FIRE:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(8);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(9);
	}
	break;

	default:
		break;
	}



}
void CPlayerScript::SkillCoolTimeCheck()
{
	if (m_tESkill->bUse) {
		if (CoolTimeCheck(m_tESkill->StartTime, m_tESkill->fCoolTime)) {
			m_tESkill->bUse = false;
		}
	}
	if (m_tZSkill->bUse) {
		if (CoolTimeCheck(m_tZSkill->StartTime, m_tZSkill->fCoolTime)) {
			m_tZSkill->bUse = false;
		}
	}


}
void CPlayerScript::StatusCheck()
{
	m_bHealCheck = false;
	m_bFlameCheck = false;
	m_bThunderCheck = false;

	if (m_pHealParticle->IsActive()) {
		for (int i = 0; i < m_arrSkill.size(); ++i) {
			if (m_arrSkill[i]->eElementType == ELEMENT_TYPE::WATER) {
				m_bHealCheck = true;
				break;
			}
		}
		if (!m_bHealCheck) {
			m_pHealParticle->SetActive(false);
		}


	}
	if (m_pFlameParticle->IsActive()) {
		for (int i = 0; i < m_arrSkill.size(); ++i) {
			if (m_arrSkill[i]->eElementType == ELEMENT_TYPE::FIRE) {
				m_bFlameCheck = true;
				break;
			}
		}
		if (!m_bFlameCheck) {
			m_pFlameParticle->SetActive(false);
		}


	}
	if (m_pThunderParticle->IsActive()) {
		for (int i = 0; i < m_arrSkill.size(); ++i) {
			if (m_arrSkill[i]->eElementType == ELEMENT_TYPE::THUNDER) {
				m_bThunderCheck = true;
				break;
			}
		}
		if (!m_bThunderCheck) {
			m_pThunderParticle->SetActive(false);
		}


	}
}
void CPlayerScript::UseSkill()
{
	if (m_bThunderCheck)
		return;
	if (KEY_AWAY(KEY_TYPE::KEY_E)) {
		if (m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->bSetSkill()) {
			if (!m_tESkill->bUse)
			{

				m_tESkill->bUse = true;
				m_tESkill->StartTime = chrono::system_clock::now();
				m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->SetSkill(m_tESkill);
			}
		}
	}
	if (KEY_AWAY(KEY_TYPE::KEY_Z)) {
		if (!m_tZSkill->bUse)
		{
			m_tZSkill->bUse = true;
			m_tZSkill->StartTime = chrono::system_clock::now();
			switch (m_tZSkill->eElementType)
			{
			case ELEMENT_TYPE::WATER:
				for (int i = 0; i < m_arrAlliance.size(); ++i) {
					if (nullptr != m_arrAlliance[i]->GetScript<CPlayerScript>()) {
						m_arrAlliance[i]->GetScript<CPlayerScript>()->DamageBySkill(m_tZSkill);
					}
				}

				DamageBySkill(m_tZSkill);
				break;
			case ELEMENT_TYPE::DARK:
			{

			}
			break;
			case ELEMENT_TYPE::FIRE:
			{
				if (m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->bSetSkill()) {
					m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->SetSkill(m_tZSkill);
					m_currentSkill = m_tZSkill;
					m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->SetPacketSkill(PACKET_SKILL::Z_FIRE);
				}
			}

			break;
			case ELEMENT_TYPE::THUNDER:
			{
				if (m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->bSetSkill()) {
					m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->SetSkill(m_tZSkill);
					m_currentSkill = m_tZSkill;
					m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->SetPacketSkill(PACKET_SKILL::Z_TUNDER);

				}
			}

			break;
			}


		}

	}
}
void CPlayerScript::DamageBySkill(SKILL* _pSkill)
{
	SKILL* pSkill = new SKILL;
	pSkill->DotDamage = _pSkill->DotDamage;
	pSkill->Code = _pSkill->Code;
	pSkill->Name = _pSkill->Name;
	pSkill->eSkillType = _pSkill->eSkillType;
	pSkill->eElementType = _pSkill->eElementType;

	pSkill->fCoolTime = _pSkill->fCoolTime;
	pSkill->fDuration = _pSkill->fDuration;
	pSkill->fDamage = _pSkill->fDamage;
	pSkill->bUse = false;
	pSkill->bFinal = false;
	pSkill->Count = 0;
	pSkill->Sum = _pSkill->Sum;
	m_arrSkill.push_back(pSkill);


}
void CPlayerScript::GetDamage()
{
	for (int i = 0; i < m_arrSkill.size(); ++i) {

		if (m_arrSkill[i]->bFinal)continue;
		if (m_arrSkill[i]->bUse == false) {
			m_arrSkill[i]->StartTime = std::chrono::system_clock::now();
			m_arrSkill[i]->bUse = true;
			switch (m_arrSkill[i]->eElementType)
			{
			case ELEMENT_TYPE::WATER:
				m_pHealParticle->SetActive(true);
				break;
			case ELEMENT_TYPE::FIRE:
				m_pThunderParticle->SetActive(true);
				break;
			default:
				break;
			}
		}
		else {
			if (TickCheck(m_arrSkill[i]->StartTime, m_arrSkill[i]->Count) != m_arrSkill[i]->Count) {
				m_iCurHp -= m_arrSkill[i]->DotDamage;
				m_arrSkill[i]->Sum += m_arrSkill[i]->DotDamage;

				m_arrSkill[i]->Count += 1;
			}

			if (m_arrSkill[i]->Count >= (int)m_arrSkill[i]->fDuration) {
				m_arrSkill[i]->bFinal = true;
				m_iCurHp += m_arrSkill[i]->Sum;

				//m_iCurHp -= 0.01f;
			}
		}
	}
	m_arrSkill.erase(std::remove_if(m_arrSkill.begin(), m_arrSkill.end(), SkillFinalCheck), m_arrSkill.end());
}


void CPlayerScript::OnDetectionEnter(CGameObject* _pOther)
{
	if (_pOther->GetLayerIdx() != GetObj()->GetLayerIdx()) {
	}
	else {
		if (nullptr != _pOther->GetScript<CPlayerScript>())
			m_arrAlliance.push_back(_pOther);
	}

}

void CPlayerScript::OnDetection(CGameObject* _pOther)
{

}

void CPlayerScript::OnDetectionExit(CGameObject* _pOther)
{
	vector<CGameObject*>::iterator iter = m_arrAlliance.begin();
	m_arrAlliance.erase(std::remove(m_arrAlliance.begin(), m_arrAlliance.end(), _pOther), m_arrAlliance.end());

}







void CPlayerScript::Update_LerpPos()
{

		Vec3 vPos = Transform()->GetLocalPos();


		//m_FColCheck(m_PrevLerpPos, m_LerpPos);
		if (m_bMoveCheck)
		{
			m_LerpPos = m_PrevLerpPos;
			m_PrevLerpPos.y = 0;
			m_LerpPos.y = 0;
			Transform()->SetLocalPos(m_PrevLerpPos);
			Network::GetInst()->send_move_block_packet(m_GetId(), m_PrevLerpPos);
			m_bMoveCheck = false;

		}
		else {

			vPos.y = 0;
			m_LerpPos.y = 0;
			Vec3 vPos2;
			if (MoveCheck(vPos2 = Vec3::Lerp(vPos, m_LerpPos, DT * (5.f)))) {
				SetPrevLerpPos(vPos2);
				m_bColCheck = true;
				Transform()->SetLocalPos(vPos);
			}
			else {
				vPos = Vec3::Lerp(vPos, m_LerpPos, DT * (5.f));
				Transform()->SetLocalPos(vPos);
			}
		}
	if (!isMain) {
		Vec3 vRot = Transform()->GetLocalRot();
		vRot = Vec3::Lerp(Transform()->GetLocalRot(), m_LerpRot, DT * (10.f));
		Transform()->SetLocalRot(vRot);
	}

}

#include "Collider3D.h"
void CPlayerScript::m_FColCheck(Vec3 _vBeforePos, Vec3 _vAfterPos)
{
	if (m_bColCheck) {
		for (int i = 0; i < m_arrColObject.size(); ++i) {
			Vec3 vTargetPos = m_arrColObject[i]->Transform()->GetWorldPos();
			m_bMoveCheck = lengthCompare(_vBeforePos, _vAfterPos, GetObj(), vTargetPos);
			if (m_bMoveCheck) {
				return;
			}
		}
	}
}
#include "MinionScript.h"
void CPlayerScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	int a = 0;


		if (_pOther->GetObj()->GetScript<CArrowScript>() != nullptr) {
		}
		else if (_pOther->GetObj()->GetScript<CProjectileScript>() != nullptr) {
		}
		else if (_pOther->GetObj()->GetScript<CPlayerScript>() != nullptr) {

		}
		else if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr) {

		}
		else if (_pOther->GetObj()->GetScript<CBowScript>() != nullptr) {

		}
		else {
			m_arrColObject.push_back(_pOther->GetObj());
			m_bColCheck = true;
		}
}

void CPlayerScript::OnCollision3D(CCollider3D* _pOther)
{

}

void CPlayerScript::OnCollision3DExit(CCollider3D* _pOther)
{
	m_bColCheck = false;

	if (_pOther->GetObj()->GetScript<CArrowScript>() != nullptr) {

	}
	else {
		vector<CGameObject*>::iterator iter = m_arrColObject.begin();
		for (int i = 0; iter != m_arrColObject.end(); ++iter, ++i) {
			if (m_arrColObject[i] == _pOther->GetObj()) {
				m_arrColObject.erase(iter);
				return;
			}

		}
	}


}
void CPlayerScript::GetDamage(const UINT& _uiDamage)
{
	m_iCurHp -= _uiDamage;
	if (m_iCurHp <= 0) {
		SetState((int)PLAYER_STATE::DIE);
	}
	else {
		SetState((int)PLAYER_STATE::DEMAGED);
	}
}


CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT), m_bCheckStartMousePoint(false), m_fArcherLocation(20.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::SetDamage(const int& _Damage)
{
	m_iCurHp -= _Damage;
	if (m_iCurHp < 0) {
		m_iCurHp = 0;
	}
	if (m_iCurHp > m_iMaxHp)
		m_iCurHp = m_iMaxHp;
}


void CPlayerScript::UpdateHelmet(Vec3 LocalPos, Vec4 Quaternion, Vec3 LocalRot, Vec3 RevolutionRot)
{
	m_pHelmetObject->Transform()->SetLocalPos(LocalPos);
	m_pHelmetObject->Transform()->SetQuaternion(Quaternion);
	m_pHelmetObject->Transform()->SetLocalRot(LocalRot);
	m_pHelmetObject->Transform()->SetRevolutionRot(RevolutionRot);
}

void CPlayerScript::AttachHelmet()
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetCurScene() == SCENE_TYPE::INGAME && isMain) {
		CGameObject* pCamera = dynamic_cast<CGameObject*>(CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];
		Vec3 vCamRot = pCamera->Transform()->GetLocalRot();
		float fCamRotDegree = XMConvertToDegrees(vCamRot.x);

		tMTBone* pHeadBone = const_cast<tMTBone*>(GetObj()->MeshRender()->GetMesh()->GetBone(7));
		tMTBone* pChestBone = const_cast<tMTBone*>(GetObj()->MeshRender()->GetMesh()->GetBone(6));

		Vec3 vChestTrans1 = pChestBone->vecKeyFrame[GetObj()->Animator3D()->GetFrameIdx()].vTranslate;
		Vec3 vChestTrans2 = pChestBone->vecKeyFrame[GetObj()->Animator3D()->GetNextFrameIdx()].vTranslate;
		Vec3 vTrans1 = pHeadBone->vecKeyFrame[GetObj()->Animator3D()->GetFrameIdx()].vTranslate;
		Vec3 vTrans2 = pHeadBone->vecKeyFrame[GetObj()->Animator3D()->GetNextFrameIdx()].vTranslate;
		Vec4 qRot1 = pHeadBone->vecKeyFrame[GetObj()->Animator3D()->GetFrameIdx()].qRot;
		Vec4 qRot2 = pHeadBone->vecKeyFrame[GetObj()->Animator3D()->GetNextFrameIdx()].qRot;

		float fFactor = GetObj()->Animator3D()->GetRatio();

		Vec3 vHeadTrans = Vec3::Lerp(vTrans1, vTrans2, fFactor);
		Vec3 vChestTrans = Vec3::Lerp(vChestTrans1, vChestTrans2, fFactor);
		Vec3 vTransDir = vHeadTrans - vChestTrans;
		vTransDir.Normalize();
		Vec3 vRotDir = vHeadTrans - Vec3(vHeadTrans.x + 10.f, vHeadTrans.y, vHeadTrans.z);
		vRotDir.Normalize();
		Vec3 vTrans = vHeadTrans; // +vTransDir * 30.f;
		Vec4 qHatRot = Vec4::Lerp(qRot1, qRot2, fFactor);

		m_pHelmetObject->Transform()->SetLocalPos(vTrans);
		m_pHelmetObject->Transform()->SetQuaternion(qHatRot);
		m_pHelmetObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-fCamRotDegree * 3 + 90.f), 0.f, 0.f));
		m_pHelmetObject->Transform()->SetRevolutionRot(Vec3(XMConvertToRadians(-fCamRotDegree * 0.6f), 0.f, 0.f));

		Network::GetInst()->send_player_helemt(m_id, vTrans, qHatRot,
			Vec3(XMConvertToRadians(-fCamRotDegree * 3 + 90.f), 0.f, 0.f), Vec3(XMConvertToRadians(-fCamRotDegree * 0.6f), 0.f, 0.f));

	}
	else if (CSceneMgr::GetInst()->GetCurScene()->GetCurScene() == SCENE_TYPE::LOBBY) {
		// 로비씬

		tMTBone* pHeadBone = const_cast<tMTBone*>(GetObj()->MeshRender()->GetMesh()->GetBone(7));
		tMTBone* pChestBone = const_cast<tMTBone*>(GetObj()->MeshRender()->GetMesh()->GetBone(6));

		Vec3 vChestTrans1 = pChestBone->vecKeyFrame[GetObj()->Animator3D()->GetFrameIdx()].vTranslate;
		Vec3 vChestTrans2 = pChestBone->vecKeyFrame[GetObj()->Animator3D()->GetNextFrameIdx()].vTranslate;
		Vec3 vTrans1 = pHeadBone->vecKeyFrame[GetObj()->Animator3D()->GetFrameIdx()].vTranslate;
		Vec3 vTrans2 = pHeadBone->vecKeyFrame[GetObj()->Animator3D()->GetNextFrameIdx()].vTranslate;
		Vec4 qRot1 = pHeadBone->vecKeyFrame[GetObj()->Animator3D()->GetFrameIdx()].qRot;
		Vec4 qRot2 = pHeadBone->vecKeyFrame[GetObj()->Animator3D()->GetNextFrameIdx()].qRot;

		float fFactor = GetObj()->Animator3D()->GetRatio();

		Vec3 vHeadTrans = Vec3::Lerp(vTrans1, vTrans2, fFactor);
		Vec3 vChestTrans = Vec3::Lerp(vChestTrans1, vChestTrans2, fFactor);
		Vec3 vTransDir = vHeadTrans - vChestTrans;
		vTransDir.Normalize();
		Vec3 vRotDir = vHeadTrans - Vec3(vHeadTrans.x + 10.f, vHeadTrans.y, vHeadTrans.z);
		vRotDir.Normalize();
		Vec3 vTrans = vHeadTrans; // +vTransDir * 30.f;
		Vec4 qHatRot = Vec4::Lerp(qRot1, qRot2, fFactor);

		m_pHelmetObject->Transform()->SetLocalPos(vTrans);
		m_pHelmetObject->Transform()->SetQuaternion(qHatRot);
		m_pHelmetObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));

	}

}