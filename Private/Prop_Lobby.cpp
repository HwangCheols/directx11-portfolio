#include "pch.h"
#include "Prop_Lobby.h"
#include "Shadow.h"

CProp_Lobby* const CProp_Lobby::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CProp_Lobby* pInstnace = new CProp_Lobby(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CProp_Lobby::Clone(void* const& pArg) {
	CProp_Lobby* pInstnace = new CProp_Lobby(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CProp_Lobby::CProp_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CProp_Lobby::CProp_Lobby(const CProp_Lobby& rhs)
	: CObject(rhs) {
}

void CProp_Lobby::Delete(void) {
	__super::Delete();
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
}

const HRESULT CProp_Lobby::Init_Create(void) {
	return S_OK;
}

const HRESULT CProp_Lobby::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_Model"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	return hr;
}

const _ubyte CProp_Lobby::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pCollider->Update(m_pTransform->Get_Matrix());
	Update_Network();

	return OBJ_NOEVENT;
}

void CProp_Lobby::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	m_pTransform->Update_Edit(0);
	if (ImGui::Button("Model_Create")) {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		string str = m_szModel;
		wstring wstr = { str.begin(),str.end() };
		__super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), wstr.c_str(), wstr.c_str(), reinterpret_cast<CComponent*&>(m_pModel));
	}
	ImGui::DragFloat("Roughness", &m_fRoughness, 0.001f);
	ImGui::DragFloat("Refract", &m_fRefract, 0.001f);
	ImGui::DragFloat("Metalness", &m_fMetalness, 0.001f);
}

void CProp_Lobby::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	if (nullptr != m_pModel) {
		if (true == pGame_Instance->IsIn_Frustum_World(m_pTransform->Get_Row(CTransform::ERow::POS), 15.f)) {
			m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
			//m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
		}
		m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW_BAKE, this);
	}
}

const HRESULT CProp_Lobby::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {

	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_fRoughness", &m_fRoughness, sizeof(_float));
		m_pShader->Set_RawValue("g_fRefract", &m_fRefract, sizeof(_float));
		m_pShader->Set_RawValue("g_fMetalness", &m_fMetalness, sizeof(_float));
		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			if (FAILED(m_pModel->Set_Resource(i, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
				m_pModel->Render(i, m_pShader, 0);
			else
				m_pModel->Render(i, m_pShader, 8);
		}
		break;
	}
	case CRenderer::EOrder::SHADOW_BAKE: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		pShadow->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 3);
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

void CProp_Lobby::Update_Network(const string& pkey) {
	Json::Value root;

}

void CProp_Lobby::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
	}

	listRecv.clear();
}
const HRESULT CProp_Lobby::Save_Data(Json::Value& root) {
	string str = m_szModel;
	root["Model"] = str;

	m_pTransform->Save_Data(root["Transform"]);
	return S_OK;
}

void CProp_Lobby::Load_Data(Json::Value& root) {
	string str = root["Model"].asString();
	wstring wstr{ str.begin(), str.end() };
	strcpy_s(m_szModel, str.c_str());
	__super::Add_Component(static_cast<_ubyte>(EScene::LOBBY), wstr.c_str(), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));

	m_pTransform->Load_Data(root["Transform"]);

	if ("Model_Lobby_Rock" == str)
		m_fRoughness = 0.9f;
	else if ("Model_LobbyTree" == str)
		m_fRoughness = 0.9f;
	else if ("Model_Lobby_Building" == str)
		m_fRoughness = 0.7f;
	else if ("Model_Lobby_LampPost" == str)
		m_fRoughness = 0.8f;
	else if ("Model_tree" == str)
		m_fRoughness = 0.9f;
	else if ("Model_Bush" == str)
		m_fRoughness = 0.659f;
	else if ("Model_grass" == str)
		m_fRoughness = 0.6f;
	else if ("Model_grass2" == str)
		m_fRoughness = 0.6f;
	else if ("Model_TallGrass" == str)
		m_fRoughness = 0.6f;
	else if ("Model_Lobby_Pebbles" == str)
		m_fRoughness = 0.762f;
	else if ("Model_small_Tree" == str)
		m_fRoughness = 0.678f;
	else if ("Model_SitTreeLog" == str)
		m_fRoughness = 0.871f;
	else if ("Model_CampFire" == str) {
		m_fRoughness = 0.590f;
		m_fMetalness = 6.373f;
	}
	else if ("Model_FirePit" == str)
		m_fRoughness = 0.826f;
	else if ("Model_Builiding02" == str)
		m_fRoughness = 0.7f;
	else if ("Model_MilitaryBag" == str)
		m_fRoughness = 0.3f;
	else if ("Model_Westwind" == str)
		m_fRoughness = 0.807f;
	else if ("Model_tree04" == str)
		m_fRoughness = 0.807f;
	else if ("Model_LobbyFence" == str)
		m_fRoughness = 0.7f;
	else if ("Model_WoodWall" == str)
		m_fRoughness = 0.822f;
	else if ("Model_Stone" == str)
		m_fRoughness = 0.74f;
	else if ("Model_SlasherBell" == str)
		m_fRoughness = 0.234f;
}