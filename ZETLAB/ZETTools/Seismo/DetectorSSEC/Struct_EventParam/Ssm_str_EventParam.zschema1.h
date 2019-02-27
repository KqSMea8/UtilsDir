#ifndef ZETLAB_SSM_STR_EVENTPARAM
#define ZETLAB_SSM_STR_EVENTPARAM

#include <zet_struct1.h>

#ifndef ZETLAB_SSM_STR_EVENTPARAM_TYPE
#define ZETLAB_SSM_STR_EVENTPARAM_TYPE (0x10019ul)
#define ZETLAB_SSM_STR_EVENTPARAM_SIZE (144)
struct Ssm_str_EventParam {
	struct ZetHeader zet_header;

	uint32_t objectID; /**< ID sensor */
	int32_t typeEvent; /**< The type of event */
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
	double dTps; /**< The difference between the time of entry into P- and S-phase, sec */
	double durationEvent; /**< The duration of the event without prehistory, sec */
	double endEventTimeSRV; /**< Time server end of the event, sec */

	struct ZetFooter zet_footer;
};
#endif /* ZETLAB_SSM_STR_EVENTPARAM_TYPE */

#endif
