#pragma once
#include "Object.h"
#include "Light_Police.h"

BEGIN_NAMESPACE(Client)
class CProp_Lobby_PoliceCar final : public CObject {
public:
	static CProp_Lobby_PoliceCar* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CProp_Lobby_PoliceCar(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CProp_Lobby_PoliceCar(const CProp_Lobby_PoliceCar& rhs);
	virtual ~CProp_Lobby_PoliceCar(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init_Create(void);
	const HRESULT Init_Clone(void* const& pArg = nullptr);
	virtual const _ubyte Update(const _double& dTimeDelta) override;
	virtual void Update_Edit(void) override;
	virtual void Late_Update(const _double& dTimeDelta) override;
	virtual const HRESULT Render(const CRenderer::EOrder& eOrder) override;
public:
	virtual void Update_Network(const string& pkey) override;
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
	_uint test = 0;
	_double m_dTime = 0.;
	char m_szModel[MAX_PATH] = "";
	_bool	m_bCreate = false;
private:
	_double m_dBlinkTime = 0.;
	_bool	m_bBlink = false;
	CObject* m_pBlueLight = nullptr;
	CObject* m_pRedLight = nullptr;
private:
	_float m_fRoughness = 0.279f;
	_float m_fRefract = 0.279f;
	_float m_fMetalness = 0.294f;
	_float2 m_fGlow = { 0.5f,1.f };

};
END_NAMESPACE