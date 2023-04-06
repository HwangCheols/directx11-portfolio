#include "pch.h"
#include "Inter_PlankAnim.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "Shadow.h"
#include "UI_Play_Icon_Hud.h"

CInter_PlankAnim* const CInter_PlankAnim::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_PlankAnim* pInstnace = new CInter_PlankAnim(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_PlankAnim::Clone(void* const& pArg) {
	CInter_PlankAnim* pInstnace = new CInter_PlankAnim(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_PlankAnim::CInter_PlankAnim(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_PlankAnim::CInter_PlankAnim(const CInter_PlankAnim& rhs)
	: CObject(rhs) {
}

void CInter_PlankAnim::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pAudio);

	Safe_Release(m_pTransform_Slasher);
	Safe_Release(m_pCollider_Slasher);
	Safe_Release(m_pTransform_F);
	Safe_Release(m_pCollider_F);

	Safe_Release(m_pTransform_B);
	Safe_Release(m_pCollider_B);
	//camper
	Safe_Release(m_pTransform_CamperL);
	Safe_Release(m_pTransform_CamperR);
	Safe_Release(m_pCollider_CamperL);
	Safe_Release(m_pCollider_CamperR);

}

const HRESULT CInter_PlankAnim::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_PlankAnim::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_PlankAnim"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Audio"), TEXT("Audio"), reinterpret_cast<CComponent*&>(m_pAudio));
	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(209.f, 0.f, 179.f, 1.f));
	m_pCollider->Set_Center(_float3{ 0.f, 0.5f, 1.f });
	m_pCollider->Set_Extents(_float3{ 0.74f, 0.5f, 1.2f });

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_Slasher"), reinterpret_cast<CComponent*&>(m_pTransform_Slasher));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_Slasher"), reinterpret_cast<CComponent*&>(m_pCollider_Slasher));
	m_pTransform_Slasher->Set_Row(CTransform::ERow::POS, XMVectorSet(0.f, 0.f, 1.f, 1.f));
	m_pCollider_Slasher->Set_Extents(_float3{ 0.1f, 0.1f, 0.1f });

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_F"), reinterpret_cast<CComponent*&>(m_pTransform_F));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_F"), reinterpret_cast<CComponent*&>(m_pCollider_F));
	m_pTransform_F->Set_Row(CTransform::ERow::POS, XMVectorSet(-1.5f, 0.f, 1.0f, 1.f));
	m_pTransform_F->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	m_pCollider_F->Set_Extents(_float3{ 0.1f, 0.1f, 0.1f });

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_B"), reinterpret_cast<CComponent*&>(m_pTransform_B));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_B"), reinterpret_cast<CComponent*&>(m_pCollider_B));
	m_pTransform_B->Set_Row(CTransform::ERow::POS, XMVectorSet(1.5f, 0.f, 1.0f, 1.f));
	m_pTransform_B->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	m_pCollider_B->Set_Extents(_float3{ 0.1f, 0.1f, 0.1f });

	m_eState = ESTATE::IDLE;

	//collider pos 0,0.8f,0,1 / extent 0.8 0.8 0.8

	//camper
	//Left
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_L"), reinterpret_cast<CComponent*&>(m_pTransform_CamperL));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider_L"), reinterpret_cast<CComponent*&>(m_pCollider_CamperL));
	m_pTransform_CamperL->Set_Row(CTransform::ERow::POS, XMVectorSet(-0.9f, 0.f, 0.8f, 1.f));
	m_pTransform_CamperL->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	//Right
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_R"), reinterpret_cast<CComponent*&>(m_pTransform_CamperR));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider_R"), reinterpret_cast<CComponent*&>(m_pCollider_CamperR));
	m_pTransform_CamperR->Set_Row(CTransform::ERow::POS, XMVectorSet(0.9f, 0.f, 0.8f, 1.f));
	m_pTransform_CamperR->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));

	return hr;
}

const _ubyte CInter_PlankAnim::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	Update_Network();

	if (pGame_Instance->Get_Input_KeyboardKey(DIK_1)) {
		m_eState = ESTATE::FALLDOWN;
	}
	if (pGame_Instance->Get_Input_KeyboardKey(DIK_2)) {
		m_eState = ESTATE::IDLE;
	}

	if (m_ePreState != m_eState) {
		switch (m_eState)
		{
		case ESTATE::IDLE:
			m_pModel->Set_Anim("SK_Plank_01_REF.ao|Plank01_StandIdle");
			m_ePreState = ESTATE::IDLE;
			m_bLoop = true;
			break;
		case ESTATE::FALLDOWN:
			m_pModel->Set_Anim("SK_Plank_01_REF.ao|Plank01_FallOnGround");
			m_ePreState = ESTATE::FALLDOWN;
			m_bLoop = false;
			break;
		case ESTATE::DOWNLOOP:
			m_pModel->Set_Anim("SK_Plank_01_REF.ao|Plank01_Idle_OnGround");
			m_ePreState = ESTATE::DOWNLOOP;
			m_bLoop = true;
			break;
		case ESTATE::PULLUP:
			m_pModel->Set_Anim("SK_Plank_01_REF.ao|Plank01_PullUp_OUT");
			m_ePreState = ESTATE::PULLUP;
			m_bLoop = false;
			break;
		case ESTATE::BREAK:
			m_pModel->Set_Anim("SK_Plank_01_REF.ao|Plank01_DestroyBookShelf");
			m_ePreState = ESTATE::BREAK;
			m_bLoop = false;
			break;
		case ESTATE::BROKEN:
			//pGame_Instance->Create_Object_Clone(TEXT("Inter_PlankBroken"), static_cast<_uint>(EScene::GAMEPLAY), TEXT("Inter_PlankBroken"), &m_pTransform->Get_Row(CTransform::ERow::POS));
			return OBJ_ERASE;
			break;
		}
	}

	switch (m_eState) {
	case ESTATE::FALLDOWN:
		//if (1.f < m_pModel->Get_Anim_TimeAcc()) {
		//	if (false == m_bSound_FallDown2) {
		//		m_bSound_FallDown2 = true;
		//		m_pAudio->Play_Sound_Rand(TEXT("sfx_pallet_killer_hit"), 3, -1, 0.1f, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
		//	}
		//}
		if (8.f < m_pModel->Get_Anim_TimeAcc()) {
			if (false == m_bSound_FallDown) {
				m_bSound_FallDown = true;
				m_pAudio->Play_Sound_Rand(TEXT("sfx_pallet_land"), 3, -1, 0.2f, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
			}
		}
		break;
	case ESTATE::BREAK:
		if (29.f < m_pModel->Get_Anim_TimeAcc()) {
			if (false == m_bSound_Break2) {
				m_bSound_Break2 = true;
				m_pAudio->Play_Sound_Rand(TEXT("sfx_pallet_kick"), 10, -1, 0.1f, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
			}
		}
		if (50.f < m_pModel->Get_Anim_TimeAcc()) {
			if (false == m_bSound_Break) {
				m_bSound_Break = true;
				m_pAudio->Play_Sound_Rand(TEXT("sfx_pallet_break"), 10, -1, 0.3f, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
			}
		}
		break;
	}

	m_iAnimFinish = m_pModel->Update_Anim(dTimeDelta, m_bLoop);


	if (ESTATE::FALLDOWN == m_eState && ANIM_END == m_iAnimFinish) {
		m_eState = ESTATE::DOWNLOOP;
	}

	//collsion
	CInfo* pInfo = CInfo::Get_Instance();
	if (CInfo::ECamper::CHERYL == pInfo->Get_Camper()) {
		if (nullptr == m_pCamper)
			m_pCamper = dynamic_cast<CCamper_GamePlay*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay")));
		if (nullptr == m_pCamper)
			return OBJ_NOEVENT;
		CCollider* pCamperCollider = dynamic_cast<CCollider*>(m_pCamper->Get_Component(TEXT("Collider")));
		if (nullptr == pCamperCollider)
			return OBJ_NOEVENT;
		if (m_pCamper->Get_Control()) {
			if (m_pCollider_CamperL->Is_Collision(pCamperCollider)) {
				switch (m_eState)
				{
				case Client::CInter_PlankAnim::ESTATE::IDLE:
					//넘어뜨리기 UI 필요
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::NURMD), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::PLANK);
					m_pCamper->Set_ColObjDir(0);//Left
					break;
				case Client::CInter_PlankAnim::ESTATE::FALLDOWN:
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::NURMD), true);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::PLANK);
					m_pCamper->Set_ColObjDir(0);//Left
					break;
				case Client::CInter_PlankAnim::ESTATE::DOWNLOOP:
					//넘기/들어가기 UI 필요
					if (m_pCamper->m_pFM->Get_State() != CFM_Camper::EState::_JumpOver_FastL)
						dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::PARKOUR), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::PLANK);
					m_pCamper->Set_ColObjDir(0);//Left
					break;
				}
			}
			else if (m_pCollider_CamperR->Is_Collision(pCamperCollider)) {
				switch (m_eState)
				{
				case Client::CInter_PlankAnim::ESTATE::IDLE:
					//넘어뜨리기 UI 필요
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::NURMD), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::PLANK);
					m_pCamper->Set_ColObjDir(1);//Right
					break;
				case Client::CInter_PlankAnim::ESTATE::FALLDOWN:
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::NURMD), true);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::PLANK);
					m_pCamper->Set_ColObjDir(1);//Right
					break;
				case Client::CInter_PlankAnim::ESTATE::DOWNLOOP:
					//넘기/들어가기 UI 필요
					if (m_pCamper->m_pFM->Get_State() != CFM_Camper::EState::_JumpOver_FastR)
						dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::PARKOUR), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::PLANK);
					m_pCamper->Set_ColObjDir(1);//Right
					break;
				}
			}
			else {
				if (m_pCamper->Get_ColObject() == this) {
					//UI필요X
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(CUI_Play_Icon_Hud::EDiv::CENTER, true);

					m_pCamper->Set_ColObject(nullptr);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::NONE);
				}
			}
		}
	}
	else if (CInfo::ESlasher::HUNTRESS == pInfo->Get_Slasher()) {
		if (nullptr == m_pSlasher)
			m_pSlasher = dynamic_cast<CSlasher_Huntress*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Slasher_Huntress")));
		if (nullptr == m_pSlasher)
			return OBJ_NOEVENT;
		if (true == m_pSlasher->Get_Control()) {
			CCollider* pSlasherCollider = dynamic_cast<CCollider*>(m_pSlasher->Get_Component(TEXT("Collider")));
			if (m_pSlasher->Get_CollisionState() != CSlasher_Huntress::ECollisionState::CAMPER_CRAWL && true == m_pCollider->Is_Collision(pSlasherCollider)) {
				m_pSlasher->Set_ColObject(this);

				_matrix Matrix_Inverse = m_pTransform->Get_Matrix_Inverse();
				_vector vSlasherLocal = XMVector3TransformCoord(dynamic_cast<CTransform*>(m_pSlasher->Get_Component(TEXT("Transform")))->Get_Row(CTransform::ERow::POS), Matrix_Inverse);

				if (0.f > XMVectorGetX(vSlasherLocal))
					m_pSlasher->Set_ColObjDir(0); // front
				else
					m_pSlasher->Set_ColObjDir(1); // Back

				if (m_eState == ESTATE::FALLDOWN && CFM_Slasher::EState::STUN != m_pSlasher->Get_FM_Slasher()->Get_State()) {
					m_pSlasher->Get_FM_Slasher()->Set_State(CFM_Slasher::EState::STUN);
					//pSlasher->Get_FM_Slasher()->Update_Network("Set_State");
				}
				else if ((m_eState == ESTATE::DOWNLOOP || m_eState == ESTATE::BREAK) && CSlasher_Huntress::EType::SLASH == m_pSlasher->Get_Type()) {
					// set ui
					(0 < m_pSlasher->Get_WeaponCount()) ?
						dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::DESTROY), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::RIGHT) :
						dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::DESTROY), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

					m_pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::PLANK);
				}
			}
			else if (false == m_pCollider->Is_Collision(pSlasherCollider)) {
				if (m_pSlasher->Get_ColObject() == this) {
					// set ui
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::DESTROY), true);

					m_pSlasher->Set_ColObject(nullptr);
					m_pSlasher->Set_Stun(false);
					m_pSlasher->Update_Network("Set_Stun");
					m_pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::NONE);
				}
			}
		}
	}

	return OBJ_NOEVENT;
}

void CInter_PlankAnim::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);

	m_pTransform_Slasher->Update_Edit(2);
	m_pCollider_Slasher->Update_Edit(3);
	m_pCollider_F->Update_Edit(4);
	m_pCollider_B->Update_Edit(5);

	m_pCollider_CamperL->Update_Edit(6);
	m_pCollider_CamperR->Update_Edit(7);
	ImGui::InputInt("ID", &m_iId);
}

void CInter_PlankAnim::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	CInfo* pInfo = CInfo::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
	CInven* pInven = CInven::Get_Instance();
	if (true == pInven->Have_Perk(static_cast<_ubyte>(CInven::INVENTYPE::PERKS_CAMPER), static_cast<_uint>(CInven::PERK_CAMPER::NEWBIE)))
		m_pRenderer->Add_Object(CRenderer::EOrder::PENETRATE, this);
}

const HRESULT CInter_PlankAnim::Render(const CRenderer::EOrder& eOrder) {
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
	case CRenderer::EOrder::SHADOW: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		pShadow->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 1);
		}
		break;
	}
	case CRenderer::EOrder::PENETRATE: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);
		CTarget* pDepth = CTarget_Mgr::Get_Instance()->Get_Target(TEXT("Depth"));
		ID3D11ShaderResourceView* pDepthSRV = pDepth->Get_ShaderResourceView();
		m_pShader->Set_Resource("g_DepthTexture", pDepthSRV);
		_float2 fWinSize = _float2{ static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY) };
		m_pShader->Set_RawValue("g_fWinSize", &fWinSize, sizeof(_float2));
		_float4 vPenetrateColor = _float4{ 1.f, 1.f, 0.f, 1.f };
		m_pShader->Set_RawValue("g_vPenetrateColor", &vPenetrateColor, sizeof(_float4));
		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 3);
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		m_pCollider->Render(pCamera);
		m_pCollider_Slasher->Render(pCamera);
		m_pCollider_F->Render(pCamera);
		m_pCollider_B->Render(pCamera);
		m_pCollider_CamperL->Render(pCamera);
		m_pCollider_CamperR->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

void CInter_PlankAnim::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_PlankAnim::Update_Network(void) {
	list<Json::Value>& root = m_pNetwork->Get_Recv(m_iId);
	for (auto iter = root.begin(); iter != root.end();) {
		string key = (*iter)["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>((*iter)["State"].asInt());
			iter = root.erase(iter);
		}
		else {
			++iter;
		}
	}
}
const HRESULT CInter_PlankAnim::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	root["Id"] = m_iId;
	return S_OK;
}

void CInter_PlankAnim::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
	m_iId = root["Id"].asInt();

	m_pCollider->Update(m_pTransform->Get_Matrix());
	m_pCollider_Slasher->Update(m_pTransform_Slasher->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_F->Update(m_pTransform_F->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_B->Update(m_pTransform_B->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_CamperL->Update(m_pTransform_CamperL->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_CamperR->Update(m_pTransform_CamperR->Get_Matrix() * m_pTransform->Get_Matrix());
}