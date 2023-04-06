#include "pch.h"
#include "Inter_Crow.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "Shadow.h"

CInter_Crow* const CInter_Crow::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Crow* pInstnace = new CInter_Crow(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Crow::Clone(void* const& pArg) {
	CInter_Crow* pInstnace = new CInter_Crow(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Crow::CInter_Crow(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Crow::CInter_Crow(const CInter_Crow& rhs)
	: CObject(rhs) {
}

void CInter_Crow::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}

const HRESULT CInter_Crow::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Crow::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Crow"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	m_pCollider->Set_Center(_float3{ 0.f,0.f,0.f });
	m_pCollider->Set_Extents(_float3{ 3.f,3.f,3.f });
	m_eState = ESTATE::IDLE;

	return hr;
}

const _ubyte CInter_Crow::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	//state
	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case ESTATE::IDLE:
			m_pModel->Set_Anim("Crow_REF.ao|Crow_Idle");
			m_ePreState = ESTATE::IDLE;
			m_bLoop = true;
			m_bRender = true;
			m_dTime = 0.;
			break;
		case ESTATE::FLY:
			m_bLoop = false;
			m_pModel->Set_Anim("Crow_REF.ao|Crow_FlyAway");
			m_ePreState = ESTATE::FLY;
			break;
		case ESTATE::LAND:
			m_bLoop = false;
			m_pModel->Set_Anim("Crow_REF.ao|Crow_Land");
			m_ePreState = ESTATE::LAND;
			break;
		}
	}
	m_iAnimFinish = m_pModel->Update_Anim(dTimeDelta, m_bLoop);
	m_pCollider->Update(m_pTransform->Get_Matrix());
	if (m_eState == ESTATE::FLY) {
		m_dTime += dTimeDelta;
		if (m_dTime >= 10.0)
			m_bRender = false;
		if (m_dTime >= 15.0) {
			m_eState = ESTATE::LAND;
			m_dTime = 0.0;
			m_bRender = true;
		}
	}
	else if (m_eState == ESTATE::LAND) {
		if (m_iAnimFinish == 1)
			m_eState = ESTATE::IDLE;
	}
	//collsion
	if (nullptr == m_plistObject)
		m_plistObject = pGame_Instance->Get_Object_ListClone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay"));
	if (nullptr == m_plistObject)
		return OBJ_NOEVENT;
	for (auto& iter : *m_plistObject) {
		CCamper_GamePlay* pCamper = dynamic_cast<CCamper_GamePlay*>(iter);
		CCollider* pCamperCollider = dynamic_cast<CCollider*>(pCamper->Get_Component(TEXT("Collider")));
		if (m_eState == ESTATE::IDLE && pCamper->Get_HaveStayCrowPerk() == false && true == m_pCollider->Is_Collision(pCamperCollider)) {
			m_eState = ESTATE::FLY;
		}
	}
	if (nullptr == m_pSlasher)
		m_pSlasher = dynamic_cast<CSlasher_Huntress*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Slasher_Huntress")));
	if (nullptr == m_pSlasher)
		return OBJ_NOEVENT;
	CCollider* pSlaherCollider = dynamic_cast<CCollider*>(m_pSlasher->Get_Component(TEXT("Collider")));
	if (nullptr == pSlaherCollider)
		return OBJ_NOEVENT;
	if (m_eState == ESTATE::IDLE && true == m_pCollider->Is_Collision(pSlaherCollider))
		m_eState = ESTATE::FLY;

	return OBJ_NOEVENT;
}

void CInter_Crow::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
}

void CInter_Crow::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
}

const HRESULT CInter_Crow::Render(const CRenderer::EOrder& eOrder) {
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
		////m_pCollider->Render(pCamera);
		break;
	}
	case CRenderer::EOrder::SHADOW: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		pShadow->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 1);
		}
	}
	}
	return S_OK;

}

void CInter_Crow::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_Crow::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>(iter["State"].asInt());
		}
	}
	listRecv.clear();
}

const HRESULT CInter_Crow::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CInter_Crow::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
}
