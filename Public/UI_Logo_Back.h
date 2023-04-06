#pragma once
#include "UI_Image.h"

BEGIN_NAMESPACE(Client)
class CUI_Logo_Back final : public CUI_Image
{
public:
	enum class EType { NORMAL, FADE, END };
	const _char* EType_ToString(const EType& eType) {
		switch (eType)
		{
		case Client::CUI_Logo_Back::EType::NORMAL: return "NORMAL";
		case Client::CUI_Logo_Back::EType::FADE:return "FADE";
		}
		return nullptr;
	}
public:
	static CUI_Logo_Back* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CUI_Logo_Back(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CUI_Logo_Back(const CUI_Logo_Back& rhs);
	virtual ~CUI_Logo_Back(void) override = default;
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
	_int m_iType = 0;
};
END_NAMESPACE

