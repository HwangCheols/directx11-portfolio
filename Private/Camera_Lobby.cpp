#include "pch.h"
#include "Camera_Lobby.h"
#include "UI_Image.h"
#include "Slasher_Huntress_Lobby.h"
#include "Weapon_Slash_Lobby.h"
#include "Camper_Lobby_Sit.h"
#include "Effect_Free3.h"
#include "Camper_Lobby_Sit.h"
#include "Camper_Lobby.h"
#include "Setting_CamperSlot.h"
#include "Inter_Crow_Lobby.h"

CCamera_Lobby* const CCamera_Lobby::Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork) {
	CCamera_Lobby* pInstnace = new CCamera_Lobby(pDevice, pContext, pNetwork);
	if (FAILED(pInstnace->Init_Create()))
		Safe_Release(pInstnace);
	return pInstnace;
}

CObject* const CCamera_Lobby::Clone(void* const& pArg) {
	CCamera_Lobby* pInstnace = new CCamera_Lobby(*this);
	if (FAILED(pInstnace->Init_Clone(pArg)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CCamera_Lobby::CCamera_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, CNetwork* const& pNetwork)
	: CCamera(pDevice, pContext, pNetwork) {
}

CCamera_Lobby::CCamera_Lobby(const CCamera_Lobby& rhs)
	: CCamera(rhs) {
}

void CCamera_Lobby::Delete(void) {
	__super::Delete();
}

const HRESULT CCamera_Lobby::Init_Create(void) {
	return S_OK;
}

const HRESULT CCamera_Lobby::Init_Clone(void* const& pArg) {
	HRESULT hr = S_OK;
	hr |= __super::Init_Clone(pArg);
	Load_Data();
	return hr;
}

const _ubyte CCamera_Lobby::Update(const _double& dTimeDelta) {
	CGame_Instance* pGame_Instance = CGame_Instance::Get_Instance();

	if (m_bIsFinished == true)
		m_dTime += dTimeDelta;

	CSlasher_Huntress_Lobby* pSlasher = dynamic_cast<CSlasher_Huntress_Lobby*>(pGame_Instance->Get_Object_Clone(static_cast<_uint>(EScene::LOBBY), TEXT("Slasher_Huntress_Lobby")));
	if (pSlasher == nullptr)
		return OBJ_NOEVENT;
	CCamper_Lobby_Sit* pCamper = dynamic_cast<CCamper_Lobby_Sit*>(pGame_Instance->Get_Object_Clone(static_cast<_uint>(EScene::LOBBY), TEXT("Camper_Lobby_Sit")));
	if (pCamper == nullptr)
		return OBJ_NOEVENT;

	//if (m_dTime >= m_dFirstLerpTime) {
	//	m_dTime = 0.0;
	//	m_bIsFinished = false;
	//	m_eCamState = ECAMSTATE::NORMAL;
	//}

	switch (m_eCamState)
	{
	case ECAMSTATE::NORMAL:
		if (pGame_Instance->Get_Input_KeyboardKey(DIK_UP) & 0x80)
			m_pTransform->Translate(m_pTransform->Get_Row(CTransform::ERow::LOOK), m_fSpeed, dTimeDelta);
		if (pGame_Instance->Get_Input_KeyboardKey(DIK_LEFT) & 0x80)
			m_pTransform->Translate(m_pTransform->Get_Row(CTransform::ERow::RIGHT), -m_fSpeed, dTimeDelta);
		if (pGame_Instance->Get_Input_KeyboardKey(DIK_RIGHT) & 0x80)
			m_pTransform->Translate(m_pTransform->Get_Row(CTransform::ERow::RIGHT), m_fSpeed, dTimeDelta);
		if (pGame_Instance->Get_Input_KeyboardKey(DIK_DOWN) & 0x80)
			m_pTransform->Translate(m_pTransform->Get_Row(CTransform::ERow::LOOK), -m_fSpeed, dTimeDelta);

		if (pGame_Instance->Get_Input_MouseButton(CInput::EMouseButton::R)) {
			_long MouseMove = 0;
			if (MouseMove = pGame_Instance->Get_Input_MouseMove(CInput::EMouseMove::X))
				m_pTransform->Rotate(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * 0.1f, dTimeDelta);
			if (MouseMove = pGame_Instance->Get_Input_MouseMove(CInput::EMouseMove::Y))
				m_pTransform->Rotate(m_pTransform->Get_Row(CTransform::ERow::RIGHT), MouseMove * 0.1f, dTimeDelta);
		}
		break;
	case ECAMSTATE::START_FIRST:
		if (m_bIsFinished == true) {
			_float fRatio = min(1.f, (_float)m_dTime / (_float)m_dFirstLerpTime);

			pSlasher->Set_State(CSlasher_Huntress_Lobby::ESTATE::WATCH);
			pCamper->Set_State(CCamper_Lobby_Sit::ESTATE::SIT);
			_vector vSourPos = m_FirstSourMatrix.r[3];
			_vector vDestPos = m_FirstDestMatrix.r[3];

			_vector vSourRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_FirstSourMatrix));
			_vector vDestRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_FirstDestMatrix));
			_matrix vResultMatrix = XMMatrixRotationQuaternion(XMQuaternionSlerp(vSourRot, vDestRot, fRatio));
			m_pTransform->Set_Matrix(vResultMatrix);
			_vector vLerpPos = XMVectorLerp(vSourPos, vDestPos, fRatio);
			m_pTransform->Set_Row(CTransform::ERow::POS, vLerpPos);
		}
		if (false == m_bCameraMoveEnd && m_dTime >= m_dFirstLerpTime) {
			m_bCameraMoveEnd = true;
			if (!m_vecUIGroups[static_cast<_uint>(m_eCamState)].empty()) {
				for_each(m_vecUIGroups[(_uint)m_eCamState].begin(), m_vecUIGroups[(_uint)m_eCamState].end(), [&](auto& iter) { iter->Set_Active(true); });
			}
			m_dTime = 0;
			m_bIsFinished = false;
			m_eCamState = ECAMSTATE::NORMAL;

			CSlasher_Huntress_Lobby* pShasher = dynamic_cast<CSlasher_Huntress_Lobby*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Slasher_Huntress_Lobby")));
			CTransform* pSlahserTransform = dynamic_cast<CTransform*>(pShasher->Get_Component(TEXT("Transform")));
			pShasher->Set_Dissolve(2.5f);

			CEffect_Free3* pEffect = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_LobbySlasherSmoke"), static_cast<_ubyte>(EScene::LOBBY), TEXT("Effect_LobbySlasherSmoke")));
			CEffect_Free3* pParticleEffect = dynamic_cast<CEffect_Free3*>(pGame_Instance->CreateGet_Object_Clone(TEXT("Effect_LobbySlasherParticle"), static_cast<_ubyte>(EScene::LOBBY), TEXT("Effect_LobbySlasherParticle")));
			CTransform* pEffectTransform = dynamic_cast<CTransform*>(pEffect->Get_Component(TEXT("Transform")));
			CTransform* pParticleEffectTransform = dynamic_cast<CTransform*>(pParticleEffect->Get_Component(TEXT("Transform")));
			pEffectTransform->Set_Row(CTransform::ERow::POS, pSlahserTransform->Get_Row(CTransform::ERow::POS));
			pParticleEffectTransform->Set_Row(CTransform::ERow::POS, pSlahserTransform->Get_Row(CTransform::ERow::POS) - pSlahserTransform->Get_Row(CTransform::ERow::RIGHT) * 0.8f + XMVectorSet(0.f, 0.5f, 0.f, 0.f) + pSlahserTransform->Get_Row(CTransform::ERow::LOOK) * 0.3f);

			list<CObject*>* plistCamper = pGame_Instance->Get_Object_ListClone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Camper_Lobby_Sit"));
			for (auto& iter : *plistCamper) {
				CCamper_Lobby_Sit* pCamperSit = dynamic_cast<CCamper_Lobby_Sit*>(iter);
				pCamperSit->Set_Dissolve(1.5f);
			}

			//Crow
			CInter_Crow_Lobby* pInterCrow = dynamic_cast<CInter_Crow_Lobby*>(pGame_Instance->Get_Object_Clone(static_cast<_uint>(EScene::LOBBY), TEXT("Inter_Crow_Lobby"), 0));
			if (nullptr != pInterCrow)
				pInterCrow->Set_State(CInter_Crow_Lobby::ESTATE::FLY);
		}
		break;
	case ECAMSTATE::START_CAMPER:
		if (m_bIsFinished == true) {
			_float fRatio = min(1.f, (_float)m_dTime / (_float)m_dStartCamperLerpTime);

			pCamper->Set_State(CCamper_Lobby_Sit::ESTATE::SIT);
			_vector vSourPos = m_FirstDestMatrix.r[3];
			_vector vDestPos = m_StartCamperMatrix.r[3];

			_vector vSourRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_FirstDestMatrix));
			_vector vDestRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_StartCamperMatrix));
			_matrix vResultMatrix = XMMatrixRotationQuaternion(XMQuaternionSlerp(vSourRot, vDestRot, fRatio));
			m_pTransform->Set_Matrix(vResultMatrix);
			_vector vLerpPos = XMVectorLerp(vSourPos, vDestPos, fRatio);
			m_pTransform->Set_Row(CTransform::ERow::POS, vLerpPos);
		}
		if (false == m_bCameraMoveEnd && m_dTime >= m_dStartCamperLerpTime) {
			m_bCameraMoveEnd = true;
			m_bCameraCamperRender = true;
			m_dTime = 0;
			m_bIsFinished = false;
			m_eCamState = ECAMSTATE::NORMAL;

			//Camper
			CInfo* pInfo = CInfo::Get_Instance();
			pInfo->Init_pCamperSlot();
			_uint iCamperSlot = 1;
			_uint iMyCamperSlot = pInfo->Get_MyCamperSlot();
			list<CObject*>* plistCamper = pGame_Instance->Get_Object_ListClone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Camper_Lobby"));
			for (auto& iter : *plistCamper) {
				CCamper_Lobby* pCamper = dynamic_cast<CCamper_Lobby*>(iter);
				CTransform* pCamperTransform = dynamic_cast<CTransform*>(pCamper->Get_Component(TEXT("Transform")));

				if (pCamper->Get_Control()) {
					CSetting_CamperSlot* pSetting = dynamic_cast<CSetting_CamperSlot*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Setting_CamperSlot")));
					CTransform* pSettingTransform = dynamic_cast<CTransform*>(pSetting->Get_Component(TEXT("Transform")));
					pCamperTransform->Set_Matrix(pSettingTransform->Get_Matrix());
					pSetting->Set_EffectRender(false);
					pCamper->Set_MyCamperSlot(iMyCamperSlot);
					pCamper->Set_Dissolve(2.5f);
					pCamper->Create_Effect();

					pInfo->Set_pCamperSlot(0, pCamper);
					pCamper->Update_Network("Add_Camper");
				}
				else {
					if (pCamper->Get_GoalDissolveLength() > 0.f) {
						CSetting_CamperSlot* pSetting = dynamic_cast<CSetting_CamperSlot*>(pGame_Instance->Get_Object_Clone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Setting_CamperSlot"), iCamperSlot));
						CTransform* pSettingTransform = dynamic_cast<CTransform*>(pSetting->Get_Component(TEXT("Transform")));
						pCamperTransform->Set_Matrix(pSettingTransform->Get_Matrix());
						pSetting->Set_EffectRender(false);

						pInfo->Set_pCamperSlot(iCamperSlot, pCamper);
						++iCamperSlot;
					}
				}
			}

			//Sit
			list<CObject*>* plistCamperSit = pGame_Instance->Get_Object_ListClone(static_cast<_ubyte>(EScene::LOBBY), TEXT("Camper_Lobby_Sit"));
			for (auto& iter : *plistCamperSit) {
				CCamper_Lobby_Sit* pCamperSit = dynamic_cast<CCamper_Lobby_Sit*>(iter);
				pCamperSit->Set_Dissolve(-0.1f);
			}
		}
		break;
	case ECAMSTATE::SELECT_SLASHER:
		if (m_bIsFinished == true) {
			_float fRatio = min(1.f, (_float)m_dTime / (_float)m_dSelectSlasherLerpTime);

			pSlasher->Set_State(CSlasher_Huntress_Lobby::ESTATE::IDLE);
			_vector vSourPos = m_FirstDestMatrix.r[3];
			_vector vDestPos = m_SelectSlasherMatrix.r[3];

			_vector vSourRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_FirstDestMatrix));
			_vector vDestRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_SelectSlasherMatrix));
			_matrix vResultMatrix = XMMatrixRotationQuaternion(XMQuaternionSlerp(vSourRot, vDestRot, fRatio));
			m_pTransform->Set_Matrix(vResultMatrix);
			_vector vLerpPos = XMVectorLerp(vSourPos, vDestPos, fRatio);
			m_pTransform->Set_Row(CTransform::ERow::POS, vLerpPos);
		}
		if (m_dTime >= m_dSelectSlasherLerpTime) {
			m_dTime = 0;
			m_bIsFinished = false;
			m_eCamState = ECAMSTATE::NORMAL;
		}
		break;
	case ECAMSTATE::SELECT_CAMPER:
		if (m_bIsFinished == true) {
			_float fRatio = min(1.f, (_float)m_dTime / (_float)m_dSelectCamperLerpTime);

			_vector vSourPos = m_StartCamperMatrix.r[3];
			_vector vDestPos = m_SelectCamperMatrix.r[3];

			_vector vSourRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_StartCamperMatrix));
			_vector vDestRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_SelectCamperMatrix));
			_matrix vResultMatrix = XMMatrixRotationQuaternion(XMQuaternionSlerp(vSourRot, vDestRot, fRatio));
			m_pTransform->Set_Matrix(vResultMatrix);
			_vector vLerpPos = XMVectorLerp(vSourPos, vDestPos, fRatio);
			m_pTransform->Set_Row(CTransform::ERow::POS, vLerpPos);
		}
		if (m_dTime >= m_dSelectCamperLerpTime) {
			m_dTime = 0;
			m_bIsFinished = false;
			m_eCamState = ECAMSTATE::NORMAL;
		}
		break;
	case ECAMSTATE::SELECT_CAMPER_TO_START_CAMPER:
		if (m_bIsFinished == true) {
			_float fRatio = min(1.f, (_float)m_dTime / (_float)m_dStartCamperLerpTime);

			_vector vSourPos = m_SelectCamperMatrix.r[3];
			_vector vDestPos = m_StartCamperMatrix.r[3];

			_vector vSourRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_SelectCamperMatrix));
			_vector vDestRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_StartCamperMatrix));
			_matrix vResultMatrix = XMMatrixRotationQuaternion(XMQuaternionSlerp(vSourRot, vDestRot, fRatio));
			m_pTransform->Set_Matrix(vResultMatrix);
			_vector vLerpPos = XMVectorLerp(vSourPos, vDestPos, fRatio);
			m_pTransform->Set_Row(CTransform::ERow::POS, vLerpPos);
		}
		if (m_dTime >= m_dStartCamperLerpTime + 0.1) {
			m_dTime = 0;
			m_bIsFinished = false;
			m_eCamState = ECAMSTATE::NORMAL;
		}
		break;
	case ECAMSTATE::START_CAMPER_TO_FIRST:
		if (m_bIsFinished == true) {
			_float fRatio = min(1.f, (_float)m_dTime / (_float)m_dStartCamperLerpTime);

			pCamper->Set_State(CCamper_Lobby_Sit::ESTATE::SIT);
			pSlasher->Set_State(CSlasher_Huntress_Lobby::ESTATE::WATCH);
			_vector vSourPos = m_StartCamperMatrix.r[3];
			_vector vDestPos = m_FirstDestMatrix.r[3];

			_vector vSourRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_StartCamperMatrix));
			_vector vDestRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_FirstDestMatrix));
			_matrix vResultMatrix = XMMatrixRotationQuaternion(XMQuaternionSlerp(vSourRot, vDestRot, fRatio));
			m_pTransform->Set_Matrix(vResultMatrix);
			_vector vLerpPos = XMVectorLerp(vSourPos, vDestPos, fRatio);
			m_pTransform->Set_Row(CTransform::ERow::POS, vLerpPos);
		}
		if (m_dTime >= m_dStartCamperLerpTime) {
			m_dTime = 0;
			m_bIsFinished = false;
			m_eCamState = ECAMSTATE::NORMAL;
		}
		break;
	case ECAMSTATE::START_SLASHER_TO_FIRST:
		if (m_bIsFinished == true) {
			_float fRatio = min(1.f, (_float)m_dTime / (_float)m_dSelectSlasherLerpTime);

			pSlasher->Set_State(CSlasher_Huntress_Lobby::ESTATE::WATCH);
			_vector vSourPos = m_SelectSlasherMatrix.r[3];
			_vector vDestPos = m_FirstDestMatrix.r[3];

			_vector vSourRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_SelectSlasherMatrix));
			_vector vDestRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_FirstDestMatrix));
			_matrix vResultMatrix = XMMatrixRotationQuaternion(XMQuaternionSlerp(vSourRot, vDestRot, fRatio));
			m_pTransform->Set_Matrix(vResultMatrix);
			_vector vLerpPos = XMVectorLerp(vSourPos, vDestPos, fRatio);
			m_pTransform->Set_Row(CTransform::ERow::POS, vLerpPos);
		}
		if (m_dTime >= m_dSelectSlasherLerpTime) {
			m_dTime = 0;
			m_bIsFinished = false;
			m_eCamState = ECAMSTATE::NORMAL;
		}
		break;
	case Client::CCamera_Lobby::ECAMSTATE::END:
		break;
	}

	if (!m_bIsFinished && ECAMSTATE::NORMAL != m_eOnGroup) {
		for_each(m_vecUIGroups[(_uint)m_eOnGroup].begin(), m_vecUIGroups[(_uint)m_eOnGroup].end(), [&](auto& iter) { iter->Set_Active(true); });
		m_eOnGroup = ECAMSTATE::NORMAL;
	}

	return OBJ_NOEVENT;
}

void CCamera_Lobby::Update_Edit(void) {
	m_pTransform->Update_Edit(0);
	ImGui::DragFloat("Speed", &m_fSpeed);

	if (ImGui::Button("SaveMatrix")) Save_Data();
	ImGui::SameLine();
	if (ImGui::Button("LoadMatrix")) Load_Data();

	if (ImGui::Button("FirstSourMatrix")) m_FirstSourMatrix = m_pTransform->Get_Matrix();
	ImGui::SameLine();
	if (ImGui::Button("FirstDestMatrix")) m_FirstDestMatrix = m_pTransform->Get_Matrix();
	ImGui::InputDouble("FirstLerpTIme", &m_dFirstLerpTime);

	if (ImGui::Button("StartSlasherDestMatrix")) m_StartSlasherMatrix = m_pTransform->Get_Matrix();
	ImGui::InputDouble("StartSlasherLerpTIme", &m_dStartSlasherLerpTime);

	if (ImGui::Button("StartCamperDestMatrix")) m_StartCamperMatrix = m_pTransform->Get_Matrix();
	ImGui::InputDouble("StartCamperLerpTIme", &m_dStartCamperLerpTime);

	if (ImGui::Button("SelectSlasherDestMatrix")) m_SelectSlasherMatrix = m_pTransform->Get_Matrix();
	ImGui::InputDouble("SelectSlasherLerpTIme", &m_dSelectSlasherLerpTime);

	if (ImGui::Button("SelectCamperDestMatrix")) m_SelectCamperMatrix = m_pTransform->Get_Matrix();
	ImGui::InputDouble("SelectCamperLerpTIme", &m_dSelectCamperLerpTime);



	if (ImGui::Button("Normal")) {
		m_eCamState = ECAMSTATE::NORMAL;
		m_dTime = 0.0;
	}
	ImGui::SameLine();
	if (ImGui::Button("First")) {
		m_eCamState = ECAMSTATE::START_FIRST;
		m_bIsFinished = true;
	}
	if (ImGui::Button("Start_Camper")) {
		m_eCamState = ECAMSTATE::START_CAMPER;
		m_bIsFinished = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Select_Camper")) {
		m_eCamState = ECAMSTATE::SELECT_CAMPER;
		m_bIsFinished = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Select_Slasher")) {
		m_eCamState = ECAMSTATE::SELECT_SLASHER;
		m_bIsFinished = true;
	}
	if (ImGui::Button("Select_Camper_to_Start_Champer")) {
		m_eCamState = ECAMSTATE::SELECT_CAMPER_TO_START_CAMPER;
		m_bIsFinished = true;
	}
	if (ImGui::Button("Start_Slasher_to_First")) {
		m_eCamState = ECAMSTATE::START_SLASHER_TO_FIRST;
		m_bIsFinished = true;
	}
	if (ImGui::Button("Start_Camper_to_First")) {
		m_eCamState = ECAMSTATE::START_CAMPER_TO_FIRST;
		m_bIsFinished = true;
	}
}

void CCamera_Lobby::Late_Update(const _double& dTimeDelta) {
	Set_OldViewMatrix();
	Set_ViewMatrix();
}

const HRESULT CCamera_Lobby::Render(const CRenderer::EOrder& eOrder) {
	return S_OK;
}

void CCamera_Lobby::Set_State(const ECAMSTATE& eState) {
	m_bCameraMoveEnd = false;
	if (ECAMSTATE::START_CAMPER_TO_FIRST == eState)
		m_bCameraCamperRender = false;
	m_eCamState = eState;
	m_dTime = 0.;
	m_bIsFinished = true;
}

void CCamera_Lobby::Set_Group(const ECAMSTATE& eOffState, const ECAMSTATE& eOnState) {
	for_each(m_vecUIGroups[(_uint)eOffState].begin(), m_vecUIGroups[(_uint)eOffState].end(), [&](auto& iter) { iter->Set_Active(false); });
	if (ECAMSTATE::END == eOnState)
		return;
	m_eOnGroup = eOnState;
}

vector<class CUI_Image*>* CCamera_Lobby::Get_Group(const _uint& iState) {
	return &m_vecUIGroups[iState];
}

void CCamera_Lobby::Add_LobbyBtn(const ECAMSTATE& eState, CUI_Image* pBtn) {
	// use / group
	m_pLobbyBtns[static_cast<_uint>(eState)] = pBtn;
}

void CCamera_Lobby::Add_UIGroup(const ECAMSTATE& eState, CUI_Image* pImage, const _bool& bActive) {
	if (m_vecUIGroups[(_uint)eState].empty()) {
		m_vecUIGroups[(_uint)eState].emplace_back(pImage);
		pImage->Set_Active(bActive);
		return;
	}
	else {
		for (auto& iter : m_vecUIGroups[(_uint)eState]) {
			if (iter != pImage) {
				m_vecUIGroups[(_uint)eState].emplace_back(pImage);
				pImage->Set_Active(bActive);
				return;
			}
		}
	}
}

const _bool& CCamera_Lobby::Get_CameraCamperRender(void) {
	return m_bCameraCamperRender;
}

const HRESULT CCamera_Lobby::Save_Data() {
	ofstream ofs(TEXT("../Bin/Resource/02.Lobby/Camera.txt"));
	if (ofs.fail())
		int i = 0;
	Json::Value root;
	_float4x4 matrix;
	//First
	XMStoreFloat4x4(&matrix, m_FirstSourMatrix);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			root["FirstSourMatrix"][to_string(i)][to_string(j)] = matrix.m[i][j];
	XMStoreFloat4x4(&matrix, m_FirstDestMatrix);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			root["FirstDestMatrix"][to_string(i)][to_string(j)] = matrix.m[i][j];
	root["FirstLerpTime"] = m_dFirstLerpTime;
	//StartSlasher
	XMStoreFloat4x4(&matrix, m_StartSlasherMatrix);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			root["StartSlasherDestMatrix"][to_string(i)][to_string(j)] = matrix.m[i][j];
	root["StartSlasherLerpTime"] = m_dStartSlasherLerpTime;
	//StartCamper
	XMStoreFloat4x4(&matrix, m_StartCamperMatrix);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			root["StartCamperDestMatrix"][to_string(i)][to_string(j)] = matrix.m[i][j];
	root["StartCamperLerpTime"] = m_dStartCamperLerpTime;
	//SelectSlasher
	XMStoreFloat4x4(&matrix, m_SelectSlasherMatrix);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			root["SelectSlasherDestMatrix"][to_string(i)][to_string(j)] = matrix.m[i][j];
	root["SelectSlasherLerpTime"] = m_dSelectSlasherLerpTime;
	//SelectCamper
	XMStoreFloat4x4(&matrix, m_SelectCamperMatrix);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			root["SelectCamperDestMatrix"][to_string(i)][to_string(j)] = matrix.m[i][j];
	root["SelectCamperLerpTime"] = m_dSelectCamperLerpTime;

	ofs << root;
	ofs.close();
	return S_OK;
}

void CCamera_Lobby::Load_Data() {
	ifstream ifs(TEXT("../Bin/Resource/02.Lobby/Camera.txt"));
	Json::Value root;
	ifs >> root;
	ifs.close();
	_float4x4 matrix;
	//first
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix.m[i][j] = root["FirstSourMatrix"][to_string(i)][to_string(j)].asFloat();
	m_FirstSourMatrix = XMLoadFloat4x4(&matrix);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix.m[i][j] = root["FirstDestMatrix"][to_string(i)][to_string(j)].asFloat();
	m_FirstDestMatrix = XMLoadFloat4x4(&matrix);
	m_dFirstLerpTime = root["FirstLerpTime"].asDouble();
	//StartSlasher
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix.m[i][j] = root["StartSlasherDestMatrix"][to_string(i)][to_string(j)].asFloat();
	m_StartSlasherMatrix = XMLoadFloat4x4(&matrix);
	m_dStartSlasherLerpTime = root["StartSlasherLerpTime"].asDouble();
	//StartCamper
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix.m[i][j] = root["StartCamperDestMatrix"][to_string(i)][to_string(j)].asFloat();
	m_StartCamperMatrix = XMLoadFloat4x4(&matrix);
	m_dStartCamperLerpTime = root["StartCamperLerpTime"].asDouble();
	//SelectSlasher
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix.m[i][j] = root["SelectSlasherDestMatrix"][to_string(i)][to_string(j)].asFloat();
	m_SelectSlasherMatrix = XMLoadFloat4x4(&matrix);
	m_dSelectSlasherLerpTime = root["SelectSlasherLerpTime"].asDouble();
	//StartCamper
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix.m[i][j] = root["SelectCamperDestMatrix"][to_string(i)][to_string(j)].asFloat();
	m_SelectCamperMatrix = XMLoadFloat4x4(&matrix);
	m_dSelectCamperLerpTime = root["SelectCamperLerpTime"].asDouble();
}