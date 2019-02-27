//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <LDS\LdsXml.h>
//------------------------------------------------------------------------------
CLdsXml::CLdsXml()
	: m_sFullFileNameXML (L"c:\\ZetLab\\Config\\lds.xml")
{}
//------------------------------------------------------------------------------
void CLdsXml::ReadLine(pugi::xml_node &hNode, Lds_cfg_Param_Line &line)
{
	pugi::xml_node hPage = hNode.first_child();
	if ((CString)hPage.name() == L"ID")
	{
		line.ID = (BYTE)hPage.attribute(L"Value").as_uint();

		hPage = hPage.next_sibling();
		if ((CString)hPage.name() == L"bOnOff")
		{
			line.bOnOff = hPage.attribute(L"Value").as_bool();
			hPage = hPage.next_sibling();
			if ((CString)hPage.name() == L"NumberSensorsSystem")
			{
				line.SetNumberOfSensorsSystem(
					(DWORD)hPage.attribute(L"Value").as_uint());
				hPage = hPage.next_sibling();
				ReadConstLine(hPage, line.constLine);
				line.SetMaxTrans(line.constLine.maxTrans);

				BYTE i = 0;
				for (auto it = line.vectSS.begin(), ie = line.vectSS.end();
					it != ie; ++it, ++i)
				{
					CString str;
					str.Format(L"ParamSS_%d", i);
					hPage = hNode.child(str);
					if (hPage)
						ReadSS(hPage, *it);
				}

				hPage = hNode.child(L"ParamTopLevel");
				if (hPage)
					ReadTopLevel(hPage, line.topLevel);
			}
		}
	}
}
//------------------------------------------------------------------------------
void CLdsXml::ReadConstLine(pugi::xml_node &hNode, Lds_cfg_Const_Line &cline)
{
	if ((CString)hNode.name() == L"ConstLine")
	{
		pugi::xml_node hPage = hNode.first_child();
		if ((CString)hPage.name() == L"SoundSpeed")
		{
			cline.soundSpeed_val = hPage.attribute(L"Value_km_c").as_float();
			cline.soundSpeed_unc = hPage.attribute(L"Uncertainty_km_c").as_float();
			cline.bSoundSpeed_Table = hPage.attribute(L"bTable").as_bool();

			hPage = hPage.next_sibling();
			if ((CString)hPage.name() == L"MaxPressure")
			{
				cline.maxPressure = hPage.attribute(L"Value_MPa").as_float();
				hPage = hPage.next_sibling();
				if ((CString)hPage.name() == L"MaxTrans")
				{
					cline.maxTrans = hPage.attribute(L"Value_m3_h").as_float();
					hPage = hPage.next_sibling();
					if ((CString)hPage.name() == L"DensityOil")
					{
						cline.densityOil = hPage.attribute(L"Value_kg_m3").as_float();
						hPage = hPage.next_sibling();
						if ((CString)hPage.name() == L"InnerDiameter")
						{
							cline.innerDiameter = hPage.attribute(L"Value_m").as_float();
							hPage = hPage.next_sibling();
							if ((CString)hPage.name() == L"MinDeltaFreqPump")
							{
								cline.minDeltaFreqPump = hPage.attribute(L"Value_Hz").as_float();
								hPage = hPage.next_sibling();
								if ((CString)hPage.name() == L"PressureForDetect")
								{
									cline.pressureDetectStart = hPage.attribute(L"Value_Start_MPa").as_float();
									cline.pressureDetectStop  = hPage.attribute(L"Value_Stop_MPa").as_float();
								}
							}
						}
					}

				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void CLdsXml::ReadSS(pugi::xml_node &hNode, Lds_cfg_Param_SS &ss)
{
	pugi::xml_node hPage = hNode.first_child();
	if ((CString)hPage.name() == L"ID")
	{
		ss.ID = (BYTE)hPage.attribute(L"Value").as_uint();

		hPage = hPage.next_sibling();
		if ((CString)hPage.name() == L"bOnOff")
		{
			ss.bOnOff = hPage.attribute(L"Value").as_bool();
			hPage = hPage.next_sibling();
			if ((CString)hPage.name() == L"Mileage")
			{
				ss.mileage = hPage.attribute(L"Value_km").as_float();
				hPage = hPage.next_sibling();
				if ((CString)hPage.name() == L"NameSRV")
				{
					ss.nameSRV = hPage.attribute(L"Value").as_string();
					hPage = hPage.next_sibling();
					if ((CString)hPage.name() == L"ParamHdr")
						ReadHdr(hPage, ss.parHdr);
					hPage = hNode.child(L"ParamPrs");
					if (hPage)
						ReadPrs(hPage, ss.parPrs);
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void CLdsXml::ReadHdr(pugi::xml_node &hNode, Lds_cfg_Param_Hdr &hdr)
{
	CString str;
	pugi::xml_node hPage = hNode.first_child();
	if ((CString)hPage.name() == L"bOnOff")
	{
		hdr.bOnOff = hPage.attribute(L"Value").as_bool();
		hPage = hPage.next_sibling();
		if ((CString)hPage.name() == L"FDWT")
		{
			hdr.parFDWT.bOnOff = hPage.attribute(L"bOnOff").as_bool();
			str = hPage.attribute(L"type").as_string();
			if (str == L"Haar")
				hdr.parFDWT.type = Haar;
			else if (str == L"db1")
				hdr.parFDWT.type = db1;
			else if (str == L"db2")
				hdr.parFDWT.type = db2;
			else if (str == L"db3")
				hdr.parFDWT.type = db3;
			else if (str == L"db4")
				hdr.parFDWT.type = db4;
			else if (str == L"db5")
				hdr.parFDWT.type = db5;
			else if (str == L"db6")
				hdr.parFDWT.type = db6;
			else if (str == L"db7")
				hdr.parFDWT.type = db7;
			else if (str == L"db8")
				hdr.parFDWT.type = db8;
			else if (str == L"db9")
				hdr.parFDWT.type = db9;
			else if (str == L"db10")
				hdr.parFDWT.type = db10;
			else if (str == L"sym1")
				hdr.parFDWT.type = sym1;
			else if (str == L"sym2")
				hdr.parFDWT.type = sym2;
			else if (str == L"sym3")
				hdr.parFDWT.type = sym3;
			else if (str == L"sym4")
				hdr.parFDWT.type = sym4;
			else if (str == L"sym5")
				hdr.parFDWT.type = sym5;
			else if (str == L"sym6")
				hdr.parFDWT.type = sym6;
			else if (str == L"sym7")
				hdr.parFDWT.type = sym7;
			else if (str == L"coif1")
				hdr.parFDWT.type = coif1;
			else if (str == L"coif2")
				hdr.parFDWT.type = coif2;
			else if (str == L"coif3")
				hdr.parFDWT.type = coif3;
			else if (str == L"coif4")
				hdr.parFDWT.type = coif4;
			else if (str == L"coif5")
				hdr.parFDWT.type = coif5;

			hdr.parFDWT.level = hPage.attribute(L"level").as_int();
			hdr.parFDWT.alfa = hPage.attribute(L"alfa").as_float();
			hdr.parFDWT.minNumBand_Percent = hPage.attribute(L"minNumBand_Percent").as_float();
			hdr.parFDWT.factorCalibrLine2 = hPage.attribute(L"FactorCalibrLine2").as_float();
			
			hPage = hPage.next_sibling();
			if ((CString)hPage.name() == L"FFT")
			{
				hdr.parFFT.bOnOff = hPage.attribute(L"bOnOff").as_bool();
				str = hPage.attribute(L"type").as_string();
				if (str == L"Hamming")
					hdr.parFFT.type = twf_Hamming;
				else if (str == L"Hann")
					hdr.parFFT.type = twf_Hann;
				else if (str == L"Bartlett")
					hdr.parFFT.type = twf_Bartlett;

				hdr.parFFT.order = hPage.attribute(L"order").as_int();
				hdr.parFFT.alfa = hPage.attribute(L"alfa").as_float();
				hdr.parFFT.minNumBand_Percent = hPage.attribute(L"minNumBand_Percent").as_float();

				hPage = hPage.next_sibling();
				if ((CString)hPage.name() == L"FOFT")
				{
					hdr.parFOFT.bOnOff = hPage.attribute(L"bOnOff").as_bool();
					str = hPage.attribute(L"base").as_string();
					int n = str.Find(L"/");
					if (n != -1)
					{
						str.Delete(0, n + 1);
						str.Trim();
						hdr.parFOFT.base = _wtoi(str);
					}
					hdr.parFOFT.minFreq = hPage.attribute(L"minFreq").as_float();
					hdr.parFOFT.alfa = hPage.attribute(L"alfa").as_float();
					hdr.parFOFT.minNumBand_Percent = hPage.attribute(L"minNumBand_Percent").as_float();
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void CLdsXml::ReadPrs(pugi::xml_node &hNode, Lds_cfg_Param_Prs &prs)
{
	pugi::xml_node hPage = hNode.first_child();
	if ((CString)hPage.name() == L"bOnOff")
	{
		prs.bOnOff = hPage.attribute(L"Value").as_bool();
		hPage = hPage.next_sibling();
		if ((CString)hPage.name() == L"Press")
		{
			prs.parPres.bOnOff = hPage.attribute(L"bOnOff").as_bool();
			prs.parPres.freqFiltr = hPage.attribute(L"FreqFiltr_Hz").as_float();
			prs.parPres.deltaPressure = hPage.attribute(L"DeltaPress_MPa").as_float();
			prs.parPres.factorCalibrLine = hPage.attribute(L"FactorCalibrLine").as_float();
		}
	}
}
//------------------------------------------------------------------------------
void CLdsXml::ReadTopLevel(pugi::xml_node &hNode, Lds_cfg_TopLevel &tl)
{
	pugi::xml_node hPage = hNode.first_child();
	if ((CString)hPage.name() == L"ParamTimeFiltr")
	{
		tl.time.lifeTime = hPage.attribute(L"LifeTime_sec").as_double();
		tl.time.maxPause = hPage.attribute(L"MaxPause_sec").as_double();
		tl.time.minDuration = hPage.attribute(L"MinDuration_sec").as_double();
		tl.time.timerResultFast = hPage.attribute(L"TimerResultFast_sec").as_double();
		tl.time.timerResultSlow = hPage.attribute(L"TimerResultSlow_sec").as_double();
		tl.time.timeSRMC_before = hPage.attribute(L"TimeSRMC_before_sec").as_double();
		tl.time.timeSRMC_after  = hPage.attribute(L"TimeSRMC_after_sec").as_double();

		hPage = hPage.next_sibling();
		if ((CString)hPage.name() == L"ParamAcst")
		{
			tl.acst.bOnOff = hPage.attribute(L"bOnOff").as_bool();
	
			hPage = hPage.next_sibling();
			if ((CString)hPage.name() == L"ParamProf")
			{
				tl.prof.bOnOff = hPage.attribute(L"bOnOff").as_bool();

				hPage = hPage.next_sibling();
				if ((CString)hPage.name() == L"ParamWave")
					tl.wave.bOnOff = hPage.attribute(L"bOnOff").as_bool();
			}
		}
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// чтение конфигурационных данных и задание времён детектирования
bool CLdsXml::ReadCFG(CString fileNameXML)
{
	pugi::xml_document hFile;
	pugi::xml_node hPagePipe;
	pugi::xml_node hPage;

	if ( !fileNameXML.IsEmpty() )
		m_sFullFileNameXML = fileNameXML;
	
	m_bInit = !m_sFullFileNameXML.IsEmpty();

	if (m_bInit)
	{	// открываю XML-файл
		m_bInit = hFile.load_file(m_sFullFileNameXML,
			pugi::parse_default,
			pugi::encoding_utf8).status == pugi::status_ok;

		hPagePipe = hFile.child(L"Pipe");
		hPage = hPagePipe.first_child();

		if ((CString)hPage.name() == L"ID")
		{
			m_parAll.ID = (BYTE)hPage.attribute(L"Value").as_uint();
			hPage = hPage.next_sibling();

			if ((CString)hPage.name() == L"bOnOff")
			{
				m_parAll.bOnOff = hPage.attribute(L"Value").as_bool();
				hPage = hPage.next_sibling();

				if ((CString)hPage.name() == L"ParamLine")
					ReadLine(hPage, m_parAll.line);
			}
		}
	}
	return m_bInit;
}
//------------------------------------------------------------------------------
//CLdsXml::~CLdsXml()
//{}
//------------------------------------------------------------------------------
bool CLdsXml::WriteSoundSpeed(
	const float mean, const float uncertainty, bool bTable)
{
	bool ret = m_bInit;
	if (ret)
	{
		Lds_cfg_Const_Line &pcl = m_parAll.line.constLine;
		pcl.bSoundSpeed_Table = bTable;
		pcl.soundSpeed_unc = uncertainty;
		pcl.soundSpeed_val = mean;
			
		pugi::xml_document hFile;
		pugi::xml_node hPage;

		// новое значение - в файл
		ret = (hFile.load_file(m_sFullFileNameXML,
			pugi::parse_default,
			pugi::encoding_utf8)).status == pugi::status_ok;
		if (ret)
		{
			hPage = hFile.child(L"Pipe");
			ret = hPage != NULL;
			if (ret)
			{
				hPage = hPage.child(L"ParamLine");
				ret = hPage != NULL;
				if (ret)
				{
					hPage = hPage.child(L"ConstLine");
					ret = hPage != NULL;
					if (ret)
					{
						hPage = hPage.child(L"SoundSpeed");
						ret = hPage != NULL;
						if (ret)
						{
							hPage.attribute(L"Value_km_c").set_value(mean);
							hPage.attribute(L"Uncertainty_km_c").set_value(uncertainty);
							hPage.attribute(L"bTable").set_value(bTable);

							ret = hFile.save_file(m_sFullFileNameXML);
						}
					}
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CLdsXml::GetChanSRV(CDSRV *psrv, BYTE idSens, bool bPrs, bool bChanSupply,
	long &chanSRV)
{
	CString name;
	chanSRV = -1;
	bool ret = (psrv != NULL) &&
				GetChanName(idSens, bPrs, bChanSupply, name);
	if (ret)
	{
		long num = psrv->QuanChan();
		ret = num > 0;
		if (ret)
		{
			for (long i = 0; i < num; ++i)
			{
				if (name == psrv->Commentary(i))
				{
					chanSRV = i;
					ret = true;
					break;
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CLdsXml::GetChanName(BYTE idSens, bool bPrs, bool bChanSupply,
	CString &name)
{
	DWORD indx = idSens - 1;
	bool ret = m_bInit && !m_parAll.line.vectSS.empty() &&
		(indx < m_parAll.line.vectSS.size());
	if (ret)
	{
		Lds_cfg_Param_SS &ss = m_parAll.line.vectSS[indx];
		if (ret)
		{
			name = ss.nameSRV + L"-1";
			if (bPrs)
				name += L"S";
			else
				name += L"D";

			if (bChanSupply)
				name += L"p";
		}
	}
	return ret;
}
//------------------------------------------------------------------------------