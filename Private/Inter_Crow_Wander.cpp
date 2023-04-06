#include "pch.h"
#include "Inter_Crow_Wander.h"
#include "Camper_GamePlay.h"
#include "Shadow.h"

CInter_Crow_Wander* const CInter_Crow_Wander::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Crow_Wander* pInstnace = new CInter_Crow_Wander(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Crow_Wander::Clone(void* const& pArg) {
	CInter_Crow_Wander* pInstnace = new CInter_Crow_Wander(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Crow_Wander::CInter_Crow_Wander(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Crow_Wander::CInter_Crow_Wander(const CInter_Crow_Wander& rhs)
	: CObject(rhs) {
}

void CInter_Crow_Wander::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
}

const HRESULT CInter_Crow_Wander::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Crow_Wander::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Crow_Wander"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	if (nullptr != pArg) {
		_vector vPos = *reinterpret_cast<_vector*>(pArg) + XMVectorSet(0.f,10.f,0.f,0.f);
		m_pTransform->Set_Row(CTransform::ERow::POS, vPos);
		m_vTargetPos = vPos;
	}
	return hr;
}

const _ubyte CInter_Crow_Wander::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	//state
	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case ESTATE::WANDER:
			m_pModel->Set_Anim("Crow_REF.ao|Crow_FlyAway");
			m_ePreState = ESTATE::WANDER;
			m_bRender = true;
			break;
		case ESTATE::END:
			m_bRender = false;
		}
	}
	if (m_eState == ESTATE::WANDER) {
		m_fAngle += (_float)dTimeDelta * 0.02f;
		m_fCosf = XMVectorGetX(m_vTargetPos) + 4.f * cosf(XMConvertToDegrees(m_fAngle));
		m_fSinf = XMVectorGetZ(m_vTargetPos) + 4.f * sinf(XMConvertToDegrees(m_fAngle));
		if (m_fAngle >= 360.f)
			m_fAngle = 0.f;
		_vector vRot = XMVectorSet(m_fCosf, XMVectorGetY(m_pTransform->Get_Row(CTransform::ERow::POS)), m_fSinf, 1.f);
		m_pTransform->LookAt(vRot);
		m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(m_fCosf, XMVectorGetY(m_pTransform->Get_Row(CTransform::ERow::POS)), m_fSinf, 1.f));
		m_dTime += dTimeDelta;
	}
	if (m_dTime >= 20.f)
		m_eState = ESTATE::END;
	m_iAnimFinish = m_pModel->Update_Anim(dTimeDelta, m_bLoop);
	return OBJ_NOEVENT;
}

void CInter_Crow_Wander::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
}

void CInter_Crow_Wander::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
}

const HRESULT CInter_Crow_Wander::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);
		_matrix Matrix = m_pTransform->Get_Matrix_Transpose();
		m_pShader->Set_RawValue("g_WorldMatrix", &Matrix, sizeof(_float4x4));
		if (m_bRender == true) {
			for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
				m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				if (FAILED(m_pModel->Set_Resource(i, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
					m_pModel->Render(i, m_pShader, 0);
				else
					m_pModel->Render(i, m_pShader, 7);
			}
		}
		break;
	}
	case CRenderer::EOrder::SHADOW: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		pShadow->Set_RawValue(m_pShader);
		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 1);
		}
	}
	}
	return S_OK;
}

void CInter_Crow_Wander::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_Crow_Wander::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>(iter["State"].asInt());
		}
	}
	listRecv.clear();
}

const HRESULT CInter_Crow_Wander::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CInter_Crow_Wander::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
}
