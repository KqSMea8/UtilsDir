#pragma once

/****************************************************************************/
/* Platform select */
#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "userenv.lib")

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#else

#error Platform is not supported

#endif

/****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

#include <libuv/include/uv_auto_link.h>

#include "zdcp.h"
#include "z76.h"

/****************************************************************************/
/* Debug tools */
#define Z76_NOP() do {} while (0)

#ifdef Z76_DEBUG
#define Z76_TRACE(fmt_, ...) \
	z76_trace("[%u] %s(): " fmt_ "\n", \
			GetCurrentThreadId(), __FUNCTION__, __VA_ARGS__)

#define Z76_TRACE_NODE(node_, fmt_, ...) \
	Z76_TRACE("<%u:0x%016" PRIx64 ":0x%x:0x%x:0x%x> " fmt_, \
			(node_)->device.type, (node_)->device.serial, \
			(node_)->state, (node_)->flags, (node_)->modes, __VA_ARGS__)

static void z76_trace(const char *format, ...)
{
	char msg[1024];
	va_list va;
	va_start(va, format);
	vsprintf_s(msg, sizeof(msg), format, va);
	OutputDebugStringA(msg);
	va_end(va);
}
#define Z76_ASSERT(cond_) Z76_NOP()
#else
#define Z76_TRACE(format_, ...)          Z76_NOP()
#define Z76_TRACE_NODE(node_, fmt_, ...) Z76_NOP()
#define Z76_ASSERT(cond_)                Z76_NOP()
#endif

/****************************************************************************/
/* Function error helpers - at error goto cleanup block */

#define Z76_EXIT(error_) \
	do { \
		error = (error_); \
		goto cleanup; \
	} while (0)

#define Z76_EXIT_IF_FALSE(cond_, error_) \
	do { \
		if (!(cond_)) \
			Z76_EXIT(error_); \
	} while (0)

#define Z76_EXIT_IF_NULL(ptr_, error_) \
	Z76_EXIT_IF_FALSE(((ptr_) != NULL), (error_))

#define Z76_EXIT_IF_ERROR(result_, error_) \
	Z76_EXIT_IF_FALSE(!(result_), (error_))

/****************************************************************************/
/* Invalid parameter validators - at error return immediately */

#define Z76_UNUSED_PARAM(param_) (param_)

#define Z76_VALIDATE_PARAM(cond_) \
	do { \
		if (!(cond_)) \
			return Z76_INVALID_PARAMETER; \
	} while (0)

#define Z76_VALIDATE_PARAM_PTR(ptr_) \
	Z76_VALIDATE_PARAM((ptr_) != NULL)

#define Z76_VALIDATE_PARAM_OBJECT(ptr_, magic_) \
	Z76_VALIDATE_PARAM((ptr_) != NULL && (ptr_)->magic == (magic_))

/****************************************************************************/
#define Z76_MALLOC(type_) ((type_ *) malloc(sizeof(type_)))

#define Z76_MALLOC_BYTES(size_) malloc(size_)

#define Z76_FREE_WITH(ptr_, free_func_) \
	do { \
		if ((ptr_) != NULL) { \
			(free_func_)(ptr_); \
			(ptr_) = NULL; \
		} \
	} while (0)

#define Z76_FREE(ptr_) Z76_FREE_WITH((ptr_), free)

#define Z76_UV_CLOSE(handle_, cb_) \
	uv_close((uv_handle_t *) (handle_), (cb_))

/****************************************************************************/
/* List structure and algorithms (based on BSD <sys/queue.h>) */
/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define Z76_LIST_HEAD(type_) \
	struct Z76_LIST_HEAD_##type_ { \
		struct type_ *first; \
	}

#define Z76_LIST_ENTRY(type_) \
	struct Z76_LIST_ENTRY_##type_ { \
		struct type_ *next; \
		struct type_ **prev_ptr; \
	}

#define Z76_LIST_FIRST(list_) \
	((list_)->first)

#define Z76_LIST_EMPTY(list_) \
	(Z76_LIST_FIRST(list_) == NULL)

#define Z76_LIST_NEXT(entry_, field_) \
	((entry_)->field_.next)

#define Z76_LIST_INIT(list_) \
	do { \
		Z76_LIST_FIRST(list_) = NULL; \
	} while (0)

#define Z76_LIST_INSERT(list_, entry_, field_) \
	do { \
		if (((entry_)->field_.next = (list_)->first) != NULL) \
			(list_)->first->field_.prev_ptr = &(entry_)->field_.next; \
		(list_)->first = (entry_); \
		(entry_)->field_.prev_ptr = &(list_)->first; \
	} while (0)

#define Z76_LIST_REMOVE(entry_, field_) \
	do { \
		if ((entry_)->field_.next != NULL) { \
			(entry_)->field_.next->field_.prev_ptr = \
					(entry_)->field_.prev_ptr; \
		} \
		*(entry_)->field_.prev_ptr = (entry_)->field_.next; \
	} while (0)

#define Z76_LIST_CLEAR(list_, free_func_) \
	while (Z76_LIST_FIRST(list_) != NULL) { \
		free_func_(Z76_LIST_FIRST(list_)); \
	}

#define Z76_LIST_FOREACH(var_, list_, field_) \
	for ((var_) = ((list_)->first); \
		(var_); \
		(var_) = ((var_)->field_.next))


/****************************************************************************/
#define Z76_LOOP_MAGIC 0x6C36377A /* 'z76l' */

enum z76_range_values {
	Z76_MAX_PORT              = 64000,
};

enum z76_timeouts {
	Z76_TIMEOUT_INFINITE        = 999999999,
	Z76_TIMEOUT_PULSE           = 2000,
	Z76_TIMEOUT_SCANNER_RESTART = 11000,
	Z76_TIMEOUT_LISTENER_READ   = 10000,
	Z76_TIMEOUT_NODE_FADE       = 30000,
	Z76_TIMEOUT_NODE_SEND       = 10000,
	Z76_TIMEOUT_NODE_REPORT     = 2000,
	Z76_TIMEOUT_NODE_RESPONSE   = 10000,
	Z76_TIMEOUT_NODE_7076       = 2000,
	Z76_TIMEOUT_NODE_DATA       = 33000,
	Z76_TIMEOUT_NODE_DATA_GSM   = 120000,
};

enum z76_buffer_sizes {
	Z76_SIZE_SCANNER_HOST = 1024,
	Z76_SIZE_RSI_SECTOR   = 512,
	Z76_SIZE_CHAIN_PACKET = 12,
};

enum z76_connect_flags {
	Z76_CONNECT_FLAG_ESTABLISHED = 0x0001,
	Z76_CONNECT_FLAG_REPORTED    = 0x0004,
};

/****************************************************************************/
struct z76_network;      /* network monitor */

struct z76_task;

struct z76_queue;
struct z76_queue_entry;

struct z76_timer;        /* periodic timer */

struct z76_scanner;      /* one scanner for each scan_addr */
struct z76_scanner_host; /* scanner socket for each interface */

struct z76_buffer;

struct z76_node;         /* device node */

/****************************************************************************/
static const char *z76_device_type_str(int type);
static const char *z76_device_flags_str(int flags);
static const char *z76_device_state_str(int state);
static const char *z76_device_zdcp_status_str(unsigned short status);
static int z76_device_compare(
		const struct z76_device *device1,
		const struct z76_device *device2);
static int z76_sockaddr_compare(
		const struct sockaddr_in *addr1,
		const struct sockaddr_in *addr2);
static int z76_cstr_to_dec(const char *cstr, unsigned long long *dec);

static int z76_zdcp_report_parse(const ZDCP_REPORT *report, const int size,
		struct z76_device *device, struct sockaddr_in *addr,
		unsigned short *status);

/****************************************************************************/
struct z76_queue {
	struct z76_loop *loop;
	uv_async_t async;
	uv_timer_t more_timer;
	uv_mutex_t tasks_mutex;
	Z76_LIST_HEAD(z76_queue_entry) tasks;

	bool closing;
};

struct z76_queue_entry {
	Z76_LIST_ENTRY(z76_queue_entry) entry;
	struct z76_task *task;
	uv_cond_t done_cond;
	uv_mutex_t done_mutex;
};

typedef void (*z76_task_cb)(struct z76_task *handle);

struct z76_task {
	size_t size;
	z76_task_cb cb;
	struct z76_queue *queue;
	int result;
};

static void z76_queue_init(struct z76_loop *loop, struct z76_queue *queue);
static void z76_queue_close(struct z76_queue *queue);
static void z76_queue_async_cb(uv_async_t *handle);
static void z76_queue_more_timer_cb(uv_timer_t *handle);

static void z76_task_init(struct z76_task *task,
		struct z76_queue *queue, size_t size, z76_task_cb cb);
static int z76_task_execute(struct z76_task *task);
static void z76_task_done(struct z76_task *task, int result);

/****************************************************************************/
struct z76_network {
	uv_interface_address_t *addr_list;
	int addr_count;

#ifdef WIN32
	HANDLE change_handle;
	OVERLAPPED change_overlapped;
#endif
};

static void z76_network_init(struct z76_network *network);
static void z76_network_close(struct z76_network *network);
static bool z76_network_check(struct z76_network *network);
static void z76_network_update(struct z76_network *network);

/****************************************************************************/
struct z76_timer {
	Z76_LIST_ENTRY(z76_timer) entry;

	int id;
	uint64_t period_ms;

	struct z76_loop *loop;
	uv_timer_t handle;
};

static void z76_timer_handle_cb(uv_timer_t *handle);
static void z76_timer_handle_close_cb(uv_handle_t *handle);

static void z76_timer_notify(struct z76_timer *timer);

/****************************************************************************/
struct z76_scanner {
	struct z76_loop *loop;
	struct sockaddr_in group_addr;
	uv_timer_t restart_timer;

	Z76_LIST_HEAD(z76_scanner_host) hosts;

	bool closing;
};

static void z76_scanner_init(struct z76_loop *loop,
		struct z76_scanner *scanner);
static void z76_scanner_restart(struct z76_scanner *scanner);
static void z76_scanner_close(struct z76_scanner *scanner);
static void z76_scanner_close_check(struct z76_scanner *scanner);
static void z76_scanner_restart_timer_close_cb(uv_handle_t *handle);
static void z76_scanner_restart_timer_cb(uv_timer_t *handle);

/****************************************************************************/
struct z76_scanner_host {
	Z76_LIST_ENTRY(z76_scanner_host) entry;

	struct z76_scanner *scanner;
	uv_udp_t udp;

	ZDCP_REPORT report;
};

static void z76_scanner_host_create(struct z76_scanner *scanner,
		const struct sockaddr_in *interface_addr);
static void z76_scanner_host_close(struct z76_scanner_host *host);
static void z76_scanner_host_udp_close_cb(uv_handle_t *handle);
static void z76_scanner_host_udp_send_cb(uv_udp_send_t* req, int status);
static void z76_scanner_host_udp_alloc_cb(uv_handle_t* handle,
		size_t suggested_size, uv_buf_t* buf);
static void z76_scanner_host_udp_recv_cb(uv_udp_t* handle, ssize_t nread,
		const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);

/****************************************************************************/
struct z76_buffer_chunk {
	Z76_LIST_ENTRY(z76_buffer_chunk) entry;

	uv_buf_t buf;
};

struct z76_buffer {
	Z76_LIST_HEAD(z76_buffer_chunk) chunks;

	size_t total_size;
	size_t first_offset;
};

static void z76_buffer_init(struct z76_buffer *buffer);
static void z76_buffer_reset(struct z76_buffer *buffer);
static size_t z76_buffer_size(struct z76_buffer *buffer);
static void z76_buffer_push(struct z76_buffer *buffer, const uv_buf_t *buf);
static size_t z76_buffer_pop(struct z76_bufffer *buffer,
		void *data, size_t data_size);

/****************************************************************************/
struct z76_node {
	Z76_LIST_ENTRY(z76_node) entry;

	struct z76_loop *loop;
	struct z76_device device;

	struct sockaddr_in addr;

	enum z76_device_states state;  /* current state (value) */
	int flags;  /* what we know about device (bits) */
	int modes;  /* what we want to do with device (bits) */

	uv_timer_t fade_timer;

	uv_timer_t state_timer;

	int connect_flags;
	uv_connect_t connect_req;
	uv_tcp_t cmd_tcp;
	uv_tcp_t data_tcp;
};

static struct z76_node *z76_node_create(struct z76_loop *loop,
		const struct z76_device *device);
static struct z76_node *z76_node_find(struct z76_loop *loop,
		const struct z76_device *device);
static void z76_node_close(struct z76_node *node);

static void z76_node_check_close(struct z76_node *node);
static void z76_node_fade_timer_close_cb(uv_handle_t *handle);
static void z76_node_cmd_tcp_close_cb(uv_handle_t *handle);
static void z76_node_data_tcp_close_cb(uv_handle_t *handle);

static void z76_node_fade_timer_cb(uv_timer_t *handle);

static void z76_node_refresh(struct z76_node *node);

static void z76_node_cmd_tcp_connect_cb(uv_connect_t *handle, int status);
static void z76_node_data_tcp_connect_cb(uv_connect_t *handle, int status);

static void z76_node_alloc_cb(uv_handle_t *handle,
		size_t suggested_size, uv_buf_t *buf);
static void z76_node_cmd_tcp_read_cb(uv_stream_t *handle,
		ssize_t nread, const uv_buf_t *buf);
static void z76_node_data_tcp_read_cb(uv_stream_t *handle,
		ssize_t nread, const uv_buf_t *buf);

static void z76_node_set_state(struct z76_node *node,
		enum z76_device_states state);
static void z76_node_notify_status(struct z76_node *node);
static void z76_node_handle_state(struct z76_node *node);

/****************************************************************************/
struct z76_loop {
	uint32_t magic;

	/* Device notification callbacks */
	z76_status_cb status_cb;
	void *status_cb_arg;

	z76_recv_cb recv_cb;
	void *recv_cb_arg;

	z76_timer_cb timer_cb;
	void *timer_cb_arg;

	/* Dispatcher loop */
	uv_loop_t uv;
	uv_thread_t thread;

	/* Pulse timer for periodic checks */
	uv_timer_t pulse_timer;

	struct z76_queue queue;

	/* Network settings change monitor */
	struct z76_network network;

	struct z76_scanner scanner;

	Z76_LIST_HEAD(z76_node) nodes;
	Z76_LIST_HEAD(z76_timer) timers;
};

static void z76_loop_thread(void *arg);

static void z76_loop_report(struct z76_loop *loop,
		const struct z76_device *device,
		const struct sockaddr_in *addr, unsigned short status);

static void z76_loop_pulse_timer_cb(uv_timer_t *handle);

/****************************************************************************/
struct z76_set_status_task {
	struct z76_task handle;
	z76_status_cb cb;
	void *arg;
};

struct z76_set_recv_task {
	struct z76_task handle;
	z76_recv_cb cb;
	void *arg;
};

struct z76_set_timer_task {
	struct z76_task handle;
	z76_timer_cb cb;
	void *arg;
};

struct z76_timer_task {
	struct z76_task handle;
	int timer_id;
	uint64_t period_ms;
};

struct z76_set_modes_task {
	struct z76_task handle;
	struct z76_device device;
	int mask_set;
	int mask_clear;
};

struct z76_get_info_task {
	struct z76_task handle;
	struct z76_device device;
	char *ip_addr_str;
	size_t ip_addr_str_size;
	uint16_t *port;
	int *state;
	int *flags;
	int *modes;
	int *rs485_baud_rate;
	int *rs485_parity;
};

struct z76_send_task {
	struct z76_task handle;
	struct z76_device device;
	int send_flags;
	const void *data;
	size_t data_size;
};

struct z76_recv_task {
	struct z76_task handle;
	struct z76_device device;
	int recv_flags;
	void *data;
	size_t *data_size_ptr;
};

static void z76_task_release_cb(struct z76_task *handle);
static void z76_task_set_status_cb(struct z76_task *handle);
static void z76_task_set_recv_cb(struct z76_task *handle);
static void z76_task_set_timer_cb(struct z76_task *handle);
static void z76_task_timer_cb(struct z76_task *handle);
static void z76_task_set_modes_cb(struct z76_task *handle);
static void z76_task_get_info_cb(struct z76_task *handle);
static void z76_task_send_cb(struct z76_task *handle);
static void z76_task_recv_cb(struct z76_task *handle);

/****************************************************************************/
static const char *z76_device_type_str(int type)
{
	switch (type) {
	case Z76_DEVICE_TYPE_7176:    return "ZET7176";
	//case Z76_DEVICE_TYPE_7076:    return "ZET7076";
	//case Z76_DEVICE_TYPE_7177:    return "ZET7177";
	}
	return "UNKNOWN";
}

static const char *z76_device_state_str(int state)
{
	switch (state) {
	case Z76_DEVICE_IDLE:         return "IDLE";
	case Z76_DEVICE_CONNECTED:    return "CONNECTED";
	case Z76_DEVICE_CONNECTING:   return "CONNECTING";
	case Z76_DEVICE_BUSY:         return "BUSY";
	case Z76_DEVICE_CONFLICTED:   return "CONFLICTED";
	}
	return "UNKNOWN_STATE";
}

static const char *z76_device_zdcp_status_str(unsigned short status)
{
	switch (status) {
	case ZDCP_REPORT_BUSY:     return "BUSY";
	case ZDCP_REPORT_PEER:     return "PEER";
	case ZDCP_REPORT_READY:    return "READY";
	case ZDCP_REPORT_CONFLICT: return "CONFLICT";
	}
	return "UNKNOWN_STATUS";
}

static int z76_device_compare(
		const struct z76_device *device1,
		const struct z76_device *device2)
{
	/*
	Z76_TRACE("<%s:0x%016llX> vs <%s:0x%016llX>",
				z76_device_type_str(device1->type), device1->serial,
				z76_device_type_str(device2->type), device2->serial);
	*/
	if (device1->type   < device2->type)   return -1;
	if (device1->type   > device2->type)   return +1;
	if (device1->serial < device2->serial) return -1;
	if (device1->serial > device2->serial) return +1;
	return 0;
}

static int z76_sockaddr_compare(
		const struct sockaddr_in *addr1,
		const struct sockaddr_in *addr2)
{
	if (addr1->sin_addr.s_addr < addr2->sin_addr.s_addr) return -1;
	if (addr1->sin_addr.s_addr > addr2->sin_addr.s_addr) return +1;
	if (addr1->sin_port < addr2->sin_port)               return -1;
	if (addr1->sin_port > addr2->sin_port)               return +1;
	return 0;
}

static int z76_zdcp_report_parse(const ZDCP_REPORT *report,
		const int size, struct z76_device *device,
		struct sockaddr_in *addr, unsigned short *status)
{
	if (size != sizeof(ZDCP_REPORT))
		return Z76_OBJECT_NOT_FOUND;

	if ((ntohl(report->header.signature) != ZDCP_SIGNATURE) ||
		(ntohs(report->header.id)        != ZDCP_ID_REPORT) ||
		(ntohs(report->header.length)    <  ZDCP_LENGTH_REPORT) ||
		(ntohs(report->port)             >  Z76_MAX_PORT))
	{
		return Z76_OBJECT_NOT_FOUND;
	}

	if (device != NULL) {
		device->serial   = ntohl(report->serial_hi);
		device->serial <<= 32;
		device->serial  += ntohl(report->serial_lo);
		device->type     = ntohl(report->type);
		device->reserved = 0;
	}
	if (addr != NULL) {
		addr->sin_family      = AF_INET;
		addr->sin_addr.s_addr = report->addr;
		addr->sin_port        = report->port;
	}
	if (status != NULL)
		*status = ntohs(report->status);

	return Z76_OK;
}

/****************************************************************************/
static void z76_queue_init(struct z76_loop *loop, struct z76_queue *queue)
{
	queue->loop = loop;

	uv_async_init(&loop->uv, &queue->async, z76_queue_async_cb);
	queue->async.data = queue;

	uv_timer_init(&loop->uv, &queue->more_timer);
	queue->more_timer.data = queue;

	uv_mutex_init(&queue->tasks_mutex);
	Z76_LIST_INIT(&queue->tasks);

	queue->closing = false;
}

static void z76_queue_close(struct z76_queue *queue)
{
	struct z76_queue_entry *entry = NULL;

	queue->closing = true;

	while (1) {
		uv_mutex_lock(&queue->tasks_mutex);
		entry = Z76_LIST_FIRST(&queue->tasks);
		if (entry)
			Z76_LIST_REMOVE(entry, entry);
		uv_mutex_unlock(&queue->tasks_mutex);
		if (!entry)
			break;

		entry->task->result = Z76_CLOSING;
		uv_cond_signal(&entry->done_cond);
	}

	Z76_UV_CLOSE(&queue->async, NULL);
	Z76_UV_CLOSE(&queue->more_timer, NULL);
	uv_mutex_destroy(&queue->tasks_mutex);
}

static void z76_queue_async_cb(uv_async_t *handle)
{
	struct z76_queue *queue = handle->data;

	if (queue->closing)
		return;

	uv_timer_start(&queue->more_timer, z76_queue_more_timer_cb, 0, 0);
}

static void z76_queue_more_timer_cb(uv_timer_t *handle)
{
	struct z76_queue *queue = handle->data;
	struct z76_queue_entry *entry = NULL;

	if (queue->closing)
		return;

	uv_mutex_lock(&queue->tasks_mutex);
	entry = Z76_LIST_FIRST(&queue->tasks);
	if (entry)
		Z76_LIST_REMOVE(entry, entry);
	uv_mutex_unlock(&queue->tasks_mutex);

	entry->task->cb(entry->task);
	uv_cond_signal(&entry->done_cond);

	if (queue->closing)
		return;

	uv_mutex_lock(&queue->tasks_mutex);
	entry = Z76_LIST_FIRST(&queue->tasks);
	uv_mutex_unlock(&queue->tasks_mutex);
	if (entry)
		z76_queue_async_cb(&queue->async);
}

static void z76_task_init(struct z76_task *task,
		struct z76_queue *queue, size_t size, z76_task_cb cb)
{
	memset(task, 0, size);
	task->queue = queue;
	task->size = size;
	task->cb = cb;
	task->result = Z76_OK;
}

static int z76_task_execute(struct z76_task *task)
{
	struct z76_queue_entry *entry = NULL;
	struct z76_queue *queue = task->queue;
	uv_thread_t self_thread = uv_thread_self();

	if (uv_thread_equal(&self_thread, &queue->loop->thread)) {
		task->cb(task);
		return task->result;
	}

	entry = Z76_MALLOC(struct z76_queue_entry);
	entry->task = task;

	uv_cond_init(&entry->done_cond);
	uv_mutex_init(&entry->done_mutex);

	uv_mutex_lock(&queue->tasks_mutex);
	Z76_LIST_INSERT(&queue->tasks, entry, entry);
	uv_mutex_unlock(&queue->tasks_mutex);

	uv_async_send(&queue->async);

	uv_cond_wait(&entry->done_cond, &entry->done_mutex);

	uv_cond_destroy(&entry->done_cond);
	uv_mutex_destroy(&entry->done_mutex);
	Z76_FREE(entry);

	return task->result;
}

/****************************************************************************/
static void z76_network_init(struct z76_network *network)
{
	network->addr_count = 0;
	network->addr_list = NULL;

#ifdef WIN32
	network->change_handle = NULL;
	network->change_overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif

	z76_network_update(network);
}

static void z76_network_close(struct z76_network *network)
{
#ifdef WIN32
	CancelIPChangeNotify(&network->change_overlapped);
	CloseHandle(network->change_overlapped.hEvent);
#endif

	if (network->addr_list) {
		uv_free_interface_addresses(network->addr_list, network->addr_count);
		network->addr_list = NULL;
		network->addr_count = 0;
	}
}

static bool z76_network_check(struct z76_network *network)
{
#ifdef WIN32
	BOOL network_changed;
	DWORD bytes = 0;

	network_changed = GetOverlappedResult(network->change_handle,
			&network->change_overlapped, &bytes, FALSE);
	if (network_changed) {
		z76_network_update(network);
		return true;
	}
#endif
	return false;
}

static void z76_network_update(struct z76_network *network)
{
#ifdef WIN32
	NotifyAddrChange(&network->change_handle, &network->change_overlapped);
#endif

	if (network->addr_list) {
		uv_free_interface_addresses(network->addr_list, network->addr_count);
		network->addr_list = NULL;
		network->addr_count = 0;
	}

	uv_interface_addresses(&network->addr_list, &network->addr_count);
}

/****************************************************************************/
static void z76_timer_handle_cb(uv_timer_t *handle)
{
	struct z76_timer *timer = handle->data;

	z76_timer_notify(timer);
}

static void z76_timer_handle_close_cb(uv_handle_t *handle)
{
	struct z76_timer *timer = handle->data;

	handle->data = NULL;
	if (!timer)
		return;

	Z76_FREE(timer);
}

static void z76_timer_notify(struct z76_timer *timer)
{
	struct z76_loop *loop = timer->loop;

	if (loop->timer_cb) {
		loop->timer_cb(loop, timer->id, loop->timer_cb_arg);
	}
}

/****************************************************************************/
static void z76_scanner_init(
		struct z76_loop *loop,
		struct z76_scanner *scanner)
{
	scanner->loop = loop;
	scanner->closing = false;

	Z76_LIST_INIT(&scanner->hosts);

	uv_ip4_addr(ZDCP_MULTICAST_ADDR_STR, ZDCP_MULTICAST_PORT,
			&scanner->group_addr);

	uv_timer_init(&loop->uv, &scanner->restart_timer);
	scanner->restart_timer.data = scanner;

	z76_scanner_restart(scanner);
}

static void z76_scanner_restart(struct z76_scanner *scanner)
{
	uv_timer_start(&scanner->restart_timer,
			z76_scanner_restart_timer_cb, 0, 0);
}

static void z76_scanner_restart_timer_close_cb(uv_handle_t *handle)
{
	struct z76_scanner *scanner = handle->data;

	if (!scanner)
		return;
	handle->data = NULL;

	z76_scanner_close_check(scanner);
}

static void z76_scanner_restart_timer_cb(uv_timer_t *handle)
{
	struct z76_scanner *scanner = handle->data;
	struct z76_network *network = NULL;
	struct z76_scanner_host *host = NULL;
	int i = 0;

	if (!Z76_LIST_EMPTY(&scanner->hosts)) {
		Z76_LIST_FOREACH(host, &scanner->hosts, entry) {
			z76_scanner_host_close(host);
		}
		return;
	}

	/* Open z76_scanner_host for each network interface */
	network = &scanner->loop->network;
	for (i = 0; i < network->addr_count; i++) {
		if (network->addr_list[i].is_internal)
			continue;
		if (network->addr_list[i].address.address4.sin_family != AF_INET)
			continue;

		z76_scanner_host_create(scanner,
				&network->addr_list[i].address.address4);
	}

	uv_timer_start(&scanner->restart_timer,
			z76_scanner_restart_timer_cb,
			Z76_TIMEOUT_SCANNER_RESTART, 0);
}

static void z76_scanner_close(struct z76_scanner *scanner)
{
	struct z76_scanner_host *host = NULL;

	scanner->closing = true;

	Z76_LIST_FOREACH(host, &scanner->hosts, entry) {
		z76_scanner_host_close(host);
	}
	Z76_UV_CLOSE(
			&scanner->restart_timer,
			z76_scanner_restart_timer_close_cb);
}

static void z76_scanner_close_check(struct z76_scanner *scanner)
{
	if (!Z76_LIST_EMPTY(&scanner->hosts))
		return;

	if (!scanner->closing) {
		z76_scanner_restart_timer_cb(&scanner->restart_timer);
		return;
	}

	if (scanner->restart_timer.data)
		return;
}

/****************************************************************************/
static void z76_scanner_host_create(
		struct z76_scanner *scanner,
		const struct sockaddr_in *interface_addr)
{
	char host_addr_str[INET_ADDRSTRLEN];
	struct sockaddr_in host_addr;
	ZDCP_PROBE probe;
	uv_buf_t probe_buf;

	struct z76_scanner_host *host = NULL;
	int r = 0;
	uv_udp_send_t *send = NULL;
	int error = Z76_OK;

	host = Z76_MALLOC(struct z76_scanner_host);
	if (!host)
		return;

	host->scanner = scanner;

	uv_udp_init(&scanner->loop->uv, &host->udp);
	host->udp.data = host;

	uv_inet_ntop(AF_INET, &interface_addr->sin_addr,
			host_addr_str, INET_ADDRSTRLEN);

	uv_ip4_addr(host_addr_str, ZDCP_MULTICAST_PORT, &host_addr);
	r = uv_udp_bind(&host->udp, (const struct sockaddr *) &host_addr,
			UV_UDP_REUSEADDR);

	uv_udp_set_membership(&host->udp,
			ZDCP_MULTICAST_ADDR_STR, host_addr_str, UV_JOIN_GROUP);

	r = uv_udp_recv_start(&host->udp,
			z76_scanner_host_udp_alloc_cb,
			z76_scanner_host_udp_recv_cb);

	probe.header.signature = htonl(ZDCP_SIGNATURE);
	probe.header.id        = htons(ZDCP_ID_PROBE);
	probe.header.length    = htons(ZDCP_LENGTH_PROBE);
	probe_buf = uv_buf_init((char *) &probe, sizeof(probe));

	send = Z76_MALLOC(uv_udp_send_t);
	if (send) {
		r = uv_udp_send(send, &host->udp, &probe_buf, 1,
				(const struct sockaddr *) &scanner->group_addr,
				z76_scanner_host_udp_send_cb);
	}

	Z76_LIST_INSERT(&scanner->hosts, host, entry);
}

static void z76_scanner_host_close(struct z76_scanner_host *host)
{
	Z76_TRACE("close host %p", host);
	Z76_UV_CLOSE(&host->udp, z76_scanner_host_udp_close_cb);
}

static void z76_scanner_host_udp_close_cb(uv_handle_t *handle)
{
	struct z76_scanner_host *host = handle->data;
	struct z76_scanner *scanner = NULL;

	handle->data = NULL;
	if (!host)
		return;

	scanner = host->scanner;

	Z76_TRACE("host %p closed", host);
	Z76_LIST_REMOVE(host, entry);
	Z76_FREE(host);

	z76_scanner_close_check(scanner);
}

static void z76_scanner_host_udp_send_cb(
		uv_udp_send_t* req,
		int status)
{
	Z76_FREE(req);
}

static void z76_scanner_host_udp_alloc_cb(
		uv_handle_t* handle,
		size_t suggested_size,
		uv_buf_t* buf)
{
	struct z76_scanner_host *host = handle->data;

	buf->base = (char *) &host->report;
	buf->len = sizeof(host->report);
}

static void z76_scanner_host_udp_recv_cb(
		uv_udp_t* handle,
		ssize_t nread,
		const uv_buf_t* buf,
		const struct sockaddr* addr,
		unsigned flags)
{
	struct z76_scanner_host *host = handle->data;
	struct z76_device device;
	struct sockaddr_in device_addr;
	char device_addr_str[INET_ADDRSTRLEN];
	unsigned short device_status;
	int r = 0;

	if (nread <= 0)
		return;

	r = z76_zdcp_report_parse((const ZDCP_REPORT *) buf->base,
			nread, &device, &device_addr, &device_status);
	if (r == Z76_OK) {
		uv_inet_ntop(AF_INET, &device_addr.sin_addr,
				device_addr_str, INET_ADDRSTRLEN);
#if 0
		Z76_TRACE("device <%s #%016llx> <%s:%u> %s",
				z76_device_type_str(device.type),
				device.serial, device_addr_str,
				ntohs(device_addr.sin_port),
				z76_device_zdcp_status_str(device_status));
#endif

		if (ntohs(device_addr.sin_port) <= Z76_MAX_PORT)
		{
			z76_loop_report(host->scanner->loop,
					&device, &device_addr, device_status);
		}
	}
}

/****************************************************************************/
static void z76_buffer_init(struct z76_buffer *buffer)
{
	Z76_LIST_INIT(&buffer->chunks);
	buffer->total_size = 0;
	buffer->first_offset = 0;
}

static void z76_buffer_reset(struct z76_buffer *buffer)
{
	struct z76_buffer_chunk *chunk = NULL;

	while (!Z76_LIST_EMPTY(&buffer->chunks)) {
		chunk = Z76_LIST_FIRST(&buffer->chunks);
		Z76_LIST_REMOVE(chunk, entry);
		Z76_FREE(chunk->buf.base);
		Z76_FREE(chunk);
	}

	buffer->total_size = 0;
	buffer->first_offset = 0;
}

static size_t z76_buffer_size(struct z76_buffer *buffer)
{
	return buffer->total_size;
}

static void z76_buffer_push(struct z76_buffer *buffer, const uv_buf_t *buf)
{
	struct z76_buffer_chunk *chunk = NULL;

	chunk = Z76_MALLOC(struct z76_buffer_chunk);
	chunk->buf.base = Z76_MALLOC_BYTES(buf->len);
	chunk->buf.len = buf->len;
	Z76_LIST_INSERT(&buffer->chunks, chunk, entry);

	buffer->total_size += chunk->buf.len;
}

static size_t z76_buffer_pop(struct z76_buffer *buffer,
		void *data, size_t data_size)
{
	struct z76_buffer_chunk *chunk = NULL;
	size_t chunk_size = 0;
	uint8_t *dst = data;
	size_t pos = 0;
	size_t left_size = data_size;

	while (left_size > 0) {
		chunk = Z76_LIST_FIRST(&buffer->chunks);
		if (!chunk)
			break;

		chunk_size = chunk->buf.len - buffer->first_offset;
		if (chunk_size > left_size)
			chunk_size = left_size;

		memcpy(dst + pos, chunk->buf.base + buffer->first_offset, chunk_size);
		pos += chunk_size;
		left_size -= chunk_size;

		if (buffer->first_offset + chunk_size < chunk->buf.len) {
			buffer->first_offset += chunk_size;
		} else {
			Z76_LIST_REMOVE(chunk, entry);
			Z76_FREE(chunk->buf.base);
			Z76_FREE(chunk);
			buffer->first_offset = 0;
		}
	}
}


/****************************************************************************/
static struct z76_node *z76_node_create(
		struct z76_loop *loop,
		const struct z76_device *device)
{
	struct z76_node *node = NULL;
	int error = Z76_OK;

	node = z76_node_find(loop, device);
	if (node)
		return node;

	node = Z76_MALLOC(struct z76_node);
	Z76_EXIT_IF_NULL(node, Z76_NOT_ENOUGH_MEMORY);

	node->loop = loop;
	node->device = *device;

	memset(&node->addr, 0, sizeof(node->addr));

	node->state = Z76_DEVICE_IDLE;
	node->flags = 0;
	node->modes = 0;

	uv_timer_init(&loop->uv, &node->fade_timer);
	node->fade_timer.data = node;

	node->connect_flags    = 0;
	node->connect_req.data = NULL;
	node->cmd_tcp.data     = NULL;
	node->data_tcp.data    = NULL;

	Z76_LIST_INSERT(&loop->nodes, node, entry);
	Z76_EXIT(Z76_OK);

cleanup:
	return node;
}

static struct z76_node *z76_node_find(
		struct z76_loop *loop,
		const struct z76_device *device)
{
	struct z76_node *node = NULL;

	Z76_LIST_FOREACH(node, &loop->nodes, entry) {
		if (z76_device_compare(&node->device, device) == 0)
			break;
	}
	return node;
}

static void z76_node_close(struct z76_node *node)
{
	Z76_LIST_REMOVE(node, entry);

	if (node->cmd_tcp.data)
		Z76_UV_CLOSE(&node->cmd_tcp, z76_node_cmd_tcp_close_cb);
	if (node->data_tcp.data)
		Z76_UV_CLOSE(&node->data_tcp, z76_node_data_tcp_close_cb);
	Z76_UV_CLOSE(&node->fade_timer, z76_node_fade_timer_close_cb);
}

static void z76_node_check_close(struct z76_node *node)
{
	if (node->fade_timer.data)
		return;
	if (node->data_tcp.data)
		return;
	if (node->cmd_tcp.data)
		return;
	if (node->connect_req.data)
		return;

	Z76_FREE(node);
}

static void z76_node_fade_timer_close_cb(uv_handle_t *handle)
{
	struct z76_node *node = handle->data;

	handle->data = NULL;
	if (!node)
		return;

	z76_node_check_close(node);
}

static void z76_node_cmd_tcp_close_cb(uv_handle_t *handle)
{
	struct z76_node *node = handle->data;

	handle->data = NULL;
	if (!node)
		return;

	z76_node_check_close(node);
}

static void z76_node_data_tcp_close_cb(uv_handle_t *handle)
{
	struct z76_node *node = handle->data;

	handle->data = NULL;
	if (!node)
		return;

	z76_node_check_close(node);
}

static void z76_node_fade_timer_cb(uv_timer_t *handle)
{
	struct z76_node *node = handle->data;

	node->flags &= ~Z76_DEVICE_CONNECTABLE;
	
	if (node->state == Z76_DEVICE_BUSY ||
			node->state == Z76_DEVICE_CONFLICTED) {
		z76_node_set_state(node, Z76_DEVICE_IDLE);
	}

	z76_node_notify_status(node);
	z76_node_handle_state(node);
}

static void z76_node_refresh(struct z76_node *node)
{
	uv_timer_start(
			&node->fade_timer,
			z76_node_fade_timer_cb,
			Z76_TIMEOUT_NODE_FADE,
			0);
}

static void z76_node_cmd_tcp_connect_cb(uv_connect_t *handle, int status)
{
	struct z76_node *node = handle->data;
	struct sockaddr_in data_addr;

	handle->data = NULL;
	if (status != 0) {
		Z76_TRACE_NODE(node, "cmd_tcp connect status %d", status);
		return;
	}

	Z76_TRACE_NODE(node, "cmd_tcp connected");

	uv_read_start((uv_stream_t *) &node->cmd_tcp,
			z76_node_alloc_cb, z76_node_cmd_tcp_read_cb);

	uv_tcp_init(&node->loop->uv, &node->data_tcp);
	node->data_tcp.data = node;

	data_addr = node->addr;
	data_addr.sin_port = htons(ntohs(data_addr.sin_port) + 1);

	node->connect_req.data = node;
	uv_tcp_connect(&node->connect_req, &node->data_tcp,
			(const struct sockaddr *) &data_addr,
			z76_node_data_tcp_connect_cb);
}

static void z76_node_data_tcp_connect_cb(uv_connect_t *handle, int status)
{
	struct z76_node *node = handle->data;

	handle->data = NULL;
	if (status != 0) {
		Z76_TRACE_NODE(node, "data_tcp connect status %d", status);
		return;
	}

	Z76_TRACE_NODE(node, "data_tcp connected");

	uv_read_start((uv_stream_t *) &node->data_tcp,
			z76_node_alloc_cb, z76_node_data_tcp_read_cb);

	node->connect_flags |= Z76_CONNECT_FLAG_ESTABLISHED;
	if (node->connect_flags & Z76_CONNECT_FLAG_REPORTED) {
		z76_node_set_state(node, Z76_DEVICE_CONNECTED);
	}
}

static void z76_node_alloc_cb(uv_handle_t *handle,
		size_t suggested_size, uv_buf_t *buf)
{
	buf->base = Z76_MALLOC_BYTES(suggested_size);
	buf->len = suggested_size;
}

static void z76_node_cmd_tcp_read_cb(uv_stream_t *handle,
		ssize_t nread, const uv_buf_t *buf)
{
	struct z76_node *node = handle->data;
	uint8_t *base = buf->base;
	struct z76_device device;
	struct sockaddr_in device_addr;
	uint16_t device_status = 0;
	int r = Z76_OK;

	r = z76_zdcp_report_parse((const ZDCP_REPORT *) base,
			nread, &device, &device_addr, &device_status);
	if (r == Z76_OK) {
		Z76_TRACE_NODE(node, "reported");

		node->connect_flags |= Z76_CONNECT_FLAG_REPORTED;
		if (node->connect_flags & Z76_CONNECT_FLAG_ESTABLISHED)
			z76_node_set_state(node, Z76_DEVICE_CONNECTED);
	}
	Z76_FREE(base);
}

static void z76_node_data_tcp_read_cb(uv_stream_t *handle,
		ssize_t nread, const uv_buf_t *buf)
{
	struct z76_node *node = handle->data;
	uint8_t *base = buf->base;

	Z76_TRACE_NODE(node, "data read %d", nread);
	Z76_FREE(base);
}

static void z76_node_set_state(struct z76_node *node,
		enum z76_device_states state)
{
	if (node->state == state)
		return;

	node->state = state;
	z76_node_notify_status(node);
}

static void z76_node_notify_status(struct z76_node *node)
{
	struct z76_loop *loop = node->loop;

	if (loop->status_cb) {
		loop->status_cb(
				loop,
				&node->device,
				node->state,
				node->flags,
				loop->status_cb_arg);
	}
}

static void z76_node_handle_state(struct z76_node *node)
{
	struct z76_loop *loop = node->loop;

	if (node->state != Z76_DEVICE_IDLE)
		return;

	if ((node->flags & Z76_DEVICE_CONNECTABLE) &&
			(node->modes & Z76_DEVICE_CONNECT)) {

		if (node->cmd_tcp.data || node->data_tcp.data)
			return;

		Z76_TRACE_NODE(node, "connecting");

		node->state = Z76_DEVICE_CONNECTING;

		uv_tcp_init(&loop->uv, &node->cmd_tcp);
		node->cmd_tcp.data = node;

		node->connect_flags = 0;
		node->connect_req.data = node;
		uv_tcp_connect(&node->connect_req, &node->cmd_tcp,
				(const struct sockaddr *) &node->addr,
				z76_node_cmd_tcp_connect_cb);
	}
}
/****************************************************************************/
static void z76_loop_thread(void *arg)
{
	struct z76_loop *loop = (struct z76_loop *) arg;

	uv_timer_init(&loop->uv, &loop->pulse_timer);
	loop->pulse_timer.data = loop;

	z76_network_init(&loop->network);
	z76_scanner_init(loop, &loop->scanner);

	uv_timer_start(&loop->pulse_timer, z76_loop_pulse_timer_cb,
			Z76_TIMEOUT_PULSE, Z76_TIMEOUT_PULSE);

	uv_run(&loop->uv, UV_RUN_DEFAULT);

	uv_loop_close(&loop->uv);

	loop->magic = 0;
	Z76_FREE(loop);
}

static void z76_loop_report(
		struct z76_loop *loop,
		const struct z76_device *device,
		const struct sockaddr_in *addr,
		unsigned short status)
{
	struct z76_node *node = NULL;
	int error = Z76_OK;

	node = z76_node_create(loop, device);

	if (status == ZDCP_REPORT_BUSY) {
		switch (node->state) {
		case Z76_DEVICE_CONNECTING:
		case Z76_DEVICE_CONNECTED:
			status = ZDCP_REPORT_READY;
			break;
		}
	}

	z76_node_refresh(node);

	if (!(node->flags & Z76_DEVICE_CONNECTABLE))
		Z76_EXIT(Z76_OK);

	if (z76_sockaddr_compare(addr, &node->addr) != 0)
		Z76_EXIT(Z76_OK);

	Z76_EXIT(Z76_OBJECT_NOT_FOUND);

cleanup:
	if (error == Z76_OK) {
		node->flags |= Z76_DEVICE_CONNECTABLE;
		node->addr = *addr;
		z76_node_notify_status(node);
	}
	z76_node_handle_state(node);
}

static void z76_loop_pulse_timer_cb(uv_timer_t *handle)
{
	struct z76_loop *loop = handle->data;

	z76_network_check(&loop->network);
//	z76_server_clean_nodes(loop);
}

/****************************************************************************/
static void z76_task_release_cb(struct z76_task *handle)
{
	struct z76_loop *loop = handle->queue->loop;
	struct z76_node *node = NULL;

	while (!Z76_LIST_EMPTY(&loop->timers)) {
		struct z76_timer *timer = Z76_LIST_FIRST(&loop->timers);
		Z76_LIST_REMOVE(timer, entry);
		Z76_UV_CLOSE(
				&timer->handle,
				z76_timer_handle_close_cb);
	}

	Z76_LIST_FOREACH(node, &loop->nodes, entry) {
		z76_node_close(node);
	}

	z76_network_close(&loop->network);
	z76_scanner_close(&loop->scanner);
	z76_queue_close(&loop->queue);

	Z76_UV_CLOSE(&loop->pulse_timer, NULL);
}

static void z76_task_set_status_cb(struct z76_task *handle)
{
	struct z76_set_status_task *task = (struct z76_set_status_task *) handle;
	struct z76_loop *loop = handle->queue->loop;

	loop->status_cb = task->cb;
	loop->status_cb_arg = task->arg;
}

static void z76_task_set_recv_cb(struct z76_task *handle)
{
	struct z76_set_recv_task *task = (struct z76_set_recv_task *) handle;
	struct z76_loop *loop = handle->queue->loop;

	loop->recv_cb = task->cb;
	loop->recv_cb_arg = task->arg;
}

static void z76_task_set_timer_cb(struct z76_task *handle)
{
	struct z76_set_timer_task *task = (struct z76_set_timer_task *) handle;
	struct z76_loop *loop = handle->queue->loop;

	loop->timer_cb = task->cb;
	loop->timer_cb_arg = task->arg;
}

static void z76_task_timer_cb(struct z76_task *handle)
{
	struct z76_timer_task *task = (struct z76_timer_task *) handle;
	struct z76_loop *loop = handle->queue->loop;
	struct z76_timer *timer = NULL;

	Z76_LIST_FOREACH(timer, &loop->timers, entry) {
		if (timer->id == task->timer_id) {
			if (task->period_ms > 0) {
				timer->period_ms = task->period_ms;
				uv_timer_start(&timer->handle, z76_timer_handle_cb,
						timer->period_ms, timer->period_ms);
			} else {
				Z76_LIST_REMOVE(timer, entry);
				uv_close((uv_handle_t *) &timer->handle,
						z76_timer_handle_close_cb);
			}
			return;
		}
	}

	if (task->period_ms <= 0)
		return;

	timer = Z76_MALLOC(struct z76_timer);
	timer->loop = loop;
	timer->id = task->timer_id;
	timer->period_ms = task->period_ms;
	Z76_LIST_INSERT(&loop->timers, timer, entry);
	uv_timer_init(&loop->uv, &timer->handle);
	timer->handle.data = timer;
	uv_timer_start(&timer->handle, z76_timer_handle_cb,
			timer->period_ms, timer->period_ms);
}

static void z76_task_set_modes_cb(struct z76_task *handle)
{
	struct z76_set_modes_task *task = (struct z76_set_modes_task *) handle;
	struct z76_loop *loop = handle->queue->loop;
	struct z76_node *node = NULL;

	node = z76_node_create(loop, &task->device);

	if (task->mask_set)
		node->modes |= task->mask_set;
	if (task->mask_clear)
		node->modes &= ~task->mask_clear;
}

static void z76_task_get_info_cb(struct z76_task *handle)
{
	struct z76_get_info_task *task = (struct z76_get_info_task *) handle;
	struct z76_loop *loop = handle->queue->loop;
	struct z76_node *node = NULL;
	int error = Z76_OK;

	node = z76_node_find(loop, &task->device);
	Z76_EXIT_IF_NULL(node, Z76_OBJECT_NOT_FOUND);

	if (task->ip_addr_str && task->ip_addr_str_size > 0) {
		uv_inet_ntop(AF_INET, &node->addr.sin_addr,
				task->ip_addr_str, task->ip_addr_str_size);
	}
	if (task->port)
		*task->port = ntohs(node->addr.sin_port);
	if (task->state)
		*task->state = node->state;
	if (task->flags)
		*task->flags = node->flags;
	if (task->modes)
		*task->modes = node->modes;
	if (task->rs485_baud_rate)
		*task->rs485_baud_rate = 0;
	if (task->rs485_parity)
		*task->rs485_parity = 0;
	return;

cleanup:
	if (task->ip_addr_str && task->ip_addr_str_size > 0)
		*task->ip_addr_str = '\0';
	if (task->port)
		*task->port = 0;
	if (task->state)
		*task->state = Z76_DEVICE_IDLE;
	if (task->flags)
		*task->flags = 0;
	if (task->modes)
		*task->modes = 0;
	if (task->rs485_baud_rate)
		*task->rs485_baud_rate = 0;
	if (task->rs485_parity)
		*task->rs485_parity = 0;
	handle->result = error;
}

static void z76_task_send_cb(struct z76_task *handle)
{
	struct z76_send_task *task = (struct z76_send_task *) handle;
	struct z76_loop *loop = handle->queue->loop;
	struct z76_node *node = NULL;

	node = z76_node_find(loop, &task->device);
	if (!node) {
		handle->result = Z76_INVALID_MODE;
		return;
	}
}

static void z76_task_recv_cb(struct z76_task *handle)
{
	struct z76_recv_task *task = (struct z76_recv_task *) handle;
	struct z76_loop *loop = handle->queue->loop;
	struct z76_node *node = NULL;

	node = z76_node_find(loop, &task->device);
	if (!node) {
		handle->result = Z76_INVALID_MODE;
		return;
	}
}

/****************************************************************************/
#if defined WIN32

void *z76_loop_calloc(size_t count, size_t size)
{
	return _calloc_dbg(count, size, _IGNORE_BLOCK, __FILE__, __LINE__);
}
#endif

int z76_loop_create(struct z76_loop **loop_ptr)
{
	struct z76_loop *loop = NULL;
	int r = 0;
	int error = Z76_OK;

	Z76_VALIDATE_PARAM_PTR(loop_ptr);

	loop = Z76_MALLOC(struct z76_loop);
	Z76_EXIT_IF_NULL(loop, Z76_NOT_ENOUGH_MEMORY);

	loop->magic         = Z76_LOOP_MAGIC;

	loop->status_cb     = NULL;
	loop->status_cb_arg = NULL;
	loop->recv_cb       = NULL;
	loop->recv_cb_arg   = NULL;
	loop->timer_cb      = NULL;
	loop->timer_cb_arg  = NULL;
	Z76_LIST_INIT(&loop->nodes);
	Z76_LIST_INIT(&loop->timers);

	uv_replace_allocator(
			malloc,
			realloc,
			z76_loop_calloc,
			free);
	uv_loop_init(&loop->uv);
	uv_replace_allocator(
			malloc,
			realloc,
			calloc,
			free);

	z76_queue_init(
			loop,
			&loop->queue);

	uv_thread_create(
			&loop->thread,
			z76_loop_thread,
			loop);

	*loop_ptr = loop;
	Z76_EXIT(Z76_OK);

cleanup:
	if (error != Z76_OK) {
		*loop_ptr = NULL;
	}

	return error;
}

int z76_loop_release(struct z76_loop **loop_ptr)
{
	struct z76_task task;
	struct z76_loop *loop = NULL;
	uv_thread_t self_thread = NULL;
	uv_thread_t loop_thread = NULL;

	Z76_VALIDATE_PARAM_PTR(loop_ptr);

	loop = *loop_ptr;

	/* Check whether loop still exists */
	if (!loop || loop->magic != Z76_LOOP_MAGIC)
		return Z76_INVALID_PARAMETER;

	self_thread = uv_thread_self();
	loop_thread = loop->thread;

	z76_task_init(
			&task,
			&loop->queue,
			sizeof(task),
			z76_task_release_cb);

	loop->magic = 0;

	z76_task_execute(&task);

	/* Wait for loop thread */
	if (!uv_thread_equal(&self_thread, &loop_thread))
		uv_thread_join(&loop_thread);

	*loop_ptr = NULL;
	return task.result;
}

int z76_set_status_cb(
		struct z76_loop *loop,
		z76_status_cb status_cb,
		void *arg)
{
	struct z76_set_status_task task;

	Z76_VALIDATE_PARAM_PTR(loop);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_set_status_cb);

	task.cb = status_cb;
	task.arg = arg;

	return z76_task_execute(&task.handle);
}

int z76_set_recv_cb(
		struct z76_loop *loop,
		z76_recv_cb recv_cb,
		void *arg)
{
	struct z76_set_recv_task task;

	Z76_VALIDATE_PARAM_PTR(loop);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_set_recv_cb);

	task.cb = recv_cb;
	task.arg = arg;

	return z76_task_execute(&task.handle);
}

int z76_set_timer_cb(
		struct z76_loop *loop,
		z76_timer_cb timer_cb,
		void *arg)
{
	struct z76_set_timer_task task;

	Z76_VALIDATE_PARAM_PTR(loop);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_set_timer_cb);

	task.cb = timer_cb;
	task.arg = arg;

	return z76_task_execute(&task.handle);
}

int z76_timer_start(
		struct z76_loop *loop,
		int timer_id,
		uint64_t period_ms)
{
	struct z76_timer_task task;

	Z76_VALIDATE_PARAM_PTR(loop);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_timer_cb);

	task.timer_id = timer_id;
	task.period_ms = period_ms;

	return z76_task_execute(&task.handle);
}

int z76_timer_stop(
		struct z76_loop *loop,
		int timer_id)
{
	struct z76_timer_task task;

	Z76_VALIDATE_PARAM_PTR(loop);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_timer_cb);

	task.timer_id = timer_id;
	task.period_ms = 0;

	return z76_task_execute(&task.handle);
}

int z76_enable(
		struct z76_loop *loop,
		const struct z76_device *device,
		int device_modes_mask)
{
	struct z76_set_modes_task task;

	Z76_VALIDATE_PARAM_PTR(loop);
	Z76_VALIDATE_PARAM_PTR(device);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_set_modes_cb);

	task.device = *device;
	task.mask_set = device_modes_mask;
	task.mask_clear = 0;

	return z76_task_execute(&task.handle);
}

int z76_disable(
		struct z76_loop *loop,
		const struct z76_device *device,
		int device_modes_mask)
{
	struct z76_set_modes_task task;

	Z76_VALIDATE_PARAM_PTR(loop);
	Z76_VALIDATE_PARAM_PTR(device);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_set_modes_cb);
	task.device = *device;
	task.mask_set = 0;
	task.mask_clear = device_modes_mask;
	return z76_task_execute(&task.handle);
}

int z76_get_addr(struct z76_loop *loop,
		const struct z76_device *device, int mode,
		char *ip_addr_str, size_t ip_addr_str_size, uint16_t *port)
{
	struct z76_get_info_task task;

	Z76_VALIDATE_PARAM_PTR(loop);
	Z76_VALIDATE_PARAM_PTR(device);
	Z76_VALIDATE_PARAM(!ip_addr_str || ip_addr_str_size > 0);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_get_info_cb);
	task.device = *device;
	task.ip_addr_str = ip_addr_str;
	task.ip_addr_str_size = ip_addr_str_size;
	task.port = port;
	return z76_task_execute(&task.handle);
}

int z76_get_info(struct z76_loop *loop,
		const struct z76_device *device,
		int *state, int *flags, int *modes)
{
	struct z76_get_info_task task;

	Z76_VALIDATE_PARAM_PTR(loop);
	Z76_VALIDATE_PARAM_PTR(device);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_get_info_cb);
	task.device = *device;
	task.state = state;
	task.flags = flags;
	task.modes = modes;
	return z76_task_execute(&task.handle);

}

int z76_get_rs485(struct z76_loop *loop,
		const struct z76_device *device,
		int *baud_rate, int *parity)
{
	struct z76_get_info_task task;

	Z76_VALIDATE_PARAM_PTR(loop);
	Z76_VALIDATE_PARAM_PTR(device);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_get_info_cb);
	task.device = *device;
	task.rs485_baud_rate = baud_rate;
	task.rs485_parity = parity;
	return z76_task_execute(&task.handle);
}

int z76_send(struct z76_loop *loop, const struct z76_device *device,
		int send_flags, const void *data, size_t data_size)
{
	struct z76_send_task task;

	Z76_VALIDATE_PARAM_PTR(loop);
	Z76_VALIDATE_PARAM_PTR(device);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_send_cb);
	task.device = *device;
	task.send_flags = send_flags;
	task.data = data;
	task.data_size = data_size;
	return z76_task_execute(&task.handle);
}

int z76_recv(struct z76_loop *loop, const struct z76_device *device,
		int recv_flags, void *data, size_t *data_size_ptr)
{
	struct z76_recv_task task;

	Z76_VALIDATE_PARAM_PTR(loop);
	Z76_VALIDATE_PARAM_PTR(device);

	z76_task_init(
			&task.handle,
			&loop->queue,
			sizeof(task),
			z76_task_recv_cb);
	task.device = *device;
	task.recv_flags = recv_flags;
	task.data = data;
	task.data_size_ptr = data_size_ptr;
	return z76_task_execute(&task.handle);
}
