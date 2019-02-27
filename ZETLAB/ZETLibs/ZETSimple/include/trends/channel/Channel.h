#pragma once

#include <rpc.h>
#include <string>

class CChannel
{
public:
	CChannel(void);
	~CChannel(void);

	std::wstring GetName() const {return m_sName;};
	void SetName(std::wstring sName){ m_sName = sName; };

	std::wstring GetComment() const { return m_sComment; };
	void SetComment(std::wstring sComment){ m_sComment = sComment; };

	double GetFrequency() const {return m_dFrequency;};
	void SetFrequency(double dFrequency){m_dFrequency = dFrequency;};

	float GetMinLevel() const {return m_fMinlevel;};
	void SetMinLevel(float fMinlevel){m_fMinlevel = fMinlevel;};

	float GetMaxLevel() const {return m_fMaxlevel;};
	void SetMaxLevel(float fMaxlevel){m_fMaxlevel = fMaxlevel;};

	float GetSense() const {return m_fSense;};
	void SetSense(float fSense){m_fSense = fSense;};

	float GetReference() const {return m_fReference;};
	void SetReference(float fReference){m_fReference = fReference;};

	float GetShift() const {return m_fShift;};
	void SetShift(float fShift){m_fShift = fShift;};

	std::wstring GetConversion() const { return m_sConversion; };
	void SetConversion(std::wstring sConversion){ m_sConversion = sConversion; };

	int GetNumber() const {return m_iChannel;};
	void SetNumber(int iChannel){m_iChannel = iChannel;};

	int GetTypeADC() const {return m_iTypeAdc;};
	void SetTypeADC(int iTypeAdc){m_iTypeAdc = iTypeAdc;};

	int GetNumberDSP() const {return m_iNumberDSP;};
	void SetNumberDSP(int iNumberDSP){m_iNumberDSP = iNumberDSP;};

	GUID GetID() const {return m_ID;};
	void SetID(GUID ID){m_ID = ID;};

	int GetType() const {return m_iType;};
	void SetType(int iType){m_iType = iType;};

	std::wstring GetGroupName() const { return m_sGroupName; };
	void SetGroupName(std::wstring sGroupName){ m_sGroupName = sGroupName; };

protected:
	std::wstring m_sName;
	std::wstring m_sComment;
	double m_dFrequency;
	float m_fMinlevel;
	float m_fMaxlevel;
	float m_fSense;
	float m_fReference;
	float m_fShift;
	std::wstring m_sConversion;
	int m_iChannel;
	int m_iTypeAdc;
	int m_iNumberDSP;
	GUID m_ID;
	int m_iType;
	std::wstring m_sGroupName;
};

class CChannelEx : public CChannel
{
public:
	CChannelEx(void) : m_sFileName(L""){};
	~CChannelEx(void){};

	std::wstring GetFileName(){ return m_sFileName; };
	void SetFileName(std::wstring sFileName){ m_sFileName = sFileName; };

protected:
	std::wstring m_sFileName;
};

