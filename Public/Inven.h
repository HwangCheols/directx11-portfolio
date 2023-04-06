#pragma once
#include "Perk.h"

BEGIN_NAMESPACE(Client)
class CInven final : public CBase {
DECLARE_SINGLETON(CInven)
public:
	enum class INVENTYPE : unsigned char {PERKS_CAMPER,PERKS_SLASHER, END};
	enum class PERK_CAMPER {FIND_CAMPER,SELF_HEAL,RUNAWAY,ESCAPE_CARRY,
		LOCKER,STAY_CROW,END_GENERATOR,FIND_SLASHER,
		FAST,NEWBIE,PURIFICATION,FIND_GENERATOR,END};
	enum class PERK_SLASHER {LOCK_GENERATOR,MORI_CAMPER,FIND_CAMPER,FIND_HEALCAMPER,
		FIND_EXITCAMPER,KICK_GENERATOR, FIND_LOCKERCAMPER,FIND_CHESTBOXCAMPER,
		TOTEM_SPEED,TOTEM_FIND,ADD_AXE,RESCUE_HOOK,END};
	typedef struct tagInvenDesc {
		INVENTYPE eInven = INVENTYPE::END;			
		_uint iIndex = 9999;							
		class CPerk* pPerk = nullptr;
	}INVEN;
private:
	explicit CInven(void);
	virtual ~CInven(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init(void);
	void Update(void);
	void Update(_ubyte uPerkType, _double dTimeDelta);
public:
	const _uint& Get_HandleNum();
	void Set_HandleNum(_uint iIndex);
	const CPerk::PERK& Get_PerkSlot(_ubyte uPerkType, _uint iIndex);
	const _uint& Get_PerkSlotIndex(_ubyte uPerkType, _uint iIndex);
	class CPerk* Get_Perk(_ubyte uPerkType, _uint iIndex);
	const _double& Get_PerkDuration(_ubyte uPerkType, _uint iIndex);
	const _bool& OkayPerk(_ubyte uPerkType);
	void Set_PerkSlot(const _uint& iSlot, const _uint& iPerk) { 
		(9999 == iPerk) ? (m_iPerkSlot[iSlot] = 9999) : (m_iPerkSlot[iSlot] = iPerk);
	}
	const _uint& Get_PerkSlot(const _uint& iSlot) { return m_iPerkSlot[iSlot]; }
public:
	class CPerk* Ready_UIPerk(_ubyte uPerkType,_uint iIndex);
public:
	_bool Use_Perk(_ubyte uPerkType, _uint iIndex);
	_bool Have_Perk(_ubyte uPerkType, _uint iIndex);
	void Ready_Perk_Camper();
	void Ready_Perk_Slasher();

private:
	INVENTYPE m_ePreType = INVENTYPE::END;
	INVENTYPE m_eType = INVENTYPE::END;

private:
	_uint m_byType = 0;
	vector<INVEN>* m_pvecPerk = nullptr;
	_uint m_iHandle = 0;
	_uint m_iPerkSlotIndex = 9999;
	_uint m_iFailIndex = 9999;
	_bool m_bOkayPerk = false;
	_double m_dFailTime = 9999.9;
private:                                                                                  
	_uint m_iPerkSlot[4] { 9999, 9999, 9999, 9999 };
	_uint m_iSlotHandel = 0;
	_uint m_iIndex = 9999;
};
END_NAMESPACE