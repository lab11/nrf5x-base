/* ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 PURPOSE: ZBOSS core API header. Time, scheduler and memory buffers API.
*/

#ifndef ZB_ZBOSS_API_CORE_H
#define ZB_ZBOSS_API_CORE_H 1

#include "zb_vendor.h"
#include "zb_config.h"
#include "zb_types.h"
#include "zb_errors.h"
#include "zb_osif.h"
#include "zb_debug.h"
#include "zb_trace.h"


/*! \addtogroup time */
/*! @{ */

/**
   \par Timer functionality.

   The idea is: platform has some timer which can be stopped or run.
   When run, it increments (or decrements - depends on platform) some counter
   until counter overflow (underflow), then issues interrupt - wakeups main loop
   if it sleeping.
   Time stored in ticks; time resolution is platform dependent, its usual value
   is 15.36 usec - 1 beacon interval.
   Note that time type has limited capacity (usually 16 bits) and can overflow.
   Macros which works with time handles overflow. It is supposed that time values will
   not differ to more then 1/2 of the maximum time value.

   All that timer macros will not be used directly by the application code - it
   is scheduler internals. The only API for timer is ZB_SCHEDULE_ALARM() call.
 */


/**
   Timer type.
 */

#ifdef ZB_TIMER_32
  typedef zb_uint32_t zb_time_t;
  #define ZB_MAX_TIME_VAL ZB_UINT32_MAX
  #define ZB_MIN_TIME_VAL ZB_UINT32_MIN
#else
/* 16 bits for 8051 - it will be hw timer value. */
  typedef zb_uint16_t zb_time_t;
  #define ZB_MAX_TIME_VAL ZB_UINT16_MAX
  #define ZB_MIN_TIME_VAL ZB_UINT16_MIN
#endif

#define ZB_HALF_MAX_TIME_VAL (ZB_MAX_TIME_VAL / 2)

/** @cond internals_doc */  
/**
 * @copydoc ZB_TIMER_GET()
 *
 * @note Don't call directly, use @ref ZB_TIMER_GET() instead
 */
zb_time_t zb_timer_get(void);
/** @endcond */ /* internals_doc */

/**
 * @return Get current timer value (beacon intervals)
 */
#define ZB_TIMER_GET() (zb_timer_get())

/**
   Time subtraction: subtract 'b' from 'a'

   Take overflow into account: change sign (subtraction order) if result >
   values_diapasin/2.
   Suppose a always >= b, so result is never negative.
   This macro will be used to calculate, for example, amount of time to sleep
   - it is positive by definition.
   Do not use it to compare time values! Use ZB_TIME_GE() instead.
   Note that both a and b is of type @ref zb_time_t. Can't decrease time (subtract
   constant from it) using this macro.

   @param a - time to subtract from
   @param b - time to subtract
   @return subtraction result
 */
#define ZB_TIME_SUBTRACT(a, b) ((zb_time_t)((a) - (b)) < ZB_HALF_MAX_TIME_VAL ? (zb_time_t)((a) - (b)) : (zb_time_t)((b) - (a)))

/**
   Time add: add 'a' to 'b'

   Overflow is possible, but this is ok - it handled by subtraction and compare macros.

   @param a - time to add to
   @param b - value to add
   @return addition result
 */
#define ZB_TIME_ADD(a, b) (zb_time_t)((a) + (b))

/**
   Compare times a and b - check that a >= b

   Taking into account overflow and unsigned values arithmetic and supposing
   difference between a and b can't be > 1/2 of the overall time values
   diapason,
   a >= b only if a - b < values_diapason/2

   @param a - first time value to compare
   @param b - second time value to compare
   @return 1 is a >= b, 0 otherwhise
 */
#define ZB_TIME_GE(a, b) ((zb_time_t)((a) - (b)) < ZB_HALF_MAX_TIME_VAL)


/**
 One second timeout
*/
#define ZB_TIME_ONE_SECOND ZB_MILLISECONDS_TO_BEACON_INTERVAL(1000)


#ifdef ZB_TIMER_32
/**
  Convert time from milliseconds to beacon intervals (32-bit platforms).
*/
#define ZB_MILLISECONDS_TO_BEACON_INTERVAL(ms) (((zb_time_t)(ms) * 1000) / ZB_BEACON_INTERVAL_USEC)
#else
/**
  Convert time from milliseconds to beacon intervals
  Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
*/
#define ZB_MILLISECONDS_TO_BEACON_INTERVAL(ms) (((10l * (zb_time_t)(ms) + 0) / (ZB_BEACON_INTERVAL_USEC / 100)))
#endif

/*
IEEE 802.15.4g p.7

Table 0 - MR-FSK symbol duration used for MAC and PHY timing parameters
Frequency band (MHz)                  MR-FSK symbol duration used for MAC
                                      and PHY timing parameters (us)
--------------------------------------------------------------------------
169.400-169.475 (Europe)              208+1/3
450-470 (US FCC Part 22/90)           104+1/6
470-510 (China)                       20
779-787 (China)                       20
863-870 (Europe)                      20
896-901 (US FCC Part 90)              100
901-902 (US FCC Part 24)              100
902-928 (US ISM)                      20
917-923.5 (Korea)                     20
928-960 (US FCC Part 22/24/90/101)    100
920-928 (Japan)                       20
950-958 (Japan)                       20
1427-1518 (US FCC Part 90)/(Canada    100
                      SRSP 301.4)
2400-2483.5 (Worldwide)               20
*/

#if   defined(ZB_SUB_GHZ_EU1) || defined(ZB_SUB_GHZ_EU2) || defined(ZB_SUB_GHZ_EU3)
#define ZB_SYMBOL_DURATION_USEC   20
#elif defined(ZB_SUB_GHZ_US)
#define ZB_SYMBOL_DURATION_USEC   20
#elif defined ZB_SUB_GHZ_JP
#define ZB_SYMBOL_DURATION_USEC   20
#elif defined ZB_SUB_GHZ_CN
#else

/**
 * @brief Symbol duration
 * @see 6.5.3.2 Symbol rate
 * The 2450 MHz PHY symbol rate shall be 62.
 * 5 ksymbol/s 40 ppm.
 */
#define ZB_SYMBOL_DURATION_USEC   16      /* 1 symbol = 16e-6 sec (mac spec 6.5.3.2 Symbol rate) */
#endif

/**
   \par Time measurement unit is beacon interval.
   It is both internal representation and value used in API.
   It is still possible to convert it to/from msec.
   aBaseSuperframeDuration = aBaseSlotDuration * aNumSuperframeSlots
   aBaseSlotDuration = 60
   aNumSuperframeSlots = 16
*/
#define ZB_ABASE_SUPERFRAME_DURATION 960

/**
 * Beacon interval in microseconds
 *
 * 1 beacon interval = aBaseSuperframeDuration * symbol duration
 *
 * 1 symbol = 16e-6 sec (mac spec 6.5.3.2 Symbol rate) for 2.4GHz ZB.
 * 1 beacon interval = 15.360 ms.
 */
#define ZB_BEACON_INTERVAL_USEC (ZB_SYMBOL_DURATION_USEC * ZB_ABASE_SUPERFRAME_DURATION)


#ifdef ZB_TIMER_32
/**
  Convert time from beacon intervals to milliseconds (32-bit platform).
*/
#define ZB_TIME_BEACON_INTERVAL_TO_MSEC(t) ((ZB_BEACON_INTERVAL_USEC * (zb_time_t)(t)) / 1000)
#else
/**
  Convert time from beacon intervals to milliseconds

  Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
*/
#define ZB_TIME_BEACON_INTERVAL_TO_MSEC(t) ((ZB_BEACON_INTERVAL_USEC / 100 * (t) - 0) / 10)
#endif

/**
  Convert time from beacon intervals to microseconds

*/
#define ZB_TIME_BEACON_INTERVAL_TO_USEC(t) ((zb_uint64_t)ZB_BEACON_INTERVAL_USEC * (t))

/**
 Quarterseconds timeout
*/
#define ZB_TIME_QUARTERECONDS(n)  (ZB_TIME_BEACON_INTERVAL_TO_MSEC((n)) / 250)

/**
 Convert from quarterseconds to msec
*/
#define ZB_QUARTERECONDS_TO_MSEC(n)  250*(n)

/**
 Convert from quarterseconds to beacon
*/
#define ZB_QUARTERECONDS_TO_BEACON_INTERVAL(qsec) ZB_MILLISECONDS_TO_BEACON_INTERVAL(250*qsec)

/**
 * Convert from seconds to milliseconds
 */
#define ZB_SECONDS_TO_MILLISECONDS(_s) (1000l*(_s))
/**
 Convert from milliseconds to microseconds
*/
#define ZB_MILLISECONDS_TO_USEC(ms) ((ms) * (1000))

/**
 Convert from microseconds to milliseconds
*/
#define ZB_USECS_TO_MILLISECONDS(usec) ((usec) / (1000))

/*! @} */

/*! \addtogroup sched */
/*! @{ */

/**
   Callback function typedef.
   Callback is function planned to execute by another function.
   Note that callback must be declared as reentrant for sdcc.

   @param param - callback parameter - usually, but not always, ref to packet buf

   @return none.

   See any sample
 */
typedef void (ZB_CODE * zb_callback_t)(zb_uint8_t param);

typedef zb_ret_t (ZB_CODE * zb_ret_callback_t)(zb_uint8_t param);

typedef void (*zb_callback_void_t)(void);


/**
   Callback function with 2 parameters typedef.
   Callback is function planned to execute by another function.
   Note that callback must be declared as reentrant for sdcc.

   @param param - callback parameter - usually, but not always, ref to packet buf
   @param cb_param - additional 2-byte callback parameter, user data.

   @return none.

   See any sample
 */
typedef void (ZB_CODE * zb_callback2_t)(zb_uint8_t param, zb_uint16_t cb_param);
/** @cond internals_doc */
/** Schedule single-param callback execution.
    (use ZB_SCHEDULE_CALLBACK() macro instead of this function).

    Schedule execution of function `func' in the main scheduler loop.

    @param func - function to execute
    @param param - callback parameter - usually, but not always ref to packet buffer
    @param use_2_param - ZB_TRUE whether additional parameter should be used
    @param user_param - additional parameter which will be used if use_2_param is ZB_TRUE
    @param is_prior - ZB_TRUE is callback is high priority

    @return RET_OK or error code.

    See sched sample
*/
zb_ret_t zb_schedule_callback(zb_callback_t func, zb_uint8_t param,
                              zb_bool_t use_2_param, zb_uint16_t user_param, zb_bool_t is_prior);
/** @endcond */ /* internals_doc */
/**
   Schedule single-param callback execution.

   Schedule execution of function `func' in the main scheduler loop.

   @param func - function to execute
   @param param - callback parameter - usually, but not always ref to packet buffer

   @return RET_OK or error code.

   See sched sample
 */

#define ZB_SCHEDULE_CALLBACK(func, param) zb_schedule_callback(func, param, ZB_FALSE, 0, ZB_FALSE)

/**
   Schedule two-param callback execution.
   Schedule execution of function `func' in the main scheduler loop.

   @param func - function to execute
   @param param - zb_uint8_t callback parameter - usually, but not always ref to
   packet buffer
   @param user_param - zb_uint16_t user parameter - usually, but not
   always short address

   @return RET_OK or error code.
   See sched sample
 */
#define ZB_SCHEDULE_CALLBACK2(func, param, user_param) zb_schedule_callback((zb_callback_t)func,  param, ZB_TRUE, user_param, ZB_FALSE)

/** @cond internals_doc */
zb_ret_t zb_schedule_alarm(zb_callback_t func, zb_uint8_t param, zb_time_t timeout_bi);
/** @endcond */ /* internals_doc */

/**
   Schedule alarm - callback to be executed after timeout.

   Function will be called via scheduler after timeout expired (maybe, plus some
   additional time).
   Timer resolution depends on implementation.
   Same callback can be scheduled for execution more then once.

   @param func - function to call via scheduler
   @param param - parameter to pass to the function
   @param timeout_bi - timeout, in beacon intervals
   @return RET_OK or error code

   See any sample
 */

#define ZB_SCHEDULE_ALARM(func, param, timeout_bi) zb_schedule_alarm(func, param, timeout_bi)

/**
   Special parameter for zb_schedule_alarm_cancel(): cancel alarm once without
   parameter check

   Cancel only one alarm without check for parameter
 */
#define ZB_ALARM_ANY_PARAM (zb_uint8_t)(-1)

/**
   Special parameter for zb_schedule_alarm_cancel(): cancel alarm for all
   parameters
 */
#define ZB_ALARM_ALL_CB (zb_uint8_t)(-2)
/** @cond internals_doc */
/**
   Cancel scheduled alarm (use macro ZB_SCHEDULE_ALARM_CANCEL() or
   ZB_SCHEDULE_ALARM_CANCEL_AND_GET_BUF() instead of this function).

   This function cancel previously scheduled alarm. Function is identified by
   the pointer.

   @param func - function to cancel
   @param param - parameter to cancel. \see ZB_ALARM_ANY_PARAM. \see ZB_ALARM_ALL_CB
   @param p_param - [out] pointer of ref buffer from cancelled flag: free buffer if its alarm will be cancel
   @return RET_OK or error code

   See reporting_srv sample
 */
zb_ret_t zb_schedule_alarm_cancel(zb_callback_t func, zb_uint8_t param, zb_uint8_t *p_param);
/** @endcond */ /* internals_doc */
/**
   Cancel scheduled alarm.

   This function cancel previously scheduled alarm. Function is identified by
   the pointer.

   @param func - function to cancel
   @param param - parameter to cancel. \see ZB_ALARM_ANY_PARAM. \see ZB_ALARM_ALL_CB
   @return RET_OK or error code

   See reporting_srv sample
 */

#define ZB_SCHEDULE_ALARM_CANCEL(func, param) zb_schedule_alarm_cancel((func), (param), NULL)

/**
   Cancel scheduled alarm and get buffer.

   This function cancel previously scheduled alarm and returns buffer ref associated with alarm.
   Function is identified by the pointer.

   @param func - function to cancel
   @param param - parameter to cancel. \see ZB_ALARM_ANY_PARAM. \see ZB_ALARM_ALL_CB
   @param p_param - [out] pointer of ref buffer from cancelled flag: free buffer if its alarm will be cancel
   @return RET_OK or error code

   @b Example:
   @code
   {
     zb_uint8_t cancelled_param;

     ZB_SCHEDULE_ALARM_CANCEL_AND_GET_BUF(my_func1, ZB_ALARM_ALL_CB, &cancelled_param);
     my_func1(cancelled_param);
   }
   @endcode

   See reporting_srv sample
 */
#define ZB_SCHEDULE_ALARM_CANCEL_AND_GET_BUF(func, param, p_param) zb_schedule_alarm_cancel((func), (param), p_param)

/** @cond internals_doc */
/**
   Get Schedule alarm time - seek alarm and return alarm time

   @param func - function to call via scheduler
   @param param - parameter to pass to the function
   @param timeout_bi - pointer on alarm timeout, in beacon intervals
   @return RET_OK or error code
 */
zb_ret_t zb_schedule_get_alarm_time(zb_callback_t func, zb_uint8_t param, zb_time_t *timeout_bi);
/** @endcond */ /* internals_doc */

/**
   Get Schedule alarm time - seek alarm and return alarm time

   @param func - function to call via scheduler
   @param param - parameter to pass to the function
   @param timeout_bi - pointer on alarm timeout, in beacon intervals
   @return RET_OK or error code
*/
#define ZB_SCHEDULE_GET_ALARM_TIME(func, param, timeout_bi) zb_schedule_get_alarm_time(func, param, timeout_bi)

/*! @} */


/**
 * @addtogroup buf
 * @{
 *   @par Pass parameters via packet buffers
 *
 *   Usual sequence of the data send/receive primitives is aps-nwk-mac down and up packet pass.
 *   Packets passed not by direct function calls but via scheduler to limit used stack size.
 *   Callback has single parameter only.
 *   Pass packet buffer as parameter.
 *   When possible, pass parameters by filling lower layer header (it is possible for nwk-mac data primitive).
 *   But usually will need a place for some additional arguments.
 *   Will put it into packet tail. Does not formally 'allocate' space at packet tail - just use it.
 *   Packet must have size enough to keep biggest possible frame + parameters.
 *   Function for getting packet tail for parameters checks for overflow and move
 *   data left if it is possible to free space at packet buffer tail.
 *
 *   Management calls have no data. All parameters passed via packet buffers.
 *
 *   @par Blockable memory allocation, reaction on memory absence
 *
 *   Main limited resource is RAM: controller has 16k only.
 *
 *   RAM used for i/o (packet) buffers, routing tables, neighbor tables, (what else?)
 *   and smaller data structures.
 *
 *   Packet buffers pool is common for in and out but its usage for In and Out is
 *   limited (50/50 initially, maybe, will tune it).  The rationale is to prevent
 *   stack stop due to inability to send data due to buffers absence if all buffers
 *   "eaten" as input.
 *
 *   Packet buffer allocation could be asynchronous operation - it means, allocate
 *   call does not return directly but via scheduler (callback call).  It allows
 *   packet allocation to block. In general, all allocation inside stack are blocked.
 *
 *   Output packets allocates at some upper layers (ZD0, ZCL, application etc).  It
 *   packet pool is empty, it means some buffers are sitting in the output queue or
 *   indirect transmit queue waiting for send. When it will be sent and freed,
 *   allocation operation unblocks (callback calls) and goes thru.
 *
 *   Input packets are always allocated in the MAC. If no space available, MAC receive
 *   blocks. It does not begin packet read if no space for it. If it blocks for a
 *   short time, it will be ok. If it blocks for long time, transceiver will probably
 *   skip next packet receive. Anyway, we will not split the packet: MAC transport
 *   will either have memory for entire packet or have no memory at all.
 */

/**
   Buffer type (direction).
 */
enum zb_buffer_types_e
{
  ZB_OUT_BUFFER = 0,   /*!< Out buffer */
  ZB_IN_BUFFER = 1     /*!< In buffer */
};

/**
   Packet buffer header.
 */
typedef ZB_PACKED_PRE struct zb_buf_hdr_s
{
  zb_uint8_t len;              /*!< current layer buffer length  */
  zb_uint8_t data_offset;      /*!< data offset in buffer buf*/
  zb_uint8_t handle;           /*!< The handle associated with the NSDU to be
                                * transmitted by the NWK layer entity.  */
  zb_bitfield_t is_in_buf:1;    /*!< if 1, this is input buffer */

  zb_bitfield_t encrypt_type:3; /*!< payload must be encrypted before send, if
                                 * !0. \see zb_secur_buf_encr_type_e.
                                 */
  zb_bitfield_t use_same_key:1;    /*!< if 1, use same nwk key# packet was
                                    * encrypted by */
  zb_bitfield_t zdo_cmd_no_resp:1; /*!< if 1, this is ZDO command with no
                                    * response - call callback at confirm  */
  zb_bitfield_t is_rx_buf:1;    /*!< if 1, this is buffer with received packet and
                                 * nwk_mac_addrs_t is at buffer tail */
  zb_bitfield_t has_aps_payload:1;   /*!< if 1, than packet comes from APS */

  zb_int16_t status;            /*!< some status to be passed with packet  */
} ZB_PACKED_STRUCT zb_buf_hdr_t;

/**
   Packet buffer
 */
typedef struct zb_buf_s
{
#ifdef ZB_BUF_SHIELD
  zb_uint8_t hdr_signature;
#endif
  union
  {
    zb_buf_hdr_t hdr;                   /*!< Buffer header, if buffer is used */
#ifndef ZB_USE_STD_MALLOC
    struct zb_buf_s *next;              /*!< Ref to next free buffer, if buffer is not used*/
#endif
  } u; /*!< Buffer header */
#if defined ZB_USE_STD_MALLOC && defined ZB_USE_ALLOC_FOR_PAYLOAD_DATA
  zb_uint8_t   *buf;                    /*!< Dynamically allocated buffer for payload */
#else
  zb_uint8_t   buf[ZB_IO_BUF_SIZE];     /*!< Data*/
#endif
#ifdef ZB_DEBUG_BUFFERS_EXT
  zb_buf_usage_t buf_allocation;
  zb_buf_usage_t buf_usages[ZB_DEBUG_BUFFERS_EXT_USAGES_COUNT];
#endif
#ifdef ZB_BUF_SHIELD
  zb_uint8_t tail_signature;
#endif
} zb_buf_t;

/**
 * @brief Return pointer to the beginning of the buffer
 *
 * @param zbbuf - pointer to buffer @ref zb_buf_t
 *
 * @return pointer to beginning of buffer
 *
 * @note see @ref ZB_BUF_BEGIN(), which returns pointer to the beginning of data written
 */
#define ZB_BUF_GET_ARRAY_PTR(zbbuf) ((zbbuf)->buf)

/** @cond internals_doc */
zb_bool_t zb_buf_check_z(zb_int_t file_id, zb_uint8_t param);

#ifdef ZB_BUF_SHIELD

zb_bool_t zb_buf_check_full(zb_int_t file_id,zb_uint8_t param);
zb_bool_t zb_buf_check_z_ok(zb_uint8_t param);


#define ZB_BUF_IS_BUSY_INLINE(param) \
  ZB_INLINE_ASSERT(zb_buf_check_full(ZB_TRACE_FILE_ID, param))

#define ZB_BUF_IS_BUSY(param) ZB_ASSERT(zb_buf_check_full(ZB_TRACE_FILE_ID, param))

#define ZB_BUF_ZERO_ASSERT(param) ZB_INLINE_ASSERT(zb_buf_check_z(ZB_TRACE_FILE_ID, param))

#else

#define ZB_BUF_IS_BUSY(param)
#define ZB_BUF_IS_BUSY_INLINE(param)
#define ZB_BUF_ZERO_ASSERT(param)

#endif

/* Function prototypes.
   Do not use it directly. Use macro instead.
 */

/**
 * @copydoc ZB_BUF_BEGIN()
 *
 * @param file_id - TRACE_ID of the file from which function is invoked
 *
 * @note don't call it directly, use @ref ZB_BUF_BEGIN() instead
 */
zb_uint8_t* zb_buf_begin(zb_int_t file_id, zb_buf_t *zbbuf);

/**
 * @copydoc ZB_BUF_LEN()
 *
 * @param file_id - TRACE_ID of the file from which function is invoked
 *
 * @note don't call it directly, use @ref ZB_BUF_LEN() instead
 */
zb_uint8_t zb_buf_len(zb_int_t file_id, zb_buf_t *zbbuf);

/**
   Get IN buffer from the buffers list.
   If no buffers available, does not block.

   @return pointer to the buffer or NULL if no buffer available.
 */
#ifdef ZB_DEBUG_BUFFERS
zb_buf_t *zb_get_in_buf(zb_uint16_t from_file, zb_uint16_t from_line);

/**
   Get IN buffer from the buffers list.
   If no buffers available, does not block.

   @return pointer to the buffer or NULL if no buffer available.
 */
#define ZB_GET_IN_BUF() zb_get_in_buf(ZB_TRACE_FILE_ID, __LINE__)
#else
zb_buf_t *zb_get_in_buf(void);

/**
   Get IN buffer from the buffers list.
   If no buffers available, does not block.

   @return pointer to the buffer or NULL if no buffer available.
 */
#define ZB_GET_IN_BUF() zb_get_in_buf()
#endif

/**
   Get OUT buffer from the buffers list.
   If no buffers available, does not block.
   @return pointer to the buffer.
 */
#ifdef ZB_DEBUG_BUFFERS
zb_buf_t *zb_get_out_buf(zb_uint16_t from_file, zb_uint16_t from_line);

/**
   Get OUT buffer from the buffers list.
   If no buffers available, does not block.
   @return pointer to the buffer.
 */
#define ZB_GET_OUT_BUF() zb_get_out_buf(ZB_TRACE_FILE_ID, __LINE__)
#else
zb_buf_t *zb_get_out_buf(void);

/**
   Get OUT buffer from the buffers list.
   If no buffers available, does not block.
   @return pointer to the buffer.
 */
#define ZB_GET_OUT_BUF() zb_get_out_buf()
#endif
/** @endcond */ /* internals_doc */


/**
 * @copydoc ZB_BUF_FROM_REF()
 *
 * @note Don't call it directly, use @ref ZB_BUF_FROM_REF() instead.
 *
 * @param from_file - TRACE_ID of the file from which function is invoked
 * @param from_line - line from which function is invoked
 */
zb_buf_t* zb_buf_from_ref(zb_uint8_t ref, zb_uint16_t from_file, zb_uint16_t from_line);

/**
 * Calculate pointer of the buffer by buffer reference.
 *
 * @param ref - buffer reference
 *
 * @return pointer to buffer header
*/
#ifdef ZB_USE_STD_MALLOC
#define ZB_BUF_FROM_REF(ref) (ZG->bpool.pool[ref])
#else
#define ZB_BUF_FROM_REF(ref) (zb_buf_from_ref((ref), ZB_TRACE_FILE_ID, __LINE__))
#endif

#ifdef ZB_USE_STD_MALLOC
#define ZB_REF_FROM_BUF(buf) get_buf_ref(buf)
#else

/**
 * @copydoc ZB_REF_FROM_BUF()
 *
 * @note don't call it directly, use @ref ZB_REF_FROM_BUF() instead
 */
zb_uint8_t zb_ref_from_buf(zb_buf_t *buf);

/**
 * @brief get buffer reference which can be passed to scheduler callback
 *
 * @param buf - buffer of interest
 *
 * @return reference to buffer
 */
#define ZB_REF_FROM_BUF(buf) zb_ref_from_buf(buf)

#endif

/**
 * Return pointer to data stored in buffer
 *
 * @param zbbuf - buffer
 *
 * @return pointer to beginning of data in buffer
 *
 * @note see @ref ZB_BUF_GET_ARRAY_PTR() which returns begining of buffer
 */
#define ZB_BUF_BEGIN(zbbuf) zb_buf_begin(ZB_TRACE_FILE_ID, zbbuf)

/**
 * Return current buffer length
 *
 * @param zbbuf - pointer to buffer
 *
 * @return size of data placed in buffer
 */
#define ZB_BUF_LEN(zbbuf) zb_buf_len(ZB_TRACE_FILE_ID, zbbuf)

/**
   Return current buffer length by buffer reference.
 */
#define ZB_BUF_LEN_BY_REF(param) ZB_BUF_LEN(ZB_BUF_FROM_REF(param))

/**
   Return pointer to data stored in buffer by buffer reference.
 */
#define ZB_BUF_BEGIN_FROM_REF(_ref) ZB_BUF_BEGIN(ZB_BUF_FROM_REF(_ref))

/**
 * @copydoc ZB_BUF_COPY()
 *
 * @note don't call it directly, use @ref ZB_BUF_COPY() instead
 */
void zb_buf_copy(zb_buf_t *dst_buf, zb_buf_t *src_buf);

/**
   Copy one buffer to another

   @param src_buf - source buffer
   @param dst_buf - destination buffer
 */
#define ZB_BUF_COPY(dst_buf, src_buf) zb_buf_copy(dst_buf, src_buf)

/**
 * @copydoc ZB_BUF_OFFSET()
 *
 * @param file_id - TRACE_ID of the file from which function is invoked
 * 
 * @note don't call it directly, use @ref ZB_BUF_OFFSET() instead
 */
zb_uint8_t zb_buf_offset(zb_int_t file_id, zb_buf_t *zbbuf);

/**
 * Return current buffer offset
 *
 * @param zbbuf - pointer to buffer
 *
 * @return offset of the data in the buffer
 */
#define ZB_BUF_OFFSET(zbbuf) zb_buf_offset(ZB_TRACE_FILE_ID, zbbuf)


/**
 * @copybrief ZB_BUF_INITIAL_ALLOC()
 *
 * @param zbbuf - buffer
 * @param size  - size to allocate
 *
 * @note don't call it directly, use @ref ZB_BUF_INITIAL_ALLOC() instead
 */
zb_void_t *zb_buf_initial_alloc(zb_buf_t *zbbuf, zb_uint8_t size);

/**
   @brief Initial space allocation in buffer.

   Allocate space in the buffer center (keep space in both buffer head
   and tail). Use it at upper layers before filling buffer by data.
   Old buffer contents is lost.

   @param zbbuf - buffer
   @param size  - size to allocate
   @param ptr   - (out) pointer to the new buffer begin

   @return pointer to the allocated space
 */
#define ZB_BUF_INITIAL_ALLOC(zbbuf, size, ptr) (ptr) = zb_buf_initial_alloc((zbbuf), (size))

/**
 * @copybrief ZB_BUF_ALLOC_LEFT()
 *
 * @param zbbuf - buffer
 * @param size  - size to allocate
 *
 * @note don't call it directly, use @ref ZB_BUF_ALLOC_LEFT() instead
*/
zb_void_t *zb_buf_smart_alloc_left(zb_buf_t *zbbuf, zb_uint8_t size);

/**
 * Allocate space at the beginning of buffer
 *
 * @param zbbuf - buffer
 * @param size  - size to allocate
 * @param ptr   - (out) pointer to the new buffer begin
 */
#define ZB_BUF_ALLOC_LEFT(zbbuf, size, ptr) (ptr) = zb_buf_smart_alloc_left((zbbuf), (size))

/**
 * @copybrief ZB_BUF_ALLOC_RIGHT()
 *
 * @param zbbuf - buffer
 * @param size  - size to allocate
 *
 * @note don't call it directly, use @ref ZB_BUF_ALLOC_RIGHT() instead
 */
zb_void_t *zb_buf_smart_alloc_right(zb_buf_t *zbbuf, zb_uint8_t size);

/**
   Allocate space at buffer end

   @param zbbuf - buffer
   @param size  - size to allocate
   @param ptr   - (out) pointer to the space allocated
 */
#define ZB_BUF_ALLOC_RIGHT(zbbuf, size, ptr) (ptr) = zb_buf_smart_alloc_right((zbbuf), (size))

/**
 * @copybrief ZB_BUF_CUT_LEFT()
 *
 * @param zbbuf - buffer
 * @param size  - size to cut
 *
 * @note don't call it directly, use @ref ZB_BUF_CUT_LEFT() instead
 */
void *zb_buf_cut_left(zb_buf_t *zbbuf, zb_uint8_t size);

/**
 * Cut space at the beginning of buffer
 *
 * @param zbbuf - buffer
 * @param size  - size to cut
 * @param ptr   - (out) pointer to the new buffer begin
*/
#define ZB_BUF_CUT_LEFT(zbbuf, size, ptr)  (ptr) = zb_buf_cut_left((zbbuf), (size))

void zb_buf_cut_left2(zb_int_t file_id, zb_buf_t *zbbuf, zb_uint8_t size);
#define ZB_BUF_CUT_LEFT2(zbbuf, size) zb_buf_cut_left2(ZB_TRACE_FILE_ID, zbbuf, size)

/**
 * @copybrief ZB_BUF_CUT_RIGHT()
 *
 * @param zbbuf - buffer
 * @param size  - size to cut
 *
 * @note don't call it directly, use @ref ZB_BUF_CUT_RIGHT() instead
 */
void zb_buf_cut_right(zb_buf_t *zbbuf, zb_uint8_t size);

/**
 * Cut space at the end of buffer
 *
 * @param zbbuf - buffer
 * @param size  - size to cut
 */
#define ZB_BUF_CUT_RIGHT(zbbuf, size) zb_buf_cut_right((zbbuf), (size))

/**
 * @copydoc ZB_GET_BUF_TAIL()
 *
 * @note don't call directly, use @ref ZB_GET_BUF_TAIL() instead
 */
zb_void_t *zb_get_buf_tail(zb_buf_t *zbbuf, zb_uint8_t size);

/**
   Get buffer tail of size 'size'

   Usually used to place external information (some parameters) to the buffer.

   @param zbbuf - buffer
   @param size - requested size

   @return pointer to the buffer tail
 */
#define ZB_GET_BUF_TAIL(zbbuf, size) zb_get_buf_tail(zbbuf, size)

/**
   Get buffer tail of size sizeof(type)

   Usually used to place external information (some parameters) to the buffer

   @param zbbuf - buffer
   @param type - data type that will be placed at the buffer end

   @return pointer to the buffer tail casted to (type*)

   @b Example
   @code
     zb_zdo_nwk_addr_req_param_t *req = ZB_GET_BUF_PARAM(buf, zb_zdo_nwk_addr_req_param_t);

     req->dst_addr = 0xffff;
     ZB_IEEE_ADDR_COPY(req->ieee_addr, g_ieee_addr_r4);
     req->start_index  = 0;
     req->request_type = 0x00;
     zb_zdo_nwk_addr_req(ZB_REF_FROM_BUF(buf), NULL);
   @endcode
 */
#define ZB_GET_BUF_PARAM(zbbuf, type) ((type *)ZB_GET_BUF_TAIL((zbbuf), sizeof(type)))

/**
   Place data of size sizeof(type) at the end of buffer

   Macro used to copy external information (some data) at the
   end of buffer

   @param zbbuf - buffer
   @param param - data to be placed in the end of buffer
   @param type - data type that will be placed at the buffer end

   @b Example
   @code
     zb_zdo_nwk_addr_req_param_t req;

     req.dst_addr = 0xffff;
     ZB_IEEE_ADDR_COPY(req.ieee_addr, g_ieee_addr_r4);
     req.start_index  = 0;
     req.request_type = 0x00;
     ZB_SET_BUF_PARAM(buf, req, zb_zdo_nwk_addr_req_param_t);
     zb_zdo_nwk_addr_req(ZB_REF_FROM_BUF(buf), NULL);
   @endcode
 */
#define ZB_SET_BUF_PARAM(zbbuf, param, type) ( ZB_BUF_IS_BUSY_INLINE(ZB_REF_FROM_BUF(zbbuf)) *((type *)ZB_GET_BUF_TAIL(zbbuf, sizeof(type))) = (param) )

/**
   Place data of size sizeof(type) at the end of buffer

   Macro used to copy external information (some data) at the end
   of buffer

   @param zbbuf - buffer
   @param param - data to be placed in the end of buffer
   @param type - data type that will be placed at the buffer end
 */
#define ZB_SET_BUF_PARAM_PTR(zbbuf, param, type) ( ZB_MEMCPY((type *)ZB_GET_BUF_TAIL(zbbuf, sizeof(type)), (param), sizeof(type)) )

/**
 * @copydoc ZB_BUF_REUSE()
 *
 * @note Don't call directly, use ZB_BUF_REUSE() instead
 */
zb_void_t zb_buf_reuse(
#ifdef ZB_DEBUG_BUFFERS
    zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_buf_t *zbbuf);

/**
 * @brief Reuse previously used buffer
 *
 * @param zbbuf - buffer
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_BUF_REUSE(zbbuf) zb_buf_reuse(ZB_TRACE_FILE_ID, __LINE__, zbbuf)
#else
#define ZB_BUF_REUSE(zbbuf) zb_buf_reuse(zbbuf)
#endif


/**
 * @copydoc ZB_FREE_BUF()
 *
 * @note Don't call it directly, use @ref ZB_FREE_BUF() instead
 */
#ifndef ZB_DEBUG_BUFFERS
void zb_free_buf(zb_buf_t *buf);
#else
void zb_do_free_buf(zb_uint_t from_file, zb_uint16_t from_line, zb_buf_t *buf);
#define zb_free_buf(buf) zb_do_free_buf(ZB_TRACE_FILE_ID, __LINE__, buf)
#endif

/**
 * @brief Free packet buffer and put it into freelist.
 *
 * Can be called from the main loop.
 *
 * @param buf - packet buffer.
 *
 * See any sample.
 */
#define ZB_FREE_BUF(buf) zb_free_buf(buf)

#define ZB_FREE_BUF_BY_REF(param)         \
do {                                      \
  if(param)                               \
  {                                       \
    zb_free_buf(ZB_BUF_FROM_REF(param));  \
  }                                       \
} while(0)

/**
 * @copydoc ZB_GET_IN_BUF_DELAYED()
 *
 * @note Don't call it directly, use @ref ZB_GET_IN_BUF_DELAYED() instead
 */
zb_ret_t zb_get_in_buf_delayed(
#ifdef ZB_DEBUG_BUFFERS
  zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_callback_t callback);

/**
 * @brief Allocate IN buffer, call a callback when the buffer is available.
 *
 * If buffer is available, schedules callback for execution immediately. If no buffers are available now,
 * schedule callback later, when buffer will be available.
 *
 * @param callback to call.
 * @return RET_OK or error code.
 *
 * @code
 *     ZB_GET_IN_BUF_DELAYED(new_buffer_allocated);
 * @endcode
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_IN_BUF_DELAYED(callback) zb_get_in_buf_delayed(ZB_TRACE_FILE_ID, __LINE__, callback)
#else
#define ZB_GET_IN_BUF_DELAYED(callback) zb_get_in_buf_delayed(callback)
#endif

/**
 * @copydoc ZB_GET_OUT_BUF_DELAYED()
 *
 * @note Don't call it directly, use @ref ZB_GET_OUT_BUF_DELAYED() instead
 */
zb_ret_t zb_get_out_buf_delayed(
#ifdef ZB_DEBUG_BUFFERS
  zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_callback_t callback);

/**
 * @brief Allocate OUT buffer, call a callback when the buffer is available.
 *
 * If buffer is available, schedules callback for execution immediatly. If no buffers are available now,
 * schedule callback later, when buffer will be available.
 *
 * @param callback to call.
 * @return RET_OK or error code.
 *
 * @code
 *     ZB_GET_OUT_BUF_DELAYED(new_buffer_allocated);
 * @endcode
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_OUT_BUF_DELAYED(callback) zb_get_out_buf_delayed(ZB_TRACE_FILE_ID, __LINE__, callback)
#else
#define ZB_GET_OUT_BUF_DELAYED(callback) zb_get_out_buf_delayed(callback)
#endif

/**
 * @brief Allocate buffer of given type and  call a callback with extra user parameter
 * when buffer of this type is available.
 *
 * If buffer is available, schedules callback for execution immediatly. If no buffers are available now,
 * schedule callback later, when buffer will be available.
 *
 * @note Don't call it directly, use @ref ZB_GET_OUT_BUF_DELAYED2() (for out buffers) instead
 *
 * @param callback - callback to call
 * @param buf_type - buffer type (either input or output)
 * @param use_2_param - whether additional param is used
 * @param user_param - will be passed to callback.
 *
 * @return RET_OK or error code.
 */
zb_ret_t zb_get_buf_delayed_2param(
#ifdef ZB_DEBUG_BUFFERS
  zb_uint16_t from_file, zb_uint16_t from_line,
#endif
  zb_callback2_t callback,
  enum zb_buffer_types_e buf_type,
  zb_bool_t use_2_param,
  zb_uint16_t user_param);

/**
 * @brief Allocate out buffer, call a callback with extra user parameter when the buffer is available.
 *
 * If buffer is available, schedules callback for execution immediatly. If no buffers are available now,
 * schedule callback later, when buffer will be available.
 *
 * @param callback - callback to call
 * @param user_param - will be passed to callback.
 *
 * @return RET_OK or error code.
 *
 *  @code
 *      ZB_GET_OUT_BUF_DELAYED2(new_buffer_allocated, user_data);
 *  @endcode
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_OUT_BUF_DELAYED2(callback, user_param) zb_get_buf_delayed_2param(ZB_TRACE_FILE_ID, __LINE__, callback, ZB_OUT_BUFFER, ZB_TRUE, user_param)
#else
#define ZB_GET_OUT_BUF_DELAYED2(callback, user_param) zb_get_buf_delayed_2param(callback, ZB_OUT_BUFFER, ZB_TRUE, user_param)
#endif
/** @cond internals_doc */
/**
 * @copydoc ZB_SWITCH_BUF()
 *
 * @note don't call directly, use @ref ZB_SWITCH_BUF() instead
 */
zb_void_t zb_switch_buf(
#ifdef ZB_DEBUG_BUFFERS
    zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_buf_t *buf, zb_uint8_t to_in);
/** @endcond */ /* internals_doc */
/**
 * Reclassify buffer : IN to OUT or OUT to IN.
 *
 * @param buf - buffer ptr (see @ref zb_buf_t)
 * @param to_in - buffer type to set (see @ref zb_buffer_types_e)
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_SWITCH_BUF(buf, to_in) zb_switch_buf(ZB_TRACE_FILE_ID, __LINE__, buf, to_in)
#else
#define ZB_SWITCH_BUF(buf, to_in) zb_switch_buf(buf,to_in)
#endif

/*! @} */

/*! \addtogroup rng */
/*! @{ */

/**
 * Maximal possible value for randomly generated (16-bit) value
 */
#define ZB_RAND_MAX ((zb_uint16_t)0xffff)

/**
 * Generate random value
 *
 * @return random value between 0 to 0xffff (16 bit)
 */
zb_uint16_t zb_random(void);


/**
   Generate random value between 0 to max_value (16 bit)
 */
#define ZB_RANDOM_VALUE(max_value) (zb_random() / (ZB_RAND_MAX / (zb_uint16_t)(max_value)))

/*! @} */

/** @cond internals_doc */
/**
   Copy 8 byte array (i.e. long address).
 */
void zb_memcpy8(zb_uint8_t *ptr, zb_uint8_t *src);
/** @endcond */ /* internals_doc */

/**
  Return absolute value
*/
#define ZB_ABS(a) ((a) < 0 ? -(a) : (a))

#endif /* ZB_ZBOSS_API_CORE_H */
