#include "StdAfx.h"
#include "VibroProtect.h"
#include "ZETFile/include/ZETFile.h"
#include <pugixml/include/pugiautolink.hpp>
#include "creatingXMLfiles.h"

using namespace pugi;

CVibroProtect::CVibroProtect(void)
	: MaxAccel(0.f)
	, AccelMesureInG(true)
	, MaxVel(0.f)
	, MaxDispUp(0.f)
	, MaxDispDown(0.f)
	, MinFreq(0.f)
	, MaxFreq(0.f)
	, MaxVolt(0.f)
	, uSerialNumber(0)
	, fPeakForce(0.f)
	, fMovingMass(0.f)
	, fMaxWeight(0.f)
	, orient(L"")
	, name(L"")
	, imgName(L"")
{
	//ZeroMemory(orient, sizeof(orient));
	//ZeroMemory(name, sizeof(name));

}

CVibroProtect::~CVibroProtect(void)
{

}

bool CVibroProtect::GetProtectData(CString sShakerParPath)
{
	FILE *file(nullptr);
	//if (_wfopen_s(&file, sShakerParPath, L"r, ccs=UTF-8") != 0 || (file == nullptr))
	if (zfOpenFile(sShakerParPath, L"r, ccs=UTF-8", &file) != 0 || (file == nullptr))
	{
		TRACE("Ошибка открытия файла параметров вибростенда.\n");
		return false;
	}
	fwscanf_s(file, L"%f", &MaxDispUp);
	fwscanf_s(file, L"%f", &MaxDispDown);
	fwscanf_s(file, L"%f", &MaxVel);
	fwscanf_s(file, L"%f", &MaxAccel);
	fwscanf_s(file, L"%f", &MinFreq);
	fwscanf_s(file, L"%f", &MaxFreq);
	fwscanf_s(file, L"%f", &MaxVolt);
	fwscanf_s(file, L"%s", orient.GetString(), 20);//TODO:SV 20);
	fwscanf_s(file, L"%s", name.GetString(), 20);//TODO:SV 20);
	zfCloseFile(file);
	if (MaxDispUp <= 0.0 || MaxDispDown <= 0.0 || MaxVel <= 0.0 || MaxAccel <= 0.0 || MinFreq < 0.0 || MaxFreq <= 0.0 || MaxVolt <= 0.0 || MaxFreq < MinFreq)
		return false;
	return true;
}

bool CVibroProtect::GetProtectDataXML(CString sShakerCfgPath)
{
	creatingXMLfiles xmlGenerator;

	if (xmlGenerator.openDocument(sShakerCfgPath))
	{
		m_unit.reserve(10);
		xmlGenerator.readRootNode(L"Technical_Characteristics_Of_The_Shaker");
		CString sUnit = L"Unit";

		name			= xmlGenerator.readNodeAsCString(L"Vibration_Name");
		uSerialNumber	= xmlGenerator.readNodeAsUINT(L"Serial_Number");
		fPeakForce		= xmlGenerator.readNodeAsFloat(L"Peak_Force", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		fMovingMass		= xmlGenerator.readNodeAsFloat(L"Effective_Moving_Mass", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		fMaxWeight		= xmlGenerator.readNodeAsFloat(L"Maximum_Weight", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		MaxDispUp		= xmlGenerator.readNodeAsFloat(L"Minimal_Displacement", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		MaxDispDown		= xmlGenerator.readNodeAsFloat(L"Maximum_Displacement", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		MinFreq			= xmlGenerator.readNodeAsFloat(L"Minimal_Frequency", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		MaxFreq			= xmlGenerator.readNodeAsFloat(L"Maximum_Frequency", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		MaxVel			= xmlGenerator.readNodeAsFloat(L"Maximum_Velocity", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		MaxAccel		= xmlGenerator.readNodeAsFloat(L"Maximum_Acceleration", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		MaxVolt			= xmlGenerator.readNodeAsFloat(L"Maximum_Voltage", sUnit);
		m_unit.push_back(xmlGenerator.getAttrAsCString());
		orient			= xmlGenerator.readNodeAsCString(L"Orientation_In_Space");
		imgName			= xmlGenerator.readNodeAsCString(L"Image_Title");
	}

	if (MaxDispUp <= 0.0 || MaxDispDown <= 0.0 || MaxVel <= 0.0 || MaxAccel <= 0.0 || MinFreq < 0.0 || MaxFreq <= 0.0 || MaxVolt <= 0.0 || MaxFreq < MinFreq)
		return false;

	return true;
}