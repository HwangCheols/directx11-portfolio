#pragma once

BEGIN_NAMESPACE(Client) 
class CSkill abstract : public CBase{
public:
	enum class TYPE{ POWER, ADD_ONS, OFFERING, PERKS, END };
public:
	virtual CSkill* const Clone(void* pArg = nullptr) PURE;
protected:
	explicit CSkill(void);
	virtual ~CSkill(void) override = default;
	virtual void Delete(void) {};
protected:
	virtual void Push() {};
};
END_NAMESPACE