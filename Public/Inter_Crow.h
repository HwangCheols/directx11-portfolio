#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Crow final : public CObject {
public:
	enum class ESTATE : _byte { IDLE, FLY, LAND, END };
public:
	static CInter_Crow* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Crow(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Crow(const CInter_Crow& rhs);
	virtual ~CInter_Crow(void) override = default;
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
	_double		m_dTime = 0.0;
	ESTATE		m_eState = ESTATE::END;
	ESTATE		m_ePreState = m_eState;
	_bool		m_bRender = false;
	_bool		m_bLoop = false;
private:
	char m_szModel[MAX_PATH] = "";

	list<class CObject*>* m_plistObject = nullptr;
	class CSlasher_Huntress* m_pSlasher = nullptr;
};
END_NAMESPACE