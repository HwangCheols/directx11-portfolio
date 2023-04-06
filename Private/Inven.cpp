#include "pch.h"
#include "Inven.h"


IMPLEMENT_SINGLETON(CInven)

CInven::CInven(void)
	:CBase() {
}

void CInven::Delete(void) {
	for (_uint i = 0; i < m_byType; ++i) {
		for (auto& iter : m_pvecPerk[i])
			Safe_Release(iter.pPerk);
		m_pvecPerk[i].clear();
	}
	Safe_Delete_Array(m_pvecPerk);
}

const HRESULT CInven::Init(void) {
	return S_OK;
}

void CInven::Update(void) {
	
}

void CInven::Update(_ubyte uPerkType, _double dTimeDelta) {
	if (m_pvecPerk[uPerkType].size() != 0) {
		for (_uint i = 0; i < 4; ++i) {
			if (m_pvecPerk[uPerkType][i].pPerk != nullptr)
				m_pvecPerk[uPerkType][i].pPerk->Update(dTimeDelta);
		}
	}
}

const _uint& CInven::Get_HandleNum() {
	return m_iHandle;
}

void CInven::Set_HandleNum(_uint iIndex) {
	m_iHandle = iIndex;
}

const CPerk::PERK& CInven::Get_PerkSlot(_ubyte uPerkType, _uint iIndex) {
	return m_pvecPerk[uPerkType][iIndex].pPerk->Get_PerkInfo();
}

const _uint& CInven::Get_PerkSlotIndex(_ubyte uPerkType, _uint iIndex) {
	if (OkayPerk(uPerkType) == false)
		return m_iFailIndex;
	return m_pvecPerk[uPerkType][iIndex].iIndex;
}

CPerk* CInven::Get_Perk(_ubyte uPerkType, _uint iIndex) {
	if (OkayPerk(uPerkType) == true) {
		for (_uint i = 0; i < 4; ++i) {
			if (m_iPerkSlot[i] == iIndex)
				return m_pvecPerk[uPerkType][i].pPerk;
		}
	}
	return nullptr;
}

const _double& CInven::Get_PerkDuration(_ubyte uPerkType, _uint iIndex) {
	if (OkayPerk(uPerkType) == true) {
		for (_uint i = 0; i < 4; ++i) {
			if (m_iPerkSlot[i] == iIndex)
				return m_pvecPerk[uPerkType][i].pPerk->Get_DuringTIme();
		}
	}
	return m_dFailTime;
}

const _bool& CInven::OkayPerk(_ubyte uPerkType) {
	if (m_pvecPerk == nullptr)
		m_bOkayPerk = false;
	else
		if (m_pvecPerk[uPerkType].size() <= 0)
			m_bOkayPerk = false;
		else
			m_bOkayPerk = true;
	return m_bOkayPerk;
}

class CPerk* CInven::Ready_UIPerk(_ubyte uPerkType, _uint iIndex) {
	if (OkayPerk(uPerkType) == true) {
		return m_pvecPerk[uPerkType][iIndex].pPerk;
	}
	return nullptr;
}

_bool CInven::Use_Perk(_ubyte uPerkType, _uint iIndex) {
	if (Have_Perk(uPerkType, iIndex) == true) {
		CPerk* pPerk = Get_Perk(uPerkType, iIndex);
		if (nullptr != pPerk) {
			if (pPerk->ReadyStart() == true) {
				pPerk->Set_Start();
				return true;
			}
		}
	}
	return false;
}

_bool CInven::Have_Perk(_ubyte uPerkType, _uint iIndex) {
	if (OkayPerk(uPerkType) == true) {
		for (_uint i = 0; i < 4; ++i) {
			if (m_pvecPerk[uPerkType][i].pPerk != nullptr) {
				if (m_pvecPerk[uPerkType][i].iIndex == iIndex)
					return true;
			}
		}
	}
	return false;
}

void CInven::Ready_Perk_Camper() {
	if (m_pvecPerk == nullptr) { 
		m_byType = static_cast<_ubyte>(INVENTYPE::END);
		m_pvecPerk = new vector<INVEN>[m_byType];
		INVEN sInven;
		ZeroMemory(&sInven, sizeof(INVEN));
		sInven.eInven = INVENTYPE::PERKS_CAMPER;
		for (_uint i = 0; i < 4; ++i) {
			sInven.iIndex = m_iPerkSlot[i];
			sInven.pPerk = CPerk::Create(true, sInven.iIndex);
			m_pvecPerk[static_cast<_ubyte>(INVENTYPE::PERKS_CAMPER)].emplace_back(sInven);
		}
	}
	else {
		for (_uint i = 0; i < m_byType; ++i) {
			for (auto& iter : m_pvecPerk[i])
				Safe_Release(iter.pPerk);
			m_pvecPerk[i].clear();
		}
		Safe_Delete_Array(m_pvecPerk);

		m_byType = static_cast<_ubyte>(INVENTYPE::END);
		m_pvecPerk = new vector<INVEN>[m_byType];
		INVEN sInven;
		ZeroMemory(&sInven, sizeof(INVEN));
		sInven.eInven = INVENTYPE::PERKS_CAMPER;
		for (_uint i = 0; i < 4; ++i) {
			sInven.iIndex = m_iPerkSlot[i];
			sInven.pPerk = CPerk::Create(true, sInven.iIndex);
			m_pvecPerk[static_cast<_ubyte>(INVENTYPE::PERKS_CAMPER)].emplace_back(sInven);
		}
	}
}

void CInven::Ready_Perk_Slasher() {
	if (m_pvecPerk == nullptr) {
		m_byType = static_cast<_ubyte>(INVENTYPE::END);
		m_pvecPerk = new vector<INVEN>[m_byType];
		INVEN sInven;
		ZeroMemory(&sInven, sizeof(INVEN));
		sInven.eInven = INVENTYPE::PERKS_SLASHER;
		for (_uint i = 0; i < 4; ++i) {
			sInven.iIndex = m_iPerkSlot[i];
			sInven.pPerk = CPerk::Create(false, sInven.iIndex);
			m_pvecPerk[static_cast<_ubyte>(INVENTYPE::PERKS_SLASHER)].emplace_back(sInven);
		}
	}
	else {
		for (_uint i = 0; i < m_byType; ++i) {
			for (auto& iter : m_pvecPerk[i])
				Safe_Release(iter.pPerk);
			m_pvecPerk[i].clear();
		}
		Safe_Delete_Array(m_pvecPerk);

		m_byType = static_cast<_ubyte>(INVENTYPE::END);
		m_pvecPerk = new vector<INVEN>[m_byType];
		INVEN sInven;
		ZeroMemory(&sInven, sizeof(INVEN));
		sInven.eInven = INVENTYPE::PERKS_SLASHER;
		for (_uint i = 0; i < 4; ++i) {
			sInven.iIndex = m_iPerkSlot[i];
			sInven.pPerk = CPerk::Create(false, sInven.iIndex);
			m_pvecPerk[static_cast<_ubyte>(INVENTYPE::PERKS_SLASHER)].emplace_back(sInven);
		}
	}
}


