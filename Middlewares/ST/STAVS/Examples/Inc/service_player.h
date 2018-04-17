/**
******************************************************************************
* @file    service_player.h
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   manage the audio player
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

#ifndef _service_player_
#define _service_player_

char_t * service_player_get_string(void);
/* ARM ARM says that Load and Store instructions are atomic and it's execution is guaranteed to be complete before interrupt handler executes. */
/* Verified by looking at arch/arm/include/asm/atomic.h */
__STATIC_INLINE uint32_t service_player_atomic_read(uint32_t *p32);
__STATIC_INLINE uint32_t service_player_atomic_read(uint32_t *p32)
{
  return ((*(volatile uint32_t *)p32));
}
__STATIC_INLINE void service_player_atomic_write(uint32_t *p32, uint32_t val );
__STATIC_INLINE void service_player_atomic_write(uint32_t *p32, uint32_t val )
{
  (*(volatile uint32_t *)p32) = val;
}




/*


Create a ring buffer object



*/


typedef struct  t_bytes_buffer
{
  uint8_t      *pBuffer;              /* Ring  buffer */
  uint32_t      nbBytes;              /* Size buffer */
  uint32_t      szConsumer;           /* Nb element in available in the ring buffer */
  uint32_t      prodPos;              /* Write  position */
  uint32_t      consumPos;            /* Read position */
  mutex_t     lock;          /* Lock mutex */
} bytes_buffer_t;

__STATIC_INLINE AVS_Result service_player_bytes_buffer_Create(bytes_buffer_t *pHandle, uint32_t nbbytes,char_t *pName);
__STATIC_INLINE AVS_Result service_player_bytes_buffer_Create(bytes_buffer_t *pHandle, uint32_t nbbytes,char_t *pName)
{
  memset(pHandle, 0, sizeof(*pHandle));
  pHandle->nbBytes = nbbytes;
  pHandle->pBuffer = (uint8_t*)pvPortMalloc(nbbytes);
  AVS_VERIFY(mutex_Create(&pHandle->lock,pName));

  AVS_ASSERT(pHandle->pBuffer);
  /* RAZ by default */
  memset(pHandle->pBuffer, 0, (size_t)nbbytes);

  return AVS_OK;
}



__STATIC_INLINE AVS_Result service_player_bytes_buffer_Delete(bytes_buffer_t *pHandle);
__STATIC_INLINE AVS_Result service_player_bytes_buffer_Delete(bytes_buffer_t *pHandle)
{
  mutex_Delete(&pHandle->lock);
  vPortFree(pHandle->pBuffer);
  return AVS_OK;
}
__STATIC_INLINE uint8_t *service_player_bytes_buffer_GetConsumer(bytes_buffer_t *pHandle);
__STATIC_INLINE uint8_t *service_player_bytes_buffer_GetConsumer(bytes_buffer_t *pHandle)
{
  return &pHandle->pBuffer[pHandle->consumPos  % pHandle->nbBytes];
}
__STATIC_INLINE uint8_t *service_player_bytes_buffer_GetProducer(bytes_buffer_t *pHandle);
__STATIC_INLINE uint8_t *service_player_bytes_buffer_GetProducer(bytes_buffer_t *pHandle)
{
  return &pHandle->pBuffer[pHandle->prodPos  % pHandle->nbBytes];
}
__STATIC_INLINE uint32_t service_player_bytes_buffer_GetProducerSizeAvailable(bytes_buffer_t *pHandle);
__STATIC_INLINE uint32_t service_player_bytes_buffer_GetProducerSizeAvailable(bytes_buffer_t *pHandle)
{
  return pHandle->nbBytes - pHandle->szConsumer;
}

/* Move a ptr consumer in the ring buffer */

__STATIC_INLINE void     service_player_bytes_buffer_MovePtrConsumer(bytes_buffer_t *pHandle, uint32_t offset, uint8_t **ppBuffer);
__STATIC_INLINE void     service_player_bytes_buffer_MovePtrConsumer(bytes_buffer_t *pHandle, uint32_t offset, uint8_t **ppBuffer)
{
  pHandle->szConsumer -= offset;
  pHandle->consumPos += offset;
  *ppBuffer += offset;
  if (pHandle->consumPos >= pHandle->nbBytes)
  {
    pHandle->consumPos = 0;
    *ppBuffer = pHandle->pBuffer;
  }
}
/* Move a ptr producer in the ring buffer */
__STATIC_INLINE void     service_player_bytes_buffer_MovePtrProducer(bytes_buffer_t *pHandle, uint32_t offset, uint8_t **ppBuffer);
__STATIC_INLINE void     service_player_bytes_buffer_MovePtrProducer(bytes_buffer_t *pHandle, uint32_t offset, uint8_t **ppBuffer)
{
  pHandle->szConsumer += offset;
  pHandle->prodPos += offset;
  *ppBuffer += offset;
  if (pHandle->prodPos >= pHandle->nbBytes)
  {
    pHandle->prodPos = 0;
    *ppBuffer = pHandle->pBuffer;
  }
}
__STATIC_INLINE void  service_player_bytes_buffer_reset(bytes_buffer_t *pHandle);
__STATIC_INLINE void  service_player_bytes_buffer_reset(bytes_buffer_t *pHandle)
{
  pHandle->prodPos = 0;
  pHandle->consumPos = 0;
  pHandle->szConsumer = 0;

}


__STATIC_INLINE char_t * alloc_string(const char_t *pString);
__STATIC_INLINE char_t * alloc_string(const char_t *pString)
{
  if( pString == (char_t *)0)
  {
    return  (char_t *)0;
  }

  char_t *pAlloc = (char_t *)pvPortMalloc(strlen(pString) + (size_t)1);
  AVS_ASSERT(pAlloc);
  strcpy(pAlloc, pString);
  return pAlloc;
}

typedef struct t_player_flags
{
  unsigned int  forground : 1;
  unsigned int  background : 1;
  unsigned int  network : 1 ; /* True if the url is a network */
  unsigned int  sent_report : 1; /* Send-report is done */
  unsigned int  pause : 1; /* player paused */
  unsigned int  playing : 1; /* player playing*/
} player_flags_t;

typedef struct   t_player_slot
{
  player_flags_t   flags;
  uint32_t       playDelay;                                                              /* Perio delay to manage events */
  char_t           *audioItem_audioItemId;                                                 /* Identifies the audioItem. string */
  char_t           *audioItem_stream_url           ;                                  /* Identifies the location of audio content. If the audio content is a binary audio attachment, the value will be a unique identifier for the content, which is formatted as follows: "cid:". Otherwise the value will be a remote http/https location. string */
  char_t           *audioItem_stream_streamFormat;                                         /* StreamFormat is included in the payload when the Play directive has an associated binary audio attachment. This parameter will not appear if the associated audio is a stream. Accepted Value: AUDIO_MPEG string */
  uint32_t       audioItem_stream_offsetInMilliseconds;                                 /* A timestamps indicating where in the stream the client must start playback. For example, when offsetInMilliseconds is set to 0, this indicates playback of the stream must start at 0, or the start of the stream. Any other value indicates that playback must start from the provided offset. long */
  time_t         audioItem_stream_expiryTime            ;                         /* The date and time in ISO 8601 format for when the stream becomes invalid.  string */
  char_t           *audioItem_stream_token                ;                               /* An opaque token that represents the current stream.  string */
  char_t           *audioItem_stream_expectedPreviousToken;                         /* An opaque token that represents the expected previous stream. */
  uint32_t        audioItem_stream_progressReport_progressReportDelayInMilliseconds;                                     /* Send event inital progressReportDelayInMilliseconds MS example : 15000 */
  uint32_t        audioItem_stream_progressReport_progressReportIntervalInMilliseconds;                                  /* Send event perodique event in MS example : 900000 */
} player_slot_t;


typedef struct t_player_context /* Context and audio stream abstraction able to read from the net or form a file */
{
  player_slot_t           *pItemPlaying;    /* Current playing slot */
  void                    *pReaderCookie;
  void                    *pPlayerCookie;

} player_context_t;




typedef struct t_player_reader
{
  AVS_Result            (*open)(player_context_t *phandle, const char_t *pURL);
  AVS_Result            (*close)(player_context_t *phandle);
  AVS_Result            (*pull)(player_context_t *phandle, void *pBuffer, uint32_t szInSByte, uint32_t *retSize);
} player_reader_t;



typedef struct t_player_media
{
  AVS_Result            (*create)(player_context_t *phandle);
  AVS_Result            (*delete)(player_context_t *phandle);
  AVS_Result            (*start)(player_context_t *phandle);
  AVS_Result            (*stop)(player_context_t *phandle);
  AVS_Result            (*check_codec)(player_context_t *phandle, uint8_t *payload, int32_t size);

} player_codec_t;


extern struct t_player_context  playerContext;
extern bytes_buffer_t           gMp3Payload;


void service_audio_read_http_create(player_reader_t *pReader);
void service_audio_read_fs_create(player_reader_t *pReader);
player_codec_t * service_audio_play_media_mp3(void);

void service_player_audio_send(AVS_Handle instance, uint32_t evt, uint32_t pparam);
void service_player_event(AVS_Handle handle, const char_t *pName);
void service_player_volume_max(AVS_Handle handle, uint32_t max);
uint32_t  service_player_get_buffer_percent(AVS_Handle handle);

void service_player_mng_fade(void);



#endif /* _service_player_ */

