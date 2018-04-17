/**
******************************************************************************
* @file    avs_sys_rng.c
* @author  MCD Application Team
* @version V1.2.1
* @date    20-02-2018
* @brief   mbedtls alternate entropy data function.
*          the mbedtls_hardware_poll() is customized to use the STM32 RNG
*          to generate random data, required for TLS encryption algorithms.
*
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2018 STMicroelectronics International N.V. 
* All rights reserved.</center></h2>
*
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted, provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, 
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
#include "avs_private.h"


static RNG_HandleTypeDef RngHandle;
static uint32_t          rng_initalized = 0;

static void ensure_rng_initialized(void);
static void ensure_rng_initialized(void)
{
  if(rng_initalized == 0)
  {
    rng_initalized = 1;
    /* Windows-specific initialization */
    /* Enable RNG peripheral clock */
    __HAL_RCC_RNG_CLK_ENABLE();
    
    /* Clock the IP */
    RngHandle.Instance = RNG;
    __HAL_RNG_ENABLE(&RngHandle);
    
    
    /* DeInitialize the RNG peripheral */
    if (HAL_RNG_DeInit(&RngHandle) != HAL_OK)
    {
      /* DeInitialization Error */
      AVS_Signal_Exeception(AVS_NULL, AVS_SIGNAL_EXCEPTION_GENERAL_ERROR);
    }
    
    /* Initialize the RNG peripheral */
    if (HAL_RNG_Init(&RngHandle) != HAL_OK)
    {
      /* Initialization Error */
      AVS_Signal_Exeception(AVS_NULL, AVS_SIGNAL_EXCEPTION_GENERAL_ERROR);
    }
  }
}


AVS_Result  avs_sys_get_rng(uint32_t *randomValue)
{
  ensure_rng_initialized();
  if(HAL_RNG_GenerateRandomNumber(&RngHandle, randomValue) != HAL_OK) 
  {
    return AVS_ERROR;
  }
  return AVS_OK;
}




