#pragma once
#include "Object.h"

BEGIN_NAMESPACE(Client)
class CCamper_Lobby final : public CObject {
	enum class ECamper : _ubyte { CHERYL, BILL, END };
public:
	static CCamper_Lobby* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CCamper_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CCamper_Lobby(const CCamper_Lobby& rhs);
	virtual ~CCamper_Lobby(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init_Create(void);
	const HRESULT Init_Clone(void* const& pArg = nullptr);
	virtual const _ubyte Update(const _double& dTimeDelta) override;
	virtual void Update_Edit(void) override;
	virtual void Late_Update(const _double& dTimeDelta) override;
	virtual const HRESULT Render(const CRenderer::EOrder& eOrder) override;
public:
	void Set_Dissolve(const _float& m_fGoal);
	void Set_Camper(const ECamper& eCamper);
	void Set_MyCamperSlot(const _uint& iSlot);
	void Create_Effect(void);
	const _float& Get_GoalDissolveLength(void);
public:
	virtual void Update_Network(const string& pKey) override;
	virtual void Update_Network(void) override;
private:
	class CTransform* m_pTransform = nullptr;
	class CRenderer* m_pRenderer = nullptr;
	class CShader* m_pShader = nullptr;
	class CModel* m_pModel_Cheryl = nullptr;
	class CModel* m_pModel_Bill = nullptr;
private:
	ECamper m_eCamper = ECamper::END;
	_bool m_bRender = true;

	_float m_fDissolveLength = 0.f;
	_float3 m_fDissolveColor = { 8.f, 0.f,0.f };
	_float3 m_fDissolveColor2 = { 8.f, 1.5f,0.f };
	_float m_fGoalDissolveLength = 0.f;
private:
	_float4x4 m_Float4x4;
	_uint m_iMyCamperSlot = 0;
};
END_NAMESPACE