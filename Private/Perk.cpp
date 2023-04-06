#include "pch.h"
#include "Perk.h"

CPerk* const CPerk::Create(_bool bIsCamper,_uint iIndex) {
	CPerk* pPerk = new CPerk();
	if (FAILED(pPerk->Init_Create(bIsCamper,iIndex)))
		Safe_Release(pPerk);
	return pPerk;
}

CSkill* const CPerk::Clone(void* pArg) {
	CPerk* pPerk = new CPerk(*this);
	if (FAILED(pPerk->Init_Clone(pArg)))
		return nullptr;
	return pPerk;
}

CPerk::CPerk(void) {
	ZeroMemory(&m_sPerk, sizeof(PERK));
}

void CPerk::Delete(void) {


}

const HRESULT CPerk::Init_Create(_bool bIsCamper,_uint iIndex) {
	ZeroMemory(&m_sPerk, sizeof(PERK));
	if (bIsCamper == true)
		Create_CamperPerk(iIndex);
	else
		Create_SlasherPerk(iIndex);
	return S_OK;
}

const HRESULT CPerk::Init_Clone(void* pArg){

	return S_OK;
}

void CPerk::Push() {

}

void CPerk::Update(_double dTimeDelta) {
	if (m_bIsTrigger == true) {
		m_dStartTime -= dTimeDelta;
		m_dDuringTime -= dTimeDelta;
		if (m_dDuringTime <= 0.0) 
			m_dDuringTime = 0.0;
		if (m_dStartTime <= 0.0) {
			m_bIsTrigger = false;
			m_dStartTime = m_dCoolTime;
			m_dDuringTime = m_sPerk.dDuringTime;
		}
	}
}

const CPerk::PERK& CPerk::Get_PerkInfo(){
	return m_sPerk;
}

void CPerk::Use_Perk() {
	m_bIsTrigger = true;
}

const _bool& CPerk::Get_IsUsePerk() const {
	return m_bIsTrigger;
}

const _double& CPerk::Get_StartTime() const {
	return m_dStartTime;
}

const _double& CPerk::Get_DuringTIme() const{
	return m_dDuringTime;
}

const _double& CPerk::Get_CoolTime() const{
	return m_dCoolTime;
}

_bool CPerk::ReadyStart(){
	if (m_dStartTime == m_dCoolTime && m_bIsTrigger == false)
		return true;
	else
		return false;
}

void CPerk::Set_Start(){
	if (m_dStartTime == m_dCoolTime && m_bIsTrigger == false)
		m_bIsTrigger = true;
}

void CPerk::Create_CamperPerk(_uint iIndex) {
	switch (iIndex)
	{
	case 0://FIND_CAMPER
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dDuringTime = 3.0;
		m_sPerk.dCoolTime = 40.0;
		break;
	case 1://SELF_HEAL
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	case 2://RUNAWAY
		m_sPerk.eType = TYPE::ACTIVE;
		m_sPerk.dDuringTime = 3.0;
		m_sPerk.dCoolTime = 9999.0;
		break;
	case 3://ESCAPE_CARRY
		m_sPerk.eType = TYPE::ACTIVE;
		m_sPerk.dCoolTime = 9999.0;
		break;
	case 4://LOCKER
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	case 5://STAY_CROW
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	case 6://END_GENERATOR
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dCoolTime = 9999.0;
		break;
	case 7://FIND_SLASHER
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dDuringTime = 5.0;
		m_sPerk.dCoolTime = 5.0;
		break;
	case 8://FAST
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dDuringTime = 2.0;
		m_sPerk.dCoolTime = 20.0;
		break;
	case 9://NEWBIE
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	case 10://PURIFICATION
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dCoolTime = 9999.0;
		break;
	case 11://FIND_GENERATOR
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dDuringTime = 10.0;
		m_sPerk.dCoolTime = 60.0;
	}
	m_dStartTime = m_sPerk.dCoolTime;
	m_dDuringTime = m_sPerk.dDuringTime;
	m_dCoolTime = m_sPerk.dCoolTime;
}

void CPerk::Create_SlasherPerk(_uint iIndex) {
	switch (iIndex)
	{
	case 0://LOCK_GENERATOR
		m_sPerk.eType = TYPE::ACTIVE;
		m_sPerk.dDuringTime = 10.;
		m_sPerk.dCoolTime = 60.0;
		break;
	case 1://MORI_CAMPER
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	case 2://FIND_CAMPER
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dCoolTime = 50.0;
		break;
	case 3://FIND_HEALCAMPER
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dDuringTime = 3.0;
		m_sPerk.dCoolTime = 50.0;
		break;
	case 4://FIND_EXITCAMPER
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dDuringTime = 5.0;
		m_sPerk.dCoolTime = 9999.0;
		break;
	case 5://KICK_GENERATOR
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dDuringTime = 60.0;
		m_sPerk.dCoolTime = 120.0;
		break;
	case 6://FIND_LOCKERCAMPER
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dCoolTime = 40.0;
		break;
	case 7://FIND_CHESTBOXCAMPER
		m_sPerk.eType = TYPE::PASSIVE;
		m_sPerk.dCoolTime = 40.0;
		break;
	case 8://TOTEM_SPEED
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	case 9://TOTEM_FIND
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	case 10://ADD_AXE
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	case 11://RESCUE_HOOK
		m_sPerk.eType = TYPE::PASSIVE;
		break;
	}
	m_dStartTime = m_sPerk.dCoolTime;
	m_dDuringTime = m_sPerk.dDuringTime;
	m_dCoolTime = m_sPerk.dCoolTime;
}


