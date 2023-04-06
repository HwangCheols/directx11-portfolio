#include "pch.h"
#include "Inter_Hook.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "Shadow.h"
#include "UI_Play_Icon_Hud.h"
#include "UI_Play_Gauge_Inter.h"
#include "Effect_Free3.h"

CInter_Hook* const CInter_Hook::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Hook* pInstnace = new CInter_Hook(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Hook::Clone(void* const& pArg) {
	CInter_Hook* pInstnace = new CInter_Hook(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Hook::CInter_Hook(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Hook::CInter_Hook(const CInter_Hook& rhs)
	: CObject(rhs) {
}

void CInter_Hook::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pShader2);
	Safe_Release(m_pModel_Hook);
	Safe_Release(m_pModel_HookSpider);
	Safe_Release(m_pCollider);
}

const HRESULT CInter_Hook::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Hook::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	//Hook
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Hook"), TEXT("Model_Hook"), reinterpret_cast<CComponent*&>(m_pModel_Hook));
	//Spider
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader2"), reinterpret_cast<CComponent*&>(m_pShader2));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_HookSpider"), TEXT("Model_Spider"), reinterpret_cast<CComponent*&>(m_pModel_HookSpider));

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(200.f, 0.f, 178.f, 1.f));
	m_pCollider->Set_Center(_float3{ 0.f, 0.5f, 1.5f });

	m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Idle");
	m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Invisible");

	return hr;
}

const _ubyte CInter_Hook::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	Update_Network();

	//hook
	/*if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_3)) {
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_GetHookedIn");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_4)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Reaction_IN");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderReaction_IN");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_5)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Reaction_Loop");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderReaction_Loop");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_6)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Reaction_Out");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderReaction_OUT");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_7)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_StabIN");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderStabIN");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_8)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_StabLoop");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderStabLoop");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_9)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_StabOut");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderStabOut");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_0)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Spider_Struggle");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_P)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle2Sacrifice");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderStruggle2Sacrifice");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_O)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle_Fail");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Spider_Struggle_Fail");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_I)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle_Fail_A");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Spider_Struggle_Fail_A");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_U)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle_MAX");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Stand");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_Y)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle_MIN");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderStruggle_MIN");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_T)) {
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Invisible");
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Idle");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_L)) {
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_GetHookedOut");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_K)) {
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_Struggle");
	}
	if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_J)) {
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_HookedFree");
		m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Invisible");
	}if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_H)) {
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_BreakEnd");
	}if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_G)) {
		m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Broken_Idle");
	}*/
	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case ESTATE::IDLE:
			m_bSpiderRender = false;
			m_bLoop = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Idle");
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Invisible");
			m_ePreState = ESTATE::IDLE;
			break;
		case ESTATE::IDLE2:
			m_bSpiderRender = true;
			m_bLoop = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Idle");
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Invisible");
			m_ePreState = ESTATE::IDLE2;
			break;
		case ESTATE::HANGING:
			m_bLoop = false;
			m_bSpiderRender = false;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_GetHookedIn");
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Invisible");
			m_ePreState = ESTATE::HANGING;
			break;
		case ESTATE::HANGING_END:
			m_bLoop = false;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_GetHookedOut");
			m_ePreState = ESTATE::HANGING_END;
			break;
			/*case ESTATE::HANGING_2:
				m_bLoop = false;
				m_bSpiderRender = true;
				m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_GetHookedIn");
				m_ePreState = ESTATE::HANGING_2;
				break;*/
		case ESTATE::SPREAD://F_Spider_Reaction_IN
			m_bLoop = false;
			m_bSpiderRender = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderReaction_IN");
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Reaction_IN");
			break;
		case ESTATE::SHAKE://F_Spider_Reaction_Loop
			m_bLoop = true;
			m_bSpiderRender = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderReaction_Loop");
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Reaction_Loop");
			m_ePreState = ESTATE::SHAKE;
			break;
		case ESTATE::ENCASE:
			m_bLoop = false;
			m_bSpiderRender = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_Struggle");
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle");
			m_ePreState = ESTATE::ENCASE;
			break;
		case ESTATE::STAB://F_Spider_Reaction_Out
			m_bLoop = true;
			m_bSpiderRender = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderReaction_OUT");
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Reaction_Out");
			m_ePreState = ESTATE::STAB;
			break;
		case ESTATE::STRUGGLE://F_Spider_Struggle
			m_bLoop = true;
			m_bSpiderRender = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Spider_Struggle");
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle");
			m_ePreState = ESTATE::STRUGGLE;
			break;
			/*case ESTATE::STAB_2:
				m_bLoop = true;
				m_bSpiderRender = true;
				m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_Struggle");
				m_pModel_HookSpider->Set_Anim("SK_HookHuntress.ao|SK_Spider_StabIN");
				m_ePreState = ESTATE::STAB_2;
				break;*/
				//case ESTATE::STRUGGLE_2:
				//	m_bLoop = true;
				//	m_bSpiderRender = true;
				//	m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_Spider_Struggle_Fail"); 
				//	m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Struggle_Fail"); 
				//	m_ePreState = ESTATE::STRUGGLE_2;
				//	break;
		case ESTATE::KILL:
			m_bLoop = false;
			m_bSpiderRender = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderStabIN"); 
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_StabIN");
			m_ePreState = ESTATE::KILL;
			break;
		case ESTATE::RAISE:
			m_bLoop = false;
			m_bSpiderRender = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_SpiderStabOut"); 
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_StabOut");		
			m_ePreState = ESTATE::RAISE;
			//create crow - Hwang
			pGame_Instance->Create_Object_Clone(TEXT("Inter_Crow_Wander"), static_cast<_uint>(EScene::GAMEPLAY), TEXT("Inter_Crow_Wander"), &m_pTransform->Get_Row(CTransform::ERow::POS));
			break;
		case ESTATE::TRY_ESCAPE:
			m_bLoop = true;
			//m_bSpiderRender = true;
			//m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHanger_Struggle"); 
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Invisible"); 
			m_ePreState = ESTATE::TRY_ESCAPE;
			break;
		case ESTATE::ESCAPE:
			m_bLoop = false;
			m_bSpiderRender = true;
			m_pModel_Hook->Set_Anim("SK_HookHuntress.ao|SK_DeerHangers_HookedFree"); 
			m_pModel_HookSpider->Set_Anim("SK_MeatHook_Spider_REF.ao|SK_Spider_Invisible"); 
			m_ePreState = ESTATE::ESCAPE;
			break;
		}
	}

	//if (ESTATE::HANGING == m_eState || ESTATE::HANGING_2 == m_eState) {
	//	if (ANIM_END == m_pModel_Hook->Update_Anim(dTimeDelta, false))
	//		m_eState = ESTATE::HANGING_END;
	//}
	///*else if (ESTATE::RAISE == m_eState || ESTATE::ESCAPE == m_eState)
	//	m_eState = ESTATE::IDLE;*/
	//else if (ESTATE::TRY_ESCAPE == m_eState)
	//{
	//	if (ANIM_END == m_pModel_Hook->Update_Anim(dTimeDelta, false))
	//		m_eState = ESTATE::IDLE;
	//}
	//else
	/*if (ESTATE::IDLE == m_eState)
	{
		m_pModel_HookSpider->Update_Anim(dTimeDelta, true);
		m_pModel_Hook->Update_Anim(dTimeDelta, true);
	}
	else {
		m_pModel_HookSpider->Update_Anim(dTimeDelta, false);
		m_pModel_Hook->Update_Anim(dTimeDelta, false);
	}*/
	m_pModel_Hook->Update_Anim(dTimeDelta, m_bLoop);
	m_pModel_HookSpider->Update_Anim(dTimeDelta, m_bLoop);

	m_pCollider->Update(m_pTransform->Get_Matrix());
	//collsion
	CInfo* pInfo = CInfo::Get_Instance();
	//***************for Camper Test**********************
	//if (CInfo::ECamper::CHERYL == pInfo->Get_Camper()) {
	//	CCamper_GamePlay* pCamper_GamePlay = dynamic_cast<CCamper_GamePlay*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay")));
	//	if (nullptr != pCamper_GamePlay)
	//	{
	//		CCollider* pCamper_GamePlayCollider = dynamic_cast<CCollider*>(pCamper_GamePlay->Get_Component(TEXT("Collider3")));
	//		if (nullptr == pCamper_GamePlayCollider)
	//			return OBJ_NOEVENT;
	//		if (pCamper_GamePlay->Get_Control())
	//		{
	//			if (m_pCollider->Is_Collision(pCamper_GamePlayCollider) && m_eState == ESTATE::IDLE ||
	//				m_pCollider->Is_Collision(pCamper_GamePlayCollider) && m_eState == ESTATE::HANGING ||
	//				m_pCollider->Is_Collision(pCamper_GamePlayCollider) && m_eState == ESTATE::HANGING_END) 
	//			{
	//				pCamper_GamePlay->Set_ColObject(this);
	//				pCamper_GamePlay->Set_CollisionState(CCamper_GamePlay::ECollisionState::HOOK);
	//			}
	//			else {
	//				if (pCamper_GamePlay->Get_ColObject() == this) {
	//					pCamper_GamePlay->Set_ColObject(nullptr);
	//					pCamper_GamePlay->Set_CollisionState(CCamper_GamePlay::ECollisionState::NONE);
	//				}
	//			}
	//		}
	//	}
	//}
	//**********************************************************

	/*else */if (CInfo::ESlasher::HUNTRESS == pInfo->Get_Slasher()) {
		CSlasher_Huntress* pSlasher = dynamic_cast<CSlasher_Huntress*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Slasher_Huntress")));
		if (nullptr != pSlasher) {
			if (true == pSlasher->Get_Control()) {
				CCollider* pSlasherCollider = dynamic_cast<CCollider*>(pSlasher->Get_Component(TEXT("Collider")));
				if (m_eState == ESTATE::IDLE && CSlasher_Huntress::EType::CARRY == pSlasher->Get_Type()
					&& true == m_pCollider->Is_Collision(pSlasherCollider)) {
					// 갈고리 거는 UI
					// Hook Hang UI On 매달기					
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::HANG), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);
					dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"))->Set_Render(CUI_Play_Gauge_Inter::EInter::HANG, true, false);

					pSlasher->Set_ColObject(this);
					pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::HOOK);
				}
				else if (false == m_pCollider->Is_Collision(pSlasherCollider)) {
					if (pSlasher->Get_ColObject() == this) {
						// Hook Hang UI Off
						dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::HANG), true);
						dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"))->Set_Render(CUI_Play_Gauge_Inter::EInter::HANG, false, false);

						pSlasher->Set_ColObject(nullptr);
						pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::NONE);
					}
				}
			}
		}
	}

	return OBJ_NOEVENT;
}

void CInter_Hook::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
	ImGui::InputInt("ID", &m_iId);
}

void CInter_Hook::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	CInfo* pInfo = CInfo::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
	if (true == pInfo->Get_Player()) {
		m_pRenderer->Add_Object(CRenderer::EOrder::PENETRATE, this);
	}
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CInter_Hook::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);
		pCamera->Set_RawValue(m_pShader2);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel_Hook->Get_NumMesh(); ++i) {
			m_pModel_Hook->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			if (FAILED(m_pModel_Hook->Set_Resource(i, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
				m_pModel_Hook->Render(i, m_pShader, 0);
			else
				m_pModel_Hook->Render(i, m_pShader, 7);
		}
		if (m_bSpiderRender) {
			m_pShader2->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
			for (_uint i = 0; i < m_pModel_HookSpider->Get_NumMesh(); ++i) {
				m_pModel_HookSpider->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader2, "g_DiffuseTexture");
				if (FAILED(m_pModel_HookSpider->Set_Resource(i, aiTextureType_NORMALS, m_pShader2, "g_NormalTexture")))
					m_pModel_HookSpider->Render(i, m_pShader2, 0);
				else
					m_pModel_HookSpider->Render(i, m_pShader2, 7);
			}
		}
		break;
	}
	case CRenderer::EOrder::SHADOW: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		pShadow->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel_Hook->Get_NumMesh(); ++i) {
			m_pModel_Hook->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel_Hook->Render(i, m_pShader, 1);
		}
		if (m_bSpiderRender) {
			m_pShader2->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
			for (_uint i = 0; i < m_pModel_HookSpider->Get_NumMesh(); ++i) {
				m_pModel_HookSpider->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader2, "g_DiffuseTexture");
				m_pModel_HookSpider->Render(i, m_pShader2, 1);
			}
		}
		break;
	}
	case CRenderer::EOrder::PENETRATE: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);
		pCamera->Set_RawValue(m_pShader2);
		CTarget* pDepth = CTarget_Mgr::Get_Instance()->Get_Target(TEXT("Depth"));
		ID3D11ShaderResourceView* pDepthSRV = pDepth->Get_ShaderResourceView();
		m_pShader->Set_Resource("g_DepthTexture", pDepthSRV);
		m_pShader2->Set_Resource("g_DepthTexture", pDepthSRV);

		_float2 fWinSize = _float2{ static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY) };
		m_pShader->Set_RawValue("g_fWinSize", &fWinSize, sizeof(_float2));
		_float4 vPenetrateColor = _float4{ 0.8f, 0.8f, 0.f, 1.f };
		m_pShader->Set_RawValue("g_vPenetrateColor", &vPenetrateColor, sizeof(_float4));
		m_pShader2->Set_RawValue("g_vPenetrateColor", &vPenetrateColor, sizeof(_float4));

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel_Hook->Get_NumMesh(); ++i) {
			m_pModel_Hook->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel_Hook->Render(i, m_pShader, 3);
		}
		if (m_bSpiderRender) {
			m_pShader2->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
			for (_uint i = 0; i < m_pModel_HookSpider->Get_NumMesh(); ++i) {
				m_pModel_HookSpider->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader2, "g_DiffuseTexture");
				m_pModel_HookSpider->Render(i, m_pShader2, 4);
			}
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		//m_pCollider->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

void CInter_Hook::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_Hook::Update_Network(void) {
	list<Json::Value>& root = m_pNetwork->Get_Recv(m_iId);
	for (auto iter = root.begin(); iter != root.end();) {
		string key = (*iter)["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>((*iter)["State"].asInt());
			iter = root.erase(iter);
		}
		else {
			++iter;
		}
	}
}
const HRESULT CInter_Hook::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	root["Id"] = m_iId;
	return S_OK;
}

void CInter_Hook::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
	m_iId = root["Id"].asInt();

	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	if ((rand() % 1000) < 500) {
		CEffect_Free3* pEffect1 = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_HookFlower"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_HookFlower")));
		CEffect_Free3* pEffect2 = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_HookSmoke"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_HookSmoke")));

		CTransform* pTransform1 = dynamic_cast<CTransform*>(pEffect1->Get_Component(TEXT("Transform")));
		CTransform* pTransform2 = dynamic_cast<CTransform*>(pEffect2->Get_Component(TEXT("Transform")));

		pTransform1->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS));
		pTransform2->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS));
	}

}