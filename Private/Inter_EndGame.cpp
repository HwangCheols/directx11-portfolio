#include "pch.h"
#include "Inter_EndGame.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "Effect_Free3.h"

CInter_EndGame* const CInter_EndGame::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_EndGame* pInstnace = new CInter_EndGame(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_EndGame::Clone(void* const& pArg) {
	CInter_EndGame* pInstnace = new CInter_EndGame(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_EndGame::CInter_EndGame(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_EndGame::CInter_EndGame(const CInter_EndGame& rhs)
	: CObject(rhs) {
}

void CInter_EndGame::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}

const HRESULT CInter_EndGame::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_EndGame::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_EndGame"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));

	if (nullptr != pArg) {
		typedef struct sEndGame {
			_int iCamperID;
			_float3 vPos;
		} ENDGAME;
		ENDGAME sEndGame;
		ZeroMemory(&sEndGame, sizeof(ENDGAME));
		sEndGame = *reinterpret_cast<ENDGAME*>(pArg);
		
		/*switch (sEndGame.iIndex) {
		case 0:
			m_iId = 249;
			break;
		case 1:
			m_iId = 248;
			break;
		case 2:
			m_iId = 247;
			break;
		case 3:
			m_iId = 246;
			break;
		}*/

		m_iCamperID = sEndGame.iCamperID;

		CCamper_GamePlay* pCamper = dynamic_cast<CCamper_GamePlay*>(pGame_Instance->Get_Object_CloneID(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Camper_GamePlay"), m_iCamperID));
		CTransform* pCamperTransform = dynamic_cast<CTransform*>(pCamper->Get_Component(TEXT("Transform")));
		
		m_pTransform->Set_Row(CTransform::ERow::RIGHT, pCamperTransform->Get_Row(CTransform::ERow::RIGHT));
		m_pTransform->Set_Row(CTransform::ERow::UP, pCamperTransform->Get_Row(CTransform::ERow::UP));
		m_pTransform->Set_Row(CTransform::ERow::LOOK, pCamperTransform->Get_Row(CTransform::ERow::LOOK));
		m_pTransform->Set_Row(CTransform::ERow::POS, XMLoadFloat3(&sEndGame.vPos));
	}

	m_eState = ESTATE::APPEAR;
	//m_iId = 246, 247, 248, 249

	return hr;
}

const _ubyte CInter_EndGame::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	//Update_Network();
	
	if (m_ePreState != m_eState) {
		CEffect_Free3* pEffectBoom = nullptr;
		CEffect_Free3* pEffectParticle = nullptr;
		CEffect_Free3* pEffectSmoke = nullptr;
		switch (m_eState) {
		case ESTATE::APPEAR:
			m_pModel->Set_Anim("Entity_EndGame_REF.ao|Entity_EndGame_FT");
			m_ePreState = ESTATE::APPEAR;
			m_bLoop = false;
			pEffectBoom = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_EndGameBoom"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_EndGameBoom")));
			pEffectParticle = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_EndGameParticle"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_EndGameParticle")));
			pEffectSmoke = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_EndGameSmoke"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_EndGameSmoke")));
			pEffectBoom->Set_Pos(m_pTransform->Get_Row(CTransform::ERow::POS));
			pEffectParticle->Set_Pos(m_pTransform->Get_Row(CTransform::ERow::POS));
			pEffectSmoke->Set_Pos(m_pTransform->Get_Row(CTransform::ERow::POS));
			break;
		case ESTATE::IDLE:
			m_pModel->Set_Anim("Entity_EndGame_REF.ao|Entity_EndGame_Idle_FT");
			m_ePreState = ESTATE::IDLE;
			m_bLoop = true;
			
			break;
		}
	}

	/*if (ANIM_END == m_pModel->Update_Anim(dTimeDelta, m_bLoop) && m_eState == ESTATE::APPEAR) {
		Set_State(ESTATE::IDLE);
		
		CCamper_GamePlay* pCamper = dynamic_cast<CCamper_GamePlay*>(pGame_Instance->Get_Object_CloneID(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Camper_GamePlay"), m_iCamperID));
		pCamper->Get_FM_Camper()->Set_State(CFM_Camper::EState::Idle);
		pCamper->Get_FM_Camper()->Update_Network("Set_State");
	}*/

	m_pModel->Update_Anim(dTimeDelta, m_bLoop);
	m_pCollider->Update(m_pTransform->Get_Matrix());

	return OBJ_NOEVENT;
}

void CInter_EndGame::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
	ImGui::InputInt("ID", &m_iId);
}

void CInter_EndGame::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	if (true == pGame_Instance->IsIn_Frustum_World(m_pTransform->Get_Row(CTransform::ERow::POS), 15.f))
		m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CInter_EndGame::Render(const CRenderer::EOrder& eOrder) {
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
		m_pCollider->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

void CInter_EndGame::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_EndGame::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>(iter["State"].asInt());
		}
	}
	listRecv.clear();
}
