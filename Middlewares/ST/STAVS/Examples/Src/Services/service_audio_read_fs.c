/**
******************************************************************************
* @file    service_audio_read_fs.c
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   manage the sample reading from file system
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

/******************************************************************************
 *
 * this code is provided as an example , It is not a production code
 *
 ******************************************************************************/

#include "service.h"


static uint8_t  *pAsset;
static uint32_t  iAssetSize;
static uint32_t  iAssetCur;


/*

 Force to close the audio stream


*/
static AVS_Result service_audio_read_fs_close(struct t_player_context *pHandle);
static AVS_Result service_audio_read_fs_close(struct t_player_context *pHandle)
{
  return AVS_OK;
}


/* Open an audio stream */
static AVS_Result service_audio_read_fs_open(struct t_player_context *phandle, const char *pURL);
static AVS_Result service_audio_read_fs_open(struct t_player_context *phandle, const char *pURL)
{
  if(strlen(pURL) > strlen("cid:")) 
  {
    pURL += strlen("cid:");
  }

  pAsset =      (uint8_t  *)(uint32_t)service_assets_load(pURL, &iAssetSize, 0);
  if(pAsset  == 0)
  {
    AVS_TRACE_ERROR("Can't find asset %s", pURL);
    return AVS_ERROR;
  }
  iAssetCur  = 0;



  return AVS_OK ;
}


/*

 pull some data


*/
static AVS_Result service_audio_read_fs_pull(player_context_t *pHandle, void *pBuffer, uint32_t szInSByte, uint32_t *retSize);
static AVS_Result service_audio_read_fs_pull(player_context_t *pHandle, void *pBuffer, uint32_t szInSByte, uint32_t *retSize)
{
  uint32_t   delta = iAssetSize - iAssetCur;
  if(delta == 0) 
  {
    return AVS_EOF;
  }
  if(szInSByte >= delta) 
  {
    szInSByte = delta;
  }
  if(retSize) 
  {
    *retSize = szInSByte;
  }
  memcpy(pBuffer, &pAsset[iAssetCur], szInSByte);
  iAssetCur += szInSByte;
  return AVS_OK;
}

/* Create the FS reader instance */
void service_audio_read_fs_create(player_reader_t *pReader)
{

  /* Init a network stream support */
  pReader->open  = service_audio_read_fs_open;
  pReader->pull  = service_audio_read_fs_pull;
  pReader->close  = service_audio_read_fs_close;
}







