#include "pch.h"
#include "Prop_GamePlay.h"
#include "Shadow.h"

CProp_GamePlay* const CProp_GamePlay::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CProp_GamePlay* pInstnace = new CProp_GamePlay(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CProp_GamePlay::Clone(void* const& pArg) {
	CProp_GamePlay* pInstnace = new CProp_GamePlay(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CProp_GamePlay::CProp_GamePlay(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CProp_GamePlay::CProp_GamePlay(const CProp_GamePlay& rhs)
	: CObject(rhs) {
}

void CProp_GamePlay::Delete(void) {
	__super::Delete();
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader2);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
}

const HRESULT CProp_GamePlay::Init_Create(void) {
	return S_OK;
}

const HRESULT CProp_GamePlay::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_Model"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelInst"), TEXT("Shader2"), reinterpret_cast<CComponent*&>(m_pShader2));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	return hr;
}

const _ubyte CProp_GamePlay::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	m_pCollider->Update(m_pTransform->Get_Matrix());
	Update_Network();

	return OBJ_NOEVENT;
}

void CProp_GamePlay::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	if (m_pModel != nullptr)
		m_pModel->Update_Edit(2);
	if (ImGui::Button("Model_Create")) {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		string str = m_szModel;
		wstring wstr = { str.begin(),str.end() };
		__super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), wstr.c_str(), wstr.c_str(), reinterpret_cast<CComponent*&>(m_pModel));
	}
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
	ImGui::DragFloat("Roughness", &m_fRoughness, 0.001f);
	ImGui::DragFloat("Refract", &m_fRefract, 0.001f);
	ImGui::DragFloat("Metalness", &m_fMetalness, 0.001f);
}

void CProp_GamePlay::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	//m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
	if (nullptr != m_pModel) {
		m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
		m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW_BAKE, this);
	}
}

const HRESULT CProp_GamePlay::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		if (nullptr != m_pModel) {
			if (m_pModel->Get_Inst() == 0) {
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
			}
			else {
				pCamera->Set_RawValue(m_pShader2);
				m_pShader2->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
				for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
					m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader2, "g_DiffuseTexture");
					if (FAILED(m_pModel->Set_Resource(i, aiTextureType_NORMALS, m_pShader2, "g_NormalTexture")))
						m_pModel->Render(i, m_pShader2, 0);
					else
						m_pModel->Render(i, m_pShader2, 2);
				}
			}
		}
		break;
	}
	case CRenderer::EOrder::SHADOW_BAKE: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		if (m_pModel->Get_Inst() == 0) {
			pShadow->Set_RawValue(m_pShader);
			m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
			for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
				m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				m_pModel->Render(i, m_pShader, 3);
			}
		}
		else {
			pShadow->Set_RawValue(m_pShader2);
			m_pShader2->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
			for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
				m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader2, "g_DiffuseTexture");
				m_pModel->Render(i, m_pShader2, 4);
			}
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		m_pCollider->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

void CProp_GamePlay::Update_Network(const string& pkey) {
	Json::Value root;

}

void CProp_GamePlay::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
	}

	listRecv.clear();
}
const HRESULT CProp_GamePlay::Save_Data(Json::Value& root) {
	string str = m_szModel;
	root["Model"] = str;

	m_pTransform->Save_Data(root["Transform"]);
	m_pModel->Save_Data(root["Model0"]);

	//root["Roughness"] = m_fRoughness;
	//root["f0"] = m_fRoughness;

	return S_OK;
}

void CProp_GamePlay::Load_Data(Json::Value& root) {
	string str = root["Model"].asString();
	wstring wstr{ str.begin(), str.end() };
	strcpy_s(m_szModel, str.c_str());
	__super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), wstr.c_str(), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));

	m_pTransform->Load_Data(root["Transform"]);
	m_pModel->Load_Data(root["Model0"]);

	if ("Model_FarmHouse2" == str)
		m_fRoughness = 0.7f;
	else if ("Model_Farm_Fence01" == str)
		m_fRoughness = 0.691f;
	else if ("Model_SM_FAR_Tree_01" == str)
		m_fRoughness = 0.88f;
	else if ("Model_DeadCow" == str)
		m_fRoughness = 0.5f;
	else if ("Model_DeadCow02" == str)
		m_fRoughness = 0.5f;
	else if ("Model_SlgHouse" == str)
		m_fRoughness = 0.675f;
	else if ("Model_SM_FAR_MilkBarrel_01" == str)
		m_fRoughness = 0.212f;
	else if ("Model_SM_FAR_MilkBarrel_05" == str)
		m_fRoughness = 0.212f;
	else if ("Model_WoodPlankStack" == str)
		m_fRoughness = 0.793f;
	else if ("Model_SM_FAR_Tractor_01" == str) {
		m_fRoughness = 0.301f;
		m_fRefract = 0.072f;
	}
	else if ("Model_SM_FAR_PostOak_01" == str)
		m_fRoughness = 0.8f;
	else if ("Model_Farm_Barrel" == str)
		m_fRoughness = 0.238f;
	else if ("Model_HayBale" == str)
		m_fRoughness = 0.6f;
	else if ("Model_MazeWall05" == str)
		m_fRoughness = 0.7f;
	else if ("Model_MazeWall04" == str)
		m_fRoughness = 0.7f;
	else if ("Model_MazeWall03" == str)
		m_fRoughness = 0.7f;
	else if ("Model_MazeWall02" == str)
		m_fRoughness = 0.7f;
	else if ("Model_MazeWall01" == str)
		m_fRoughness = 0.7f;
	else if ("Model_ExitDoorObj" == str)
		m_fRoughness = 0.7f;


	//-------------------------------------
	if ("Model_SM_FAR_PostOak_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 2.f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.3f, 2.f, 0.3f });
	}
	if ("Model_tree04" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 2.f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.4f, 2.f, 0.4f });
	}
	if ("Model_Farm_Barrel" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.5f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.3f, 0.5f, 0.3f });
	}
	if ("Model_SM_Crate03" == str ||
		"Model_SM_Crate04" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.94f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.42f, 0.94f, 0.42f });
	}
	if ("Model_WallHalf" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 1.15f, -1.16f });
		m_pCollider->Set_Extents(_float3{ 0.19f, 1.15f, 1.36f });
	}
	if ("Model_SM_CableReel_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.525f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.78f, 0.525f, 0.78f });
	}
	if ("Model_Dumpster" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.96f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.75f, 0.96f, 1.6f });
	}
	if ("Model_LogPileBig" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.66f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.2f, 0.66f, 0.81f });
	}
	if ("Model_SM_FAR_MilkBarrel_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.68f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.47f, 0.68f, 0.47f });
	}
	if ("Model_SM_FAR_MilkBarrel_03" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.93f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.47f, 0.93f, 0.47f });
	}
	if ("Model_SM_FAR_CornStackCover_02" == str) {
		m_pCollider->Set_Center(_float3{ 0.12f, 1.125f, -0.045f });
		m_pCollider->Set_Extents(_float3{ 1.34f, 1.125f, 1.595f });
	}
	if ("Model_SM_ION_CommonRock_03" == str) {
		m_pCollider->Set_Center(_float3{ 0.12f, 0.86f, -0.18f });
		m_pCollider->Set_Extents(_float3{ 1.69f, 0.86f, 1.67f });
	}
	if ("Model_SM_ASY_RuinsWall_StraightBack_01" == str) {
		m_pCollider->Set_Center(_float3{ -0.07f, 0.98f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.23f, 0.98f, 0.24f });
	}
	if ("Model_SM_ASY_RuinsWall_StraightRight_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.2f, 0.695f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.2f, 0.695f, 0.24f });
	}
	if ("Model_SM_FAR_MilkBarrel_02" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.815f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.47f, 0.815f, 0.47f });
	}
	if ("Model_BrickBlockerPile" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.715f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.87f, 0.715f, 0.59f });
	}
	if ("Model_WoodPlankStack" == str) {
		m_pCollider->Set_Center(_float3{ -0.015f, 0.515f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.605f, 0.515f, 2.105f });
	}
	if ("Model_LogPile01" == str) {
		m_pCollider->Set_Center(_float3{ 0.035f, 0.835f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.035f, 0.835f, 0.55f });
	}
	if ("Model_SM_Crate02" == str) {
		m_pCollider->Set_Center(_float3{ 0.035f, 0.795f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.47f, 0.795f, 0.41f });
	}
	if ("Model_SM_ION_CommonRock_05" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 1.65f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.95f, 1.65f, 2.02f });
	}
	if ("Model_SM_ASY_RuinsWall_Blocker_01" == str) {
		m_pCollider->Set_Center(_float3{ -0.155f, 0.615f, 0.02f });
		m_pCollider->Set_Extents(_float3{ 1.365f, 0.615f, 0.25f });
	}
	if ("Model_SM_Crate07" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.59f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.f, 0.59f, 0.77f });
	}
	if ("Model_Stone2" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.8f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.33f, 0.8f, 0.66f });
	}
	if ("Model_SM_MTR_AbandonedCar_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.83f, 0.155f });
		m_pCollider->Set_Extents(_float3{ 1.03f, 0.83f, 2.505f });
	}
	if ("Model_SM_Crate05" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.635f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.02f, 0.635f, 0.8f });
	}
	if ("Model_LobbyTree" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 2.f, 0.115f });
		m_pCollider->Set_Extents(_float3{ 0.36f, 2.f, 0.365f });
	}
	if ("Model_HayBale" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.38f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.5f, 0.38f, 0.8f });
	}
	if ("Model_Box" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.475f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.5f, 0.475f, 0.5f });
	}
	if ("Model_MazeWall04" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 1.58f, 1.52f });
		m_pCollider->Set_Extents(_float3{ 0.31f, 1.58f, 1.62f });
	}
	if ("Model_SM_ION_Rectangular_Rock_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 1.285f, 0.f });
		m_pCollider->Set_Extents(_float3{ 2.23f, 1.285f, 2.23f });
	}
	if ("Model_SM_FAR_MilkBarrel_04" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.685f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.43f, 0.685f, 0.43f });
	}
	if ("Model_SM_FAR_MilkBarrel_05" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.825f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.46f, 0.825f, 0.46f });
	}
	if ("Model_SM_Fin_ForkLift01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 1.315f, -0.83f });
		m_pCollider->Set_Extents(_float3{ 0.67f, 1.315f, 1.28f });
	}
	if ("Model_SM_FAR_MilkBarrel_06" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.95f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.44f, 0.95f, 0.44f });
	}
	if ("Model_Lobby_LampPost" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 1.76f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.14f, 1.76f, 0.14f });
	}
	if ("Model_SM_FAR_WoodFence_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.93f, 0.5f });
		m_pCollider->Set_Extents(_float3{ 0.16f, 0.93f, 1.43f });
	}
	if ("Model_Lobby_Rock" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.83f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.32f, 0.83f, 0.69f });
	}
	if ("Model_SM_Haiti_Pagoda02" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.785f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.54f, 0.785f, 0.54f });
	}
	if ("Model_SM_FAR_ManureSpreader_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 1.12f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.f, 1.12f, 2.56f });
	}
	if ("Model_SM_ION_CommonRock_04" == str) {
		m_pCollider->Set_Center(_float3{ -0.01f, 1.45f, 0.095f });
		m_pCollider->Set_Extents(_float3{ 1.65f, 1.45f, 1.285f });
	}
	if ("Model_SM_FAR_Transporter_01" == str) {
		m_pCollider->Set_Center(_float3{ -0.025f, 1.f, 0.55f });
		m_pCollider->Set_Extents(_float3{ 1.015f, 1.f, 1.74f });
	}
	if ("Model_SM_FAR_Tree_01" == str) {
		m_pCollider->Set_Center(_float3{ -0.08f, 2.f, -0.15f });
		m_pCollider->Set_Extents(_float3{ 1.07f, 2.f, 1.17f });
	}
	if ("Model_SM_FAR_CowTrough_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.295f, 0.f });
		m_pCollider->Set_Extents(_float3{ 1.09f, 0.295f, 0.32f });
	}
	if ("Model_SM_ION_CommonRock_01" == str) {
		m_pCollider->Set_Center(_float3{ -0.26f, 1.325f, 0.11f });
		m_pCollider->Set_Extents(_float3{ 1.48f, 1.325f, 1.3f });
	}
	if ("Model_SM_FAR_Tractor_01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.735f, 0.11f });
		m_pCollider->Set_Extents(_float3{ 0.995f, 0.735f, 1.49f });
	}
	if ("Model_SM_ION_Pointy_Rock_01" == str) {
		m_pCollider->Set_Center(_float3{ -0.065f, 1.41f, -0.01f });
		m_pCollider->Set_Extents(_float3{ 1.595f, 1.41f, 1.4f });
	}
	if ("Model_SM_Crate01" == str) {
		m_pCollider->Set_Center(_float3{ 0.f, 0.615f, 0.f });
		m_pCollider->Set_Extents(_float3{ 0.53f, 0.615f, 0.41f });
	}
	if ("Model_SM_ION_CommonRock_02" == str) {
		m_pCollider->Set_Center(_float3{ 0.02f, 0.825f, 0.025f });
		m_pCollider->Set_Extents(_float3{ 1.93f, 0.825f, 1.105f });
	}
}