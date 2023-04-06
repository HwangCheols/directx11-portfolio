#include "pch.h"
#include "UI_Loading_Back.h"

CUI_Loading_Back* const CUI_Loading_Back::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CUI_Loading_Back* pInstnace = new CUI_Loading_Back(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CUI_Loading_Back::Clone(void* const& pArg) {
	CUI_Loading_Back* pInstnace = new CUI_Loading_Back(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CUI_Loading_Back::CUI_Loading_Back(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CUI_Image(pDevice, pContext, pNetwork) {
}

CUI_Loading_Back::CUI_Loading_Back(const CUI_Loading_Back& rhs)
	: CUI_Image(rhs) {
}

void CUI_Loading_Back::Delete(void) {
	__super::Delete();
	Safe_Release(m_pLoading);
}

const HRESULT CUI_Loading_Back::Init_Create(void) {
	HRESULT hr = S_OK;
	return hr;
}

const HRESULT CUI_Loading_Back::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Init_Clone(/*L"back"*/);
	__super::Add_Component(static_cast<_ubyte>(EScene::STATIC), Get_TextureKey(L"back").c_str(), TEXT("Texture"), reinterpret_cast<CComponent*&>(m_pTexture));	
	m_pRectTransform->Set_Size((_float)g_iWinSizeX, (_float)g_iWinSizeY);
	m_iPass = 1;

	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	memcpy(&m_iType, pArg, sizeof(_int));
	if (0 > m_iType) {
		m_iDepth = -1;
		SImageDesc sImage;
		// text
		sImage.Reset();
		sImage.pRectTransform = dynamic_cast<CRectTransform*>(pGame_Instance->CreateGet_Component_Clone(static_cast<_ubyte>(EScene::STATIC), TEXT("RectTransform")));
		sImage.pCollider = dynamic_cast<CCollider*>(pGame_Instance->CreateGet_Component_Clone(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB")));
		sImage.pTexture = dynamic_cast<CTexture*>(pGame_Instance->CreateGet_Component_Clone(static_cast<_ubyte>(EScene::STATIC), __super::Get_TextureKey(L"txt_loading").c_str()));
		lstrcpy(sImage.szTexture, __super::Get_TextureKey(L"txt_loading").c_str());
		Set_SizeOfTexture(sImage.pTexture, sImage.pRectTransform);
		sImage.pRectTransform->Set_Edge(CRectTransform::EEdge::RBOT);
		sImage.pRectTransform->Set_State(CRectTransform::EState::POS, _float3(-75.f, -40.f, 0.f));
		sImage.iDepth = 1;
		sImage.iPass = 1;
		sImage.vColor = { 0.6f, 0.6f, 0.6f, 0.8f };
		m_vecImages.emplace_back(sImage);

		m_dTime = 0.;
		m_pLoading = dynamic_cast<CUI_Image*>(pGame_Instance->CreateGet_Object_Clone(L"Loading_Icon_Circle", static_cast<_ubyte>(EScene::LOADING), L"Loading_Icon_Circle"));
	}
	else {
		m_iDepth = 100;
		m_vColor = { 1.f, 1.f, 1.f, 1.f };
	}

	this->Update(0.02);
	return hr;
}

const _ubyte CUI_Loading_Back::Update(const _double& dTimeDelta) {
	__super::Update(dTimeDelta);
	if (0 > m_iType) {
		if (nullptr != m_pLoading && 0 == (_int)m_dTime % 5) {
			if (!m_pLoading->Get_Active() /*&& (0 == rand() % 5)*/)
				m_pLoading->Set_Active(true);
		}
		m_dTime += dTimeDelta;
	}
	else {
		m_vColor.w -= (_float)dTimeDelta * .5f;
		if (0.f >= m_vColor.w) 
			m_vColor.w = 0.f;
	}
	return OBJ_NOEVENT;
}

void CUI_Loading_Back::Update_Edit(void) {
	__super::Update_Edit();
	ImGui::Separator();
	__super::Update_Edit_Images();
}

void CUI_Loading_Back::Late_Update(const _double& dTimeDelta) {
	__super::Late_Update(dTimeDelta);
}

const HRESULT CUI_Loading_Back::Render(const CRenderer::EOrder& eOrder) {
	HRESULT hr = S_OK;
	if (!m_bActiveSelf)
		return hr;
	hr |= __super::Set_RawValue(m_pShader);

	if (0 > m_iType) {
		hr |= m_pShader->Set_RawValue("g_WorldMatrix", &m_pRectTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		hr |= m_pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));
		hr |= m_pTexture->Set_Resource(m_pShader, "g_DiffuseTexture", 0);
		hr |= m_pVBRect->Render(m_pShader, m_iPass);

		for (auto& iter : m_vecImages) {
			if (!iter.bRender || nullptr == iter.pTexture || nullptr == iter.pRectTransform)
				continue;
			if (iter.bCollider)
				iter.pCollider->Render();

			iter.pTexture->Set_Resource(m_pShader, "g_DiffuseTexture", 0);
			m_pShader->Set_RawValue("g_WorldMatrix", &iter.pRectTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
			m_pShader->Set_RawValue("g_vColor", &iter.vColor, sizeof(_float4));
			m_pVBRect->Render(m_pShader, iter.iPass);
		}
	}
	else {
		hr |= m_pShader->Set_RawValue("g_WorldMatrix", &m_pRectTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		hr |= m_pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));
		hr |= m_pTexture->Set_Resource(m_pShader, "g_DiffuseTexture", 0);
		hr |= m_pVBRect->Render(m_pShader, m_iPass);
	}	
	return hr;
}

