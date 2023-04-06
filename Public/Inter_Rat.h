#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Rat final : public CObject {
public:
	enum class ESTATE : _byte { IDLE, WALK, RUN, END };
public:
	static CInter_Rat* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Rat(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Rat(const CInter_Rat& rhs);
	virtual ~CInter_Rat(void) override = default;
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
private:
	_uint		m_iAnimFinish = 0;
	_uint		m_iAnimNum = 0;
	_double		m_dTime = 0.0;
	_vector		m_vPlayerLook;
	ESTATE		m_eState = ESTATE::END;
	ESTATE		m_ePreState = m_eState;
	_bool		m_bLoop = false;
	_bool		m_bCamper = true;
private:
	char m_szModel[MAX_PATH] = "";
private:
	list<class CObject*>* m_plistCamper = nullptr;
	class CSlasher_Huntress* m_pSlasher = nullptr;

};
END_NAMESPACE