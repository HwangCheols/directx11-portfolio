#include "pch.h"
#include "Prop_Lobby_PoliceCar.h"
#include "Shadow.h"

CProp_Lobby_PoliceCar* const CProp_Lobby_PoliceCar::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CProp_Lobby_PoliceCar* pInstnace = new CProp_Lobby_PoliceCar(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CProp_Lobby_PoliceCar::Clone(void* const& pArg) {
	CProp_Lobby_PoliceCar* pInstnace = new CProp_Lobby_PoliceCar(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CProp_Lobby_PoliceCar::CProp_Lobby_PoliceCar(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CProp_Lobby_PoliceCar::CProp_Lobby_PoliceCar(const CProp_Lobby_PoliceCar& rhs)
	: CObject(rhs) {
}

void CProp_Lobby_PoliceCar::Delete(void) {
	__super::Delete();
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
}

const HRESULT CProp_Lobby_PoliceCar::Init_Create(void) {
	return S_OK;
}

const HRESULT CProp_Lobby_PoliceCar::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_Model"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), TEXT("Model_policecar"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));

	CLight::SDesc sDesc;
	ZeroMemory(&sDesc, sizeof(CLight::SDesc));
	sDesc.eType = CLight::EType::POINT;
	sDesc.fRange = 6.327f;
	sDesc.vDiffuse = { 0.716f,0.f,0.f, 1.f };
	sDesc.vAmbient = { 0.324f,0.f,0.f, 1.f };
	sDesc.vSpecular = { 0.303f,0.f,0.f, 0.f };
	m_pRedLight = pGame_Instance->CreateGet_Object_Clone(TEXT("Light_Police"), static_cast<_ubyte>(EScene::LOBBY), TEXT("Light_Police"), &sDesc);

	ZeroMemory(&sDesc, sizeof(CLight::SDesc));
	sDesc.eType = CLight::EType::POINT;
	sDesc.fRange = 6.327f;
	sDesc.vDiffuse = { 0.f,0.f,0.716f, 1.f };
	sDesc.vAmbient = { 0.f,0.f,0.324f, 1.f };
	sDesc.vSpecular = { 0.f,0.f,0.303f, 0.f };
	m_pBlueLight = pGame_Instance->CreateGet_Object_Clone(TEXT("Light_Police"), static_cast<_ubyte>(EScene::LOBBY), TEXT("Light_Police"), &sDesc);

	return hr;
}

const _ubyte CProp_Lobby_PoliceCar::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	m_pCollider->Update(m_pTransform->Get_Matrix());
	Update_Network();

	m_dBlinkTime += dTimeDelta;
	if (m_dBlinkTime > 1.) {
		m_bBlink = !m_bBlink;
		m_dBlinkTime = 0.;
	}

	if (m_bBlink) {
		reinterpret_cast<CLight_Police*&>(m_pBlueLight)->Set_OnOff(true);
		reinterpret_cast<CLight_Police*&>(m_pRedLight)->Set_OnOff(false);
	}
	else {
		reinterpret_cast<CLight_Police*&>(m_pBlueLight)->Set_OnOff(false);
		reinterpret_cast<CLight_Police*&>(m_pRedLight)->Set_OnOff(true);
	}
	return OBJ_NOEVENT;
}

void CProp_Lobby_PoliceCar::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	ImGui::DragFloat("Roughness", &m_fRoughness, 0.001f);
	ImGui::DragFloat("Refract", &m_fRefract, 0.001f);
	ImGui::DragFloat("Metalness", &m_fMetalness, 0.001f);
	ImGui::DragFloat2("GlowBuffer", (_float*)&m_fGlow, 0.01f);
}

void CProp_Lobby_PoliceCar::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	if (true == pGame_Instance->IsIn_Frustum_World(m_pTransform->Get_Row(CTransform::ERow::POS), 15.f))
		if (nullptr != m_pModel) {
			m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
			m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
			m_pRenderer->Add_Object(CRenderer::EOrder::GLOW, this);
			//m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
		}
}

const HRESULT CProp_Lobby_PoliceCar::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {

	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_fRoughness", &m_fRoughness, sizeof(_float));
		m_pShader->Set_RawValue("g_fRefract", &m_fRefract, sizeof(_float));
		m_pShader->Set_RawValue("g_fMetalness", &m_fMetalness, sizeof(_float));
		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 0);
		}
		break;
	}
	case CRenderer::EOrder::SHADOW: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		pShadow->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 3);
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		//m_pCollider->Render(pCamera);
		break;
	}
	case CRenderer::EOrder::GLOW: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		m_pShader->Set_RawValue("g_bBlink", &m_bBlink, sizeof(_bool));
		m_pShader->Set_RawValue("g_fGlow_Buffer", &m_fGlow, sizeof(_float2));
		m_pModel->Set_Resource(2, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
		m_pModel->Set_Resource(2, aiTextureType_EMISSIVE, m_pShader, "g_EmissiveTexture");
		
		m_pModel->Render(2, m_pShader, 5);
		break;
	}
	}
	return S_OK;
}

void CProp_Lobby_PoliceCar::Update_Network(const string& pkey) {
	Json::Value root;

}

void CProp_Lobby_PoliceCar::Update_Network(void) {
	list<Json::Value> listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
	}

	listRecv.clear();
}
const HRESULT CProp_Lobby_PoliceCar::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CProp_Lobby_PoliceCar::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);

	CTransform* pRedTransform = dynamic_cast<CTransform*>(m_pRedLight->Get_Component(TEXT("Transform")));
	pRedTransform->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS) - m_pTransform->Get_Row(CTransform::ERow::RIGHT) * 0.5f + XMVectorSet(0.f, 2.f, 0.f, 0.f));
	CTransform* pBlueTransform = dynamic_cast<CTransform*>(m_pBlueLight->Get_Component(TEXT("Transform")));
	pBlueTransform->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS) + m_pTransform->Get_Row(CTransform::ERow::RIGHT) * 0.5f + XMVectorSet(0.f, 2.f, 0.f, 0.f));
}