#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Hatch final : public CObject {
public:
	enum class ESTATE : _byte { CLOSE_LOOP, OPEN, OPEN_LOOP, CLOSE_SLASHER, END };
public:
	static CInter_Hatch* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Hatch(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Hatch(const CInter_Hatch& rhs);
	virtual ~CInter_Hatch(void) override = default;
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
public:
	virtual const HRESULT Save_Data(Json::Value& root) override;
	virtual void Load_Data(Json::Value& root) override;
private:
	class CTransform* m_pTransform = nullptr;
	class CRenderer* m_pRenderer = nullptr;
	class CCollider* m_pCollider = nullptr;
	class CShader* m_pShader = nullptr;
	class CModel* m_pModel = nullptr;

	class CTransform* m_pTransform_Slasher = nullptr;
	class CCollider* m_pCollider_Slasher = nullptr;
private:
	ESTATE m_eState = ESTATE::END;
	ESTATE m_ePreState = m_eState;

	_uint m_iAnimFinish = 0;
	_bool m_bLoop = true;
	_bool m_bOpen = false;
	_bool m_bCamper = true;
	//_bool m_b
private:
	char m_szModel[MAX_PATH] = "";
private:
	class CEffect_Free3* pEffect[2] = { nullptr };
};
END_NAMESPACE