#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Locker final : public CObject {
public:
	enum class ESTATE : _byte {
		IDLE, OPEN_SLASHER, TAKEOUT_WEAPON, TAKEOUT_CAMPER,
		HIDE_CAMPER, HIDEFAST_CAMPER, COMEOUT_CAMPER, FULLSWING, PEEKING_LOCKE, END // 추가로 생존자가 들어가고, 대기 상태, 혼자 나올때
	};//Closet01_DoorFullSwing,
public:
	static CInter_Locker* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Locker(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Locker(const CInter_Locker& rhs);
	virtual ~CInter_Locker(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init_Create(void);
	const HRESULT Init_Clone(void* const& pArg = nullptr);
	virtual const _ubyte Update(const _double& dTimeDelta) override;
	virtual void Update_Edit(void) override;
	virtual void Late_Update(const _double& dTimeDelta) override;
	virtual const HRESULT Render(const CRenderer::EOrder& eOrder) override;
public:
	const ESTATE& Get_State(void) const {
		return m_eState;
	}
	//const _bool& Get_IsInCamper(void) { return m_bIsInCamper; }
	//void Set_IsInCamper(const _bool& bIsInCamper) { m_bIsInCamper = bIsInCamper; }

	const _int& Get_CamperID(void);
	void Set_CamperID(const _int& iCamperID);

	void Set_State(ESTATE eState) {
		m_eState = eState;
		Update_Network("Set_State");
	}

public:
	virtual void Update_Network(const string& pKey) override;
	virtual void Update_Network(void) override;
public:
	virtual const HRESULT Save_Data(Json::Value& root) override;
	virtual void Load_Data(Json::Value& root) override;
private:
	class CTransform* m_pTransform = nullptr;
	class CRenderer* m_pRenderer = nullptr;
	class CCollider* m_pCollider = nullptr;

	class CShader* m_pShader = nullptr;
	class CModel* m_pModel = nullptr;

	class CTransform* m_pTransformAxe[3] = { nullptr };
private://camper
	class CTransform* m_pColTransform = nullptr;
	class CTransform* m_pColTransform_Grab = nullptr;
	class CCollider* m_pCollider2 = nullptr;
	class CCollider* m_pCollider_Grab = nullptr;
	class CAudio* m_pAudio = nullptr;
private:
	class CModel* m_pModelAxe[3] = { nullptr };

	class CTransform* m_pTransform_Slasher = nullptr;
	class CCollider* m_pCollider_Slasher = nullptr;
private:
	_uint		m_iAnimFinish = 0;
	ESTATE		m_eState = ESTATE::END;
	ESTATE		m_ePreState = m_eState;
	ESTATE		m_ePreState2 = m_eState;

	//_bool		m_bIsInCamper = false;
	_bool		m_bLoop = false;
	_bool		m_bLoopAxe = true;
	_bool		m_bRenderAxe = false;

	_float3		m_fRootPrePos = { 0.f,0.f,0.f };
	_float4x4	m_fRootOffsetMatrix;

	_int		m_iCamperID = -1;
private:
	char m_szModel[MAX_PATH] = "";
private:
	class CCamper_GamePlay* m_pCamper = nullptr;
	class CSlasher_Huntress* m_pSlasher = nullptr;
};
END_NAMESPACE