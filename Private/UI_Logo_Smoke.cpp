#include "pch.h"
#include "UI_Logo_Smoke.h"

CUI_Logo_Smoke* const CUI_Logo_Smoke::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CUI_Logo_Smoke* pInstnace = new CUI_Logo_Smoke(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CUI_Logo_Smoke::Clone(void* const& pArg) {
	CUI_Logo_Smoke* pInstnace = new CUI_Logo_Smoke(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CUI_Logo_Smoke::CUI_Logo_Smoke(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CUI_Image(pDevice, pContext, pNetwork) {
}

CUI_Logo_Smoke::CUI_Logo_Smoke(const CUI_Logo_Smoke& rhs)
	: CUI_Image(rhs) {
}

void CUI_Logo_Smoke::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTexture2);
}

const HRESULT CUI_Logo_Smoke::Init_Create(void) {
	HRESULT hr = S_OK;
	return hr;
}

const HRESULT CUI_Logo_Smoke::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Init_Clone(TEXT("T_TileNoise"));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::LOGO), TEXT("Texture_T_TileNoise"), TEXT("Texture1"), reinterpret_cast<CComponent*&>(m_pTexture2));
	return hr;
}

const _ubyte CUI_Logo_Smoke::Update(const _double& dTimeDelta) {
	__super::Update(dTimeDelta);
	m_vTrans.x += (_float)dTimeDelta * 0.03f;
	m_vTrans.y += (_float)dTimeDelta * 0.05f;

	return OBJ_NOEVENT;
}

void CUI_Logo_Smoke::Update_Edit(void) {
	__super::Update_Edit();
	ImGui::Separator();
}

void CUI_Logo_Smoke::Late_Update(const _double& dTimeDelta) {
	__super::Late_Update(dTimeDelta);
}

const HRESULT CUI_Logo_Smoke::Render(const CRenderer::EOrder& eOrder) {
	HRESULT hr = S_OK;
	if (!m_bActiveSelf)
		return hr;
	hr |= __super::Render(eOrder);
	hr |= m_pTexture2->Set_Resource(m_pShader, "g_FlowTexture", 0);
	hr |= m_pShader->Set_RawValue("g_vTrans", &m_vTrans, sizeof(_float2));
	_float fDelta = 0.5f;
	hr |= m_pShader->Set_RawValue("g_fDelta", &fDelta, sizeof(_float));
	hr |= m_pVBRect->Render(m_pShader, m_iPass);
	return hr;
}

const HRESULT CUI_Logo_Smoke::Save_Data(Json::Value& root) {
	__super::Save_Data(root);
	return S_OK;
}

void CUI_Logo_Smoke::Load_Data(Json::Value& root) {
	__super::Load_Data(root);
}
