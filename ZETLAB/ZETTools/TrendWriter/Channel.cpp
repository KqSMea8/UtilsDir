#include "StdAfx.h"
#include "Channel.h"

CChannel::CChannel(void)
	: m_dFrequency(0.)
	, m_fSublevel(0.f)
	, m_fMinlevel(0.f)
	, m_fMaxlevel(0.f)
	, m_fSense(0.f)
	, m_fReference(0.f)
	, m_fShift(0.f)
	, m_iChannel(0)
	, m_iTypeAdc(0)
	, m_iType(0)
	, m_iNumberDSP(0)
	, m_lStatus(0)
	, m_ID(GUID_NULL)
{
}

CChannel::~CChannel(void)
{
}

void CChannel::operator = (const CHANNEL_STRUCT & structure)
{
	m_lStatus = structure.status;
	m_ID = structure.id;
	m_sComment = (CString)structure.name;
	m_sConversion = (CString)structure.measure;
	m_dFrequency = structure.frequency;
	m_fSublevel = (float)structure.subLevel;
	m_fMinlevel = (float)structure.minLevel;
	m_fMaxlevel = (float)structure.maxLevel;
	m_fSense = (float)structure.sense;
	m_fReference = (float)structure.referense;
	m_fShift = (float)structure.dc;
}