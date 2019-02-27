//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
//#include <vector>
#include <LDS\lds.h>
//#include <mathZet.h>
//#include <pugixml-1.4\src\pugixml.hpp>
//#include <Metrology\SpaceProbability.h>
#include <ippFDWT\OncePacketFDWT.h>
#include <ZetThread\ThreadSRVmultiSelf.h>
//------------------------------------------------------------------------------
bool SearchTimeBegin(CDSRV *psrv, MultiChannelSelf *pReadParam,
	double &Tx, bool bTimeInDate);
//------------------------------------------------------------------------------