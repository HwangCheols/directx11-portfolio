#include "pch.h"
#include "Inter_ChestBox.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "Shadow.h"
#include "UI_Play_Icon_Hud.h"
#include "UI_Play_Gauge_Inter.h"

CInter_ChestBox* const CInter_ChestBox::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_ChestBox* pInstnace = new CInter_ChestBox(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_ChestBox::Clone(void* const& pArg) {
	CInter_ChestBox* pInstnace = new CInter_ChestBox(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_ChestBox::CInter_ChestBox(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_ChestBox::CInter_ChestBox(const CInter_ChestBox& rhs)
	: CObject(rhs) {
}

void CInter_ChestBox::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pItemTransform);
	Safe_Release(m_pColTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pShader2);
	Safe_Release(m_pModel);
	Safe_Release(m_pAudio);
	//	Safe_Release(m_pMediKit_Model);
		//Safe_Release(m_pFlashLight_Model);
	Safe_Release(m_pItem_Model);


	Safe_Release(m_pCollider);
	Safe_Release(m_pCollider2);

}

const HRESULT CInter_ChestBox::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_ChestBox::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();//
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_ChestBox"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Audio"), TEXT("Audio"), reinterpret_cast<CComponent*&>(m_pAudio));
	//m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(203.f, 0.f, 174.f, 1.f));
	m_pCollider->Set_Center(_float3{ 0.0f, 0.36f, 0.32f });
	m_pCollider->Set_Extents(_float3{ 0.7f, 0.3f, 0.3f });

	m_eState = ESTATE::IDLE;

	// Camper
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("ColTransform"), reinterpret_cast<CComponent*&>(m_pColTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider2"), reinterpret_cast<CComponent*&>(m_pCollider2));
	m_pColTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(0.12f, 0.f, 0.78f, 1.f));
	m_pColTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

	//Item
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform2"), reinterpret_cast<CComponent*&>(m_pItemTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_Model"), TEXT("Shader2"), reinterpret_cast<CComponent*&>(m_pShader2));
	m_eItemKind = (EItemKind)(rand() % 1);
	m_eItemKind = (EItemKind)1;//
	switch (m_eItemKind)
	{
	case EItemKind::MEDIKIT:
		hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_ChestBox_MediKit"), TEXT("Model2"), reinterpret_cast<CComponent*&>(m_pItem_Model));
		break;
	case EItemKind::FLASHLIGHT:
		hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_ChestBox_FlashLight"), TEXT("Model2"), reinterpret_cast<CComponent*&>(m_pItem_Model));
		break;
	}

	m_pItemTransform->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS) + XMVectorSet(-0.05f, 0.2f, 0.f, 1.f));
	m_pItemTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(90.f));

	return hr;
}

const _ubyte CInter_ChestBox::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	Update_Network();

	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case ESTATE::IDLE:
			m_fSound_OpenLoop = 1.f;
			m_pModel->Set_Anim("SK_SR_Chest01_REF.ao|SK_Chest_Idle");
			m_ePreState = ESTATE::IDLE;
			break;
		case ESTATE::OPEN_STEP1:
			m_pModel->Set_Anim("SK_SR_Chest01_REF.ao|SK_Chest_In");
			m_ePreState = ESTATE::OPEN_STEP1;
			break;
		case ESTATE::OPEN_STEP2:
			m_pModel->Set_Anim("SK_SR_Chest01_REF.ao|SK_Chest_Loop");
			m_ePreState = ESTATE::OPEN_STEP2;
			break;
		case ESTATE::OPEN_STEP3:
			m_pModel->Set_Anim("SK_SR_Chest01_REF.ao|SK_Chest_Open_Idle");
			m_ePreState = ESTATE::OPEN_STEP3;
			break;
		case ESTATE::OPEN_END:
			m_pAudio->Play_Sound_Rand(TEXT("Chest_Open_Mix_Bounce"), 2, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 10.f });
			m_pModel->Set_Anim("SK_SR_Chest01_REF.ao|SK_Chest_Out");
			m_ePreState = ESTATE::OPEN_END;
			m_dTime += dTimeDelta;
			break;
		}
	}

	switch (m_eState) {
	case ESTATE::OPEN_STEP1:
		if (m_pModel->Get_Anim_KeyTimeAcc(18)) {
			m_pAudio->Play_Sound_Rand(TEXT("sfx_chest_padlock_start"), 3, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 10.f });
		}
		break;
	case ESTATE::OPEN_STEP2:
		m_fSound_OpenLoop += static_cast<_float>(dTimeDelta);
		if (0.9f < m_fSound_OpenLoop) {
			m_fSound_OpenLoop = 0.f;
			m_pAudio->Play_Sound_Rand(TEXT("sfx_chest_padlock_handle"), 9, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 10.f });
		}
		break;
	}

	m_iAnimFinish = m_pModel->Update_Anim(dTimeDelta, false);


	//collsion
	CInfo* pInfo = CInfo::Get_Instance();
	if (CInfo::ECamper::CHERYL == pInfo->Get_Camper()) {
		if (nullptr == m_pCamper)
			m_pCamper = dynamic_cast<CCamper_GamePlay*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay")));
		if (nullptr == m_pCamper)
			return OBJ_NOEVENT;
		CCollider* pCamperCollider = dynamic_cast<CCollider*>(m_pCamper->Get_Component(TEXT("Collider")));
		if (nullptr == pCamperCollider)
			return OBJ_NOEVENT;
		if (m_pCamper->Get_Control()) {
			if (m_pCollider->Is_Collision(pCamperCollider)) {
				// set hud
				CUI_Play_Icon_Hud* pHud = dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"));
				CUI_Play_Gauge_Inter* pGauge = dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"));
				switch (m_eState)
				{
				case ESTATE::IDLE:
					pHud->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::UNLOCK), CUI_Play_Icon_Hud::EKey::MOUSE_L, CUI_Play_Icon_Hud::EDiv::CENTER);
					pGauge->Set_Render(CUI_Play_Gauge_Inter::EInter::CHEST, true, false);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::ChestBox);
					break;
				case ESTATE::OPEN_STEP1:
					pHud->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::CANCLE), CUI_Play_Icon_Hud::EKey::MOUSE_L, CUI_Play_Icon_Hud::EDiv::CENTER);
					pGauge->Set_Inter(CUI_Play_Gauge_Inter::EInter::CHEST, true);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::ChestBox);
					break;
				case ESTATE::OPEN_STEP2:
					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::ChestBox);
					break;
				case ESTATE::OPEN_STEP3:
					pGauge->Set_Render(CUI_Play_Gauge_Inter::EInter::CHEST, false, false);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::ChestBox);
					break;
				case ESTATE::OPEN_END:
					if (!m_bIsUse)
						pHud->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::GET), CUI_Play_Icon_Hud::EKey::MOUSE_L, CUI_Play_Icon_Hud::EDiv::CENTER);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::ChestBox);
					break;
				}
				// set gauge
				pGauge->Set_Gauge(CUI_Play_Gauge_Inter::EInter::CHEST, m_fGauge);
			}
			else {
				if (m_pCamper->Get_ColObject() == this) {
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(CUI_Play_Icon_Hud::EDiv::CENTER, true);
					dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"))->Set_Render(CUI_Play_Gauge_Inter::EInter::CHEST, false, false);
					
					m_pCamper->Set_ColObject(nullptr);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::NONE);
				}
			}
		}
	}

	return OBJ_NOEVENT;
}

void CInter_ChestBox::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
	m_pCollider2->Update_Edit(2);
	ImGui::InputInt("ID", &m_iId);
}

void CInter_ChestBox::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CInter_ChestBox::Render(const CRenderer::EOrder& eOrder) {
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
		if (!m_bIsUse) {
			_matrix Matrix = m_pItemTransform->Get_Matrix() * m_pTransform->Get_Matrix();
			m_pShader2->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
			for (_uint i = 0; i < m_pItem_Model->Get_NumMesh(); ++i) {
				m_pItem_Model->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader2, "g_DiffuseTexture");
				if (FAILED(m_pItem_Model->Set_Resource(i, aiTextureType_NORMALS, m_pShader2, "g_NormalTexture")))
					m_pItem_Model->Render(i, m_pShader2, 0);
				else
					m_pItem_Model->Render(i, m_pShader2, 8);
			}
		}
		break;
	}
	case CRenderer::EOrder::SHADOW: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		pShadow->Set_RawValue(m_pShader);
		pShadow->Set_RawValue(m_pShader2);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 1);
		}
		if (!m_bIsUse) {
			_matrix Matrix = m_pItemTransform->Get_Matrix() * m_pTransform->Get_Matrix();
			m_pShader2->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
			for (_uint i = 0; i < m_pItem_Model->Get_NumMesh(); ++i) {
				m_pItem_Model->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader2, "g_DiffuseTexture");
				m_pItem_Model->Render(i, m_pShader2, 3);
			}
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		//m_pCollider->Render(pCamera);
		//m_pCollider2->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

void CInter_ChestBox::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Set_ItemUse" == pKey) {
		root["Set_ItemUse"] = static_cast<_bool>(m_bIsUse);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Add_Gauge" == pKey) {
		root["Gauge"] = m_fAddGauge;
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_ChestBox::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>(iter["State"].asInt());
		}
		else if ("Set_ItemUse" == key) {
			m_bIsUse = static_cast<_bool>(iter["Set_ItemUse"].asBool());
		}
		else if ("Add_Gauge" == key) {
			m_fAddGauge = static_cast<_float>(iter["State"].asFloat());
			m_fGauge += m_fAddGauge;
		}

	}
	listRecv.clear();
}
const HRESULT CInter_ChestBox::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	root["Id"] = m_iId;
	return S_OK;
}

void CInter_ChestBox::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
	m_iId = root["Id"].asInt();

	m_pCollider->Update(m_pTransform->Get_Matrix());
	m_pCollider2->Update(m_pColTransform->Get_Matrix() * m_pTransform->Get_Matrix());
}

const _float& CInter_ChestBox::Get_Gauge(void)
{
	return m_fGauge;
}

const void CInter_ChestBox::Add_Gauge(const _float& fGauge)
{
	m_fAddGauge = fGauge;
	m_fGauge += m_fAddGauge;
	Update_Network("Add_Gauge");
}
