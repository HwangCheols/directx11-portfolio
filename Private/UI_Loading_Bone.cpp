#include "pch.h"
#include "UI_Loading_Bone.h"

CUI_Loading_Bone* const CUI_Loading_Bone::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CUI_Loading_Bone* pInstnace = new CUI_Loading_Bone(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CUI_Loading_Bone::Clone(void* const& pArg) {
	CUI_Loading_Bone* pInstnace = new CUI_Loading_Bone(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CUI_Loading_Bone::CUI_Loading_Bone(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CUI_Image(pDevice, pContext, pNetwork) {
}

CUI_Loading_Bone::CUI_Loading_Bone(const CUI_Loading_Bone& rhs)
	: CUI_Image(rhs) {
}

void CUI_Loading_Bone::Delete(void) {
	__super::Delete();
}

const HRESULT CUI_Loading_Bone::Init_Create(void) {
	HRESULT hr = S_OK;
	return hr;
}

const HRESULT CUI_Loading_Bone::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Init_Clone();
	__super::Add_Component(static_cast<_ubyte>(EScene::STATIC), Get_TextureKey(L"LoadingScreen_I8").c_str(), TEXT("Texture"), reinterpret_cast<CComponent*&>(m_pTexture));
	m_pRectTransform->Set_State(CRectTransform::EState::SCL, _float3(7.f, 3.5f, 1.f));
	m_pRectTransform->Set_State(CRectTransform::EState::POS, _float3(0.f, -15.f, 0.f));
	m_iPass = 1;
	m_vColor = { 0.4f, 0.4f, 0.4f, 1.f };
	m_iDepth = 1;

	this->Update(0.02);
	return hr;
}

const _ubyte CUI_Loading_Bone::Update(const _double& dTimeDelta) {
	__super::Update(dTimeDelta);
	if (!m_bActiveSelf)
		return OBJ_NOEVENT;
	return OBJ_NOEVENT;
}

void CUI_Loading_Bone::Update_Edit(void) {
	__super::Update_Edit();
	ImGui::Separator();
}

void CUI_Loading_Bone::Late_Update(const _double& dTimeDelta) {
	__super::Late_Update(dTimeDelta);
}

const HRESULT CUI_Loading_Bone::Render(const CRenderer::EOrder& eOrder) {
	HRESULT hr = S_OK;
	if (!m_bActiveSelf)
		return hr;
	hr |= __super::Render(eOrder);
	hr |= m_pVBRect->Render(m_pShader, m_iPass);
	return hr;
}
