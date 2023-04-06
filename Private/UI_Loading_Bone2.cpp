#include "pch.h"
#include "UI_Loading_Bone2.h"

CUI_Loading_Bone2* const CUI_Loading_Bone2::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CUI_Loading_Bone2* pInstnace = new CUI_Loading_Bone2(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CUI_Loading_Bone2::Clone(void* const& pArg) {
	CUI_Loading_Bone2* pInstnace = new CUI_Loading_Bone2(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CUI_Loading_Bone2::CUI_Loading_Bone2(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CUI_Image(pDevice, pContext, pNetwork) {
}

CUI_Loading_Bone2::CUI_Loading_Bone2(const CUI_Loading_Bone2& rhs)
	: CUI_Image(rhs) {
}

void CUI_Loading_Bone2::Delete(void) {
	__super::Delete();
	//for (_uint i = 0; i < 5; ++i) {
	//	Safe_Release(m_pBoneRectTransform[i]);
	//	Safe_Release(m_pBoneTexture[i]);
	//}
}

const HRESULT CUI_Loading_Bone2::Init_Create(void) {
	HRESULT hr = S_OK;
	return hr;
}

const HRESULT CUI_Loading_Bone2::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Init_Clone();

	if (nullptr != pArg) {
		memcpy(&m_iType, pArg, sizeof(_int));
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		if (nullptr != m_pTexture)
			Safe_Release(m_pTexture);
		switch (static_cast<EType>(m_iType))
		{
		case Client::CUI_Loading_Bone2::EType::ONE: {
			__super::Add_Component(static_cast<_ubyte>(EScene::STATIC), Get_TextureKey(L"LoadingScreen_B1").c_str(), TEXT("Texture"), reinterpret_cast<CComponent*&>(m_pTexture));
			CUI_Image* pParent = dynamic_cast<CUI_Image*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOADING), L"Loading_Bone"));
			this->Set_Parent(pParent);
			m_pRectTransform->Set_Size(12.5f, 100.f);
			m_pRectTransform->Set_State(CRectTransform::EState::POS, _float3(-15.4f, 0.f, 0.f));
		}
												  break;
		case Client::CUI_Loading_Bone2::EType::TWO: {
			__super::Add_Component(static_cast<_ubyte>(EScene::STATIC), Get_TextureKey(L"LoadingScreen_B2").c_str(), TEXT("Texture"), reinterpret_cast<CComponent*&>(m_pTexture));
			CUI_Image* pParent = dynamic_cast<CUI_Image*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOADING), L"Loading_Bone"));
			this->Set_Parent(pParent);
			m_pRectTransform->Set_Size(12.5f, 100.f);
			m_pRectTransform->Set_State(CRectTransform::EState::POS, _float3(-5.58f, -0.01f, 0.f));
		}
												  break;
		case Client::CUI_Loading_Bone2::EType::THREE: {
			__super::Add_Component(static_cast<_ubyte>(EScene::STATIC), Get_TextureKey(L"LoadingScreen_B3").c_str(), TEXT("Texture"), reinterpret_cast<CComponent*&>(m_pTexture));
			CUI_Image* pParent = dynamic_cast<CUI_Image*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOADING), L"Loading_Bone"));
			this->Set_Parent(pParent);
			m_pRectTransform->Set_Size(12.5f, 100.f);
			m_pRectTransform->Set_State(CRectTransform::EState::POS, _float3(4.05f, 0.2f, 0.f));
		}
													break;
		case Client::CUI_Loading_Bone2::EType::FOUR: {
			__super::Add_Component(static_cast<_ubyte>(EScene::STATIC), Get_TextureKey(L"LoadingScreen_B4").c_str(), TEXT("Texture"), reinterpret_cast<CComponent*&>(m_pTexture));
			CUI_Image* pParent = dynamic_cast<CUI_Image*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOADING), L"Loading_Bone"));
			this->Set_Parent(pParent);
			m_pRectTransform->Set_Size(12.5f, 100.f);
			m_pRectTransform->Set_State(CRectTransform::EState::POS, _float3(12.97f, 0.03f, 0.f));
		}
												   break;
		case Client::CUI_Loading_Bone2::EType::FIVE: {
			__super::Add_Component(static_cast<_ubyte>(EScene::STATIC), Get_TextureKey(L"LoadingScreen_B5").c_str(), TEXT("Texture"), reinterpret_cast<CComponent*&>(m_pTexture));
			CUI_Image* pParent = dynamic_cast<CUI_Image*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOADING), L"Loading_Bone"));
			this->Set_Parent(pParent);
			m_pRectTransform->Set_Size(100.f, 50.f);
			m_pRectTransform->Set_State(CRectTransform::EState::POS, _float3(-1.1f, -0.16f, 0.f));
		}
												   break;
		default: return hr;
		}

		m_iPass = static_cast<_uint>(EUIPass::PASS_LOADING);
		m_iDepth = 3 + (static_cast<_uint>(EType::END) - 1) - m_iType;
	}

	this->Update(0.02);
	return hr;
}

const _ubyte CUI_Loading_Bone2::Update(const _double& dTimeDelta) {
	if (m_bDead)
		return OBJ_ERASE;
	__super::Update(dTimeDelta);
	if (!m_bActiveSelf)
		return OBJ_NOEVENT;

	m_fLoadingTime += (_float)dTimeDelta;
	if (m_fLoadingTime >= 5.0f)
		m_fLoadingTime = 0.0f;

	return OBJ_NOEVENT;
}

void CUI_Loading_Bone2::Update_Edit(void) {
	__super::Update_Edit();
	ImGui::Separator();
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	ImGui::SameLine();
	ImGui::SliderInt("type", (_int*)&m_iType, 0, static_cast<_int>(EType::END) - 1, EType_ToString(static_cast<EType>(m_iType)));
}

void CUI_Loading_Bone2::Late_Update(const _double& dTimeDelta) {
	__super::Late_Update(dTimeDelta);
}

const HRESULT CUI_Loading_Bone2::Render(const CRenderer::EOrder& eOrder) {
	HRESULT hr = S_OK;
	if (!m_bActiveSelf || nullptr == m_pTexture)
		return hr;

	m_fRatio = m_fLoadingTime;
	m_fRatio /= 6.f;

	hr |= __super::Set_RawValue(m_pShader);
	hr |= m_pTexture->Set_Resource(m_pShader, "g_DiffuseTexture", 0);
	hr |= m_pShader->Set_RawValue("g_WorldMatrix", &m_pRectTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
	hr |= m_pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));

	switch (static_cast<EType>(m_iType))
	{
	case Client::CUI_Loading_Bone2::EType::ONE: {
		if (m_fRatio <= 0.15f) {
			_float fRatio = m_fRatio * 5.f;
			hr |= m_pShader->Set_RawValue("g_fLoading", &fRatio, sizeof(_float));
			hr |= m_pVBRect->Render(m_pShader, m_iPass);
		}
		else
			hr |= m_pVBRect->Render(m_pShader, 1);
	}
											  break;
	case Client::CUI_Loading_Bone2::EType::TWO: {
		if (m_fRatio > 0.15f || m_fRatio <= 0.3f) {
			_float fRatio = (m_fRatio - 0.15f) * 5.f;
			hr |= m_pShader->Set_RawValue("g_fLoading", &fRatio, sizeof(_float));
			hr |= m_pVBRect->Render(m_pShader, m_iPass);
		}
		else
			hr |= m_pVBRect->Render(m_pShader, 1);
	}
											  break;
	case Client::CUI_Loading_Bone2::EType::THREE: {
		if (m_fRatio > 0.3f || m_fRatio <= 0.45f) {
			_float fRatio = (m_fRatio - 0.3f) * 5.f;
			hr |= m_pShader->Set_RawValue("g_fLoading", &fRatio, sizeof(_float));
			hr |= m_pVBRect->Render(m_pShader, m_iPass);
		}
		else
			hr |= m_pVBRect->Render(m_pShader, 1);
	}
												break;
	case Client::CUI_Loading_Bone2::EType::FOUR: {
		if (m_fRatio > 0.45f || m_fRatio <= 0.6f) {
			_float fRatio = (m_fRatio - 0.45f) * 5.f;
			hr |= m_pShader->Set_RawValue("g_fLoading", &fRatio, sizeof(_float));
			hr |= m_pVBRect->Render(m_pShader, m_iPass);
		}
		else
			hr |= m_pVBRect->Render(m_pShader, 1);
	}
											   break;
	case Client::CUI_Loading_Bone2::EType::FIVE: {
		if (m_fRatio >= 0.7f || m_fRatio < 1.f) {
			_float fRatio = (m_fRatio - 0.7f) * 10.f;
			hr |= m_pShader->Set_RawValue("g_fLoading", &fRatio, sizeof(_float));
			hr |= m_pVBRect->Render(m_pShader, m_iPass);
		}
		else
			hr |= m_pVBRect->Render(m_pShader, 1);
	}
											   break;
	}
	
	return hr;
}
