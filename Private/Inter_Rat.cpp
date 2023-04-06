#include "pch.h"
#include "Inter_Rat.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "Shadow.h"

CInter_Rat* const CInter_Rat::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Rat* pInstnace = new CInter_Rat(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Rat::Clone(void* const& pArg) {
	CInter_Rat* pInstnace = new CInter_Rat(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Rat::CInter_Rat(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Rat::CInter_Rat(const CInter_Rat& rhs)
	: CObject(rhs) {
}

void CInter_Rat::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}

const HRESULT CInter_Rat::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Rat::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Rat"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(_float(rand() % 4), 0.f, _float(rand() % 4), 1.f));
	m_pCollider->Set_Extents(_float3{ 2.f,2.f,2.f });
	m_eState = ESTATE::IDLE;
	return hr;
}

const _ubyte CInter_Rat::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	//state
	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case ESTATE::IDLE:
			m_pModel->Set_Anim("Rat_REF.ao|Rat_Idle_Standing_RT");
			m_ePreState = ESTATE::IDLE;
			m_iAnimNum = 1;
			m_dTime = 0.;
			break;
		case ESTATE::WALK:
			m_pModel->Set_Anim("Rat_REF.ao|Rat_Run_LT_2_Idle_RT");
			m_ePreState = ESTATE::WALK;
			m_iAnimNum = 5;
			break;
		case ESTATE::RUN:
			m_bLoop = true;
			m_pModel->Set_Anim("Rat_REF.ao|Rat_Run_RT");
			m_ePreState = ESTATE::RUN;
			m_iAnimNum = 6;
			break;
		}
	}
	if (m_eState == ESTATE::WALK) {
		m_pTransform->Translate(m_pTransform->Get_Row(CTransform::ERow::LOOK), 2.f, dTimeDelta);
	}
	else if (m_eState == ESTATE::RUN) {
		_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector vLook = XMVector3Normalize(m_vPlayerLook);
		_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));
		_float3 vSize = m_pTransform->Get_Scale();
		vUp = XMVector3Cross(vLook, vRight);
		m_pTransform->Set_Row(CTransform::ERow::RIGHT, vRight * vSize.x);
		m_pTransform->Set_Row(CTransform::ERow::UP, vUp * vSize.y);
		m_pTransform->Set_Row(CTransform::ERow::LOOK, vLook * vSize.z);
		m_pTransform->Translate(m_vPlayerLook, 2.f, dTimeDelta);
		if (m_dTime >= 5.0) {
			m_eState = ESTATE::IDLE;
		}
		m_dTime += dTimeDelta;
	}
	m_iAnimFinish = m_pModel->Update_Anim(dTimeDelta, m_bLoop);
	m_pCollider->Update(m_pTransform->Get_Matrix());
	//collsion
	if (nullptr == m_plistCamper)
		m_plistCamper = pGame_Instance->Get_Object_ListClone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay"));
	if (nullptr == m_plistCamper)
		return OBJ_NOEVENT;
	for (auto& iter : *m_plistCamper) {
		CCamper_GamePlay* pCamper = dynamic_cast<CCamper_GamePlay*>(iter);
		CCollider* pCamperCollider = dynamic_cast<CCollider*>(pCamper->Get_Component(TEXT("Collider")));
		CTransform* pCamperTransform = dynamic_cast<CTransform*>(pCamper->Get_Component(TEXT("Transform")));
		if (m_eState == ESTATE::IDLE && true == m_pCollider->Is_Collision(pCamperCollider)) {
			m_vPlayerLook = m_pTransform->Get_Row(CTransform::ERow::POS) - pCamperTransform->Get_Row(CTransform::ERow::POS);
			m_eState = ESTATE::RUN;
		}
	}
	if (nullptr == m_pSlasher)
		m_pSlasher = dynamic_cast<CSlasher_Huntress*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Slasher_Huntress")));
	if (nullptr == m_pSlasher)
		return OBJ_NOEVENT;
	CCollider* pSlasherCollider = dynamic_cast<CCollider*>(m_pSlasher->Get_Component(TEXT("Collider")));
	CTransform* pSlasherTransform = dynamic_cast<CTransform*>(m_pSlasher->Get_Component(TEXT("Transform")));
	if (m_eState == ESTATE::IDLE && true == m_pCollider->Is_Collision(pSlasherCollider)) {
		m_vPlayerLook = m_pTransform->Get_Row(CTransform::ERow::POS) - pSlasherTransform->Get_Row(CTransform::ERow::POS);
		m_eState = ESTATE::RUN;
	}

	return OBJ_NOEVENT;
}

void CInter_Rat::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
}

void CInter_Rat::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
}

const HRESULT CInter_Rat::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			if (FAILED(m_pModel->Set_Resource(i, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
				m_pModel->Render(i, m_pShader, 0);
			else
				m_pModel->Render(i, m_pShader, 7);
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		////m_pCollider->Render(pCamera);
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

void CInter_Rat::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_Rat::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>(iter["State"].asInt());
		}
	}
	listRecv.clear();
}

const HRESULT CInter_Rat::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CInter_Rat::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
}
