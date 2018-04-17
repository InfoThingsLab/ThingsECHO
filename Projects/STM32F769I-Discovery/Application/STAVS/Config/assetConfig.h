/**
  ******************************************************************************
  * @file    assetConfig.h
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    26-January-20167
  * @brief   asset configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#ifndef __ASSERTCONFIG_H__
#define __ASSERTCONFIG_H__

/* Define to prevent recursive inclusion -------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This File is intended to be included keil .sct file
 * The purposse is to avoid definition of empty sections when assets are already  flashed to QSPI (AVS_USE_QSPI_FLASHED  defined)
 * In this case (empty section) the Keil linker would complain
 */

/* Flag AVS_USE_QSPI_FLASHED shall be defined at projetc levels*/
/*  defined   :  Assets are already flashed in qspi ( save time to build and  flash the product). */
/*  undefined :  Generate exe with asset in qspi sections. */

/* Flag AVS_CREATE_QSPI_SECTION is used for Keil .sct file only */
/*  defined   :  only if AVS_USE_QSPI_FLASHED is undefined  */
/*  undefined :  only if AVS_USE_QSPI_FLASHED is defined . */
#if defined ( __CC_ARM   ) 
/*#define AVS_CREATE_QSPI_SECTION */
#endif

#define AVS_USE_QSPI             /* set the flag if the assets must be placed in qspi ( product with full assets) if unset the assets will go in flash ( hex must be flash once witgh STutility)*/

#ifdef __cplusplus
}
#endif
#endif /* __ASSERTCONFIG_H__ */

