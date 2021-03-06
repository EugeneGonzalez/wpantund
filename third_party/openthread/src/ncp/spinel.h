/*
 *    Copyright (c) 2016, The OpenThread Authors.
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 *    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file contains definitions of spinel API.
 */

#ifndef SPINEL_HEADER_INCLUDED
#define SPINEL_HEADER_INCLUDED 1

#ifdef SPINEL_PLATFORM_HEADER
#include SPINEL_PLATFORM_HEADER
#else // ifdef SPINEL_PLATFORM_HEADER
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#endif // else SPINEL_PLATFORM_HEADER

// ----------------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

# if defined(__GNUC__)
#  define SPINEL_API_EXTERN             extern __attribute__ ((visibility ("default")))
#  define SPINEL_API_NONNULL_ALL        __attribute__((nonnull))
#  define SPINEL_API_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
# endif // ifdef __GNUC__

# if !defined(__BEGIN_DECLS) || !defined(__END_DECLS)
#  if defined(__cplusplus)
#   define __BEGIN_DECLS   extern "C" {
#   define __END_DECLS     }
#  else // if defined(__cplusplus)
#   define __BEGIN_DECLS
#   define __END_DECLS
#  endif // else defined(__cplusplus)
# endif // if !defined(__BEGIN_DECLS) || !defined(__END_DECLS)

#endif // ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef SPINEL_API_EXTERN
# define SPINEL_API_EXTERN              extern
#endif

#ifndef SPINEL_API_NONNULL_ALL
# define SPINEL_API_NONNULL_ALL
#endif

#ifndef SPINEL_API_WARN_UNUSED_RESULT
# define SPINEL_API_WARN_UNUSED_RESULT
#endif

// ----------------------------------------------------------------------------

#define SPINEL_PROTOCOL_VERSION_THREAD_MAJOR    4
#define SPINEL_PROTOCOL_VERSION_THREAD_MINOR    3

#define SPINEL_FRAME_MAX_SIZE           1300

/**
 * @def SPINEL_ENCRYPTER_EXTRA_DATA_SIZE
 *
 *  The size of extra data to be allocated for spinel frame buffer,
 *  needed by Spinel Encrypter.
 *
 */
#define SPINEL_ENCRYPTER_EXTRA_DATA_SIZE  0

/**
 * @def SPINEL_FRAME_BUFFER_SIZE
 *
 *  The size of buffer large enough to fit one whole spinel frame with extra data
 *  needed by Spinel Encrypter.
 *
 */
#define SPINEL_FRAME_BUFFER_SIZE          (SPINEL_FRAME_MAX_SIZE + SPINEL_ENCRYPTER_EXTRA_DATA_SIZE)

/// Macro for generating bit masks using bit index from the spec
#define SPINEL_BIT_MASK(bit_index,field_bit_count)                                                      \
                                        ( (1 << ((field_bit_count) - 1)) >> (bit_index) )

// ----------------------------------------------------------------------------

__BEGIN_DECLS

typedef enum
{
    SPINEL_STATUS_OK                    = 0, ///< Operation has completed successfully.
    SPINEL_STATUS_FAILURE               = 1, ///< Operation has failed for some undefined reason.

    SPINEL_STATUS_UNIMPLEMENTED         = 2, ///< Given operation has not been implemented.
    SPINEL_STATUS_INVALID_ARGUMENT      = 3, ///< An argument to the operation is invalid.
    SPINEL_STATUS_INVALID_STATE         = 4, ///< This operation is invalid for the current device state.
    SPINEL_STATUS_INVALID_COMMAND       = 5, ///< This command is not recognized.
    SPINEL_STATUS_INVALID_INTERFACE     = 6, ///< This interface is not supported.
    SPINEL_STATUS_INTERNAL_ERROR        = 7, ///< An internal runtime error has occured.
    SPINEL_STATUS_SECURITY_ERROR        = 8, ///< A security/authentication error has occured.
    SPINEL_STATUS_PARSE_ERROR           = 9, ///< A error has occured while parsing the command.
    SPINEL_STATUS_IN_PROGRESS           = 10, ///< This operation is in progress.
    SPINEL_STATUS_NOMEM                 = 11, ///< Operation prevented due to memory pressure.
    SPINEL_STATUS_BUSY                  = 12, ///< The device is currently performing a mutually exclusive operation
    SPINEL_STATUS_PROP_NOT_FOUND        = 13, ///< The given property is not recognized.
    SPINEL_STATUS_DROPPED               = 14, ///< A/The packet was dropped.
    SPINEL_STATUS_EMPTY                 = 15, ///< The result of the operation is empty.
    SPINEL_STATUS_CMD_TOO_BIG           = 16, ///< The command was too large to fit in the internal buffer.
    SPINEL_STATUS_NO_ACK                = 17, ///< The packet was not acknowledged.
    SPINEL_STATUS_CCA_FAILURE           = 18, ///< The packet was not sent due to a CCA failure.
    SPINEL_STATUS_ALREADY               = 19, ///< The operation is already in progress.
    SPINEL_STATUS_ITEM_NOT_FOUND        = 20, ///< The given item could not be found.
    SPINEL_STATUS_INVALID_COMMAND_FOR_PROP
                                        = 21, ///< The given command cannot be performed on this property.

    SPINEL_STATUS_JOIN__BEGIN           = 104,

    /// Generic failure to associate with other peers.
    /**
     *  This status error should not be used by implementors if
     *  enough information is available to determine that one of the
     *  later join failure status codes would be more accurate.
     *
     *  \sa SPINEL_PROP_NET_REQUIRE_JOIN_EXISTING
     */
    SPINEL_STATUS_JOIN_FAILURE          = SPINEL_STATUS_JOIN__BEGIN + 0,

    /// The node found other peers but was unable to decode their packets.
    /**
     *  Typically this error code indicates that the network
     *  key has been set incorrectly.
     *
     *  \sa SPINEL_PROP_NET_REQUIRE_JOIN_EXISTING
     */
    SPINEL_STATUS_JOIN_SECURITY         = SPINEL_STATUS_JOIN__BEGIN + 1,

    /// The node was unable to find any other peers on the network.
    /**
     *  \sa SPINEL_PROP_NET_REQUIRE_JOIN_EXISTING
     */
    SPINEL_STATUS_JOIN_NO_PEERS         = SPINEL_STATUS_JOIN__BEGIN + 2,

    /// The only potential peer nodes found are incompatible.
    /**
     *  \sa SPINEL_PROP_NET_REQUIRE_JOIN_EXISTING
     */
    SPINEL_STATUS_JOIN_INCOMPATIBLE     = SPINEL_STATUS_JOIN__BEGIN + 3,

    SPINEL_STATUS_JOIN__END             = 112,

    SPINEL_STATUS_RESET__BEGIN          = 112,
    SPINEL_STATUS_RESET_POWER_ON        = SPINEL_STATUS_RESET__BEGIN + 0,
    SPINEL_STATUS_RESET_EXTERNAL        = SPINEL_STATUS_RESET__BEGIN + 1,
    SPINEL_STATUS_RESET_SOFTWARE        = SPINEL_STATUS_RESET__BEGIN + 2,
    SPINEL_STATUS_RESET_FAULT           = SPINEL_STATUS_RESET__BEGIN + 3,
    SPINEL_STATUS_RESET_CRASH           = SPINEL_STATUS_RESET__BEGIN + 4,
    SPINEL_STATUS_RESET_ASSERT          = SPINEL_STATUS_RESET__BEGIN + 5,
    SPINEL_STATUS_RESET_OTHER           = SPINEL_STATUS_RESET__BEGIN + 6,
    SPINEL_STATUS_RESET_UNKNOWN         = SPINEL_STATUS_RESET__BEGIN + 7,
    SPINEL_STATUS_RESET_WATCHDOG        = SPINEL_STATUS_RESET__BEGIN + 8,
    SPINEL_STATUS_RESET__END            = 128,

    SPINEL_STATUS_VENDOR__BEGIN         = 15360,
    SPINEL_STATUS_VENDOR__END           = 16384,

    SPINEL_STATUS_STACK_NATIVE__BEGIN   = 16384,
    SPINEL_STATUS_STACK_NATIVE__END     = 81920,

    SPINEL_STATUS_EXPERIMENTAL__BEGIN   = 2000000,
    SPINEL_STATUS_EXPERIMENTAL__END     = 2097152,
} spinel_status_t;

typedef enum
{
    SPINEL_NET_ROLE_DETACHED            = 0,
    SPINEL_NET_ROLE_CHILD               = 1,
    SPINEL_NET_ROLE_ROUTER              = 2,
    SPINEL_NET_ROLE_LEADER              = 3,
} spinel_net_role_t;

typedef enum
{
    SPINEL_IPV6_ICMP_PING_OFFLOAD_DISABLED       = 0,
    SPINEL_IPV6_ICMP_PING_OFFLOAD_UNICAST_ONLY   = 1,
    SPINEL_IPV6_ICMP_PING_OFFLOAD_MULTICAST_ONLY = 2,
    SPINEL_IPV6_ICMP_PING_OFFLOAD_ALL            = 3,
} spinel_ipv6_icmp_ping_offload_mode_t;

typedef enum
{
    SPINEL_SCAN_STATE_IDLE              = 0,
    SPINEL_SCAN_STATE_BEACON            = 1,
    SPINEL_SCAN_STATE_ENERGY            = 2,
    SPINEL_SCAN_STATE_DISCOVER          = 3,
} spinel_scan_state_t;

typedef enum
{
    SPINEL_MCU_POWER_STATE_ON           = 0,
    SPINEL_MCU_POWER_STATE_LOW_POWER    = 1,
    SPINEL_MCU_POWER_STATE_OFF          = 2,
} spinel_mcu_power_state_t;

// The `spinel_power_state_t` enumeration and `POWER_STATE`
// property are deprecated. Please use `MCU_POWER_STATE`
// instead.
typedef enum
{
    SPINEL_POWER_STATE_OFFLINE          = 0,
    SPINEL_POWER_STATE_DEEP_SLEEP       = 1,
    SPINEL_POWER_STATE_STANDBY          = 2,
    SPINEL_POWER_STATE_LOW_POWER        = 3,
    SPINEL_POWER_STATE_ONLINE           = 4,
} spinel_power_state_t;

typedef enum
{
    SPINEL_HOST_POWER_STATE_OFFLINE     = 0,
    SPINEL_HOST_POWER_STATE_DEEP_SLEEP  = 1,
    SPINEL_HOST_POWER_STATE_RESERVED    = 2,
    SPINEL_HOST_POWER_STATE_LOW_POWER   = 3,
    SPINEL_HOST_POWER_STATE_ONLINE      = 4,
} spinel_host_power_state_t;

enum
{
    SPINEL_NET_FLAG_ON_MESH             = (1 << 0),
    SPINEL_NET_FLAG_DEFAULT_ROUTE       = (1 << 1),
    SPINEL_NET_FLAG_CONFIGURE           = (1 << 2),
    SPINEL_NET_FLAG_DHCP                = (1 << 3),
    SPINEL_NET_FLAG_SLAAC               = (1 << 4),
    SPINEL_NET_FLAG_PREFERRED           = (1 << 5),

    SPINEL_NET_FLAG_PREFERENCE_OFFSET   = 6,
    SPINEL_NET_FLAG_PREFERENCE_MASK     = (3 << SPINEL_NET_FLAG_PREFERENCE_OFFSET),
};

enum
{
    SPINEL_ROUTE_PREFERENCE_HIGH        = (1 << SPINEL_NET_FLAG_PREFERENCE_OFFSET),
    SPINEL_ROUTE_PREFERENCE_MEDIUM      = (0 << SPINEL_NET_FLAG_PREFERENCE_OFFSET),
    SPINEL_ROUTE_PREFERENCE_LOW         = (3 << SPINEL_NET_FLAG_PREFERENCE_OFFSET),
};

enum
{
    SPINEL_THREAD_MODE_FULL_NETWORK_DATA    = (1 << 0),
    SPINEL_THREAD_MODE_FULL_FUNCTION_DEV    = (1 << 1),
    SPINEL_THREAD_MODE_SECURE_DATA_REQUEST  = (1 << 2),
    SPINEL_THREAD_MODE_RX_ON_WHEN_IDLE      = (1 << 3),
};

enum {
    SPINEL_GPIO_FLAG_DIR_INPUT          = 0,
    SPINEL_GPIO_FLAG_DIR_OUTPUT         = SPINEL_BIT_MASK(0, 8),
    SPINEL_GPIO_FLAG_PULL_UP            = SPINEL_BIT_MASK(1, 8),
    SPINEL_GPIO_FLAG_PULL_DOWN          = SPINEL_BIT_MASK(2, 8),
    SPINEL_GPIO_FLAG_OPEN_DRAIN         = SPINEL_BIT_MASK(2, 8),
    SPINEL_GPIO_FLAG_TRIGGER_NONE       = 0,
    SPINEL_GPIO_FLAG_TRIGGER_RISING     = SPINEL_BIT_MASK(3, 8),
    SPINEL_GPIO_FLAG_TRIGGER_FALLING    = SPINEL_BIT_MASK(4, 8),
    SPINEL_GPIO_FLAG_TRIGGER_ANY        = SPINEL_GPIO_FLAG_TRIGGER_RISING
                                        | SPINEL_GPIO_FLAG_TRIGGER_FALLING,
};

enum
{
    SPINEL_PROTOCOL_TYPE_BOOTLOADER     = 0,
    SPINEL_PROTOCOL_TYPE_ZIGBEE_IP      = 2,
    SPINEL_PROTOCOL_TYPE_THREAD         = 3,
};

enum
{
    SPINEL_MAC_PROMISCUOUS_MODE_OFF     = 0, ///< Normal MAC filtering is in place.
    SPINEL_MAC_PROMISCUOUS_MODE_NETWORK = 1, ///< All MAC packets matching network are passed up the stack.
    SPINEL_MAC_PROMISCUOUS_MODE_FULL    = 2, ///< All decoded MAC packets are passed up the stack.
};

enum
{
    SPINEL_NCP_LOG_LEVEL_EMERG          = 0,
    SPINEL_NCP_LOG_LEVEL_ALERT          = 1,
    SPINEL_NCP_LOG_LEVEL_CRIT           = 2,
    SPINEL_NCP_LOG_LEVEL_ERR            = 3,
    SPINEL_NCP_LOG_LEVEL_WARN           = 4,
    SPINEL_NCP_LOG_LEVEL_NOTICE         = 5,
    SPINEL_NCP_LOG_LEVEL_INFO           = 6,
    SPINEL_NCP_LOG_LEVEL_DEBUG          = 7,
};

enum
{
    SPINEL_NCP_LOG_REGION_NONE          = 0,
    SPINEL_NCP_LOG_REGION_OT_API        = 1,
    SPINEL_NCP_LOG_REGION_OT_MLE        = 2,
    SPINEL_NCP_LOG_REGION_OT_ARP        = 3,
    SPINEL_NCP_LOG_REGION_OT_NET_DATA   = 4,
    SPINEL_NCP_LOG_REGION_OT_ICMP       = 5,
    SPINEL_NCP_LOG_REGION_OT_IP6        = 6,
    SPINEL_NCP_LOG_REGION_OT_MAC        = 7,
    SPINEL_NCP_LOG_REGION_OT_MEM        = 8,
    SPINEL_NCP_LOG_REGION_OT_NCP        = 9,
    SPINEL_NCP_LOG_REGION_OT_MESH_COP   = 10,
    SPINEL_NCP_LOG_REGION_OT_NET_DIAG   = 11,
    SPINEL_NCP_LOG_REGION_OT_PLATFORM   = 12,
    SPINEL_NCP_LOG_REGION_OT_COAP       = 13,
    SPINEL_NCP_LOG_REGION_OT_CLI        = 14,
    SPINEL_NCP_LOG_REGION_OT_CORE       = 15,
    SPINEL_NCP_LOG_REGION_OT_UTIL       = 16,
};

typedef struct
{
    uint8_t bytes[8];
} spinel_eui64_t;

typedef struct
{
    uint8_t bytes[8];
} spinel_net_xpanid_t;

typedef struct
{
    uint8_t bytes[16];
} spinel_net_pskc_t;

typedef struct
{
    uint8_t bytes[6];
} spinel_eui48_t;

typedef struct
{
    uint8_t bytes[16];
} spinel_ipv6addr_t;

typedef int spinel_ssize_t;
typedef unsigned int spinel_size_t;
typedef uint8_t spinel_tid_t;
typedef unsigned int spinel_cid_t;

enum
{
    SPINEL_MD_FLAG_TX                   = 0x0001, //!< Packet was transmitted, not received.
    SPINEL_MD_FLAG_BAD_FCS              = 0x0004, //!< Packet was received with bad FCS
    SPINEL_MD_FLAG_DUPE                 = 0x0008, //!< Packet seems to be a duplicate
    SPINEL_MD_FLAG_RESERVED             = 0xFFF2, //!< Flags reserved for future use.
};

enum
{
    SPINEL_CMD_NOOP                     = 0,
    SPINEL_CMD_RESET                    = 1,
    SPINEL_CMD_PROP_VALUE_GET           = 2,
    SPINEL_CMD_PROP_VALUE_SET           = 3,
    SPINEL_CMD_PROP_VALUE_INSERT        = 4,
    SPINEL_CMD_PROP_VALUE_REMOVE        = 5,
    SPINEL_CMD_PROP_VALUE_IS            = 6,
    SPINEL_CMD_PROP_VALUE_INSERTED      = 7,
    SPINEL_CMD_PROP_VALUE_REMOVED       = 8,

    SPINEL_CMD_NET_SAVE                 = 9,
    SPINEL_CMD_NET_CLEAR                = 10,
    SPINEL_CMD_NET_RECALL               = 11,

    SPINEL_CMD_HBO_OFFLOAD              = 12,
    SPINEL_CMD_HBO_RECLAIM              = 13,
    SPINEL_CMD_HBO_DROP                 = 14,
    SPINEL_CMD_HBO_OFFLOADED            = 15,
    SPINEL_CMD_HBO_RECLAIMED            = 16,
    SPINEL_CMD_HBO_DROPED               = 17,

    SPINEL_CMD_PEEK                     = 18,
    SPINEL_CMD_PEEK_RET                 = 19,
    SPINEL_CMD_POKE                     = 20,

    SPINEL_CMD_PROP_VALUE_MULTI_GET     = 21,
    SPINEL_CMD_PROP_VALUE_MULTI_SET     = 22,
    SPINEL_CMD_PROP_VALUES_ARE          = 23,

    SPINEL_CMD_NEST__BEGIN              = 15296,
    SPINEL_CMD_NEST__END                = 15360,

    SPINEL_CMD_VENDOR__BEGIN            = 15360,
    SPINEL_CMD_VENDOR__END              = 16384,

    SPINEL_CMD_EXPERIMENTAL__BEGIN      = 2000000,
    SPINEL_CMD_EXPERIMENTAL__END        = 2097152,
};

enum
{
    SPINEL_CAP_LOCK                     = 1,
    SPINEL_CAP_NET_SAVE                 = 2,
    SPINEL_CAP_HBO                      = 3,
    SPINEL_CAP_POWER_SAVE               = 4,

    SPINEL_CAP_COUNTERS                 = 5,
    SPINEL_CAP_JAM_DETECT               = 6,

    SPINEL_CAP_PEEK_POKE                = 7,

    SPINEL_CAP_WRITABLE_RAW_STREAM      = 8,
    SPINEL_CAP_GPIO                     = 9,
    SPINEL_CAP_TRNG                     = 10,
    SPINEL_CAP_CMD_MULTI                = 11,
    SPINEL_CAP_UNSOL_UPDATE_FILTER      = 12,
    SPINEL_CAP_MCU_POWER_STATE          = 13,

    SPINEL_CAP_802_15_4__BEGIN          = 16,
    SPINEL_CAP_802_15_4_2003            = (SPINEL_CAP_802_15_4__BEGIN + 0),
    SPINEL_CAP_802_15_4_2006            = (SPINEL_CAP_802_15_4__BEGIN + 1),
    SPINEL_CAP_802_15_4_2011            = (SPINEL_CAP_802_15_4__BEGIN + 2),
    SPINEL_CAP_802_15_4_PIB             = (SPINEL_CAP_802_15_4__BEGIN + 5),
    SPINEL_CAP_802_15_4_2450MHZ_OQPSK   = (SPINEL_CAP_802_15_4__BEGIN + 8),
    SPINEL_CAP_802_15_4_915MHZ_OQPSK    = (SPINEL_CAP_802_15_4__BEGIN + 9),
    SPINEL_CAP_802_15_4_868MHZ_OQPSK    = (SPINEL_CAP_802_15_4__BEGIN + 10),
    SPINEL_CAP_802_15_4_915MHZ_BPSK     = (SPINEL_CAP_802_15_4__BEGIN + 11),
    SPINEL_CAP_802_15_4_868MHZ_BPSK     = (SPINEL_CAP_802_15_4__BEGIN + 12),
    SPINEL_CAP_802_15_4_915MHZ_ASK      = (SPINEL_CAP_802_15_4__BEGIN + 13),
    SPINEL_CAP_802_15_4_868MHZ_ASK      = (SPINEL_CAP_802_15_4__BEGIN + 14),
    SPINEL_CAP_802_15_4__END            = 32,

    SPINEL_CAP_ROLE__BEGIN              = 48,
    SPINEL_CAP_ROLE_ROUTER              = (SPINEL_CAP_ROLE__BEGIN + 0),
    SPINEL_CAP_ROLE_SLEEPY              = (SPINEL_CAP_ROLE__BEGIN + 1),
    SPINEL_CAP_ROLE__END                = 52,

    SPINEL_CAP_NET__BEGIN               = 52,
    SPINEL_CAP_NET_THREAD_1_0           = (SPINEL_CAP_NET__BEGIN + 0),
    SPINEL_CAP_NET__END                 = 64,

    SPINEL_CAP_OPENTHREAD__BEGIN        = 512,
    SPINEL_CAP_MAC_WHITELIST            = (SPINEL_CAP_OPENTHREAD__BEGIN + 0),
    SPINEL_CAP_MAC_RAW                  = (SPINEL_CAP_OPENTHREAD__BEGIN + 1),
    SPINEL_CAP_OOB_STEERING_DATA        = (SPINEL_CAP_OPENTHREAD__BEGIN + 2),
    SPINEL_CAP_CHANNEL_MONITOR          = (SPINEL_CAP_OPENTHREAD__BEGIN + 3),
    SPINEL_CAP_ERROR_RATE_TRACKING      = (SPINEL_CAP_OPENTHREAD__BEGIN + 4),
    SPINEL_CAP_CHANNEL_MANAGER          = (SPINEL_CAP_OPENTHREAD__BEGIN + 5),
    SPINEL_CAP_OPENTHREAD_LOG_METADATA  = (SPINEL_CAP_OPENTHREAD__BEGIN + 6),
    SPINEL_CAP_TIME_SYNC                = (SPINEL_CAP_OPENTHREAD__BEGIN + 7),
    SPINEL_CAP_OPENTHREAD__END          = 640,

    SPINEL_CAP_THREAD__BEGIN            = 1024,
    SPINEL_CAP_THREAD_COMMISSIONER      = (SPINEL_CAP_THREAD__BEGIN + 0),
    SPINEL_CAP_THREAD_TMF_PROXY         = (SPINEL_CAP_THREAD__BEGIN + 1),
    SPINEL_CAP_THREAD__END              = 1152,

    SPINEL_CAP_NEST__BEGIN              = 15296,
    SPINEL_CAP_NEST_LEGACY_INTERFACE    = (SPINEL_CAP_NEST__BEGIN + 0),
    SPINEL_CAP_NEST_LEGACY_NET_WAKE     = (SPINEL_CAP_NEST__BEGIN + 1),
    SPINEL_CAP_NEST_TRANSMIT_HOOK       = (SPINEL_CAP_NEST__BEGIN + 2),
    SPINEL_CAP_NEST__END                = 15360,

    SPINEL_CAP_VENDOR__BEGIN            = 15360,
    SPINEL_CAP_VENDOR__END              = 16384,

    SPINEL_CAP_EXPERIMENTAL__BEGIN      = 2000000,
    SPINEL_CAP_EXPERIMENTAL__END        = 2097152,
};

typedef enum
{
    SPINEL_PROP_LAST_STATUS             = 0,        ///< status [i]
    SPINEL_PROP_PROTOCOL_VERSION        = 1,        ///< major, minor [i,i]
    SPINEL_PROP_NCP_VERSION             = 2,        ///< version string [U]
    SPINEL_PROP_INTERFACE_TYPE          = 3,        ///< [i]
    SPINEL_PROP_VENDOR_ID               = 4,        ///< [i]
    SPINEL_PROP_CAPS                    = 5,        ///< capability list [A(i)]
    SPINEL_PROP_INTERFACE_COUNT         = 6,        ///< Interface count [C]
    SPINEL_PROP_POWER_STATE             = 7,        ///< PowerState [C] (deprecated, use `MCU_POWER_STATE` instead).
    SPINEL_PROP_HWADDR                  = 8,        ///< PermEUI64 [E]
    SPINEL_PROP_LOCK                    = 9,        ///< PropLock [b]
    SPINEL_PROP_HBO_MEM_MAX             = 10,       ///< Max offload mem [S]
    SPINEL_PROP_HBO_BLOCK_MAX           = 11,       ///< Max offload block [S]
    SPINEL_PROP_HOST_POWER_STATE        = 12,       ///< Host MCU power state [C]
    SPINEL_PROP_MCU_POWER_STATE         = 13,       ///< NCP's MCU power state [c]

    SPINEL_PROP_BASE_EXT__BEGIN         = 0x1000,

    /// GPIO Configuration
    /** Format: `A(CCU)`
     *  Type: Read-Only (Optionally Read-write using `CMD_PROP_VALUE_INSERT`)
     *
     * An array of structures which contain the following fields:
     *
     * *   `C`: GPIO Number
     * *   `C`: GPIO Configuration Flags
     * *   `U`: Human-readable GPIO name
     *
     * GPIOs which do not have a corresponding entry are not supported.
     *
     * The configuration parameter contains the configuration flags for the
     * GPIO:
     *
     *       0   1   2   3   4   5   6   7
     *     +---+---+---+---+---+---+---+---+
     *     |DIR|PUP|PDN|TRIGGER|  RESERVED |
     *     +---+---+---+---+---+---+---+---+
     *             |O/D|
     *             +---+
     *
     * *   `DIR`: Pin direction. Clear (0) for input, set (1) for output.
     * *   `PUP`: Pull-up enabled flag.
     * *   `PDN`/`O/D`: Flag meaning depends on pin direction:
     *     *   Input: Pull-down enabled.
     *     *   Output: Output is an open-drain.
     * *   `TRIGGER`: Enumeration describing how pin changes generate
     *     asynchronous notification commands (TBD) from the NCP to the host.
     *     *   0: Feature disabled for this pin
     *     *   1: Trigger on falling edge
     *     *   2: Trigger on rising edge
     *     *   3: Trigger on level change
     * *   `RESERVED`: Bits reserved for future use. Always cleared to zero
     *     and ignored when read.
     *
     * As an optional feature, the configuration of individual pins may be
     * modified using the `CMD_PROP_VALUE_INSERT` command. Only the GPIO
     * number and flags fields MUST be present, the GPIO name (if present)
     * would be ignored. This command can only be used to modify the
     * configuration of GPIOs which are already exposed---it cannot be used
     * by the host to add additional GPIOs.
     */
    SPINEL_PROP_GPIO_CONFIG             = SPINEL_PROP_BASE_EXT__BEGIN + 0,

    /// GPIO State Bitmask
    /** Format: `D`
     *  Type: Read-Write
     *
     * Contains a bit field identifying the state of the GPIOs. The length of
     * the data associated with these properties depends on the number of
     * GPIOs. If you have 10 GPIOs, you'd have two bytes. GPIOs are numbered
     * from most significant bit to least significant bit, so 0x80 is GPIO 0,
     * 0x40 is GPIO 1, etc.
     *
     * For GPIOs configured as inputs:
     *
     * *   `CMD_PROP_VAUE_GET`: The value of the associated bit describes the
     *     logic level read from the pin.
     * *   `CMD_PROP_VALUE_SET`: The value of the associated bit is ignored
     *     for these pins.
     *
     * For GPIOs configured as outputs:
     *
     * *   `CMD_PROP_VAUE_GET`: The value of the associated bit is
     *     implementation specific.
     * *   `CMD_PROP_VALUE_SET`: The value of the associated bit determines
     *     the new logic level of the output. If this pin is configured as an
     *     open-drain, setting the associated bit to 1 will cause the pin to
     *     enter a Hi-Z state.
     *
     * For GPIOs which are not specified in `PROP_GPIO_CONFIG`:
     *
     * *   `CMD_PROP_VAUE_GET`: The value of the associated bit is
     *     implementation specific.
     * *   `CMD_PROP_VALUE_SET`: The value of the associated bit MUST be
     *     ignored by the NCP.
     *
     * When writing, unspecified bits are assumed to be zero.
     */
    SPINEL_PROP_GPIO_STATE              = SPINEL_PROP_BASE_EXT__BEGIN + 2,

    /// GPIO State Set-Only Bitmask
    /** Format: `D`
     *  Type: Write-Only
     *
     * Allows for the state of various output GPIOs to be set without affecting
     * other GPIO states. Contains a bit field identifying the output GPIOs that
     * should have their state set to 1.
     *
     * When writing, unspecified bits are assumed to be zero. The value of
     * any bits for GPIOs which are not specified in `PROP_GPIO_CONFIG` MUST
     * be ignored.
     */
    SPINEL_PROP_GPIO_STATE_SET          = SPINEL_PROP_BASE_EXT__BEGIN + 3,

    /// GPIO State Clear-Only Bitmask
    /** Format: `D`
     *  Type: Write-Only
     *
     * Allows for the state of various output GPIOs to be cleared without affecting
     * other GPIO states. Contains a bit field identifying the output GPIOs that
     * should have their state cleared to 0.
     *
     * When writing, unspecified bits are assumed to be zero. The value of
     * any bits for GPIOs which are not specified in `PROP_GPIO_CONFIG` MUST
     * be ignored.
     */
    SPINEL_PROP_GPIO_STATE_CLEAR        = SPINEL_PROP_BASE_EXT__BEGIN + 4,

    /// 32-bit random number from TRNG, ready-to-use.
    SPINEL_PROP_TRNG_32                 = SPINEL_PROP_BASE_EXT__BEGIN + 5,

    /// 16 random bytes from TRNG, ready-to-use.
    SPINEL_PROP_TRNG_128                = SPINEL_PROP_BASE_EXT__BEGIN + 6,

    /// Raw samples from TRNG entropy source representing 32 bits of entropy.
    SPINEL_PROP_TRNG_RAW_32             = SPINEL_PROP_BASE_EXT__BEGIN + 7,


    /// NCP Unsolicited update filter
    /** Format: `A(I)`
     *  Type: Read-Write (optional Insert-Remove)
     *  Required capability: `CAP_UNSOL_UPDATE_FILTER`
     *
     * Contains a list of properties which are excluded from generating
     * unsolicited value updates. This property is empty after reset.
     * In other words, the host may opt-out of unsolicited property updates
     * for a specific property by adding that property id to this list.
     * Hosts SHOULD NOT add properties to this list which are not
     * present in `PROP_UNSOL_UPDATE_LIST`. If such properties are added,
     * the NCP ignores the unsupported properties.
     */
    SPINEL_PROP_UNSOL_UPDATE_FILTER     = SPINEL_PROP_BASE_EXT__BEGIN + 8,

    /// List of properties capable of generating unsolicited value update.
    /** Format: `A(I)`
     *  Type: Read-Only
     *  Required capability: `CAP_UNSOL_UPDATE_FILTER`
     *
     * Contains a list of properties which are capable of generating
     * unsolicited value updates. This list can be used when populating
     * `PROP_UNSOL_UPDATE_FILTER` to disable all unsolicited property
     * updates.
     *
     * This property is intended to effectively behave as a constant
     * for a given NCP firmware.
     */
    SPINEL_PROP_UNSOL_UPDATE_LIST       = SPINEL_PROP_BASE_EXT__BEGIN + 9,

    SPINEL_PROP_BASE_EXT__END           = 0x1100,

    SPINEL_PROP_PHY__BEGIN              = 0x20,
    SPINEL_PROP_PHY_ENABLED             = SPINEL_PROP_PHY__BEGIN + 0, ///< [b]
    SPINEL_PROP_PHY_CHAN                = SPINEL_PROP_PHY__BEGIN + 1, ///< [C]
    SPINEL_PROP_PHY_CHAN_SUPPORTED      = SPINEL_PROP_PHY__BEGIN + 2, ///< [A(C)]
    SPINEL_PROP_PHY_FREQ                = SPINEL_PROP_PHY__BEGIN + 3, ///< kHz [L]
    SPINEL_PROP_PHY_CCA_THRESHOLD       = SPINEL_PROP_PHY__BEGIN + 4, ///< dBm [c]
    SPINEL_PROP_PHY_TX_POWER            = SPINEL_PROP_PHY__BEGIN + 5, ///< [c]
    SPINEL_PROP_PHY_RSSI                = SPINEL_PROP_PHY__BEGIN + 6, ///< dBm [c]
    SPINEL_PROP_PHY_RX_SENSITIVITY      = SPINEL_PROP_PHY__BEGIN + 7, ///< dBm [c]
    SPINEL_PROP_PHY__END                = 0x30,

    SPINEL_PROP_PHY_EXT__BEGIN          = 0x1200,

    /// Signal Jamming Detection Enable
    /** Format: `b`
     *
     * Indicates if jamming detection is enabled or disabled. Set to true
     * to enable jamming detection.
     */
    SPINEL_PROP_JAM_DETECT_ENABLE       = SPINEL_PROP_PHY_EXT__BEGIN + 0,

    /// Signal Jamming Detected Indicator
    /** Format: `b` (Read-Only)
     *
     * Set to true if radio jamming is detected. Set to false otherwise.
     *
     * When jamming detection is enabled, changes to the value of this
     * property are emitted asynchronously via `CMD_PROP_VALUE_IS`.
     */
    SPINEL_PROP_JAM_DETECTED            = SPINEL_PROP_PHY_EXT__BEGIN + 1,

    /// Jamming detection RSSI threshold
    /** Format: `c`
     *  Units: dBm
     *
     * This parameter describes the threshold RSSI level (measured in
     * dBm) above which the jamming detection will consider the
     * channel blocked.
     */
    SPINEL_PROP_JAM_DETECT_RSSI_THRESHOLD
                                        = SPINEL_PROP_PHY_EXT__BEGIN + 2,

    /// Jamming detection window size
    /** Format: `C`
     *  Units: Seconds (1-63)
     *
     * This parameter describes the window period for signal jamming
     * detection.
     */
    SPINEL_PROP_JAM_DETECT_WINDOW       = SPINEL_PROP_PHY_EXT__BEGIN + 3,

    /// Jamming detection busy period
    /** Format: `C`
     *  Units: Seconds (1-63)
     *
     * This parameter describes the number of aggregate seconds within
     * the detection window where the RSSI must be above
     * `PROP_JAM_DETECT_RSSI_THRESHOLD` to trigger detection.
     *
     * The behavior of the jamming detection feature when `PROP_JAM_DETECT_BUSY`
     * is larger than `PROP_JAM_DETECT_WINDOW` is undefined.
     */
    SPINEL_PROP_JAM_DETECT_BUSY         = SPINEL_PROP_PHY_EXT__BEGIN + 4,

    /// Jamming detection history bitmap (for debugging)
    /** Format: `X` (read-only)
     *
     * This value provides information about current state of jamming detection
     * module for monitoring/debugging purpose. It returns a 64-bit value where
     * each bit corresponds to one second interval starting with bit 0 for the
     * most recent interval and bit 63 for the oldest intervals (63 sec earlier).
     * The bit is set to 1 if the jamming detection module observed/detected
     * high signal level during the corresponding one second interval.
     *
     */
    SPINEL_PROP_JAM_DETECT_HISTORY_BITMAP
                                        = SPINEL_PROP_PHY_EXT__BEGIN + 5,

    /// Channel monitoring sample interval
    /** Format: `L` (read-only)
     *  Units: Milliseconds
     *
     * Required capability: SPINEL_CAP_CHANNEL_MONITOR
     *
     * If channel monitoring is enabled and active, every sample interval, a
     * zero-duration Energy Scan is performed, collecting a single RSSI sample
     * per channel. The RSSI samples are compared with a pre-specified RSSI
     * threshold.
     *
     */
    SPINEL_PROP_CHANNEL_MONITOR_SAMPLE_INTERVAL
                                        = SPINEL_PROP_PHY_EXT__BEGIN + 6,

    /// Channel monitoring RSSI threshold
    /** Format: `c` (read-only)
     *  Units: dBm
     *
     * Required capability: SPINEL_CAP_CHANNEL_MONITOR
     *
     * This value specifies the threshold used by channel monitoring module.
     * Channel monitoring maintains the average rate of RSSI samples that
     * are above the threshold within (approximately) a pre-specified number
     * of samples (sample window).
     *
     */
    SPINEL_PROP_CHANNEL_MONITOR_RSSI_THRESHOLD
                                        = SPINEL_PROP_PHY_EXT__BEGIN + 7,

    /// Channel monitoring sample window
    /** Format: `L` (read-only)
     *  Units: Number of samples
     *
     * Required capability: SPINEL_CAP_CHANNEL_MONITOR
     *
     * The averaging sample window length (in units of number of channel
     * samples) used by channel monitoring module. Channel monitoring will
     * sample all channels every sample interval. It maintains the average rate
     * of RSSI samples that are above the RSSI threshold within (approximately)
     * the sample window.
     *
     */
    SPINEL_PROP_CHANNEL_MONITOR_SAMPLE_WINDOW
                                        = SPINEL_PROP_PHY_EXT__BEGIN + 8,

    /// Channel monitoring sample count
    /** Format: `L` (read-only)
     *  Units: Number of samples
     *
     * Required capability: SPINEL_CAP_CHANNEL_MONITOR
     *
     * Total number of RSSI samples (per channel) taken by the channel
     * monitoring module since its start (since Thread network interface
     * was enabled).
     *
     */
    SPINEL_PROP_CHANNEL_MONITOR_SAMPLE_COUNT
                                        = SPINEL_PROP_PHY_EXT__BEGIN + 9,

    /// Channel monitoring channel occupancy
    /** Format: `A(t(CU))` (read-only)
     *
     * Required capability: SPINEL_CAP_CHANNEL_MONITOR
     *
     * Data per item is:
     *
     *  `C`: Channel
     *  `U`: Channel occupancy indicator
     *
     * The channel occupancy value represents the average rate/percentage of
     * RSSI samples that were above RSSI threshold ("bad" RSSI samples) within
     * (approximately) sample window latest RSSI samples.
     *
     * Max value of `0xffff` indicates all RSSI samples were above RSSI
     * threshold (i.e. 100% of samples were "bad").
     *
     */
    SPINEL_PROP_CHANNEL_MONITOR_CHANNEL_OCCUPANCY
                                        = SPINEL_PROP_PHY_EXT__BEGIN + 10,

    SPINEL_PROP_PHY_EXT__END            = 0x1300,

    SPINEL_PROP_MAC__BEGIN              = 0x30,
    SPINEL_PROP_MAC_SCAN_STATE          = SPINEL_PROP_MAC__BEGIN + 0,  ///< [C]
    SPINEL_PROP_MAC_SCAN_MASK           = SPINEL_PROP_MAC__BEGIN + 1,  ///< [A(C)]
    SPINEL_PROP_MAC_SCAN_PERIOD         = SPINEL_PROP_MAC__BEGIN + 2,  ///< ms-per-channel [S]
    SPINEL_PROP_MAC_SCAN_BEACON         = SPINEL_PROP_MAC__BEGIN + 3,  ///< chan,rssi,mac_data,net_data [CcdD]
    SPINEL_PROP_MAC_15_4_LADDR          = SPINEL_PROP_MAC__BEGIN + 4,  ///< [E]
    SPINEL_PROP_MAC_15_4_SADDR          = SPINEL_PROP_MAC__BEGIN + 5,  ///< [S]
    SPINEL_PROP_MAC_15_4_PANID          = SPINEL_PROP_MAC__BEGIN + 6,  ///< [S]
    SPINEL_PROP_MAC_RAW_STREAM_ENABLED  = SPINEL_PROP_MAC__BEGIN + 7,  ///< [C]
    SPINEL_PROP_MAC_PROMISCUOUS_MODE    = SPINEL_PROP_MAC__BEGIN + 8,  ///< [C]
    SPINEL_PROP_MAC_ENERGY_SCAN_RESULT  = SPINEL_PROP_MAC__BEGIN + 9,  ///< chan,maxRssi [Cc]
    SPINEL_PROP_MAC_DATA_POLL_PERIOD    = SPINEL_PROP_MAC__BEGIN + 10, ///< pollPeriod (in ms) [L]
    SPINEL_PROP_MAC__END                = 0x40,

    SPINEL_PROP_MAC_EXT__BEGIN          = 0x1300,
    /// MAC Whitelist
    /** Format: `A(t(Ec))`
     *
     * Structure Parameters:
     *
     * * `E`: EUI64 address of node
     * * `c`: Optional fixed RSSI. 127 means not set.
     */
    SPINEL_PROP_MAC_WHITELIST           = SPINEL_PROP_MAC_EXT__BEGIN + 0,

    /// MAC Whitelist Enabled Flag
    /** Format: `b`
     */
    SPINEL_PROP_MAC_WHITELIST_ENABLED   = SPINEL_PROP_MAC_EXT__BEGIN + 1,

    /// MAC Extended Address
    /** Format: `E`
     *
     *  Specified by Thread. Randomly-chosen, but non-volatile EUI-64.
     */
    SPINEL_PROP_MAC_EXTENDED_ADDR       = SPINEL_PROP_MAC_EXT__BEGIN + 2,

    /// MAC Source Match Enabled Flag
    /** Format: `b`
     */
    SPINEL_PROP_MAC_SRC_MATCH_ENABLED   = SPINEL_PROP_MAC_EXT__BEGIN + 3,

    /// MAC Source Match Short Address List
    /** Format: `A(S)`
     */
    SPINEL_PROP_MAC_SRC_MATCH_SHORT_ADDRESSES
                                        = SPINEL_PROP_MAC_EXT__BEGIN + 4,

    /// MAC Source Match Extended Address List
    /** Format: `A(E)`
     */
    SPINEL_PROP_MAC_SRC_MATCH_EXTENDED_ADDRESSES
                                        = SPINEL_PROP_MAC_EXT__BEGIN + 5,

    /// MAC Blacklist
    /** Format: `A(t(E))`
     *
     * Structure Parameters:
     *
     * * `E`: EUI64 address of node
     */
    SPINEL_PROP_MAC_BLACKLIST           = SPINEL_PROP_MAC_EXT__BEGIN + 6,

    /// MAC Blacklist Enabled Flag
    /** Format: `b`
     */
    SPINEL_PROP_MAC_BLACKLIST_ENABLED   = SPINEL_PROP_MAC_EXT__BEGIN + 7,

    /// MAC Received Signal Strength Filter
    /** Format: `A(t(Ec))`
     *
     * Structure Parameters:
     *
     * * `E`: Optional EUI64 address of node. Set default RSS if not included.
     * * `c`: Fixed RSS. OT_MAC_FILTER_FIXED_RSS_OVERRIDE_DISABLED(127) means not set.
     */
    SPINEL_PROP_MAC_FIXED_RSS           = SPINEL_PROP_MAC_EXT__BEGIN + 8,

    /// The CCA failure rate
    /** Format: `S`
     *
     * This property provides the current CCA (Clear Channel Assessment) failure rate.
     *
     * Maximum value `0xffff` corresponding to 100% failure rate.
     *
     */
    SPINEL_PROP_MAC_CCA_FAILURE_RATE    = SPINEL_PROP_MAC_EXT__BEGIN + 9,

    SPINEL_PROP_MAC_EXT__END            = 0x1400,

    SPINEL_PROP_NET__BEGIN              = 0x40,
    SPINEL_PROP_NET_SAVED               = SPINEL_PROP_NET__BEGIN + 0, ///< [b]
    SPINEL_PROP_NET_IF_UP               = SPINEL_PROP_NET__BEGIN + 1, ///< [b]
    SPINEL_PROP_NET_STACK_UP            = SPINEL_PROP_NET__BEGIN + 2, ///< [b]
    SPINEL_PROP_NET_ROLE                = SPINEL_PROP_NET__BEGIN + 3, ///< [C]
    SPINEL_PROP_NET_NETWORK_NAME        = SPINEL_PROP_NET__BEGIN + 4, ///< [U]
    SPINEL_PROP_NET_XPANID              = SPINEL_PROP_NET__BEGIN + 5, ///< [D]
    SPINEL_PROP_NET_MASTER_KEY          = SPINEL_PROP_NET__BEGIN + 6, ///< [D]
    SPINEL_PROP_NET_KEY_SEQUENCE_COUNTER
                                        = SPINEL_PROP_NET__BEGIN + 7, ///< [L]
    SPINEL_PROP_NET_PARTITION_ID        = SPINEL_PROP_NET__BEGIN + 8, ///< [L]

    /// Require Join Existing
    /** Format: `b`
     *  Default Value: `false`
     *
     * This flag is typically used for nodes that are associating with an
     * existing network for the first time. If this is set to `true` before
     * `PROP_NET_STACK_UP` is set to `true`, the
     * creation of a new partition at association is prevented. If the node
     * cannot associate with an existing partition, `PROP_LAST_STATUS` will
     * emit a status that indicates why the association failed and
     * `PROP_NET_STACK_UP` will automatically revert to `false`.
     *
     * Once associated with an existing partition, this flag automatically
     * reverts to `false`.
     *
     * The behavior of this property being set to `true` when
     * `PROP_NET_STACK_UP` is already set to `true` is undefined.
     */
    SPINEL_PROP_NET_REQUIRE_JOIN_EXISTING
                                        = SPINEL_PROP_NET__BEGIN + 9,

    SPINEL_PROP_NET_KEY_SWITCH_GUARDTIME
                                        = SPINEL_PROP_NET__BEGIN + 10, ///< [L]

    SPINEL_PROP_NET_PSKC                = SPINEL_PROP_NET__BEGIN + 11, ///< [D]

    SPINEL_PROP_NET__END                = 0x50,

    SPINEL_PROP_THREAD__BEGIN           = 0x50,
    SPINEL_PROP_THREAD_LEADER_ADDR      = SPINEL_PROP_THREAD__BEGIN + 0, ///< [6]
    SPINEL_PROP_THREAD_PARENT           = SPINEL_PROP_THREAD__BEGIN + 1, ///< LADDR, SADDR [ES]

    /// Thread Child Table
    /** Format: [A(t(ESLLCCcCc)] - Read only
     *
     * Data per item is:
     *
     *  `E`: Extended address
     *  `S`: RLOC16
     *  `L`: Timeout (in seconds)
     *  `L`: Age (in seconds)
     *  `L`: Network Data version
     *  `C`: Link Quality In
     *  `c`: Average RSS (in dBm)
     *  `C`: Mode (bit-flags)
     *  `c`: Last RSSI (in dBm)
     *
     */
    SPINEL_PROP_THREAD_CHILD_TABLE      = SPINEL_PROP_THREAD__BEGIN + 2,
    SPINEL_PROP_THREAD_LEADER_RID       = SPINEL_PROP_THREAD__BEGIN + 3, ///< [C]
    SPINEL_PROP_THREAD_LEADER_WEIGHT    = SPINEL_PROP_THREAD__BEGIN + 4, ///< [C]
    SPINEL_PROP_THREAD_LOCAL_LEADER_WEIGHT
                                        = SPINEL_PROP_THREAD__BEGIN + 5, ///< [C]
    SPINEL_PROP_THREAD_NETWORK_DATA     = SPINEL_PROP_THREAD__BEGIN + 6, ///< [D]
    SPINEL_PROP_THREAD_NETWORK_DATA_VERSION
                                        = SPINEL_PROP_THREAD__BEGIN + 7, ///< [S]
    SPINEL_PROP_THREAD_STABLE_NETWORK_DATA
                                        = SPINEL_PROP_THREAD__BEGIN + 8, ///< [D]
    SPINEL_PROP_THREAD_STABLE_NETWORK_DATA_VERSION
                                        = SPINEL_PROP_THREAD__BEGIN + 9,  ///< [S]

    /// On-Mesh Prefixes
    /** Format: `A(t(6CbCbS))`
     *
     * Data per item is:
     *
     *  `6`: IPv6 Prefix
     *  `C`: Prefix length in bits
     *  `b`: Stable flag
     *  `C`: TLV flags
     *  `b`: "Is defined locally" flag. Set if this network was locally
     *       defined. Assumed to be true for set, insert and replace. Clear if
     *       the on mesh network was defined by another node.
     *  `S`: The RLOC16 of the device that registered this on-mesh prefix entry.
     *       This value is not used and ignored when adding an on-mesh prefix.
     *
     */
    SPINEL_PROP_THREAD_ON_MESH_NETS     = SPINEL_PROP_THREAD__BEGIN + 10,

    /// Off-mesh routes
    /** Format: [A(t(6CbCbb))]
     *
     * Data per item is:
     *
     *  `6`: Route Prefix
     *  `C`: Prefix length in bits
     *  `b`: Stable flag
     *  `C`: Route preference flags
     *  `b`: "Is defined locally" flag. Set if this route info was locally
     *       defined as part of local network data. Assumed to be true for set,
     *       insert and replace. Clear if the route is part of partition's network
     *       data.
     *  `b`: "Next hop is this device" flag. Set if the next hop for the
     *       route is this device itself (i.e., route was added by this device)
     *       This value is ignored when adding an external route. For any added
     *       route the next hop is this device.
     *  `S`: The RLOC16 of the device that registered this route entry.
     *       This value is not used and ignored when adding a route.
     *
     */
    SPINEL_PROP_THREAD_OFF_MESH_ROUTES  = SPINEL_PROP_THREAD__BEGIN + 11,

    SPINEL_PROP_THREAD_ASSISTING_PORTS  = SPINEL_PROP_THREAD__BEGIN + 12, ///< array(portn) [A(S)]
    SPINEL_PROP_THREAD_ALLOW_LOCAL_NET_DATA_CHANGE
                                        = SPINEL_PROP_THREAD__BEGIN + 13, ///< [b]

    /// Thread Mode
    /** Format: `C`
     *
     *  This property contains the value of the mode
     *  TLV for this node. The meaning of the bits in this
     *  bitfield are defined by section 4.5.2 of the Thread
     *  specification.
     */
    SPINEL_PROP_THREAD_MODE             = SPINEL_PROP_THREAD__BEGIN + 14,
    SPINEL_PROP_THREAD__END             = 0x60,

    SPINEL_PROP_THREAD_EXT__BEGIN       = 0x1500,

    /// Thread Child Timeout
    /** Format: `L`
     *
     *  Used when operating in the Child role.
     */
    SPINEL_PROP_THREAD_CHILD_TIMEOUT    = SPINEL_PROP_THREAD_EXT__BEGIN + 0,

    /// Thread RLOC16
    /** Format: `S`
     */
    SPINEL_PROP_THREAD_RLOC16           = SPINEL_PROP_THREAD_EXT__BEGIN + 1,

    /// Thread Router Upgrade Threshold
    /** Format: `C`
     */
    SPINEL_PROP_THREAD_ROUTER_UPGRADE_THRESHOLD
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 2,

    /// Thread Context Reuse Delay
    /** Format: `L`
     */
    SPINEL_PROP_THREAD_CONTEXT_REUSE_DELAY
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 3,

    /// Thread Network ID Timeout
    /** Format: `C`
     */
    SPINEL_PROP_THREAD_NETWORK_ID_TIMEOUT
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 4,

    /// List of active thread router ids
    /** Format: `A(C)`
     *
     * Note that some implementations may not support CMD_GET_VALUE
     * routerids, but may support CMD_REMOVE_VALUE when the node is
     * a leader.
     */
    SPINEL_PROP_THREAD_ACTIVE_ROUTER_IDS
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 5,

    /// Forward IPv6 packets that use RLOC16 addresses to HOST.
    /** Format: `b`
     */
    SPINEL_PROP_THREAD_RLOC16_DEBUG_PASSTHRU
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 6,

    /// This property indicates whether or not the `Router Role` is enabled.
    /** Format `b`
     */
    SPINEL_PROP_THREAD_ROUTER_ROLE_ENABLED
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 7,

    /// Thread Router Downgrade Threshold
    /** Format: `C`
     */
    SPINEL_PROP_THREAD_ROUTER_DOWNGRADE_THRESHOLD
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 8,

    /// Thread Router Selection Jitter
    /** Format: `C`
     */
    SPINEL_PROP_THREAD_ROUTER_SELECTION_JITTER
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 9,

    /// Thread Preferred Router Id
    /** Format: `C` - Write only
     */
    SPINEL_PROP_THREAD_PREFERRED_ROUTER_ID
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 10,

    /// Thread Neighbor Table
    /** Format: `A(t(ESLCcCbLLc))` - Read only
     *
     * Data per item is:
     *
     *  `E`: Extended address
     *  `S`: RLOC16
     *  `L`: Age (in seconds)
     *  `C`: Link Quality In
     *  `c`: Average RSS (in dBm)
     *  `C`: Mode (bit-flags)
     *  `b`: `true` if neighbor is a child, `false` otherwise.
     *  `L`: Link Frame Counter
     *  `L`: MLE Frame Counter
     *  `c`: The last RSSI (in dBm)
     *
     */
    SPINEL_PROP_THREAD_NEIGHBOR_TABLE   = SPINEL_PROP_THREAD_EXT__BEGIN + 11,

    /// Thread Max Child Count
    /** Format: `C`
     */
    SPINEL_PROP_THREAD_CHILD_COUNT_MAX  = SPINEL_PROP_THREAD_EXT__BEGIN + 12,

    /// Leader network data
    /** Format: `D` - Read only
     */
    SPINEL_PROP_THREAD_LEADER_NETWORK_DATA
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 13,

    /// Stable leader network data
    /** Format: `D` - Read only
     */
    SPINEL_PROP_THREAD_STABLE_LEADER_NETWORK_DATA
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 14,

    /// Thread joiner data
    /** Format `A(T(ULE))`
    *  PSKd, joiner timeout, eui64 (optional)
    */
    SPINEL_PROP_THREAD_JOINERS          = SPINEL_PROP_THREAD_EXT__BEGIN + 15,

    /// Thread commissioner enable
    /** Format `b`
     *
     * Default value is `false`.
     */
    SPINEL_PROP_THREAD_COMMISSIONER_ENABLED
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 16,

    /// Thread TMF proxy enable
    /** Format `b`
     *
     * Default value is `false`.
     */
    SPINEL_PROP_THREAD_TMF_PROXY_ENABLED
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 17,

    /// Thread TMF proxy stream
    /** Format `dSS`
     */
    SPINEL_PROP_THREAD_TMF_PROXY_STREAM = SPINEL_PROP_THREAD_EXT__BEGIN + 18,

    /// Thread "joiner" flag used during discovery scan operation
    /** Format `b`
     *
     * This property defines the Joiner Flag value in the Discovery Request TLV.
     *
     * Default value is `false`.
     */
    SPINEL_PROP_THREAD_DISCOVERY_SCAN_JOINER_FLAG
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 19,

    /// Enable EUI64 filtering for discovery scan operation.
    /** Format `b`
     *
     * Default value is `false`
     */
    SPINEL_PROP_THREAD_DISCOVERY_SCAN_ENABLE_FILTERING
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 20,

    /// PANID used for Discovery scan operation (used for PANID filtering).
    /** Format: `S`
     *
     * Default value is 0xffff (Broadcast PAN) to disable PANID filtering
     *
     */
    SPINEL_PROP_THREAD_DISCOVERY_SCAN_PANID
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 21,

    /// Thread (out of band) steering data for MLE Discovery Response.
    /** Format `E` - Write only
     *
     * Required capability: SPINEL_CAP_OOB_STEERING_DATA.
     *
     * Writing to this property allows to set/update the MLE Discovery Response steering data out of band.
     *
     *  - All zeros to clear the steering data (indicating that there is no steering data).
     *  - All 0xFFs to set steering data/bloom filter to accept/allow all.
     *  - A specific EUI64 which is then added to current steering data/bloom filter.
     *
     */
    SPINEL_PROP_THREAD_STEERING_DATA    = SPINEL_PROP_THREAD_EXT__BEGIN + 22,

    /// Thread Router Table.
    /** Format: `A(t(ESCCCCCCb)` - Read only
     *
     * Data per item is:
     *
     *  `E`: IEEE 802.15.4 Extended Address
     *  `S`: RLOC16
     *  `C`: Router ID
     *  `C`: Next hop to router
     *  `C`: Path cost to router
     *  `C`: Link Quality In
     *  `C`: Link Quality Out
     *  `C`: Age (seconds since last heard)
     *  `b`: Link established with Router ID or not.
     *
     */
    SPINEL_PROP_THREAD_ROUTER_TABLE     = SPINEL_PROP_THREAD_EXT__BEGIN + 23,

    /// Thread Active Operational Dataset
    /** Format: `A(t(iD))` - Read-Write
     *
     * This property provides access to current Thread Active Operational Dataset. A Thread device maintains the
     * Operational Dataset that it has stored locally and the one currently in use by the partition to which it is
     * attached. This property corresponds to the locally stored Dataset on the device.
     *
     * Operational Dataset consists of a set of supported properties (e.g., channel, master key, network name, PAN id,
     * etc). Note that not all supported properties may be present (have a value) in a Dataset.
     *
     * The Dataset value is encoded as an array of structs containing pairs of property key (as `i`) followed by the
     * property value (as `D`). The property value must follow the format associated with the corresponding property.
     *
     * On write, any unknown/unsupported property keys must be ignored.
     *
     * The following properties can be included in a Dataset list:
     *
     *   SPINEL_PROP_DATASET_ACTIVE_TIMESTAMP
     *   SPINEL_PROP_PHY_CHAN
     *   SPINEL_PROP_PHY_CHAN_SUPPORTED (Channel Mask Page 0)
     *   SPINEL_PROP_NET_MASTER_KEY
     *   SPINEL_PROP_NET_NETWORK_NAME
     *   SPINEL_PROP_NET_XPANID
     *   SPINEL_PROP_MAC_15_4_PANID
     *   SPINEL_PROP_IPV6_ML_PREFIX
     *   SPINEL_PROP_NET_PSKC
     *   SPINEL_PROP_DATASET_SECURITY_POLICY
     *
     */
    SPINEL_PROP_THREAD_ACTIVE_DATASET   = SPINEL_PROP_THREAD_EXT__BEGIN + 24,

    /// Thread Pending Operational Dataset
    /** Format: `A(t(iD))` - Read-Write
     *
     * This property provide access to current locally stored Pending Operational Dataset.
     *
     * The formatting of this property follows the same rules as in SPINEL_PROP_THREAD_ACTIVE_DATASET.
     *
     * In addition supported properties in SPINEL_PROP_THREAD_ACTIVE_DATASET, the following properties can also
     * be included in the Pending Dataset:
     *
     *   SPINEL_PROP_DATASET_PENDING_TIMESTAMP
     *   SPINEL_PROP_DATASET_DELAY_TIMER
     *
     */
    SPINEL_PROP_THREAD_PENDING_DATASET  = SPINEL_PROP_THREAD_EXT__BEGIN + 25,

    /// Thread Active Operational Dataset (MGMT send)
    /** Format: `A(t(iD))` - Write only
     *
     * The formatting of this property follows the same rules as in SPINEL_PROP_THREAD_ACTIVE_DATASET.
     *
     * This is write-only property. When written, it triggers a MGMT_ACTIVE_SET meshcop command to be sent to leader
     * with the given Dataset. The spinel frame response should be a `LAST_STATUS` with the status of the transmission
     * of MGMT_ACTIVE_SET command.
     *
     * In addition to supported properties in SPINEL_PROP_THREAD_ACTIVE_DATASET, the following property can be
     * included in the Dataset (to allow for custom raw TLVs):
     *
     *    SPINEL_PROP_DATASET_RAW_TLVS
     *
     */
    SPINEL_PROP_THREAD_MGMT_ACTIVE_DATASET
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 26,

    /// Thread Pending Operational Dataset (MGMT send)
    /** Format: `A(t(iD))` - Write only
     *
     * This property is similar to SPINEL_PROP_THREAD_PENDING_DATASET and follows the same format and rules.
     *
     * In addition to supported properties in SPINEL_PROP_THREAD_PENDING_DATASET, the following property can be
     * included the Dataset (to allow for custom raw TLVs to be provided).
     *
     *    SPINEL_PROP_DATASET_RAW_TLVS
     *
     */
    SPINEL_PROP_THREAD_MGMT_PENDING_DATASET
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 27,

    /// Operational Dataset Active Timestamp
    /** Format: `X` - No direct read or write
     *
     * It can only be included in one of the Dataset related properties below:
     *
     *   SPINEL_PROP_THREAD_ACTIVE_DATASET
     *   SPINEL_PROP_THREAD_PENDING_DATASET
     *   SPINEL_PROP_THREAD_MGMT_ACTIVE_DATASET
     *   SPINEL_PROP_THREAD_MGMT_PENDING_DATASET
     *
     */
    SPINEL_PROP_DATASET_ACTIVE_TIMESTAMP
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 28,

    /// Operational Dataset Pending Timestamp
    /** Format: `X` - No direct read or write
     *
     * It can only be included in one of the Pending Dataset properties:
     *
     *   SPINEL_PROP_THREAD_PENDING_DATASET
     *   SPINEL_PROP_THREAD_MGMT_PENDING_DATASET
     *
     */
    SPINEL_PROP_DATASET_PENDING_TIMESTAMP
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 29,

    /// Operational Dataset Delay Timer
    /** Format: `L` - No direct read or write
     *
     * Delay timer (in ms) specifies the time renaming until Thread devices overwrite the value in the Active
     * Operational Dataset with the corresponding values in the Pending Operational Dataset.
     *
     * It can only be included in one of the Pending Dataset properties:
     *
     *   SPINEL_PROP_THREAD_PENDING_DATASET
     *   SPINEL_PROP_THREAD_MGMT_PENDING_DATASET
     *
     */
    SPINEL_PROP_DATASET_DELAY_TIMER     = SPINEL_PROP_THREAD_EXT__BEGIN + 30,

    /// Operational Dataset Security Policy
    /** Format: `SC` - No direct read or write
     *
     * It can only be included in one of the Dataset related properties below:
     *
     *   SPINEL_PROP_THREAD_ACTIVE_DATASET
     *   SPINEL_PROP_THREAD_PENDING_DATASET
     *   SPINEL_PROP_THREAD_MGMT_ACTIVE_DATASET
     *   SPINEL_PROP_THREAD_MGMT_PENDING_DATASET
     *
     * Content is
     *   `S` : Key Rotation Time (in units of hour)
     *   `C` : Security Policy Flags (as specified in Thread 1.1 Section 8.10.1.15)
     *
     */
    SPINEL_PROP_DATASET_SECURITY_POLICY = SPINEL_PROP_THREAD_EXT__BEGIN + 31,

    /// Operational Dataset Additional Raw TLVs
    /** Format: `D` - No direct read or write
     *
     * This property defines extra raw TLVs that can be added to an Operational DataSet.
     *
     * It can only be included in one of the following Dataset properties:
     *
     *   SPINEL_PROP_THREAD_MGMT_ACTIVE_DATASET
     *   SPINEL_PROP_THREAD_MGMT_PENDING_DATASET
     *
     */
    SPINEL_PROP_DATASET_RAW_TLVS        = SPINEL_PROP_THREAD_EXT__BEGIN + 32,

    /// Child table addresses
    /** Format: `A(t(ESA(6)))` - Read only
     *
     * This property provides the list of all addresses associated with every child
     * including any registered IPv6 addresses.
     *
     * Data per item is:
     *
     *  `E`: Extended address of the child
     *  `S`: RLOC16 of the child
     *  `A(6)`: List of IPv6 addresses registered by the child (if any)
     *
     */
    SPINEL_PROP_THREAD_CHILD_TABLE_ADDRESSES
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 33,

    /// Neighbor Table Frame and Message Error Rates
    /** Format: `A(t(ESSScc))`
     *  Required capability: `CAP_ERROR_RATE_TRACKING`
     *
     * This property provides link quality related info including
     * frame and (IPv6) message error rates for all neighbors.
     *
     * With regards to message error rate, note that a larger (IPv6)
     * message can be fragmented and sent as multiple MAC frames. The
     * message transmission is considered a failure, if any of its
     * fragments fail after all MAC retry attempts.
     *
     * Data per item is:
     *
     *  `E`: Extended address of the neighbor
     *  `S`: RLOC16 of the neighbor
     *  `S`: Frame error rate (0 -> 0%, 0xffff -> 100%)
     *  `S`: Message error rate (0 -> 0%, 0xffff -> 100%)
     *  `c`: Average RSSI (in dBm)
     *  `c`: Last RSSI (in dBm)
     *
     */
    SPINEL_PROP_THREAD_NEIGHBOR_TABLE_ERROR_RATES
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 34,

    /// EID (Endpoint Identifier) IPv6 Address Cache Table
    /** Format `A(t(6SC))`
     *
     * This property provides Thread EID address cache table.
     *
     * Data per item is:
     *
     *  `6` : Target IPv6 address
     *  `S` : RLOC16 of target
     *  `C` : Age (order of use, 0 indicates most recently used entry)
     *
     */
    SPINEL_PROP_THREAD_ADDRESS_CACHE_TABLE
                                        = SPINEL_PROP_THREAD_EXT__BEGIN + 35,

    SPINEL_PROP_THREAD_EXT__END         = 0x1600,

    SPINEL_PROP_IPV6__BEGIN             = 0x60,
    SPINEL_PROP_IPV6_LL_ADDR            = SPINEL_PROP_IPV6__BEGIN + 0, ///< [6]
    SPINEL_PROP_IPV6_ML_ADDR            = SPINEL_PROP_IPV6__BEGIN + 1, ///< [6C]
    SPINEL_PROP_IPV6_ML_PREFIX          = SPINEL_PROP_IPV6__BEGIN + 2, ///< [6C]

    /// IPv6 Address Table
    /** Format: `A(t(6CLLC))`
     *
     * This property provides all unicast addresses.
     *
     * Array of structures containing:
     *
     *  `6`: IPv6 Address
     *  `C`: Network Prefix Length
     *  `L`: Valid Lifetime
     *  `L`: Preferred Lifetime
     *  `C`: Flags
     *
     */
    SPINEL_PROP_IPV6_ADDRESS_TABLE      = SPINEL_PROP_IPV6__BEGIN + 3,

    SPINEL_PROP_IPV6_ROUTE_TABLE        = SPINEL_PROP_IPV6__BEGIN + 4, ///< array(ipv6prefix,prefixlen,iface,flags) [A(t(6CCC))]

    /// IPv6 ICMP Ping Offload
    /** Format: `b`
     *
     * Allow the NCP to directly respond to ICMP ping requests. If this is
     * turned on, ping request ICMP packets will not be passed to the host.
     *
     * Default value is `false`.
     */
    SPINEL_PROP_IPV6_ICMP_PING_OFFLOAD  = SPINEL_PROP_IPV6__BEGIN + 5, ///< [b]

    SPINEL_PROP_IPV6_MULTICAST_ADDRESS_TABLE
                                        = SPINEL_PROP_IPV6__BEGIN + 6, ///< [A(t(6))]

    /// IPv6 ICMP Ping Offload
    /** Format: `C`
     *
     * Allow the NCP to directly respond to ICMP ping requests. If this is
     * turned on, ping request ICMP packets will not be passed to the host.
     *
     * This property allows enabling responses sent to unicast only, multicast
     * only, or both.
     *
     * Default value is `NET_IPV6_ICMP_PING_OFFLOAD_DISABLED`.
     */
    SPINEL_PROP_IPV6_ICMP_PING_OFFLOAD_MODE
                                        = SPINEL_PROP_IPV6__BEGIN + 7, ///< [b]

    SPINEL_PROP_IPV6__END               = 0x70,

    SPINEL_PROP_STREAM__BEGIN           = 0x70,
    SPINEL_PROP_STREAM_DEBUG            = SPINEL_PROP_STREAM__BEGIN + 0, ///< [U]
    SPINEL_PROP_STREAM_RAW              = SPINEL_PROP_STREAM__BEGIN + 1, ///< [dD]
    SPINEL_PROP_STREAM_NET              = SPINEL_PROP_STREAM__BEGIN + 2, ///< [dD]
    SPINEL_PROP_STREAM_NET_INSECURE     = SPINEL_PROP_STREAM__BEGIN + 3, ///< [dD]

    /// Log Stream
    /** Format: `UD` (stream, read only)
     *
     * This property is a read-only streaming property which provides
     * formatted log string from NCP. This property provides asynchronous
     * `CMD_PROP_VALUE_IS` updates with a new log string and includes
     * optional meta data.
     *
     *   `U`: The log string
     *   `D`: Log metadata (optional).
     *
     * Any data after the log string is considered metadata and is OPTIONAL.
     * Pretense of `SPINEL_CAP_OPENTHREAD_LOG_METADATA` capability
     * indicates that OpenThread log metadata format is used as defined
     * below:
     *
     *    `C`: Log level (as per definition in enumeration
     *         `SPINEL_NCP_LOG_LEVEL_<level>`)
     *    `i`: OpenThread Log region (as per definition in enumeration
     *         `SPINEL_NCP_LOG_REGION_<region>).
     *
     */
    SPINEL_PROP_STREAM_LOG              = SPINEL_PROP_STREAM__BEGIN + 4,
    SPINEL_PROP_STREAM__END             = 0x80,

    SPINEL_PROP_OPENTHREAD__BEGIN       = 0x1900,

    /// Channel Manager - Channel Change New Channel
    /** Format: `C` (read-write)
     *
     * Required capability: SPINEL_CAP_CHANNEL_MANAGER
     *
     * Setting this property triggers the Channel Manager to start
     * a channel change process. The network switches to the given
     * channel after the specified delay (see `CHANNEL_MANAGER_DELAY`).
     *
     * A subsequent write to this property will cancel an ongoing
     * (previously requested) channel change.
     *
     */
    SPINEL_PROP_CHANNEL_MANAGER_NEW_CHANNEL
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 0,

    /// Channel Manager - Channel Change Delay
    /** Format 'S'
     *  Units: seconds
     *
     * Required capability: SPINEL_CAP_CHANNEL_MANAGER
     *
     * This property specifies the delay (in seconds) to be used for
     * a channel change request.
     *
     * The delay should preferably be longer than maximum data poll
     * interval used by all sleepy-end-devices within the Thread
     * network.
     *
     */
    SPINEL_PROP_CHANNEL_MANAGER_DELAY   = SPINEL_PROP_OPENTHREAD__BEGIN + 1,

    /// Channel Manager Supported Channels
    /** Format 'A(C)'
     *
     * Required capability: SPINEL_CAP_CHANNEL_MANAGER
     *
     * This property specifies the list of supported channels.
     *
     */
    SPINEL_PROP_CHANNEL_MANAGER_SUPPORTED_CHANNELS
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 2,

    /// Channel Manager Favored Channels
    /** Format 'A(C)'
     *
     * Required capability: SPINEL_CAP_CHANNEL_MANAGER
     *
     * This property specifies the list of favored channels (when `ChannelManager` is asked to select channel)
     *
     */
    SPINEL_PROP_CHANNEL_MANAGER_FAVORED_CHANNELS
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 3,

    /// Channel Manager Channel Select Trigger
    /** Format 'b'
     *
     * Required capability: SPINEL_CAP_CHANNEL_MANAGER
     *
     * Writing to this property triggers a request on `ChannelManager` to select a new channel.
     *
     * Once a Channel Select is triggered, the Channel Manager will perform the following 3 steps:
     *
     * 1) `ChannelManager` decides if the channel change would be helpful. This check can be skipped if in the input
     *    boolean to this property is set to `true` (skipping the quality check).
     *    This step uses the collected link quality metrics on the device such as CCA failure rate, frame and message
     *    error rates per neighbor, etc. to determine if the current channel quality is at the level that justifies
     *    a channel change.
     *
     * 2) If first step passes, then `ChannelManager` selects a potentially better channel. It uses the collected
     *    channel quality data by `ChannelMonitor` module. The supported and favored channels are used at this step.
     *
     * 3) If the newly selected channel is different from the current channel, `ChannelManager` requests/starts the
     *    channel change process.
     *
     * Reading this property always yields `false`.
     *
     */
    SPINEL_PROP_CHANNEL_MANAGER_CHANNEL_SELECT
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 4,

    /// Channel Manager Auto Channel Selection Enabled
    /** Format 'b'
     *
     * Required capability: SPINEL_CAP_CHANNEL_MANAGER
     *
     * This property indicates if auto-channel-selection functionality is enabled/disabled on `ChannelManager`.
     *
     * When enabled, `ChannelManager` will periodically checks and attempts to select a new channel. The period interval
     * is specified by `SPINEL_PROP_CHANNEL_MANAGER_AUTO_SELECT_INTERVAL`.
     *
     */
    SPINEL_PROP_CHANNEL_MANAGER_AUTO_SELECT_ENABLED
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 5,

    /// Channel Manager Auto Channel Selection Interval
    /** Format 'L'
     *  units: seconds
     *
     * Required capability: SPINEL_CAP_CHANNEL_MANAGER
     *
     * This property specifies the auto-channel-selection check interval (in seconds).
     *
     */
    SPINEL_PROP_CHANNEL_MANAGER_AUTO_SELECT_INTERVAL
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 6,

    /// Thread network time.
    /** Format: `Xc` - Read only
     *
     * Data per item is:
     *
     *  `X`: The Thread network time, in microseconds.
     *  `c`: Time synchronization status.
     *
     */
    SPINEL_PROP_THREAD_NETWORK_TIME
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 7,

    /// Thread time synchronization period
    /** Format: `S` - Read-Write
     *
     * Data per item is:
     *
     *  `S`: Time synchronization period, in seconds.
     *
     */
    SPINEL_PROP_TIME_SYNC_PERIOD
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 8,

    /// Thread Time synchronization XTAL accuracy threshold for Router
    /** Format: `S` - Read-Write
     *
     * Data per item is:
     *
     *  `S`: The XTAL accuracy threshold for Router, in PPM.
     *
     */
    SPINEL_PROP_TIME_SYNC_XTAL_THRESHOLD
                                        = SPINEL_PROP_OPENTHREAD__BEGIN + 9,


    SPINEL_PROP_OPENTHREAD__END         = 0x2000,

    /// UART Bitrate
    /** Format: `L`
     *
     *  If the NCP is using a UART to communicate with the host,
     *  this property allows the host to change the bitrate
     *  of the serial connection. The value encoding is `L`,
     *  which is a little-endian 32-bit unsigned integer.
     *  The host should not assume that all possible numeric values
     *  are supported.
     *
     *  If implemented by the NCP, this property should be persistent
     *  across software resets and forgotten upon hardware resets.
     *
     *  This property is only implemented when a UART is being
     *  used for Spinel. This property is optional.
     *
     *  When changing the bitrate, all frames will be received
     *  at the previous bitrate until the response frame to this command
     *  is received. Once a successful response frame is received by
     *  the host, all further frames will be transmitted at the new
     *  bitrate.
     */
    SPINEL_PROP_UART_BITRATE            = 0x100,

    /// UART Software Flow Control
    /** Format: `b`
     *
     *  If the NCP is using a UART to communicate with the host,
     *  this property allows the host to determine if software flow
     *  control (XON/XOFF style) should be used and (optionally) to
     *  turn it on or off.
     *
     *  This property is only implemented when a UART is being
     *  used for Spinel. This property is optional.
     */
    SPINEL_PROP_UART_XON_XOFF           = 0x101,

    SPINEL_PROP_15_4_PIB__BEGIN         = 1024,
    // For direct access to the 802.15.4 PID.
    // Individual registers are fetched using
    // `SPINEL_PROP_15_4_PIB__BEGIN+[PIB_IDENTIFIER]`
    // Only supported if SPINEL_CAP_15_4_PIB is set.
    //
    // For brevity, the entire 802.15.4 PIB space is
    // not defined here, but a few choice attributes
    // are defined for illustration and convenience.
    SPINEL_PROP_15_4_PIB_PHY_CHANNELS_SUPPORTED
                                        = SPINEL_PROP_15_4_PIB__BEGIN + 0x01, ///< [A(L)]
    SPINEL_PROP_15_4_PIB_MAC_PROMISCUOUS_MODE
                                        = SPINEL_PROP_15_4_PIB__BEGIN + 0x51, ///< [b]
    SPINEL_PROP_15_4_PIB_MAC_SECURITY_ENABLED
                                        = SPINEL_PROP_15_4_PIB__BEGIN + 0x5d, ///< [b]
    SPINEL_PROP_15_4_PIB__END           = 1280,

    SPINEL_PROP_CNTR__BEGIN             = 1280,

    /// Counter reset behavior
    /** Format: `C`
     *  Writing a '1' to this property will reset
     *  all of the counters to zero. */
    SPINEL_PROP_CNTR_RESET              = SPINEL_PROP_CNTR__BEGIN + 0,

    /// The total number of transmissions.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_TOTAL       = SPINEL_PROP_CNTR__BEGIN + 1,

    /// The number of transmissions with ack request.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_ACK_REQ     = SPINEL_PROP_CNTR__BEGIN + 2,

    /// The number of transmissions that were acked.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_ACKED       = SPINEL_PROP_CNTR__BEGIN + 3,

    /// The number of transmissions without ack request.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_NO_ACK_REQ  = SPINEL_PROP_CNTR__BEGIN + 4,

    /// The number of transmitted data.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_DATA        = SPINEL_PROP_CNTR__BEGIN + 5,

    /// The number of transmitted data poll.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_DATA_POLL   = SPINEL_PROP_CNTR__BEGIN + 6,

    /// The number of transmitted beacon.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_BEACON      = SPINEL_PROP_CNTR__BEGIN + 7,

    /// The number of transmitted beacon request.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_BEACON_REQ  = SPINEL_PROP_CNTR__BEGIN + 8,

    /// The number of transmitted other types of frames.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_OTHER       = SPINEL_PROP_CNTR__BEGIN + 9,

    /// The number of retransmission times.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_RETRY       = SPINEL_PROP_CNTR__BEGIN + 10,

    /// The number of CCA failure times.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_ERR_CCA         = SPINEL_PROP_CNTR__BEGIN + 11,

    /// The number of unicast packets transmitted.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_UNICAST     = SPINEL_PROP_CNTR__BEGIN + 12,

    /// The number of broadcast packets transmitted.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_PKT_BROADCAST   = SPINEL_PROP_CNTR__BEGIN + 13,

    /// The number of frame transmission failures due to abort error.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_ERR_ABORT       = SPINEL_PROP_CNTR__BEGIN + 14,

    /// The total number of received packets.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_TOTAL       = SPINEL_PROP_CNTR__BEGIN + 100,

    /// The number of received data.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_DATA        = SPINEL_PROP_CNTR__BEGIN + 101,

    /// The number of received data poll.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_DATA_POLL   = SPINEL_PROP_CNTR__BEGIN + 102,

    /// The number of received beacon.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_BEACON      = SPINEL_PROP_CNTR__BEGIN + 103,

    /// The number of received beacon request.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_BEACON_REQ  = SPINEL_PROP_CNTR__BEGIN + 104,

    /// The number of received other types of frames.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_OTHER       = SPINEL_PROP_CNTR__BEGIN + 105,

    /// The number of received packets filtered by whitelist.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_FILT_WL     = SPINEL_PROP_CNTR__BEGIN + 106,

    /// The number of received packets filtered by destination check.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_FILT_DA     = SPINEL_PROP_CNTR__BEGIN + 107,

    /// The number of received packets that are empty.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_ERR_EMPTY       = SPINEL_PROP_CNTR__BEGIN + 108,

    /// The number of received packets from an unknown neighbor.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_ERR_UKWN_NBR    = SPINEL_PROP_CNTR__BEGIN + 109,

    /// The number of received packets whose source address is invalid.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_ERR_NVLD_SADDR  = SPINEL_PROP_CNTR__BEGIN + 110,

    /// The number of received packets with a security error.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_ERR_SECURITY    = SPINEL_PROP_CNTR__BEGIN + 111,

    /// The number of received packets with a checksum error.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_ERR_BAD_FCS     = SPINEL_PROP_CNTR__BEGIN + 112,

    /// The number of received packets with other errors.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_ERR_OTHER       = SPINEL_PROP_CNTR__BEGIN + 113,

    /// The number of received duplicated.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_DUP         = SPINEL_PROP_CNTR__BEGIN + 114,

    /// The number of unicast packets received.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_UNICAST     = SPINEL_PROP_CNTR__BEGIN + 115,

    /// The number of broadcast packets received.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_PKT_BROADCAST   = SPINEL_PROP_CNTR__BEGIN + 116,

    /// The total number of secure transmitted IP messages.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_IP_SEC_TOTAL    = SPINEL_PROP_CNTR__BEGIN + 200,

    /// The total number of insecure transmitted IP messages.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_IP_INSEC_TOTAL  = SPINEL_PROP_CNTR__BEGIN + 201,

    /// The number of dropped (not transmitted) IP messages.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_IP_DROPPED      = SPINEL_PROP_CNTR__BEGIN + 202,

    /// The total number of secure received IP message.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_IP_SEC_TOTAL    = SPINEL_PROP_CNTR__BEGIN + 203,

    /// The total number of insecure received IP message.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_IP_INSEC_TOTAL  = SPINEL_PROP_CNTR__BEGIN + 204,

    /// The number of dropped received IP messages.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_IP_DROPPED      = SPINEL_PROP_CNTR__BEGIN + 205,

    /// The number of transmitted spinel frames.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_TX_SPINEL_TOTAL    = SPINEL_PROP_CNTR__BEGIN + 300,

    /// The number of received spinel frames.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_SPINEL_TOTAL    = SPINEL_PROP_CNTR__BEGIN + 301,

    /// The number of received spinel frames with error.
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_SPINEL_ERR      = SPINEL_PROP_CNTR__BEGIN + 302,

    /// Number of out of order received spinel frames (tid increase by more than 1).
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_RX_SPINEL_OUT_OF_ORDER_TID
                                        = SPINEL_PROP_CNTR__BEGIN + 303,

    /// The number of successful Tx IP packets
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_IP_TX_SUCCESS      = SPINEL_PROP_CNTR__BEGIN + 304,

    /// The number of successful Rx IP packets
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_IP_RX_SUCCESS      = SPINEL_PROP_CNTR__BEGIN + 305,

    /// The number of failed Tx IP packets
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_IP_TX_FAILURE      = SPINEL_PROP_CNTR__BEGIN + 306,

    /// The number of failed Rx IP packets
    /** Format: `L` (Read-only) */
    SPINEL_PROP_CNTR_IP_RX_FAILURE      = SPINEL_PROP_CNTR__BEGIN + 307,

    /// The message buffer counter info
    /** Format: `SSSSSSSSSSSSSSSS` (Read-only)
     *      `S`, (TotalBuffers)           The number of buffers in the pool.
     *      `S`, (FreeBuffers)            The number of free message buffers.
     *      `S`, (6loSendMessages)        The number of messages in the 6lo send queue.
     *      `S`, (6loSendBuffers)         The number of buffers in the 6lo send queue.
     *      `S`, (6loReassemblyMessages)  The number of messages in the 6LoWPAN reassembly queue.
     *      `S`, (6loReassemblyBuffers)   The number of buffers in the 6LoWPAN reassembly queue.
     *      `S`, (Ip6Messages)            The number of messages in the IPv6 send queue.
     *      `S`, (Ip6Buffers)             The number of buffers in the IPv6 send queue.
     *      `S`, (MplMessages)            The number of messages in the MPL send queue.
     *      `S`, (MplBuffers)             The number of buffers in the MPL send queue.
     *      `S`, (MleMessages)            The number of messages in the MLE send queue.
     *      `S`, (MleBuffers)             The number of buffers in the MLE send queue.
     *      `S`, (ArpMessages)            The number of messages in the ARP send queue.
     *      `S`, (ArpBuffers)             The number of buffers in the ARP send queue.
     *      `S`, (CoapMessages)           The number of messages in the CoAP send queue.
     *      `S`, (CoapBuffers)            The number of buffers in the CoAP send queue.
     */
    SPINEL_PROP_MSG_BUFFER_COUNTERS     = SPINEL_PROP_CNTR__BEGIN + 400,

    /// All MAC related counters.
    /** Format: t(A(L))t(A(L))  (Read-only)
     *
     * The contents include two structs, first one corresponds to
     * all transmit related MAC counters, second one provides the
     * receive related counters.
     *
     * The transmit structure includes:
     *
     *   'L': TxTotal              (The total number of transmissions).
     *   'L': TxUnicast            (The total number of unicast transmissions).
     *   'L': TxBroadcast          (The total number of broadcast transmissions).
     *   'L': TxAckRequested       (The number of transmissions with ack request).
     *   'L': TxAcked              (The number of transmissions that were acked).
     *   'L': TxNoAckRequested     (The number of transmissions without ack request).
     *   'L': TxData               (The number of transmitted data).
     *   'L': TxDataPoll           (The number of transmitted data poll).
     *   'L': TxBeacon             (The number of transmitted beacon).
     *   'L': TxBeaconRequest      (The number of transmitted beacon request).
     *   'L': TxOther              (The number of transmitted other types of frames).
     *   'L': TxRetry              (The number of retransmission times).
     *   'L': TxErrCca             (The number of CCA failure times).
     *   'L': TxErrAbort           (The number of frame transmission failures due to abort error).
     *   'L': TxErrBusyChannel     (The number of frames that were dropped due to a busy channel).
     *
     * The receive structure includes:
     *
     *   'L': RxTotal              (The total number of received packets).
     *   'L': RxUnicast            (The total number of unicast packets received).
     *   'L': RxBroadcast          (The total number of broadcast packets received).
     *   'L': RxData               (The number of received data).
     *   'L': RxDataPoll           (The number of received data poll).
     *   'L': RxBeacon             (The number of received beacon).
     *   'L': RxBeaconRequest      (The number of received beacon request).
     *   'L': RxOther              (The number of received other types of frames).
     *   'L': RxAddressFiltered    (The number of received packets filtered by address filter (whitelist or blacklist)).
     *   'L': RxDestAddrFiltered   (The number of received packets filtered by destination check).
     *   'L': RxDuplicated         (The number of received duplicated packets).
     *   'L': RxErrNoFrame         (The number of received packets with no or malformed content).
     *   'L': RxErrUnknownNeighbor (The number of received packets from unknown neighbor).
     *   'L': RxErrInvalidSrcAddr  (The number of received packets whose source address is invalid).
     *   'L': RxErrSec             (The number of received packets with security error).
     *   'L': RxErrFcs             (The number of received packets with FCS error).
     *   'L': RxErrOther           (The number of received packets with other error).
     */
    SPINEL_PROP_CNTR_ALL_MAC_COUNTERS   =  SPINEL_PROP_CNTR__BEGIN + 401,

    SPINEL_PROP_CNTR__END               = 2048,

    SPINEL_PROP_NEST__BEGIN             = 15296,
    SPINEL_PROP_NEST_STREAM_MFG         = SPINEL_PROP_NEST__BEGIN + 0,

    /// The legacy network ULA prefix (8 bytes)
    /** Format: 'D' */
    SPINEL_PROP_NEST_LEGACY_ULA_PREFIX  = SPINEL_PROP_NEST__BEGIN + 1,

    /// The EUI64 of last node joined using legacy protocol (if none, all zero EUI64 is returned).
    /** Format: 'E' */
    SPINEL_PROP_NEST_LEGACY_LAST_NODE_JOINED
                                        = SPINEL_PROP_NEST__BEGIN + 2,

    SPINEL_PROP_NEST__END               = 15360,

    SPINEL_PROP_VENDOR__BEGIN           = 15360,
    SPINEL_PROP_VENDOR__END             = 16384,

    SPINEL_PROP_DEBUG__BEGIN            = 16384,

    /// Testing platform assert
    /** Format: 'b' (read-only)
     *
     * Reading this property will cause an assert on the NCP. This is intended for testing the assert functionality of
     * underlying platform/NCP. Assert should ideally cause the NCP to reset, but if this is not supported a `false`
     * boolean is returned in response.
     *
     */
    SPINEL_PROP_DEBUG_TEST_ASSERT       = SPINEL_PROP_DEBUG__BEGIN + 0,

    /// The NCP log level.
    /** Format: `C` */
    SPINEL_PROP_DEBUG_NCP_LOG_LEVEL     = SPINEL_PROP_DEBUG__BEGIN + 1,

    /// Testing platform watchdog
    /** Format: Empty  (read-only)
     *
     * Reading this property will causes NCP to start a `while(true) ;` loop and thus triggering a watchdog.
     *
     * This is intended for testing the watchdog functionality on the underlying platform/NCP.
     *
     */
    SPINEL_PROP_DEBUG_TEST_WATCHDOG     = SPINEL_PROP_DEBUG__BEGIN + 2,

    SPINEL_PROP_DEBUG__END              = 17408,

    SPINEL_PROP_EXPERIMENTAL__BEGIN     = 2000000,
    SPINEL_PROP_EXPERIMENTAL__END       = 2097152,
} spinel_prop_key_t;

// ----------------------------------------------------------------------------

#define SPINEL_HEADER_FLAG              0x80

#define SPINEL_HEADER_TID_SHIFT         0
#define SPINEL_HEADER_TID_MASK          (15 << SPINEL_HEADER_TID_SHIFT)

#define SPINEL_HEADER_IID_SHIFT         4
#define SPINEL_HEADER_IID_MASK          (3 << SPINEL_HEADER_IID_SHIFT)

#define SPINEL_HEADER_IID_0             (0 << SPINEL_HEADER_IID_SHIFT)
#define SPINEL_HEADER_IID_1             (1 << SPINEL_HEADER_IID_SHIFT)
#define SPINEL_HEADER_IID_2             (2 << SPINEL_HEADER_IID_SHIFT)
#define SPINEL_HEADER_IID_3             (3 << SPINEL_HEADER_IID_SHIFT)

#define SPINEL_HEADER_GET_IID(x)        (((x) & SPINEL_HEADER_IID_MASK) >> SPINEL_HEADER_IID_SHIFT)
#define SPINEL_HEADER_GET_TID(x)        (spinel_tid_t)(((x)&SPINEL_HEADER_TID_MASK)>>SPINEL_HEADER_TID_SHIFT)

#define SPINEL_GET_NEXT_TID(x)          (spinel_tid_t)((x)>=0xF?1:(x)+1)

#define SPINEL_BEACON_THREAD_FLAG_VERSION_SHIFT                                                         \
                                        4

#define SPINEL_BEACON_THREAD_FLAG_VERSION_MASK                                                          \
                                        (0xf << SPINEL_BEACON_THREAD_FLAG_VERSION_SHIFT)

#define SPINEL_BEACON_THREAD_FLAG_JOINABLE                                                              \
                                        (1 << 0)

#define SPINEL_BEACON_THREAD_FLAG_NATIVE                                                                \
                                       (1 << 3)

// ----------------------------------------------------------------------------

enum
{
    SPINEL_DATATYPE_NULL_C              = 0,
    SPINEL_DATATYPE_VOID_C              = '.',
    SPINEL_DATATYPE_BOOL_C              = 'b',
    SPINEL_DATATYPE_UINT8_C             = 'C',
    SPINEL_DATATYPE_INT8_C              = 'c',
    SPINEL_DATATYPE_UINT16_C            = 'S',
    SPINEL_DATATYPE_INT16_C             = 's',
    SPINEL_DATATYPE_UINT32_C            = 'L',
    SPINEL_DATATYPE_INT32_C             = 'l',
    SPINEL_DATATYPE_UINT64_C            = 'X',
    SPINEL_DATATYPE_INT64_C             = 'x',
    SPINEL_DATATYPE_UINT_PACKED_C       = 'i',
    SPINEL_DATATYPE_IPv6ADDR_C          = '6',
    SPINEL_DATATYPE_EUI64_C             = 'E',
    SPINEL_DATATYPE_EUI48_C             = 'e',
    SPINEL_DATATYPE_DATA_WLEN_C         = 'd',
    SPINEL_DATATYPE_DATA_C              = 'D',
    SPINEL_DATATYPE_UTF8_C              = 'U', //!< Zero-Terminated UTF8-Encoded String
    SPINEL_DATATYPE_STRUCT_C            = 't',
    SPINEL_DATATYPE_ARRAY_C             = 'A',
};

typedef char spinel_datatype_t;

#define SPINEL_DATATYPE_NULL_S          ""
#define SPINEL_DATATYPE_VOID_S          "."
#define SPINEL_DATATYPE_BOOL_S          "b"
#define SPINEL_DATATYPE_UINT8_S         "C"
#define SPINEL_DATATYPE_INT8_S          "c"
#define SPINEL_DATATYPE_UINT16_S        "S"
#define SPINEL_DATATYPE_INT16_S         "s"
#define SPINEL_DATATYPE_UINT32_S        "L"
#define SPINEL_DATATYPE_INT32_S         "l"
#define SPINEL_DATATYPE_UINT64_S        "X"
#define SPINEL_DATATYPE_INT64_S         "x"
#define SPINEL_DATATYPE_UINT_PACKED_S   "i"
#define SPINEL_DATATYPE_IPv6ADDR_S      "6"
#define SPINEL_DATATYPE_EUI64_S         "E"
#define SPINEL_DATATYPE_EUI48_S         "e"
#define SPINEL_DATATYPE_DATA_WLEN_S     "d"
#define SPINEL_DATATYPE_DATA_S          "D"
#define SPINEL_DATATYPE_UTF8_S          "U" //!< Zero-Terminated UTF8-Encoded String

#define SPINEL_DATATYPE_ARRAY_S(x)      "A(" x ")"
#define SPINEL_DATATYPE_STRUCT_S(x)     "t(" x ")"

#define SPINEL_DATATYPE_ARRAY_STRUCT_S(x)                                                               \
                                        SPINEL_DATATYPE_ARRAY_S(SPINEL_DATATYPE_STRUCT_WLEN_S(x))

#define SPINEL_DATATYPE_COMMAND_S       SPINEL_DATATYPE_UINT8_S       /* header  */                     \
                                        SPINEL_DATATYPE_UINT_PACKED_S /* command */

#define SPINEL_DATATYPE_COMMAND_PROP_S  SPINEL_DATATYPE_COMMAND_S     /* prop command  */               \
                                        SPINEL_DATATYPE_UINT_PACKED_S /* property id */

#define SPINEL_DATATYPE_MAC_SCAN_RESULT_S(mac_format_str, net_format_str)                               \
                                        SPINEL_DATATYPE_UINT8_S /* Channel */                           \
                                        SPINEL_DATATYPE_INT8_S  /* RSSI */                              \
                                        SPINEL_DATATYPE_STRUCT_S(mac_format_str) /* mac-layer data */   \
                                        SPINEL_DATATYPE_STRUCT_S(net_format_str) /* net-layer data */

#define SPINEL_802_15_4_DATATYPE_MAC_SCAN_RESULT_V1_S                                                   \
                                        SPINEL_DATATYPE_EUI64_S  /* laddr */                            \
                                        SPINEL_DATATYPE_UINT16_S /* saddr */                            \
                                        SPINEL_DATATYPE_UINT16_S /* panid */                            \
                                        SPINEL_DATATYPE_UINT8_S  /* lqi   */

#define SPINEL_NET_DATATYPE_MAC_SCAN_RESULT_V1_S                                                        \
                                        SPINEL_DATATYPE_UINT_PACKED_S /* type */                        \
                                        SPINEL_DATATYPE_UINT8_S /* flags */                             \
                                        SPINEL_DATATYPE_UTF8_S /* network name */                       \
                                        SPINEL_DATATYPE_DATA_WLEN_S /* xpanid */

#define SPINEL_NET_DATATYPE_MAC_SCAN_RESULT_V2_S                                                        \
                                        SPINEL_NET_DATATYPE_MAC_SCAN_RESULT_V1_S                        \
                                        SPINEL_DATATYPE_DATA_WLEN_S /* steering data */


#define SPINEL_MAX_UINT_PACKED          2097151

SPINEL_API_EXTERN spinel_ssize_t spinel_datatype_pack(uint8_t *data_out, spinel_size_t data_len,
                                                      const char *pack_format, ...);
SPINEL_API_EXTERN spinel_ssize_t spinel_datatype_vpack(uint8_t *data_out, spinel_size_t data_len,
                                                       const char *pack_format, va_list args);
SPINEL_API_EXTERN spinel_ssize_t spinel_datatype_unpack(const uint8_t *data_in, spinel_size_t data_len,
                                                        const char *pack_format, ...);
/**
 * This function parses spinel data similar to sscanf().
 *
 * This function actually calls spinel_datatype_vunpack_in_place() to parse data.
 *
 * @param[in]   data_in     A pointer to the data to parse.
 * @param[in]   data_len    The length of @p data_in in bytes.
 * @param[in]   pack_format C string that contains a format string follows the same specification of spinel.
 * @param[in]   ...         Additional arguments depending on the format string @p pack_format.
 *
 * @returns The parsed length in bytes.
 *
 * @note This function behaves different from `spinel_datatype_unpack()`:
 *       - This function expects composite data arguments of pointer to data type, while `spinel_datatype_unpack()`
 *         expects them of pointer to data type pointer. For example, if `SPINEL_DATATYPE_EUI64_C` is present in
 *         @p pack_format, this function expects a `spinel_eui64_t *` is included in variable arguments, while
 *         `spinel_datatype_unpack()` expects a `spinel_eui64_t **` is included.
 *       - For `SPINEL_DATATYPE_UTF8_C`, this function expects two arguments, the first of type `char *` and the
 *         second is of type `size_t` to indicate length of the provided buffer in the first argument just like
 *         `strncpy()`, while `spinel_datatype_unpack()` only expects a `const char **`.
 *
 * @sa spinel_datatype_vunpack_in_place()
 *
 */
SPINEL_API_EXTERN spinel_ssize_t spinel_datatype_unpack_in_place(const uint8_t *data_in, spinel_size_t data_len,
                                                        const char *pack_format, ...);
SPINEL_API_EXTERN spinel_ssize_t spinel_datatype_vunpack(const uint8_t *data_in, spinel_size_t data_len,
                                                         const char *pack_format, va_list args);
/**
 * This function parses spinel data similar to vsscanf().
 *
 * @param[in]   data_in     A pointer to the data to parse.
 * @param[in]   data_len    The length of @p data_in in bytes.
 * @param[in]   pack_format C string that contains a format string follows the same specification of spinel.
 * @param[in]   args        A value identifying a variable arguments list.
 *
 * @returns The parsed length in bytes.
 *
 * @note This function behaves different from `spinel_datatype_vunpack()`:
 *       - This function expects composite data arguments of pointer to data type, while `spinel_datatype_vunpack()`
 *         expects them of pointer to data type pointer. For example, if `SPINEL_DATATYPE_EUI64_C` is present in
 *         @p pack_format, this function expects a `spinel_eui64_t *` is included in variable arguments, while
 *         `spinel_datatype_vunpack()` expects a `spinel_eui64_t **` is included.
 *       - For `SPINEL_DATATYPE_UTF8_C`, this function expects two arguments, the first of type `char *` and the
 *         second is of type `size_t` to indicate length of the provided buffer in the first argument just like
 *         `strncpy()`, while `spinel_datatype_vunpack()` only expects a `const char **`.
 *
 * @sa spinel_datatype_unpack_in_place()
 *
 */
SPINEL_API_EXTERN spinel_ssize_t spinel_datatype_vunpack_in_place(const uint8_t *data_in, spinel_size_t data_len,
                                                         const char *pack_format, va_list args);

SPINEL_API_EXTERN spinel_ssize_t spinel_packed_uint_decode(const uint8_t *bytes, spinel_size_t len,
                                                           unsigned int *value);
SPINEL_API_EXTERN spinel_ssize_t spinel_packed_uint_encode(uint8_t *bytes, spinel_size_t len, unsigned int value);
SPINEL_API_EXTERN spinel_ssize_t spinel_packed_uint_size(unsigned int value);

SPINEL_API_EXTERN const char *spinel_next_packed_datatype(const char *pack_format);

// ----------------------------------------------------------------------------

SPINEL_API_EXTERN const char *spinel_prop_key_to_cstr(spinel_prop_key_t prop_key);

SPINEL_API_EXTERN const char *spinel_net_role_to_cstr(uint8_t net_role);

SPINEL_API_EXTERN const char *spinel_mcu_power_state_to_cstr(spinel_mcu_power_state_t mcu_power_state);

SPINEL_API_EXTERN const char *spinel_status_to_cstr(spinel_status_t status);

SPINEL_API_EXTERN const char *spinel_capability_to_cstr(unsigned int capability);

// ----------------------------------------------------------------------------

__END_DECLS

#endif /* defined(SPINEL_HEADER_INCLUDED) */
