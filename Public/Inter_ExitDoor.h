#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_ExitDoor final : public CObject {
public:
	enum class ESTATE : _byte { LOCK, CLOSED, GEAR, OPENING, KILLER_OPEN, END };
public:
	static CInter_ExitDoor* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_ExitDoor(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_ExitDoor(const CInter_ExitDoor& rhs);
	virtual ~CInter_ExitDoor(void) override = default;
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
	void Set_State(const ESTATE& eState, const _bool& bNetwork);
public:
	const _float& Get_Gauge(void);
	const void Add_Gauge(const _float& fGauge);
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
	class CAudio* m_pAudio = nullptr;

	class CTransform* m_pColTransform = nullptr;
	class CCollider* m_pCollider2 = nullptr;

	class CCollider* m_pCollider_Door = nullptr;
private:
	_uint		m_iAnimFinish = 0;
	_uint		m_iAnimNum = 0;
	ESTATE		m_eState = ESTATE::END;
	ESTATE		m_ePreState = m_eState;

	_float m_fGauge = 0.f;
	_float m_fAddGauge = 0.f;

	_bool		m_bLoop = true;
	_bool		m_bOpen = true;
private:
	char m_szModel[MAX_PATH] = "";
};
END_NAMESPACE