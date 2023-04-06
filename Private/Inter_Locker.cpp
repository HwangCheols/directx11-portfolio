#include "pch.h"
#include "Inter_Locker.h"
#include "Slasher_Huntress.h"
#include "Camper_GamePlay.h"
#include "Shadow.h"
#include "UI_Play_Icon_Hud.h"

CInter_Locker* const CInter_Locker::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Locker* pInstnace = new CInter_Locker(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Locker::Clone(void* const& pArg) {
	CInter_Locker* pInstnace = new CInter_Locker(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Locker::CInter_Locker(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Locker::CInter_Locker(const CInter_Locker& rhs)
	: CObject(rhs) {
}

void CInter_Locker::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pColTransform);
	Safe_Release(m_pColTransform_Grab);

	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pCollider2);
	Safe_Release(m_pCollider_Grab);

	for (_uint i = 0; i < 3; ++i) {
		Safe_Release(m_pTransformAxe[i]);
		Safe_Release(m_pModelAxe[i]);
	}
	Safe_Release(m_pTransform_Slasher);
	Safe_Release(m_pCollider_Slasher);

	Safe_Release(m_pAudio);
}

const HRESULT CInter_Locker::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Locker::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Locker"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Audio"), TEXT("Audio"), reinterpret_cast<CComponent*&>(m_pAudio));
	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(206.f, 0.f, 171.f, 1.f));
	m_pCollider->Set_Center(_float3{ 0.f, 0.f, -0.5f });
	//Camper
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("ColTransform"), reinterpret_cast<CComponent*&>(m_pColTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("ColTransform_Grab"), reinterpret_cast<CComponent*&>(m_pColTransform_Grab));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider2"), reinterpret_cast<CComponent*&>(m_pCollider2));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_Grab"), reinterpret_cast<CComponent*&>(m_pCollider_Grab));
	m_pColTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(0.f, 0.f, -0.9f, 1.f));
	m_pColTransform_Grab->Set_Row(CTransform::ERow::POS, XMVectorSet(-0.22f, 0.f, 0.f, 1.f));

	//Slasher
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_Slasher"), reinterpret_cast<CComponent*&>(m_pTransform_Slasher));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_Slasher"), reinterpret_cast<CComponent*&>(m_pCollider_Slasher));
	m_pTransform_Slasher->Set_Row(CTransform::ERow::POS, XMVectorSet(0.f, 0.f, -0.91667f, 1.f));
	m_pCollider_Slasher->Set_Extents(_float3{ 0.1f, 0.1f, 0.1f });
	//Weapon
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform2"), reinterpret_cast<CComponent*&>(m_pTransformAxe[0]));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform3"), reinterpret_cast<CComponent*&>(m_pTransformAxe[1]));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform4"), reinterpret_cast<CComponent*&>(m_pTransformAxe[2]));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Locker_Weapon"), TEXT("Model2"), reinterpret_cast<CComponent*&>(m_pModelAxe[0]));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Locker_Weapon"), TEXT("Model3"), reinterpret_cast<CComponent*&>(m_pModelAxe[1]));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Locker_Weapon"), TEXT("Model4"), reinterpret_cast<CComponent*&>(m_pModelAxe[2]));

	for (_uint i = 0; i < 3; ++i)
		m_pModelAxe[i]->Set_Anim("BE_SW01_REF.ao|BE_SW01_REF.ao|BE_SW01_REF.ao|BE_Hatchet_Reload");

	m_pTransformAxe[0]->Set_Row(CTransform::ERow::POS, XMVectorSet(0.03f, 1.35f, 0.15f, 1.f));
	m_pTransformAxe[1]->Set_Row(CTransform::ERow::POS, XMVectorSet(-0.29f, 1.35f, 0.15f, 1.f));
	m_pTransformAxe[2]->Set_Row(CTransform::ERow::POS, XMVectorSet(0.32f, 1.35f, 0.15f, 1.f));

	m_eState = ESTATE::IDLE;

	m_fRootOffsetMatrix = m_pModelAxe[0]->Get_BoneOffsetMatrix("joint_Char");
	m_fRootPrePos = m_pTransformAxe[0]->Get_Float3(CTransform::ERow::POS);

	return hr;
}

const _ubyte CInter_Locker::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	_matrix BoneMatrix = m_pModel->Get_BoneMatrix("joint_AttachKiller");
	_matrix matrix = BoneMatrix * m_pTransform->Get_Matrix();
	_vector v = matrix.r[3];

	Update_Network();

	//Closet01_DoorPeekingLocke
	//_vector fColPos = m_pTransform->Get_Row(CTransform::ERow::POS) + XMVECTOR()
	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case ESTATE::IDLE: {
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet01_DoorIdleClosed");
			m_pModelAxe[0]->Set_Anim("BE_SW01_REF.ao|BE_SW01_REF.ao|BE_SW01_REF.ao|BE_Hatchet_Reload");
			m_pTransformAxe[0]->Set_Row(CTransform::ERow::POS, XMVectorSet(m_fRootPrePos.x, m_fRootPrePos.y, m_fRootPrePos.z, 1.f));
			m_ePreState = ESTATE::IDLE;
			m_bLoop = true;
			m_bRenderAxe = true;
		}
						 break;
		case ESTATE::OPEN_SLASHER: {
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet01_DoorOpenEmpty");
			m_ePreState = ESTATE::OPEN_SLASHER;
			m_bLoop = false;
		}
								 break;
		case ESTATE::TAKEOUT_WEAPON: {
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet01_Hatchet_Reload");
			m_pModelAxe[0]->Set_Anim("BE_SW01_REF.ao|BE_SW01_REF.ao|BE_SW01_REF.ao|BE_Hatchet_Reload.001");
			m_ePreState = ESTATE::TAKEOUT_WEAPON;
			m_bLoop = false;
		}
								   break;
		case ESTATE::TAKEOUT_CAMPER: {
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet01_DoorSlasherPickCamper");
			m_ePreState = ESTATE::TAKEOUT_CAMPER;
			m_bLoop = false;
			//m_bIsInCamper = false;
		}
								   break;
		case ESTATE::HIDE_CAMPER: {
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet01_DoorHide");
			m_ePreState = ESTATE::HIDE_CAMPER;
			m_bLoop = false;
			//m_bIsInCamper = true;
		}
								break;
		case ESTATE::HIDEFAST_CAMPER: {
			m_pAudio->Play_Sound(TEXT("sfx_closet_open_fast_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet01_DoorHideFast");
			m_ePreState = ESTATE::HIDEFAST_CAMPER;
			m_bLoop = false;
			//m_bIsInCamper = true;
			break;
		}
		case ESTATE::COMEOUT_CAMPER: {
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet01_DoorUnhide");
			m_ePreState = ESTATE::COMEOUT_CAMPER;
			m_bLoop = false;
			//m_bIsInCamper = false;
			break;
		}
		case ESTATE::PEEKING_LOCKE: {
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet_DoorFull");
			m_ePreState = ESTATE::PEEKING_LOCKE;
			m_ePreState2 = ESTATE::PEEKING_LOCKE;
			m_bLoop = false;
			//m_bIsInCamper = true;
			break;
		}
		case ESTATE::FULLSWING: {
			m_pModel->Set_Anim("SK_Closet01_REF.ao|Closet01_DoorFullSwing");
			m_ePreState = ESTATE::FULLSWING;
			m_bLoop = false;
			break;
		}
		}
	}

	switch (m_eState) {
	case ESTATE::OPEN_SLASHER:
		if (m_pModel->Get_Anim_KeyTimeAcc(13))
			m_pAudio->Play_Sound(TEXT("sfx_closet_open_fast_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(16))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });

		if (m_pModel->Get_Anim_KeyTimeAcc(35))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(37))
			m_pAudio->Play_Sound(TEXT("sfx_closet_close_fast_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(62))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(69))
			m_pAudio->Play_Sound(TEXT("sfx_closet_close_fast_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		break;
	case ESTATE::TAKEOUT_WEAPON:
		if (m_pModel->Get_Anim_KeyTimeAcc(13))
			m_pAudio->Play_Sound(TEXT("sfx_closet_open_fast_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(16))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(101))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(112))
			m_pAudio->Play_Sound(TEXT("sfx_closet_close_fast_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
	case ESTATE::TAKEOUT_CAMPER:
		break;
	case ESTATE::HIDE_CAMPER:
		if (m_pModel->Get_Anim_KeyTimeAcc(2))
			m_pAudio->Play_Sound(TEXT("sfx_closet_open_slow_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(5))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(39))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(50))
			m_pAudio->Play_Sound(TEXT("sfx_closet_close_slow_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		break;
	case ESTATE::HIDEFAST_CAMPER:
		if (m_pModel->Get_Anim_KeyTimeAcc(2))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(10))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(14))
			m_pAudio->Play_Sound(TEXT("sfx_closet_close_fast_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		break;
	case ESTATE::COMEOUT_CAMPER:
		if (m_pModel->Get_Anim_KeyTimeAcc(8))
			m_pAudio->Play_Sound(TEXT("sfx_closet_open_slow_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(21))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(58))
			m_pAudio->Play_Sound_Rand(TEXT("sfx_closet_squeak"), 4, -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		if (m_pModel->Get_Anim_KeyTimeAcc(63))
			m_pAudio->Play_Sound(TEXT("sfx_closet_close_slow_01"), -1, 1.f, FMOD_3D, m_pTransform, { 0.f, 0.f ,0.f }, { 1.f, 5.f });
		break;
	}

	if (m_eState == ESTATE::TAKEOUT_WEAPON) {
		_vector vPos;
		_float4x4 vSocket;
		_matrix matrix2 = m_pTransformAxe[0]->Get_Matrix();
		matrix2.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		XMStoreFloat4x4(&vSocket, (XMLoadFloat4x4(&m_fRootOffsetMatrix) * m_pModelAxe[0]->Get_RootBoneMatrix("joint_Char")) * matrix2);
		vPos = XMLoadFloat4x4(&vSocket).r[3] + XMLoadFloat3(&m_fRootPrePos);
		m_pTransformAxe[0]->Set_Row(CTransform::ERow::POS, vPos);

		if ((_uint)m_pModelAxe[0]->Get_Anim_TimeAcc() >= 47)
			m_bRenderAxe = false;
	}

	if (!(m_ePreState2 == ESTATE::PEEKING_LOCKE && m_eState == ESTATE::HIDE_CAMPER))
		m_iAnimFinish = m_pModel->Update_Anim(dTimeDelta, m_bLoop);

	if (m_iAnimFinish)
		m_ePreState2 = m_ePreState;

	m_pModelAxe[0]->Update_Anim(dTimeDelta, m_bLoopAxe);
	m_pModelAxe[1]->Update_Anim(0.f, m_bLoopAxe);
	m_pModelAxe[2]->Update_Anim(0.f, m_bLoopAxe);

	m_pCollider->Update(m_pTransform->Get_Matrix());
	m_pCollider_Slasher->Update(m_pTransform_Slasher->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider2->Update(m_pColTransform->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_Grab->Update(m_pColTransform_Grab->Get_Matrix() * m_pTransform->Get_Matrix());

	//collsion
	CInfo* pInfo = CInfo::Get_Instance();
	if (CInfo::ECamper::CHERYL == pInfo->Get_Camper()) {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		//list<CObject*>* plistCamper = pGame_Instance->Get_Object_ListClone(pGame_Instance->Get_Scene(), TEXT("Camper_GamePlay"));
		if (nullptr == m_pCamper)
			m_pCamper = dynamic_cast<CCamper_GamePlay*>(pGame_Instance->Get_Object_Clone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay")));
		if (nullptr == m_pCamper)
			return OBJ_NOEVENT;

		CCollider* pCamperCollider = dynamic_cast<CCollider*>(m_pCamper->Get_Component(TEXT("Collider")));
		if (nullptr == pCamperCollider)
			return OBJ_NOEVENT;

		if (m_pCamper->Get_Control()) {
			if (m_pCollider->Is_Collision(pCamperCollider)) {
				switch (m_eState)
				{
				case ESTATE::IDLE:
					//set ui
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::HIDE), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::LOCKER);
					break;
				case ESTATE::TAKEOUT_CAMPER:
					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::LOCKER);
					break;
				case ESTATE::HIDE_CAMPER:
					//closet_idle
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::HIDE), true);

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::LOCKER);
					break;
				case ESTATE::HIDEFAST_CAMPER:
					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::LOCKER);
					break;
				case ESTATE::COMEOUT_CAMPER:
					// ui off

					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::LOCKER);
					break;
				case ESTATE::FULLSWING:
					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::LOCKER);
					break;
				case ESTATE::PEEKING_LOCKE:
					m_pCamper->Set_ColObject(this);
					m_pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::LOCKER);
					break;
				}
			}
			else {
				if (m_pCamper->Get_ColObject() == this/* && m_eState != ESTATE::HIDE_CAMPER*/) {
					// set ui
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

			if (CSlasher_Huntress::EType::SLASH == m_pSlasher->Get_Type() && (m_eState == ESTATE::IDLE || m_eState == ESTATE::HIDE_CAMPER || m_eState == ESTATE::HIDEFAST_CAMPER)
				&& m_pSlasher->Get_CollisionState() != CSlasher_Huntress::ECollisionState::CAMPER_CRAWL && true == m_pCollider->Is_Collision(pSlasherCollider)) {
				// set ui
				(0 < m_pSlasher->Get_WeaponCount()) ?
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::FIND), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::RIGHT) :
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::FIND), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

				m_pSlasher->Set_ColObject(this);
				m_pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::LOCKER);
			}
			else if (false == m_pCollider->Is_Collision(pSlasherCollider)) {
				if (m_pSlasher->Get_ColObject() == this) {
					// set ui
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::FIND), true);

					m_pSlasher->Set_ColObject(nullptr);
					m_pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::NONE);
				}
			}
		}
	}

	return OBJ_NOEVENT;
}

void CInter_Locker::Update_Edit(void) {
	ImGui::InputText("Model", m_szModel, IM_ARRAYSIZE(m_szModel));
	m_pTransform_Slasher->Update_Edit(4);

	/*m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
	m_pColTransform->Update_Edit(2);*/
	/*m_pTransformAxe[0]->Update_Edit(2);
	m_pTransformAxe[1]->Update_Edit(3);
	m_pTransformAxe[2]->Update_Edit(4);*/
	/*
	m_pCollider2->Update_Edit(3);
	m_pTransform_Slasher->Update_Edit(4);
	m_pCollider_Slasher->Update_Edit(5);*/
	m_pCollider_Grab->Update_Edit(6);
	ImGui::InputInt("ID", &m_iId);
}

void CInter_Locker::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	CInfo* pInfo = CInfo::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
	CInven* pInven = CInven::Get_Instance();
	if (true == pInven->Have_Perk(static_cast<_ubyte>(CInven::INVENTYPE::PERKS_CAMPER), static_cast<_uint>(CInven::PERK_CAMPER::NEWBIE)))
		m_pRenderer->Add_Object(CRenderer::EOrder::PENETRATE, this);
}

const HRESULT CInter_Locker::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Matrix_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < m_pModel->Get_NumMesh(); ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			if (FAILED(m_pModel->Set_Resource(i, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
				m_pModel->Render(i, m_pShader, 6);
			else
				m_pModel->Render(i, m_pShader, 6);
		}

		if (m_bRenderAxe == true) {
			_matrix Matrix = m_pTransformAxe[0]->Get_Matrix() * m_pTransform->Get_Matrix();
			m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
			for (_uint k = 0; k < m_pModel->Get_NumMesh(); ++k) {
				m_pModelAxe[0]->Set_Resource(k, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				if (FAILED(m_pModelAxe[0]->Set_Resource(k, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
					m_pModelAxe[0]->Render(k, m_pShader, 0);
				else
					m_pModelAxe[0]->Render(k, m_pShader, 7);
			}
		}
		/*else {
			_matrix Matrix = m_pTransformAxe[0]->Get_Matrix() * m_pTransform->Get_Matrix();
			m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
			for (_uint k = 0; k < m_pModel->Get_NumMesh(); ++k) {
				m_pModelAxe[0]->Set_Resource(k, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				if (FAILED(m_pModelAxe[0]->Set_Resource(k, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
					m_pModelAxe[0]->Render(k, m_pShader, 0);
				else
					m_pModelAxe[0]->Render(k, m_pShader, 7);
			}
		}*/

		for (_uint j = 1; j < 3; ++j) {
			_matrix Matrix = m_pTransformAxe[j]->Get_Matrix() * m_pTransform->Get_Matrix();
			m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
			for (_uint k = 0; k < m_pModel->Get_NumMesh(); ++k) {
				m_pModelAxe[j]->Set_Resource(k, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				if (FAILED(m_pModelAxe[j]->Set_Resource(k, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
					m_pModelAxe[j]->Render(k, m_pShader, 0);
				else
					m_pModelAxe[j]->Render(k, m_pShader, 7);
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

		_matrix Matrix = m_pTransformAxe[0]->Get_Matrix() * m_pTransform->Get_Matrix();
		m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
		for (_uint k = 0; k < m_pModel->Get_NumMesh(); ++k) {
			m_pModelAxe[0]->Set_Resource(k, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModelAxe[0]->Render(k, m_pShader, 1);
		}

		for (_uint j = 1; j < 3; ++j) {
			_matrix Matrix = m_pTransformAxe[j]->Get_Matrix() * m_pTransform->Get_Matrix();
			m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
			for (_uint k = 0; k < m_pModel->Get_NumMesh(); ++k) {
				m_pModelAxe[j]->Set_Resource(k, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				m_pModelAxe[j]->Render(k, m_pShader, 1);
			}
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
		//m_pCollider->Render(pCamera);
		//m_pCollider_Slasher->Render(pCamera);
		//m_pCollider2->Render(pCamera);
		m_pCollider_Grab->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

const _int& CInter_Locker::Get_CamperID(void) {
	return m_iCamperID;
}

void CInter_Locker::Set_CamperID(const _int& iCamperID) {
	m_iCamperID = iCamperID;
}

void CInter_Locker::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Set_CamperID" == pKey) {
		root["CamperID"] = static_cast<_int>(m_iCamperID);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_Locker::Update_Network(void) {
	list<Json::Value>& root = m_pNetwork->Get_Recv(m_iId);
	for (auto iter = root.begin(); iter != root.end();) {
		string key = (*iter)["Key"].asString();
		if ("Set_State" == key) {
			m_eState = static_cast<ESTATE>((*iter)["State"].asInt());
			iter = root.erase(iter);
		}
		else if ("Set_CamperID" == key) {
			m_iCamperID = static_cast<_int>((*iter)["CamperID"].asInt());
			iter = root.erase(iter);
		}
		else {
			++iter;
		}
	}
}
const HRESULT CInter_Locker::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	root["Id"] = m_iId;
	return S_OK;
}

void CInter_Locker::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
	m_iId = root["Id"].asInt();


}