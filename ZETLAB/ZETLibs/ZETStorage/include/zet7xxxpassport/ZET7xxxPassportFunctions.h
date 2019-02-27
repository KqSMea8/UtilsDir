#ifndef zet7xxx_passport_functions_h_
#define zet7xxx_passport_functions_h_

#include <string>
#include <MsOffice\msword.tlh>

using namespace MSWord;

/** Пространство имен zet7xxx. */
namespace zet7xxx
{
	namespace doc
	{
		//////////////////////////////////////////////////////////////////
		//						PASSPORT MAKER							//
		//////////////////////////////////////////////////////////////////
		CellPtr getSerialCell		  (_In_ const _DocumentPtr& pDocument);
		CellPtr getDateShipmentCell	  (_In_ const _DocumentPtr& pDocument);
		CellPtr getNodeCell			  (_In_ const _DocumentPtr& pDocument);
		CellPtr getBaudRateCell		  (_In_ const _DocumentPtr& pDocument);
		CellPtr getParityCell		  (_In_ const _DocumentPtr& pDocument);
		CellPtr getRangeCell		  (_In_ const _DocumentPtr& pDocument);
		CellPtr getRepresentativeCell (_In_ const _DocumentPtr& pDocument);
		//////////////////////////////////////////////////////////////////
		//						SERTIFICATE MAKER						//
		//////////////////////////////////////////////////////////////////
		CellPtr getCertificateNumberCell	(_In_ const _DocumentPtr& pDocument);
		CellPtr getValidUntilCell			(_In_ const _DocumentPtr& pDocument);
		CellPtr getMeasuringInstrumentCell	(_In_ const _DocumentPtr& pDocument);
		CellPtr getGosreestrCell			(_In_ const _DocumentPtr& pDocument);
		CellPtr getSerialNumberCell			(_In_ const _DocumentPtr& pDocument);
		CellPtr getVerifiedAccordingCell	(_In_ const _DocumentPtr& pDocument);
		CellPtr getUsingEtalonsCell			(_In_ const _DocumentPtr& pDocument);
		CellPtr getInfluencingFactorsCell	(_In_ const _DocumentPtr& pDocument);
		CellPtr getVerificationCell			(_In_ const _DocumentPtr& pDocument);
		CellPtr getCertificateDateCell		(_In_ const _DocumentPtr& pDocument);

		// установка инициалов и имени для первого человека
		void SetFirstPerson(	_In_ const _DocumentPtr& pDocument,
								_In_ const std::wstring& Post,
								_In_ const std::wstring& Initial);
		// установка инициалов и имени для первого человека
		void SetSecondPerson(	_In_ const _DocumentPtr& pDocument,
								_In_ const std::wstring& Post,
								_In_ const std::wstring& Initial);
	}
}

#endif	// zet7xxx_passport_functions_h_
