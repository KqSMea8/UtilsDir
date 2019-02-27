#ifndef ZET_STRUCT1_H
#define ZET_STRUCT1_H

#include <stdint.h>

#ifndef ZETLAB_ZETHEADER_SIZE
#define ZETLAB_ZETHEADER_SIZE (40)
struct ZetHeader {
	uint32_t size;
	uint32_t type;

	/* Structure address */
	uint64_t device_id;
	uint32_t module_id;
	uint16_t process_id;
	uint16_t channel_id;

	/* Structure timestamp */
	uint64_t seconds;
	uint32_t nanoseconds;
	uint32_t ticks;
};
#if __cplusplus > 199711L || _MSC_VER >= 1800
static_assert(sizeof(ZetHeader) == ZETLAB_ZETHEADER_SIZE, "Compiler breaks sizeof(ZetHeader)");
#endif
#endif /* ZETLAB_ZETHEADER_SIZE */

#ifndef ZETLAB_ZETFOOTER_SIZE
#define ZETLAB_ZETFOOTER_SIZE (8)
struct ZetFooter {
	uint32_t size;
	uint32_t crc;
};
#if __cplusplus > 199711L || _MSC_VER >= 1800
static_assert(sizeof(ZetFooter) == ZETLAB_ZETFOOTER_SIZE, "Compiler breaks sizeof(ZetFooter)");
#endif
#endif /* ZETLAB_ZETFOOTER_SIZE */

#ifndef ZETLAB_FLEXIBLE_ARRAY_SIZE
#define ZETLAB_FLEXIBLE_ARRAY_SIZE (1)
#endif /* ZETLAB_FLEXIBLE_ARRAY_SIZE */

#ifndef ZETLAB_ZETVOID_SIZE
#define ZETLAB_ZETVOID_SIZE (ZETLAB_ZETHEADER_SIZE + ZETLAB_ZETFOOTER_SIZE)
struct ZetVoid {
	struct ZetHeader zet_header;
	struct ZetFooter zet_footer;
};
#if __cplusplus > 199711L || _MSC_VER >= 1800
static_assert(sizeof(ZetVoid) == ZETLAB_ZETVOID_SIZE, "Compiler breaks sizeof(ZetVoid)");
#endif
#endif /* ZETLAB_ZETVOID_SIZE */

#endif
