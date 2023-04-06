#include "pch.h"
#include "Slasher_Huntress_Lobby.h"
#include "FM_Slasher.h"
#include "Shadow.h"
#include "Weapon_Slash_Lobby.h"

CSlasher_Huntress_Lobby* const CSlasher_Huntress_Lobby::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CSlasher_Huntress_Lobby* pInstance = new CSlasher_Huntress_Lobby(pDevice, pContext, pNetwork);
	if (FAILED(pInstance->Init_Create()))
		Safe_Release(pInstance);
	return pInstance;
}

CObject* const CSlasher_Huntress_Lobby::Clone(void* const& pArg) {
	CSlasher_Huntress_Lobby* pInstance = new CSlasher_Huntress_Lobby(*this);
	if (FAILED(pInstance->Init_Clone(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

CSlasher_Huntress_Lobby::CSlasher_Huntress_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CSlasher_Huntress_Lobby::CSlasher_Huntress_Lobby(const CSlasher_Huntress_Lobby& rhs)
	: CObject(rhs) {
}

void CSlasher_Huntress_Lobby::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pOrbitTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
}

const HRESULT CSlasher_Huntress_Lobby::Init_Create(void) {
	return S_OK;
}

const HRESULT CSlasher_Huntress_Lobby::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("OrbitTransform"), reinterpret_cast<CComponent*&>(m_pOrbitTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), TEXT("Model_Slasher_Huntress_Lobby"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	//m_pModel->Set_Anim("BE_DSkeleton_REF.ao|BE_Idle_Slash");

	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pTexture_Dissolve = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_T_DissolveTexturesEntity")));
	m_pTexture_Alpha = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_Fxt_verticalGradient1")));

	m_iId = 201;
	return hr;
}

const _ubyte CSlasher_Huntress_Lobby::Update(const _double& dTimeDelta) {
	if (m_fDissolveLength < m_fGoalDissolveLength) {
		m_fDissolveLength += 3.f * static_cast<_float>(dTimeDelta);
		if (m_fDissolveLength > m_fGoalDissolveLength)
			m_fDissolveLength = m_fGoalDissolveLength;
	}
	else if (m_fDissolveLength > m_fGoalDissolveLength) {
		m_fDissolveLength -= 3.f * static_cast<_float>(dTimeDelta);
		if (m_fDissolveLength < m_fGoalDissolveLength)
			m_fDissolveLength = m_fGoalDissolveLength;
	}

	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
	CTransform* pCamTransform = dynamic_cast<CTransform*>(pCamera->Get_Component(TEXT("Transform")));
	switch (m_eState)
	{
	case ESTATE::WATCH:
		m_iAnimIndex = 0;
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-50.f));
		break; 
	case ESTATE::IDLE:
		if (m_iAnimEnd == 1) {
			pGame_Instance->Create_Object_Clone(TEXT("Weapon_Throw"), static_cast<_uint>(EScene::LOBBY), TEXT("Weapon_Throw"));
			m_eState = ESTATE::IDLE_THROW;
			break;
		}
		else{
			m_iAnimIndex = 5;
		}
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-140.f));
		break;
	case ESTATE::IDLE_THROW:
		if (m_iAnimEnd == 1) {
			m_eState = ESTATE::IDLE;
			break;
		}
		m_iAnimIndex = 3;
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-140.f));
		break;
	case ESTATE::SELECT:
		break;
	}
	m_pModel->Set_Anim(m_iAnimIndex);
	m_iAnimEnd = m_pModel->Update_Anim(dTimeDelta,true);

	if ((_uint)(m_pModel->Get_Anim_TimeAcc() >= 20 && m_eState == ESTATE::IDLE_THROW))
		m_bRenderWeapon = true;
	if((_uint)(m_pModel->Get_Anim_TimeAcc() >= 390 && m_eState == ESTATE::IDLE_THROW))
		m_bRenderWeapon = false;

	Update_Network();
	return OBJ_NOEVENT;
}

void CSlasher_Huntress_Lobby::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	ImGui::DragFloat("Dissolve", &m_fGoalDissolveLength, 0.01f);
	ImGui::InputFloat3("Color", (_float*)&m_fDissolveColor);
	ImGui::InputFloat3("Color2", (_float*)&m_fDissolveColor2);

	ImGui::DragFloat("Roughness", &m_fRoughness, 0.001f);
	ImGui::DragFloat("Refract", &m_fRefract, 0.001f);
	ImGui::DragFloat("Metalness", &m_fMetalness, 0.001f);
}

void CSlasher_Huntress_Lobby::Late_Update(const _double& dTimeDelta) {
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::GLOW, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::PENETRATE, this);
}

const HRESULT CSlasher_Huntress_Lobby::Render(const CRenderer::EOrder& eOrder) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		//CTexture* pDissolve = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_T_DissolveTexturesEntity")));
		//CTexture* pAlpha = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_Fxt_verticalGradient1")));
		m_pTexture_Dissolve->Set_Resource(m_pShader, "g_DissolveTexture", 0);
		m_pTexture_Alpha->Set_Resource(m_pShader, "g_DissolveAlpha", 0);
		m_pShader->Set_RawValue("g_fRoughness", &m_fRoughness, sizeof(_float));
		m_pShader->Set_RawValue("g_fRefract", &m_fRefract, sizeof(_float));
		m_pShader->Set_RawValue("g_fMetalness", &m_fMetalness, sizeof(_float));
		m_pShader->Set_RawValue("g_DissolveLength", &m_fDissolveLength, sizeof(_float));
		m_pShader->Set_RawValue("g_DissolveColor", &m_fDissolveColor, sizeof(_float3));

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 2);
		}
		break;
	}
										 break;
	case CRenderer::EOrder::GLOW: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		//CTexture* pDissolve = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_T_DissolveTexturesEntity")));
		//CTexture* pAlpha = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_Fxt_verticalGradient1")));
		m_pTexture_Dissolve->Set_Resource(m_pShader, "g_DissolveTexture", 0);
		m_pTexture_Alpha->Set_Resource(m_pShader, "g_DissolveAlpha", 0);
		m_pShader->Set_RawValue("g_DissolveLength", &m_fDissolveLength, sizeof(_float));
		m_pShader->Set_RawValue("g_DissolveColor", &m_fDissolveColor2, sizeof(_float3));

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 4);
		}
		break;
	}
										 break;
	//case CRenderer::EOrder::PENETRATE: {
	//	CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
	//	pCamera->Set_RawValue(m_pShader);
	//	CTarget* pDepth = CTarget_Mgr::Get_Instance()->Get_Target(TEXT("Depth"));
	//	ID3D11ShaderResourceView* pDepthSRV = pDepth->Get_ShaderResourceView();
	//	m_pShader->Set_Resource("g_DepthTexture", pDepthSRV);
	//	_float4 vPenetrateColor= _float4{ 1.f, 0.f, 0.f, 1.f };
	//	m_pShader->Set_RawValue("g_vPenetrateColor", &vPenetrateColor, sizeof(_float4));
	//	m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
	//	for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
	//		m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
	//		m_pModel->Render(i, m_pShader, 3);
	//	}
	//}
	//								 break;
	}
	return S_OK;
}

void CSlasher_Huntress_Lobby::Set_Dissolve(const _float& m_fGoal) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_fGoalDissolveLength = m_fGoal;
	CWeapon_Slash_Lobby* pWeapon = dynamic_cast<CWeapon_Slash_Lobby*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Weapon_Slash_Lobby")));
	pWeapon->Set_Dissolve(2.5f);
}

const HRESULT CSlasher_Huntress_Lobby::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CSlasher_Huntress_Lobby::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
}

void CSlasher_Huntress_Lobby::Update_Network(const string& pKey) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	Json::Value root;
	if ("Set_Dissolve" == pKey) {
		root["Dissolve"] = m_fGoalDissolveLength;
		CWeapon_Slash_Lobby* pWeapon = dynamic_cast<CWeapon_Slash_Lobby*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Weapon_Slash_Lobby")));
		pWeapon->Set_Dissolve(2.5f);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CSlasher_Huntress_Lobby::Update_Network(void) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	list<Json::Value>& list = m_pNetwork->Get_Recv(m_iId);
	for (auto& root : list) {
		string Key = root["Key"].asString();
		if ("Set_Dissolve" == Key) {
			m_fGoalDissolveLength = root["Dissolve"].asFloat();
			CWeapon_Slash_Lobby* pWeapon = dynamic_cast<CWeapon_Slash_Lobby*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Weapon_Slash_Lobby")));
			pWeapon->Set_Dissolve(2.5f);
		}
	}
	list.clear();
}
