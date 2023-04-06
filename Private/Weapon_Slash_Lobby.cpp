#include "pch.h"
#include "Weapon_Slash_Lobby.h"
#include "Slasher_Huntress_Lobby.h"

CWeapon_Slash_Lobby* const CWeapon_Slash_Lobby::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CWeapon_Slash_Lobby* pInstance = new CWeapon_Slash_Lobby(pDevice, pContext, pNetwork);
	if (FAILED(pInstance->Init_Create()))
		Safe_Release(pInstance);
	return pInstance;
}

CObject* const CWeapon_Slash_Lobby::Clone(void* const& pArg) {
	CWeapon_Slash_Lobby* pInstance = new CWeapon_Slash_Lobby(*this);
	if (FAILED(pInstance->Init_Clone(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

CWeapon_Slash_Lobby::CWeapon_Slash_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CWeapon_Slash_Lobby::CWeapon_Slash_Lobby(const CWeapon_Slash_Lobby& rhs)
	: CObject(rhs) {
}

void CWeapon_Slash_Lobby::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
}

const HRESULT CWeapon_Slash_Lobby::Init_Create(void) {
	return S_OK;
}

const HRESULT CWeapon_Slash_Lobby::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_Model"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), TEXT("Model_Weapon_Slash_Lobby"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));

	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_Texture_pDissolve = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_T_DissolveTexturesEntity")));
	m_Texture_pAlpha = dynamic_cast<CTexture*>(pGame_Instance->Get_Component_Prototype(static_cast<_ubyte>(EScene::STATIC), TEXT("Texture_Fxt_verticalGradient1")));
	return hr;
}

const _ubyte CWeapon_Slash_Lobby::Update(const _double& dTimeDelta) {
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
	return OBJ_NOEVENT;
}

void CWeapon_Slash_Lobby::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	ImGui::DragFloat("Dissolve", &m_fGoalDissolveLength, 0.01f);

	ImGui::DragFloat("Roughness", &m_fRoughness, 0.001f);
	ImGui::DragFloat("Refract", &m_fRefract, 0.001f);
	ImGui::DragFloat("Metalness", &m_fMetalness, 0.001f);
}

void CWeapon_Slash_Lobby::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	CSlasher_Huntress_Lobby* pSlasher = dynamic_cast<CSlasher_Huntress_Lobby*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::LOBBY), TEXT("Slasher_Huntress_Lobby")));
	if (nullptr == pSlasher)
		return;
	CTransform* pSlasherTransform = dynamic_cast<CTransform*>(pSlasher->Get_Component(TEXT("Transform")));
	if (nullptr == pSlasherTransform)
		return;
	_matrix BoneMatrix;
	CModel* pModel = dynamic_cast<CModel*>(pSlasher->Get_Component(TEXT("Model")));
	if (pSlasher->Get_State() == CSlasher_Huntress_Lobby::ESTATE::IDLE || pSlasher->Get_State() == CSlasher_Huntress_Lobby::ESTATE::IDLE_THROW)
		BoneMatrix = pModel->Get_BoneMatrix("joint_WeaponRT_01");
	else
		BoneMatrix = pModel->Get_BoneMatrix("joint_WeaponLT_01");


	_matrix SocketMatrix;

	SocketMatrix = BoneMatrix * pSlasherTransform->Get_Matrix();

	m_pTransform->Set_Row(CTransform::ERow::RIGHT, -SocketMatrix.r[0]);
	m_pTransform->Set_Row(CTransform::ERow::UP, SocketMatrix.r[1]);
	m_pTransform->Set_Row(CTransform::ERow::LOOK, -SocketMatrix.r[2]);
	m_pTransform->Set_Row(CTransform::ERow::POS, SocketMatrix.r[3]);

	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::GLOW, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CWeapon_Slash_Lobby::Render(const CRenderer::EOrder& eOrder) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_Texture_pDissolve->Set_Resource(m_pShader, "g_DissolveTexture", 0);
		m_Texture_pAlpha->Set_Resource(m_pShader, "g_DissolveAlpha", 0);
		m_pShader->Set_RawValue("g_DissolveLength", &m_fDissolveLength, sizeof(_float));
		m_pShader->Set_RawValue("g_DissolveColor", &m_fDissolveColor, sizeof(_float3));
		m_pShader->Set_RawValue("g_fRoughness", &m_fRoughness, sizeof(_float));
		m_pShader->Set_RawValue("g_fRefract", &m_fRefract, sizeof(_float));
		m_pShader->Set_RawValue("g_fMetalness", &m_fMetalness, sizeof(_float));
		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 6);
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
			m_pModel->Render(i, m_pShader, 7);
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		break;
	}
	}
	return S_OK;
}

void CWeapon_Slash_Lobby::Set_Dissolve(const _float& m_fGoal) {
	m_fGoalDissolveLength = m_fGoal;
}

const HRESULT CWeapon_Slash_Lobby::Save_Data(Json::Value& root) {
	return S_OK;
}

void CWeapon_Slash_Lobby::Load_Data(Json::Value& root) {

}
