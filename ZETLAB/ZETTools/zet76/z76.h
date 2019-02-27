/* z76 -- ZET SENSOR Ethernet Interface */
#ifndef Z76_INCLUDED
#define Z76_INCLUDED

#define Z76_VERSION_MAJOR 1
#define Z76_VERSION_MINOR 0
#define Z76_VERSION_BUILD 0x20161117

#include <stdint.h>

#ifdef __cplusplus
#define Z76_EXTERN extern "C"
#else
#define Z76_EXTERN extern
#endif

/* Error codes for all functions */
enum z76_errors
{
	Z76_OK                =  0,
	Z76_ERROR             = -1,
	Z76_INVALID_PARAMETER = -2,
	Z76_OBJECT_NOT_FOUND  = -3,
	Z76_NOT_ENOUGH_MEMORY = -4,
	Z76_NOT_SUPPORTED     = -5,
	Z76_INVALID_STATE     = -6,
	Z76_INVALID_MODE      = -7,
	Z76_TIMEOUT           = -8,
	Z76_AGAIN             = -9, /* loop busy, try again */
	Z76_CLOSING           = -10,
};

/* Supported device type values */
enum z76_device_types
{
	Z76_DEVICE_TYPE_UNKNOWN = 0,
	Z76_DEVICE_TYPE_7176    = 25,
//	Z76_DEVICE_TYPE_7076    = 46,
//	Z76_DEVICE_TYPE_7177    = 50,
};

/* Device state values */
enum z76_device_states
{
	Z76_DEVICE_IDLE        = 0,

	Z76_DEVICE_CONNECTING  = 1,
	Z76_DEVICE_CONNECTED   = 2,

	/* State 3 is reserved */
//    Z76_DEVICE_ACCEPTED    = 4,

	Z76_DEVICE_BUSY        = 5,
	Z76_DEVICE_CONFLICTED  = 6,

	//Z76_DEVICE_CONTROLLING = 7,
	//Z76_DEVICE_CONTROLLED  = 8,
};

/* Device modes bits */
enum z76_device_modes
{
	Z76_DEVICE_CONNECT = 0x0002,
	Z76_DEVICE_ACCEPT  = 0x0004,
	Z76_DEVICE_CONTROL = 0x0008,

	/* Drop all incoming chain data on active device */
	Z76_DEVICE_SKIP_CHAIN = 0x10000,
};

/* Device flags bits */
enum z76_device_flags
{
	Z76_DEVICE_CONNECTABLE  = Z76_DEVICE_CONNECT,
	Z76_DEVICE_ACCEPTABLE   = Z76_DEVICE_ACCEPT,
	Z76_DEVICE_CONTROLLABLE = Z76_DEVICE_CONTROL,
};

/* Device send flags */
enum z76_send_flags
{
	Z76_SEND_DONTWAIT = 0x0080, /* may return Z76_AGAIN */
};

/* Device recv event flags */
enum z76_recv_flags
{
	Z76_RECV_CHAIN    = 0x0001,
	Z76_RECV_RESPONSE = 0x0002,
	Z76_RECV_CONTROL  = 0x0008,
	/* Bits till 0x0040 are reserved for future use */

	Z76_RECV_DONTWAIT = 0x0080, /* may return Z76_AGAIN */
	Z76_RECV_ERROR    = 0x0100,
	Z76_RECV_TIMEOUT  = 0x0200,
	Z76_RECV_CLEAR    = 0x0400, /* clear buffer */
};

/* Device control mode commands */
enum z76_control_commands
{
	Z76_COMMAND_RESTART   = 1,
	Z76_COMMAND_RESET     = 2,
	Z76_COMMAND_RSI_READ  = 3,
	Z76_COMMAND_RSI_WRITE = 4,
};

/* Device RS-485 parity checks */
enum z76_parity_checks
{
	Z76_PARITY_UNKNOWN = 0,
	Z76_PARITY_NONE    = 1,
	Z76_PARITY_ODD     = 2,
	Z76_PARITY_EVEN    = 3,
	Z76_PARITY_MARK    = 4,
	Z76_PARITY_SPACE   = 5,
};

/* */
struct z76_loop;

/* */
struct z76_device {
	uint64_t serial;
	uint32_t type;
	uint32_t reserved;
};

typedef void (*z76_status_cb)(struct z76_loop *loop,
		const struct z76_device *device, int state, int flags, void *arg);

typedef void (*z76_recv_cb)(struct z76_loop *loop,
		const struct z76_device *device, int recv_flags, void *arg);

typedef void (*z76_timer_cb)(struct z76_loop *loop, int timer_id, void *arg);

/* Base */
Z76_EXTERN int z76_loop_create(struct z76_loop **loop_ptr);
Z76_EXTERN int z76_loop_release(struct z76_loop **loop_ptr);

/* Set callback for device status change events */
Z76_EXTERN int z76_set_status_cb(struct z76_loop *loop,
		z76_status_cb status_cb, void *arg);

/* Set callback for device recv events */
Z76_EXTERN int z76_set_recv_cb(struct z76_loop *loop,
		z76_recv_cb recv_cb, void *arg);

/* Set callback for periodic timers */
Z76_EXTERN int z76_set_timer_cb(struct z76_loop *loop,
		z76_timer_cb timer_cb, void *arg);

/* Periodic timers */
Z76_EXTERN int z76_timer_start(struct z76_loop *loop, int timer_id,
		uint64_t period_ms);
Z76_EXTERN int z76_timer_stop(struct z76_loop *loop, int timer_id);

/* Set or clear device modes mask */
Z76_EXTERN int z76_enable(struct z76_loop *loop,
		const struct z76_device *device, int device_modes_mask);
Z76_EXTERN int z76_disable(struct z76_loop *loop,
		const struct z76_device *device, int device_modes_mask);

/* Retrieve device info */
Z76_EXTERN int z76_get_addr(struct z76_loop *loop,
		const struct z76_device *device, int mode,
		char *ip_addr_str, size_t ip_addr_str_size, uint16_t *port);

Z76_EXTERN int z76_get_info(struct z76_loop *loop,
		const struct z76_device *device,
		int *state, int *flags, int *modes);

Z76_EXTERN int z76_get_rs485(struct z76_loop *loop,
		const struct z76_device *device,
		int *baud_rate, int *parity);

/* Device data exchange */
Z76_EXTERN int z76_send(struct z76_loop *loop,
		const struct z76_device *device,
		int send_flags, const void *data, size_t data_size);

Z76_EXTERN int z76_recv(struct z76_loop *loop,
		const struct z76_device *device,
		int recv_flags, void *data, size_t *data_size_ptr);

#endif /* Z76_INCLUDED */
