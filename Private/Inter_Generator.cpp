#include "pch.h"
#include "Inter_Generator.h"
#include "Camper_GamePlay.h"
#include "Slasher_Huntress.h"
#include "Shadow.h"
#include "Effect_Free3.h"
#include "UI_Play_Icon_Hud.h"
#include "UI_Play_Gauge_Inter.h"

CInter_Generator* const CInter_Generator::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Generator* pInstnace = new CInter_Generator(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Generator::Clone(void* const& pArg) {
	CInter_Generator* pInstnace = new CInter_Generator(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Generator::CInter_Generator(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Generator::CInter_Generator(const CInter_Generator& rhs)
	: CObject(rhs) {
}

void CInter_Generator::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pTransform2);
	Safe_Release(m_pTransform_F);
	Safe_Release(m_pTransform_B);
	Safe_Release(m_pTransform_L);
	Safe_Release(m_pTransform_R);

	Safe_Release(m_pCollider_F);
	Safe_Release(m_pCollider_B);
	Safe_Release(m_pCollider_L);
	Safe_Release(m_pCollider_R);

	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);


	Safe_Release(m_pTransform_S_C);
	Safe_Release(m_pTransform_S_F);
	Safe_Release(m_pTransform_S_B);
	Safe_Release(m_pTransform_S_L);
	Safe_Release(m_pTransform_S_R);

	Safe_Release(m_pCollider_S_C);
	Safe_Release(m_pCollider_S_F);
	Safe_Release(m_pCollider_S_B);
	Safe_Release(m_pCollider_S_L);
	Safe_Release(m_pCollider_S_R);

	Safe_Release(m_pTransform_GodRay);
	Safe_Release(m_pCollider_GodRay);

	Safe_Release(m_pTransform_Entity);
	Safe_Release(m_pModel_Entity);

	Safe_Release(m_pAudio);
	//Safe_Release(m_pEffectLockParticle);
	//Safe_Release(m_pEffectLockSmoke);
}

const HRESULT CInter_Generator::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Generator::Init_Clone(void* const& pArg) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform2"), reinterpret_cast<CComponent*&>(m_pTransform2));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Shader_ModelAnim"), TEXT("Shader"), reinterpret_cast<CComponent*&>(m_pShader));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Inter_Generator02"), TEXT("Model"), reinterpret_cast<CComponent*&>(m_pModel));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Audio"), TEXT("Audio"), reinterpret_cast<CComponent*&>(m_pAudio));

	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(203.f, 0.f, 179.f, 1.f));
	m_pCollider->Set_Extents(_float3{ 1.1f, 1.1f ,1.1f });
	//Lamp pos restart
	m_pTransform2->Set_Row(CTransform::ERow::POS, XMVectorSet(0.01f, -0.09f, -1.39f, 1.f));
	m_pTransform2->Rotation(m_pTransform2->Get_Row(CTransform::ERow::RIGHT), XMConvertToRadians(90.f));

	//camper
	// front
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_F"), reinterpret_cast<CComponent*&>(m_pTransform_F));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_F"), reinterpret_cast<CComponent*&>(m_pCollider_F));
	m_pTransform_F->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pTransform_F->Set_Row(CTransform::ERow::POS, XMVectorSet(0.f, 0.f, -0.5f, 1.f));
	m_pCollider_F->Set_Extents(_float3{ 0.1f, 0.1f ,0.1f });
	//Back
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_B"), reinterpret_cast<CComponent*&>(m_pTransform_B));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_B"), reinterpret_cast<CComponent*&>(m_pCollider_B));
	m_pTransform_B->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pTransform_B->Set_Row(CTransform::ERow::POS, XMVectorSet(0.f, 0.f, 0.9f, 1.f));
	m_pTransform_B->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_pCollider_B->Set_Extents(_float3{ 0.1f, 0.1f ,0.1f });
	//Left
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_L"), reinterpret_cast<CComponent*&>(m_pTransform_L));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_L"), reinterpret_cast<CComponent*&>(m_pCollider_L));
	m_pTransform_L->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pTransform_L->Set_Row(CTransform::ERow::POS, XMVectorSet(-0.5f, 0.f, 0.f, 1.f));
	m_pTransform_L->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	m_pCollider_L->Set_Extents(_float3{ 0.1f, 0.1f ,0.1f });
	//Right
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_R"), reinterpret_cast<CComponent*&>(m_pTransform_R));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_R"), reinterpret_cast<CComponent*&>(m_pCollider_R));
	m_pTransform_R->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pTransform_R->Set_Row(CTransform::ERow::POS, XMVectorSet(-0.1f, 0.f, 0.f, 1.f));
	m_pTransform_R->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	m_pCollider_R->Set_Extents(_float3{ 0.5f, 0.1f ,0.1f });

	// Slasher
	// center
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_S_C"), reinterpret_cast<CComponent*&>(m_pTransform_S_C));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_S_C"), reinterpret_cast<CComponent*&>(m_pCollider_S_C));
	// front
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_S_F"), reinterpret_cast<CComponent*&>(m_pTransform_S_F));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_S_F"), reinterpret_cast<CComponent*&>(m_pCollider_S_F));
	m_pTransform_S_F->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pTransform_S_F->Set_Row(CTransform::ERow::POS, XMVectorSet(0.f, 0.f, -1.4f, 1.f));
	m_pCollider_S_F->Set_Extents(_float3{ 0.1f, 0.1f ,0.1f });
	//Back
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_S_B"), reinterpret_cast<CComponent*&>(m_pTransform_S_B));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_S_B"), reinterpret_cast<CComponent*&>(m_pCollider_S_B));
	m_pTransform_S_B->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pTransform_S_B->Set_Row(CTransform::ERow::POS, XMVectorSet(0.f, 0.f, 1.4f, 1.f));
	m_pTransform_S_B->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_pCollider_S_B->Set_Extents(_float3{ 0.1f, 0.1f ,0.1f });
	//Left
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_S_L"), reinterpret_cast<CComponent*&>(m_pTransform_S_L));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_S_L"), reinterpret_cast<CComponent*&>(m_pCollider_S_L));
	m_pTransform_S_L->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pTransform_S_L->Set_Row(CTransform::ERow::POS, XMVectorSet(-1.4f, 0.f, 0.f, 1.f));
	m_pTransform_S_L->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	m_pCollider_S_L->Set_Extents(_float3{ 0.1f, 0.1f ,0.1f });
	//Right
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_S_R"), reinterpret_cast<CComponent*&>(m_pTransform_S_R));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_S_R"), reinterpret_cast<CComponent*&>(m_pCollider_S_R));
	m_pTransform_S_R->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pTransform_S_R->Set_Row(CTransform::ERow::POS, XMVectorSet(1.4f, 0.f, 0.f, 1.f));
	m_pTransform_S_R->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	m_pCollider_S_R->Set_Extents(_float3{ 0.1f, 0.1f ,0.1f });

	//GodRay
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_GodRay"), reinterpret_cast<CComponent*&>(m_pTransform_GodRay));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_AABB"), TEXT("Collider_GodRay"), reinterpret_cast<CComponent*&>(m_pCollider_GodRay));
	m_pTransform_GodRay->Set_Row(CTransform::ERow::POS, XMVectorSet(0.f, 3.1f, -0.71f, 1.f));
	m_pCollider_GodRay->Set_Extents(_float3{ 0.1f, 0.1f ,0.1f });

	//Entity - hwang
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_Entity"), reinterpret_cast<CComponent*&>(m_pTransform_Entity));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Model_Entity_Generator"), TEXT("Model_Entity"), reinterpret_cast<CComponent*&>(m_pModel_Entity));

	//Light
	CLight::SDesc sLight;
	sLight.eType = CLight::EType::SPOT;
	sLight.vDiffuse = _float4{ 2.f, 2.f, 2.f, 1.f };
	sLight.vAmbient = _float4{ 2.f, 2.f, 2.f, 1.f };
	sLight.vSpecular = _float4{ 3.f, 3.f, 3.f, 1.f };
	sLight.fRange = 6.f;
	sLight.fAngle = _float2{ 0.5f, 1.5f };
	m_pLight = dynamic_cast<CLight*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Light_Free_NoSave"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Light_Free_NoSave"), &sLight));
	m_pLight->Set_OnOff(false);
	m_eState = EState::OFF;

	m_pTarget_Depth = CTarget_Mgr::Get_Instance()->Get_Target(TEXT("Depth"));
	return hr;
}

const _ubyte CInter_Generator::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	Update_Network();

	if (m_eState != EState::LOCK_IN && m_eState != EState::LOCK_IDLE && m_eState != EState::LOCK_END && m_eState != EState::CRASH) {
		if (m_fGauge < 1.f)
			m_eState = EState::OFF;
		else if (m_fGauge < 25.f)
			m_eState = EState::ONE;
		else if (m_fGauge < 50.f)
			m_eState = EState::TWO;
		else if (m_fGauge < 75.f)
			m_eState = EState::THREE;
		else if (m_fGauge < 100.f)
			m_eState = EState::FOUR;
		else {
			m_eState = EState::ON;
			// gauge off
			dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"))->End_Gauge(CUI_Play_Gauge_Inter::EInter::GEN);
		}
		m_pModel->Update_Anim(dTimeDelta, true);
	}


	if (m_ePreState != m_eState) {
		switch (m_eState) {
		case EState::OFF: {
			Set_State(EState::OFF);
			m_ePreState = EState::OFF;
			if (m_pEffectCrash != nullptr)
				m_pEffectCrash->Set_RepeatEnd();

			break;
		}
		case EState::ONE: {
			Set_State(EState::ONE);
			m_ePreState = EState::ONE;
			if (m_pEffectCrash != nullptr)
				m_pEffectCrash->Set_RepeatEnd();
			break;
		}
		case EState::TWO: {
			Set_State(EState::TWO);
			m_ePreState = EState::TWO;
			if (m_pEffectCrash != nullptr)
				m_pEffectCrash->Set_RepeatEnd();
			break;
		}
		case EState::THREE: {
			Set_State(EState::THREE);
			m_ePreState = EState::THREE;
			if (m_pEffectCrash != nullptr)
				m_pEffectCrash->Set_RepeatEnd();
			break;
		}
		case EState::ON: {
			Set_State(EState::ON);
			m_ePreState = EState::ON;
			CEffect_Free3* pEffectClear = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_GeneratorClearSpark"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_GeneratorClearSpark")));
			CTransform* pEffect = dynamic_cast<CTransform*>(pEffectClear->Get_Component(TEXT("Transform")));
			pEffect->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS));
			break;
		}
		case EState::CRASH: {
			if (ANIM_END == m_pModel->Update_Anim(dTimeDelta, false)) {
				if (m_fGauge < 1.f)
					Set_State(EState::OFF);
				else if (m_fGauge < 25.f)
					Set_State(EState::ONE);
				else if (m_fGauge < 50.f)
					Set_State(EState::TWO);
				else if (m_fGauge < 75.f)
					Set_State(EState::THREE);
				else if (m_fGauge < 100.f)
					Set_State(EState::FOUR);
				else
					Set_State(EState::ON);
			}

			break;
		}
		case EState::LOCK_IN: {
			m_bIsLoop = false;
			m_pEffectLockParticle = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_GeneratorLockParticle"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_GeneratorLockParticle")));
			m_pEffectLockSmoke = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_GeneratorLockSmoke"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_GeneratorLockSmoke")));
			CTransform* pParticleTransform = dynamic_cast<CTransform*>(m_pEffectLockParticle->Get_Component(TEXT("Transform")));
			CTransform* pSmokeTransform = dynamic_cast<CTransform*>(m_pEffectLockSmoke->Get_Component(TEXT("Transform")));
			pParticleTransform->Set_Matrix(m_pTransform->Get_Matrix());
			pSmokeTransform->Set_Matrix(m_pTransform->Get_Matrix());
			pSmokeTransform->Set_Row(CTransform::ERow::POS, pSmokeTransform->Get_Row(CTransform::ERow::POS) + XMVectorSet(0.f, 1.f, 0.f, 0.f));
			m_ePreState = m_eState;
			break;
		}
		case EState::LOCK_IDLE:
			Set_State(EState::LOCK_IDLE);
			m_bIsLoop = true;
			m_ePreState = m_eState;
			break;

		case EState::LOCK_END:
			Set_State(EState::LOCK_END);
			m_pEffectLockParticle->Set_RepeatEnd();
			m_pEffectLockSmoke->Set_RepeatEnd();
			m_bIsLoop = false;
			m_ePreState = m_eState;
			break;
		}
	}

	if (m_eState == EState::LOCK_IN) {
		if (ANIM_END == m_pModel_Entity->Update_Anim(dTimeDelta, m_bIsLoop))
			m_eState = EState::LOCK_IDLE;
	}
	else if (m_eState == EState::LOCK_IDLE) {
		m_pModel_Entity->Update_Anim(dTimeDelta, m_bIsLoop);
	}
	else if (m_eState == EState::LOCK_END) {
		if (ANIM_END == m_pModel_Entity->Update_Anim(dTimeDelta, m_bIsLoop)) {
			m_eState = m_ePreLockState;
			m_bIsLock = false;
		}
	}

	_float fVolume = (m_fGauge / 100.f) * 0.1f;
	switch (m_eState) {
	case EState::ONE:
		m_fSound_One += static_cast<_float>(dTimeDelta);
		if (0.35f < m_fSound_One) {
			m_fSound_One = 0.f;

			m_pAudio->Play_Sound_Rand(TEXT("sfx_generator_piston_compress"), 4, -1, fVolume, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
			m_pAudio->Play_Sound_Rand(TEXT("sfx_generator_piston"), 4, -1, fVolume, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
		}
		break;
	case EState::TWO:
		m_fSound_One += static_cast<_float>(dTimeDelta);
		if (0.34f < m_fSound_One) {
			m_fSound_One = 0.f;
			m_pAudio->Play_Sound_Rand(TEXT("sfx_generator_piston_compress"), 4, -1, fVolume, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
			m_pAudio->Play_Sound_Rand(TEXT("sfx_generator_piston"), 4, -1, fVolume, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
		}
		break;
	case EState::THREE:
		m_fSound_One += static_cast<_float>(dTimeDelta);
		if (0.33f < m_fSound_One) {
			m_fSound_One = 0.f;
			m_pAudio->Play_Sound_Rand(TEXT("sfx_generator_piston_compress"), 4, -1, fVolume, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
			m_pAudio->Play_Sound_Rand(TEXT("sfx_generator_piston"), 4, -1, fVolume, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
		}
		break;
	case EState::FOUR:
		m_fSound_One += static_cast<_float>(dTimeDelta);
		if (0.32f < m_fSound_One) {
			m_fSound_One = 0.f;
			m_pAudio->Play_Sound_Rand(TEXT("sfx_generator_piston_compress"), 4, -1, fVolume, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
			m_pAudio->Play_Sound_Rand(TEXT("sfx_generator_piston"), 4, -1, fVolume, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
		}
		break;
	case EState::ON:
		m_pAudio->Set_3DAttribute(0, m_pTransform);
		break;
	}

	//collsion
	CInfo* pInfo = CInfo::Get_Instance();
	if (CInfo::ECamper::CHERYL == pInfo->Get_Camper()) {
		if (nullptr == m_plistCamper)
			m_plistCamper = pGame_Instance->Get_Object_ListClone(static_cast<_int>(EScene::GAMEPLAY), TEXT("Camper_GamePlay"));
		if (nullptr == m_plistCamper)
			return OBJ_NOEVENT;
		for (auto& iter : *m_plistCamper) {
			CCamper_GamePlay* pCamper = dynamic_cast<CCamper_GamePlay*>(iter);
			CCollider* pCamperCollider = dynamic_cast<CCollider*>(pCamper->Get_Component(TEXT("Collider")));
			if (!pCamper->Get_Control())
				continue;
			if (EState::ON != m_eState && EState::LOCK_IN != m_eState && EState::LOCK_IDLE != m_eState && EState::LOCK_END != m_eState
				&& m_pCollider->Is_Collision(pCamperCollider)) {
				// set ui 
				CUI_Play_Icon_Hud* pHud = dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"));
				CUI_Play_Gauge_Inter* pGauge = dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"));

				pHud->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::REPAIR), CUI_Play_Icon_Hud::EKey::MOUSE_L, CUI_Play_Icon_Hud::EDiv::CENTER);
				pGauge->Set_Render(CUI_Play_Gauge_Inter::EInter::GEN, true, false);

				if (CFM_Camper::EState::Generator_IdleB == pCamper->m_pFM->Get_State() ||
					CFM_Camper::EState::Generator_IdleF == pCamper->m_pFM->Get_State() ||
					CFM_Camper::EState::Generator_IdleL == pCamper->m_pFM->Get_State() ||
					CFM_Camper::EState::Generator_IdleR == pCamper->m_pFM->Get_State()) {
					pHud->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::CANCLE), CUI_Play_Icon_Hud::EKey::MOUSE_L, CUI_Play_Icon_Hud::EDiv::CENTER);
					pGauge->Set_Inter(CUI_Play_Gauge_Inter::EInter::GEN, true);
				}
				else {
					if (CFM_Camper::EState::Generator_IdleB_Faile == pCamper->m_pFM->Get_State() ||
						CFM_Camper::EState::Generator_IdleF_Faile == pCamper->m_pFM->Get_State() ||
						CFM_Camper::EState::Generator_IdleL_Faile == pCamper->m_pFM->Get_State() ||
						CFM_Camper::EState::Generator_IdleR_Faile == pCamper->m_pFM->Get_State()) {
						pHud->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::REPAIR), CUI_Play_Icon_Hud::EKey::MOUSE_L, CUI_Play_Icon_Hud::EDiv::CENTER);
						pGauge->Set_Inter(CUI_Play_Gauge_Inter::EInter::GEN, false);
					}
					else {

					}
				}
				// set gauge
				pGauge->Set_Gauge(CUI_Play_Gauge_Inter::EInter::GEN, m_fGauge);

				pCamper->Set_ColObject(this);
				pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::GENERATOR);


				CTransform* pCamperTransform = dynamic_cast<CTransform*>(pCamper->Get_Component(TEXT("Transform")));
				_matrix MatrixInverse = m_pTransform->Get_Matrix_Inverse();
				_float3 vLocal; XMStoreFloat3(&vLocal, XMVector3TransformCoord(pCamperTransform->Get_Row(CTransform::ERow::POS), MatrixInverse));
				if (abs(vLocal.x) > abs(vLocal.z)) {
					if (vLocal.x > 0)
						pCamper->Set_ColObjDir(3);//Right
					else
						pCamper->Set_ColObjDir(2);//Left
				}
				else {
					if (vLocal.z > 0)
						pCamper->Set_ColObjDir(1);//Back
					else
						pCamper->Set_ColObjDir(0);//front
				}
			}
			else if (false == m_pCollider->Is_Collision(pCamperCollider)) {
				if (pCamper->Get_ColObject() == this) {
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(CUI_Play_Icon_Hud::EDiv::CENTER, true);
					dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"))->Set_Render(CUI_Play_Gauge_Inter::EInter::GEN, false, false);

					pCamper->Set_ColObject(nullptr);
					pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::NONE);
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

			if (CSlasher_Huntress::EType::SLASH == m_pSlasher->Get_Type() && (m_eState == EState::ONE || m_eState == EState::TWO || m_eState == EState::THREE)
				&& m_pSlasher->Get_CollisionState() != CSlasher_Huntress::ECollisionState::CAMPER_CRAWL
				&& true == m_pCollider->Is_Collision(pSlasherCollider)) {
				m_pSlasher->Set_ColObject(this);
				m_pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::GENERATOR);

				CUI_Play_Icon_Hud* pHud = dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"));
				CUI_Play_Gauge_Inter* pGauge = dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"));
				// Generator Sonsang UI On 
				if (0.f < m_fGauge) {
					(0 < m_pSlasher->Get_WeaponCount()) ? pHud->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::SONSANG), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::RIGHT) :
						pHud->Set_HudOn(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::SONSANG), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);
					pGauge->Set_Render(CUI_Play_Gauge_Inter::EInter::SONSANG, true, false);
				}

				CTransform* pSlasherTransform = dynamic_cast<CTransform*>(m_pSlasher->Get_Component(TEXT("Transform")));
				_matrix MatrixInverse = m_pTransform->Get_Matrix_Inverse();
				_float3 vLocal;
				XMStoreFloat3(&vLocal, XMVector3TransformCoord(pSlasherTransform->Get_Row(CTransform::ERow::POS), MatrixInverse));
				if (abs(vLocal.x) > abs(vLocal.z)) {
					if (vLocal.x > 0)
						m_pSlasher->Set_ColObjDir(3);//Right
					else
						m_pSlasher->Set_ColObjDir(2);//Left
				}
				else {
					if (vLocal.z > 0)
						m_pSlasher->Set_ColObjDir(1);//Back
					else
						m_pSlasher->Set_ColObjDir(0);//front
				}
			}
			else if (false == m_pCollider->Is_Collision(pSlasherCollider)) {
				if (m_pSlasher->Get_ColObject() == this) {
					// set ui
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::SONSANG), true);
					dynamic_cast<CUI_Play_Gauge_Inter*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Gauge_Inter"))->Set_Render(CUI_Play_Gauge_Inter::EInter::SONSANG, false, false);

					m_pSlasher->Set_ColObject(nullptr);
					m_pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::NONE);
				}
			}
		}
	}

	//hwang
	if (m_bIsLock == true)
		m_dLockTime -= dTimeDelta;
	if (m_dLockTime <= 0.0) {
		m_dLockTime = 15.0;	//Duration
		m_eState = EState::LOCK_END;
	}
	return OBJ_NOEVENT;
}

void CInter_Generator::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	m_pTransform2->Update_Edit(1);
	m_pTransform_S_C->Update_Edit(2);
	//m_pCollider->Update_Edit(2);
	/*m_pCollider_F->Update_Edit(3);
	m_pCollider_B->Update_Edit(4);
	m_pCollider_L->Update_Edit(5);
	m_pCollider_R->Update_Edit(6);
	m_pTransform_GodRay->Update_Edit(7);*/
	ImGui::InputInt("ID", &m_iId);

}

void CInter_Generator::Late_Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
	CInfo* pInfo = CInfo::Get_Instance();
	CInven* pInven = CInven::Get_Instance();
	m_pRenderer->Add_Object(CRenderer::EOrder::NONALPHABLEND, this);
	m_pRenderer->Add_Object(CRenderer::EOrder::SHADOW, this);
	if (EState::ON == m_eState)
		m_pRenderer->Add_Object(CRenderer::EOrder::GODRAY, this);
	if ((true == pInfo->Get_Player() && EState::ON != m_eState) ||
		(true == pInven->Have_Perk(static_cast<_ubyte>(CInven::INVENTYPE::PERKS_CAMPER), static_cast<_uint>(CInven::PERK_CAMPER::FIND_GENERATOR)) && m_bPerkRender == true))
		m_pRenderer->Add_Object(CRenderer::EOrder::PENETRATE, this);

	m_fSuccesTime -= static_cast<_float>(dTimeDelta);
	if (m_fSuccesTime > 0.f) {
		m_pRenderer->Add_Object(CRenderer::EOrder::UI, this);
	}

	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CInter_Generator::Render(const CRenderer::EOrder& eOrder) {
	switch (eOrder) {
	case CRenderer::EOrder::NONALPHABLEND: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < 2; ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 0);
		}
		_matrix Matrix = m_pTransform2->Get_Matrix() * m_pTransform->Get_Matrix();
		m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
		for (_uint i = 2; i < 4; ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 0);
		}
		if (m_bIsLock == true) {
			Matrix = m_pTransform_Entity->Get_Matrix() * m_pTransform->Get_Matrix();
			m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
			for (_uint i = 0; i < m_pModel_Entity->Get_NumMesh(); ++i) {
				m_pModel_Entity->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				m_pModel_Entity->Render(i, m_pShader, 0);
			}
		}
		break;
	}
	case CRenderer::EOrder::SHADOW: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CShadow* pShadow = dynamic_cast<CShadow*>(pGame_Instance->Get_Object_Clone(pGame_Instance->Get_Scene(), TEXT("Shadow")));
		pShadow->Set_RawValue(m_pShader);

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < 2; ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			if (FAILED(m_pModel->Set_Resource(i, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
				m_pModel->Render(i, m_pShader, 1);
			else
				m_pModel->Render(i, m_pShader, 7);
		}
		_matrix Matrix = m_pTransform2->Get_Matrix() * m_pTransform->Get_Matrix();
		m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
		for (_uint i = 2; i < 4; ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			if (FAILED(m_pModel->Set_Resource(i, aiTextureType_NORMALS, m_pShader, "g_NormalTexture")))
				m_pModel->Render(i, m_pShader, 1);
			else
				m_pModel->Render(i, m_pShader, 8);
		}
		break;
	}
	case CRenderer::EOrder::GODRAY: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);

		m_pCollider->Render(pCamera);
		m_pCollider_S_C->Render(pCamera);

		_matrix Matrix = m_pTransform2->Get_Matrix() * m_pTransform->Get_Matrix();
		m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
		for (_uint i = 3; i < 4; ++i) {
			m_pModel->Set_Resource(i, aiTextureType_EMISSIVE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 5);
		}

		_matrix Matrix2 = m_pTransform_GodRay->Get_Matrix() * m_pTransform->Get_Matrix();
		CFragment* pFragment = CFragment::Get_Instance();
		pFragment->Set_GodRayPos(Matrix2.r[3]);
		break;
	}
	case CRenderer::EOrder::PENETRATE: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);
		CInfo* pInfo = CInfo::Get_Instance();
		ID3D11ShaderResourceView* pDepthSRV = m_pTarget_Depth->Get_ShaderResourceView();
		m_pShader->Set_Resource("g_DepthTexture", pDepthSRV);
		_float2 fWinSize = _float2{ static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY) };
		m_pShader->Set_RawValue("g_fWinSize", &fWinSize, sizeof(_float2));
		_float4 vPenetrateColor;
		if (pInfo->Get_Player() == true)
			vPenetrateColor = _float4{ 1.f, 0.f, 0.f, 1.f };
		else
			vPenetrateColor = _float4{ 1.f, 1.f, 0.f, 1.f };
		m_pShader->Set_RawValue("g_vPenetrateColor", &vPenetrateColor, sizeof(_float4));
		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < 2; ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 3);
		}
		_matrix Matrix = m_pTransform2->Get_Matrix() * m_pTransform->Get_Matrix();
		m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
		for (_uint i = 2; i < 4; ++i) {
			m_pModel->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
			m_pModel->Render(i, m_pShader, 3);
		}
		//hwang-Perk
		if (m_bIsLock == true) {
			vPenetrateColor = _float4{ 1.f, 1.f, 0.f, 1.f };
			m_pShader->Set_RawValue("g_vPenetrateColor", &vPenetrateColor, sizeof(_float4));
			Matrix = m_pTransform_Entity->Get_Matrix() * m_pTransform->Get_Matrix();
			m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
			for (_uint i = 0; i < m_pModel_Entity->Get_NumMesh(); ++i) {
				m_pModel_Entity->Set_Resource(i, aiTextureType_DIFFUSE, m_pShader, "g_DiffuseTexture");
				m_pModel_Entity->Render(i, m_pShader, 3);
			}
		}
		break;
	}
	case CRenderer::EOrder::UI: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);
		m_pShader->Set_RawValue("g_vCameraPos", &pCamera->Get_Pos(), sizeof(_float3));
		m_pShader->Set_RawValue("g_fSuccesTime", &m_fSuccesTime, sizeof(_float));
		_float4 vUIColor = _float4{ 1.f, 1.f, 0.f, 1.f };
		m_pShader->Set_RawValue("g_vUIColor", &vUIColor, sizeof(_float4));
		_float4 vUIColor2 = _float4{ 1.f, 0.2f, 0.f, 1.f };
		m_pShader->Set_RawValue("g_vUIColor2", &vUIColor2, sizeof(_float4));

		m_pShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_Float4x4_Transpose(), sizeof(_float4x4));
		for (_uint i = 0; i < 2; ++i) {
			m_pModel->Render(i, m_pShader, 8);
		}
		_matrix Matrix = m_pTransform2->Get_Matrix() * m_pTransform->Get_Matrix();
		m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(Matrix), sizeof(_float4x4));
		for (_uint i = 2; i < 4; ++i) {
			m_pModel->Render(i, m_pShader, 8);
		}
		break;
	}
	case CRenderer::EOrder::NONLIGHT: {
		CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
		pCamera->Set_RawValue(m_pShader);
		m_pCollider->Render(pCamera);
		m_pCollider_GodRay->Render(pCamera);

		m_pCollider_S_F->Render(pCamera);
		m_pCollider_S_B->Render(pCamera);
		m_pCollider_S_L->Render(pCamera);
		m_pCollider_S_R->Render(pCamera);
		break;
	}
	}
	return S_OK;
}

const CInter_Generator::EState& CInter_Generator::Get_State(void) const {
	return m_eState;
}

void CInter_Generator::Set_State(const EState& eState) {
	switch (eState) {
	case EState::OFF:
		m_pModel->Set_Anim(static_cast<_uint>(2));
		break;
	case EState::ONE:
		m_pModel->Set_Anim(static_cast<_uint>(15));
		break;
	case EState::TWO:
		m_pModel->Set_Anim(static_cast<_uint>(16));
		break;
	case EState::THREE:
		m_pModel->Set_Anim(static_cast<_uint>(17));
		break;
	case EState::FOUR:
		m_pModel->Set_Anim(static_cast<_uint>(18));
		break;
	case EState::ON: {
		CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();
		CInfo* pInfo = CInfo::Get_Instance();
		if (true == pInfo->Get_Player()) {
			CObject* pSmoke = pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_GeneratorSuccesSmokeUI"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_GeneratorSuccesSmokeUI"));
			CTransform* pSmokeTransform = dynamic_cast<CTransform*>(pSmoke->Get_Component(TEXT("Transform")));
			pSmokeTransform->Set_Row(CTransform::ERow::POS, m_pTransform->Get_Row(CTransform::ERow::POS) + XMVectorSet(0.f, 1.f, 0.f, 0.f));
			m_fSuccesTime = 5.f;
		}
		CObject* pEffect = pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_GeneratorSuccesParticle"), static_cast<_ubyte>(EScene::GAMEPLAY), TEXT("Effect_GeneratorSuccesParticle"));
		CTransform* pEffectTransform = dynamic_cast<CTransform*>(pEffect->Get_Component(TEXT("Transform")));
		pEffectTransform->Set_Matrix(m_pTransform->Get_Matrix());
		m_pModel->Set_Anim(static_cast<_uint>(13));
		m_pLight->Set_OnOff(true);

		m_pAudio->Play_Sound(TEXT("sfx_generator_start_01"), -1, 0.2f, FMOD_3D, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
		m_pAudio->Play_Sound(TEXT("Generator_Start_Loops_Bounce"), 0, 0.2f, FMOD_3D | FMOD_LOOP_NORMAL, m_pTransform, _float3{ 0.f, 0.f, 0.f }, _float2{ 1.f, 5.f });
		break;
	}
	case EState::CRASH:
		m_pModel->Set_Anim(static_cast<_uint>(1));
		break;
	case EState::LOCK_IN:
		m_bIsLock = true;
		m_pModel_Entity->Set_Anim(static_cast<_uint>(0));
		m_ePreLockState = m_eState;
		break;
	case EState::LOCK_IDLE:
		m_pModel_Entity->Set_Anim(static_cast<_uint>(1));
		break;
	case EState::LOCK_END:
		m_pModel_Entity->Set_Anim(static_cast<_uint>(2));
		break;
	}
	m_eState = eState;

	if (true == m_bFlag)
		Update_Network("Set_State");
}

const _float& CInter_Generator::Get_Gauge(void) {
	return m_fGauge;
}

const void CInter_Generator::Add_Gauge(const _float& fGauge) {
	m_fAddGauge = fGauge;
	m_fGauge += m_fAddGauge;
	Update_Network("Add_Gauge");
}

void CInter_Generator::Update_Network(const string& pKey) {
	Json::Value root;
	if ("Set_State" == pKey) {
		root["State"] = static_cast<_int>(m_eState);
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Add_Gauge" == pKey) {
		root["Gauge"] = m_fAddGauge;
		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
	if ("Set_PlaceID" == pKey) {
		root["Place"]["Front"] = static_cast<_bool>(m_bExist[0]);
		root["Place"]["Back"] = static_cast<_bool>(m_bExist[1]);
		root["Place"]["Left"] = static_cast<_bool>(m_bExist[2]);
		root["Place"]["Right"] = static_cast<_bool>(m_bExist[3]);

		m_pNetwork->Set_Send(m_iId, pKey, root);
	}
}

void CInter_Generator::Update_Network(void) {
	list<Json::Value>& listRecv = m_pNetwork->Get_Recv(m_iId);
	for (auto& iter : listRecv) {
		string key = iter["Key"].asString();
		if ("Set_State" == key) {
			m_bFlag = false;
			if (static_cast<EState>(iter["State"].asInt()) == EState::LOCK_IN)
				m_bIsLock = true;
			if (static_cast<EState>(iter["State"].asInt()) == EState::LOCK_END)
				m_bIsLock = false;
			Set_State(static_cast<EState>(iter["State"].asInt()));
		}
		if ("Add_Gauge" == key) {
			m_fAddGauge = iter["Gauge"].asFloat();
			m_fGauge += m_fAddGauge;
		}
		if ("Set_PlaceID" == key) {
			m_bExist[0] = iter["Place"]["Front"].asBool();
			m_bExist[1] = iter["Place"]["Back"].asBool();
			m_bExist[2] = iter["Place"]["Left"].asBool();
			m_bExist[3] = iter["Place"]["Right"].asBool();
		}
	}
	listRecv.clear();
}
const HRESULT CInter_Generator::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	root["Id"] = m_iId;
	return S_OK;
}

void CInter_Generator::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
	m_iId = root["Id"].asInt();

	CTransform* pLightTransform = dynamic_cast<CTransform*>(m_pLight->Get_Component(TEXT("Transform")));
	pLightTransform->Set_Matrix(m_pTransform->Get_Matrix());
	pLightTransform->Translate(XMVectorSet(0.f, 1.f, 0.f, 0.f), 3.2f, 1.);
	pLightTransform->Translate(XMVectorSet(0.f, 0.f, 1.f, 0.f), -0.7f, 1.);
	pLightTransform->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_pCollider->Update(m_pTransform->Get_Matrix());
	m_pCollider_F->Update(m_pTransform_F->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_B->Update(m_pTransform_B->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_L->Update(m_pTransform_L->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_R->Update(m_pTransform_R->Get_Matrix() * m_pTransform->Get_Matrix());

	m_pCollider_S_C->Update(m_pTransform_S_C->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_S_F->Update(m_pTransform_S_F->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_S_B->Update(m_pTransform_S_B->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_S_L->Update(m_pTransform_S_L->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_S_R->Update(m_pTransform_S_R->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_GodRay->Update(m_pTransform_GodRay->Get_Matrix() * m_pTransform->Get_Matrix());
}

const _bool& CInter_Generator::Get_Flag(void) {
	return m_bFlag;
}

void CInter_Generator::Set_Flag(const _bool& bFlag) {
	m_bFlag = bFlag;
}
