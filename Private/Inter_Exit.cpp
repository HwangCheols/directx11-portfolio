#include "pch.h"
#include "Inter_Exit.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"

CInter_Exit* const CInter_Exit::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Exit* pInstnace = new CInter_Exit(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Exit::Clone(void* const& pArg) {
	CInter_Exit* pInstnace = new CInter_Exit(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Exit::CInter_Exit(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Exit::CInter_Exit(const CInter_Exit& rhs)
	: CObject(rhs) {
}

void CInter_Exit::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pCollider);
}

const HRESULT CInter_Exit::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Exit::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	return hr;
}

const _ubyte CInter_Exit::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	//collsion
	CInfo* pInfo = CInfo::Get_Instance();
	if (CInfo::ECamper::CHERYL == pInfo->Get_Camper()) {
		list<CObject*>* plistObject = pGame_Instance->Get_Object_ListClone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay"));
		if (nullptr == plistObject)
			return OBJ_NOEVENT;
		for (auto& iter : *plistObject) {
			CCamper_GamePlay* pCamper = dynamic_cast<CCamper_GamePlay*>(iter);
			CCollider* pCamperCollider = dynamic_cast<CCollider*>(pCamper->Get_Component(TEXT("Collider")));
			if (pCamper->Get_Control()) {
				if (m_pCollider->Is_Collision(pCamperCollider)) {
					pCamper->Get_FM_Camper()->Set_State(CFM_Camper::EState::Ending_Run);
				}				
			}
		}
	}

	m_pCollider->Update(m_pTransform->Get_Matrix());
	//Update_Network();

	return OBJ_NOEVENT;
}

void CInter_Exit::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
}

void CInter_Exit::Late_Update(const _double& dTimeDelta) {
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CInter_Exit::Render(const CRenderer::EOrder& eOrder) {
	CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
	//m_pCollider->Render(pCamera);
	return S_OK;
}

const HRESULT CInter_Exit::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CInter_Exit::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
}
