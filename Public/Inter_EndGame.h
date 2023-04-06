#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_EndGame final : public CObject {
public:
	enum class ESTATE : _byte { APPEAR, IDLE, END };
public:
	static CInter_EndGame* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_EndGame(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_EndGame(const CInter_EndGame& rhs);
	virtual ~CInter_EndGame(void) override = default;
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

	_int m_iCamperID = -1;
	
	_bool		m_bLoop = false;
};
END_NAMESPACE