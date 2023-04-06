#include "pch.h"
#include "Inter_Hatch.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "Inter_ExitDoor.h"
#include "UI_Play_Icon_Hud.h"
#include "Effect_Free3.h"

CInter_Hatch* const CInter_Hatch::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Hatch* pInstnace = new CInter_Hatch(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Hatch::Clone(void* const& pArg) {
	CInter_Hatch* pInstnace = new CInter_Hatch(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Hatch::CInter_Hatch(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Hatch::CInter_Hatch(const CInter_Hatch& rhs)
	: CObject(rhs) {
}

void CInter_Hatch::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);

	Safe_Release(m_pTransform_Slasher);
	Safe_Release(m_pCollider_Slasher);
}

const HRESULT CInter_Hatch::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Hatch::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Hatch"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_Slasher"), reinterpret_cast<CComponent*&>(m_pTransform_Slasher));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_Slasher"), reinterpret_cast<CComponent*&>(m_pCollider_Slasher));
	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(201.f, 0.f, 172.f, 1.f));
	m_eState = ESTATE::OPEN_LOOP;
	
	return hr;
}

const _ubyte CInter_Hatch::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	Update_Network();

	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case ESTATE::CLOSE_LOOP:
			m_pModel->Set_Anim("SK_Hatch_REF.ao|SK_Hatch_IdleClose");
			m_ePreState = ESTATE::CLOSE_LOOP;
			m_bLoop = true;
			break;
		case ESTATE::OPEN:
			m_pModel->Set_Anim("SK_Hatch_REF.ao|SK_Hatch_Open");
			pEffect[0] = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_HatchGround"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_HatchGround")));
			pEffect[0]->Set_Pos(m_pTransform->Get_Row(CTransform::ERow::POS));
			pEffect[1] = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_HatchSmoke"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_HatchSmoke")));
			pEffect[1]->Set_Pos(m_pTransform->Get_Row(CTransform::ERow::POS));
			m_ePreState = ESTATE::OPEN;
			m_bLoop = false;
			break;
		case ESTATE::OPEN_LOOP:
			m_pModel->Set_Anim("SK_Hatch_REF.ao|SK_Hatch_IdleOpen");
			m_ePreState = ESTATE::OPEN_LOOP;
			m_bLoop = true;
			break;
		case ESTATE::CLOSE_SLASHER:
			m_pModel->Set_Anim("SK_Hatch_REF.ao|SK_Hatch_KillerClose");
			/*pEffect[0]->Set_RepeatEnd();
			pEffect[1]->Set_RepeatEnd();*/
			m_ePreState = ESTATE::CLOSE_SLASHER;
			m_bLoop = false;
			break;
		}
	}
	m_iAnimFinish = m_pModel->Update_Anim(dTimeDelta, m_bLoop);
	m_pCollider->Update(m_pTransform->Get_Matrix());
	m_pCollider_Slasher->Update(m_pTransform_Slasher->Get_Matrix());

	CInfo* pInfo = CInfo::Get_Instance();
	CInter_ExitDoor* pExitDoor = dynamic_cast<CInter_ExitDoor*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Inter_ExitDoor")));

	if (false == m_bOpen && (1 == pInfo->Get_CamperCount() || pExitDoor->Get_State() == CInter_ExitDoor::ESTATE::OPENING)) {
		Set_State(ESTATE::OPEN);
		m_bOpen = true;
	}

	if (ESTATE::OPEN == m_eState && ANIM_END == m_iAnimFinish)
		Set_State(ESTATE::OPEN_LOOP);
	if (ESTATE::CLOSE_SLASHER == m_eState && ANIM_END == m_iAnimFinish)
		Set_State(ESTATE::CLOSE_LOOP);

	//collsion
	if (CInfo::ECamper::CHERYL == pInfo->Get_Camper()) {
		CCamper_GamePlay* pCamper_GamePlay = dynamic_cast<CCamper_GamePlay*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay")));
		if (nullptr == pCamper_GamePlay)
			return OBJ_NOEVENT;
		CCollider* pCamper_GamePlayCollider = dynamic_cast<CCollider*>(pCamper_GamePlay->Get_Component(TEXT("Collider")));
		if (nullptr == pCamper_GamePlayCollider)
			return OBJ_NOEVENT;
		if (pCamper_GamePlay->Get_Control())
		{
			if (m_eState == ESTATE::OPEN_LOOP && m_pCollider->Is_Collision(pCamper_GamePlayCollider)) {
				//∫ÒªÛ≈ª√‚±∏∑Œ ≈ª√‚«‘
				dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::ESCAPE), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

				pCamper_GamePlay->Set_ColObject(this);
				pCamper_GamePlay->Set_CollisionState(CCamper_GamePlay::ECollisionState::HATCH);
			}
			else {
				if (pCamper_GamePlay->Get_ColObject() == this) {
					//UI« ø‰X
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::ESCAPE), true);

					pCamper_GamePlay->Set_ColObject(nullptr);
					pCamper_GamePlay->Set_CollisionState(CCamper_GamePlay::ECollisionState::NONE);
				}
			}
		}
	}
	else if (CInfo::ESlasher::HUNTRESS == pInfo->Get_Slasher()) {
		CSlasher_Huntress* pSlasher = dynamic_cast<CSlasher_Huntress*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Slasher_Huntress")));
		if (nullptr == pSlasher)
			return OBJ_NOEVENT;

		if (true == pSlasher->Get_Control()) {
			CCollider* pSlasherCollider = dynamic_cast<CCollider*>(pSlasher->Get_Component(TEXT("Collider")));
			if (nullptr == pSlasherCollider)
				return OBJ_NOEVENT;

			if (m_eState == ESTATE::OPEN_LOOP && CSlasher_Huntress::EType::SLASH == pSlasher->Get_Type() && pSlasher->Get_CollisionState() != CSlasher_Huntress::ECollisionState::CAMPER_CRAWL
				&& true == m_pCollider->Is_Collision(pSlasherCollider)) {
				// ∞≥±∏∏€ UI
				// Hatch Close UI On ¥›±‚
				(0 < pSlasher->Get_WeaponCount()) ?
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::CLOSE), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::RIGHT) :
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::CLOSE), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

				pSlasher->Set_ColObject(this);
				pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::HATCH);
			}
			else if (false == m_pCollider->Is_Collision(pSlasherCollider)) {
				if (pSlasher->Get_ColObject() == this) {
					// Hatch Close UI Off
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::CLOSE), true);

					pSlasher->Set_ColObject(nullptr);
					pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::NONE);
				}
			}
		}
	}

	return OBJ_NOEVENT;
}

void CInter_Hatch::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);

	m_pTransform_Slasher->Update_Edit(2);
	m_pCollider_Slasher->Update_Edit(3);
	ImGui::InputInt("ID", &m_iId);
}

void CInter_Hatch::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	if (true == pGame_Instance->IsIn_Frustum_World(m_pTransform->Get_Row(CTransform::ERow::POS), 15.f))
		m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CInter_Hatch::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			if (FAILED(m_pModel->Set_Resource(i, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
				m_pModel->Render(i, m_pShader, 0);
			else
				m_pModel->Render(i, m_pShader, 7);
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		//m_pCollider->Render(pCamera);
		m_pCollider_Slasher->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

void CInter_Hatch::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}

}

void CInter_Hatch::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>(iter["State"].asInt());
		}
	}
	listRecv.clear();
}

const HRESULT CInter_Hatch::Save_Data(Json::Value& root){
	m_pTransform->Save_Data(root["Transform"]);
	root["Id"] = m_iId;
	return S_OK;
}

void CInter_Hatch::Load_Data(Json::Value& root){
	m_pTransform->Load_Data(root["Transform"]);
	m_iId = root["Id"].asInt();
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	
}
