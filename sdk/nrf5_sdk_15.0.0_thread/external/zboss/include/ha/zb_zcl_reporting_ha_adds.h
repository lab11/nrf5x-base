/**
 * ZBOSS Zigbee 3.0
 * 
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * 
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
 * 
 * PURPOSE: HA extensions for ZCL reporting scheme
*/

#if ! defined ZB_ZCL_REPORTING_HA_ADDS_H
#define ZB_ZCL_REPORTING_HA_ADDS_H

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_REPORTING_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
  * @{
  *   @details
  *   HA extensions for ZCL reporting.
  *   See HA spec 7.1.5
  */

/*! Boundary value for minimum reporting interval for HA profile */
#define ZB_HA_MIN_REPORTING_INTERVAL          0x0001

/*! Minimum value of maximum reporting interval for HA profile */
#define ZB_HA_MAX_REPORTING_INTERVAL_BOUNDARY 0x003C

/*! Check minimum and maximum reporting intervals fields of
 *  configure reporting command
 *
 *  @param min_interval    minimum reporting interval 
 *  @param max_interval    maximum reporting interval 
 *  @returns ZB_TRUE if reporting intervals are valid, ZB_FALSE otherwise
 */
#define ZB_HA_CHECK_REPORTING_INTERVALS(min_interval, max_interval) \
( (((max_interval) == 0) \
     || ((max_interval) >= ZB_HA_MAX_REPORTING_INTERVAL_BOUNDARY)) \
  && ((min_interval) >= ZB_HA_MIN_REPORTING_INTERVAL))

/*! @} */ /* ZB_ZCL_REPORTING_HA_ADDS */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ! defined ZB_ZCL_REPORTING_HA_ADDS */
