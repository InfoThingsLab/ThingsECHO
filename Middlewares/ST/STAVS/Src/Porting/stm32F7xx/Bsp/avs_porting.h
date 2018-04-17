/*******************************************************************************
* @file    avs_porting.h
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

#ifndef _avs_porting_
#define _avs_porting_

#ifdef __cplusplus
extern "C" {
#endif
#include "../avs_board_f7.h"

/* Porting layer for platform operation */
#define PORTING_LAYER_BSP


typedef enum bsp_options
{
  BSP_NO_OPTION = 0,            /* Selection the microphone sample feed , use the optional parameter as AFE_Feed_Options returns AVS_OK or AVS_ERROR */
} BSP_Options;





typedef enum st_bsp_configs
{
  AVS_DEFAULT_CONFIG = 0,
  ST_BSP_CONFIG_NUM_MIC_1 = AVS_DEFAULT_CONFIG

} ST_BSP_Configs;

typedef enum st_bsp_profiles
{
  AVS_DEFAULT_PROFILE = 0,
  ST_BSP_PROFILE_NO_POSTPROCESSING = AVS_DEFAULT_PROFILE

} ST_BSP_Profiles;


typedef struct oavs_Audio_Factory_Platform
{
  uint32_t numConfig;
  uint32_t numProfile;
} AVS_Audio_Factory_Platform;

typedef AVS_Audio_Factory_Platform AVS_Audio_Platform;
typedef BSP_Options                AVS_Audio_Options;


#ifdef __cplusplus
};
#endif
#endif /* _avs_porting_ */
