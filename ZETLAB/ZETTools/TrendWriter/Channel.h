#pragma once
#include <chaninfo.h>

class CChannel
{
public:
	CChannel(void);
	~CChannel(void);

	void operator = (const CHANNEL_STRUCT & structure);

	CString GetName(){return m_sName;};
	void SetName(CString sName){m_sName = sName;};

	CString GetComment(){return m_sComment;};
	void SetComment(CString sComment){m_sComment = sComment;};

	double GetFrequency(){return m_dFrequency;};
	void SetFrequency(double dFrequency){m_dFrequency = dFrequency;};

	float GetSubLevel(){ return m_fSublevel; };
	void SetSubLevel(float fSublevel){ m_fSublevel = fSublevel; };

	float GetMinLevel(){return m_fMinlevel;};
	void SetMinLevel(float fMinlevel){m_fMinlevel = fMinlevel;};

	float GetMaxLevel(){return m_fMaxlevel;};
	void SetMaxLevel(float fMaxlevel){m_fMaxlevel = fMaxlevel;};

	float GetSense(){return m_fSense;};
	void SetSense(float fSense){m_fSense = fSense;};

	float GetReference(){return m_fReference;};
	void SetReference(float fReference){m_fReference = fReference;};

	float GetShift(){return m_fShift;};
	void SetShift(float fShift){m_fShift = fShift;};

	CString GetConversion(){return m_sConversion;};
	void SetConversion(CString sConversion){m_sConversion = sConversion;};

	int GetNumber(){return m_iChannel;};
	void SetNumber(int iChannel){m_iChannel = iChannel;};

	int GetTypeADC(){return m_iTypeAdc;};
	void SetTypeADC(int iTypeAdc){m_iTypeAdc = iTypeAdc;};

	int GetNumberDSP(){return m_iNumberDSP;};
	void SetNumberDSP(int iNumberDSP){m_iNumberDSP = iNumberDSP;};

	GUID GetID(){return m_ID;};
	void SetID(GUID ID){m_ID = ID;};

	int GetType(){return m_iType;};
	void SetType(int iType){m_iType = iType;};

	CString GetGroupName(){return m_sGroupName;};
	void SetGroupName(CString sGroupName){m_sGroupName = sGroupName;};

protected:
	CString m_sName;
	CString m_sComment;
	double m_dFrequency;
	float m_fMinlevel;
	float m_fMaxlevel;
	float m_fSense;
	float m_fReference;
	float m_fShift;
	CString m_sConversion;
	int m_iChannel;
	int m_iTypeAdc;
	int m_iNumberDSP;
	GUID m_ID;
	int m_iType;
	CString m_sGroupName;
	long m_lStatus;
	float m_fSublevel;
};

