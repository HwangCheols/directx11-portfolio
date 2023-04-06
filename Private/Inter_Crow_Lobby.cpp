#include "pch.h"
#include "Inter_Crow_Lobby.h"
#include "Slasher_Huntress.h"
#include "Shadow.h"

CInter_Crow_Lobby* const CInter_Crow_Lobby::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Crow_Lobby* pInstnace = new CInter_Crow_Lobby(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Crow_Lobby::Clone(void* const& pArg) {
	CInter_Crow_Lobby* pInstnace = new CInter_Crow_Lobby(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Crow_Lobby::CInter_Crow_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Crow_Lobby::CInter_Crow_Lobby(const CInter_Crow_Lobby& rhs)
	: CObject(rhs) {
}

void CInter_Crow_Lobby::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}

const HRESULT CInter_Crow_Lobby::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Crow_Lobby::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), TEXT("Model_Inter_Crow"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	return hr;
}

const _ubyte CInter_Crow_Lobby::Update(const _double& dTimeDelta) {
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
		if (m_dTime >= 3.0)
			m_bRender = false;
	}

	return OBJ_NOEVENT;
}

void CInter_Crow_Lobby::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(2);
}

void CInter_Crow_Lobby::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
}

const HRESULT CInter_Crow_Lobby::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			if (m_bRender == true)
				m_pModel->Render(i, m_pShader, 0);
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

const HRESULT CInter_Crow_Lobby::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CInter_Crow_Lobby::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
}

