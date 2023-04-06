#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Totem final : public CObject {
public:
	enum class ESTATE : _byte { IDLE, BREAK, ON, END };
public:
	static CInter_Totem* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Totem(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Totem(const CInter_Totem& rhs);
	virtual ~CInter_Totem(void) override = default;
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
	void Set_State(ESTATE eState) {
		m_eState = eState;
		Update_Network("Set_State");
	}
public:
	const _float& Get_Gauge(void);
	const void Add_Gauge(const _float& fGauge);
public:
	virtual void Update_Network(const string& pKey) override;
	virtual void Update_Network(void) override;
private:
	class CTransform* m_pTransform = nullptr;
	class CRenderer* m_pRenderer = nullptr;
	class CCollider* m_pCollider = nullptr;
	class CShader* m_pShader = nullptr;
	class CModel* m_pModel = nullptr;
private:
	ESTATE		m_eState = ESTATE::END;
	ESTATE		m_ePreState = m_eState;
private:
	char m_szModel[MAX_PATH] = "";
	_float m_fAddGauge = 0.f;
	_float m_fGauge = 0.f;

};
END_NAMESPACE