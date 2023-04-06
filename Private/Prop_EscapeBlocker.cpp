#include "pch.h"
#include "Prop_EscapeBlocker.h"
#include "Shadow.h"
#include "Slasher_Huntress.h"
#include "Texture.h"

CProp_EscapeBlocker* const CProp_EscapeBlocker::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CProp_EscapeBlocker* pInstnace = new CProp_EscapeBlocker(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CProp_EscapeBlocker::Clone(void* const& pArg) {
	CProp_EscapeBlocker* pInstnace = new CProp_EscapeBlocker(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CProp_EscapeBlocker::CProp_EscapeBlocker(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CProp_EscapeBlocker::CProp_EscapeBlocker(const CProp_EscapeBlocker& rhs)
	: CObject(rhs) {
}

void CProp_EscapeBlocker::Delete(void) {
	__super::Delete();
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
}

const HRESULT CProp_EscapeBlocker::Init_Create(void) {
	return S_OK;
}

const HRESULT CProp_EscapeBlocker::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_Model"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_EscapeBlocker"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	m_pCollider->Set_Extents(_float3{ 2.f,2.f,2.f });
	return hr;
}

const _ubyte CProp_EscapeBlocker::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pCollider->Update(m_pTransform->Get_Matrix());

	CSlasher_Huntress* pSlasher = dynamic_cast<CSlasher_Huntress*>(pGame_Instance->Get_Object_Clone(static_cast<_uint>(EScene::GAMEPLAY), TEXT("Slasher_Huntress")));
	if (nullptr != pSlasher) {
		CCollider* pSlasherCollider = dynamic_cast<CCollider*>(pSlasher->Get_Component(TEXT("Collider")));
		CTransform* pSlasherTransform = dynamic_cast<CTransform*>(pSlasher->Get_Component(TEXT("Transform")));
		if (m_pCollider->Is_Collision(pSlasherCollider) == true) {
			m_fGoalDissolveLength = 6.f;
			if (m_fDissolveLength < m_fGoalDissolveLength) {
				m_fDissolveLength += 12.f * static_cast<_float>(dTimeDelta);
				if (m_fDissolveLength > m_fGoalDissolveLength)
					m_fDissolveLength = m_fGoalDissolveLength;
			}
			else if (m_fDissolveLength > m_fGoalDissolveLength) {
				m_fDissolveLength -= 12.f * static_cast<_float>(dTimeDelta);
				if (m_fDissolveLength < m_fGoalDissolveLength)
					m_fDissolveLength = m_fGoalDissolveLength;
			}
		}
	}
	return OBJ_NOEVENT;
}

void CProp_EscapeBlocker::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
	ImGui::DragFloat("Dissolve", &m_fGoalDissolveLength, 0.01f);
	ImGui::InputFloat3("Color", (_float*)&m_fDissolveColor);
	ImGui::InputFloat3("Color2", (_float*)&m_fDissolveColor2);
}

void CProp_EscapeBlocker::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CProp_EscapeBlocker::Render(const CRenderer::EOrder& eOrder) {
	if (m_bRender == true) {
		switch (eOrder) {
		case CRenderer::EOrder::NONALPHABLEND: {
			CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
			pCamera->Set_RawValue(m_pShader);
			CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
			CTexture* pDissolve = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_T_DissolveTexturesEntity")));
			CTexture* pAlpha = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_Fxt_verticalGradient1")));
			pDissolve->Set_Resource(m_pShader, "g_DissolveTexture", 0);
			pAlpha->Set_Resource(m_pShader, "g_DissolveAlpha", 0);
			m_pShader->Set_RawValue("g_DissolveLength", &m_fDissolveLength, sizeof(_float));
			m_pShader->Set_RawValue("g_DissolveColor", &m_fDissolveColor, sizeof(_float3));
			m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
			for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
				m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				m_pModel->Render(i, m_pShader, 7);
			}
			break;
		}
		case CRenderer::EOrder::NONLIGHT: {
			CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
			m_pCollider->Render(pCamera);

			break;
		}
		}
	}
	return S_OK;
}

void CProp_EscapeBlocker::Update_Network(const string& pkey)
{
}

void CProp_EscapeBlocker::Update_Network(void)
{
}

const HRESULT CProp_EscapeBlocker::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CProp_EscapeBlocker::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
}
