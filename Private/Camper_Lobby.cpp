#include "pch.h"
#include "Camper_Lobby.h"
#include "Camera_Lobby.h"
#include "Setting_CamperSlot.h"

CCamper_Lobby* const CCamper_Lobby::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CCamper_Lobby* pInstance = new CCamper_Lobby(pDevice, pContext, pNetwork);
	if (FAILED(pInstance->Init_Create()))
		Safe_Release(pInstance);
	return pInstance;
}

CObject* const CCamper_Lobby::Clone(void* const& pArg) {
	CCamper_Lobby* pInstance = new CCamper_Lobby(*this);
	if (FAILED(pInstance->Init_Clone(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

CCamper_Lobby::CCamper_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CCamper_Lobby::CCamper_Lobby(const CCamper_Lobby& rhs)
	: CObject(rhs) {
}

void CCamper_Lobby::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel_Cheryl);
	Safe_Release(m_pModel_Bill);
}

const HRESULT CCamper_Lobby::Init_Create(void) {
	return S_OK;
}

const HRESULT CCamper_Lobby::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), TEXT("Model_Lobby_Cheryl"), TEXT("Model_Cheryl"), reinterpret_cast<CComponent*&>(m_pModel_Cheryl));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), TEXT("Model_Lobby_Cheryl"), TEXT("Model_Bill"), reinterpret_cast<CComponent*&>(m_pModel_Bill));
	return hr;
}

const _ubyte CCamper_Lobby::Update(const _double& dTimeDelta) {
	CCamera_Lobby* pCamera = dynamic_cast<CCamera_Lobby*>(CCamera::Get_Camera(static_cast<_uint>(0)));

	if (true == pCamera->Get_CameraCamperRender()) {
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
	}
	else {
		if (m_fDissolveLength < -0.1f) {
			m_fDissolveLength += 3.f * static_cast<_float>(dTimeDelta);
			if (m_fDissolveLength > -0.1f)
				m_fDissolveLength = -0.1f;
		}
		else if (m_fDissolveLength > -0.1f) {
			m_fDissolveLength -= 3.f * static_cast<_float>(dTimeDelta);
			if (m_fDissolveLength < -0.1f)
				m_fDissolveLength = -0.1f;
		}
	}


	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pModel_Cheryl->Update_Anim(dTimeDelta, true);
	m_pModel_Bill->Update_Anim(dTimeDelta, true);

	Update_Network();
	return OBJ_NOEVENT;
}

void CCamper_Lobby::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
}

void CCamper_Lobby::Late_Update(const _double& dTimeDelta) {
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::GLOW, this);
}

const HRESULT CCamper_Lobby::Render(const CRenderer::EOrder& eOrder) {
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
		for (_uint i = 0; i < m_pModel_Cheryl->Get_NumMesh(); ++i) {
			m_pModel_Cheryl->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel_Cheryl->Render(i, m_pShader, 2);
		}
		break;
	}
										 break;
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
		for (_uint i = 0; i < m_pModel_Cheryl->Get_NumMesh(); ++i) {
			m_pModel_Cheryl->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel_Cheryl->Render(i, m_pShader, 4);
		}
		break;
	}
										 break;
	}
	return S_OK;
}

void CCamper_Lobby::Set_Dissolve(const _float& m_fGoal) {
	m_fGoalDissolveLength = m_fGoal;
	Update_Network("Set_Dissolve");
}

void CCamper_Lobby::Set_Camper(const ECamper& eCamper) {
	m_eCamper = eCamper;
	if (m_bControl)
		Update_Network("Set_Camper");
}

void CCamper_Lobby::Set_MyCamperSlot(const _uint& iSlot) {
	m_iMyCamperSlot = iSlot;
	Update_Network("Set_MyCamperSlot");
}

void CCamper_Lobby::Create_Effect(void) {
	if (m_bControl)
		Update_Network("Create_Effect");
}

const _float& CCamper_Lobby::Get_GoalDissolveLength(void) {
	return m_fGoalDissolveLength;
}

void CCamper_Lobby::Update_Network(const string& pKey) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	Json::Value root;
	if ("Add_Camper" == pKey) {
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Pop_Camper" == pKey) {
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Set_Dissolve" == pKey) {
		root["Dissolve"] = m_fGoalDissolveLength;
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Create_Effect" == pKey) {
		CObject* pEffect = pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_LobbyCamperSmoke"), static_cast<_ubyte>(EScene::LOBBY), TEXT("Effect_LobbyCamperSmoke"));
		CTransform* pEffectTransform = dynamic_cast<CTransform*>(pEffect->Get_Component(TEXT("Transform")));
		pEffectTransform->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS));
		pEffectTransform->Set_Scale(_float3{ 1.f, 1.f, 1.f });
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Set_Camper" == pKey) {
		root["Camper"] = static_cast<_int>(m_eCamper);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Set_MyCamperSlot" == pKey) {
		root["MyCamperSlot"] = m_iMyCamperSlot;
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
};

void CCamper_Lobby::Update_Network(void) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	list<Json::Value>& iter = m_pNetwork->Get_Recv(m_iId);
	for (auto& root : iter) {
		string Key = root["Key"].asString();
		if ("Add_Camper" == Key) {
			CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
			CInfo* pInfo = CInfo::Get_Instance();
			//Camper
			CCamper_Lobby** pCamperSlot = pInfo->Get_pCamperSlot();

			_uint iCamperSlot = 0;
			for (iCamperSlot = 0; iCamperSlot < 4; ++iCamperSlot) {
				if (pCamperSlot[iCamperSlot] == nullptr)
					break;
			}
			pInfo->Set_pCamperSlot(iCamperSlot, this);

			CSetting_CamperSlot* pSetting = dynamic_cast<CSetting_CamperSlot*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Setting_CamperSlot"), iCamperSlot));
			CTransform* pSettingTransform = dynamic_cast<CTransform*>(pSetting->Get_Component(TEXT("Transform")));
			m_pTransform->Set_Matrix(pSettingTransform->Get_Matrix());
			pSetting->Set_EffectRender(false);
		}
		if ("Pop_Camper" == Key) {
			CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
			CInfo* pInfo = CInfo::Get_Instance();
			//Camper
			CCamper_Lobby** pCamperSlot = pInfo->Get_pCamperSlot();

			_uint iCamperSlot = 0;
			for (iCamperSlot = 0; iCamperSlot < 4; ++iCamperSlot) {
				if (pCamperSlot[iCamperSlot] == this)
					break;
			}
			pInfo->Set_pCamperSlot(iCamperSlot, nullptr);
			_uint iMyCamperSlot = pInfo->Get_MyCamperSlot();
			if (iMyCamperSlot > m_iMyCamperSlot) {
				pInfo->Set_MyCamperSlot(--iMyCamperSlot);
			}

			CSetting_CamperSlot* pSetting = dynamic_cast<CSetting_CamperSlot*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Setting_CamperSlot"), iCamperSlot));
			CTransform* pSettingTransform = dynamic_cast<CTransform*>(pSetting->Get_Component(TEXT("Transform")));
			m_pTransform->Set_Matrix(pSettingTransform->Get_Matrix());
			pSetting->Set_EffectRender(true);
		}
		if ("Set_Dissolve" == Key) {
			m_fGoalDissolveLength = root["Dissolve"].asFloat();
		}
		if ("Create_Effect" == Key) {
			CObject* pEffect = pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_LobbyCamperSmoke"), static_cast<_ubyte>(EScene::LOBBY), TEXT("Effect_LobbyCamperSmoke"));
			CTransform* pEffectTransform = dynamic_cast<CTransform*>(pEffect->Get_Component(TEXT("Transform")));
			pEffectTransform->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS) - XMVectorSet(0.f, 0.8f, 0.f, 0.f));
			pEffectTransform->Set_Scale(_float3{ 0.7f, 0.5f, 0.7f });
		}
		if ("Set_Camper" == Key) {
			m_eCamper = static_cast<ECamper>(root["Camper"].asInt());
		}
		if ("Set_MyCamperSlot" == Key) {
			m_iMyCamperSlot = root["MyCamperSlot"].asInt();
		}
	}
	iter.clear();
};