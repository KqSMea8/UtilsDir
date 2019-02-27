/* ZET Device Control Protocol header (version YYYYMMDD) */
#ifndef ZDCP_INCLUDED
#define ZDCP_INCLUDED               (20171110)

/* Changelog
 * 20160309 Using <stdint.h> types and Unix format (LF)
 * 20160112 Added command ZDCP_COMMAND_PEER_ADDR
**/

#include <stdint.h>

/* Multicast group */
#define ZDCP_MULTICAST_ADDR_STR     "239.192.71.76"
#define ZDCP_MULTICAST_ADDR         (0xefc0474c)
#define ZDCP_MULTICAST_PORT         (7176)

/* Common protocol signature ('ZDCP') */
#define ZDCP_SIGNATURE              (0x5a444350)

/* List of packet id values */
#define ZDCP_ID_REPORT              (0x0000)
#define ZDCP_ID_PROBE               (0x0001)
#define ZDCP_ID_RESPONSE            (0x0002)
#define ZDCP_ID_COMMAND             (0x0003)

/* List of report statuses */
#define ZDCP_REPORT_READY           (0x0000) /* Ready to connect */
#define ZDCP_REPORT_BUSY            (0x0001) /* Passive mode */
#define ZDCP_REPORT_PEER            (0x0002) /* Active (peer) mode */
#define ZDCP_REPORT_CONFLICT        (0x0003) /* IP conflict detected */

/* List of response status values */
#define ZDCP_RESPONSE_OK            (0x0000) /* Successfully executed */
#define ZDCP_RESPONSE_UNRECOGNIZED  (0x0001) /* Unknown command */
#define ZDCP_RESPONSE_DECLINED      (0x0002) /* Recognized but refused */
#define ZDCP_RESPONSE_INVALID_PARAM (0x0003) /* Invalid param value */
#define ZDCP_RESPONSE_ERROR         (0x0004) /* Some kind of error */

/* List of commands */
#define ZDCP_COMMAND_PING           (0x0000) /* Ping command */
#define ZDCP_COMMAND_RESTART        (0x0001) /* Restart device */
#define ZDCP_COMMAND_RESET          (0x0002) /* Reset settings */
#define ZDCP_COMMAND_RSI_READ       (0x0003) /* Read sector via RSI */
#define ZDCP_COMMAND_RSI_WRITE      (0x0004) /* Write sector via RSI */
#define ZDCP_COMMAND_PEER_ADDR      (0x0005) /* Get peer IP addr */

/* Target serial and type value for broadcasted commands */
#define ZDCP_TARGET_BROADCAST       (0xFFFFffffL)

/* Packet length values */
#define ZDCP_LENGTH_REPORT    (sizeof(ZDCP_REPORT)    - sizeof(ZDCP_HEADER))
#define ZDCP_LENGTH_REPORT_EX (sizeof(ZDCP_REPORT_EX) - sizeof(ZDCP_HEADER))
#define ZDCP_LENGTH_PROBE     (sizeof(ZDCP_PROBE)     - sizeof(ZDCP_HEADER))
#define ZDCP_LENGTH_RESPONSE  (sizeof(ZDCP_RESPONSE)  - sizeof(ZDCP_HEADER))
#define ZDCP_LENGTH_COMMAND   (sizeof(ZDCP_COMMAND)   - sizeof(ZDCP_HEADER))

/* Header of packets in protocol */
typedef struct ZDCP_HEADER_STRUCT {
    uint32_t signature; /* ZDCP_SIGNATURE */
    uint16_t id;        /* Packet type: ZDCP_ID_XXXX */
    uint16_t length;    /* Length of data followed by this header */
} ZDCP_HEADER;

/* Report is sent by devices automatically or in response to probe */
typedef struct ZDCP_REPORT_STRUCT {
    ZDCP_HEADER header;

    uint32_t serial_hi; /* Device serial number */
    uint32_t serial_lo;
    uint32_t type;      /* Device type */

    uint32_t addr;      /* Modbus IP addr */
    uint16_t port;      /* Modbus port */
    uint16_t status;    /* Modbus status: ZDCP_REPORT_XXXX */
} ZDCP_REPORT;

/* Extended report contains additional info */
typedef struct ZDCP_REPORT_EX_STRUCT {
    ZDCP_HEADER header;

    uint32_t serial_hi; /* Device serial number */
    uint32_t serial_lo;
    uint32_t type;      /* Device type */

    uint32_t addr;      /* Modbus IP addr */
    uint16_t port;      /* Modbus port */
    uint16_t status;    /* Modbus status: ZDCP_REPORT_XXXX */

    uint32_t peer_addr; /* Peer IP addr */

    /* Device label (UTF-8) can follow */
} ZDCP_REPORT_EX;

/* Probe is a detection request to all devices for reports */
typedef struct ZDCP_PROBE_STRUCT {
    ZDCP_HEADER header;
} ZDCP_PROBE;

/* Result is sent by device in response to command */
typedef struct ZDCP_RESPONSE_STRUCT {
    ZDCP_HEADER header;

    uint32_t serial_hi; /* Device serial number */
    uint32_t serial_lo;
    uint32_t type;      /* Device type */

    uint32_t sequence; /* Sequence number from command */
    uint32_t command;  /* Command on which we respond */
    uint32_t response; /* ZDCP_RESPONSE_XXXX */

    /* Depending on command and result, some data can follow this struct */
} ZDCP_RESPONSE;

/* Command is an action request to specific device */
typedef struct ZDCP_COMMAND_STRUCT {
    ZDCP_HEADER header;

    uint32_t serial_hi; /* Target serial number (or all one`s) */
    uint32_t serial_lo;
    uint32_t type;      /* Target type (or all one`s) */

    uint32_t sequence; /* Sequence number to ensure ordering */
    uint32_t command;  /* ZDCP_COMMAND_XXXX: command to execute */
    uint32_t param;    /* Command parameter depending on command */

    /* Depending on command and param, some data can follow this struct */
} ZDCP_COMMAND;

/* Common packet for recv() */
typedef union ZDCP_COMMON_PACKET_UNION {
    ZDCP_HEADER header;

    ZDCP_REPORT    report;
    ZDCP_REPORT_EX report_ex;
    ZDCP_PROBE     probe;
    ZDCP_RESPONSE  response;
    ZDCP_COMMAND   command;
} ZDCP_COMMON_PACKET;

#endif /* ZDCP_INCLUDED */
