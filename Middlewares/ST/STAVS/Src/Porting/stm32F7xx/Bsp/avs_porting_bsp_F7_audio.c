/*******************************************************************************
* @file    avs_porting_bsp_F7_audio.c
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
/*
*
*
* this module is in charge to manage audio configuration and management
*
*
*
*/


#include "avs_private.h"
#include "avs_porting_bsp_F7.h"



__weak  AVS_Result init_bsp_audio(AVS_audio_handle *pHandle);
__weak  AVS_Result init_bsp_audio(AVS_audio_handle *pHandle)
{
  return AVS_ERROR;
}
__weak AVS_Result term_bsp_audio(AVS_audio_handle *pHandle);
__weak AVS_Result term_bsp_audio(AVS_audio_handle *pHandle)
{
  return AVS_OK;
}


AVS_Result  platform_Audio_default(AVS_audio_handle *pHandle)
{
  avs_init_default_interger(&pHandle->pFactory->platform.numConfig, ST_BSP_CONFIG_NUM_MIC_1);
  avs_init_default_interger(&pHandle->pFactory->platform.numProfile, ST_BSP_PROFILE_NO_POSTPROCESSING);
  avs_init_default_interger(&pHandle->pFactory->initVolume, 60);
  avs_init_default_interger(&pHandle->pFactory->freqenceOut, 48000);
  avs_init_default_interger(&pHandle->pFactory->freqenceIn, 16000);
  avs_init_default_interger(&pHandle->pFactory->chOut, 2);
  avs_init_default_interger(&pHandle->pFactory->chIn, 1);



  return AVS_OK;
}


AVS_Result  platform_Audio_init(AVS_audio_handle *pHandle)
{
  AVS_ASSERT(pHandle->pFactory != 0);
  /* For BSP debug audio  prorting layer ,we support only default profile and config - ie 0) */
  if( (pHandle->pFactory->platform.numConfig != 0) || (pHandle->pFactory->platform.numProfile != 0))
  {
    AVS_TRACE_ERROR("For BSP debug audio  prorting layer ,we support only default profile and config - ie 0)");
    return AVS_ERROR;
  }

  return init_bsp_audio(pHandle);
}

AVS_Result  platform_Audio_term(AVS_audio_handle *pHandle)
{
  return term_bsp_audio(pHandle);
}

