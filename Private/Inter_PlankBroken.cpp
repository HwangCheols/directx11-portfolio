#include "pch.h"
#include "Inter_PlankBroken.h"
#include "Slasher_Huntress.h"

CInter_PlankBroken* const CInter_PlankBroken::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_PlankBroken* pInstnace = new CInter_PlankBroken(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_PlankBroken::Clone(void* const& pArg) {
	CInter_PlankBroken* pInstnace = new CInter_PlankBroken(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_PlankBroken::CInter_PlankBroken(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_PlankBroken::CInter_PlankBroken(const CInter_PlankBroken& rhs)
	: CObject(rhs) {
}

void CInter_PlankBroken::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}

const HRESULT CInter_PlankBroken::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_PlankBroken::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_Model"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_PlankBroken"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	if (nullptr != pArg) {
		_vector vPos = *reinterpret_cast<_vector*>(pArg);
		m_vPos = vPos;
		m_pTransform->Set_Row(CTransform::ERow::POS, vPos + XMVectorSet(0.f,0.5f,0.f,0.f));
	}
	return hr;
}

const _ubyte CInter_PlankBroken::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_dTime += dTimeDelta;
	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorLerp(m_pTransform->Get_Row(CTransform::ERow::POS), m_vPos, 0.1f));
	if (m_dTime >= 1.0)
		return OBJ_ERASE;

	Update_Network();

	return OBJ_NOEVENT;
}

void CInter_PlankBroken::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(2);
}

void CInter_PlankBroken::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	if (true == pGame_Instance->IsIn_Frustum_World(m_pTransform->Get_Row(CTransform::ERow::POS), 15.f))
		m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CInter_PlankBroken::Render(const CRenderer::EOrder& eOrder) {
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
				m_pModel->Render(i, m_pShader, 8);
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		//m_pCollider->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

void CInter_PlankBroken::Update_Network(const string& pKey){
	Json::Value root;
	if ("Set_State" == pKey) {
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_PlankBroken::Update_Network(void){
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
		}
	}
	listRecv.clear();
}
