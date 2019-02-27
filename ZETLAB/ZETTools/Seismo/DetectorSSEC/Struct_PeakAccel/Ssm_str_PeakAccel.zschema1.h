#ifndef ZETLAB_SSM_STR_PEAKACCEL
#define ZETLAB_SSM_STR_PEAKACCEL

#include <zet_struct1.h>

#ifndef ZETLAB_SSM_STR_PEAKACCEL_TYPE
#define ZETLAB_SSM_STR_PEAKACCEL_TYPE (0x1003bul)
#define ZETLAB_SSM_STR_PEAKACCEL_SIZE (112)
struct Ssm_str_PeakAccel {
	struct ZetHeader zet_header;

	uint32_t objectID; /**< ID sensor */
	int32_t quality; /**< The quality of data */
	float peakAccelX; /**< The peak value of the X-acceleration, m/c2 */
	float peakAccelY; /**< The peak value of the Y-acceleration, m/c2 */
	float peakAccelZ; /**< The peak value of the Z-acceleration, m/c2 */
	float peakAccelA; /**< The peak value of the Ampl-acceleration, m/c2 */
	float peakVelocityX; /**< The peak value of the X-velocity, m/c */
	float peakVelocityY; /**< The peak value of the Y-velocity, m/c */
	float peakVelocityZ; /**< The peak value of the Z-velocity, m/c */
	float peakVelocityA; /**< The peak value of the Ampl-velocity, m/c */
	float peakMotionX; /**< The peak value of the X-motion, mm */
	float peakMotionY; /**< The peak value of the Y-motion, mm */
	float peakMotionZ; /**< The peak value of the Z-motion, mm */
	float peakMotionA; /**< The peak value of the Ampl-motion, mm */
	uint32_t intensity; /**< Intensity, MSK-64 */

	uint32_t _padding_to_104;
	struct ZetFooter zet_footer;
};
#endif /* ZETLAB_SSM_STR_PEAKACCEL_TYPE */

#endif
