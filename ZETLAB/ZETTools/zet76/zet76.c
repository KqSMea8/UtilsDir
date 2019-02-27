#pragma once


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#include <stdlib.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/thread.h>
#include <event2/auto_link.h>

#include "zdcp.h"
#include "zet76.h"

/****************************************************************************/
/* Some guidelines */
/*
1. Prefer int over bool, size_t, etc.
2. Prefer enum over #define.
3. For this project, use spaces.
4. Try to align similar lines.
5. Do not trust input parameters.
6. Do not forget about multithreaded environment.
7. End names of macrofunction params with underscore (_).
8. Do not repeat yourself (DRY).
9. Prefer code simplicity and clearness over productivity (KISS).
10. Do not be too paranoic about memory economy, we are not at stone age.
11. Do not curse goto statement: it is not a taboo in C.
*/

/****************************************************************************/
/* Debug tools */
#define ZET76_NOP() do {} while (0)

#ifdef ZET76_DEBUG
#define ZET76_TRACE(format_, ...)                                           \
    zet76_trace("[%u] %s(): " format_ "\n",                                 \
           GetCurrentThreadId(), __FUNCTION__, __VA_ARGS__)

static void zet76_trace(const char *format, ...)
{
    char msg[1024];
    va_list va;
    va_start(va, format);
    vsprintf_s(msg, sizeof(msg), format, va);
    OutputDebugStringA(msg);
    va_end(va);
}
#define ZET76_ASSERT(cond_)         ZET76_NOP()
#else
#define ZET76_TRACE(format_, ...)   ZET76_NOP()
#define ZET76_ASSERT(cond_)         ZET76_NOP()
#endif

/****************************************************************************/
enum zet76_magic_numbers {
    ZET76_MAGIC_NULL   = 0,         /* not an object */
    ZET76_MAGIC_SERVER = 'z76s',    /* struct zet76_server */
    ZET76_MAGIC_NODE   = 'z76n',    /* struct zet76_node */
};

/****************************************************************************/
enum zet76_range_values {
    ZET76_MAX_PORT              = 64000,
    ZET76_MAX_CONTROL_DATA_SIZE = 512,
    ZET76_MAX_SNMP_OID_LEN      = 31,
    ZET76_MAX_SNMP_STRING_LEN   = ZET76_MAX_SNMP_OID_LEN * sizeof(int),

    ZET76_MIN_SNMP_REQUEST_ID   = 1,
    ZET76_MAX_SNMP_REQUEST_ID   = 0xFFFF,

    ZET76_MAX_LABEL_LEN         = 64,
};

/****************************************************************************/
enum zet76_timeouts {
    ZET76_TIMEOUT_INFINITE        = 999999999,
    ZET76_TIMEOUT_PULSE           = 2000,
    ZET76_TIMEOUT_SCANNER_REFRESH = 11000,
    ZET76_TIMEOUT_SNMP_BROADCAST  = 12000,
    ZET76_TIMEOUT_LISTENER_READ   = 10000,
    ZET76_TIMEOUT_NODE_FADE       = 30000,
    ZET76_TIMEOUT_NODE_SEND       = 10000,
    ZET76_TIMEOUT_NODE_REPORT     = 2000,
    ZET76_TIMEOUT_NODE_RESPONSE   = 10000,
    ZET76_TIMEOUT_NODE_7076       = 2000,
    ZET76_TIMEOUT_NODE_DATA       = 33000,
    ZET76_TIMEOUT_NODE_DATA_GSM   = 120000,
};

/****************************************************************************/
enum zet76_network_flags {
    /* Marks the first unicast ip address in network interface */
    ZET76_NETWORK_FLAG_FIRST = 0x0001,
};

/****************************************************************************/
enum zet76_buffer_sizes {
    ZET76_SIZE_SCANNER_HOST = 1024,
    ZET76_SIZE_SNMP_HOST    = 1024,
    ZET76_SIZE_RSI_SECTOR   = 512,
    ZET76_SIZE_CHAIN_PACKET = 12,
};

/****************************************************************************/
enum zet76_connect_flags {
    ZET76_CONNECT_FLAG_ESTABLISHED = 0x0001,
    ZET76_CONNECT_FLAG_REPORTED    = 0x0004,
};

/****************************************************************************/
enum zet76_control_send_states
{
    ZET76_CONTROL_SEND_IDLE = 0,    /* Ready to send next command */
    ZET76_CONTROL_SEND_WAIT = 1,    /* Command sent - waiting for response */
    ZET76_CONTROL_SEND_DONE = 2,    /* Got a response */
};

/****************************************************************************/
enum zet76_control_recv_states {
    ZET76_CONTROL_RECV_HEADER   = 0,    /* Reading ZDCP_HEADER */
    ZET76_CONTROL_RECV_SKIP     = 1,    /* Skipping packet */
    ZET76_CONTROL_RECV_PACKET   = 2,    /* Reading ZDCP_RESPONSE */
    ZET76_CONTROL_RECV_DATA     = 3,    /* Reading response data */
};

/****************************************************************************/
enum zet76_bufferevent_options {
    ZET76_BEV_OPTIONS = BEV_OPT_CLOSE_ON_FREE |
                        BEV_OPT_THREADSAFE |
                        BEV_OPT_DEFER_CALLBACKS |
                        BEV_OPT_UNLOCK_CALLBACKS,
};

/****************************************************************************/
enum zet76_snmp_ports {
    ZET76_SNMP_PORT = 161,
};

/****************************************************************************/
enum zet76_snmp_types {
    ZET76_SNMP_TYPE_UNKNOWN      = 0x00,

    /* Basic types */
    ZET76_SNMP_TYPE_SEQUENCE     = 0x00 | 0x20 | 0x10,
    ZET76_SNMP_TYPE_INTEGER      = 0x00 | 0x00 | 0x02,
    ZET76_SNMP_TYPE_STRING       = 0x00 | 0x00 | 0x04,
    ZET76_SNMP_TYPE_NULL         = 0x00 | 0x00 | 0x05,
    ZET76_SNMP_TYPE_OID          = 0x00 | 0x00 | 0x06,

    /* Application specific types */
    ZET76_SNMP_TYPE_IP           = 0x40 | 0x00 | 0x00,

    /* SNMP PDU types */
    ZET76_SNMP_TYPE_PDU_GET      = 0x80 | 0x20 | 0x00,
    ZET76_SNMP_TYPE_PDU_GET_NEXT = 0x80 | 0x20 | 0x01,
    ZET76_SMNP_TYPE_PDU_RESPONSE = 0x80 | 0x20 | 0x02,
    ZET76_SNMP_TYPE_PDU_SET      = 0x80 | 0x20 | 0x03,
};

/****************************************************************************/
enum zet76_snmp_rs232_types {
    ZET76_SNMP_RS232_TYPE_UNKNOWN = 0,
    ZET76_SNMP_RS232_TYPE_OTHER   = 1,
    ZET76_SNMP_RS232_TYPE_RS232   = 2,
    ZET76_SNMP_RS232_TYPE_RS422   = 3,
    ZET76_SNMP_RS232_TYPE_RS423   = 4,
    ZET76_SNMP_RS232_TYPE_V35     = 5,
    ZET76_SNMP_RS232_TYPE_X21     = 6,
};

/****************************************************************************/
enum zet76_snmp_tcp_states {
    ZET76_SNMP_TCP_UNKNOWN      = 0,
    ZET76_SNMP_TCP_CLOSED       = 1,
    ZET76_SNMP_TCP_LISTEN       = 2,
    ZET76_SNMP_TCP_SYN_SENT     = 3,
    ZET76_SNMP_TCP_SYN_RECEIVED = 4,
    ZET76_SNMP_TCP_ESTABLISHED  = 5,
    ZET76_SNMP_TCP_FIN_WAIT_1   = 6,
    ZET76_SNMP_TCP_FIN_WAIT_2   = 7,
    ZET76_SNMP_TCP_CLOSE_WAIT   = 8,
    ZET76_SNMP_TCP_LAST_ACK     = 9,
    ZET76_SNMP_TCP_CLOSING      = 10,
    ZET76_SNMP_TCP_TIME_WAIT    = 11,
    ZET76_SNMP_TCP_DELETE_TCB   = 12,
};

/****************************************************************************/
enum zet76_snmp_serial_masks {
    ZET76_SNMP_SERIAL_MASK              = 0xFFFF0000,

    ZET76_SNMP_SERIAL_MOXA_NE_4110A     = 0x41100000,
    ZET76_SNMP_SERIAL_MOXA_NPORT_W2150A = 0x21500000,
    ZET76_SNMP_SERIAL_MOXA_MGATE_MB3180 = 0x31800000,
};

/****************************************************************************/
#define ZET76_SNMP_COMMUNITY         "zet7076"
#define ZET76_SNMP_MOXA_NE_4110A     "NE-4110A_"
#define ZET76_SNMP_MOXA_NPORT_W2150A "NPortW2150A_"
#define ZET76_SNMP_MOXA_MGATE_MB3180 "MG-MB3180_"

#define ZET76_MODBUS_TCP_DEFAULT_PORT 502

/****************************************************************************/
#define ZET76_MALLOC(type_) ((type_ *) malloc(sizeof(type_)))

#define ZET76_FREE(ptr_, free_func_)                                        \
    do {                                                                    \
        if ((ptr_) != NULL) {                                               \
            (free_func_)(ptr_);                                             \
            (ptr_) = NULL;                                                  \
        }                                                                   \
    } while (0)


/****************************************************************************/
#define ZET76_LO_U32(u64_) ((unsigned long) ((u64_) & 0xFFFFffffUL))
#define ZET76_HI_U32(u64_) ((unsigned long) ((u64_) >> 32))

/****************************************************************************/
/* Function error helpers - at error goto cleanup block */

#define ZET76_EXIT(error_)                                                  \
    do {                                                                    \
        error = (error_);                                                   \
        goto cleanup;                                                       \
    } while (0)

#define ZET76_EXIT_IF_FALSE(cond_, error_)                                  \
    do {                                                                    \
        if (!(cond_))                                                       \
            ZET76_EXIT(error_);                                             \
    } while (0)

#define ZET76_EXIT_IF_NULL(ptr_, error_)                                    \
    ZET76_EXIT_IF_FALSE(((ptr_) != NULL), (error_))

#define ZET76_EXIT_IF_ERROR(result_, error_)                                \
    ZET76_EXIT_IF_FALSE(!(result_), (error_))


/****************************************************************************/
#define ZET76_VALIDATE_OBJECT(obj_, magic_)                                 \
    do {                                                                    \
        if ((obj_) == NULL || (obj_)->magic != (magic_))                    \
            return;                                                         \
    } while (0)


/****************************************************************************/
/* Invalid parameter validators - at error return immediately */

#define ZET76_UNUSED_PARAM(param_) (param_)

#define ZET76_VALIDATE_PARAM(cond_)                                         \
    do {                                                                    \
        if (!(cond_))                                                       \
            return ZET76_INVALID_PARAMETER;                                 \
    } while (0)

#define ZET76_VALIDATE_PARAM_PTR(ptr_)                                      \
    ZET76_VALIDATE_PARAM((ptr_) != NULL)

#define ZET76_VALIDATE_PARAM_OBJECT(ptr_, magic_)                           \
    ZET76_VALIDATE_PARAM((ptr_) != NULL && (ptr_)->magic == (magic_))


/****************************************************************************/
/* Multithreading synchronization guard */

#define ZET76_GUARD()                                                       \
    struct {                                                                \
        CRITICAL_SECTION cs;                                                \
    }

#define ZET76_GUARD_INIT(guard_)                                            \
    InitializeCriticalSectionAndSpinCount(&((guard_).cs), 2000)

#define ZET76_GUARD_FREE(guard_)                                            \
    DeleteCriticalSection(&((guard_).cs))

#define ZET76_GUARD_LOCK(guard_)                                            \
    EnterCriticalSection(&((guard_).cs))

#define ZET76_GUARD_UNLOCK(guard_)                                          \
    LeaveCriticalSection(&((guard_).cs))

#define ZET76_GUARD_TRYLOCK(guard_)                                         \
    TryEnterCriticalSection(&((guard_).cs))


/****************************************************************************/
#define ZET76_REFCOUNT()                                                    \
    struct {                                                                \
        volatile LONG atomic_count;                                         \
    }

#define ZET76_REFCOUNT_NULL()                                               \
    { 0 }

#define ZET76_REFCOUNT_INIT(ref_count_, init_value_)                        \
    do { (ref_count_).atomic_count = (init_value_); } while (0)

#define ZET76_REFCOUNT_INC(ref_count_)                                      \
    InterlockedIncrement(&(ref_count_).atomic_count)

#define ZET76_REFCOUNT_DEC(ref_count_)                                      \
    InterlockedDecrement(&(ref_count_).atomic_count)


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

#define ZET76_LIST_HEAD(type_)                                              \
    struct ZET76_LIST_HEAD_##type_ {                                        \
        struct type_ *first;                                                \
    }

#define ZET76_LIST_ENTRY(type_)                                             \
    struct ZET76_LIST_ENTRY_##type_ {                                       \
        struct type_ *next;                                                 \
        struct type_ **prev;                                                \
    }

#define ZET76_LIST_FIRST(list_)                                             \
    ((list_)->first)

#define ZET76_LIST_EMPTY(list_)                                             \
    (ZET76_LIST_FIRST(list_) == NULL)

#define ZET76_LIST_NEXT(entry_, field_)                                     \
    ((entry_)->field_.next)

#define ZET76_LIST_INIT(list_)                                              \
    do {                                                                    \
        ZET76_LIST_FIRST(list_) = NULL;                                     \
    } while (0)

#define ZET76_LIST_INSERT(list_, entry_, field_)                            \
    do {                                                                    \
        if (((entry_)->field_.next = (list_)->first) != NULL)               \
            (list_)->first->field_.prev = &(entry_)->field_.next;           \
        (list_)->first = (entry_);                                          \
        (entry_)->field_.prev = &(list_)->first;                            \
    } while (0)

#define ZET76_LIST_REMOVE(entry_, field_)                                   \
    do {                                                                    \
        if ((entry_)->field_.next != NULL)                                  \
            (entry_)->field_.next->field_.prev = (entry_)->field_.prev;     \
        *(entry_)->field_.prev = (entry_)->field_.next;                     \
    } while (0)

#define ZET76_LIST_CLEAR(list_, free_func_)                                 \
    while (ZET76_LIST_FIRST(list_) != NULL) {                               \
        free_func_(ZET76_LIST_FIRST(list_));                                \
    }

/****************************************************************************/
#define ZET76_SNMP_ENCODE(buf, pos, type, code)                             \
    do {                                                                    \
        int saved_pos;                                                      \
        (buf)[(pos)++] = (type);                                            \
        saved_pos = (pos)++;                                                \
        code;                                                               \
        (buf)[saved_pos] = (pos) - 1 - saved_pos;                           \
    } while (0)

/****************************************************************************/
struct zet76_network;

struct zet76_timer;             /* periodic timer */

struct zet76_scanner;           /* one scanner for each scan_addr */
struct zet76_scanner_host;      /* scanner socket for each interface */

struct zet76_snmp_oid;          /* ASN.1 OBJECT-IDENTIFIER */
struct zet76_snmp_string;       /* ASN.1 OCTET-STRING */
struct zet76_snmp_value;        /* ASN.1 union value */
struct zet76_snmp_object;       /* SNMP object */
struct zet76_snmp;              /* one SNMP scanner for each server */
struct zet76_snmp_host;         /* SNMP session for each ip address */
struct zet76_snmp_peer;         /* SNMP agent info */

struct zet76_listener;          /* one listener for each listen_addr */
struct zet76_listener_host;     /* server socket for each ip address */
struct zet76_listener_peer;     /* socket for each accepted connection */

struct zet76_node;

/****************************************************************************/
struct zet76_server {
    unsigned long magic;    /* to validate object ptr */

    ZET76_GUARD() guard;

    /* Device notification */
    zet76_status_callback *status_callback;
    void *status_callback_arg;

    zet76_recv_callback *recv_callback;
    void *recv_callback_arg;

    zet76_timer_callback *timer_callback;
    void *timer_callback_arg;

    /* Events base to dispatch */
    struct event_base *base;

    /* Periodic timer (pulse) */
    struct event *pulse_event;

    /* Network settings change monitor */
    struct event *network_event;
#ifdef WIN32
    HANDLE network_change_handle;
    OVERLAPPED network_change;
#endif

    struct zet76_network *networks;
    int networks_count;

    ZET76_LIST_HEAD(zet76_timer) timers;
    ZET76_LIST_HEAD(zet76_scanner) scanners;
    ZET76_LIST_HEAD(zet76_snmp) snmps;
    ZET76_LIST_HEAD(zet76_listener) listeners;
    ZET76_LIST_HEAD(zet76_node) nodes;

#ifdef WIN32
    WSADATA winsock;

    HANDLE thread_handle;
    DWORD thread_id;
#endif
};

/****************************************************************************/
struct zet76_network {
    struct in_addr addr;
    int flags;

    /* This struct must be of size that is multiple of 8 bytes */
};

/****************************************************************************/
struct zet76_timer {
    ZET76_LIST_ENTRY(zet76_timer) entry;

    int id;
    int period_ms;

    struct zet76_server *server;
    struct event *event;
};

/****************************************************************************/
struct zet76_scanner {
    ZET76_LIST_ENTRY(zet76_scanner) entry;

    struct sockaddr_in scan_addr;
    struct zet76_server *server;
    struct event *refresh_event;

    ZET76_REFCOUNT() ref_count;

    ZET76_LIST_HEAD(zet76_scanner_host) hosts;
};

/****************************************************************************/
struct zet76_scanner_host {
    ZET76_LIST_ENTRY(zet76_scanner_host) entry;

    evutil_socket_t sock;
    struct event *report_event;
};

/****************************************************************************/
struct zet76_snmp_oid {
    int len;
    int integers[ZET76_MAX_SNMP_OID_LEN];
};

/****************************************************************************/
struct zet76_snmp_string {
    int len;
    char octets[ZET76_MAX_SNMP_STRING_LEN];
};

/****************************************************************************/
struct zet76_snmp_value {
    int type;

    union {
        long sequence_len;
        long long integer;
        IN_ADDR ip;
        struct zet76_snmp_oid oid;
        struct zet76_snmp_string string;
    } data;
};

/****************************************************************************/
struct zet76_snmp_object {
    struct zet76_snmp_oid name;
    struct zet76_snmp_value value;
};

/****************************************************************************/
struct zet76_snmp {
    ZET76_LIST_ENTRY(zet76_snmp) entry;

    struct sockaddr_in broadcast_addr;
    struct zet76_server *server;

    struct zet76_snmp_oid sys_name_oid;
    struct zet76_snmp_oid sys_location_oid;
    struct zet76_snmp_oid tcp_state_oid;
    struct zet76_snmp_oid rs232_type_oid;
    struct zet76_snmp_oid rs232_speed_oid;
    struct zet76_snmp_oid rs232_bits_oid;
    struct zet76_snmp_oid rs232_parity_oid;

    ZET76_REFCOUNT() ref_count;

    ZET76_LIST_HEAD(zet76_snmp_host) hosts;
};

/****************************************************************************/
struct zet76_snmp_host {
    ZET76_LIST_ENTRY(zet76_snmp_host) entry;

    struct sockaddr_in addr;
    struct zet76_snmp *snmp;

    evutil_socket_t sock;               /* broadcast UDP */
    struct event *broadcast_event;      /* broadcast periodic timer */

    int broadcast_request_id;

    ZET76_LIST_HEAD(zet76_snmp_peer) peers;
};

/****************************************************************************/
struct zet76_snmp_peer {
    ZET76_LIST_ENTRY(zet76_snmp_peer) entry;

    struct sockaddr_in addr;

    struct zet76_device id;
    int mb3180_avail;
    int tcp_state;
    int baud_rate;
    int parity;

    unsigned short request_id;

    struct event *fade_event;
};

/****************************************************************************/
struct zet76_listener {
    ZET76_LIST_ENTRY(zet76_listener) entry;

    unsigned short listen_port;
    struct zet76_server *server;

    ZET76_REFCOUNT() ref_count;

    ZET76_LIST_HEAD(zet76_listener_host) hosts;
    ZET76_LIST_HEAD(zet76_listener_peer) peers;
};

/****************************************************************************/
struct zet76_listener_host {
    ZET76_LIST_ENTRY(zet76_listener_host) entry;

    struct sockaddr_in addr;

    struct evconnlistener *cmd;
    struct evconnlistener *dat;
};

/****************************************************************************/
struct zet76_listener_peer {
    ZET76_LIST_ENTRY(zet76_listener_peer) entry;

    struct sockaddr_in addr;

    struct zet76_listener *listener;
    struct event *read_event;
    struct zet76_device id;

    evutil_socket_t sock_cmd;
    evutil_socket_t sock_dat;
};

/****************************************************************************/
struct zet76_node {
    unsigned long magic;    /* to validate struct zet76_node */

    ZET76_LIST_ENTRY(zet76_node) entry;

    struct zet76_server *server;
    struct event *fade_event;           /* heartbeat fade event */
    struct event *disable_event;        /* disable event */

    struct zet76_device id;
    int baud_rate;
    int parity;

    char label[ZET76_MAX_LABEL_LEN];
    int label_len;
    struct in_addr peer_addr;
    int peer_addr_avail;

    int state;  /* current state (value) */
    int flags;  /* that's what we know about device (bits) */
    int modes;  /* that's what we want to do with device (bits) */

    /* For ZET76_DEVICE_CONNECT (passive mode) */
    struct sockaddr_in connect_addr;
    struct bufferevent *connect_cmd;    /* modbus command socket */
    struct bufferevent *connect_dat;    /* chain data socket */
    int connect_flags;                  /* connecting flags */

    /* For ZET76_DEVICE_ACCEPT (active mode) */
    struct sockaddr_in accept_addr;
    struct bufferevent *accept_cmd;     /* modbus command socket */
    struct bufferevent *accept_dat;     /* chain data socket */

    /* For ZET76_DEVICE_CONTROL (ZDCP via TCP) */
    struct sockaddr_in control_addr;
    struct bufferevent *control_bev;    /* ZDCP control socket */
    struct evbuffer *control_buf;       /* response buffer */
    int control_send;                   /* send phase */
    int control_recv;                   /* recv phase */
    unsigned long control_zdcp_seq;     /* last sent control sequence */
    unsigned long control_zdcp_cmd;     /* last sent control command */
    unsigned long control_zdcp_res;     /* last recv control response */
    int control_recv_size;              /* bytes left to recv */

    unsigned long long rx_bytes;        /* total bytes received */
};

/****************************************************************************/
static int zet76_device_type_supported(int type);
static const char *zet76_device_type_str(int type);
static const char *zet76_device_flags_str(int flags);
static const char *zet76_device_state_str(int state);

static void zet76_timeval_init(struct timeval *tv, int timeout_ms);
static void zet76_event_add(struct event *ev, int timeout_ms);
static void zet76_bufferevent_set_timeouts(struct bufferevent *bev,
                                           int timeout_read_ms,
                                           int timeout_write_ms);

static int zet76_sockaddr_compare(const struct sockaddr_in *addr1,
                                  const struct sockaddr_in *addr2);

static int zet76_cstr_to_dec(const char *cstr, unsigned long long *dec);

static int zet76_zdcp_report_parse(const ZDCP_REPORT *report,
                                   const int size,
                                   struct zet76_device *device,
                                   struct sockaddr_in *addr,
                                   unsigned short *status,
                                   const struct in_addr **peer_addr,
                                   const char **label,
                                   int *label_len);

/****************************************************************************/
#ifdef WIN32
static DWORD WINAPI zet76_server_thread_win32(LPVOID arg);
static DWORD WINAPI zet76_node_thread_win32(LPVOID arg);
#endif

/****************************************************************************/
static void zet76_server_thread(struct zet76_server *server);

static void zet76_server_monitor_network(struct zet76_server *server);
static void zet76_server_clean_nodes(struct zet76_server *server);

static void zet76_server_on_pulse(evutil_socket_t sock,
                                  short which, void *arg);
static void zet76_server_on_network(evutil_socket_t sock,
                                    short which, void *arg);

/****************************************************************************/
static void zet76_timer_free(struct zet76_timer *timer);
static void zet76_timer_on_tick(evutil_socket_t sock,
                                short which, void *arg);

/****************************************************************************/
static void zet76_scanner_free(struct zet76_scanner *scanner);

static void zet76_scanner_restart(struct zet76_scanner *scanner);
static void zet76_scanner_on_refresh(evutil_socket_t sock,
                                     short which, void *arg);
static void zet76_scanner_host_create(struct zet76_scanner *scanner,
                                      const struct in_addr *addr);
static void zet76_scanner_host_free(struct zet76_scanner_host *host);
static void zet76_scanner_on_report(evutil_socket_t sock,
                                    short which, void *arg);

/****************************************************************************/
static int zet76_snmp_compare_oid(const struct zet76_snmp_oid *oid1,
                                  const struct zet76_snmp_oid *oid2);
static void zet76_snmp_encode_oid(struct zet76_snmp_oid *oid,
                                  const struct zet76_snmp_oid *parent,
                                  ...);
static int zet76_snmp_encode_request(void *data,
                                     int size,
                                     unsigned char request_type,
                                     int request_id,
                                     const struct zet76_snmp_oid *name);
static int zet76_snmp_decode_value(const void *data,
                                   int size,
                                   struct zet76_snmp_value *value);
static int zet76_snmp_decode_response(const void *data,
                                      int size,
                                      int *request_id,
                                      struct zet76_snmp_object *object);
static void zet76_snmp_start(struct zet76_server *server,
                             unsigned short snmp_port);
static void zet76_snmp_stop(struct zet76_server *server,
                            unsigned short snmp_port);
static void zet76_snmp_free(struct zet76_snmp *snmp);

static void zet76_snmp_restart(struct zet76_snmp *snmp);
static void zet76_snmp_host_create(struct zet76_snmp *snmp,
                                   const struct in_addr *addr);
static void zet76_snmp_host_free(struct zet76_snmp_host *host);
static struct zet76_snmp_peer *zet76_snmp_peer_create(
                                        struct zet76_snmp_host *host,
                                        const struct zet76_device *device,
                                        const IN_ADDR in_addr);
static void zet76_snmp_peer_free(struct zet76_snmp_peer *peer);
static void zet76_snmp_peer_on_fade(evutil_socket_t sock,
                                    short which, void *arg);
static void zet76_snmp_on_response(evutil_socket_t sock,
                                   short which, void *arg);
static void zet76_snmp_on_connect(struct zet76_server *server,
                                  const struct zet76_device *id);
static int zet76_snmp_peer_is_mb3180(struct zet76_snmp_peer *peer);

/****************************************************************************/
static void zet76_listener_free(struct zet76_listener *listener);

static void zet76_listener_restart(struct zet76_listener *listener);
static void zet76_listener_host_create(struct zet76_listener *listener,
                                       const struct in_addr *addr);
static void zet76_listener_host_free(struct zet76_listener_host *host);
static void zet76_listener_on_cmd(struct evconnlistener *ecl,
                                  evutil_socket_t sock,
                                  struct sockaddr *addr,
                                  int addr_size,
                                  void *arg);
static void zet76_listener_on_dat(struct evconnlistener *ecl,
                                  evutil_socket_t sock,
                                  struct sockaddr *addr,
                                  int addr_size,
                                  void *arg);
static void zet76_listener_on_report(evutil_socket_t sock,
                                     short which, void *arg);
static void zet76_listener_accept(struct zet76_listener_peer *peer);

static void zet76_listener_peer_free(struct zet76_listener_peer *peer);

/****************************************************************************/
static int zet76_device_compare(const struct zet76_device *device1,
                                const struct zet76_device *device2);

static void zet76_device_on_report(struct zet76_server *server,
                                   const struct zet76_device *device,
                                   const struct sockaddr_in *addr,
                                   unsigned short status,
                                   int baud_rate,
                                   int parity,
                                   const struct in_addr *peer_addr,
                                   const char *label,
                                   int label_len);

static void zet76_node_set_state(struct zet76_node *node,
                                 int state);


/****************************************************************************/
static void zet76_node_thread(struct zet76_node *node);

static struct zet76_node *zet76_node_create(struct zet76_server *server,
                                            const struct zet76_device *id);
static struct zet76_node *zet76_node_open(struct zet76_server *server,
                                          const struct zet76_device *id);
static void zet76_node_free(struct zet76_node *node);

static void zet76_node_notify_status(struct zet76_node *node);
static void zet76_node_notify_recv(struct zet76_node *node, int recv_flags);
static void zet76_node_handle_state(struct zet76_node *node);

static void zet76_node_stop_connect(struct zet76_node *node);
static void zet76_node_stop_control(struct zet76_node *node);
static void zet76_node_stop_accept(struct zet76_node *node);
static void zet76_node_on_disable(evutil_socket_t sock,
                                  short which, void *arg);

static void zet76_node_on_fade(evutil_socket_t sock, short which, void *arg);
static void zet76_node_on_response(evutil_socket_t sock,
                                   short which, void *arg);

static void zet76_node_on_read(struct bufferevent *bev, void *arg);
static void zet76_node_on_control(struct bufferevent *bev, void *arg);
static void zet76_node_on_event(struct bufferevent *bev,
                                short which, void *arg);

static void zet76_node_control_reset(struct zet76_node *node);

/****************************************************************************/

static volatile LONG zet76_global_refs = 0;

ZET76_EXPORT zet76_global_init(void)
{
    if (InterlockedIncrement(&zet76_global_refs) == 1) {
        ZET76_TRACE("");
#ifdef WIN32
        evthread_use_windows_threads();
#endif
    }
    return ZET76_OK;
}

ZET76_EXPORT zet76_global_free(void)
{
    if (InterlockedCompareExchange(&zet76_global_refs, 0, 0) == 0)
        return ZET76_OK;

    if (InterlockedDecrement(&zet76_global_refs) == 0) {
        ZET76_TRACE("");
#if LIBEVENT_VERSION_NUMBER >= 0x02010200
        libevent_global_shutdown();
#endif
    }
    return ZET76_OK;
}

/****************************************************************************/
static int zet76_device_type_supported(int type)
{
    switch (type) {
    case ZET76_DEVICE_TYPE_7176:
    case ZET76_DEVICE_TYPE_7076:
    case ZET76_DEVICE_TYPE_7177:
        return 1;
    }
    return 0;
}

static const char *zet76_device_type_str(int type)
{
    switch (type) {
    case ZET76_DEVICE_TYPE_7176:    return "ZET7176";
    case ZET76_DEVICE_TYPE_7076:    return "ZET7076";
    case ZET76_DEVICE_TYPE_7177:    return "ZET7177";
    }
    return "UNKNOWN";
}

static const char *zet76_device_state_str(int state)
{
    switch (state) {
    case ZET76_DEVICE_IDLE:         return "IDLE";
    case ZET76_DEVICE_CONNECTED:    return "CONNECTED";
    case ZET76_DEVICE_CONNECTING:   return "CONNECTING";
    case ZET76_DEVICE_ACCEPTED:     return "ACCEPTED";
    case ZET76_DEVICE_BUSY:         return "BUSY";
    case ZET76_DEVICE_CONFLICTED:   return "CONFLICTED";
    case ZET76_DEVICE_CONTROLLED:   return "CONTROLLED";
    case ZET76_DEVICE_CONTROLLING:  return "CONTROLLING";
    }
    return "UNKNOWN_STATE";
}

/****************************************************************************/
static void zet76_timeval_init(struct timeval *tv, int timeout_ms)
{
    tv->tv_sec  = (timeout_ms / 1000);
    tv->tv_usec = (timeout_ms % 1000) * 1000;
}

static void zet76_event_add(struct event *ev, int timeout_ms)
{
    struct timeval tv;

    if (timeout_ms >= 0 && timeout_ms != ZET76_TIMEOUT_INFINITE) {
        zet76_timeval_init(&tv, timeout_ms);
        event_add(ev, &tv);
    } else {
        event_add(ev, NULL);
    }
}

static void zet76_bufferevent_set_timeouts(struct bufferevent *bev,
                                           int timeout_read_ms,
                                           int timeout_write_ms)
{
    struct timeval tv_read;
    struct timeval tv_write;

    zet76_timeval_init(&tv_read,  timeout_read_ms);
    zet76_timeval_init(&tv_write, timeout_read_ms);
    bufferevent_set_timeouts(bev, &tv_read, &tv_write);
}

/****************************************************************************/
static int zet76_sockaddr_compare(const struct sockaddr_in *addr1,
                                  const struct sockaddr_in *addr2)
{
    /* These numbers are in network byte order,
       but we don't really need compare, just some ordering */
    if (addr1->sin_addr.s_addr < addr2->sin_addr.s_addr)    return -1;
    if (addr1->sin_addr.s_addr > addr2->sin_addr.s_addr)    return +1;
    if (addr1->sin_port < addr2->sin_port)                  return -1;
    if (addr1->sin_port > addr2->sin_port)                  return +1;
    return 0;
}

/****************************************************************************/
static int zet76_cstr_to_dec(const char *cstr, unsigned long long *dec)
{
    int digits = 0;

    *dec = 0;
    /* Skip leading whitespaces */
    while (*cstr == ' ' || *cstr == '\t') {
        cstr++;
    }
    while (*cstr >= '0' && *cstr <= '9') {
        *dec = ((*dec) * 10) + ((*cstr) - '0');
        cstr++;
        digits++;
    }

    return digits;
}

/****************************************************************************/
static int zet76_zdcp_report_parse(const ZDCP_REPORT *report,
                                   const int size,
                                   struct zet76_device *device,
                                   struct sockaddr_in *addr,
                                   unsigned short *status,
                                   const struct in_addr **peer_addr,
                                   const char **label,
                                   int *label_len)
{
    uint16_t length;

    if (size < sizeof(ZDCP_REPORT))
        return ZET76_OBJECT_NOT_FOUND;

    if ((ntohl(report->header.signature) != ZDCP_SIGNATURE) ||
        (ntohs(report->header.id)        != ZDCP_ID_REPORT))
    {
        return ZET76_OBJECT_NOT_FOUND;
    }

    length = ntohs(report->header.length);
    if ((length <  ZDCP_LENGTH_REPORT) ||
        (length < size - sizeof(ZDCP_HEADER)))
    {
        return ZET76_OBJECT_NOT_FOUND;
    }

    if (ntohs(report->port) > ZET76_MAX_PORT)
        return ZET76_OBJECT_NOT_FOUND;

    if (!zet76_device_type_supported(ntohl(report->type)))
        return ZET76_OBJECT_NOT_FOUND;

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
    if (status != NULL) {
        *status = ntohs(report->status);
    }
    if (peer_addr != NULL) {
        if (length >= ZDCP_LENGTH_REPORT_EX) {
            const ZDCP_REPORT_EX *ex = (const ZDCP_REPORT_EX *) report;
            *peer_addr = (const struct in_addr *) &ex->peer_addr;
        } else {
            *peer_addr = NULL;
        }
    }
    if (label != NULL && label_len != NULL) {
        if (length >= ZDCP_LENGTH_REPORT_EX) {
            *label = (const char *) report;
            *label += sizeof(ZDCP_REPORT_EX);
            *label_len = length - ZDCP_LENGTH_REPORT_EX;
        } else {
            *label = NULL;
            *label_len = -1;
        }
    }

    return ZET76_OK;
}

/****************************************************************************/
#ifdef WIN32
static DWORD WINAPI zet76_server_thread_win32(LPVOID arg)
{
    struct zet76_server *server = (struct zet76_server *) arg;

    zet76_server_thread(server);
    return 0;
}
#endif /* WIN32 */

/****************************************************************************/
static void zet76_server_thread(struct zet76_server *server)
{
    event_base_dispatch(server->base);
}

/****************************************************************************/
static void zet76_server_monitor_network(struct zet76_server *server)
{
#ifdef WIN32
    BOOL changed = FALSE;
    DWORD bytes = 0;

    if (server->network_change_handle == NULL)
        return;

    /* Monitor changes in Windows network settings */
    changed = GetOverlappedResult(server->network_change_handle,
                                  &server->network_change,
                                  &bytes,
                                  FALSE);
    if (changed)
        event_active(server->network_event, EV_TIMEOUT, 0);

#else
    /* TODO IMPLEMENT!! */
#error zet76_server_monitor_network is not implemented
#endif
}

/****************************************************************************/
static void zet76_server_clean_nodes(struct zet76_server *server)
{
    struct zet76_node *node = NULL;
    struct zet76_node *node_next = NULL;

    ZET76_GUARD_LOCK(server->guard);
    for (node  = ZET76_LIST_FIRST(&server->nodes);
         node != NULL;
         node  = node_next)
    {
        node_next = ZET76_LIST_NEXT(node, entry);
        if ((node->state == ZET76_DEVICE_IDLE) &&
            (node->flags == 0) &&
            (node->modes == 0))
        {
            zet76_node_free(node);
        }
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_server_on_pulse(evutil_socket_t sock,
                                  short which, void *arg)
{
    struct zet76_server *server = (struct zet76_server *) arg;

    ZET76_UNUSED_PARAM(sock);

    ZET76_GUARD_LOCK(server->guard);
    zet76_server_monitor_network(server);
    zet76_server_clean_nodes(server);
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_server_on_network(evutil_socket_t sock,
                                    short which, void *arg)
{
    struct zet76_server *server = (struct zet76_server *) arg;

#ifdef WIN32
    IP_ADAPTER_ADDRESSES *aa_buffer = NULL;
    IP_ADAPTER_ADDRESSES *aa = NULL;
    IP_ADAPTER_UNICAST_ADDRESS *unicast = NULL;
    ULONG aa_buffer_size = 16384;
    ULONG aa_error = 0;
    ULONG aa_flags = 0;

    struct sockaddr_in *addr = NULL;
    struct zet76_network *current_network = NULL;
    void *networks_buffer = NULL;
    INT addr_size = sizeof(struct sockaddr_in);

    struct zet76_scanner *scanner = NULL;
    struct zet76_snmp *snmp = NULL;
    struct zet76_listener *listener = NULL;

    ZET76_UNUSED_PARAM(sock);

    ZET76_TRACE("");
    if (server->networks != NULL) {
        free(server->networks);
        server->networks = NULL;
        server->networks_count = 0;
    }

    /* Rescan network interfaces to get host IP addresses */

get_adapter_addresses: /* goto here on ERROR_BUFFER_OVERFLOW error */
    if (aa_buffer != NULL)
        free(aa_buffer);
    aa_buffer = (IP_ADAPTER_ADDRESSES *) malloc(aa_buffer_size);
    if (aa_buffer == NULL) {
        aa_error = ERROR_NOT_ENOUGH_MEMORY;
        return;
    }

    aa_flags = GAA_FLAG_SKIP_ANYCAST |
               GAA_FLAG_SKIP_MULTICAST |
               GAA_FLAG_SKIP_DNS_SERVER |
               GAA_FLAG_SKIP_FRIENDLY_NAME;

    aa_error = GetAdaptersAddresses(AF_INET,
                                    aa_flags,
                                    NULL,
                                    aa_buffer,
                                    &aa_buffer_size);
    if (aa_error == ERROR_BUFFER_OVERFLOW && aa_buffer_size > 16384)
        goto get_adapter_addresses;

    if (aa_error != 0) {
        /* TODO : signal error */
        free(aa_buffer);
        return;
    }

count_and_fill_networks: /* goto here to fill networks after counting */
    for (aa = aa_buffer; aa != NULL; aa = aa->Next) {
        if (aa->OperStatus != IfOperStatusUp)
            continue;
        if (aa->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
            continue;

        for (unicast  = aa->FirstUnicastAddress;
             unicast != NULL;
             unicast  = unicast->Next)
        {
            if (unicast->Address.iSockaddrLength < addr_size)
                continue;

            if (current_network == NULL) {
                /* We are on the first pass (counting) */
                server->networks_count++;
                continue;
            }

            /* We are on the second pass (filling in) */
            addr = (struct sockaddr_in *) unicast->Address.lpSockaddr;
            current_network->addr  = addr->sin_addr;
            current_network->flags = 0;

            if (unicast == aa->FirstUnicastAddress)
                current_network->flags |= ZET76_NETWORK_FLAG_FIRST;

            ZET76_TRACE("add network <%s> [%ws]",
                        inet_ntoa(current_network->addr),
                        (current_network->flags & ZET76_NETWORK_FLAG_FIRST) ?
                                aa->Description : L"...");

            current_network++;
        }
    }

    if (server->networks == NULL && server->networks_count > 0) {
        /* First pass succeeded - now alloc array and fill it */
        networks_buffer = calloc(server->networks_count,
                                 sizeof(struct zet76_network));
        server->networks = (struct zet76_network *) networks_buffer;
        if (server->networks != NULL) {
            current_network = &server->networks[0];
            goto count_and_fill_networks;
        }
    }

    free(aa_buffer);
#endif

    for (scanner  = ZET76_LIST_FIRST(&server->scanners);
         scanner != NULL;
         scanner  = ZET76_LIST_NEXT(scanner, entry))
    {
        zet76_scanner_restart(scanner);
    }

    for (snmp  = ZET76_LIST_FIRST(&server->snmps);
         snmp != NULL;
         snmp  = ZET76_LIST_NEXT(snmp, entry))
    {
        zet76_snmp_restart(snmp);
    }

    for (listener  = ZET76_LIST_FIRST(&server->listeners);
         listener != NULL;
         listener  = ZET76_LIST_NEXT(listener, entry))
    {
        zet76_listener_restart(listener);
    }

#ifdef WIN32
    NotifyAddrChange(&server->network_change_handle,
                     &server->network_change);
#endif
}

/****************************************************************************/
ZET76_EXPORT zet76_server_create(struct zet76_server **server_ptr)
{
    struct zet76_server *server = NULL;
    int r = 0;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_PTR(server_ptr);

    server = ZET76_MALLOC(struct zet76_server);
    ZET76_EXIT_IF_NULL(server, ZET76_NOT_ENOUGH_MEMORY);

    ZET76_GUARD_INIT(server->guard);
    server->magic                 = ZET76_MAGIC_SERVER;
    server->status_callback       = NULL;
    server->status_callback_arg   = NULL;
    server->recv_callback         = NULL;
    server->recv_callback_arg     = NULL;
    server->timer_callback        = NULL;
    server->timer_callback_arg    = NULL;
    server->base                  = NULL;
    server->pulse_event           = NULL;
    server->network_event         = NULL;
    server->network_change_handle = NULL;
    server->network_change.hEvent = NULL;
    server->networks              = NULL;
    server->networks_count        = 0;
    ZET76_LIST_INIT(&server->timers);
    ZET76_LIST_INIT(&server->scanners);
    ZET76_LIST_INIT(&server->snmps);
    ZET76_LIST_INIT(&server->listeners);
    ZET76_LIST_INIT(&server->nodes);

#ifdef WIN32
    server->thread_handle         = NULL;
    server->thread_id             = 0;
    server->winsock.wVersion      = 0;

    if (WSAStartup(MAKEWORD(2, 2), &server->winsock) != 0) {
        server->winsock.wVersion = 0;
        ZET76_EXIT(ZET76_NOT_SUPPORTED);
    }
    ZET76_TRACE("WSAStartup() %s %s ver %u.%u",
                server->winsock.szDescription,
                server->winsock.szSystemStatus,
                HIBYTE(server->winsock.wVersion),
                LOBYTE(server->winsock.wVersion));
#endif

    server->base = event_base_new();
    ZET76_EXIT_IF_NULL(server->base, ZET76_NOT_ENOUGH_MEMORY);

    server->pulse_event = event_new(server->base,
                                    -1,
                                    EV_PERSIST | EV_TIMEOUT,
                                    zet76_server_on_pulse,
                                    server);
    ZET76_EXIT_IF_NULL(server->pulse_event, ZET76_NOT_ENOUGH_MEMORY);
    zet76_event_add(server->pulse_event, ZET76_TIMEOUT_PULSE);

    server->network_event = event_new(server->base,
                                      -1,
                                      EV_PERSIST | EV_TIMEOUT,
                                      zet76_server_on_network,
                                      server);
    ZET76_EXIT_IF_NULL(server->network_event, ZET76_NOT_ENOUGH_MEMORY);
    event_add(server->network_event, NULL);
    event_active(server->network_event, EV_TIMEOUT, 0);

#ifdef WIN32
    server->network_change.hEvent = CreateEvent(NULL,
                                                FALSE,
                                                FALSE,
                                                NULL);
#endif

    server->thread_handle = CreateThread(NULL,
                                         0,
                                         zet76_server_thread_win32,
                                         server,
                                         0,
                                         &server->thread_id);
    ZET76_EXIT_IF_NULL(server->thread_handle, ZET76_NOT_ENOUGH_MEMORY);

    *server_ptr = server;

cleanup:
    if (error != ZET76_OK) {
        zet76_server_free(&server);
        *server_ptr = NULL;
    }

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_server_free(struct zet76_server **server_ptr)
{
    struct zet76_server *server = NULL;

    ZET76_VALIDATE_PARAM_PTR(server_ptr);

    server = *server_ptr;
    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);

    ZET76_GUARD_LOCK(server->guard);

    if (server->thread_handle != NULL) {
        event_base_loopbreak(server->base);

        WaitForSingleObject(server->thread_handle, INFINITE);
        ZET76_FREE(server->thread_handle, CloseHandle);
    }

    ZET76_LIST_CLEAR(&server->nodes,     zet76_node_free);
    ZET76_LIST_CLEAR(&server->listeners, zet76_listener_free);
    ZET76_LIST_CLEAR(&server->snmps,     zet76_snmp_free);
    ZET76_LIST_CLEAR(&server->scanners,  zet76_scanner_free);
    ZET76_LIST_CLEAR(&server->timers,    zet76_timer_free);

    ZET76_FREE(server->networks, free);

    ZET76_FREE(server->network_change.hEvent, CloseHandle);
    ZET76_FREE(server->network_event, event_free);
    ZET76_FREE(server->pulse_event, event_free);

    if (server->base != NULL) {
        event_base_free(server->base);
        server->base = NULL;
    }

#ifdef WIN32
    if (server->winsock.wVersion != 0) {
        ZET76_TRACE("WSACleanup()");
        WSACleanup();
    }
#endif

    server->magic = ZET76_MAGIC_NULL;

    ZET76_GUARD_UNLOCK(server->guard);
    ZET76_GUARD_FREE(server->guard);
    free(server);

    *server_ptr = NULL;

    return ZET76_OK;
}

/****************************************************************************/
ZET76_EXPORT zet76_notify_status(struct zet76_server *server,
                                 zet76_status_callback *status_callback,
                                 void *status_callback_arg)
{
    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);

    ZET76_GUARD_LOCK(server->guard);
    server->status_callback     = status_callback;
    server->status_callback_arg = status_callback_arg;
    ZET76_GUARD_UNLOCK(server->guard);

    return ZET76_OK;
}

/****************************************************************************/
ZET76_EXPORT zet76_notify_recv(struct zet76_server *server,
                               zet76_recv_callback *recv_callback,
                               void *recv_callback_arg)
{
    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);

    ZET76_GUARD_LOCK(server->guard);
    server->recv_callback     = recv_callback;
    server->recv_callback_arg = recv_callback_arg;
    ZET76_GUARD_UNLOCK(server->guard);

    return ZET76_OK;
}

/****************************************************************************/
ZET76_EXPORT zet76_notify_timer(struct zet76_server *server,
                                zet76_timer_callback *timer_callback,
                                void *timer_callback_arg)
{
    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);

    ZET76_GUARD_LOCK(server->guard);
    server->timer_callback     = timer_callback;
    server->timer_callback_arg = timer_callback_arg;
    ZET76_GUARD_UNLOCK(server->guard);

    return ZET76_OK;
}

/****************************************************************************/
static void zet76_timer_free(struct zet76_timer *timer)
{
    struct zet76_server *server = timer->server;

    ZET76_GUARD_LOCK(server->guard);
    ZET76_FREE(timer->event, event_free);
    ZET76_LIST_REMOVE(timer, entry);
    ZET76_GUARD_UNLOCK(server->guard);

    free(timer);
}

/****************************************************************************/
static void zet76_timer_on_tick(evutil_socket_t sock,
                                short which, void *arg)
{
    struct zet76_timer *timer = (struct zet76_timer *) arg;
    struct zet76_server *server = timer->server;

    if ((which & EV_TIMEOUT) == 0)
        return;

    ZET76_GUARD_LOCK(server->guard);
    if (server->timer_callback != NULL) {
        server->timer_callback(server,
                               timer->id,
                               server->timer_callback_arg);
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
ZET76_EXPORT zet76_timer_start(struct zet76_server *server,
                               int timer_id,
                               int period_ms)
{
    struct zet76_timer *timer = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM(period_ms > 0);

    ZET76_GUARD_LOCK(server->guard);
    for (timer  = ZET76_LIST_FIRST(&server->timers);
         timer != NULL;
         timer  = ZET76_LIST_NEXT(timer, entry))
    {
        if (timer->id == timer_id) {
            ZET76_TRACE("timer %d already started", timer_id);
            ZET76_EXIT(ZET76_OBJECT_NOT_FOUND);
        }
    }

    timer = ZET76_MALLOC(struct zet76_timer);
    ZET76_EXIT_IF_NULL(timer, ZET76_NOT_ENOUGH_MEMORY);

    timer->id        = timer_id;
    timer->period_ms = period_ms;
    timer->server    = server;
    timer->event = event_new(timer->server->base,
                             -1,
                             EV_PERSIST | EV_TIMEOUT,
                             zet76_timer_on_tick,
                             timer);
    ZET76_EXIT_IF_NULL(timer->event, ZET76_NOT_ENOUGH_MEMORY);

    ZET76_LIST_INSERT(&server->timers, timer, entry);
    zet76_event_add(timer->event, timer->period_ms);

cleanup:
    if (error != ZET76_OK) {
        if (timer != NULL) {
            ZET76_FREE(timer->event, event_free);
            free(timer);
        }
    }
    ZET76_GUARD_UNLOCK(server->guard);

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_timer_stop(struct zet76_server *server,
                              int timer_id)
{
    struct zet76_timer *timer = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);

    ZET76_GUARD_LOCK(server->guard);
    for (timer  = ZET76_LIST_FIRST(&server->timers);
         timer != NULL;
         timer  = ZET76_LIST_NEXT(timer, entry))
    {
        if (timer->id == timer_id) {
            zet76_timer_free(timer);
            ZET76_EXIT(ZET76_OK);
        }
    }

    ZET76_EXIT(ZET76_OBJECT_NOT_FOUND);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    return error;
}

/****************************************************************************/
static void zet76_scanner_free(struct zet76_scanner *scanner)
{
    struct zet76_server *server = scanner->server;

    ZET76_GUARD_LOCK(server->guard);
    ZET76_LIST_REMOVE(scanner, entry);
    ZET76_GUARD_UNLOCK(server->guard);

    ZET76_LIST_CLEAR(&scanner->hosts, zet76_scanner_host_free);
    ZET76_FREE(scanner->refresh_event, event_free);
    zet76_snmp_stop(server, 0);
    free(scanner);
}

/****************************************************************************/
static void zet76_scanner_restart(struct zet76_scanner *scanner)
{
    struct zet76_server *server = NULL;
    struct zet76_scanner_host *host = NULL;
    int index = 0;

    ZET76_LIST_CLEAR(&scanner->hosts, zet76_scanner_host_free);

    /* Open scanner_host for each network interface */

    server = scanner->server;
    ZET76_GUARD_LOCK(server->guard);
    for (index = 0; index < server->networks_count; index++) {
        /*
           Network interface can have several addresses, but for multicast
           it is enough to bind to one address per interface.
           Skip address if it is not first address of the interface.
        */
        if (!(server->networks[index].flags & ZET76_NETWORK_FLAG_FIRST))
            continue;

        zet76_scanner_host_create(scanner, &server->networks[index].addr);
    }
    zet76_event_add(scanner->refresh_event, ZET76_TIMEOUT_SCANNER_REFRESH);
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_scanner_on_refresh(evutil_socket_t sock,
                                     short which, void *arg)
{
    struct zet76_scanner *scanner = (struct zet76_scanner *) arg;

    zet76_scanner_restart(scanner);
}

/****************************************************************************/
static void zet76_scanner_host_create(struct zet76_scanner *scanner,
                                      const struct in_addr *addr)
{
    struct sockaddr_in host_addr;
    struct ip_mreq multicast;
    ZDCP_PROBE probe;

    struct zet76_scanner_host *host = NULL;
    evutil_socket_t sock = INVALID_SOCKET;
    int r = 0;
    int reusable = 1;
    int error = ZET76_OK;

    /* First try to open multicast UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ZET76_EXIT_IF_FALSE(sock != INVALID_SOCKET, ZET76_NOT_ENOUGH_MEMORY);

    r = evutil_make_socket_nonblocking(sock);
    ZET76_EXIT_IF_ERROR(r, ZET76_NOT_ENOUGH_MEMORY);

    r = setsockopt(sock,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   (const char *) &reusable,
                   sizeof(reusable));
    ZET76_EXIT_IF_ERROR(r, ZET76_NOT_ENOUGH_MEMORY);

    multicast.imr_interface = *addr;
    multicast.imr_multiaddr = scanner->scan_addr.sin_addr;
    r = setsockopt(sock,
                   IPPROTO_IP,
                   IP_ADD_MEMBERSHIP,
                   (char*) &multicast,
                   sizeof(multicast));
    ZET76_EXIT_IF_ERROR(r, ZET76_NOT_ENOUGH_MEMORY);

    host_addr.sin_family = AF_INET;
    host_addr.sin_addr   = *addr;
    host_addr.sin_port   = scanner->scan_addr.sin_port;
    r = bind(sock, (struct sockaddr *) &host_addr, sizeof(host_addr));
    ZET76_EXIT_IF_ERROR(r, ZET76_NOT_ENOUGH_MEMORY);

    host = ZET76_MALLOC(struct zet76_scanner_host);
    ZET76_EXIT_IF_NULL(host, ZET76_NOT_ENOUGH_MEMORY);

    host->sock = sock;

    host->report_event = event_new(scanner->server->base,
                                   sock,
                                   EV_PERSIST | EV_READ,
                                   zet76_scanner_on_report,
                                   scanner);
    ZET76_EXIT_IF_NULL(host->report_event, ZET76_NOT_ENOUGH_MEMORY);

    probe.header.signature = htonl(ZDCP_SIGNATURE);
    probe.header.id        = htons(ZDCP_ID_PROBE);
    probe.header.length    = htons(ZDCP_LENGTH_PROBE);
    r = sendto(sock,
               (const char *) &probe,
               sizeof(ZDCP_PROBE),
               0,
               (const struct sockaddr *) &scanner->scan_addr,
               sizeof(scanner->scan_addr));

    zet76_event_add(host->report_event, ZET76_TIMEOUT_INFINITE);
#if 0
    ZET76_TRACE("   open scanner socket %d for <%s:%u>",
                sock,
                inet_ntoa(host_addr.sin_addr),
                ntohs(host_addr.sin_port),
                sock);
#endif
    ZET76_LIST_INSERT(&scanner->hosts, host, entry);

cleanup:
    if (error != ZET76_OK) {
        if (host != NULL) {
            ZET76_FREE(host->report_event, event_free);
            free(host);
        }
        if (sock != INVALID_SOCKET)
            evutil_closesocket(sock);
    }
}

/****************************************************************************/
static void zet76_scanner_host_free(struct zet76_scanner_host *host)
{
#if 0
    ZET76_TRACE("  closing host %d", host->sock);
#endif
    ZET76_LIST_REMOVE(host, entry);
    ZET76_FREE(host->report_event, event_free);
    evutil_closesocket(host->sock);
    free(host);
}

/****************************************************************************/
static void zet76_scanner_on_report(evutil_socket_t sock,
                                    short which, void *arg)
{
    char buffer[ZET76_SIZE_SCANNER_HOST];
    struct sockaddr_in addr;
    int addr_size;
    struct zet76_device device;
    unsigned short status;
    const struct in_addr *peer_addr;
    const char *label;
    int label_len;

    struct zet76_scanner *scanner = (struct zet76_scanner *) arg;
    int r = 0;

    if (which & EV_READ) {
        addr_size = sizeof(addr);
        r = recvfrom(sock,
                     buffer,
                     ZET76_SIZE_SCANNER_HOST,
                     0,
                     (struct sockaddr *) &addr,
                     &addr_size);
        r = zet76_zdcp_report_parse((const ZDCP_REPORT *) buffer,
                                    r,
                                    &device,
                                    &addr,
                                    &status,
                                    &peer_addr,
                                    &label,
                                    &label_len);
        if (r == ZET76_OK) {
            ZET76_TRACE("device <%s #%016llx> <%s:%u> %s",
                        zet76_device_type_str(device.type),
                        device.serial,
                        inet_ntoa(addr.sin_addr),
                        ntohs(addr.sin_port),
                        status == ZDCP_REPORT_BUSY     ? "BUSY" :
                        status == ZDCP_REPORT_PEER     ? "PEER" :
                        status == ZDCP_REPORT_READY    ? "READY" :
                        status == ZDCP_REPORT_CONFLICT ? "CONFLICT" :
                        "unknown!!!!");

//            if (status != ZDCP_REPORT_BUSY &&
//                ntohs(addr.sin_port) <= ZET76_MAX_PORT)
            zet76_device_on_report(scanner->server,
                                   &device,
                                   &addr,
                                   status,
                                   0,
                                   ZET76_PARITY_UNKNOWN,
                                   peer_addr,
                                   label,
                                   label_len);
        }
    }
}

/****************************************************************************/
ZET76_EXPORT zet76_scanner_control(struct zet76_server *server,
                                   const struct zet76_device* device,
                                   int control_command)
{
    ZDCP_COMMON_PACKET zdcp;
    struct zet76_scanner* scanner;
    struct zet76_scanner_host* host;
    unsigned long zdcp_cmd = ZDCP_COMMAND_PING;
    int count = 0;

    switch (control_command) {
    case ZET76_COMMAND_RESTART:
        zdcp_cmd = ZDCP_COMMAND_RESTART;
        break;
    case ZET76_COMMAND_RESET:
        zdcp_cmd = ZDCP_COMMAND_RESET;
        break;
    default:
        return ZET76_NOT_SUPPORTED;
    }

    zdcp.header.signature  = htonl(ZDCP_SIGNATURE);
    zdcp.header.id         = htons(ZDCP_ID_COMMAND);
    zdcp.header.length     = htons(ZDCP_LENGTH_COMMAND);

    if (device != NULL) {
        zdcp.command.serial_hi = htonl(ZET76_HI_U32(device->serial));
        zdcp.command.serial_lo = htonl(ZET76_LO_U32(device->serial));
        zdcp.command.type      = htonl(device->type);
    } else {
        zdcp.command.serial_hi = htonl(ZDCP_TARGET_BROADCAST);
        zdcp.command.serial_lo = htonl(ZDCP_TARGET_BROADCAST);
        zdcp.command.type      = htonl(ZDCP_TARGET_BROADCAST);
    }

    zdcp.command.command   = htonl(zdcp_cmd);
    zdcp.command.param     = 0;

    ZET76_GUARD_LOCK(server->guard);
    for (scanner  = ZET76_LIST_FIRST(&server->scanners);
         scanner != NULL;
         scanner  = ZET76_LIST_NEXT(scanner, entry))
    {
        for (host  = ZET76_LIST_FIRST(&scanner->hosts);
             host != NULL;
             host  = ZET76_LIST_NEXT(host, entry))
        {
            sendto(host->sock,
                   (const char *) &zdcp,
                   sizeof(ZDCP_COMMAND),
                   0,
                   (const struct sockaddr *) &scanner->scan_addr,
                   sizeof(scanner->scan_addr));
            count++;
        }
    }
    ZET76_GUARD_UNLOCK(server->guard);

    return (count > 0) ? ZET76_OK : ZET76_OBJECT_NOT_FOUND;
}

/****************************************************************************/
ZET76_EXPORT zet76_scanner_start(struct zet76_server *server,
                                 const char *scan_ip,
                                 unsigned short scan_port)
{
    struct zet76_scanner *scanner = NULL;
    struct sockaddr_in addr;
    int r;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM(scan_port <= ZET76_MAX_PORT);

    if (scan_ip == NULL)
        scan_ip  = ZET76_SCAN_IP;
    if (scan_port == 0)
        scan_port  = ZET76_SCAN_PORT;

    r = evutil_inet_pton(AF_INET, scan_ip, &addr.sin_addr);
    ZET76_VALIDATE_PARAM(r != 0);

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(scan_port);

    ZET76_TRACE("<%s:%u>",
                inet_ntoa(addr.sin_addr),
                ntohs(addr.sin_port));

    ZET76_GUARD_LOCK(server->guard);
    for (scanner  = ZET76_LIST_FIRST(&server->scanners);
         scanner != NULL;
         scanner  = ZET76_LIST_NEXT(scanner, entry))
    {
        if (zet76_sockaddr_compare(&scanner->scan_addr, &addr) == 0) {
            ZET76_TRACE("duplicate scanner found");
            ZET76_REFCOUNT_INC(scanner->ref_count);
            ZET76_EXIT(ZET76_OK);
        }
    }

    scanner = ZET76_MALLOC(struct zet76_scanner);
    ZET76_EXIT_IF_NULL(scanner, ZET76_NOT_ENOUGH_MEMORY);

    scanner->scan_addr     = addr;
    scanner->server        = server;
    scanner->refresh_event = event_new(server->base,
                                       -1,
                                       EV_TIMEOUT,
                                       zet76_scanner_on_refresh,
                                       scanner);
    ZET76_EXIT_IF_NULL(scanner->refresh_event, ZET76_NOT_ENOUGH_MEMORY);

    ZET76_REFCOUNT_INIT(scanner->ref_count, 1);
    ZET76_LIST_INIT(&scanner->hosts);

    ZET76_LIST_INSERT(&server->scanners, scanner, entry);

    zet76_scanner_restart(scanner);

    zet76_snmp_start(server, 0);

cleanup:
    if (error != ZET76_OK) {
        if (scanner)
            free(scanner);
    }
    ZET76_GUARD_UNLOCK(server->guard);

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_scanner_stop(struct zet76_server *server,
                                const char *scan_ip,
                                unsigned short scan_port)
{
    struct zet76_scanner *scanner = NULL;
    struct sockaddr_in addr;
    int r;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM(scan_port <= ZET76_MAX_PORT);

    if (scan_ip == NULL)
        scan_ip  = ZET76_SCAN_IP;
    if (scan_port == 0)
        scan_port  = ZET76_SCAN_PORT;

    r = evutil_inet_pton(AF_INET, scan_ip, &addr.sin_addr);
    ZET76_VALIDATE_PARAM(r != 0);

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(scan_port);

    ZET76_TRACE("<%s:%u>",
                inet_ntoa(addr.sin_addr),
                ntohs(addr.sin_port));

    ZET76_GUARD_LOCK(server->guard);
    for (scanner  = ZET76_LIST_FIRST(&server->scanners);
         scanner != NULL;
         scanner  = ZET76_LIST_NEXT(scanner, entry))
    {
        if (zet76_sockaddr_compare(&scanner->scan_addr, &addr) == 0) {
            if (ZET76_REFCOUNT_DEC(scanner->ref_count) == 0) {
                zet76_scanner_free(scanner);
            } else {
                ZET76_TRACE("scanner shared");
            }
            ZET76_EXIT(ZET76_OK);
        }
    }

    ZET76_EXIT(ZET76_OBJECT_NOT_FOUND);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    return error;
}

/****************************************************************************/
static int zet76_snmp_compare_oid(const struct zet76_snmp_oid *oid1,
                                  const struct zet76_snmp_oid *oid2)
{
    int i;

    if (oid1->len < oid2->len)
        return -1;

    for (i = 0; i < oid2->len; i++) {
        if (oid1->integers[i] != oid2->integers[i])
            return -1;
    }

    if (oid1->len > oid2->len)
        return oid1->len - oid2->len;

    return 0;
}

/****************************************************************************/
static void zet76_snmp_encode_oid(struct zet76_snmp_oid *oid,
                                  const struct zet76_snmp_oid *parent,
                                  ...)
{
    va_list ap;
    int integer;

    if (parent) {
        if (parent != oid) {
            for (oid->len = 0; oid->len < parent->len; oid->len++) {
                oid->integers[oid->len] = parent->integers[oid->len];
            }
        }
    } else {
        oid->len = 0;
    }

    va_start(ap, parent);
    while (oid->len < ZET76_MAX_SNMP_OID_LEN) {
        integer = va_arg(ap, int);
        if (integer < 0)
            break;

        oid->integers[oid->len] = integer;
        oid->len++;
    }
    va_end(ap);
}

/****************************************************************************/
static int zet76_snmp_encode_request(void *data,
                                     int size,
                                     unsigned char request_type,
                                     int request_id,
                                     const struct zet76_snmp_oid *name)
{
    int pos;
    int len;
    int integer;
    int i;

    unsigned char *buf = (unsigned char *) data;

    /* Set-Request is not supported because it's not needed */
    if (request_type != ZET76_SNMP_TYPE_PDU_GET &&
        request_type != ZET76_SNMP_TYPE_PDU_GET_NEXT)
    {
        return 0;
    }

    /* Encode message */

    pos = 0;
    ZET76_SNMP_ENCODE(buf, pos, ZET76_SNMP_TYPE_SEQUENCE, {
        /* Version-1 (0) */
        buf[pos++] = ZET76_SNMP_TYPE_INTEGER;
        buf[pos++] = 1;
        buf[pos++] = 0;

        /* Community */
        ZET76_SNMP_ENCODE(buf, pos, ZET76_SNMP_TYPE_STRING, {
            for (i = 0; ZET76_SNMP_COMMUNITY[i]; i++) {
                buf[pos++] = ZET76_SNMP_COMMUNITY[i];
            }
        });

        /* PDU Get-Next-Request */
        ZET76_SNMP_ENCODE(buf, pos, request_type, {

            /* Request ID */
            ZET76_SNMP_ENCODE(buf, pos, ZET76_SNMP_TYPE_INTEGER, {
                len = 1;
                while (request_id >> (len * 8)) {
                    len++;
                }
                len--;
                if (request_id >= 0) {
                    /* Non-negative number - the highest bit should be 0 */
                    if ((request_id >> (len * 8)) & 0x80) {
                        buf[pos++] = 0x00;
                    }
                } else {
                    /* Negative number - drop 0xFF's */
                    while (len > 0 &&
                        (((request_id >> (len * 8)) & 0xFF) == 0xFF) &&
                        ((request_id >> ((len - 1) * 8)) & 0x80))
                    {
                        len--;
                    }
                    /* Add 0xFF if highest bit is 0 */
                    if (!((request_id >> (len * 8)) & 0x80)) {
                        buf[pos++] = 0x00;
                    }
                }
                do {
                    buf[pos++] = (request_id >> (len * 8)) & 0xFF;
                } while (len-- > 0);
            });

            /* Error Status (0) */
            buf[pos++] = ZET76_SNMP_TYPE_INTEGER;
            buf[pos++] = 1;
            buf[pos++] = 0;

            /* Error Index (0) */
            buf[pos++] = ZET76_SNMP_TYPE_INTEGER;
            buf[pos++] = 1;
            buf[pos++] = 0;

            /* Object List */
            ZET76_SNMP_ENCODE(buf, pos, ZET76_SNMP_TYPE_SEQUENCE, {

                /* Object */
                ZET76_SNMP_ENCODE(buf, pos, ZET76_SNMP_TYPE_SEQUENCE, {

                    /* Object Name (OID) */
                    ZET76_SNMP_ENCODE(buf, pos, ZET76_SNMP_TYPE_OID, {
                        buf[pos++] = name->integers[0] * 40 +
                                     name->integers[1];
                        for (i = 2; i < name->len; i++) {
                            integer = name->integers[i];
                            len = 1;
                            while (integer >> (len * 7)) {
                                len++;
                            }
                            while (len > 1) {
                                len--;
                                buf[pos]  = (integer >> (len * 7)) & 0x7F;
                                buf[pos] |= 0x80;
                                pos++;
                            }
                            buf[pos++] = integer & 0x7F;
                        }
                    });

                    /* Object Value (NULL) */
                    buf[pos++] = ZET76_SNMP_TYPE_NULL;
                    buf[pos++] = 0;
                });
            });
        });
    });

    return pos;
}

/****************************************************************************/
/* Returns decoded size */
static int zet76_snmp_decode_value(const void *data,
                                   int size,
                                   struct zet76_snmp_value *value)
{
    const unsigned char *buf = (const unsigned char *) data;
    int type;
    int len;
    int pos;
    int i;

    value->type = ZET76_SNMP_TYPE_UNKNOWN;

    if (size < 2)
        return 0;

    pos = 0;
    type = buf[pos++];

    len = 0;
    while (size > pos) {
        len = (len << 7) | (buf[pos] & 0x7F);
        if (!(buf[pos++] & 0x80))
            break;
        /* Too big length */
        if (pos > 4)
            return 0;
    }

    if (size < pos + len)
        return 0;

    switch (type) {
    /* Multiple Values (Sequences) */
    case ZET76_SNMP_TYPE_SEQUENCE:
    case ZET76_SMNP_TYPE_PDU_RESPONSE:
        /* Do not decode contents */
        value->data.sequence_len = len;
        break;

    /* Single values */
    case ZET76_SNMP_TYPE_NULL:
        if (len != 0)
            return 0;
        break;
    case ZET76_SNMP_TYPE_INTEGER:
        if (len < 0 || len > 8)
            return 0;
        if (len == 0) {
        }
        if (buf[pos] & 0x80)
            value->data.integer = -1;
        else
            value->data.integer = 0;

        for (i = 0; i < len; i++) {
            value->data.integer <<= 8;
            value->data.integer  |= buf[pos++];
        }
        break;
    case ZET76_SNMP_TYPE_STRING:
        /* Max 30 octets + zero octet allowed */
        if (len <= 0 || len >= ZET76_MAX_SNMP_STRING_LEN)
            return 0;
        value->data.string.len = len;
        for (i = 0; i < value->data.string.len; i++) {
            value->data.string.octets[i] = buf[pos++];
        }
        /* Terminate with zero char for safety */
        value->data.string.octets[value->data.string.len] = '\0';
        break;
    case ZET76_SNMP_TYPE_OID:
        if (len <= 0)
            return 0;
        value->data.oid.len = 0;
        value->data.oid.integers[value->data.oid.len++] = buf[pos] / 40;
        value->data.oid.integers[value->data.oid.len++] = buf[pos] % 40;
        pos++;

        value->data.oid.integers[value->data.oid.len] = 0;
        for (i = 1; i < len; i++) {
            value->data.oid.integers[value->data.oid.len] <<= 7;
            value->data.oid.integers[value->data.oid.len]  += buf[pos] & 0x7F;
            if ((buf[pos] & 0x80) == 0) {
                value->data.oid.len++;
                if (value->data.oid.len < ZET76_MAX_SNMP_OID_LEN)
                    value->data.oid.integers[value->data.oid.len] = 0;
                else if (i + 1 < len)
                    return 0;
            }
            pos++;
        }
        break;
    case ZET76_SNMP_TYPE_IP:
        if (len != 4)
            return 0;
        value->data.ip.s_addr = 0;
        for (i = 0; i < len; i++) {
            value->data.ip.s_addr <<= 8;
            value->data.ip.s_addr  += buf[pos++];
        }
        break;
    default:
        return 0;
    }

    value->type = type;
    return pos;
}

/****************************************************************************/
static int zet76_snmp_decode_response(const void *data,
                                      int size,
                                      int *request_id,
                                      struct zet76_snmp_object *object)
{
    static const int response_type_table[] = {
        ZET76_SNMP_TYPE_SEQUENCE,
        ZET76_SNMP_TYPE_INTEGER,                /* 1 - version */
        ZET76_SNMP_TYPE_STRING,                 /* 2 - community */
        ZET76_SMNP_TYPE_PDU_RESPONSE,
        ZET76_SNMP_TYPE_INTEGER,                /* 4 - request_id */
        ZET76_SNMP_TYPE_INTEGER,                /* 5 - error_status */
        ZET76_SNMP_TYPE_INTEGER,                /* 6 - error_index */
        ZET76_SNMP_TYPE_SEQUENCE,
        ZET76_SNMP_TYPE_SEQUENCE,
        ZET76_SNMP_TYPE_OID,                    /* 9 - object name */
        /* last value can be of any type */
    };

    struct zet76_snmp_value value;

    const unsigned char *buf = (const unsigned char *) data;

    int stage;
    int total_size;
    int decoded;

    object->name.len = 0;
    object->value.type = ZET76_SNMP_TYPE_UNKNOWN;

    total_size = 0;
    stage = 0;
    while (1) {
        decoded = zet76_snmp_decode_value(buf, size, &value);
        if (decoded < 2 || value.type == ZET76_SNMP_TYPE_UNKNOWN) {
            return 0;
        }
        if (decoded > size) {
            return 0;
        }
        buf        += decoded;
        size       -= decoded;
        total_size += decoded;

        if ((stage < (sizeof(response_type_table) / sizeof(int))) &&
            (value.type != response_type_table[stage]))
        {
            return 0;
        }

        switch (stage) {
        case 1:
            if (value.data.integer != 0)
                return 0;
            break;
        case 2:
            if (strcmp(value.data.string.octets, ZET76_SNMP_COMMUNITY) != 0)
                return 0;
            break;
        case 4:
            /* Validate request id */
            if (value.data.integer < ZET76_MIN_SNMP_REQUEST_ID)
                return 0;
            if (value.data.integer > ZET76_MAX_SNMP_REQUEST_ID)
                return 0;
            *request_id = value.data.integer & 0xFFFF;
            break;
        case 5:
        case 6:
            /* Validate error-status and error-index */
            if (value.data.integer != 0)
                return 0;
            break;
        case 9:
            object->name = value.data.oid;
            break;
        case 10:
            /* Only single values are allowed */
            if (value.type & 0x20)
                return 0;
            object->value = value;
            return total_size;
        }

        stage++;
    }
    return 0;
}

/****************************************************************************/
static void zet76_snmp_start(struct zet76_server *server,
                             unsigned short snmp_port)
{
    struct zet76_snmp_oid oid;
    struct sockaddr_in addr;
    struct zet76_snmp *snmp = NULL;
    int port = 1;
    int error = ZET76_OK;

    ZET76_VALIDATE_OBJECT(server, ZET76_MAGIC_SERVER);
    if (snmp_port > ZET76_MAX_PORT)
        return;

    if (snmp_port == 0)
        snmp_port  = ZET76_SNMP_PORT;

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_BROADCAST;
    addr.sin_port        = htons(snmp_port);

    ZET76_GUARD_LOCK(server->guard);
    for (snmp  = ZET76_LIST_FIRST(&server->snmps);
         snmp != NULL;
         snmp  = ZET76_LIST_NEXT(snmp, entry))
    {
        if (zet76_sockaddr_compare(&snmp->broadcast_addr, &addr) == 0) {
            ZET76_TRACE("duplicate snmp found");
            ZET76_REFCOUNT_INC(snmp->ref_count);
            ZET76_EXIT(ZET76_OK);
        }
    }

    snmp = ZET76_MALLOC(struct zet76_snmp);
    ZET76_EXIT_IF_NULL(snmp, ZET76_NOT_ENOUGH_MEMORY);

    snmp->broadcast_addr = addr;
    snmp->server         = server;

    /* Last parameter should be < 0 */
    zet76_snmp_encode_oid(&oid, NULL, 1, 3, 6, 1, 2, 1, -1);
    zet76_snmp_encode_oid(&snmp->sys_name_oid,     &oid, 1, 5, 0, -1);
    zet76_snmp_encode_oid(&snmp->sys_location_oid, &oid, 1, 6, 0, -1);
    zet76_snmp_encode_oid(&snmp->tcp_state_oid,    &oid, 6, 13, 1, 1, -1);

    /* RS232 port params */
    port = 1;
    zet76_snmp_encode_oid(&oid, &oid, 10, 33, -1);
    zet76_snmp_encode_oid(&snmp->rs232_type_oid,   &oid, 2, 1, 2, port, -1);
    zet76_snmp_encode_oid(&snmp->rs232_speed_oid,  &oid, 2, 1, 5, port, -1);
    zet76_snmp_encode_oid(&snmp->rs232_bits_oid,   &oid, 3, 1, 2, port, -1);
    zet76_snmp_encode_oid(&snmp->rs232_parity_oid, &oid, 3, 1, 4, port, -1);

    ZET76_REFCOUNT_INIT(snmp->ref_count, 1);
    ZET76_LIST_INIT(&snmp->hosts);

    ZET76_LIST_INSERT(&server->snmps, snmp, entry);

    zet76_snmp_restart(snmp);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_snmp_stop(struct zet76_server *server,
                            unsigned short snmp_port)
{
    struct zet76_snmp *snmp = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_OBJECT(server, ZET76_MAGIC_SERVER);
    if (snmp_port > ZET76_MAX_PORT)
        return;

    if (snmp_port == 0)
        snmp_port  = ZET76_SNMP_PORT;

    snmp_port = htons(snmp_port);

    ZET76_GUARD_LOCK(server->guard);
    for (snmp  = ZET76_LIST_FIRST(&server->snmps);
         snmp != NULL;
         snmp  = ZET76_LIST_NEXT(snmp, entry))
    {
        if (snmp->broadcast_addr.sin_port == snmp_port) {
            if (ZET76_REFCOUNT_DEC(snmp->ref_count) == 0) {
                zet76_snmp_free(snmp);
            } else {
                ZET76_TRACE("snmp shared");
            }
            ZET76_EXIT(ZET76_OK);
        }
    }

    ZET76_EXIT(ZET76_OBJECT_NOT_FOUND);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_snmp_free(struct zet76_snmp *snmp)
{
    struct zet76_server *server = snmp->server;

    ZET76_GUARD_LOCK(server->guard);
    ZET76_LIST_REMOVE(snmp, entry);
    ZET76_GUARD_UNLOCK(server->guard);

    ZET76_LIST_CLEAR(&snmp->hosts, zet76_snmp_host_free);
    free(snmp);
}

/****************************************************************************/
static void zet76_snmp_restart(struct zet76_snmp *snmp)
{
    struct zet76_server *server = NULL;
    struct zet76_snmp_host *host = NULL;
    int index = 0;

    /* Open listener_host for each network address */

    server = snmp->server;
    ZET76_GUARD_LOCK(server->guard);
    ZET76_LIST_CLEAR(&snmp->hosts, zet76_snmp_host_free);

    for (index = 0; index < server->networks_count; index++) {
        zet76_snmp_host_create(snmp, &server->networks[index].addr);
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_snmp_host_create(struct zet76_snmp *snmp,
                                   const struct in_addr *addr)
{
    struct sockaddr_in host_addr;

    struct zet76_snmp_host *host = NULL;
    evutil_socket_t sock = INVALID_SOCKET;
    int reusable = 1;
    int broadcast = 1;
    int r = 0;
    int error = ZET76_OK;

    /* First try to open multicast UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ZET76_EXIT_IF_FALSE(sock != INVALID_SOCKET, ZET76_NOT_ENOUGH_MEMORY);

    r = evutil_make_socket_nonblocking(sock);
    ZET76_EXIT_IF_ERROR(r, ZET76_NOT_ENOUGH_MEMORY);

    r = setsockopt(sock,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   (const char *) &reusable,
                   sizeof(reusable));
    ZET76_EXIT_IF_ERROR(r, ZET76_NOT_ENOUGH_MEMORY);

    r = setsockopt(sock,
                   SOL_SOCKET,
                   SO_BROADCAST,
                   (const char *) &broadcast,
                   sizeof(broadcast));
    ZET76_EXIT_IF_ERROR(r, ZET76_NOT_ENOUGH_MEMORY);

    host_addr.sin_family = AF_INET;
    host_addr.sin_addr   = *addr;
    host_addr.sin_port   = 0;
    r = bind(sock, (struct sockaddr *) &host_addr, sizeof(host_addr));
    ZET76_EXIT_IF_ERROR(r, ZET76_NOT_ENOUGH_MEMORY);

    host = ZET76_MALLOC(struct zet76_snmp_host);
    ZET76_EXIT_IF_NULL(host, ZET76_NOT_ENOUGH_MEMORY);

    host->addr = host_addr;
    host->snmp = snmp;
    host->sock = sock;

    host->broadcast_event = event_new(snmp->server->base,
                                      sock,
                                      EV_PERSIST | EV_READ | EV_TIMEOUT,
                                      zet76_snmp_on_response,
                                      host);
    ZET76_EXIT_IF_NULL(host->broadcast_event, ZET76_NOT_ENOUGH_MEMORY);

    zet76_event_add(host->broadcast_event, ZET76_TIMEOUT_SNMP_BROADCAST);
    event_active(host->broadcast_event, EV_TIMEOUT, 0);

    ZET76_LIST_INIT(&host->peers);
    host->broadcast_request_id = 0;

    ZET76_LIST_INSERT(&snmp->hosts, host, entry);

    ZET76_TRACE("   open snmp socket %d for <%s:%u>",
                sock,
                inet_ntoa(host_addr.sin_addr),
                ntohs(host_addr.sin_port));

cleanup:
    if (error != ZET76_OK) {
        if (host != NULL) {
            ZET76_FREE(host->broadcast_event, event_free);
            free(host);
        }
        if (sock != INVALID_SOCKET)
            evutil_closesocket(sock);
    }
}

/****************************************************************************/
static void zet76_snmp_host_free(struct zet76_snmp_host *host)
{
    ZET76_TRACE("  closing host %d", host->sock);

    ZET76_LIST_REMOVE(host, entry);

    ZET76_LIST_CLEAR(&host->peers, zet76_snmp_peer_free);
    ZET76_FREE(host->broadcast_event, event_free);
    evutil_closesocket(host->sock);
    free(host);
}

/****************************************************************************/
static struct zet76_snmp_peer *zet76_snmp_peer_create(
                                        struct zet76_snmp_host *host,
                                        const struct zet76_device *device,
                                        const IN_ADDR in_addr)
{
    struct zet76_snmp_peer *peer = NULL;
    int error = ZET76_OK;

    ZET76_TRACE(" create snmp peer <%s:0x%I64x>",
                zet76_device_type_str(device->type),
                device->serial);

    /* Try to find peer with specified in_addr */
    for (peer  = ZET76_LIST_FIRST(&host->peers);
         peer != NULL;
         peer  = ZET76_LIST_NEXT(peer, entry))
    {
        if (peer->addr.sin_addr.s_addr == in_addr.s_addr) {
            /* Already exists */
            break;
        }
    }

    /* Create new peer */
    if (peer == NULL) {
        peer = ZET76_MALLOC(struct zet76_snmp_peer);
        ZET76_EXIT_IF_NULL(peer, ZET76_NOT_ENOUGH_MEMORY);

        peer->fade_event = event_new(host->snmp->server->base,
                                     -1,
                                     EV_TIMEOUT,
                                     zet76_snmp_peer_on_fade,
                                     peer);
        ZET76_EXIT_IF_NULL(peer->fade_event, ZET76_NOT_ENOUGH_MEMORY);

        ZET76_LIST_INSERT(&host->peers, peer, entry);
    }

    zet76_event_add(peer->fade_event, ZET76_TIMEOUT_NODE_FADE);

    /* Reset parameters */
    peer->id              = *device;
    peer->addr.sin_family = AF_INET;
    peer->addr.sin_addr   = in_addr;
    peer->addr.sin_port   = 0;
    peer->mb3180_avail    = 0;
    peer->tcp_state       = ZET76_SNMP_TCP_UNKNOWN;
    peer->baud_rate       = 0;
    peer->parity          = ZET76_PARITY_UNKNOWN;
    peer->request_id      = device->serial & 0xFFFF;

    if (peer->request_id < ZET76_MIN_SNMP_REQUEST_ID)
        peer->request_id = ZET76_MIN_SNMP_REQUEST_ID;
    if (peer->request_id > ZET76_MAX_SNMP_REQUEST_ID)
        peer->request_id = ZET76_MAX_SNMP_REQUEST_ID;

cleanup:
    if (error != ZET76_OK) {
        if (peer != NULL) {
            ZET76_FREE(peer->fade_event, event_free);
            free(peer);
        }
    }
    return peer;
}

/****************************************************************************/
static void zet76_snmp_peer_free(struct zet76_snmp_peer *peer)
{
    ZET76_TRACE("  closing peer <0x%I64x>", peer->id.serial);

    ZET76_FREE(peer->fade_event, event_free);
    ZET76_LIST_REMOVE(peer, entry);

    free(peer);
}

/****************************************************************************/
static void zet76_snmp_on_response(evutil_socket_t sock,
                                   short which, void *arg)
{
    char buffer[ZET76_SIZE_SNMP_HOST];
    struct zet76_snmp_object object;
    struct sockaddr_in addr;
    struct zet76_device device;

    struct zet76_snmp_host *host = (struct zet76_snmp_host *) arg;
    struct zet76_snmp_peer *peer = NULL;
    struct zet76_snmp *snmp = NULL;
    const char *cstr = NULL;
    unsigned long long decimal = 0;
    unsigned long ip = 0;
    unsigned char request_type = ZET76_SNMP_TYPE_UNKNOWN;
    struct zet76_snmp_oid *request_name = NULL;
    int request_id = 0;
    int buffer_size = 0;
    int addr_size = sizeof(addr);
    int i = 0;
    int r = 0;

    if (!host)
        return;

    snmp = host->snmp;

    /* This 'while' statement is actually 'if' with 'break's */
    while (which & EV_READ) {
        r = recvfrom(sock,
                     buffer,
                     sizeof(buffer),
                     0,
                     (struct sockaddr *) &addr,
                     &addr_size);
        if (r <= 0)
            break;

        r = zet76_snmp_decode_response(buffer,
                                       r,
                                       &request_id,
                                       &object);
        if (r <= 0)
            break;

        if (zet76_snmp_compare_oid(&object.name, &snmp->sys_name_oid) == 0) {
            if (request_id != host->broadcast_request_id)
                break;
            if (object.value.type != ZET76_SNMP_TYPE_STRING)
                break;

            device.type = ZET76_DEVICE_TYPE_UNKNOWN;

            cstr = object.value.data.string.octets;
            r = strlen(ZET76_SNMP_MOXA_NE_4110A);
            if (memcmp(cstr, ZET76_SNMP_MOXA_NE_4110A, r) == 0) {
                if (zet76_cstr_to_dec(&cstr[r], &device.serial) == 0)
                    break;

                device.type = ZET76_DEVICE_TYPE_7076;
                device.serial &= ~(ZET76_SNMP_SERIAL_MASK);
                device.serial |= ZET76_SNMP_SERIAL_MOXA_NE_4110A;
            }
            r = strlen(ZET76_SNMP_MOXA_NPORT_W2150A);
            if (memcmp(cstr, ZET76_SNMP_MOXA_NPORT_W2150A, r) == 0) {
                if (zet76_cstr_to_dec(&cstr[r], &device.serial) == 0)
                    break;

                device.type = ZET76_DEVICE_TYPE_7076;
                device.serial &= ~(ZET76_SNMP_SERIAL_MASK);
                device.serial |= ZET76_SNMP_SERIAL_MOXA_NPORT_W2150A;
            }
            r = strlen(ZET76_SNMP_MOXA_MGATE_MB3180);
            if (memcmp(cstr, ZET76_SNMP_MOXA_MGATE_MB3180, r) == 0) {
                if (zet76_cstr_to_dec(&cstr[r], &device.serial) == 0)
                    break;

                device.type = ZET76_DEVICE_TYPE_7076;
                device.serial &= ~(ZET76_SNMP_SERIAL_MASK);
                device.serial |= ZET76_SNMP_SERIAL_MOXA_MGATE_MB3180;
            }
            if (device.type == ZET76_DEVICE_TYPE_UNKNOWN)
                break;

            ZET76_TRACE("detected device \"%s\" <0x%I64x> from <%s:%u>",
                        object.value.data.string.octets,
                        device.serial,
                        inet_ntoa(addr.sin_addr),
                        ntohs(addr.sin_port));

            peer = zet76_snmp_peer_create(host, &device, addr.sin_addr);

            if (zet76_snmp_peer_is_mb3180(peer)) {
                /* Skip SNMP location for MGate - port is already known */
                peer->addr.sin_port =
                        htons((unsigned short) ZET76_MODBUS_TCP_DEFAULT_PORT);

                request_type = ZET76_SNMP_TYPE_PDU_GET_NEXT;
                request_name = &snmp->tcp_state_oid;
            } else {
                request_type = ZET76_SNMP_TYPE_PDU_GET;
                request_name = &snmp->sys_location_oid;
            }
            break;
        }

        /* Find peer */
        for (peer  = ZET76_LIST_FIRST(&host->peers);
             peer != NULL;
             peer  = ZET76_LIST_NEXT(peer, entry))
        {
            if (peer->addr.sin_addr.s_addr == addr.sin_addr.s_addr) {
                break;
            }
        }
        if (peer == NULL)
            break;
        if (peer->request_id != request_id)
            break;

        /* Got sysLocation (string) containing TCP port */
        if (zet76_snmp_compare_oid(&object.name,
                                   &snmp->sys_location_oid) == 0)
        {
            if (object.value.type != ZET76_SNMP_TYPE_STRING)
                break;

            cstr = object.value.data.string.octets;
            if (zet76_cstr_to_dec(cstr, &decimal) == 0)
                break;

            /* Validate port */
            if (decimal <= 0 || decimal > ZET76_MAX_PORT)
                break;

            peer->addr.sin_port = htons((unsigned short) decimal);
            ZET76_TRACE("port %I64d\n", decimal);

            /* Get request for TCP conn state */
            request_type = ZET76_SNMP_TYPE_PDU_GET_NEXT;
            request_name = &snmp->tcp_state_oid;
            break;
        }

        /* Got tcpConnState */
        if (zet76_snmp_compare_oid(&object.name, &snmp->tcp_state_oid) > 0)
        {
            if (object.name.len < snmp->tcp_state_oid.len + 10)
                break;
            if (object.value.type != ZET76_SNMP_TYPE_INTEGER)
                break;
            if (peer->addr.sin_port == 0)
                break;
//            if (peer->tcp_state != ZET76_SNMP_TCP_UNKNOWN)
                //break;

            /* Validate index ip.port */
            if ((peer->id.serial & ZET76_SNMP_SERIAL_MASK) ==
                ZET76_SNMP_SERIAL_MOXA_NPORT_W2150A)
            {
                /* In NPort, index ip should match 0.0.0.0 */
                ip = 0x0;
            }
            else
            {
                /* In other devices, index ip should match device ip */
                ip = ntohl(peer->addr.sin_addr.s_addr);
            }

            /* Check IP addr */
            i = snmp->tcp_state_oid.len;
            if (object.name.integers[i + 0] != ((ip >> 24) & 0xFF) ||
                object.name.integers[i + 1] != ((ip >> 16) & 0xFF) ||
                object.name.integers[i + 2] != ((ip >>  8) & 0xFF) ||
                object.name.integers[i + 3] != ((ip >>  0) & 0xFF))
            {
                /* Request next */
                request_type = ZET76_SNMP_TYPE_PDU_GET_NEXT;
                request_name = &object.name;
                break;
            }

            /* Check port number */
            if (object.name.integers[i + 4] != ntohs(peer->addr.sin_port)) {
                if (zet76_snmp_peer_is_mb3180(peer) && peer->mb3180_avail) {
                    /* MGate MB3180 is really available */
                    peer->mb3180_avail = 0;
                    peer->tcp_state = ZET76_SNMP_TCP_LISTEN;
                    ZET76_TRACE("tcp state %d", peer->tcp_state);
                    request_type = ZET76_SNMP_TYPE_PDU_GET;
                    request_name = &snmp->rs232_type_oid;
                    break;
                }

                /* Request next */
                request_type = ZET76_SNMP_TYPE_PDU_GET_NEXT;
                request_name = &object.name;
                break;
            }

            /* For MGate MB3180, we should check next tcp entries */
            if (zet76_snmp_peer_is_mb3180(peer) &&
                object.name.integers[i + 5] == 0 &&
                object.name.integers[i + 6] == 0 &&
                object.name.integers[i + 7] == 0 &&
                object.name.integers[i + 8] == 0 &&
                object.name.integers[i + 9] == 0 &&
                ((int) object.value.data.integer) == ZET76_SNMP_TCP_LISTEN)
            {
                /* Remember and request next */
                peer->mb3180_avail = 1;
                request_type = ZET76_SNMP_TYPE_PDU_GET_NEXT;
                request_name = &object.name;
                break;
            }

            /* Update state and start reading RS232 port 1 params */
            peer->tcp_state = (int) object.value.data.integer;
            ZET76_TRACE("tcp state %d", peer->tcp_state);

#if 0
            /* Serial Settings will be requested on connection */
            if (peer->tcp_state == ZET76_SNMP_TCP_LISTEN)
                r = ZDCP_REPORT_READY;
            else
                r = ZDCP_REPORT_BUSY;
            zet76_device_on_report(snmp->server,
                                   &peer->id,
                                   &peer->addr,
                                   r,
                                   peer->baud_rate,
                                   peer->parity);
#endif

            request_type = ZET76_SNMP_TYPE_PDU_GET;
            request_name = &snmp->rs232_type_oid;
            break;
        }

        /* Got RS232 type */
        if (zet76_snmp_compare_oid(&object.name,
                                   &snmp->rs232_type_oid) == 0)
        {
            if (object.value.type != ZET76_SNMP_TYPE_INTEGER)
                break;

            /* RS-485 has type 'other' */
            if ((peer->id.serial & ZET76_SNMP_SERIAL_MASK) !=
                ZET76_SNMP_SERIAL_MOXA_NE_4110A)
            {
                if (object.value.data.integer ==
                    ZET76_SNMP_RS232_TYPE_UNKNOWN)
                {
                    object.value.data.integer = ZET76_SNMP_RS232_TYPE_OTHER;
                }
            }
            if (object.value.data.integer != ZET76_SNMP_RS232_TYPE_OTHER)
                break;

            request_type = ZET76_SNMP_TYPE_PDU_GET;
            request_name = &snmp->rs232_speed_oid;
            break;
        }

        /* Got RS232 speed (baud rate) */
        if (zet76_snmp_compare_oid(&object.name,
                                   &snmp->rs232_speed_oid) == 0)
        {
            if (object.value.type != ZET76_SNMP_TYPE_INTEGER)
                break;

            peer->baud_rate = (int) object.value.data.integer;
            request_type = ZET76_SNMP_TYPE_PDU_GET;
            request_name = &snmp->rs232_bits_oid;
            break;
        }

        /* Got RS232 data bits (5..8) */
        if (zet76_snmp_compare_oid(&object.name,
                                   &snmp->rs232_bits_oid) == 0)
        {
            if (object.value.type != ZET76_SNMP_TYPE_INTEGER ||
                object.value.data.integer != 8)
            {
                peer->baud_rate = 0;
                peer->parity = ZET76_PARITY_UNKNOWN;
                break;
            }
            request_type = ZET76_SNMP_TYPE_PDU_GET;
            request_name = &snmp->rs232_parity_oid;
            break;
        }

        /* Got last required component - RS232 parity */
        if (zet76_snmp_compare_oid(&object.name,
                                   &snmp->rs232_parity_oid) == 0)
        {
            if (object.value.type != ZET76_SNMP_TYPE_INTEGER)
                break;

            peer->parity = (int) object.value.data.integer;

            ZET76_TRACE("port baud %d parity %d",
                        peer->baud_rate, peer->parity);
            if (peer->addr.sin_port == 0 ||
                peer->tcp_state     == ZET76_SNMP_TCP_UNKNOWN ||
                peer->baud_rate     == 0 ||
                peer->parity        == ZET76_PARITY_UNKNOWN)
            {
                break;
            }

            /* Now we can register device node */
            if (peer->tcp_state == ZET76_SNMP_TCP_LISTEN)
                r = ZDCP_REPORT_READY;
            else
                r = ZDCP_REPORT_BUSY;
            zet76_device_on_report(snmp->server,
                                   &peer->id,
                                   &peer->addr,
                                   r,
                                   peer->baud_rate,
                                   peer->parity,
                                   NULL,
                                   NULL,
                                   0);
            break;
        }

        /* Break here because our 'while' is actually 'if' */
        break;
    }

    if (peer != NULL &&
        request_type != ZET76_SNMP_TYPE_UNKNOWN &&
        request_name != NULL)
    {
        /* Circular decrement */
        peer->request_id--;
        if (peer->request_id < ZET76_MIN_SNMP_REQUEST_ID)
            peer->request_id = ZET76_MAX_SNMP_REQUEST_ID;
        if (peer->request_id > ZET76_MAX_SNMP_REQUEST_ID)
            peer->request_id = ZET76_MIN_SNMP_REQUEST_ID;

        buffer_size = zet76_snmp_encode_request(buffer,
                                                sizeof(buffer),
                                                request_type,
                                                peer->request_id,
                                                request_name);
        if (buffer_size > 0) {
            r = sendto(sock,
                       buffer,
                       buffer_size,
                       0,
                       (const struct sockaddr *) &addr,
                       sizeof(addr));
        }
    }

//             if (status != ZDCP_REPORT_BUSY &&
//                 ntohs(addr.sin_port) <= ZET76_MAX_PORT)
//             {
//                 zet76_device_on_report(snmp->server,
//                                        &device,
//                                        &addr,
//                                        status);
//             }

    if (which & EV_TIMEOUT) {
        /* Circular increment */
        host->broadcast_request_id++;
        if (host->broadcast_request_id < ZET76_MIN_SNMP_REQUEST_ID)
            host->broadcast_request_id = ZET76_MAX_SNMP_REQUEST_ID;
        if (host->broadcast_request_id > ZET76_MAX_SNMP_REQUEST_ID)
            host->broadcast_request_id = ZET76_MIN_SNMP_REQUEST_ID;

        buffer_size = zet76_snmp_encode_request(buffer,
                                                sizeof(buffer),
                                                ZET76_SNMP_TYPE_PDU_GET,
                                                host->broadcast_request_id,
                                                &snmp->sys_name_oid);
        if (buffer_size > 0) {
            r = sendto(sock,
                       buffer,
                       buffer_size,
                       0,
                       (const struct sockaddr *) &snmp->broadcast_addr,
                       sizeof(snmp->broadcast_addr));
        }

        // ZET76_TRACE("snmp broadcast size %d result %d", buffer_size, r);
    }
}

/****************************************************************************/
static void zet76_snmp_on_connect(struct zet76_server *server,
                                  const struct zet76_device *id)
{
    char buffer[ZET76_SIZE_SNMP_HOST];
    struct sockaddr_in addr;
    struct zet76_snmp *snmp;
    struct zet76_snmp_host *host;
    struct zet76_snmp_peer *peer;
    int buffer_size = 0;
    int r = 0;

    /* Find peer */
    for (snmp  = ZET76_LIST_FIRST(&server->snmps);
         snmp != NULL;
         snmp  = ZET76_LIST_NEXT(snmp, entry))
    {
        for (host  = ZET76_LIST_FIRST(&snmp->hosts);
             host != NULL;
             host  = ZET76_LIST_NEXT(host, entry))
        {
            for (peer  = ZET76_LIST_FIRST(&host->peers);
                 peer != NULL;
                 peer  = ZET76_LIST_NEXT(peer, entry))
            {
                if (zet76_device_compare(&peer->id, id) == 0) {
                    goto peer_found;
                }
            }
        }
    }
    return;

peer_found:
    /* Circular decrement */
    peer->request_id--;
    if (peer->request_id < ZET76_MIN_SNMP_REQUEST_ID)
        peer->request_id = ZET76_MAX_SNMP_REQUEST_ID;
    if (peer->request_id > ZET76_MAX_SNMP_REQUEST_ID)
        peer->request_id = ZET76_MIN_SNMP_REQUEST_ID;

    buffer_size = zet76_snmp_encode_request(buffer,
                                            sizeof(buffer),
                                            ZET76_SNMP_TYPE_PDU_GET,
                                            peer->request_id,
                                            &snmp->rs232_type_oid);
    if (buffer_size > 0) {
        addr = peer->addr;
        addr.sin_port = htons(ZET76_SNMP_PORT);

        r = sendto(host->sock,
                   buffer,
                   buffer_size,
                   0,
                   (const struct sockaddr *) &addr,
                   sizeof(addr));
    }
}

/****************************************************************************/
static int zet76_snmp_peer_is_mb3180(struct zet76_snmp_peer *peer)
{
    unsigned long serial_mask;

    if (peer->id.type != ZET76_DEVICE_TYPE_7076)
        return 0;

    serial_mask = (peer->id.serial & ZET76_SNMP_SERIAL_MASK);
    return (serial_mask == ZET76_SNMP_SERIAL_MOXA_MGATE_MB3180);
}

/****************************************************************************/
/* Fade timeout - means snmp peer is lost */
static void zet76_snmp_peer_on_fade(evutil_socket_t sock,
                                    short which, void *arg)
{
    struct zet76_snmp_peer *peer = (struct zet76_snmp_peer *) arg;

    ZET76_UNUSED_PARAM(sock);
    ZET76_UNUSED_PARAM(which);

    ZET76_TRACE("");
    zet76_snmp_peer_free(peer);
}

/****************************************************************************/
ZET76_EXPORT zet76_listener_start(struct zet76_server *server,
                                  unsigned short listen_port)
{
    struct zet76_listener *listener = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM(listen_port <= ZET76_MAX_PORT);

    if (listen_port == 0)
        listen_port = ZET76_LISTEN_PORT;

    ZET76_GUARD_LOCK(server->guard);
    for (listener  = ZET76_LIST_FIRST(&server->listeners);
         listener != NULL;
         listener  = ZET76_LIST_NEXT(listener, entry))
    {
        if (listener->listen_port == listen_port) {
            ZET76_TRACE("duplicate listener found");
            ZET76_REFCOUNT_INC(listener->ref_count);
            ZET76_EXIT(ZET76_OK);
        }
    }

    listener = ZET76_MALLOC(struct zet76_listener);
    ZET76_EXIT_IF_NULL(listener, ZET76_NOT_ENOUGH_MEMORY);

    listener->listen_port = listen_port;
    listener->server      = server;
    ZET76_REFCOUNT_INIT(listener->ref_count, 1);
    ZET76_LIST_INIT(&listener->hosts);
    ZET76_LIST_INIT(&listener->peers);

    ZET76_LIST_INSERT(&server->listeners, listener, entry);

    zet76_listener_restart(listener);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_listener_stop(struct zet76_server *server,
                                 unsigned short listen_port)
{
    struct zet76_listener *listener = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM(listen_port <= ZET76_MAX_PORT);

    if (listen_port == 0) {
        listen_port = ZET76_LISTEN_PORT;
    }

    ZET76_GUARD_LOCK(server->guard);
    for (listener  = ZET76_LIST_FIRST(&server->listeners);
         listener != NULL;
         listener  = ZET76_LIST_NEXT(listener, entry))
    {
        if (listener->listen_port == listen_port) {
            if (ZET76_REFCOUNT_DEC(listener->ref_count) == 0) {
                zet76_listener_free(listener);
            } else {
                ZET76_TRACE("listener shared");
            }
            ZET76_EXIT(ZET76_OK);
        }
    }

    ZET76_EXIT(ZET76_OBJECT_NOT_FOUND);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    return error;
}

/****************************************************************************/
static void zet76_listener_free(struct zet76_listener *listener)
{
    struct zet76_server *server = listener->server;

    ZET76_GUARD_LOCK(server->guard);
    ZET76_LIST_REMOVE(listener, entry);
    ZET76_GUARD_UNLOCK(server->guard);

    ZET76_LIST_CLEAR(&listener->hosts, zet76_listener_host_free);
    ZET76_LIST_CLEAR(&listener->peers, zet76_listener_peer_free);
    free(listener);
}

/****************************************************************************/
static void zet76_listener_restart(struct zet76_listener *listener)
{
    struct zet76_server *server = NULL;
    int index = 0;

    /* Open listener_host for each network address */

    server = listener->server;
    ZET76_GUARD_LOCK(server->guard);
    ZET76_LIST_CLEAR(&listener->hosts, zet76_listener_host_free);

    for (index = 0; index < server->networks_count; index++) {
        zet76_listener_host_create(listener, &server->networks[index].addr);
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_listener_on_cmd(struct evconnlistener *cmd,
                                  evutil_socket_t sock,
                                  struct sockaddr *addr,
                                  int addr_size,
                                  void *arg)
{
    struct zet76_listener *listener = (struct zet76_listener *) arg;
    struct zet76_listener_peer *peer = NULL;
    const struct sockaddr_in *peer_addr = (const struct sockaddr_in *) addr;
    int error = 0;

    for (peer  = ZET76_LIST_FIRST(&listener->peers);
         peer != NULL;
         peer  = ZET76_LIST_NEXT(peer, entry))
    {
        if (zet76_sockaddr_compare(&peer->addr, peer_addr) == 0)
            break;
    }
    if (peer != NULL) {
        ZET76_EXIT(ZET76_OK);
    }

    peer = ZET76_MALLOC(struct zet76_listener_peer);
    ZET76_EXIT_IF_NULL(peer, ZET76_NOT_ENOUGH_MEMORY);

    peer->addr      = *((struct sockaddr_in *) addr);
    peer->listener  = listener;
    peer->id.serial = 0;
    peer->id.type   = 0;
    peer->sock_cmd  = sock;
    peer->sock_dat  = INVALID_SOCKET;

    peer->read_event = event_new(listener->server->base,
                                 sock,
                                 EV_READ | EV_TIMEOUT,
                                 zet76_listener_on_report,
                                 peer);
    ZET76_EXIT_IF_NULL(peer->read_event, ZET76_NOT_ENOUGH_MEMORY);

    zet76_event_add(peer->read_event, ZET76_TIMEOUT_LISTENER_READ);

    ZET76_LIST_INSERT(&listener->peers, peer, entry);

cleanup:
    ZET76_TRACE("!!!! accept cmd %X sock %X from <%s:%u> error %X",
                cmd,
                sock,
                inet_ntoa(((struct sockaddr_in *) addr)->sin_addr),
                ntohs(((struct sockaddr_in *) addr)->sin_port),
                error);

    if (error != ZET76_OK) {
        if (peer != NULL) {
            ZET76_FREE(peer->read_event, event_free);
            free(peer);
        }
        evutil_closesocket(sock);
    }
}

/****************************************************************************/
static void zet76_listener_on_dat(struct evconnlistener *dat,
                                  evutil_socket_t sock,
                                  struct sockaddr *addr,
                                  int addr_size,
                                  void *arg)
{
    struct sockaddr_in cmd_addr;

    struct zet76_listener *listener = (struct zet76_listener *) arg;
    struct zet76_listener_peer *peer = NULL;
    int error = 0;

    cmd_addr = *((struct sockaddr_in *) addr);
    ZET76_EXIT_IF_FALSE((cmd_addr.sin_port != 0), ZET76_OBJECT_NOT_FOUND);

    cmd_addr.sin_port = htons(ntohs(cmd_addr.sin_port) - 1);

    for (peer  = ZET76_LIST_FIRST(&listener->peers);
         peer != NULL;
         peer  = ZET76_LIST_NEXT(peer, entry))
    {
        if (zet76_sockaddr_compare(&peer->addr, &cmd_addr) == 0)
            break;
    }
    ZET76_EXIT_IF_NULL(peer, ZET76_OBJECT_NOT_FOUND - 1);

    ZET76_EXIT_IF_FALSE((peer->sock_cmd != INVALID_SOCKET),
                        ZET76_OBJECT_NOT_FOUND);

    peer->sock_dat = sock;
    zet76_listener_accept(peer);

cleanup:
    ZET76_TRACE("!!!! accept dat %X sock %X from <%s:%u> error %d",
                dat,
                sock,
                inet_ntoa(((struct sockaddr_in *) addr)->sin_addr),
                ntohs(((struct sockaddr_in *) addr)->sin_port),
                error);
    if (error != ZET76_OK) {
        evutil_closesocket(sock);
    }
}

/****************************************************************************/
static void zet76_listener_on_report(evutil_socket_t sock,
                                     short which, void *arg)
{
    char buffer[ZET76_SIZE_SCANNER_HOST];
    ZDCP_REPORT* report = (ZDCP_REPORT*) buffer;

    struct zet76_listener_peer *peer = (struct zet76_listener_peer *) arg;
    int r = 0;

    if (sock != peer->sock_cmd)
        return;

    if (which & EV_READ) {
        r = recv(sock, buffer, sizeof(ZDCP_REPORT), 0);
        if (r >= sizeof(ZDCP_REPORT)) {
            r = zet76_zdcp_report_parse((const ZDCP_REPORT *) buffer,
                                        r,
                                        &peer->id,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL);
            if (r == ZET76_OK) {
                ZET76_TRACE("device <%s #%016llx>",
                            zet76_device_type_str(peer->id.type),
                            peer->id.serial);

                zet76_listener_accept(peer);
                return;
            }
        }

        ZET76_TRACE("read not report");
    }

    ZET76_TRACE("read which %X%s", which,
                (which & EV_TIMEOUT) ? " TIMEOUT" : "");

    zet76_listener_peer_free(peer);
}

/****************************************************************************/
static void zet76_listener_accept(struct zet76_listener_peer *peer)
{
    struct zet76_server *server = peer->listener->server;
    struct zet76_node *node = NULL;
    struct bufferevent *cmd = NULL;
    struct bufferevent *dat = NULL;
    struct evbuffer *evb = NULL;
    int error = ZET76_OK;

    if (peer->sock_cmd  == INVALID_SOCKET ||
        peer->id.type   == 0 ||
        peer->id.serial == 0)
    {
        ZET76_TRACE("not yet ready");
        return;
    }

    switch (peer->id.type) {
    case ZET76_DEVICE_TYPE_7176:
        if (peer->sock_dat == INVALID_SOCKET) {
            ZET76_TRACE("zet7176 not yet ready");
            return;
        }
        break;
    case ZET76_DEVICE_TYPE_7177:
        if (peer->sock_dat != INVALID_SOCKET) {
            ZET76_TRACE("zet7177 got too many connections");
            zet76_listener_peer_free(peer);
            return;
        }
        break;
    default:
        ZET76_TRACE("Device type %d is not allowed to connect", peer->id.type);
        zet76_listener_peer_free(peer);
        return;
    }

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_create(server, &peer->id);
    ZET76_EXIT_IF_NULL(node, ZET76_NOT_ENOUGH_MEMORY);

    switch (peer->id.type) {
    case ZET76_DEVICE_TYPE_7176:
        if (node->flags & ZET76_DEVICE_ACCEPTABLE) {
            /* Reconnect */
            zet76_node_stop_accept(node);
        }
        node->accept_cmd = bufferevent_socket_new(server->base,
                                                  peer->sock_cmd,
                                                  ZET76_BEV_OPTIONS);
        ZET76_EXIT_IF_NULL(node->accept_cmd, ZET76_NOT_ENOUGH_MEMORY);

        peer->sock_cmd = INVALID_SOCKET;
        bufferevent_disable(node->accept_cmd, EV_READ | EV_WRITE);
        zet76_bufferevent_set_timeouts(node->accept_cmd,
                                       ZET76_TIMEOUT_NODE_DATA,
                                       ZET76_TIMEOUT_NODE_SEND);
        bufferevent_setcb(node->accept_cmd,
                          zet76_node_on_read,
                          NULL,
                          zet76_node_on_event,
                          node);

        node->accept_dat = bufferevent_socket_new(server->base,
                                                  peer->sock_dat,
                                                  ZET76_BEV_OPTIONS);
        ZET76_EXIT_IF_NULL(node->accept_dat, ZET76_NOT_ENOUGH_MEMORY);

        peer->sock_dat = INVALID_SOCKET;
        bufferevent_disable(node->accept_dat, EV_READ | EV_WRITE);
        zet76_bufferevent_set_timeouts(node->accept_dat,
                                       ZET76_TIMEOUT_NODE_DATA,
                                       ZET76_TIMEOUT_NODE_SEND);
        bufferevent_setcb(node->accept_dat,
                          zet76_node_on_read,
                          NULL,
                          zet76_node_on_event,
                          node);
        break;
    case ZET76_DEVICE_TYPE_7177:
        if (node->flags & ZET76_DEVICE_ACCEPTABLE) {
            /* Reconnect */
#if 0
            if (node->accept_dat) {
                /* Save data from previous connection */
                evb = evbuffer_new();
                ZET76_EXIT_IF_NULL(evb, ZET76_NOT_ENOUGH_MEMORY);
                bufferevent_read_buffer(node->accept_dat, evb);
            }
#endif
            zet76_node_stop_accept(node);
            //ZET76_FREE(node->accept_dat, bufferevent_free);
            //ZET76_FREE(node->accept_cmd, bufferevent_free);
        }
        node->accept_dat = bufferevent_socket_new(server->base,
                                                  peer->sock_cmd,
                                                  ZET76_BEV_OPTIONS);
        ZET76_EXIT_IF_NULL(node->accept_dat, ZET76_NOT_ENOUGH_MEMORY);

        peer->sock_cmd = INVALID_SOCKET;
        bufferevent_disable(node->accept_dat, EV_READ | EV_WRITE);
        zet76_bufferevent_set_timeouts(node->accept_dat,
                                       ZET76_TIMEOUT_NODE_DATA_GSM,
                                       ZET76_TIMEOUT_NODE_SEND);
        bufferevent_setcb(node->accept_dat,
                          zet76_node_on_read,
                          NULL,
                          zet76_node_on_event,
                          node);
        if (evb) {
            bufferevent_write_buffer(node->accept_dat, evb);
        }
        break;
    }

    node->accept_addr = peer->addr;
    node->flags |= ZET76_DEVICE_ACCEPTABLE;

cleanup:
//    if (error != ZET76_OK) {
        zet76_listener_peer_free(peer);
    //}
    if (evb) {
       evbuffer_free(evb);
    }

    if (error == ZET76_NOT_ENOUGH_MEMORY) {
        if (node != NULL) {
            ZET76_FREE(node->accept_dat, bufferevent_free);
            ZET76_FREE(node->accept_cmd, bufferevent_free);
        }
    }

    if (error == ZET76_OK) {
        ZET76_TRACE("device <%s #%016llx> <%s:%u> became acceptable",
                    zet76_device_type_str(node->id.type),
                    node->id.serial,
                    inet_ntoa(node->accept_addr.sin_addr),
                    ntohs(node->accept_addr.sin_port));
        zet76_node_notify_status(node);
    }
    zet76_node_handle_state(node);

    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_listener_host_create(struct zet76_listener *listener,
                                       const struct in_addr *addr)
{
    struct sockaddr_in host_addr;

    struct zet76_listener_host *host = NULL;
    struct evconnlistener *cmd = NULL;
    struct evconnlistener *dat = NULL;
    int error = ZET76_OK;

    host_addr.sin_family = AF_INET;
    host_addr.sin_addr   = *addr;
    host_addr.sin_port   = htons(listener->listen_port);
    cmd = evconnlistener_new_bind(listener->server->base,
                                  zet76_listener_on_cmd,
                                  listener,
                                  LEV_OPT_CLOSE_ON_FREE |
                                  LEV_OPT_REUSEABLE,
                                  10,
                                  (struct sockaddr *) &host_addr,
                                  sizeof(host_addr));
    ZET76_EXIT_IF_NULL(cmd, ZET76_NOT_ENOUGH_MEMORY);

    host_addr.sin_port = htons(listener->listen_port + 1);
    dat = evconnlistener_new_bind(listener->server->base,
                                  zet76_listener_on_dat,
                                  listener,
                                  LEV_OPT_CLOSE_ON_FREE |
                                  LEV_OPT_REUSEABLE,
                                  10,
                                  (struct sockaddr *) &host_addr,
                                  sizeof(host_addr));
    ZET76_EXIT_IF_NULL(dat, ZET76_NOT_ENOUGH_MEMORY);

    host = ZET76_MALLOC(struct zet76_listener_host);
    ZET76_EXIT_IF_NULL(host, ZET76_NOT_ENOUGH_MEMORY);

    host->addr = host_addr;
    host->cmd  = cmd;
    host->dat  = dat;

    ZET76_LIST_INSERT(&listener->hosts, host, entry);

    ZET76_TRACE("listening to <%s:%u/%u>",
                inet_ntoa(host_addr.sin_addr),
                listener->listen_port,
                listener->listen_port + 1);

cleanup:
    if (error != ZET76_OK) {
        if (host != NULL) {
            free(host);
        }
        if (dat != NULL) {
            evconnlistener_free(dat);
        }
        if (cmd != NULL) {
            evconnlistener_free(cmd);
        }
    }
}

/****************************************************************************/
static void zet76_listener_host_free(struct zet76_listener_host *host)
{
    ZET76_TRACE("  closing hostd");

    ZET76_FREE(host->dat, evconnlistener_free);
    ZET76_FREE(host->cmd, evconnlistener_free);
    ZET76_LIST_REMOVE(host, entry);
    free(host);
}

/****************************************************************************/
static void zet76_listener_peer_free(struct zet76_listener_peer *peer)
{
    ZET76_TRACE("<%s:%u>",
                inet_ntoa(peer->addr.sin_addr),
                ntohs(peer->addr.sin_port));

    if (peer->sock_dat != INVALID_SOCKET)
        evutil_closesocket(peer->sock_dat);
    if (peer->sock_cmd != INVALID_SOCKET)
        evutil_closesocket(peer->sock_cmd);

    ZET76_FREE(peer->read_event, event_free);
    ZET76_LIST_REMOVE(peer, entry);
    free(peer);
}

/****************************************************************************/
static int zet76_device_compare(const struct zet76_device *device1,
                                const struct zet76_device *device2)
{
    /*
    ZET76_TRACE("<%s:0x%016llX> vs <%s:0x%016llX>",
                zet76_device_type_str(device1->type), device1->serial,
                zet76_device_type_str(device2->type), device2->serial);
    */
    if (device1->type   < device2->type)    return -1;
    if (device1->type   > device2->type)    return +1;
    if (device1->serial < device2->serial)  return -1;
    if (device1->serial > device2->serial)  return +1;
    return 0;
}

/****************************************************************************/
static struct zet76_node *zet76_node_open(struct zet76_server *server,
                                          const struct zet76_device *id)
{
    struct zet76_node *node = NULL;

    ZET76_GUARD_LOCK(server->guard);
    for (node  = ZET76_LIST_FIRST(&server->nodes);
         node != NULL;
         node  = ZET76_LIST_NEXT(node, entry))
    {
        if (zet76_device_compare(&node->id, id) == 0)
            break;
    }
    ZET76_GUARD_UNLOCK(server->guard);

    return node;
}

/****************************************************************************/
static struct zet76_node *zet76_node_create(struct zet76_server *server,
                                            const struct zet76_device *id)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_GUARD_LOCK(server->guard);

    node = zet76_node_open(server, id);
    if (node != NULL) {
        //ZET76_TRACE("device exists with state %d flags 0x%X modes 0x%X",
        //            node->state, node->flags, node->modes);
        ZET76_EXIT(ZET76_OK);
    }

    node = ZET76_MALLOC(struct zet76_node);
    ZET76_EXIT_IF_NULL(node, ZET76_NOT_ENOUGH_MEMORY);

    node->magic  = ZET76_MAGIC_NODE;
    node->server = server;

    node->id        = *id;
    node->baud_rate = 0;
    node->parity    = ZET76_PARITY_UNKNOWN;

    node->label_len        = -1;
    node->label[0]         = '\0';
    node->peer_addr.s_addr = INADDR_ANY;
    node->peer_addr_avail  = 0;

    node->state     = ZET76_DEVICE_IDLE;
    node->flags     = 0;
    node->modes     = 0;

    node->fade_event    = NULL;
    node->disable_event = NULL;

    node->connect_addr.sin_family      = AF_INET;
    node->connect_addr.sin_addr.s_addr = INADDR_ANY;
    node->connect_addr.sin_port        = 0;
    node->connect_cmd                  = NULL;
    node->connect_dat                  = NULL;
    node->connect_flags                = 0;

    node->control_addr.sin_family      = AF_INET;
    node->control_addr.sin_addr.s_addr = INADDR_ANY;
    node->control_addr.sin_port        = 0;
    node->control_bev                  = NULL;
    node->control_buf                  = NULL;
    node->control_send                 = ZET76_CONTROL_SEND_IDLE;
    node->control_recv                 = ZET76_CONTROL_RECV_HEADER;
    node->control_zdcp_seq             = 0;
    node->control_zdcp_cmd             = ZDCP_COMMAND_PING;
    node->control_zdcp_res             = ZDCP_RESPONSE_OK;
    node->control_recv_size            = 0;

    node->accept_addr.sin_family       = AF_INET;
    node->accept_addr.sin_addr.s_addr  = INADDR_ANY;
    node->accept_addr.sin_port         = 0;
    node->accept_cmd                   = NULL;
    node->accept_dat                   = NULL;

    node->rx_bytes                     = 0;

    node->fade_event = event_new(node->server->base,
                                 -1,
                                 EV_TIMEOUT,
                                 zet76_node_on_fade,
                                 node);
    ZET76_EXIT_IF_NULL(node->fade_event, ZET76_NOT_ENOUGH_MEMORY);

    ZET76_LIST_INSERT(&server->nodes, node, entry);
    ZET76_TRACE("device created");

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    if (error != ZET76_OK) {
        if (node != NULL) {
            zet76_node_free(node);
            node = NULL;
        }
    }
    return node;
}

/****************************************************************************/
static void zet76_device_on_report(struct zet76_server *server,
                                   const struct zet76_device *device,
                                   const struct sockaddr_in *addr,
                                   unsigned short status,
                                   int baud_rate,
                                   int parity,
                                   const struct in_addr *peer_addr,
                                   const char *label,
                                   int label_len)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_create(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_NOT_ENOUGH_MEMORY);

    if (status == ZDCP_REPORT_BUSY) {
        switch (node->state) {
        case ZET76_DEVICE_CONNECTING:
        case ZET76_DEVICE_CONNECTED:
        case ZET76_DEVICE_CONTROLLING:
        case ZET76_DEVICE_CONTROLLED:
            /* Node is busy because of us */
            status = ZDCP_REPORT_READY;
            break;
        default:
            /* Node is not connectable */
            //ZET76_EXIT(ZET76_OBJECT_NOT_FOUND);
            break;
        }
    }

    zet76_event_add(node->fade_event, ZET76_TIMEOUT_NODE_FADE);

    if (status == ZDCP_REPORT_BUSY) {
        zet76_node_stop_control(node);
        zet76_node_stop_connect(node);
        zet76_node_stop_accept(node);
        zet76_node_set_state(node, ZET76_DEVICE_BUSY);
    } else if (status == ZDCP_REPORT_CONFLICT) {
        zet76_node_stop_control(node);
        zet76_node_stop_connect(node);
        zet76_node_stop_accept(node);
        zet76_node_set_state(node, ZET76_DEVICE_CONFLICTED);
    } else if (node->state == ZET76_DEVICE_BUSY ||
               node->state == ZET76_DEVICE_CONFLICTED)
    {
        zet76_node_set_state(node, ZET76_DEVICE_IDLE);
    }

    if (baud_rate == 0 && parity == ZET76_PARITY_UNKNOWN) {
        baud_rate = node->baud_rate;
        parity    = node->parity;
    }

    if (zet76_sockaddr_compare(addr, &node->connect_addr) != 0 ||
        baud_rate != node->baud_rate ||
        parity != node->parity)
    {
        switch (node->state) {
        case ZET76_DEVICE_CONNECTING:
        case ZET76_DEVICE_CONNECTED:
            zet76_node_stop_connect(node);
            break;
        }
    }

    if (!(node->flags & ZET76_DEVICE_CONNECTABLE))
        ZET76_EXIT(ZET76_OK);

    if (!(node->flags & ZET76_DEVICE_CONTROLLABLE) &&
        node->id.type == ZET76_DEVICE_TYPE_7176)
    {
        ZET76_EXIT(ZET76_OK);
    }

    if (zet76_sockaddr_compare(addr, &node->connect_addr) != 0)
        ZET76_EXIT(ZET76_OK);

    if (baud_rate != node->baud_rate || parity != node->parity)
        ZET76_EXIT(ZET76_OK);

    if (peer_addr != NULL) {
        if (node->peer_addr.s_addr != peer_addr->s_addr ||
            !node->peer_addr_avail)
        {
            ZET76_EXIT(ZET76_OK);
        }
    }

    if (label && label_len > 0) {
        if (label_len > ZET76_MAX_LABEL_LEN)
            label_len = ZET76_MAX_LABEL_LEN;
        if (node->label_len != label_len ||
            memcmp(node->label, label, label_len))
        {
            ZET76_EXIT(ZET76_OK);
        }
    } else {
        label_len = -1;
    }

    /* Nothing to do */
    ZET76_EXIT(ZET76_OBJECT_NOT_FOUND);

cleanup:
    if (error == ZET76_OK) {
        node->flags |= ZET76_DEVICE_CONNECTABLE;
        if (node->id.type == ZET76_DEVICE_TYPE_7176)
            node->flags |= ZET76_DEVICE_CONTROLLABLE;
        else
            node->flags &= (~ZET76_DEVICE_CONTROLLABLE);

        node->connect_addr = *addr;

        node->control_addr = node->connect_addr;
        node->control_addr.sin_port  = ntohs(node->control_addr.sin_port);
        node->control_addr.sin_port += 4;
        node->control_addr.sin_port  = htons(node->control_addr.sin_port);

        node->baud_rate = baud_rate;
        node->parity    = parity;

        if (peer_addr != NULL) {
            node->peer_addr = *peer_addr;
            node->peer_addr_avail = 1;
        }

        if (label_len > 0) {
            node->label_len = label_len;
            memcpy(node->label, label, node->label_len);
        }

        ZET76_TRACE("device <%s #%016llx> <%s:%u> became connectable",
                    zet76_device_type_str(device->type),
                    device->serial,
                    inet_ntoa(addr->sin_addr),
                    ntohs(addr->sin_port));
        zet76_node_notify_status(node);
    }
    zet76_node_handle_state(node);

    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_set_state(struct zet76_node *node,
                                 int state)
{
    struct zet76_server *server = node->server;
    int r;

    ZET76_GUARD_LOCK(server->guard);
    if (node->state != state) {
        node->state  = state;

        r = 0;
        switch (state) {
        case ZET76_DEVICE_CONTROLLED:
            bufferevent_enable(node->control_bev, EV_READ | EV_WRITE);
            break;
        case ZET76_DEVICE_CONNECTED:
            bufferevent_enable(node->connect_cmd, EV_READ | EV_WRITE);
            bufferevent_enable(node->connect_dat, EV_READ);
            break;
        case ZET76_DEVICE_ACCEPTED:
            if (node->accept_cmd)
                bufferevent_enable(node->accept_cmd, EV_READ | EV_WRITE);
            bufferevent_enable(node->accept_dat, EV_READ);
            break;
        default:
            r = -1;
            break;
        }

        zet76_node_notify_status(node);
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_free(struct zet76_node *node)
{
    struct zet76_server *server = node->server;

    ZET76_TRACE("<%u:0x%I64x>", node->id.type, node->id.serial);
    ZET76_GUARD_LOCK(server->guard);

    zet76_node_notify_status(node);

    ZET76_LIST_REMOVE(node, entry);

    ZET76_FREE(node->accept_dat,     bufferevent_free);
    ZET76_FREE(node->accept_cmd,     bufferevent_free);
    ZET76_FREE(node->control_bev,    bufferevent_free);
    ZET76_FREE(node->connect_dat,    bufferevent_free);
    ZET76_FREE(node->connect_cmd,    bufferevent_free);
    ZET76_FREE(node->fade_event,     event_free);

    node->magic = ZET76_MAGIC_NULL;

    ZET76_GUARD_UNLOCK(server->guard);
    free(node);
}

/****************************************************************************/
static void zet76_node_notify_status(struct zet76_node *node)
{
    struct zet76_server *server = node->server;

    ZET76_VALIDATE_OBJECT(node, ZET76_MAGIC_NODE);

    ZET76_GUARD_LOCK(server->guard);
    if (server->status_callback != NULL && server->thread_handle != NULL) {
        server->status_callback(server,
                                &node->id,
                                node->state,
                                node->flags,
                                server->status_callback_arg);
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_notify_recv(struct zet76_node *node, int recv_flags)
{
    struct zet76_server *server = node->server;

    if (GetCurrentThreadId() != server->thread_id)
        return;

    ZET76_GUARD_LOCK(server->guard);
    if (server->recv_callback != NULL) {
        server->recv_callback(server,
                              &node->id,
                              recv_flags,
                              server->recv_callback_arg);
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
/* Fade timeout - means node is lost */
static void zet76_node_on_fade(evutil_socket_t sock, short which, void *arg)
{
    struct zet76_node *node = (struct zet76_node *) arg;
    struct zet76_server *server;

    ZET76_UNUSED_PARAM(sock);
    ZET76_VALIDATE_OBJECT(node, ZET76_MAGIC_NODE);

    server = node->server;
    ZET76_VALIDATE_OBJECT(server, ZET76_MAGIC_SERVER);


    ZET76_TRACE("");
    ZET76_GUARD_LOCK(server->guard);
    node->flags &= (~ZET76_DEVICE_CONNECTABLE);
    node->flags &= (~ZET76_DEVICE_CONTROLLABLE);

    if (node->state == ZET76_DEVICE_BUSY ||
        node->state == ZET76_DEVICE_CONFLICTED)
    {
        zet76_node_set_state(node, ZET76_DEVICE_IDLE);
    }

    zet76_node_notify_status(node);
    zet76_node_handle_state(node);

    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
ZET76_EXPORT zet76_enable(struct zet76_server *server,
                          const struct zet76_device *device,
                          int device_modes)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    ZET76_TRACE("<%s #%016llx>",
                zet76_device_type_str(device->type),
                device->serial);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_create(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_NOT_ENOUGH_MEMORY);

    node->modes |= device_modes;
    zet76_node_handle_state(node);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_disable(struct zet76_server *server,
                           const struct zet76_device *device,
                           int device_modes)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    ZET76_TRACE("<%s #%016llx>",
                zet76_device_type_str(device->type),
                device->serial);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_create(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_NOT_ENOUGH_MEMORY);

    node->modes &= ~device_modes;

    if (node->disable_event == NULL) {
        node->disable_event = event_new(server->base,
                                        -1,
                                        EV_TIMEOUT,
                                        zet76_node_on_disable,
                                        node);
        ZET76_EXIT_IF_NULL(node->disable_event, ZET76_NOT_ENOUGH_MEMORY);

        zet76_event_add(node->disable_event, 0);
    }

cleanup:
    if (error != ZET76_OK) {
        ZET76_FREE(node->disable_event, event_free);
    }

    ZET76_GUARD_UNLOCK(server->guard);
    return error;
}

/****************************************************************************/
#if 0
ZET76_EXPORT zet76_add(struct zet76_server *server,
                       const struct zet76_device *device,
                       const char *ip,
                       unsigned short port,
                       int fade_timeout_ms)
{
    struct zet76_node *node = NULL;
    struct sockaddr_in addr;
    int r;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);
    ZET76_VALIDATE_PARAM_PTR(ip);
    ZET76_VALIDATE_PARAM(port > 0 && port <= ZET76_MAX_PORT);

    r = evutil_inet_pton(AF_INET, ip, &addr.sin_addr);
    ZET76_VALIDATE_PARAM(r != 0);

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);

    ZET76_TRACE("<%s #%016llx>",
                zet76_device_type_str(device->type),
                device->serial);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_create(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_NOT_ENOUGH_MEMORY);

    /* Restart fade timer or turn it off */
    if (fade_timeout_ms > 0 && fade_timeout_ms < ZET76_TIMEOUT_INFINITE) {
        zet76_event_add(node->fade_event, ZET76_TIMEOUT_NODE_FADE);
    } else {
        event_del(node->fade_event);
    }

    if (node->flags & ZET76_DEVICE_CONTROLLABLE) {
        node->flags &= (~ZET76_DEVICE_CONTROLLABLE);
    }

    if (!(node->flags & ZET76_DEVICE_CONNECTABLE) ||
        zet76_sockaddr_compare(&addr, &node->connect_addr) != 0)
    {
        node->flags |= ZET76_DEVICE_CONNECTABLE;
        node->connect_addr = addr;

        ZET76_TRACE("device <%s #%016llx> <%s:%u> added",
                    zet76_device_type_str(device->type),
                    device->serial,
                    inet_ntoa(addr.sin_addr),
                    ntohs(addr.sin_port));

        zet76_node_notify_status(node);
        zet76_node_handle_state(node);

        ZET76_EXIT(ZET76_OK);
    }

    /* Nothing to do */
    ZET76_EXIT(ZET76_OK);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_remove(struct zet76_server *server,
                          const struct zet76_device *device)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    ZET76_TRACE("<%s #%016llx>",
                zet76_device_type_str(device->type),
                device->serial);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_open(server, device);
    if (node == NULL) {
        ZET76_TRACE("device doesn' exist");
        ZET76_EXIT(ZET76_OBJECT_NOT_FOUND);
    }

    node->flags &= (~ZET76_DEVICE_CONNECTABLE);
    node->flags &= (~ZET76_DEVICE_CONTROLLABLE);

    if (node->state == ZET76_DEVICE_BUSY ||
        node->state == ZET76_DEVICE_CONFLICTED)
    {
        zet76_node_set_state(node, ZET76_DEVICE_IDLE);
    }

    zet76_node_notify_status(node);
    zet76_node_handle_state(node);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
    return error;
}
#endif

/****************************************************************************/
/* Control response receive timeout - means no reponse */
static void zet76_node_on_response(evutil_socket_t sock,
                                   short which, void *arg)
{
    struct zet76_node *node = (struct zet76_node *) arg;
    struct zet76_server *server;

    ZET76_UNUSED_PARAM(sock);
    ZET76_VALIDATE_OBJECT(node, ZET76_MAGIC_NODE);

    server = node->server;
    ZET76_VALIDATE_OBJECT(server, ZET76_MAGIC_SERVER);

    ZET76_GUARD_LOCK(server->guard);
    switch (node->state) {
    case ZET76_DEVICE_CONTROLLED:
        if (node->control_send == ZET76_CONTROL_SEND_WAIT) {
            ZET76_TRACE("response timeout");
            zet76_node_control_reset(node);
            zet76_node_notify_recv(node,
                                   ZET76_RECV_CONTROL | ZET76_RECV_TIMEOUT);
        }
        break;
    case ZET76_DEVICE_CONNECTED:
    case ZET76_DEVICE_ACCEPTED:
        zet76_node_notify_recv(node,
                               ZET76_RECV_RESPONSE | ZET76_RECV_TIMEOUT);
        break;
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_on_read(struct bufferevent *bev, void *arg)
{
    char buffer[ZET76_SIZE_SCANNER_HOST];
    struct zet76_device report_id;

    struct zet76_node *node = (struct zet76_node *) arg;
    struct zet76_server *server;
    int r = 0;

    ZET76_VALIDATE_OBJECT(node, ZET76_MAGIC_NODE);

    server = node->server;
    ZET76_GUARD_LOCK(server->guard);
    if (node->state == ZET76_DEVICE_CONNECTED) {
        if (bev == node->connect_cmd) {
            r = ZET76_RECV_RESPONSE;
        }
        if (bev == node->connect_dat) {
            r = ZET76_RECV_CHAIN;
        }
    }
    if (node->state == ZET76_DEVICE_ACCEPTED) {
        if (bev == node->accept_cmd) {
            r = ZET76_RECV_RESPONSE;
        }
        if (bev == node->accept_dat) {
            r = ZET76_RECV_CHAIN;
        }
    }

    if (r == ZET76_RECV_CHAIN && (node->modes & ZET76_DEVICE_SKIP_CHAIN)) {
        struct evbuffer *input = bufferevent_get_input(bev);
        if (input) {
            r = evbuffer_get_length(input) / ZET76_SIZE_CHAIN_PACKET;
            if (r > 0)
                evbuffer_drain(input, r * ZET76_SIZE_CHAIN_PACKET);
        }
    } else if (r != 0) {
        zet76_node_notify_recv(node, r);
    } else if (bev == node->connect_cmd &&
               node->id.type != ZET76_DEVICE_TYPE_7076)
    {
        /* Device may send ZDCP_REPORT via connect_cmd */
        r = bufferevent_read(bev, buffer, ZET76_SIZE_SCANNER_HOST);
        r = zet76_zdcp_report_parse((const ZDCP_REPORT *) buffer,
                                    r,
                                    &report_id,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL);
        if (r == ZET76_OK) {
            ZET76_TRACE("device report <%s #%016llx>",
                        zet76_device_type_str(report_id.type),
                        report_id.serial);

            if (zet76_device_compare(&report_id, &node->id) == 0) {
                zet76_bufferevent_set_timeouts(node->connect_cmd,
                                               ZET76_TIMEOUT_NODE_RESPONSE,
                                               ZET76_TIMEOUT_NODE_SEND);
                node->connect_flags |= ZET76_CONNECT_FLAG_REPORTED;
                if (node->connect_flags & ZET76_CONNECT_FLAG_ESTABLISHED)
                    zet76_node_set_state(node, ZET76_DEVICE_CONNECTED);
            } else {
                zet76_node_stop_connect(node);
            }
        }
    }
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_on_control(struct bufferevent *bev, void *arg)
{
    ZDCP_COMMON_PACKET zdcp;
    struct zet76_device response_id;

    struct zet76_node *node = (struct zet76_node *) arg;
    struct zet76_server *server;
    struct evbuffer *input;
    int r = 0;
    int error = ZET76_OK;

    ZET76_VALIDATE_OBJECT(node, ZET76_MAGIC_NODE);

    server = node->server;
    ZET76_VALIDATE_OBJECT(server, ZET76_MAGIC_SERVER);

    ZET76_GUARD_LOCK(server->guard);

    ZET76_EXIT_IF_FALSE(node->state == ZET76_DEVICE_CONTROLLED,
                        ZET76_INVALID_STATE);

    input = bufferevent_get_input(bev);
    ZET76_EXIT_IF_NULL(input, ZET76_ERROR);

    while (evbuffer_get_length(input) > 0) {
        switch (node->control_recv) {
        case ZET76_CONTROL_RECV_HEADER:
            /* Try read header */
            r = evbuffer_copyout(input, &zdcp.header, sizeof(ZDCP_HEADER));
            ZET76_EXIT_IF_FALSE(r == sizeof(ZDCP_HEADER), ZET76_ERROR);

            /* Parse header */
            zdcp.header.signature = ntohl(zdcp.header.signature);
            zdcp.header.id        = ntohs(zdcp.header.id);
            zdcp.header.length    = ntohs(zdcp.header.length);

            if (zdcp.header.signature != ZDCP_SIGNATURE) {
                ZET76_TRACE("not a ZDCP packet - skip 1 byte");
                evbuffer_drain(input, 1);
                break;
            }

            evbuffer_drain(input, sizeof(ZDCP_HEADER));
            node->control_recv_size = zdcp.header.length;

            if (node->control_send != ZET76_CONTROL_SEND_WAIT ||
                zdcp.header.id != ZDCP_ID_RESPONSE)
            {
                ZET76_TRACE("skip not waited packet id %u length %u",
                            zdcp.header.id,
                            zdcp.header.length);
                node->control_recv = ZET76_CONTROL_RECV_SKIP;
                break;
            }

            node->control_recv = ZET76_CONTROL_RECV_PACKET;
            //break; -- FALLTHROUGH

        case ZET76_CONTROL_RECV_PACKET:
            /* Try to read packet after header */
            r = evbuffer_copyout(input,
                                 &zdcp.response.serial_hi,
                                 ZDCP_LENGTH_RESPONSE);
            ZET76_EXIT_IF_FALSE(r == ZDCP_LENGTH_RESPONSE, ZET76_ERROR);

            zdcp.response.serial_hi = ntohl(zdcp.response.serial_hi);
            zdcp.response.serial_lo = ntohl(zdcp.response.serial_lo);
            zdcp.response.type      = ntohl(zdcp.response.type);
            zdcp.response.sequence  = ntohl(zdcp.response.sequence);
            zdcp.response.command   = ntohl(zdcp.response.command);
            zdcp.response.response  = ntohl(zdcp.response.response);

            evbuffer_drain(input, ZDCP_LENGTH_RESPONSE);
            node->control_recv_size -= ZDCP_LENGTH_RESPONSE;

            response_id.type     = zdcp.response.type;
            response_id.serial   = zdcp.response.serial_hi;
            response_id.serial <<= 32;
            response_id.serial  += zdcp.response.serial_lo;

            if (node->control_send != ZET76_CONTROL_SEND_WAIT ||
                zet76_device_compare(&node->id, &response_id) != 0 ||
                zdcp.response.sequence != node->control_zdcp_seq ||
                zdcp.response.command  != node->control_zdcp_cmd)
            {
                ZET76_TRACE("skip not matched response from <%u:0x%I64X>"
                            "with sequence %u command %u",
                            response_id.type,
                            response_id.serial,
                            zdcp.response.sequence,
                            zdcp.response.command);
                node->control_recv = ZET76_CONTROL_RECV_SKIP;
                break;
            }

            node->control_zdcp_res = zdcp.response.response;
            node->control_recv = ZET76_CONTROL_RECV_DATA;
            //break; -- FALLTHROUGH

        case ZET76_CONTROL_RECV_DATA:
            if (node->control_recv_size > 0) {
                r = evbuffer_remove_buffer(input,
                                           node->control_buf,
                                           node->control_recv_size);
                ZET76_EXIT_IF_FALSE(r > 0, ZET76_ERROR);
                node->control_recv_size -= r;
            }
            if (node->control_recv_size == 0) {
                node->control_recv = ZET76_CONTROL_RECV_HEADER;

                if (node->control_send == ZET76_CONTROL_SEND_WAIT) {
                    ZET76_TRACE("response result %u length %d",
                                node->control_zdcp_res,
                                evbuffer_get_length(node->control_buf));

                    r = ZET76_RECV_CONTROL;
                    if (node->control_zdcp_res != ZDCP_RESPONSE_OK)
                        r |= ZET76_RECV_ERROR;

                    node->control_send = ZET76_CONTROL_SEND_DONE;

                    zet76_node_notify_recv(node, r);
                }
            }
            break;


        case ZET76_CONTROL_RECV_SKIP:
            r = evbuffer_drain(input, node->control_recv_size);
            ZET76_EXIT_IF_FALSE(r > 0, ZET76_ERROR);

            node->control_recv_size -= r;
            if (node->control_recv_size == 0)
                node->control_recv = ZET76_CONTROL_RECV_HEADER;
            break;

        default:
            node->control_recv = ZET76_CONTROL_RECV_HEADER;
            break;
        }
    }

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_on_event(struct bufferevent *bev,
                                short which, void *arg)
{
    struct sockaddr_in dat_addr;

    struct zet76_node *node = (struct zet76_node *) arg;
    struct zet76_server *server = node->server;
    int r = 0;

    if (bev != node->connect_dat &&
        bev != node->control_bev &&
        bev != node->connect_cmd &&
        bev != node->accept_dat)
    {
        ZET76_TRACE("%s which %X:%s%s%s%s%s%s",
                    bev == node->control_bev ? "control_bev" :
                    bev == node->connect_cmd ? "connect_cmd" :
                    bev == node->connect_dat ? "connect_dat" :
                    bev == node->accept_cmd ? "accept_cmd" :
                    bev == node->accept_dat ? "accept_dat" :
                    "UNKNOWN",
                    which,
                    (which & BEV_EVENT_READING)   ? " reading"   : "",
                    (which & BEV_EVENT_WRITING)   ? " writing"   : "",
                    (which & BEV_EVENT_EOF)       ? " eof"       : "",
                    (which & BEV_EVENT_ERROR)     ? " error"     : "",
                    (which & BEV_EVENT_TIMEOUT)   ? " timeout"   : "",
                    (which & BEV_EVENT_CONNECTED) ? " connected" : "");
    }

    ZET76_GUARD_LOCK(server->guard);

    if (which & (BEV_EVENT_ERROR | BEV_EVENT_EOF)) {
        int error = EVUTIL_SOCKET_ERROR();
        if (bev == node->control_bev)
            zet76_node_stop_control(node);
        else if (bev == node->connect_cmd || bev == node->connect_dat)
            zet76_node_stop_connect(node);
        else if (bev == node->accept_cmd || bev == node->accept_dat)
            zet76_node_stop_accept(node);
    }

    if (which & BEV_EVENT_CONNECTED) {
        if (bev == node->control_bev) {
            ZET76_FREE(node->control_buf, evbuffer_free);

            r = 0;

            node->control_buf = evbuffer_new();
            if (node->control_buf == NULL)
                r = -1;

            if (r == 0) {
                node->control_recv_size = 0;
                node->control_zdcp_cmd  = ZDCP_COMMAND_PING;

                zet76_node_set_state(node, ZET76_DEVICE_CONTROLLED);
            } else {
                zet76_node_stop_control(node);
            }
        }
        else if (bev == node->connect_cmd) {
#ifdef _WIN32
            struct tcp_keepalive keep;
            DWORD bytes;
#endif
            evutil_socket_t sock;
            int keep_alive = 1;

            sock = bufferevent_getfd(node->connect_cmd);
            setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,
                       (const char *)&keep_alive, sizeof(keep_alive));

#ifdef _WIN32
            keep.onoff = keep_alive;
            keep.keepalivetime = ZET76_TIMEOUT_NODE_DATA;
            keep.keepaliveinterval = 1000;
            bytes = 0;
            WSAIoctl(sock, SIO_KEEPALIVE_VALS,
                     &keep, sizeof(keep), NULL, 0,
                     &bytes, NULL, NULL);
#endif

            dat_addr = node->connect_addr;
            dat_addr.sin_port = htons(ntohs(dat_addr.sin_port) + 1);

            bufferevent_enable(node->connect_cmd, EV_READ | EV_WRITE);

            node->connect_dat = bufferevent_socket_new(
                    node->server->base,
                    -1,
                    ZET76_BEV_OPTIONS);
            //ZET76_EXIT_IF_NULL(node->connect_dat,
            //                   ZET76_NOT_ENOUGH_MEMORY);

            bufferevent_setcb(node->connect_dat,
                              zet76_node_on_read,
                              NULL,
                              zet76_node_on_event,
                              node);

            zet76_bufferevent_set_timeouts(node->connect_dat,
                                           ZET76_TIMEOUT_NODE_DATA,
                                           ZET76_TIMEOUT_NODE_SEND);
            if (node->id.type != ZET76_DEVICE_TYPE_7076) {
                ZET76_TRACE("!!!!!!!!!!!!!!!!!!! connect_dat begin");
                bufferevent_socket_connect(node->connect_dat,
                                           (struct sockaddr *) &dat_addr,
                                           sizeof(dat_addr));
                ZET76_TRACE("!!!!!!!!!!!!!!!!!!! connect_dat end");
            } else {
                zet76_snmp_on_connect(node->server, &node->id);
                node->connect_flags |= ZET76_CONNECT_FLAG_ESTABLISHED;
                zet76_node_set_state(node, ZET76_DEVICE_CONNECTED);
            }
        }
        else if (bev == node->connect_dat) {
            if (node->id.type != ZET76_DEVICE_TYPE_7076) {
                node->connect_flags |= ZET76_CONNECT_FLAG_ESTABLISHED;
                if (node->connect_flags & ZET76_CONNECT_FLAG_REPORTED)
                    zet76_node_set_state(node, ZET76_DEVICE_CONNECTED);
            }
        }
    }

    if (which & BEV_EVENT_TIMEOUT) {
        if (which & BEV_EVENT_READING) {
            bufferevent_enable(bev, EV_READ);

            r = ZET76_RECV_TIMEOUT;
            if (node->state == ZET76_DEVICE_CONNECTED) {
                if (bev == node->connect_cmd)
                    zet76_node_notify_recv(node, r | ZET76_RECV_RESPONSE);
                if (bev == node->connect_dat)
                    zet76_node_notify_recv(node, r | ZET76_RECV_CHAIN);
            }
            else if (node->state == ZET76_DEVICE_ACCEPTED) {
                if (bev == node->accept_cmd)
                    zet76_node_notify_recv(node, r | ZET76_RECV_RESPONSE);
                if (bev == node->accept_dat)
                    zet76_node_stop_accept(node);
//                    zet76_node_notify_recv(node, r | ZET76_RECV_CHAIN);
            }
            else if (node->state == ZET76_DEVICE_CONNECTING) {
                if (node->id.type != ZET76_DEVICE_TYPE_7076 &&
                    bev == node->connect_cmd)
                {
                    zet76_bufferevent_set_timeouts(
                            node->connect_cmd,
                            ZET76_TIMEOUT_NODE_RESPONSE,
                            ZET76_TIMEOUT_NODE_SEND);
                    node->connect_flags |= ZET76_CONNECT_FLAG_REPORTED;
                    if (node->connect_flags & ZET76_CONNECT_FLAG_ESTABLISHED)
                        zet76_node_set_state(node, ZET76_DEVICE_CONNECTED);
                }
            }
        }

        if (which & BEV_EVENT_WRITING) {
            bufferevent_enable(bev, EV_WRITE);
        }
    }

    zet76_node_handle_state(node);

    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_control_reset(struct zet76_node *node)
{
    int size;

    node->control_send     = ZET76_CONTROL_SEND_IDLE;
    node->control_zdcp_cmd = ZDCP_COMMAND_PING;
    node->control_zdcp_res = ZDCP_RESPONSE_OK;

    size = evbuffer_get_length(node->control_buf);
    evbuffer_drain(node->control_buf, size);

    if (node->control_recv == ZET76_CONTROL_RECV_DATA)
        node->control_recv = ZET76_CONTROL_RECV_SKIP;
}

/****************************************************************************/
static void zet76_node_handle_state(struct zet76_node *node)
{
    struct zet76_server *server = node->server;
    int error = ZET76_OK;

    ZET76_GUARD_LOCK(server->guard);

    if (node->state != ZET76_DEVICE_IDLE) {
        if (node->modes & ZET76_DEVICE_SKIP_CHAIN) {
            struct evbuffer *input = NULL;
            switch (node->state) {
            case ZET76_DEVICE_CONNECTED:
                input = bufferevent_get_input(node->connect_dat);
                break;
            case ZET76_DEVICE_ACCEPTED:
                input = bufferevent_get_input(node->accept_dat);
                break;
            }
            if (input) {
                int r = evbuffer_get_length(input) / ZET76_SIZE_CHAIN_PACKET;
                if (r > 0)
                    evbuffer_drain(input, r * ZET76_SIZE_CHAIN_PACKET);
            }
        }
        ZET76_EXIT(ZET76_OK);
    }

    /* Try to control */
    if ((node->flags & ZET76_DEVICE_CONTROLLABLE) &&
        (node->modes & ZET76_DEVICE_CONTROL))
    {
        if (node->control_bev != NULL) {
            ZET76_TRACE("CONTROL_BEV IS NOT NULL!!!!!");
            ZET76_EXIT(ZET76_OK);
        }
        if (node->control_buf != NULL) {
            ZET76_TRACE("CONTROL_BUF IS NOT NULL!!!!!");
            ZET76_EXIT(ZET76_OK);
        }
        node->state = ZET76_DEVICE_CONTROLLING;

        ZET76_TRACE("controlling  <%s:%u>",
                    inet_ntoa(node->control_addr.sin_addr),
                    ntohs(node->control_addr.sin_port));

        node->control_bev = bufferevent_socket_new(node->server->base,
                                                   -1,
                                                   ZET76_BEV_OPTIONS);
        ZET76_EXIT_IF_NULL(node->control_bev, ZET76_NOT_ENOUGH_MEMORY);

        ZET76_TRACE("created control_bev %X", node->control_bev);
        bufferevent_setcb(node->control_bev,
                          zet76_node_on_control,
                          NULL,
                          zet76_node_on_event,
                          node);

        zet76_bufferevent_set_timeouts(node->control_bev,
                                       ZET76_TIMEOUT_NODE_DATA,
                                       ZET76_TIMEOUT_NODE_SEND);

        bufferevent_socket_connect(node->control_bev,
                                   (struct sockaddr *) &node->control_addr,
                                   sizeof(node->control_addr));
        ZET76_EXIT(ZET76_OK);
    }

    /* Try to connect */
    if ((node->flags & ZET76_DEVICE_CONNECTABLE) &&
        (node->modes & ZET76_DEVICE_CONNECT))
    {
        if (node->connect_cmd != NULL) {
            ZET76_TRACE("CONNECT_CMD IS NOT NULL!!!!!");
            ZET76_EXIT(ZET76_OK);
        }
        node->state = ZET76_DEVICE_CONNECTING;

        ZET76_TRACE("connecting to <%s:%u>",
                    inet_ntoa(node->connect_addr.sin_addr),
                    ntohs(node->connect_addr.sin_port));

        node->connect_flags = 0;
        node->connect_cmd = bufferevent_socket_new(node->server->base,
                                                   -1,
                                                   ZET76_BEV_OPTIONS);
        ZET76_EXIT_IF_NULL(node->connect_cmd, ZET76_NOT_ENOUGH_MEMORY);

        ZET76_TRACE("created connect_cmd %X", node->connect_cmd);
        bufferevent_setcb(node->connect_cmd,
                          zet76_node_on_read,
                          NULL,
                          zet76_node_on_event,
                          node);
        if (node->id.type == ZET76_DEVICE_TYPE_7076) {
            zet76_bufferevent_set_timeouts(node->connect_cmd,
                                           ZET76_TIMEOUT_NODE_7076,
                                           ZET76_TIMEOUT_NODE_SEND);
        } else {
            zet76_bufferevent_set_timeouts(node->connect_cmd,
                                           ZET76_TIMEOUT_NODE_REPORT,
                                           ZET76_TIMEOUT_NODE_SEND);
        }

        bufferevent_socket_connect(node->connect_cmd,
                                   (struct sockaddr *) &node->connect_addr,
                                   sizeof(node->connect_addr));
        ZET76_EXIT(ZET76_OK);
    }

    /* At least, try to accept */
    if ((node->flags & ZET76_DEVICE_ACCEPTABLE) &&
        (node->modes & ZET76_DEVICE_ACCEPT))
    {
        zet76_node_set_state(node, ZET76_DEVICE_ACCEPTED);
        ZET76_EXIT(ZET76_OK);
    }

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_stop_connect(struct zet76_node *node)
{
    struct zet76_server *server = node->server;

    ZET76_GUARD_LOCK(server->guard);
    //ZET76_TRACE("state %s", zet76_device_state_str(node->state));

    switch (node->state) {
    case ZET76_DEVICE_CONNECTED:
    case ZET76_DEVICE_CONNECTING:
        ZET76_FREE(node->connect_dat, bufferevent_free);
        ZET76_FREE(node->connect_cmd, bufferevent_free);
        node->connect_flags = 0;
        zet76_node_set_state(node, ZET76_DEVICE_IDLE);
        break;
    }

    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_stop_control(struct zet76_node *node)
{
    struct zet76_server *server = node->server;

    ZET76_GUARD_LOCK(server->guard);
    //ZET76_TRACE("state %s", zet76_device_state_str(node->state));

    switch (node->state) {
    case ZET76_DEVICE_CONTROLLED:
    case ZET76_DEVICE_CONTROLLING:
        ZET76_FREE(node->control_buf, evbuffer_free);
        ZET76_FREE(node->control_bev, bufferevent_free);
        zet76_node_set_state(node, ZET76_DEVICE_IDLE);
        break;
    }

    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_stop_accept(struct zet76_node *node)
{
    struct zet76_server *server = node->server;

    ZET76_GUARD_LOCK(server->guard);
    //ZET76_TRACE("state %s", zet76_device_state_str(node->state));

    ZET76_FREE(node->accept_dat, bufferevent_free);
    ZET76_FREE(node->accept_cmd, bufferevent_free);

    /* Reset ACCEPTABLE flag because of closed sockets */
    node->flags &= ~ZET76_DEVICE_ACCEPTABLE;

    switch (node->state) {
    case ZET76_DEVICE_ACCEPTED:
        zet76_node_set_state(node, ZET76_DEVICE_IDLE);
        break;
    default:
        zet76_node_notify_status(node);
    }

    ZET76_GUARD_UNLOCK(server->guard);
}

/****************************************************************************/
static void zet76_node_on_disable(evutil_socket_t sock,
                                  short which, void *arg)
{
    struct zet76_node *node = (struct zet76_node *) arg;

    ZET76_UNUSED_PARAM(sock);
    ZET76_UNUSED_PARAM(which);

    ZET76_VALIDATE_OBJECT(node, ZET76_MAGIC_NODE);

    ZET76_GUARD_LOCK(node->server->guard);
    ZET76_FREE(node->disable_event, event_free);

    switch (node->state) {
    case ZET76_DEVICE_CONTROLLED:
    case ZET76_DEVICE_CONTROLLING:
        if (!(node->modes & ZET76_DEVICE_CONTROL)) {
            zet76_node_stop_control(node);
        }
        break;
    case ZET76_DEVICE_CONNECTED:
    case ZET76_DEVICE_CONNECTING:
        if (!(node->modes & ZET76_DEVICE_CONNECT)) {
            zet76_node_stop_connect(node);
        }
        break;
    case ZET76_DEVICE_ACCEPTED:
        if (!(node->modes & ZET76_DEVICE_ACCEPT)) {
            zet76_node_stop_accept(node);
        }
        break;
    }

    zet76_node_handle_state(node);
    ZET76_GUARD_UNLOCK(node->server->guard);
}

/****************************************************************************/
ZET76_EXPORT zet76_get_address(struct zet76_server *server,
                               const struct zet76_device *device,
                               int mode,
                               char *ip,
                               int ip_size,
                               unsigned short *port)
{
    struct sockaddr_in address;
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    if (mode & ZET76_DEVICE_CONTROL) {
        if (node->flags & ZET76_DEVICE_CONTROLLABLE) {
            address = node->control_addr;
            ZET76_EXIT(ZET76_OK);
        }
        ZET76_EXIT(ZET76_INVALID_MODE);
    }

    if (mode & ZET76_DEVICE_CONNECT) {
        if (node->flags & ZET76_DEVICE_CONNECTABLE) {
            address = node->connect_addr;
            ZET76_EXIT(ZET76_OK);
        }
        ZET76_EXIT(ZET76_INVALID_MODE);
    }

    if (mode & ZET76_DEVICE_ACCEPT) {
        if (node->flags & ZET76_DEVICE_ACCEPTABLE) {
            address = node->accept_addr;
            ZET76_EXIT(ZET76_OK);
        }
        ZET76_EXIT(ZET76_INVALID_MODE);
    }

    ZET76_EXIT(ZET76_INVALID_MODE);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    if (error == ZET76_OK) {
        if (ip != NULL)
            evutil_inet_ntop(AF_INET, &address.sin_addr, ip, ip_size);
        if (port != NULL)
            *port = ntohs(address.sin_port);
    } else {
        if (ip != NULL)
            *ip = '\0';
        if (port != NULL)
            *port = 0;
    }

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_get_info(struct zet76_server *server,
                            const struct zet76_device *device,
                            int *state,
                            int *flags,
                            int *modes)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    if (state != NULL)
        *state = node->state;
    if (flags != NULL)
        *flags = node->flags;
    if (modes != NULL)
        *modes = node->modes;

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    if (error != ZET76_OK) {
        if (state != NULL)
            *state = ZET76_DEVICE_IDLE;
        if (flags != NULL)
            *flags = 0;
        if (modes != NULL)
            *modes = 0;
    }

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_get_stats(struct zet76_server *server,
                             const struct zet76_device *device,
                             unsigned long long *rx_bytes)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    if (rx_bytes != NULL)
        *rx_bytes = node->rx_bytes;

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    if (error != ZET76_OK) {
        if (rx_bytes != NULL)
            *rx_bytes = 0;
    }

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_get_rs485(struct zet76_server *server,
                             const struct zet76_device *device,
                             int *baud_rate,
                             int *parity)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    if (baud_rate != NULL)
        *baud_rate = node->baud_rate;
    if (parity != NULL)
        *parity = node->parity;

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    if (error != ZET76_OK) {
        if (baud_rate != NULL)
            *baud_rate = 0;
        if (parity != NULL)
            *parity = 0;
    }

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_get_peer(struct zet76_server *server,
                            const struct zet76_device *device,
                            char *peer_ip,
                            int peer_ip_size)
{
    struct zet76_node *node = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);
    ZET76_VALIDATE_PARAM(peer_ip && peer_ip_size > 1);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    ZET76_EXIT_IF_FALSE(node->peer_addr_avail, ZET76_ERROR); // TODO - another error code

    peer_ip[0] = '\0';

    evutil_inet_ntop(
            AF_INET,
            &node->peer_addr,
            peer_ip, peer_ip_size);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_get_label(struct zet76_server *server,
                             const struct zet76_device *device,
                             wchar_t *label,
                             int label_chars)
{
    struct zet76_node *node = NULL;
    int len = 0;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);
    ZET76_VALIDATE_PARAM(label && label_chars > 1);

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    ZET76_EXIT_IF_FALSE(node->label_len >= 0, ZET76_ERROR); // TODO - another error code

    if (node->label_len <= 0) {
        label[0] = L'\0';
    } else {
        len = MultiByteToWideChar(
                CP_UTF8,
                0,
                node->label,
                node->label_len,
                label,
                label_chars - 1);
        if (len <= 0) {
            label[0] = L'\0';
            ZET76_EXIT(ZET76_ERROR);
        }
        label[len] = L'\0';
    }

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);
    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_send(struct zet76_server *server,
                        const struct zet76_device *device,
                        int send_flags,
                        const void *data,
                        int data_size)
{
    struct zet76_node *node = NULL;
    struct bufferevent *bev = NULL;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);
    ZET76_VALIDATE_PARAM_PTR(data);
    ZET76_VALIDATE_PARAM(data_size > 0);

    if (send_flags & ZET76_SEND_DONTWAIT) {
        if (!ZET76_GUARD_TRYLOCK(server->guard))
            return ZET76_AGAIN;
    } else {
        ZET76_GUARD_LOCK(server->guard);
    }
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    switch (node->state) {
    case ZET76_DEVICE_CONNECTED:
        bev = node->connect_cmd;
        break;
    case ZET76_DEVICE_ACCEPTED:
        bev = node->accept_cmd;
        break;
    default:
        ZET76_EXIT(ZET76_INVALID_STATE);
    }

    if (bev == NULL) {
        ZET76_EXIT(ZET76_INVALID_STATE);
    }
    bufferevent_enable(bev, EV_READ); /* rearm read timeout */
    bufferevent_write(bev, data, data_size);

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    return error;
}

/****************************************************************************/
ZET76_EXPORT zet76_recv(struct zet76_server *server,
                        const struct zet76_device *device,
                        int recv_flags,
                        void *data,
                        int *data_size)
{
    struct zet76_node *node = NULL;
    struct bufferevent *bev = NULL;
    int r;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    if (recv_flags & ZET76_RECV_DONTWAIT) {
        if (!ZET76_GUARD_TRYLOCK(server->guard))
            return ZET76_AGAIN;
    } else {
        ZET76_GUARD_LOCK(server->guard);
    }
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    switch (node->state) {
    case ZET76_DEVICE_CONNECTED:
        if (recv_flags & ZET76_RECV_CHAIN)
            bev = node->connect_dat;
        else if (recv_flags & ZET76_RECV_RESPONSE)
            bev = node->connect_cmd;
        else
            ZET76_EXIT(ZET76_INVALID_PARAMETER);
        break;

    case ZET76_DEVICE_ACCEPTED:
        if (recv_flags & ZET76_RECV_CHAIN)
            bev = node->accept_dat;
        else if (recv_flags & ZET76_RECV_RESPONSE)
            bev = node->accept_cmd;
        else
            ZET76_EXIT(ZET76_INVALID_PARAMETER);
        break;

    case ZET76_DEVICE_CONTROLLED:
        ZET76_EXIT_IF_FALSE(recv_flags & ZET76_RECV_CONTROL,
                            ZET76_INVALID_STATE);
        ZET76_EXIT_IF_FALSE(node->control_send == ZET76_CONTROL_SEND_DONE,
                            ZET76_INVALID_STATE);

        if (data == NULL)
            r = evbuffer_get_length(node->control_buf);
        else if (data_size && *data_size > 0)
            r = evbuffer_remove(node->control_buf, data, *data_size);
        else
            r = 0;
        if (data_size)
            *data_size = r;

        if (evbuffer_get_length(node->control_buf) <= 0)
            node->control_send = ZET76_CONTROL_SEND_IDLE;

        ZET76_EXIT(ZET76_OK);

    default:
        ZET76_EXIT(ZET76_INVALID_STATE);
    }

    if (bev == NULL) {
        r = 0;
    } else if (recv_flags & ZET76_RECV_CLEAR) {
        /* Drain entire input buffer, ignore parameters */
        struct evbuffer *input = bufferevent_get_input(bev);
        if (input) {
            r = evbuffer_get_length(input);
            evbuffer_drain(input, r);
        }
        r = 0;
    } else if (data == NULL) {
        struct evbuffer *input = bufferevent_get_input(bev);
        r = input ? evbuffer_get_length(input) : 0;
    } else if (data_size && *data_size > 0) {
        r = bufferevent_read(bev, data, *data_size);
        if (r >= 0) {
            node->rx_bytes += r;
        }
    } else {
        r = 0;
    }
    if (data_size)
        *data_size = r;

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

	if (error != ZET76_OK && data_size)
        *data_size = 0;
    return error;

}

/****************************************************************************/
ZET76_EXPORT zet76_control(struct zet76_server *server,
                           const struct zet76_device *device,
                           int control_command,
                           unsigned long param,
                           const void *data,
                           int data_size)
{
    ZDCP_COMMON_PACKET zdcp;
    struct zet76_node *node = NULL;
    unsigned long zdcp_cmd;
    int r;
    int error = ZET76_OK;

    ZET76_VALIDATE_PARAM_OBJECT(server, ZET76_MAGIC_SERVER);
    ZET76_VALIDATE_PARAM_PTR(device);

    if (data != NULL) {
        ZET76_VALIDATE_PARAM(data_size >= 0);
        ZET76_VALIDATE_PARAM(data_size <= ZET76_MAX_CONTROL_DATA_SIZE);
    } else {
        data_size = 0;
    }

    switch (control_command) {
    case ZET76_COMMAND_RESTART:
        zdcp_cmd = ZDCP_COMMAND_RESTART;
        break;
    case ZET76_COMMAND_RESET:
        zdcp_cmd = ZDCP_COMMAND_RESET;
        break;
    case ZET76_COMMAND_RSI_READ:
        zdcp_cmd = ZDCP_COMMAND_RSI_READ;
        break;
    case ZET76_COMMAND_RSI_WRITE:
        ZET76_VALIDATE_PARAM(data_size >= ZET76_SIZE_RSI_SECTOR);
        zdcp_cmd = ZDCP_COMMAND_RSI_WRITE;
        break;
    default:
        return ZET76_NOT_SUPPORTED;
    }

    ZET76_GUARD_LOCK(server->guard);
    node = zet76_node_open(server, device);
    ZET76_EXIT_IF_NULL(node, ZET76_OBJECT_NOT_FOUND);

    ZET76_EXIT_IF_FALSE(node->state == ZET76_DEVICE_CONTROLLED,
                        ZET76_INVALID_STATE);
    ZET76_EXIT_IF_FALSE(node->control_send != ZET76_CONTROL_SEND_WAIT,
                        ZET76_INVALID_STATE);

    zet76_node_control_reset(node);

    node->control_zdcp_cmd = zdcp_cmd;
    node->control_zdcp_seq = ntohl(htonl(node->control_zdcp_seq) + 1);

    zdcp.header.signature  = htonl(ZDCP_SIGNATURE);
    zdcp.header.id         = htons(ZDCP_ID_COMMAND);
    zdcp.header.length     = htons(ZDCP_LENGTH_COMMAND + data_size);
    zdcp.command.serial_hi = htonl(ZET76_HI_U32(device->serial));
    zdcp.command.serial_lo = htonl(ZET76_LO_U32(device->serial));
    zdcp.command.type      = htonl(node->id.type);
    zdcp.command.sequence  = htonl(node->control_zdcp_seq);
    zdcp.command.command   = htonl(node->control_zdcp_cmd);
    zdcp.command.param     = htonl(param);

    r = bufferevent_write(node->control_bev, &zdcp, sizeof(ZDCP_COMMAND));
    ZET76_EXIT_IF_FALSE(r == 0, ZET76_ERROR);
    if (data_size > 0) {
        r = bufferevent_write(node->control_bev, data, data_size);
        ZET76_EXIT_IF_FALSE(r == 0, ZET76_ERROR);
    }
    bufferevent_flush(node->control_bev, EV_WRITE, BEV_FLUSH);

    node->control_send = ZET76_CONTROL_SEND_WAIT;

cleanup:
    ZET76_GUARD_UNLOCK(server->guard);

    return error;
}
