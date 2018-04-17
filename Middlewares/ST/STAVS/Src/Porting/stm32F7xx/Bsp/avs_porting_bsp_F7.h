/*******************************************************************************
* @file    avs_porting_bsp_F7.h
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

#ifndef _avs_porting_bsp_F7_
#define _avs_porting_bsp_F7_
#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <../avs_board_f7.h>



AVS_Result  platform_Audio_init(AVS_audio_handle *pHandle);
AVS_Result  platform_Audio_term(AVS_audio_handle *pHandle);
AVS_Result  platform_Audio_default(AVS_audio_handle *pHandle);
uint32_t    platform_Audio_ioctl(AVS_audio_handle *pHandle, uint32_t code, uint32_t wparam, uint32_t lparam);
AVS_Result  platform_Network_init(AVS_instance_handle *pHandle);
AVS_Result  platform_Network_term(AVS_instance_handle *pHandle);
uint32_t    platform_Network_ioctl(AVS_instance_handle *pHandle, uint32_t code, uint32_t wparam, uint32_t lparam);
AVS_Result  platform_Network_Solve_macAddr(AVS_instance_handle *pHandle);
AVS_Result  platform_MP3_decoder_init(AVS_audio_handle *pHandle);
AVS_Result  platform_MP3_decoder_term(AVS_audio_handle *pHandle);
uint32_t    platform_MP3_decoder_ioctl(AVS_audio_handle *pHandle, uint32_t code, uint32_t wparam, uint32_t lparam);
AVS_Result  platform_MP3_decoder_inject(AVS_audio_handle *pAHandle, uint8_t *pMp3Payload, uint32_t MP3PayloadSize);
AVS_Result  platform_Sys_init(void);
uint32_t    platform_Sys_rnd(void);
uint32_t    platform_Sys_ioctl(AVS_instance_handle *pHandle, uint32_t code, uint32_t wparam, uint32_t lparam);



#ifdef __cplusplus
};
#endif

#endif /* _avs_porting_bsp_F7_ */

