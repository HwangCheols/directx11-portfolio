#pragma once
#include "UI_Image.h"

BEGIN_NAMESPACE(Client)
class CUI_Loading_Bone2 final : public CUI_Image
{
public:
	enum class EType { ONE, TWO, THREE, FOUR, FIVE, END };
	const _char* EType_ToString(const EType& eType) {
		switch (eType)
		{
		case Client::CUI_Loading_Bone2::EType::ONE: return "ONE";
		case Client::CUI_Loading_Bone2::EType::TWO: return "TWO";
		case Client::CUI_Loading_Bone2::EType::THREE: return "THREE";
		case Client::CUI_Loading_Bone2::EType::FOUR: return "FOUR";
		case Client::CUI_Loading_Bone2::EType::FIVE: return "FIVE";
		}
		return nullptr;
	}
public:
	static CUI_Loading_Bone2* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CUI_Loading_Bone2(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CUI_Loading_Bone2(const CUI_Loading_Bone2& rhs);
	virtual ~CUI_Loading_Bone2(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init_Create(void);
	const HRESULT Init_Clone(void* const& pArg = nullptr);
	virtual const _ubyte Update(const _double& dTimeDelta) override;
	virtual void Update_Edit(void) override;
	virtual void Late_Update(const _double& dTimeDelta) override;
	virtual const HRESULT Render(const CRenderer::EOrder& eOrder) override;
private:
	CTexture* m_pBoneTexture[5] = { nullptr };
	CRectTransform* m_pBoneRectTransform[5] = {nullptr};
private:
	_float m_fLoadingTime = 0.f;
	_float m_fRatio = 0.0f;
	_bool m_bStartCoolTime = false;
	_int m_iType = -1;
};
END_NAMESPACE

