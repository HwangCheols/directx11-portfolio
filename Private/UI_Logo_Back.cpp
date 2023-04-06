#include "pch.h"
#include "UI_Logo_Back.h"

CUI_Logo_Back* const CUI_Logo_Back::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CUI_Logo_Back* pInstnace = new CUI_Logo_Back(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CUI_Logo_Back::Clone(void* const& pArg) {
	CUI_Logo_Back* pInstnace = new CUI_Logo_Back(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CUI_Logo_Back::CUI_Logo_Back(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CUI_Image(pDevice, pContext, pNetwork) {
}

CUI_Logo_Back::CUI_Logo_Back(const CUI_Logo_Back& rhs)
	: CUI_Image(rhs) {
}

void CUI_Logo_Back::Delete(void) {
	__super::Delete();
}

const HRESULT CUI_Logo_Back::Init_Create(void) {
	HRESULT hr = S_OK;
	return hr;
}

const HRESULT CUI_Logo_Back::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Init_Clone(TEXT("back"));
	this->Update(0.02);
	return hr;
}

const _ubyte CUI_Logo_Back::Update(const _double& dTimeDelta) {
	__super::Update(dTimeDelta);

	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	if (EType::FADE == static_cast<EType>(m_iType)) {
		if (pGame_Instance->Get_Input_KeyboardKeyDown(DIK_SPACE) && !m_bPreActive)
			m_bActiveSelf = true;

		if (m_bActiveSelf) {
			m_vColor.w += (_float)dTimeDelta * 0.5f;
			if (1.f <= m_vColor.w) {
				m_vColor.w = 1.f;
				g_bChangeScene = true;
				return OBJ_ERASE;
			}
		}
	}
	else {
		if (!m_bActiveSelf)
			return OBJ_NOEVENT;
	}

	return OBJ_NOEVENT;
}

void CUI_Logo_Back::Update_Edit(void) {
	__super::Update_Edit();
	ImGui::Separator();
	ImGui::SliderInt("type", &m_iType, 0, (_int)(EType::END) - 1, this->EType_ToString(static_cast<EType>(m_iType)));
}

void CUI_Logo_Back::Late_Update(const _double& dTimeDelta) {
	__super::Late_Update(dTimeDelta);
}

const HRESULT CUI_Logo_Back::Render(const CRenderer::EOrder& eOrder) {
	HRESULT hr = S_OK;
	if (!m_bActiveSelf)
		return hr;
	if (hr == __super::Render(eOrder))
		hr |= m_pVBRect->Render(m_pShader, m_iPass);

	return hr;
}

const HRESULT CUI_Logo_Back::Save_Data(Json::Value& root) {
	__super::Save_Data(root);
	root["Type"] = m_iType;
	return S_OK;
}

void CUI_Logo_Back::Load_Data(Json::Value& root) {
	__super::Load_Data(root);
	m_iType = root["Type"].asInt();
}
