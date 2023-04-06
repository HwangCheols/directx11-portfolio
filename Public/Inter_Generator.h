#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Generator final : public CObject {
public:
	enum class EState : _byte {
		OFF, ONE, TWO, THREE, FOUR, ON, CRASH, LOCK_IN, LOCK_IDLE, LOCK_END, END
	};
public:
	static CInter_Generator* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Generator(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Generator(const CInter_Generator& rhs);
	virtual ~CInter_Generator(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init_Create(void);
	const HRESULT Init_Clone(void* const& pArg = nullptr);
	virtual const _ubyte Update(const _double& dTimeDelta) override;
	virtual void Update_Edit(void) override;
	virtual void Late_Update(const _double& dTimeDelta) override;
	virtual const HRESULT Render(const CRenderer::EOrder& eOrder) override;
public:
	const EState& Get_State(void) const;
	void Set_State(const EState& eState);
public:
	const _float& Get_Gauge(void);
	const void Add_Gauge(const _float& fGauge);
public:
	virtual void Update_Network(const string& pKey) override;
	virtual void Update_Network(void) override;
public:
	virtual const HRESULT Save_Data(Json::Value& root) override;
	virtual void Load_Data(Json::Value& root) override;

public:
	_bool Get_Exist(_uint i) { return m_bExist[i]; }
	void Set_Exist(_uint i, _bool isExist) { m_bExist[i] = isExist; }

	const _bool& Get_Flag(void);
	void Set_Flag(const _bool& bFlag);

	//hwang
	void Set_PerkRender(_bool bRender) { m_bPerkRender = bRender; }
private:
	class CTransform* m_pTransform = nullptr;
	class CTransform* m_pTransform2 = nullptr;
	class CRenderer* m_pRenderer = nullptr;
	class CCollider* m_pCollider = nullptr;
	class CShader* m_pShader = nullptr;
	class CModel* m_pModel = nullptr;
	class CAudio* m_pAudio = nullptr;
private:
	class CTransform* m_pTransform_F = nullptr;
	class CCollider* m_pCollider_F = nullptr;
	class CTransform* m_pTransform_B = nullptr;
	class CCollider* m_pCollider_B = nullptr;
	class CTransform* m_pTransform_L = nullptr;
	class CCollider* m_pCollider_L = nullptr;
	class CTransform* m_pTransform_R = nullptr;
	class CCollider* m_pCollider_R = nullptr;
private:
	class CTransform* m_pTransform_S_C = nullptr;
	class CCollider* m_pCollider_S_C = nullptr;
	class CTransform* m_pTransform_S_F = nullptr;
	class CCollider* m_pCollider_S_F = nullptr;
	class CTransform* m_pTransform_S_B = nullptr;
	class CCollider* m_pCollider_S_B = nullptr;
	class CTransform* m_pTransform_S_L = nullptr;
	class CCollider* m_pCollider_S_L = nullptr;
	class CTransform* m_pTransform_S_R = nullptr;
	class CCollider* m_pCollider_S_R = nullptr;
private:
	class CTransform* m_pTransform_GodRay = nullptr;
	class CCollider* m_pCollider_GodRay = nullptr;

	class CTransform* m_pTransform_Entity = nullptr;
	class CModel* m_pModel_Entity = nullptr;

	CTarget* m_pTarget_Depth = nullptr;
private:
	EState m_eState = EState::END;
	EState m_ePreState = m_eState;
	_float m_fGauge = 0.f;
	_float m_fAddGauge = 0.f;

	_bool m_bFlag = true;
	_bool m_bExist[4] = { 0, };
private:
	CLight* m_pLight = nullptr;
	class CEffect_Free3* m_pEffectCrash = nullptr;
	class CEffect_Free3* m_pEffectLockParticle = nullptr;
	class CEffect_Free3* m_pEffectLockSmoke = nullptr;
	_float m_fSuccesTime = 0.f;

	list<class CObject*>* m_plistCamper = nullptr;
	class CSlasher_Huntress* m_pSlasher = nullptr;
private:
	EState m_ePreLockState = m_ePreState;
	_bool m_bIsLock = false;
	_bool m_bIsLoop = false;
	_double m_dLockTime = 10.0;
	//hwang
	_bool m_bPerkRender = false;
	_float m_fSound_One = 0.f;
};
END_NAMESPACE