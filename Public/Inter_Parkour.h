#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CInter_Parkour final : public CObject {
public:
	static CInter_Parkour* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CInter_Parkour(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CInter_Parkour(const CInter_Parkour& rhs);
	virtual ~CInter_Parkour(void) override = default;
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

	class CTransform* m_pTransform_F = nullptr;
	class CCollider* m_pCollider_F = nullptr;
	class CTransform* m_pTransform_B = nullptr;
	class CCollider* m_pCollider_B = nullptr;

	class CTransform* m_pTransform_S_F = nullptr;
	class CCollider* m_pCollider_S_F = nullptr;
	class CTransform* m_pTransform_S_B = nullptr;
	class CCollider* m_pCollider_S_B = nullptr;
private:
	list<class CObject*>* m_plistCamper = nullptr;
	class CSlasher_Huntress* m_pSlasher = nullptr;
};
END_NAMESPACE