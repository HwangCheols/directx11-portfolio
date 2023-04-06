#include "pch.h"
#include "Camper_Lobby_Sit.h"
#include "FM_Slasher.h"
#include "Shadow.h"

CCamper_Lobby_Sit* const CCamper_Lobby_Sit::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CCamper_Lobby_Sit* pInstance = new CCamper_Lobby_Sit(pDevice, pContext, pNetwork);
	if (FAILED(pInstance->Init_Create()))
		Safe_Release(pInstance);
	return pInstance;
}

CObject* const CCamper_Lobby_Sit::Clone(void* const& pArg) {
	CCamper_Lobby_Sit* pInstance = new CCamper_Lobby_Sit(*this);
	if (FAILED(pInstance->Init_Clone(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

CCamper_Lobby_Sit::CCamper_Lobby_Sit(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CCamper_Lobby_Sit::CCamper_Lobby_Sit(const CCamper_Lobby_Sit& rhs)
	: CObject(rhs) {
}

void CCamper_Lobby_Sit::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pOrbitTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pModel2);
}

const HRESULT CCamper_Lobby_Sit::Init_Create(void) {
	return S_OK;
}

const HRESULT CCamper_Lobby_Sit::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("OrbitTransform"), reinterpret_cast<CComponent*&>(m_pOrbitTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), TEXT("Model_Lobby_Cheryl"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));

	return hr;
}

const _ubyte CCamper_Lobby_Sit::Update(const _double& dTimeDelta) {
	if (m_fDissolveLength < m_fGoalDissolveLength) {
		m_fDissolveLength += 3.f * static_cast<_float>(dTimeDelta);
		if (m_fDissolveLength > m_fGoalDissolveLength)
			m_fDissolveLength = m_fGoalDissolveLength;
	}
	else {
		m_fDissolveLength -= 3.f * static_cast<_float>(dTimeDelta);
		if (m_fDissolveLength < m_fGoalDissolveLength)
			m_fDissolveLength = m_fGoalDissolveLength;
	}
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
	CTransform* pCamTransform = dynamic_cast<CTransform*>(pCamera->Get_Component(TEXT("Transform")));
	switch (m_eState)
	{
	case ESTATE::SIT:
		m_iAnimIndex = 2;
		break;
	case ESTATE::IDEL:
		m_iAnimIndex = 1;
		//m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(52.09f, 0.f, 35.07f,1.f));
		//m_pTransform->LookAt(XMVectorSet(46.212f, 0.f, 30.647f, 1.f));
		break;
	case ESTATE::IDEL2:
		m_iAnimIndex = 1;
		//m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(51.333f, 0.f, 35.722f, 1.f));
		//m_pTransform->LookAt(XMVectorSet(46.212f, 0.f, 30.647f, 1.f));
		break;
	case ESTATE::IDEL3:
		m_iAnimIndex = 1;
		//m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(50.401f, 0.f, 36.193f, 1.f));
		//m_pTransform->LookAt(XMVectorSet(46.212f, 0.f, 30.647f, 1.f));
		break;
	case ESTATE::IDEL4:
		m_iAnimIndex = 1;
		//m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(49.142f, 0.f, 35.647f, 1.f));
		//m_pTransform->LookAt(XMVectorSet(46.212f, 0.f, 30.647f, 1.f));
		break;
	case ESTATE::SELECT:
		m_iAnimIndex = 0;
		m_pTransform->LookAt(pCamTransform->Get_Row(CTransform::ERow::POS));
		break;
	}
	m_pModel->Set_Anim(m_iAnimIndex);
	m_iAnimEnd = m_pModel->Update_Anim(dTimeDelta, true);
	m_pCollider->Update(m_pTransform->Get_Matrix());

	return OBJ_NOEVENT;
}

void CCamper_Lobby_Sit::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
}

void CCamper_Lobby_Sit::Late_Update(const _double& dTimeDelta) {
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::GLOW, this);
}

const HRESULT CCamper_Lobby_Sit::Render(const CRenderer::EOrder& eOrder) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		CTexture* pDissolve = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_T_DissolveTexturesEntity")));
		CTexture* pAlpha = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_Fxt_verticalGradient1")));
		pDissolve->Set_Resource(m_pShader, "g_DissolveTexture", 0);
		pAlpha->Set_Resource(m_pShader, "g_DissolveAlpha", 0);
		m_pShader->Set_RawValue("g_DissolveLength", &m_fDissolveLength, sizeof(_float));
		m_pShader->Set_RawValue("g_DissolveColor", &m_fDissolveColor, sizeof(_float3));

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 2);
		}
		break;
	}
	case CRenderer::EOrder::GLOW: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		CTexture* pDissolve = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_T_DissolveTexturesEntity")));
		CTexture* pAlpha = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_Fxt_verticalGradient1")));
		pDissolve->Set_Resource(m_pShader, "g_DissolveTexture", 0);
		pAlpha->Set_Resource(m_pShader, "g_DissolveAlpha", 0);
		m_pShader->Set_RawValue("g_DissolveLength", &m_fDissolveLength, sizeof(_float));
		m_pShader->Set_RawValue("g_DissolveColor", &m_fDissolveColor2, sizeof(_float3));

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 4);
		}
		break;
	}
	}
	return S_OK;
}

void CCamper_Lobby_Sit::Set_Dissolve(const _float& m_fGoal) {
	m_fGoalDissolveLength = m_fGoal;
}

const HRESULT CCamper_Lobby_Sit::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CCamper_Lobby_Sit::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
}
