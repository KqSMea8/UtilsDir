#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\ChannelInfo.h"
//#include "resource.h"

CChannelInfo::CChannelInfo(CDSRV* pSRV)
{
	m_pServer = pSRV;
}

CChannelInfo::~CChannelInfo()
{
	m_ChannelInfoMap.clear();
}

void CChannelInfo::FillChannelInfo()
{
	m_ChannelInfoMap.clear();
	//нужно вывести список измерительных каналов
	long lQuantity(m_pServer->QuanChan());

	for (int i = 0; i < lQuantity; ++i)
	{
		FillChannel(i);
	}

}

void CChannelInfo::FillChannel(long lNumber)
{
	m_ChannelInfoMap[lNumber].sName = m_pServer->Commentary(lNumber);
	m_ChannelInfoMap[lNumber].dTime = double((long long(m_pServer->CurrentTime(lNumber))) + 1);
	m_ChannelInfoMap[lNumber].dFrequency = m_pServer->Frequency(lNumber);
	m_pServer->IdChan(lNumber, &m_ChannelInfoMap[lNumber].idChan);
	m_ChannelInfoMap[lNumber].dWatermarkPeriod = double(SIZEOFWMF) * 2. / m_ChannelInfoMap[lNumber].dFrequency; 
	if (m_ChannelInfoMap[lNumber].dWatermarkPeriod <= 1.)
		m_ChannelInfoMap[lNumber].dWatermarkPeriod = 1.;
	else
	{
		if (m_ChannelInfoMap[lNumber].dWatermarkPeriod <= 10.)
			m_ChannelInfoMap[lNumber].dWatermarkPeriod = 10.;
		else
			m_ChannelInfoMap[lNumber].dWatermarkPeriod = 100.;
	}
	 
	m_ChannelInfoMap[lNumber].lLevels = FillChanLevels(m_ChannelInfoMap[lNumber].pLevels, m_pServer->GroupName(lNumber));
	m_ChannelInfoMap[lNumber].pLevels[m_ChannelInfoMap[lNumber].lLevels] = m_ChannelInfoMap[lNumber].sName;
	m_ChannelInfoMap[lNumber].lLevels++;
	for (int i = 0; i < m_ChannelInfoMap[lNumber].lLevels; ++i)
		m_ChannelInfoMap[lNumber].sGroupName += m_ChannelInfoMap[lNumber].pLevels[i] + L";";
	
	m_ChannelInfoMap[lNumber].sConversion = m_pServer->Conversion(lNumber);
	m_ChannelInfoMap[lNumber].iStatus = m_pServer->GetStatus(lNumber);
}

long CChannelInfo::FillChanLevels(CString* pLevels, CString sGroupName)
{
	long lQuantity(0);
	for (int i = 0; i < MAX_CHAN_LEVELS; ++i)
		pLevels[i] = L"";

	if (sGroupName != L"")
	{
		sGroupName += L";";
		CString sTemp(L"");
		
		while (sGroupName.Find(L";") != -1)
		{
			sTemp = sGroupName.Mid(0, sGroupName.Find(L";"));
			sGroupName.Delete(0, sGroupName.Find(L";") + 1);
			if (sTemp != L"")
			{
				pLevels[lQuantity] = sTemp;
				lQuantity++;
			}
		}
	}
	//если группового имени нет
	if (lQuantity == 0)
	{
		CString sTemp;
		//sTemp.LoadString(IDS_MY_COMPUTER);
		sTemp = L"Мой компьютер";
		pLevels[0] = sTemp;
		sTemp.LoadString(IDS_NO_NAME);
		pLevels[1] = sTemp;
		lQuantity = 2;
	}
	//если это модуль данного компьютера
	if (lQuantity == 1)
	{
		pLevels[1] = pLevels[0];
		CString sTemp;
		//sTemp.LoadString(IDS_MY_COMPUTER); 
		sTemp = L"Мой компьютер";
		pLevels[0] = sTemp;
		lQuantity = 2;
	}
	
	return lQuantity;
}

long CChannelInfo::GetMaxLevels()
{
	long lMax(0);
	long lQuantity(m_ChannelInfoMap.size());

	for (int i = 0; i < lQuantity; ++i)
	{
		if (m_ChannelInfoMap[i].lLevels > lMax)
			lMax = m_ChannelInfoMap[i].lLevels;
	}

	return lMax;
}