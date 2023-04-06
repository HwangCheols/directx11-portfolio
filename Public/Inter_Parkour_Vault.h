#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Parkour_Vault final : public CObject {
public:
	static CInter_Parkour_Vault* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Parkour_Vault(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Parkour_Vault(const CInter_Parkour_Vault& rhs);
	virtual ~CInter_Parkour_Vault(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init_Create(void);
	const HRESULT Init_Clone(void* const& pArg = nullptr);
	virtual const _ubyte Update(const _double& dTimeDelta) override;
	virtual void Update_Edit(void) override;
	virtual void Late_Update(const _double& dTimeDelta) override;
	virtual const HRESULT Render(const CRenderer::EOrder& eOrder) override;
public:
	virtual const HRESULT Save_Data(Json::Value& root) override;
	virtual void Load_Data(Json::Value& root) override;
private:
	class CTransform* m_pTransform = nullptr;
	class CRenderer* m_pRenderer = nullptr;
	class CCollider* m_pCollider = nullptr;
	class CShader* m_pShader = nullptr;
	class CModel* m_pModel = nullptr;

	class CTransform* m_pTransform_F = nullptr;
	class CCollider* m_pCollider_F = nullptr;
	class CTransform* m_pTransform_B = nullptr;
	class CCollider* m_pCollider_B = nullptr;
private:
	list<class CObject*>* m_plistCamper = nullptr;

};
END_NAMESPACE