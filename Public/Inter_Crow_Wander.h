#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Crow_Wander final : public CObject {
public:
	enum class ESTATE : _byte {WANDER, END };
public:
	static CInter_Crow_Wander* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Crow_Wander(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Crow_Wander(const CInter_Crow_Wander& rhs);
	virtual ~CInter_Crow_Wander(void) override = default;
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
	void Set_State(ESTATE eState,_vector vPos) {
		m_eState = eState;
		m_pTransform->Set_Row(CTransform::ERow::POS, vPos);
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
	class CShader* m_pShader = nullptr;
	class CModel* m_pModel = nullptr;
private:
	_uint		m_iAnimFinish = 0;
	_float		m_fCosf = 0.f, m_fSinf = 0.f;
	_float		m_fAngle = 0.f;
	_double		m_dTime = 0.0;
	_bool		m_bRender = true;
	_bool		m_bLoop = true;
	_vector		m_vTargetPos;
	ESTATE		m_eState = ESTATE::WANDER;
	ESTATE		m_ePreState = ESTATE::END;
private:
	char m_szModel[MAX_PATH] = "";
};
END_NAMESPACE