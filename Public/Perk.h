#pragma once
#include "Skill.h"

BEGIN_NAMESPACE(Client)
class CPerk final : public CSkill {
public:
	enum class TYPE { ACTIVE, PASSIVE, END };
	typedef struct PerkDesc {
		TYPE eType;
		_double dDuringTime;
		_double dCoolTime;
	}PERK;
public:
	static CPerk* const Create(_bool bIsCamper,_uint iIndex);
	virtual CSkill* const Clone(void* pArg) override;
private:
	explicit CPerk(void);
	virtual ~CPerk(void) override = default;
	virtual void Delete(void) override;
private:
	const HRESULT Init_Create(_bool bIsCamper,_uint iIndex);
	const HRESULT Init_Clone(void* pArg);
public:
	virtual void Push() override;
	void Update(_double dTimeDelta);
public:
	const PERK& Get_PerkInfo();
	const _bool& Get_IsUsePerk() const;
	const _double& Get_StartTime() const;
	const _double& Get_DuringTIme() const;
	const _double& Get_CoolTime() const;
	_bool ReadyStart();
	void Set_Start();
private:
	void Use_Perk();
private:
	void Create_CamperPerk(_uint iIndex);
	void Create_SlasherPerk(_uint iIndex);
private:
	PERK m_sPerk;
	_double m_dStartTime = 0.0;
	_double m_dDuringTime = 0.0;
	_double m_dCoolTime = 0.0;
	_bool m_bIsTrigger = false;
};
END_NAMESPACE