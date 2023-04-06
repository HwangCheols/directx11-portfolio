#include "pch.h"
#include "Inter_Totem.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "UI_Play_Gauge_Inter.h"
#include "UI_Play_Icon_Hud.h"
#include "UI_Play_Gauge_Inter.h"

CInter_Totem* const CInter_Totem::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Totem* pInstnace = new CInter_Totem(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Totem::Clone(void* const& pArg) {
	CInter_Totem* pInstnace = new CInter_Totem(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Totem::CInter_Totem(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Totem::CInter_Totem(const CInter_Totem& rhs)
	: CObject(rhs) {
}

void CInter_Totem::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}

const HRESULT CInter_Totem::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Totem::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_Model"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Totem"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));

	/*_uint iRandom = rand() % 4;
	switch (iRandom)
	{
	case 0:
		m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(219.963f, 0.f, 211.126f, 1.f));
		break;
	case 1:
		m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(129.155f, 0.f, 207.943f, 1.f));
		break;
	case 2:
		m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(125.792f, 0.f, 148.594f, 1.f));
		break;
	case 3:
		m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(222.149f, 0.f, 131.478f, 1.f));
		break;
	}*/
	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(222.149f, 0.f, 131.478f, 1.f));
	m_eState = ESTATE::IDLE;
	m_iId = 298;

	return hr;
}

const _ubyte CInter_Totem::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	Update_Network();

	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case ESTATE::IDLE:
			m_ePreState = ESTATE::IDLE;
			break;
		case ESTATE::BREAK:
			//토템 파괴 이펙트 넣어주삼
			return OBJ_ERASE;
			break;		
		case ESTATE::ON:
			m_ePreState = ESTATE::ON;
			break;
		}

	}

	m_pCollider->Update(m_pTransform->Get_Matrix());

	//collsion
	CInfo* pInfo = CInfo::Get_Instance();
	if (CInfo::ECamper::CHERYL == pInfo->Get_Camper()) {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		list<class CObject*>* CamperList = pGame_Instance->Get_Object_ListClone(pGame_Instance->Get_Scene(), TEXT("Camper_GamePlay"));
		CCamper_GamePlay* pCamper_GamePlay = dynamic_cast<CCamper_GamePlay*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay")));

		if (nullptr == pCamper_GamePlay)
			return OBJ_NOEVENT;

		CCollider* pCamper_GamePlayCollider = dynamic_cast<CCollider*>(pCamper_GamePlay->Get_Component(TEXT("Collider")));
		if (nullptr == pCamper_GamePlayCollider)
			return OBJ_NOEVENT;

		if (pCamper_GamePlay->Get_Control()) {
			CUI_Play_Icon_Hud* pHud = dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"));
			CUI_Play_Gauge_Inter* pGauge = dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"));

			if (m_eState == ESTATE::IDLE && m_pCollider->Is_Collision(pCamper_GamePlayCollider)) {
				//토템 정화 UI필요 -> inter : totemloop
				pHud->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::TOTEM1), CUI_Play_Icon_Hud::EKey::MOUSE_L, CUI_Play_Icon_Hud::EDiv::CENTER);
				pGauge->Set_Render(CUI_Play_Gauge_Inter::EInter::TOTEM, true, false);

				pCamper_GamePlay->Set_ColObject(this);
				pCamper_GamePlay->Set_CollisionState(CCamper_GamePlay::ECollisionState::TOTEM);
			}
			else {
				if (pCamper_GamePlay->Get_ColObject() == this) {
					//토템 정화 UI필요X
					pHud->Set_HudOff(CUI_Play_Icon_Hud::EDiv::CENTER, true);
					pGauge->Set_Render(CUI_Play_Gauge_Inter::EInter::TOTEM, false, false);

					pCamper_GamePlay->Set_ColObject(nullptr);
					pCamper_GamePlay->Set_CollisionState(CCamper_GamePlay::ECollisionState::NONE);
				}
			}
			// set gauge
			pGauge->Set_Gauge(CUI_Play_Gauge_Inter::EInter::TOTEM, m_fGauge);
		}
	}
	if (CInfo::ESlasher::HUNTRESS == pInfo->Get_Slasher()) {
		CSlasher_Huntress* pSlasher = dynamic_cast<CSlasher_Huntress*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Slasher_Huntress")));
		if (nullptr == pSlasher)
			return OBJ_NOEVENT;

		if (true == pSlasher->Get_Control() && pSlasher->Get_FM_Slasher()->Get_State() != CFM_Slasher::EState::DESTROY_TOTEM) {
			CCollider* pSlasherCollider = dynamic_cast<CCollider*>(pSlasher->Get_Component(TEXT("Collider")));
			if (nullptr == pSlasherCollider)
				return OBJ_NOEVENT;

			if (m_eState == ESTATE::IDLE && CSlasher_Huntress::EType::SLASH == pSlasher->Get_Type() && pSlasher->Get_CollisionState() != CSlasher_Huntress::ECollisionState::CAMPER_CRAWL
				&& true == m_pCollider->Is_Collision(pSlasherCollider)) {
				
				// 토템 추가하게 되면 토템 관련 UI
				// Totem Destroy UI On 파괴
				(0 < pSlasher->Get_WeaponCount()) ?
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::DESTROY), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::RIGHT) :
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::DESTROY), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);
				
				pSlasher->Set_ColObject(this);
				pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::TOTEM);
			}
			else if (false == m_pCollider->Is_Collision(pSlasherCollider)) {
				if (pSlasher->Get_ColObject() == this) {
					// Totem Destroy UI Off
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::DESTROY), true);

					pSlasher->Set_ColObject(nullptr);
					pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::NONE);
				}
			}
		}
	}

	return OBJ_NOEVENT;
}

void CInter_Totem::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
	ImGui::InputInt("ID", &m_iId);
}

void CInter_Totem::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	if (true == pGame_Instance->IsIn_Frustum_World(m_pTransform->Get_Row(CTransform::ERow::POS), 15.f))
		m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}
const _float& CInter_Totem::Get_Gauge(void){
	return m_fGauge;
}
const void CInter_Totem::Add_Gauge(const _float& fGauge) {
	m_fAddGauge = fGauge;
	m_fGauge += m_fAddGauge;
	Update_Network("Add_Gauge");
}

const HRESULT CInter_Totem::Render(const CRenderer::EOrder& eOrder) {
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
				m_pModel->Render(i, m_pShader, 8);
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		m_pCollider->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

void CInter_Totem::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Add_Gauge" == pKey) {
		root["Gauge"] = m_fAddGauge;
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_Totem::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>(iter["State"].asInt());
		}
		else if ("Add_Gauge" == key) {
			m_fAddGauge = static_cast<_float>(iter["State"].asFloat());
			m_fGauge += m_fAddGauge;
		}
	}
	listRecv.clear();
}
