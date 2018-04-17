/*******************************************************************************
* @file    avs_porting_bsp_sys.c
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   porting layer file
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
#include "avs_porting_bsp_F7.h"

/*


     We assume that the HAL_Init is done and we have to initialize  the system only for
     IP used by the STAVS system, audio and instance will be initialied later


*/

AVS_Result      platform_Sys_init(void)
{
  HAL_InitTick(0); /* [DSe] TODO -  dafault tick priority wont work (handler never called) */
  HAL_ResumeTick();
  return AVS_OK;
}


/*

 returns a random uint32_t

*/
uint32_t platform_Sys_rnd(void)
{
  uint32_t value;
  if(avs_sys_get_rng(&value) != AVS_OK)
  {
      AVS_ASSERT(0);
      return 0;
  }
  return value;
}



uint32_t   platform_Sys_ioctl(AVS_instance_handle *pHandle, uint32_t code, uint32_t wparam, uint32_t lparam)
{
  if(code == AVS_SYS_SET_LED)
  {
    Led_TypeDef tLed[] = {LED1, LED2};
    if(wparam >= 2 )
    {
      wparam = 1;
    }
    if(lparam == 0)
    {
      BSP_LED_Off(tLed[wparam]);
    }
    else
    {
      BSP_LED_On(tLed[wparam]);
    }
    return AVS_OK;
  }

  return AVS_NOT_IMPL;
}








