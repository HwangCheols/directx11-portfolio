#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CSlasher_Huntress_Lobby final : public CObject {
public:
	enum class ESTATE { WATCH, IDLE, IDLE_THROW, SELECT, END };
public:
	static CSlasher_Huntress_Lobby* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CSlasher_Huntress_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CSlasher_Huntress_Lobby(const CSlasher_Huntress_Lobby& rhs);
	virtual ~CSlasher_Huntress_Lobby(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init_Create(void);
	const HRESULT Init_Clone(void* const& pArg = nullptr);
	virtual const _ubyte Update(const _double& dTimeDelta) override;
	virtual void Update_Edit(void) override;
	virtual void Late_Update(const _double& dTimeDelta) override;
	virtual const HRESULT Render(const CRenderer::EOrder& eOrder) override;
public:
	const ESTATE& Get_State(void) const { return m_eState; }
	void Set_State(ESTATE eState) { m_eState = eState; }
	const _bool& Get_RenderWeapon(void) const { return m_bRenderWeapon; }
	void Set_Dissolve(const _float& m_fGoal);
public:
	virtual const HRESULT Save_Data(Json::Value& root) override;
	virtual void Load_Data(Json::Value& root) override;
public:
	virtual void Update_Network(const string& pKey) override;
	virtual void Update_Network(void) override;
private:
	class CTransform* m_pTransform = nullptr;
	class CTransform* m_pOrbitTransform = nullptr;
	class CRenderer* m_pRenderer = nullptr;
	class CShader* m_pShader = nullptr;
	class CModel* m_pModel = nullptr;

	class CTexture* m_pTexture_Dissolve = nullptr;
	class CTexture* m_pTexture_Alpha = nullptr;
private:
	_bool m_bRenderWeapon = false;
	_uint m_iAnimEnd = 0;
	_uint m_iAnimIndex = 0;
	ESTATE m_eState = ESTATE::WATCH;

	_float m_fDissolveLength = 0.f;
	_float3 m_fDissolveColor = { 8.f, 0.f,0.f };
	_float3 m_fDissolveColor2 = { 8.f, 1.5f,0.f };
	_float m_fGoalDissolveLength = 0.f;
private:
	_float m_fRoughness = 0.1f;
	_float m_fRefract = 0.5f;
	_float m_fMetalness = 0.f;

};
END_NAMESPACE