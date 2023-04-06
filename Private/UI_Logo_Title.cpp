#include "pch.h"
#include "UI_Logo_Title.h"

CUI_Logo_Title* const CUI_Logo_Title::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CUI_Logo_Title* pInstnace = new CUI_Logo_Title(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CUI_Logo_Title::Clone(void* const& pArg) {
	CUI_Logo_Title* pInstnace = new CUI_Logo_Title(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CUI_Logo_Title::CUI_Logo_Title(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CUI_Image(pDevice, pContext, pNetwork) {
}

CUI_Logo_Title::CUI_Logo_Title(const CUI_Logo_Title& rhs)
	: CUI_Image(rhs) {
}

void CUI_Logo_Title::Delete(void) {
	__super::Delete();
}

const HRESULT CUI_Logo_Title::Init_Create(void) {
	HRESULT hr = S_OK;
	hr |= __super::Init(g_iWinSizeX, g_iWinSizeY);
	return hr;
}

const HRESULT CUI_Logo_Title::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Init_Clone(TEXT("SplashScreen_I1"));
	return hr;
}

const _ubyte CUI_Logo_Title::Update(const _double& dTimeDelta) {
	__super::Update(dTimeDelta);
	return OBJ_NOEVENT;
}

void CUI_Logo_Title::Update_Edit(void) {
	__super::Update_Edit();
	ImGui::Separator();
}

void CUI_Logo_Title::Late_Update(const _double& dTimeDelta) {
	__super::Late_Update(dTimeDelta);

}

const HRESULT CUI_Logo_Title::Render(const CRenderer::EOrder& eOrder) {
	HRESULT hr = S_OK;
	if (!m_bActiveSelf)
		return hr;
	hr |= (CRenderer::EOrder::UI != eOrder);
	hr |= __super::Render(eOrder);

	hr |= m_pVBRect->Render(m_pShader, m_iPass);
	return hr;
}
