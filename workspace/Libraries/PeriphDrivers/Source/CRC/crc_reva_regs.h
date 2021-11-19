/**
 * @file    crc_reva_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the CRC_REVA Peripheral Module.
 */

/* ****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *
 *************************************************************************** */

#ifndef _CRC_REVA_REGS_H_
#define _CRC_REVA_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
 
#if defined (__ICCARM__)
  #pragma system_include
#endif
 
#if defined (__CC_ARM)
  #pragma anon_unions
#endif
/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     crc_reva
 * @defgroup    crc_reva_registers CRC_REVA_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the CRC_REVA Peripheral Module.
 * @details CRC Registers.
 */

/**
 * @ingroup crc_reva_registers
 * Structure type to access the CRC_REVA Registers.
 */
typedef struct {
    __IO uint32_t ctrl;                 /**< <tt>\b 0x0000:</tt> CRC_REVA CTRL Register */
    __IO uint32_t datain;               /**< <tt>\b 0x0004:</tt> CRC_REVA DATAIN Register */
    __IO uint32_t poly;                 /**< <tt>\b 0x0008:</tt> CRC_REVA POLY Register */
    __IO uint32_t val;                  /**< <tt>\b 0x000C:</tt> CRC_REVA VAL Register */
} mxc_crc_reva_regs_t;

/* Register offsets for module CRC_REVA */
/**
 * @ingroup    crc_reva_registers
 * @defgroup   CRC_REVA_Register_Offsets Register Offsets
 * @brief      CRC_REVA Peripheral Register Offsets from the CRC_REVA Base Peripheral Address. 
 * @{
 */
 #define MXC_R_CRC_REVA_CTRL                ((uint32_t)0x00000000UL) /**< Offset from CRC_REVA Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_CRC_REVA_DATAIN              ((uint32_t)0x00000004UL) /**< Offset from CRC_REVA Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_CRC_REVA_POLY                ((uint32_t)0x00000008UL) /**< Offset from CRC_REVA Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_CRC_REVA_VAL                 ((uint32_t)0x0000000CUL) /**< Offset from CRC_REVA Base Address: <tt> 0x000C</tt> */ 
/**@} end of group crc_reva_registers */

/**
 * @ingroup  crc_reva_registers
 * @defgroup CRC_REVA_CTRL CRC_REVA_CTRL
 * @brief    AES SRAM KEY
 * @{
 */
 #define MXC_F_CRC_REVA_CTRL_EN_POS                     0 /**< CTRL_EN Position */
 #define MXC_F_CRC_REVA_CTRL_EN                         ((uint32_t)(0x1UL << MXC_F_CRC_REVA_CTRL_EN_POS)) /**< CTRL_EN Mask */

 #define MXC_F_CRC_REVA_CTRL_DMA_EN_POS                 1 /**< CTRL_DMA_EN Position */
 #define MXC_F_CRC_REVA_CTRL_DMA_EN                     ((uint32_t)(0x1UL << MXC_F_CRC_REVA_CTRL_DMA_EN_POS)) /**< CTRL_DMA_EN Mask */

 #define MXC_F_CRC_REVA_CTRL_MSB_POS                    2 /**< CTRL_MSB Position */
 #define MXC_F_CRC_REVA_CTRL_MSB                        ((uint32_t)(0x1UL << MXC_F_CRC_REVA_CTRL_MSB_POS)) /**< CTRL_MSB Mask */

 #define MXC_F_CRC_REVA_CTRL_BYTE_SWAP_IN_POS           3 /**< CTRL_BYTE_SWAP_IN Position */
 #define MXC_F_CRC_REVA_CTRL_BYTE_SWAP_IN               ((uint32_t)(0x1UL << MXC_F_CRC_REVA_CTRL_BYTE_SWAP_IN_POS)) /**< CTRL_BYTE_SWAP_IN Mask */

 #define MXC_F_CRC_REVA_CTRL_BYTE_SWAP_OUT_POS          4 /**< CTRL_BYTE_SWAP_OUT Position */
 #define MXC_F_CRC_REVA_CTRL_BYTE_SWAP_OUT              ((uint32_t)(0x1UL << MXC_F_CRC_REVA_CTRL_BYTE_SWAP_OUT_POS)) /**< CTRL_BYTE_SWAP_OUT Mask */

 #define MXC_F_CRC_REVA_CTRL_BUSY_POS                   16 /**< CTRL_BUSY Position */
 #define MXC_F_CRC_REVA_CTRL_BUSY                       ((uint32_t)(0x1UL << MXC_F_CRC_REVA_CTRL_BUSY_POS)) /**< CTRL_BUSY Mask */

/**@} end of group CRC_REVA_CTRL_Register */

/**
 * @ingroup  crc_reva_registers
 * @defgroup CRC_REVA_DATAIN CRC_REVA_DATAIN
 * @brief    CRC Data Input
 * @{
 */
 #define MXC_F_CRC_REVA_DATAIN_DATA_POS                 0 /**< DATAIN_DATA Position */
 #define MXC_F_CRC_REVA_DATAIN_DATA                     ((uint32_t)(0xFFFFFFFFUL << MXC_F_CRC_REVA_DATAIN_DATA_POS)) /**< DATAIN_DATA Mask */

/**@} end of group CRC_REVA_DATAIN_Register */

/**
 * @ingroup  crc_reva_registers
 * @defgroup CRC_REVA_POLY CRC_REVA_POLY
 * @brief    CRC Polynomial
 * @{
 */
 #define MXC_F_CRC_REVA_POLY_POLY_POS                   0 /**< POLY_POLY Position */
 #define MXC_F_CRC_REVA_POLY_POLY                       ((uint32_t)(0xFFFFFFFFUL << MXC_F_CRC_REVA_POLY_POLY_POS)) /**< POLY_POLY Mask */

/**@} end of group CRC_REVA_POLY_Register */

/**
 * @ingroup  crc_reva_registers
 * @defgroup CRC_REVA_VAL CRC_REVA_VAL
 * @brief    Current CRC Value
 * @{
 */
 #define MXC_F_CRC_REVA_VAL_VALUE_POS                   0 /**< VAL_VALUE Position */
 #define MXC_F_CRC_REVA_VAL_VALUE                       ((uint32_t)(0xFFFFFFFFUL << MXC_F_CRC_REVA_VAL_VALUE_POS)) /**< VAL_VALUE Mask */

/**@} end of group CRC_REVA_VAL_Register */

#ifdef __cplusplus
}
#endif

#endif /* _CRC_REVA_REGS_H_ */