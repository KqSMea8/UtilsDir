/* zet76 -- ZET SENSOR Ethernet Interface */
#ifndef ZET76_INCLUDED
#define ZET76_INCLUDED      (20160523)

/* Brief History
 * 20160523 Fixed TCP state detection for MGate MB3180 (SNMP)
 * 20160405 Added recv flag ZET76_RECV_CLEAR to clear recv buffer
 * 20160303 Added state ZET76_DEVICE_BUSY, fixed bufferevents
 * 20151103 Added flags ZET76_SEND_DONTWAIT/ZET76_RECV_DONTWAIT
 * -------- broken compatibility
 * 20151030 Added mode ZET76_DEVICE_SKIP_CHAIN to drop incoming chain data
 * 20150923 Call status callbacks only from the server thread
 * 20150916 Improved reliability of ZET 7177 connections
 * 20150624 Added support for MOXA MGate MB3180 (RS485 <-> Modbus TCP)
 * 20150128 Added support for ZET 7177 (GSM)
 * 20140402 Added periodic timers support
 * 20130808 Added support for MOXA NPort W2150A (RS485 <-> Wi-Fi)
 * 20130808 Removed memory leaking zet76_node::response_event
 * 20130717 SNMP scanner now requests MOXA NE-4110A TCP port state
**/

#ifdef __cplusplus
#define ZET76_EXPORT extern "C" int
#else
#define ZET76_EXPORT extern int
#endif

/* Default multicast ip and port for device scanner */
#define ZET76_SCAN_IP       "239.192.71.76"
#define ZET76_SCAN_PORT     (7176)

/* Default server port for device listener */
#define ZET76_LISTEN_PORT   (7176)

/* Error codes for all functions */
enum zet76_errors
{
    ZET76_OK                =  0,
    ZET76_ERROR             = -1,
    ZET76_INVALID_PARAMETER = -2,
    ZET76_OBJECT_NOT_FOUND  = -3,
    ZET76_NOT_ENOUGH_MEMORY = -4,
    ZET76_NOT_SUPPORTED     = -5,
    ZET76_INVALID_STATE     = -6,
    ZET76_INVALID_MODE      = -7,
    ZET76_TIMEOUT           = -8,
    ZET76_AGAIN             = -9, /* Server busy, try again */
};

/* Supported device type values */
enum zet76_device_types
{
    ZET76_DEVICE_TYPE_UNKNOWN = 0,
    ZET76_DEVICE_TYPE_7176    = 25,
    ZET76_DEVICE_TYPE_7076    = 46,
    ZET76_DEVICE_TYPE_7177    = 50,
};

/* Device state values */
enum zet76_device_states
{
    ZET76_DEVICE_IDLE        = 0,

    ZET76_DEVICE_CONNECTING  = 1,
    ZET76_DEVICE_CONNECTED   = 2,

    /* State 3 is reserved */
    ZET76_DEVICE_ACCEPTED    = 4,

    ZET76_DEVICE_BUSY        = 5,
    ZET76_DEVICE_CONFLICTED  = 6,

    ZET76_DEVICE_CONTROLLING = 7,
    ZET76_DEVICE_CONTROLLED  = 8,
};

/* Device modes bits */
enum zet76_device_modes
{
    ZET76_DEVICE_CONNECT = 0x0002,
    ZET76_DEVICE_ACCEPT  = 0x0004,
    ZET76_DEVICE_CONTROL = 0x0008,

    /* Drop all incoming chain data on active device */
    ZET76_DEVICE_SKIP_CHAIN = 0x10000,
};

/* Device flags bits */
enum zet76_device_flags
{
    ZET76_DEVICE_CONNECTABLE  = ZET76_DEVICE_CONNECT,
    ZET76_DEVICE_ACCEPTABLE   = ZET76_DEVICE_ACCEPT,
    ZET76_DEVICE_CONTROLLABLE = ZET76_DEVICE_CONTROL,
};

/* Device send flags */
enum zet76_send_flags
{
    ZET76_SEND_DONTWAIT = 0x0080, /* may return ZET76_AGAIN */
};

/* Device recv event flags */
enum zet76_recv_flags
{
    ZET76_RECV_CHAIN    = 0x0001,
    ZET76_RECV_RESPONSE = 0x0002,
    ZET76_RECV_CONTROL  = 0x0008,
    /* Bits till 0x0040 are reserved for future use */

    ZET76_RECV_DONTWAIT = 0x0080, /* may return ZET76_AGAIN */
    ZET76_RECV_ERROR    = 0x0100,
    ZET76_RECV_TIMEOUT  = 0x0200,
    ZET76_RECV_CLEAR    = 0x0400, /* clear buffer */
};

/* Device control mode commands */
enum zet76_control_commands
{
    ZET76_COMMAND_RESTART   = 1,
    ZET76_COMMAND_RESET     = 2,
    ZET76_COMMAND_RSI_READ  = 3,
    ZET76_COMMAND_RSI_WRITE = 4,
};

/* Device RS-485 parity checks */
enum zet76_parity_checks
{
    ZET76_PARITY_UNKNOWN = 0,
    ZET76_PARITY_NONE    = 1,
    ZET76_PARITY_ODD     = 2,
    ZET76_PARITY_EVEN    = 3,
    ZET76_PARITY_MARK    = 4,
    ZET76_PARITY_SPACE   = 5,
};

/* */
struct zet76_server;

/* */
struct zet76_device
{
    unsigned long long serial;
    unsigned long type;
    unsigned long reserved;
};


typedef void (zet76_status_callback)(struct zet76_server *server,
                                     const struct zet76_device *device,
                                     int state,
                                     int flags,
                                     void *arg);

typedef void (zet76_recv_callback)(struct zet76_server *server,
                                   const struct zet76_device *device,
                                   int recv_flags,
                                   void *arg);

typedef void (zet76_timer_callback)(struct zet76_server *server,
                                    int timer_id,
                                    void *arg);

/* Run once */
ZET76_EXPORT zet76_global_init(void);
ZET76_EXPORT zet76_global_free(void);


/* Base */
ZET76_EXPORT zet76_server_create(struct zet76_server **server);
ZET76_EXPORT zet76_server_free(struct zet76_server **server);


/* Set callback for device status change events */
ZET76_EXPORT zet76_notify_status(struct zet76_server *server,
                                 zet76_status_callback *status_callback,
                                 void *status_callback_arg);


/* Set callback for device recv events */
ZET76_EXPORT zet76_notify_recv(struct zet76_server *server,
                               zet76_recv_callback *recv_callback,
                               void *recv_callback_arg);


/* Set callback for periodic timers */
ZET76_EXPORT zet76_notify_timer(struct zet76_server *server,
                                zet76_timer_callback *timer_callback,
                                void *timer_callback_arg);


/* Periodic timers */
ZET76_EXPORT zet76_timer_start(struct zet76_server *server,
                               int timer_id,
                               int period_ms);

ZET76_EXPORT zet76_timer_stop(struct zet76_server *server,
                              int timer_id);


/* Scanner of devices available for connecting */
/* Scanning is implemented using ZDCP protocol */

ZET76_EXPORT zet76_scanner_start(struct zet76_server *server,
                                 const char *scan_ip,
                                 unsigned short scan_port);
ZET76_EXPORT zet76_scanner_stop(struct zet76_server *server,
                                const char *scan_ip,
                                unsigned short scan_port);

/*
  Send control command to device (or broadcasts to all if device == NULL).
  Command is sent by all started scanners.
*/
ZET76_EXPORT zet76_scanner_control(struct zet76_server *server,
                                   const struct zet76_device* device,
                                   int control_command);


/* Listener for active device connections that can be accepted */

/*
  If ip == NULL or "", server will listen to all available interfaces.
  If port == 0, default port will be used.
*/
ZET76_EXPORT zet76_listener_start(struct zet76_server *server,
                                  unsigned short port);
ZET76_EXPORT zet76_listener_stop(struct zet76_server *server,
                                 unsigned short port);

/* Connect to device detected by scanner */
ZET76_EXPORT zet76_enable(struct zet76_server *server,
                          const struct zet76_device *device,
                          int device_modes);
ZET76_EXPORT zet76_disable(struct zet76_server *server,
                           const struct zet76_device *device,
                           int device_modes);

/* Manually add/remove device with ZET76_DEVICE_CONNECTABLE flag */
/*
ZET76_EXPORT zet76_add(struct zet76_server *server,
                       const struct zet76_device *device,
                       const char *ip,
                       unsigned short port,
                       int fade_timeout_ms);
ZET76_EXPORT zet76_remove(struct zet76_server *server,
                          const struct zet76_device *device);
*/

/* Retrieve device info */
ZET76_EXPORT zet76_get_address(struct zet76_server *server,
                               const struct zet76_device *device,
                               int mode,
                               char *ip,
                               int ip_size,
                               unsigned short *port);

ZET76_EXPORT zet76_get_info(struct zet76_server *server,
                            const struct zet76_device *device,
                            int *state,
                            int *flags,
                            int *modes);

ZET76_EXPORT zet76_get_stats(struct zet76_server *server,
                             const struct zet76_device *device,
                             unsigned long long *rx_bytes);

ZET76_EXPORT zet76_get_rs485(struct zet76_server *server,
                             const struct zet76_device *device,
                             int *baud_rate,
                             int *parity);

ZET76_EXPORT zet76_get_peer(struct zet76_server *server,
                            const struct zet76_device *device,
                            char *peer_ip,
                            int peer_ip_size);

ZET76_EXPORT zet76_get_label(struct zet76_server *server,
                             const struct zet76_device *device,
                             wchar_t *label,
                             int label_chars);

/* Device data exchange */
ZET76_EXPORT zet76_send(struct zet76_server *server,
                        const struct zet76_device *device,
                        int send_flags,
                        const void *data,
                        int data_size);

ZET76_EXPORT zet76_recv(struct zet76_server *server,
                        const struct zet76_device *device,
                        int recv_flags,
                        void *data,
                        int *data_size);

/* Device control */
ZET76_EXPORT zet76_control(struct zet76_server *server,
                           const struct zet76_device *device,
                           int control_command,
                           unsigned long param,
                           const void *data,
                           int data_size);

#endif /* ZET76_INCLUDED */
