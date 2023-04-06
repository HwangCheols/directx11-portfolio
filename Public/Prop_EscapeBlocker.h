#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CProp_EscapeBlocker final : public CObject {
public:
	static CProp_EscapeBlocker* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CProp_EscapeBlocker(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CProp_EscapeBlocker(const CProp_EscapeBlocker& rhs);
	virtual ~CProp_EscapeBlocker(void) override = default;
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
	_bool	m_bRender = true;
	_float m_fDissolveLength = 0.f;
	_float3 m_fDissolveColor = { 8.f, 0.f,0.f };
	_float3 m_fDissolveColor2 = { 8.f, 1.5f,0.f };
	_float m_fGoalDissolveLength = 0.f;
};
END_NAMESPACE