#ifndef ZETLAB_SSM_CFG_ALGR_MONITOREVARCOMP
#define ZETLAB_SSM_CFG_ALGR_MONITOREVARCOMP

#include <stdint.h>

#ifndef ZETLAB_ZETHEADER_SIZE
#define ZETLAB_ZETHEADER_SIZE (40)
struct ZetHeader {
	uint32_t size;
	uint32_t type;
	uint64_t device_id;
	uint32_t module_id;
	uint16_t process_id;
	uint16_t channel_id;
	uint64_t seconds;
	uint32_t nanoseconds;
	uint32_t ticks;
};
#endif /* ZETLAB_ZETHEADER_SIZE */

#ifndef ZETLAB_ZETFOOTER_SIZE
#define ZETLAB_ZETFOOTER_SIZE (4)
struct ZetFooter {
	uint8_t crc[4];
};
#endif /* ZETLAB_ZETFOOTER_SIZE */

#define ZETLAB_SSM_CFG_ALGR_MONITOREVARCOMP_SIZE (52)
#define ZETLAB_SSM_CFG_ALGR_MONITOREVARCOMP_TYPE (0x10033ul)

/** SKSV. MonitoreConstComponent algorithm configuration. */
struct Ssm_cfg_Algr_MonitoreVarComp {
	struct ZetHeader zet_header;

	float MaxThreshold; /**< Maximum threshold */
	float MinThreshold; /**< Minimum threshold */

	struct ZetFooter zet_footer;
};

#define ZETLAB_SSM_DIAG_ALGR_MONITOREVARCOMP_SIZE (48)
#define ZETLAB_SSM_DIAG_ALGR_MONITOREVARCOMP_TYPE (0x10034ul)

struct Ssm_diag_Algr_MonitoreVarComp {
	struct ZetHeader zet_header;

	float MeanValue; /**< variable value */

	struct ZetFooter zet_footer;
};

#define ZETLAB_SSM_RES_ALGR_MONITOREVARCOMP_SIZE (48)
#define ZETLAB_SSM_RES_ALGR_MONITOREVARCOMP_TYPE (0x10035ul)

/** SKSV. MonitoreVarCopmonent result */
struct Ssm_res_Algr_MonitoreVarComp {
	struct ZetHeader zet_header;

	uint32_t meanval; /**< Flag. The signal exceeded a threshold */

	struct ZetFooter zet_footer;
};

#endif
