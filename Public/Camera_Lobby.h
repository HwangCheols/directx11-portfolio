#pragma once
#include "Camera.h"

BEGIN_NAMESPACE(Client)
class CCamera_Lobby final : public CCamera {
public:
	enum class ECAMSTATE {
		NORMAL, START_FIRST, SELECT_SLASHER, START_CAMPER, SELECT_CAMPER,
		SELECT_CAMPER_TO_START_CAMPER, START_CAMPER_TO_FIRST, START_SLASHER_TO_FIRST, END
	};
	static const _char* ECamState_ToString(const ECAMSTATE& eType) {
		switch (eType)
		{
		case ECAMSTATE::NORMAL: return "NORMAL";
		case ECAMSTATE::START_FIRST: return "STATRT_FIRST";
		case ECAMSTATE::SELECT_SLASHER: return "SELECT_SLASHER";
		case ECAMSTATE::START_CAMPER: return "START_CAMPER";
		case ECAMSTATE::SELECT_CAMPER: return "SELECT_CAMPER";
		case ECAMSTATE::SELECT_CAMPER_TO_START_CAMPER: return "SELECT_CAMPER_TO_START_CAMPER";
		case ECAMSTATE::START_CAMPER_TO_FIRST: return "START_CAMPER_TO_FIRST";
		case ECAMSTATE::START_SLASHER_TO_FIRST: return "START_SLASHER_TO_FIRST";
		}
		return nullptr;
	}
public:
	static CCamera_Lobby* const Create(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	virtual CObject* const Clone(void* const& pArg = nullptr) override;
private:
	explicit CCamera_Lobby(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext, class CNetwork* const& pNetwork);
	explicit CCamera_Lobby(const CCamera_Lobby& rhs);
	virtual ~CCamera_Lobby(void) override = default;
	virtual void Delete(void) override;
public:
	const HRESULT Init_Create(void);
	const HRESULT Init_Clone(void* const& pArg = nullptr);
	virtual const _ubyte Update(const _double& dTimeDelta) override;
	virtual void Update_Edit(void) override;
	virtual void Late_Update(const _double& dTimeDelta) override;
	virtual const HRESULT Render(const CRenderer::EOrder& eOrder) override;
public:
	const _bool& Get_IsFinished() { return m_bIsFinished; }
	const _bool& Get_IsMoveEnd() { return m_bCameraMoveEnd; }
	const ECAMSTATE& Get_CamState() { return m_eCamState; };

	void Set_State(const ECAMSTATE& eState);
	void Set_Group(const ECAMSTATE& eOffState, const ECAMSTATE& eOnState = ECAMSTATE::END);
	vector<class CUI_Image*>* Get_Group(const _uint& iState);
	void Add_LobbyBtn(const ECAMSTATE& eState, class CUI_Image* pBtn);
	void Add_UIGroup(const ECAMSTATE& eState, class CUI_Image* pImage, const _bool& bActive = false);

	const _bool& Get_CameraCamperRender(void);
public:
	const HRESULT Save_Data();
	void Load_Data();
private:
	ECAMSTATE m_eCamState = ECAMSTATE::START_FIRST;
	ECAMSTATE m_ePreCamState = ECAMSTATE::START_FIRST;
	_float	m_fSpeed = 10.0f;

	_matrix m_FirstSourMatrix, m_FirstDestMatrix;
	_matrix m_StartSlasherMatrix;
	_matrix m_StartCamperMatrix;
	_matrix m_SelectSlasherMatrix;
	_matrix m_SelectCamperMatrix;

	_bool	m_bClick = false;
	_bool	m_bIsFinished = true;

	_double m_dTime = 0.0;
	_double m_dFirstLerpTime = 0.0;
	_double m_dStartSlasherLerpTime = 0.0;
	_double m_dStartCamperLerpTime = 0.0;
	_double m_dSelectSlasherLerpTime = 0.0;
	_double m_dSelectCamperLerpTime = 0.0;

	class CUI_Image* m_pLobbyBtns[static_cast<_uint>(ECAMSTATE::END)] = { nullptr, };
	vector<class CUI_Image*> m_vecUIGroups[static_cast<_uint>(ECAMSTATE::END)];
	ECAMSTATE m_eOnGroup = ECAMSTATE::START_FIRST;
private:
	_bool m_bCameraMoveEnd = false;
	_bool m_bCameraCamperRender = false;
};
END_NAMESPACE