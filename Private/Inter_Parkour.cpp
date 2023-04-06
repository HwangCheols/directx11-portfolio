#include "pch.h"
#include "Inter_Parkour.h"
#include "Camper_GamePlay.h"
#include "Slasher_Huntress.h"
#include "UI_Play_Icon_Hud.h"

CInter_Parkour* const CInter_Parkour::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CInter_Parkour* pInstnace = new CInter_Parkour(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CInter_Parkour::Clone(void* const& pArg) {
	CInter_Parkour* pInstnace = new CInter_Parkour(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CInter_Parkour::CInter_Parkour(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CObject(pDevice, pContext, pNetwork) {
}

CInter_Parkour::CInter_Parkour(const CInter_Parkour& rhs)
	: CObject(rhs) {
}

void CInter_Parkour::Delete(void) {
	__super::Delete();
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pCollider);

	Safe_Release(m_pTransform_F);
	Safe_Release(m_pCollider_F);
	Safe_Release(m_pTransform_B);
	Safe_Release(m_pCollider_B);

	Safe_Release(m_pTransform_S_F);
	Safe_Release(m_pCollider_S_F);
	Safe_Release(m_pTransform_S_B);
	Safe_Release(m_pCollider_S_B);
}

const HRESULT CInter_Parkour::Init_Create(void) {
	return S_OK;
}

const HRESULT CInter_Parkour::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform"), reinterpret_cast<CComponent*&>(m_pTransform));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Renderer"), TEXT("Renderer"), reinterpret_cast<CComponent*&>(m_pRenderer));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider"), reinterpret_cast<CComponent*&>(m_pCollider));
	m_pTransform->Set_Row(CTransform::ERow::POS, XMVectorSet(196.f, 0.f, 173.f, 1.f));
	m_pCollider->Set_Extents(_float3{ 0.5f, 0.5f, 0.8f });

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_F"), reinterpret_cast<CComponent*&>(m_pTransform_F));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_F"), reinterpret_cast<CComponent*&>(m_pCollider_F));
	m_pTransform_F->Translate(XMVectorSet(0.f, 0.f, 1.f, 0.f), -0.9f, 1.);
	m_pCollider_F->Set_Extents(_float3{ 0.1f, 0.1f, 0.1f });

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_B"), reinterpret_cast<CComponent*&>(m_pTransform_B));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_B"), reinterpret_cast<CComponent*&>(m_pCollider_B));
	m_pTransform_B->Translate(XMVectorSet(0.f, 0.f, 1.f, 0.f), 0.9f, 1.);
	m_pTransform_B->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
	m_pCollider_B->Set_Extents(_float3{ 0.1f, 0.1f, 0.1f });

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_S_F"), reinterpret_cast<CComponent*&>(m_pTransform_S_F));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_S_F"), reinterpret_cast<CComponent*&>(m_pCollider_S_F));
	m_pTransform_S_F->Translate(XMVectorSet(0.f, 0.f, 1.f, 0.f), -0.6f, 1.);
	m_pCollider_S_F->Set_Extents(_float3{ 0.1f, 0.1f, 0.1f });

	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Transform"), TEXT("Transform_S_B"), reinterpret_cast<CComponent*&>(m_pTransform_S_B));
	hr |= __super::Add_Component(static_cast<_ubyte>(EScene::STATIC), TEXT("Collider_OBB"), TEXT("Collider_S_B"), reinterpret_cast<CComponent*&>(m_pCollider_S_B));
	m_pTransform_S_B->Translate(XMVectorSet(0.f, 0.f, 1.f, 0.f), 0.6f, 1.);
	m_pTransform_S_B->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
	m_pCollider_S_B->Set_Extents(_float3{ 0.1f, 0.1f, 0.1f });

	return hr;
}

const _ubyte CInter_Parkour::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

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
			if (pCamper->Get_Control()) {
				if (m_pCollider->Is_Collision(pCamperCollider)) {
					//넘기/들어가기 UI필요
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ECamper::PARKOUR), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

					pCamper->Set_ColObject(this);
					pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::PARKOUR);

					_matrix Matrix_Inverse = m_pTransform->Get_Matrix_Inverse();
					_vector vCamperLocal = XMVector3TransformCoord(dynamic_cast<CTransform*>(pCamper->Get_Component(TEXT("Transform")))->Get_Row(CTransform::ERow::POS), Matrix_Inverse);

					if (0.f > XMVectorGetZ(vCamperLocal))
						pCamper->Set_ColObjDir(0); // front
					else
						pCamper->Set_ColObjDir(1); // Back

					if (pCamper->Get_FM_Camper()->Get_State() == CFM_Camper::EState::ParkourVault_Jump)
						dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::PARKOUR), true);
				}
				else {
					if (pCamper->Get_ColObject() == this) {
						// UI필요X
						dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ECamper::PARKOUR), true);

						pCamper->Set_ColObject(nullptr);
						pCamper->Set_CollisionState(CCamper_GamePlay::ECollisionState::NONE);
					}
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

			if (m_pSlasher->Get_FM_Slasher()->Get_State() != CFM_Slasher::EState::PARKOUR && m_pSlasher->Get_CollisionState() != CSlasher_Huntress::ECollisionState::CAMPER_CRAWL
				&& true == m_pCollider->Is_Collision(pSlasherCollider)) {
				m_pSlasher->Set_ColObject(this);
				m_pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::PARKOUR);
				// 넘기 들어가기 UI 로 바꿔주셈 
				(0 < m_pSlasher->Get_WeaponCount()) ?
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ESlasher::PARKOUR), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::RIGHT) :
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOn(static_cast<_int>(CUI_Play_Icon_Hud::ESlasher::PARKOUR), CUI_Play_Icon_Hud::EKey::SPACE, CUI_Play_Icon_Hud::EDiv::CENTER);

				_matrix Matrix_Inverse = m_pTransform->Get_Matrix_Inverse();
				_vector vSlasherLocal = XMVector3TransformCoord(dynamic_cast<CTransform*>(m_pSlasher->Get_Component(TEXT("Transform")))->Get_Row(CTransform::ERow::POS), Matrix_Inverse);

				if (0.f > XMVectorGetZ(vSlasherLocal))
					m_pSlasher->Set_ColObjDir(0); // front
				else
					m_pSlasher->Set_ColObjDir(1); // Back
			}
			else if (false == m_pCollider->Is_Collision(pSlasherCollider)) {
				if (m_pSlasher->Get_ColObject() == this) {
					// set ui
					dynamic_cast<CUI_Play_Icon_Hud*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::GAMEPLAY), L"UI_Play_Icon_Hud"))->Set_HudOff(static_cast<_uint>(CUI_Play_Icon_Hud::ESlasher::PARKOUR), true);

					m_pSlasher->Set_ColObject(nullptr);
					m_pSlasher->Set_CollisionState(CSlasher_Huntress::ECollisionState::NONE);
				}
			}
		}
	}

	m_pCollider->Update(m_pTransform->Get_Matrix());
	m_pCollider_F->Update(m_pTransform_F->Get_Matrix() * m_pTransform->Get_Matrix());
	m_pCollider_B->Update(m_pTransform_B->Get_Matrix() * m_pTransform->Get_Matrix());

	return OBJ_NOEVENT;
}

void CInter_Parkour::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	m_pCollider->Update_Edit(1);
	m_pTransform_F->Update_Edit(2);
	m_pTransform_B->Update_Edit(3);
	ImGui::InputInt("ID", &m_iId);
}

void CInter_Parkour::Late_Update(const _double& dTimeDelta) {
	m_pRenderer->Add_Object(CRenderer::EOrder::NONLIGHT, this);
}

const HRESULT CInter_Parkour::Render(const CRenderer::EOrder& eOrder) {
	CCamera* pCamera = CCamera::Get_Camera(static_cast<_uint>(0));
	//m_pCollider->Render(pCamera);
	//m_pCollider_F->Render(pCamera);
	//m_pCollider_B->Render(pCamera);
	return S_OK;
}

const HRESULT CInter_Parkour::Save_Data(Json::Value& root) {
	m_pTransform->Save_Data(root["Transform"]);
	root["Id"] = m_iId;
	return S_OK;
}

void CInter_Parkour::Load_Data(Json::Value& root) {
	m_pTransform->Load_Data(root["Transform"]);
	m_iId = root["Id"].asInt();
}
