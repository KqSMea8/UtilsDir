#ifndef ZETLAB_SSM_STR_EVENTSTATION
#define ZETLAB_SSM_STR_EVENTSTATION

#include <zet_struct1.h>

#ifndef ZETLAB_SSM_STR_EVENTSTATION_TYPE
#define ZETLAB_SSM_STR_EVENTSTATION_TYPE (0x1002dul)
#define ZETLAB_SSM_STR_EVENTSTATION_SIZE (224)
/** Station event parameters */
struct Ssm_str_EventStation {
	struct ZetHeader zet_header;

	uint32_t objectID; /**< ID station */
	char shortName[32]; /**< Station Short Name */

	/* Station coordinates */
	uint32_t _padding_to_80;
	double latitude; /**< Station Latitude, degrees */
	double longitude; /**< Station Longitude, degrees */
	double altitude; /**< Station Altitude, meters above sea level */

	uint32_t typeEvent; /**< Event Code */
	uint32_t operationCode; /**< Station Operation Code */
	float velocityP; /**< VelocityP, km/s */
	float magnitude; /**< The value of magnitude of the earthquake, unit Richter scale */
	float distance; /**< The value of distance to hypocenter, km */
	float intensity; /**< Intensity, MSK-64 */
	float peakAccelX; /**< The peak value of the X-acceleration of the event, m/c2 */
	float peakAccelY; /**< The peak value of the Y-acceleration of the event, m/c2 */
	float peakAccelZ; /**< The peak value of the Z-acceleration of the event, m/c2 */
	float peakAccelA; /**< The peak value of the Ampl-acceleration of the event, m/c2 */
	float peakVelocityX; /**< The peak value of the X-velocity of the event, m/c */
	float peakVelocityY; /**< The peak value of the Y-velocity of the event, m/c */
	float peakVelocityZ; /**< The peak value of the Z-velocity of the event, m/c */
	float peakVelocityA; /**< The peak value of the Ampl-velocity of the event, m/c */
	float peakMotionX; /**< The peak value of the X-motion of the event, mm */
	float peakMotionY; /**< The peak value of the Y-motion of the event, mm */
	float peakMotionZ; /**< The peak value of the Z-motion of the event, mm */
	float peakMotionA; /**< The peak value of the Ampl-motion of the event, mm */
	float mainPeriodX; /**< The main X-period of the event, sec */
	float mainPeriodY; /**< The main Y-period of the event, sec */
	float mainPeriodZ; /**< The main Z-period of the event, sec */
	float signalSrcSNR; /**< The SNR of the source signal */
	double mileage; /**< Station pipe mileage, kilometers */

	/* The coefficients for calculating the magnitude */
	float magn_A; /**< multiplikatiynfy factor for calculating the magnitude */
	float magn_B; /**< additive factor for calculating the magnitude */
	float magn_C; /**< correction factor for calculating the magnitude */

	uint32_t _padding_to_216;
	struct ZetFooter zet_footer;
};
#endif /* ZETLAB_SSM_STR_EVENTSTATION_TYPE */

#endif
