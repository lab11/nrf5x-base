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
PURPOSE: general-purpose typedefs
*/

#if ! defined ZB_TYPES_H
#define ZB_TYPES_H 1

#ifndef ZB_CONFIG_H
#error zb_config.h must be included before zb_types.h. Dont do it directly. Include zb_common.h or zboss_api.h instead.
#endif

/**
 * @addtogroup base_types
 * @{
 */

#define ZB_32BIT_WORD

/* Really need xdata declaration here, not in osif: don't want to include osig.h here */
#ifdef ZB_IAR
#define ZB_XDATA
#define ZB_CODE
#define ZB_IAR_CODE
#else
#define ZB_XDATA
#define ZB_CODE
#define ZB_IAR_CODE
#endif

/* register modifier for variables. Can be defined to "register". Will it help to the compiler? */
/* #define ZB_REGISTER */
#define ZB_REGISTER register

#define ZB_VOID_ARGLIST void

/* Not sure all C compilers support 'const'. Let's add some conditional
 * compilation when we will find such compiler  */
#define ZB_CONST const

/* inline directive for functions placed into .h files in C */
#if defined __GNUC__ || defined __SUNPRO_C || defined  __cplusplus
#define ZB_INLINE inline
#define ZB_INLINE_IN_C
#define ZB_BITFIELD_CAST(x) (x)
#elif defined _MSC_VER
#define ZB_INLINE __inline
#define ZB_BITFIELD_CAST(x) (zb_uint_t)(x)
#else
/* which other C compiler knows about inline? */
#define ZB_INLINE
#define ZB_BITFIELD_CAST(x) (x)
#endif

#define ZB_CPP_STR2(x) #x
/**
   Create a string at compile time, like 0 - "0"
 */
#define ZB_CPP_STR(x) ZB_CPP_STR2(x)

/** @brief General purpose boolean type. */
enum zb_bool_e
{
  ZB_FALSE = 0, /**< False value literal. */
  ZB_TRUE = 1   /**<  True value literal. */
};

#if defined WIN32 && !defined ZB_WINDOWS
#define ZB_WINDOWS
#endif

#if (! defined UNIX) || (defined ZB_WINDOWS)

/* base types */

typedef char               zb_char_t;

typedef unsigned char      zb_uchar_t;

typedef unsigned char      zb_uint8_t;

typedef signed char        zb_int8_t;

typedef unsigned short     zb_uint16_t;

typedef signed short       zb_int16_t;
#if   defined ZB_16BIT_WORD

typedef unsigned long      zb_uint32_t;

typedef signed long        zb_int32_t;

typedef zb_uint16_t        zb_bitfield_t;
typedef zb_uint32_t        zb_lbitfield_t;

typedef zb_int16_t         zb_sbitfield_t;

/* FIXME: 16 or 32? */
typedef zb_uint32_t        zb_size_t;

typedef long long          zb_int64_t;
typedef unsigned long long zb_uint64_t;

#else /* defined ZB8051 */
/*
   project-local 4-byte unsigned int type
*/
typedef unsigned int       zb_uint32_t;
/*
   project-local 4-byte signed int type
*/
typedef signed int         zb_int32_t;

/*
   type to be used for unsigned bit fields inside structure
*/
#ifndef ZB_IAR
typedef zb_uint32_t        zb_bitfield_t;
#else
typedef zb_uint8_t         zb_bitfield_t;
#endif
typedef zb_uint32_t        zb_lbitfield_t;
/*
   type to be used for signed bit fields inside structure
*/
typedef zb_int32_t         zb_sbitfield_t;

typedef zb_uint32_t        zb_size_t;

typedef long long          zb_int64_t;
typedef unsigned long long zb_uint64_t;

#endif /* defined ZB8051 */

#else  /* ! defined  UNIX || ZB_WINDOWS */
/* Unix */

#include <inttypes.h>

typedef uint8_t            zb_uint8_t;
typedef int8_t             zb_int8_t;
typedef uint16_t           zb_uint16_t;
typedef int16_t            zb_int16_t;
typedef uint32_t           zb_uint32_t;
typedef int32_t            zb_int32_t;

typedef long long          zb_int64_t;
typedef unsigned long long zb_uint64_t;

typedef char               zb_char_t;
typedef unsigned char      zb_uchar_t;

typedef unsigned           zb_bitfield_t;
typedef unsigned           zb_lbitfield_t;
typedef signed             zb_sbitfield_t;

/* Integer with size equal to the pointer size. */
typedef zb_uint32_t        zb_size_t;
typedef zb_size_t          zb_ptrsize_uint_t;

#endif  /* ! defined UNIX */

/*
  Our short and int definition.
  Short size == 8051 register size, int size is 2 bytes.
 */
#ifdef ZB_8BIT_WORD
/*
   Short int (can fit into single CPU register)
 */
typedef zb_int8_t          zb_short_t;
/*
    short unsigned int (can fit into single CPU register)
 */
typedef zb_uint8_t         zb_ushort_t;
/*
    short unsigned int (can fit into single CPU register) used as a loop index
 */
typedef zb_ushort_t        zb_uindex_t;
/*
    int (at least 2 bytes)
 */
typedef zb_int16_t         zb_int_t;
/*
    unsigned int (at least 2 bytes)
 */
typedef zb_uint16_t        zb_uint_t;
/*
    long int (at least 4 bytes)
 */
typedef zb_int32_t         zb_long_t;
/*
    unsigned long int (at least 4 bytes)
 */
typedef zb_uint32_t        zb_ulong_t;

#elif defined ZB_16BIT_WORD
/*
    short int (can fit into single CPU register)
 */
typedef int                zb_short_t;
/*
    unsigned short int (can fit into single CPU register)
 */
typedef unsigned int       zb_ushort_t;
/*
    short unsigned int (can fit into single CPU register) used as a loop index
 */
typedef zb_ushort_t         zb_uindex_t;
/*
    int (at least 2 bytes)
 */
typedef int                zb_int_t;
/*
    unsigned int (at least 2 bytes)
 */
typedef unsigned int       zb_uint_t;
/*
    long int (at least 4 bytes)
 */
typedef zb_int32_t           zb_long_t;
/*
    unsigned long int (at least 4 bytes)
 */
typedef zb_uint32_t          zb_ulong_t;

#else  /* 32-bit word */
/*
    short int (can fit into single CPU register)
 */
typedef int                zb_short_t;
/*
    unsigned short int (can fit into single CPU register)
 */
typedef unsigned int       zb_ushort_t;
/*
    short unsigned int (can fit into single CPU register) used as a loop index
 */
typedef zb_ushort_t         zb_uindex_t;
/*
    int (at least 2 bytes)
 */
typedef int                zb_int_t;
/*
    unsigned int (at least 2 bytes)
 */
typedef unsigned int       zb_uint_t;
/*
    long int (at least 4 bytes)
 */
typedef zb_int_t           zb_long_t;
/*
    unsigned long int (at least 4 bytes)
 */
typedef zb_uint_t          zb_ulong_t;
#endif

typedef void *             zb_voidp_t;
typedef void               zb_void_t;

/** @brief General purpose boolean type. */
typedef enum zb_bool_e zb_bool_t;

#define ZB_INT8_MIN       (-127 - 1)
#define ZB_INT8_MAX       127
#define ZB_UINT8_MIN      0
#define ZB_UINT8_MAX      255

#define ZB_INT16_MIN       (-32767 - 1)
#define ZB_INT16_MAX       32767
#define ZB_UINT16_MIN      0
#define ZB_UINT16_MAX      65535

/* 2147483648 is unsigned indeed - can't change its sign. prevent warning from
 * msvc 8 */
#define ZB_INT32_MIN       (-2147483647L - 1)
#define ZB_INT32_MAX       2147483647L
#define ZB_UINT32_MIN      0UL
#define ZB_UINT32_MAX      4294967295UL

#define ZB_UINT_MIN      0UL

/*
  Short defined as register size, int as large enough int on this platform.
  32 bit on PC, 8 and 16 bit on 8051
 */

/*
  Max value constants per type
 */
#ifdef ZB_32BIT_WORD

#define ZB_SHORT_MIN       ZB_INT32_MIN
#define ZB_SHORT_MAX       ZB_INT32_MAX
#define ZB_USHORT_MAX      ZB_UINT32_MAX

#define ZB_INT_MIN       ZB_INT32_MIN
#define ZB_INT_MAX       ZB_INT32_MAX
#define ZB_UINT_MAX      ZB_UINT32_MAX

#define ZB_INT_MASK      0x7fffffff

#elif defined ZB_16BIT_WORD

#define ZB_SHORT_MIN       ZB_INT8_MIN
#define ZB_SHORT_MAX       ZB_INT8_MAX
#define ZB_USHORT_MAX      ZB_UINT8_MAX

#define ZB_INT_MIN       ZB_INT16_MIN
#define ZB_INT_MAX       ZB_INT16_MAX
#define ZB_UINT_MAX      ZB_UINT16_MAX

#define ZB_INT_MASK      0x7fff

#elif defined ZB_8BIT_WORD

#define ZB_SHORT_MIN       ZB_INT8_MIN
#define ZB_SHORT_MAX       ZB_INT8_MAX
#define ZB_USHORT_MAX      ZB_UINT8_MAX

#define ZB_INT_MIN       ZB_INT16_MIN
#define ZB_INT_MAX       ZB_INT16_MAX
#define ZB_UINT_MAX      ZB_UINT16_MAX

#define ZB_INT_MASK      0x7fff

#else
#error Portme
#endif

#ifdef __GNUC__
  #define ZB_PACKED_STRUCT __attribute__ ((packed))
  #define ZB_WEAK __attribute__ ((weak))
#else
  #define ZB_PACKED_STRUCT
#endif

/* IAR or Keil ARM CPU */
#if (defined __IAR_SYSTEMS_ICC__ || defined __ARMCC_VERSION) && !defined ZB8051
#define ZB_PACKED_PRE __packed
#define ZB_WEAK_PRE __weak
#else
#define ZB_PACKED_PRE
#endif

#ifndef ZB_WEAK_PRE
#define ZB_WEAK_PRE
#endif
#ifndef ZB_WEAK
#define ZB_WEAK
#endif

/*
   8-bytes address (xpanid or long device address) base type
 */
typedef zb_uint8_t zb_64bit_addr_t[8];

/*
   8-bytes data base type
 */
typedef zb_uint8_t zb_64bit_data_t[8];

extern ZB_CODE ZB_CONST zb_64bit_addr_t g_zero_addr;
extern ZB_CODE ZB_CONST zb_64bit_addr_t g_unknown_ieee_addr;

/*
   Return true if long address is zero
 */
  /* g_zero_addr is declared as ZB_CONST which allows IAR to place it in CODE memory.
     Compiled this by IAR 7.60 for 8051.
     This placement changes pointer type making it unusable
     Is this cast needed here?
  */
#define ZB_IS_64BIT_ADDR_ZERO(addr) (!ZB_MEMCMP((addr), (void const*)g_zero_addr, 8))
#define ZB_IS_64BIT_ADDR_UNKNOWN(addr) (!ZB_MEMCMP((addr), (void const*)g_unknown_ieee_addr, 8))

/*
   Clear long address
 */
#define ZB_64BIT_ADDR_ZERO(addr)                \
  ZB_MEMSET((addr), 0, 8)

/*
   Make long address unknown
 */
#define ZB_64BIT_ADDR_UNKNOWN(addr)                \
  ZB_MEMSET((addr), -1, 8)

/*
   Copy long address
 */
#define ZB_64BIT_ADDR_COPY(dst, src) ZB_MEMCPY(dst, src, sizeof(zb_64bit_addr_t))

/*
   Return 1 if long addresses are equal
 */
#define ZB_64BIT_ADDR_CMP(one, two) ((zb_bool_t)!ZB_MEMCMP((one), (two), 8))

/*
  Long (64-bit) device address
 */
typedef zb_64bit_addr_t zb_ieee_addr_t;
/*
  Long (64-bit) Extented pan id
 */
typedef zb_64bit_addr_t zb_ext_pan_id_t;

#define ZB_EXTPANID_IS_ZERO ZB_IS_64BIT_ADDR_ZERO
#define ZB_EXTPANID_ZERO ZB_64BIT_ADDR_ZERO
#define ZB_EXTPANID_COPY ZB_64BIT_ADDR_COPY
#define ZB_EXTPANID_CMP ZB_64BIT_ADDR_CMP

#define ZB_IEEE_ADDR_IS_ZERO ZB_IS_64BIT_ADDR_ZERO
#define ZB_IEEE_ADDR_IS_UNKNOWN ZB_IS_64BIT_ADDR_UNKNOWN
#define ZB_IEEE_ADDR_ZERO ZB_64BIT_ADDR_ZERO
#define ZB_IEEE_ADDR_UNKNOWN ZB_64BIT_ADDR_UNKNOWN
#define ZB_IEEE_ADDR_COPY ZB_64BIT_ADDR_COPY
#define ZB_IEEE_ADDR_CMP ZB_64BIT_ADDR_CMP

#define ZB_CCM_KEY_IS_ZERO(k) (ZB_IS_64BIT_ADDR_ZERO((zb_uint8_t*)(k)) && ZB_IS_64BIT_ADDR_ZERO(((zb_uint8_t*)(k)) + 8))

#define ZB_ADDR_CMP(addr_mode, addr1, addr2)                            \
  ((addr_mode == ZB_ADDR_16BIT_DEV_OR_BROADCAST) ?                      \
   (addr1.addr_short == addr2.addr_short) : ZB_64BIT_ADDR_CMP(addr1.addr_long, addr2.addr_long))

typedef ZB_PACKED_PRE union zb_addr_u_t
{
  zb_uint16_t  addr_short;
  zb_ieee_addr_t addr_long;
} ZB_PACKED_STRUCT
zb_addr_u;

/*
 definitions for constants of given type
*/
#define ZB_INT8_C(c)  c
#define ZB_UINT8_C(c) c ## U

#define ZB_INT16_C(c)  c
#define ZB_UINT16_C(c) c ## U

#define ZB_INT32_C(c)  c ## L
#define ZB_UINT32_C(c) c ## UL

/* Compiler for Geniatech platform doesn't support __builtin_offsetof(),
   should we really use it on other platforms? */
/* 12/11/2017 EE CR:MINOR Check for GCC version and use __builtin_offsetof (like #if defined __GNUC__ && __GNUC__ >= 4 ...) */
/* #ifdef __GNUC__ */
/* #define ZB_OFFSETOF(t, f) __builtin_offsetof(t,f) */
/* #else */
#define ZB_OFFSETOF(t, f) (zb_size_t)(&((t *)0)->f)
/* #endif */

#define ZB_OFFSETOF_VAR(s, f) (zb_size_t)(((zb_int8_t *)(&(s)->f)) - ((zb_int8_t *)(s)))

#define ZB_SIZEOF_FIELD(type, field) (sizeof(((type*)0)->field))

#define ZB_ARRAY_SIZE(arr) (sizeof((arr))/sizeof((arr)[0]))

#define ZB_SIGNED_SHIFT(v, s) ((zb_int_t)(v) >> (s))

  #define ZB_ASSERT_IF_NOT_ALIGNED(data_type,length) \
  ZB_ASSERT_COMPILE_DECL(((sizeof(data_type) % (length)) == 0));

  #define ZB_ASSERT_IF_NOT_ALIGNED_TO_4(data_type) ZB_ASSERT_IF_NOT_ALIGNED(data_type,4)

/**
 *  @name Endian change API.
 *  Macros to change words endian and access words at potentially
 *  non-aligned pointers.
 *
 *  ZigBee uses little endian, see ZB spec, subclause 1.2.1.3.
 *  @{
 */

/**
 */

#define ZB_8BIT_SIZE 1
#define ZB_16BIT_SIZE 2
#define ZB_24BIT_SIZE 3
#define ZB_32BIT_SIZE 4
#define ZB_48BIT_SIZE 6
#define ZB_64BIT_SIZE 8

#ifdef ZB_LITTLE_ENDIAN

#ifdef ZB_NEED_ALIGN
/**
   Convert 16-bits integer from the host endian to the little endian

   @param ptr - destination pointer. It is ok if it not aligned to 2.
   @param val - source pointer. It is ok if it not aligned to 2.
*/

#define ZB_HTOLE16(ptr, val)   ZB_MEMCPY((ptr), (val), ZB_16BIT_SIZE)
#define ZB_HTOLE32(ptr, val)   ZB_MEMCPY((ptr), (val), ZB_32BIT_SIZE)
#define ZB_HTOLE64(ptr, val)   ZB_MEMCPY((ptr), (val), ZB_64BIT_SIZE)
#else

#define ZB_HTOLE16(ptr, val)   (((zb_uint16_t *)(ptr))[0] = *((zb_uint16_t *)(val)))
#define ZB_HTOLE32(ptr, val)   (((zb_uint32_t *)(ptr))[0] = *((zb_uint32_t *)(val)))
/*
#define ZB_HTOLE64(ptr, val)                              \
  (((zb_uint32_t *)(ptr))[0] = ((zb_uint32_t *)(val))[0], \
   ((zb_uint32_t *)(ptr))[1] = ((zb_uint32_t *)(val))[1])
*/
#define ZB_HTOLE64(ptr, val) zb_memcpy8(ptr, val)
#endif  /* need_align */

#define ZB_HTOLE16_ONPLACE(v)

#define ZB_HTOLE32_ONPLACE(v)

#define ZB_HTOLE64_ONPLACE(v)

#define ZB_HTOBE16(ptr, val)                            \
  (((zb_uint8_t *)(ptr))[0] = ((zb_uint8_t *)(val))[1], \
   ((zb_uint8_t *)(ptr))[1] = ((zb_uint8_t *)(val))[0]  \
  )

void zb_htobe32(zb_uint8_t ZB_XDATA *ptr, zb_uint8_t ZB_XDATA *val);
#define ZB_HTOBE32(ptr, val) zb_htobe32((zb_uint8_t*)(ptr), (zb_uint8_t*)(val))

#define ZB_HTOBE16_VAL(ptr, val)                \
{                                               \
  zb_uint16_t _v = (val);                       \
  ZB_HTOBE16((ptr), &_v);                       \
}

#else  /* !little endian */

void zb_htole16(zb_uint8_t ZB_XDATA *ptr, zb_uint8_t ZB_XDATA *val);
#define ZB_HTOLE16(ptr, val) zb_htole16((zb_uint8_t*)(ptr), (zb_uint8_t*)(val))

#define ZB_HTOLE16_ONPLACE(val) 		\
{						\
  zb_uint8_t *pval = (zb_uint8_t*)val;		\
  zb_uint8_t a = pval[0];			\
  pval[0] = pval[1]; 				\
  pval[1] = a;  				\
}

#define ZB_HTOLE32_ONPLACE(val) { zb_uint32_t _v = *(val); ZB_HTOLE32((val), &_v); }

void zb_htole32(zb_uint8_t ZB_XDATA *ptr, zb_uint8_t ZB_XDATA *val);
#define ZB_HTOLE32(ptr, val) zb_htole32((zb_uint8_t*)(ptr), (zb_uint8_t*)(val))

#define ZB_HTOBE32(ptr, val) ZB_MEMCPY(ptr, val, 4)

#define ZB_HTOBE16(ptr, val) (*(zb_uint16_t *)(ptr)) = *((zb_uint16_t *)(val))

#ifdef ZB_NEED_ALIGN

#define ZB_HTOBE16_VAL(ptr, val)                \
{                                               \
  zb_uint16_t _v = (val);                       \
  ZB_MEMCPY((ptr), &_v, 2);                     \
}

#else

#define ZB_HTOBE16_VAL(ptr, val) ((zb_uint16_t *)(ptr))[0] = (val)

#endif  /* ZB_NEED_ALIGN */

/* let's not rotate 64-bit address: store it as
 * 8-byte array in the order it transmitted */

#define ZB_HTOLE64(ptr, val)   zb_memcpy8(ptr, val)

#endif

#define ZB_HTOLE32_VAL(ptr, val)               \
{                                              \
  zb_uint32_t tmp_val = (val);                 \
  ZB_HTOLE32((ptr), &tmp_val);                 \
}

#define ZB_HTOLE16_VAL(ptr, val)               \
{                                              \
  zb_uint16_t tmp_val = (val);                 \
  ZB_HTOLE16((ptr), &tmp_val);                 \
}

/**
   Put next 2-bute value into buffer, move pointer

   To be used for headers compose.

   @param dst - (in/out) address os the buffer pointer
          As a side effect it will be incremented by 2.
   @param val - value
 */
void zb_put_next_htole16(zb_uint8_t **dst, zb_uint16_t val);

#ifdef ZB_LITTLE_ENDIAN
#define ZB_PUT_NEXT_HTOLE16(ptr, val)  \
{                                               \
*((ptr)++) = (val) & 0xff;                      \
*((ptr)++) = ((val) >> 8) & 0xff;               \
}

#else
#define ZB_PUT_NEXT_HTOLE16(ptr, val)  \
{                                               \
  *((ptr)++) = ((val) >> 8) & 0xff;             \
  *((ptr)++) = (val) & 0xff;                    \
}
#endif                          /* ZB_LITTLE_ENDIAN */

void* zb_put_next_htole32(zb_uint8_t *dst, zb_uint32_t val1);

#define ZB_PUT_NEXT_HTOLE32(ptr, val) (ptr) = zb_put_next_htole32((ptr), (val))

#ifndef ZB_IAR
void zb_get_next_letoh16(zb_uint16_t *dst, zb_uint8_t **src);
#else
/* for IAR define as macro due to problems with packet structs */
#define zb_get_next_letoh16(dst, src)           \
{                                               \
  ZB_LETOH16((dst), *(src));                    \
  (*(src)) = (void *)(((char *)(*(src))) + 2);  \
}
#endif

void* zb_put_next_2_htole16(zb_uint8_t *dst, zb_uint16_t val1, zb_uint16_t val2);
void* zb_put_next_2_htole32(zb_uint8_t *dst, zb_uint32_t val1, zb_uint32_t val2);
#define ZB_LETOH64(dst, src) zb_memcpy8((zb_uint8_t*)dst, (zb_uint8_t*)src)

/**
   Convert 16-bits integer from the little endian to the host endian

   @param ptr - destination pointer. It is ok if it not aligned to 2.
   @param val - source pointer. It is ok if it not aligned to 2.
*/
#define ZB_LETOH16 ZB_HTOLE16
#define ZB_LETOH24 ZB_HTOLE24
#define ZB_LETOH48 ZB_HTOLE48
#define ZB_LETOH32 ZB_HTOLE32
#define ZB_BETOH16 ZB_HTOBE16
#define ZB_BETOH32 ZB_HTOBE32

#define ZB_LETOH16_ONPLACE ZB_HTOLE16_ONPLACE
#define ZB_LETOH32_ONPLACE ZB_HTOLE32_ONPLACE

/** @} */ /* Endian change API. */

#define ZB_GET_LOW_BYTE(val) ((val) & 0xFF)
#define ZB_GET_HI_BYTE(val)  (((val) >> 8) & 0xFF)

#define ZB_SET_LOW_BYTE(res, val) (res) = (((res) & 0xFF00) | ((val) & 0xFF))
#define ZB_SET_HI_BYTE(res, val) (res) = ((((val) << 8) & 0xFF00) | ((res) & 0xFF))

#define ZB_PKT_16B_ZERO_BYTE 0
#define ZB_PKT_16B_FIRST_BYTE 1

#ifdef ZB_NEED_ALIGN

#define ZB_ASSIGN_UINT16(ptr, vp) ZB_MEMCPY((ptr), (vp), 2)
#define ZB_ASSIGN_INT16(ptr, vp)  ZB_MEMCPY((ptr), (vp), 2)
#define ZB_COPY_UINT24(ptr, vp)   ZB_MEMCPY((ptr), (vp), 3)
#define ZB_COPY_INT24(ptr, vp)    ZB_MEMCPY((ptr), (vp), 3)
#define ZB_ASSIGN_UINT32(ptr, vp) ZB_MEMCPY((ptr), (vp), 4)
#define ZB_ASSIGN_INT32(ptr, vp)  ZB_MEMCPY((ptr), (vp), 4)

#else

#define ZB_ASSIGN_UINT16(ptr, vp) *((zb_uint16_t*)(ptr)) = *(zb_uint16_t*)(vp)
#define ZB_ASSIGN_INT16(ptr, vp)  *((zb_int16_t*)(ptr)) = *(zb_int16_t*)(vp)
#define ZB_COPY_UINT24(ptr, vp)   *((zb_uint24_t*)(ptr)) = *(zb_uint24_t*)(vp)
#define ZB_COPY_INT24(ptr, vp)    *((zb_int24_t*)(ptr)) = *(zb_int24_t*)(vp)
#define ZB_ASSIGN_UINT32(ptr, vp) *((zb_uint32_t*)(ptr)) = *(zb_uint32_t*)(vp)
#define ZB_ASSIGN_INT32(ptr, vp)  *((zb_int32_t*)(ptr)) = *(zb_int32_t*)(vp)

#endif  /* ZB_NEED_ALIGN */

#define ZB_ARRAY_IDX_BY_ELEM(ar, el) ((el) - (ar))

#define ZB_ARRAY_IDX_BY_STRUCT_ELEM(ar, el, el_size) ( ((zb_size_t)(el) - (zb_size_t)(ar)) / (el_size) )

/**
 *  @name Base types.
 *  @{
 *    @typedef zb_char_t
 *    @brief Project-local char type.
 *
 *    @typedef zb_uchar_t
 *    @brief Project-local unsigned char type.
 *
 *    @typedef zb_uint8_t
 *    @brief Project-local 1-byte unsigned int type.
 *
 *    @typedef zb_int8_t
 *    @brief Project-local 1-byte signed int type.
 *
 *    @typedef zb_uint16_t
 *    @brief Project-local 2-byte unsigned int type.
 *
 *
 *    @typedef zb_int16_t
 *    @brief Project-local 2-byte signed int type.
 *
 *    @typedef zb_int32_t
 *    @brief Project-local 4-byte signed int type.
 *
 *    @typedef zb_uint32_t
 *    @brief Project-local 4-byte unsigned int type.
 *
 *    @typedef zb_bitfield_t
 *    @brief Type to be used for unsigned bit fields inside structure.
 *
 *    @typedef zb_sbitfield_t
 *    @brief Type to be used for signed bit fields inside structure.
 *
 *    @typedef zb_size_t
 *    @brief Project-local size_t type.
 *
 *    @typedef zb_short_t
 *    @brief Short int (can fit into single CPU register).
 *
 *    @typedef zb_ushort_t
 *    @brief Short unsigned int (can fit into single CPU register).
 *
 *    @typedef zb_int_t
 *    @brief Int (at least 2 bytes).
 *
 *    @typedef zb_uint_t
 *    @brief Unsigned int (at least 2 bytes).
 *
 *    @typedef zb_long_t
 *    @brief Long int (at least 4 bytes).
 *
 *    @typedef zb_ulong_t
 *    @brief Unsigned long int (at least 4 bytes).
 *
 *    @typedef zb_void_t
 *    @brief Project-local void type.
 *
 *    @typedef zb_voidp_t
 *    @brief Project-local "pointer to void" type.
 *
 *    @typedef zb_bool_t
 *    @brief Boolean type can be ZB_TRUE or ZB_FALSE
 *  @}
 */

/**
 *  @name Address types and API.
 *  @{
 *    @typedef zb_64bit_addr_t
 *    @brief 8-bytes address (xpanid or long device address) base type.
 *
 *    @typedef zb_ieee_addr_t
 *    @brief Long (64-bit) device address.
 *
 *    @typedef zb_ext_pan_id_t
 *    @brief Long (64-bit) extended PAN Id.
 *
 *    @union zb_addr_u
 *    @brief Union to address either long or short address.
 *
 *    @def ZB_IS_64BIT_ADDR_ZERO
 *    @brief Checks that long address is a zero one.
 *    @param addr address to check.
 *    @returns ZB_TRUE if address is zero, ZB_FALSE otherwise.
 *
 *    @def ZB_64BIT_ADDR_ZERO
 *    @brief Clears long address.
 *    @param addr address to clear.
 *
 *    @def ZB_64BIT_ADDR_COPY
 *    @brief Copy long address.
 *    @param dst where to copy the address.
 *    @param src address to copy.
 *
 *    @def ZB_64BIT_ADDR_CMP
 *    @brief Check two long addresses are equal.
 *    @param one address to compare.
 *    @param two address to compare to.
 *    @returns ZB_TRUE if addresses are equal, ZB_FALSE otherwise.
 *  @}
 */

#define MIN_SIGNED_24BIT_VAL ((zb_int32_t)0xFF800001)  /* FIXME: which value to prefer? Because 0x800000 is reserved in ZCL */
#define MAX_SIGNED_24BIT_VAL ((zb_int32_t)0x7FFFFF)

#define MIN_SIGNED_48BIT_VAL ((zb_int32_t)0xFFFF800000000001)
#define MAX_SIGNED_48BIT_VAL ((zb_int32_t)0x7FFFFFFFFFFF)

#define ZB_S64_FROM_S48(x) ((x & 0xFFFFFFFFFFFF) | ((x & 0x800000000000) ? 0xFFFF000000000000 : 0x0))

#define ZB_S64_TO_S48(x) ((zb_int64_t)(x & 0xFFFFFFFFFFFF))

#define ZB_S32_FROM_S24(x) ((x & 0xFFFFFF) | ((x & 0x800000) ? 0xFF000000 : 0x0))

#define ZB_S32_TO_S24(x) ((zb_int32_t)(x & 0xFFFFFF))

#ifdef ZB_UINT24_48_SUPPORT

#define ZB_INT32_MINUS_ONE    (0xFFFFFFFF)
#define ZB_INT24_MAX_POSITIVE (0x7FFFFF)

#define ZB_INT24_IS_NEGATIVE(int24_val) \
  ((int24_val).high & 0x80)

#if defined ZB_LITTLE_ENDIAN

typedef ZB_PACKED_PRE struct zb_int24_s
{
  zb_uint16_t low;
  zb_int8_t   high;
} ZB_PACKED_STRUCT zb_int24_t;

typedef ZB_PACKED_PRE struct zb_uint24_s
{
  zb_uint16_t low;
  zb_uint8_t high;
} ZB_PACKED_STRUCT zb_uint24_t;

typedef ZB_PACKED_PRE struct zb_uint48_s
{
  zb_uint32_t low;
  zb_uint16_t high;
} ZB_PACKED_STRUCT zb_uint48_t;

typedef ZB_PACKED_PRE struct zb_int48_s
{
  zb_uint32_t low;
  zb_int16_t high;
} ZB_PACKED_STRUCT zb_int48_t;

#else /* defined ZB_BIG_ENDIAN */

typedef ZB_PACKED_PRE struct zb_int24_s
{
  zb_int8_t   high;
  zb_uint16_t low;
} ZB_PACKED_STRUCT zb_int24_t;

typedef ZB_PACKED_PRE struct zb_uint24_s
{
  zb_uint8_t high;
  zb_uint16_t low;
} ZB_PACKED_STRUCT zb_uint24_t;

typedef ZB_PACKED_PRE struct zb_uint48_s
{
  zb_uint16_t high;
  zb_uint32_t low;
} ZB_PACKED_STRUCT zb_uint48_t;

typedef ZB_PACKED_PRE struct zb_int48_s
{
  zb_int16_t high;
  zb_uint32_t low;
} ZB_PACKED_STRUCT zb_int48_t;

#endif /* ZB_LITTLE_ENDIAN/ZB_BIG_ENDIAN */

#define ZB_INIT_INT24(high_val, low_val) \
{                                          \
  .low = (low_val),                        \
  .high = (high_val)                       \
}

#define ZB_ASSIGN_INT24(var, high_val, low_val)  \
{                                          \
  var.low = (low_val);                     \
  var.high = (high_val);                   \
}

#define ZB_GET_INT32_FROM_INT24(int24_val)                             \
  (zb_int32_t)                                                         \
  (                                                                    \
   ((ZB_INT24_IS_NEGATIVE(int24_val))? ((zb_uint32_t)0xFF << 24): 0) | \
   ((zb_uint32_t)(int24_val).high << 16)                             | \
   (int24_val).low                                                     \
  )

#define ZB_ASSIGN_UINT24_FROM_UINT32(uint32_val)  \
{                                                 \
  .low = (zb_uint16_t)((uint32_val) & 0xFFFF),    \
    .high = (zb_uint8_t)((uint32_val) >> 16)      \
}

#define ZB_GET_UINT32_FROM_UINT24(uint24_val)    \
  (((uint24_val).high << 16) + (uint24_val).low)

#define ZB_INIT_UINT48(high_val, low_val)  \
{                                          \
  .low = (low_val),                        \
  .high = (high_val)                       \
}

#define ZB_INIT_UINT24(high_val, low_val)  \
{                                          \
  .low = (low_val),                        \
  .high = (high_val)                       \
}

#define ZB_ASSIGN_UINT48(var, high_val, low_val) \
{                                          \
  var.low = (low_val);                        \
  var.high = (high_val);                      \
}

#define ZB_ASSIGN_UINT24(var, high_val, low_val) \
{                                                                    \
  var.low = (low_val);                        \
  var.high = (high_val);                      \
}

#define ZB_UINT48_IS_EQUAL(val1, val2)                               \
  (((val1).high == (val2).high) && ((val1).low == (val2).low))

#define ZB_UINT48_IS_GREAT(val1, val2)                               \
  ((val1).high > (val2).high ||                                      \
   ((val1).high == (val2).high && (val1).low > (val2).low))

#define ZB_UINT48_IS_LESS(val1, val2)                                \
  (!ZB_UINT48_IS_EQUAL(val1, val2) && !ZB_UINT48_IS_GREAT(val1, val2))

#define ZB_UINT48_IS_GREAT_OR_EQUAL(val1, val2)                      \
  (!ZB_UINT48_IS_LESS(val1, val2))

#define ZB_UINT48_IS_LESS_OR_EQUAL(val1, val2)                       \
  (!ZB_UINT48_IS_GREAT(val1, val2))

#if defined ZB_LITTLE_ENDIAN

#define ZB_HTOLE24(ptr, val)   ZB_MEMCPY((ptr), (val), ZB_24BIT_SIZE)
#define ZB_HTOLE48(ptr, val)   ZB_MEMCPY((ptr), (val), ZB_48BIT_SIZE)

#else /* ZB_BIG_ENDIAN */

zb_void_t zb_reverse_bytes(zb_uint8_t *ptr, zb_uint8_t *val, zb_uint8_t size);

#define ZB_HTOLE24(ptr, val)   zb_reverse_bytes((zb_uint8_t*)(ptr), (val), ZB_24BIT_SIZE)
#define ZB_HTOLE48(ptr, val)   zb_reverse_bytes((zb_uint8_t*)(ptr), (val), ZB_48BIT_SIZE)

#endif /* ZB_BIG_ENDIAN */

/**
 * @name 24-bit and 48-bit arithmetic API
 * @{
 */

/** Return statuses of mathematical operations */
enum zb_math_status_e
{
  ZB_MATH_OK = 0,
  ZB_MATH_OVERFLOW,
  ZB_MATH_ERROR,
};

/**
 * Convert unsigned 32-bit value to unsigned 24-bit value
 * @param[in]  var - unsigned 32-bit value
 * @param[out]  res - pointer to unsigned 24-bit value
 */
zb_void_t zb_uint32_to_uint24(zb_uint32_t var, zb_uint24_t *res);

/**
 * Convert signed 24-bit value to signed 32-bit value
 * @param[in]  var - pointer to signed 24-bit value
 * @return  signed 32-bit representation of 24-bit value
 */
zb_int32_t zb_int24_to_int32(const zb_int24_t *var);

/**
 * Convert signed 32-bit value to signed 24-bit value
 * @param[in]  var - signed 32-bit value
 * @param[out]  res - pointer to signed 24-bit value
 */
zb_void_t zb_int32_to_int24(zb_int32_t var, zb_int24_t *res);

/**
 * Convert signed 32-bit value to unsigned 24-bit value
 * @param[in]  var - signed 32-bit value
 * @param[out]  res - pointer to unsigned 24-bit value
 */
zb_void_t zb_int32_to_uint24(zb_int32_t var, zb_uint24_t *res);

/**
 * Convert unsigned 24-bit value to signed 32-bit value
 * @param[in]  var - pointer to unsigned 24-bit value
 * @return signed 32-bit representation of unsigned 24-bit value
 */
zb_int32_t zb_uint24_to_int32(const zb_uint24_t *var);

/**
 * Addition of two unsigned 24-bit values
 * @param[in]  f - pointer to first unsigned 24-bit operand
 * @param[in]  s - pointer to second unsigned 24-bit operand
 * @param[out]  r - pointer to unsigned 24-bit variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint24_add(const zb_uint24_t *f, const zb_uint24_t *s, zb_uint24_t *r);

/**
 * Addition of two signed 24-bit values
 * @param[in]  f - pointer to first signed 24-bit operand
 * @param[in]  s - pointer to second signed 24-bit operand
 * @param[out]  r - pointer to 24-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int24_add(const zb_int24_t *f, const zb_int24_t *s, zb_int24_t *r);

/**
 * Subtraction of two signed 24-bit values
 * @param[in]  f - pointer to first signed 24-bit operand
 * @param[in]  s - pointer to second signed 24-bit operand
 * @param[out]  r - pointer to 24-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int24_sub(const zb_int24_t *f, const zb_int24_t *s, zb_int24_t *r);

/**
 * Subtraction of two unsigned 24-bit values
 * @param[in]  f - pointer to first unsigned 24-bit operand
 * @param[in]  s - pointer to second unsigned 24-bit operand
 * @param[out]  r - pointer to 24-bit unsigned variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint24_sub(const zb_uint24_t *f, const zb_uint24_t *s, zb_uint24_t *r);

/**
 * Negation of signed 24-bit values
 * @param[in]  f - pointer to signed 24-bit operand
 * @param[out]  r - pointer to 24-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int24_neg(const zb_int24_t *f, zb_int24_t *r);

/**
 * Multiplication of two unsigned 24-bit values
 * @param[in]  f - pointer to first unsigned 24-bit operand
 * @param[in]  s - pointer to second unsigned 24-bit operand
 * @param[out]  r - pointer to 24-bit unsigned variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint24_mul(const zb_uint24_t *f, const zb_uint24_t *s, zb_uint24_t *r);

/**
 * Multiplication of two signed 24-bit values
 * @param[in]  f - pointer to first signed 24-bit operand
 * @param[in]  s - pointer to second signed 24-bit operand
 * @param[out]  r - pointer to 24-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int24_mul(const zb_int24_t *f, const zb_int24_t *s, zb_int24_t *r);

/**
 * Integer division of two unsigned 24-bit values
 * @param[in]  f - pointer to first unsigned 24-bit operand
 * @param[in]  s - pointer to second unsigned 24-bit operand
 * @param[out]  r - pointer to unsigned 24-bit variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint24_div(const zb_uint24_t *f, const zb_uint24_t *s, zb_uint24_t *r);

/**
 * Integer division of two signed 24-bit values
 * @param[in]  f - pointer to first signed 24-bit operand
 * @param[in]  s - pointer to second signed 24-bit operand
 * @param[out]  r - pointer to 24-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int24_div(const zb_int24_t *f, const zb_int24_t *s, zb_int24_t *r);

/**
 * Modular division of two unsigned 24-bit values
 * @param[in]  f - pointer to first unsigned 24-bit operand
 * @param[in]  s - pointer to second unsigned 24-bit operand
 * @param[out]  r - pointer to unsigned 24-bit variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint24_mod(const zb_uint24_t *f, const zb_uint24_t *s, zb_uint24_t *r);

/**
 * Modular division of two signed 24-bit values
 * @param[in]  f - pointer to first signed 24-bit operand
 * @param[in]  s - pointer to second signed 24-bit operand
 * @param[out]  r - pointer to 24-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int24_mod(const zb_int24_t *f, const zb_int24_t *s, zb_int24_t *r);

/**
 * Convert 64-bit value to 48-bit representation
 * @param[in]  var - 64-bit value
 * @param[out]  res - pointer to 48-bit variable
 */
zb_void_t zb_uint64_to_uint48(zb_uint64_t var, zb_uint48_t *res);

/**
 * Convert signed 48-bit value to signed 64-bit value
 * @param[in]  var - pointer to signed 48-bit value
 * @return signed 64-bit representation of signed 48-bit value
 */
zb_int64_t zb_int48_to_int64(const zb_int48_t *var);

/**
 * Convert signed 64-bit value to signed 48-bit value
 * @param[in]  var - signed 64-bit value
 * @param[out] res - pointer to signed 48-bit value
 */
zb_void_t zb_int64_to_int48(zb_int64_t var, zb_int48_t *res);

/**
 * Convert signed 64-bit value to unsigned 48-bit value
 * @param[in]  var - signed 64-bit value
 * @param[out]  res - unsigned 48-bit value
 * @return     [description]
 */
zb_void_t zb_int64_to_uint48(zb_int64_t var, zb_uint48_t *res);

/**
 * Convert unsigned 48-bit value to signed 64-bit value
 * @param[in]  var - pointer to unsigned 64-bit value
 * @return signed 64-bit representation of unsigned 48-bit value
 */
zb_int64_t zb_uint48_to_int64(const zb_uint48_t *var);

/**
 * Addition of two unsigned 48-bit values
 * @param[in]  f - pointer to first insigne4824-bit operand
 * @param[in]  s - pointer to second insigne4824-bit operand
 * @param[out]  r - pointer to 48signed 24-bit variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint48_add(const zb_uint48_t *f, const zb_uint48_t *s, zb_uint48_t *r);

/**
 * Addition of two signed 48-bit values
 * @param[in]  f - pointer to first signed 48-bit operand
 * @param[in]  s - pointer to second signed 48-bit operand
 * @param[out]  r - pointer to 48-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int48_add(const zb_int48_t *f, const zb_int48_t *s, zb_int48_t *r);

/**
 * Subtraction of two signed 48-bit values
 * @param[in]  f - pointer to first signed 48-bit operand
 * @param[in]  s - pointer to second signed 48-bit operand
 * @param[out]  r - pointer to 48-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int48_sub(const zb_int48_t *f, const zb_int48_t *s, zb_int48_t *r);

/**
 * Subtraction of two unsigned 48-bit values
 * @param[in]  f - pointer to first unsigne4824-bit operand
 * @param[in]  s - pointer to second unsigne4824-bit operand
 * @param[out]  r - pointer to 48-bit unsigned variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint48_sub(const zb_uint48_t *f, const zb_uint48_t *s, zb_uint48_t *r);

/**
 * Negation of signed 24-bit values
 * @param[in]  f - pointer to signed 24-bit operand
 * @param[out]  r - pointer to 24-bit signed variable to store result of operation
 * @return  @e ZB_MATH_OK on success and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int48_neg(const zb_int48_t *f, zb_int48_t *r);

/**
 * Multiplication of two unsigned 48-bit values
 * @param[in]  f - pointer to first unsigned 48-bit operand
 * @param[in]  s - pointer to second unsigned 48-bit operand
 * @param[out]  r - pointer to 48-bit unsigned variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint48_mul(const zb_uint48_t *f, const zb_uint48_t *s, zb_uint48_t *r);

/**
 * Multiplication of two signed 48-bit values
 * @param[in]  f - pointer to first signed 48-bit operand
 * @param[in]  s - pointer to second signed 48-bit operand
 * @param[out]  r - pointer to 48-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int48_mul(const zb_int48_t *f, const zb_int48_t *s, zb_int48_t *r);

/**
 * Integer division of two unsigned 48-bit values
 * @param[in]  f - pointer to first unsigned 48-bit operand
 * @param[in]  s - pointer to second unsigned 48-bit operand
 * @param[out]  r - pointer to unsigned 48-bit variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint48_div(const zb_uint48_t *f, const zb_uint48_t *s, zb_uint48_t *r);

/**
 * Integer division of two signed 48-bit values
 * @param[in]  f - pointer to first signed 48-bit operand
 * @param[in]  s - pointer to second signed 48-bit operand
 * @param[out]  r - pointer to 48-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int48_div(const zb_int48_t *f, const zb_int48_t *s, zb_int48_t *r);

/**
 * Modular division of two unsigned 48-bit values
 * @param[in]  f - pointer to first unsigned 48-bit operand
 * @param[in]  s - pointer to second unsigned 48-bit operand
 * @param[out]  r - pointer to unsigned 48-bit variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_uint48_mod(const zb_uint48_t *f, const zb_uint48_t *s, zb_uint48_t *r);

/**
 * Modular division of two signed 48-bit values
 * @param[in]  f - pointer to first signed 48-bit operand
 * @param[in]  s - pointer to second signed 48-bit operand
 * @param[out]  r - pointer to 48-bit signed variable to store result of operation
 * @return   @e ZB_MATH_OK on success, @e ZB_MATH_OVERFLOW on arithmetic overflow
 * and @e ZB_MATH_ERROR on failure (see @ref zb_math_status_e).
 */
zb_uint8_t zb_int48_mod(const zb_int48_t *f, const zb_int48_t *s, zb_int48_t *r);

/** @} */ /* 24_and_48_bit_arithm */

#define ZB_UINT48_ADD(val1, val2)  zb_uint48_add(&val1, &val2, &val1)

#define ZB_UINT48_SUB(val1, val2)  zb_uint48_sub(&val1, &val2, &val1)

#define ZB_INT24_FROM_INT32(var, int32_val)                                                       \
{                                                                                                 \
  if (int32_val >= 0)                                                                             \
  {                                                                                               \
    if (int32_val > (zb_int32_t)ZB_INT24_MAX_POSITIVE)                                            \
    {                                                                                             \
      int32_val = (zb_int32_t)ZB_INT24_MAX_POSITIVE;                                              \
    }                                                                                             \
  }                                                                                               \
  else                                                                                            \
  {                                                                                               \
    if (int32_val < (zb_int32_t)ZB_INT32_MINUS_ONE - (zb_int32_t)ZB_INT24_MAX_POSITIVE)           \
    {                                                                                             \
      int32_val = (zb_int32_t)ZB_INT32_MINUS_ONE - (zb_int32_t)ZB_INT24_MAX_POSITIVE;             \
    }                                                                                             \
  }                                                                                               \
  zb_int32_to_int24(int32_val, &var);                                                             \
}

#else /* ZB_UINT24_48_SUPPORT */

/* Now U48 type implemented as U32 */
/* Now S24 type implemented as S32 */

typedef zb_uint64_t  zb_uint48_t;
typedef zb_int64_t   zb_int48_t;
typedef zb_int32_t            zb_int24_t;
typedef zb_uint32_t           zb_uint24_t;

#define ZB_INIT_UINT48(high_val, low_val)     \
  (high_val)

#define ZB_ASSIGN_UINT48(high_val, low_val)     \
  (high_val)

/* Stubs for operations with this types */
#define ZB_UINT48_ADD(val1, val2) ((val1) + (val2))
#define ZB_UINT48_SUB(val1, val2) ((val1) - (val2))

#define ZB_ASSIGN_INT24_FROM_INT32(int32_val) \
  (int32_val)

#define ZB_GET_INT32_FROM_INT24(int24_val)    \
  (int24_val)

#define ZB_ASSIGN_UINT24_FROM_UINT32(uint32_val) \
  (uint32_val)

#define ZB_GET_UINT32_FROM_UINT24(uint24_val)    \
  (uint24_val)

#ifdef ZB_LITTLE_ENDIAN

#define ZB_HTOLE24(ptr, val)   ZB_MEMCPY((ptr), (val), ZB_24BIT_SIZE)
#define ZB_HTOLE48(ptr, val)   ZB_MEMCPY((ptr), (val), ZB_48BIT_SIZE)

#else

#define ZB_HTOLE24(ptr, val)   ZB_HTOLE32((ptr), (val))
#define ZB_HTOLE48(ptr, val)   ZB_HTOLE32((ptr), (val))

#endif

#define ZB_UINT48_IS_EQUAL(val1, val2)                               \
  ((val1) == (val2))

#define ZB_UINT48_IS_GREAT(val1, val2)                               \
  ((val1) > (val2))

#define ZB_UINT48_IS_LESS(val1, val2)                                \
  ((val1) < (val2))

#define ZB_UINT48_IS_GREAT_OR_EQUAL(val1, val2)                      \
  (!ZB_UINT48_IS_LESS(val1, val2))

#define ZB_UINT48_IS_LESS_OR_EQUAL(val1, val2)                       \
  (!ZB_UINT48_IS_GREAT(val1, val2))

#endif /* ZB_UINT24_48_SUPPORT */

#define PUT_DATA24(ptr, val) (ZB_HTOLE24(ptr, val), (ptr) += ZB_24BIT_SIZE)
#define PUT_DATA24_VAL(ptr, val)              \
{                                             \
  zb_int24_t tmp_val = (val);                 \
  ZB_HTOLE24((ptr), &tmp_val);                \
  (ptr) += ZB_24BIT_SIZE;                \
}

#define PUT_DATA48(ptr, val) (ZB_HTOLE48(ptr, val), (ptr) += ZB_48BIT_SIZE)
#define PUT_DATA48_VAL(ptr, val)              \
{                                             \
  zb_uint48_t tmp_val = (val);                \
  ZB_HTOLE48((ptr), &tmp_val);                \
  (ptr) += ZB_48BIT_SIZE;               \
}

/* take MSB nibble from uint8_t */
#define ZB_UINT8_MSB_NIBBLE(X) (((X) >> 4) & 0x0F)

/* take LSB nibble from uint8_t */
#define ZB_UINT8_LSB_NIBBLE(X) ((X) & 0x0F)

/* convert pair of nibbles into uint8_t */
#define ZB_UINT4x2_TO_UINT8(MSB_N, LSB_N) \
  ((MSB_N << 4) | ZB_UINT8_LSB_NIBBLE(LSB_N))

/** @} */

#endif /* ! defined ZB_TYPES_H */
