/**
******************************************************************************
* @file    avs_spirit_mp3_decoder_imp.c
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   main application file
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
* this module to initialize a driver according to the porting layer platform
*
*
*
*/


#include "avs_private.h"

MISRAC_DISABLE          /* Disable all C-State for external dependencies */
#include "spiritMP3Dec.h"
MISRAC_ENABLE          /* Enable all  C-State */




#define SIZE_MP3_RING_BUFFER            1024
#define SLEEP_CONSUM_TIME               10
#define SIZE_BUFFER_PCM_IN_SAMPLE       256
#define NB_CHANNEL_MP3                  2
#define MP3_TASK_STACK_SIZE             500
#define MP3_TASK_PRIORITY               avs_core_task_priority_idle
#define MP3_TASK_NAME                   "AVS:MP3 player"


typedef struct  avs_bytes_buffer
{
  uint8_t      *pBuffer;              /* Ring  buffer */
  uint32_t      nbBytes;              /* Size buffer */
  int32_t       szConsumer;           /* Nb element in available in the ring buffer */
  uint32_t      prodPos;              /* Write  position */
  uint32_t      consumPos;            /* Read position */
  avs_mutex     lock;                 /* Lock mutex */
} Avs_bytes_buffer;


/*


 Create a ring buffer object



*/
__STATIC_INLINE AVS_Result avs_bytes_buffer_Create(Avs_bytes_buffer *pHandle, uint32_t nbbytes);
__STATIC_INLINE AVS_Result avs_bytes_buffer_Create(Avs_bytes_buffer *pHandle, uint32_t nbbytes)
{
  memset(pHandle, 0, sizeof(*pHandle));
  pHandle->nbBytes = nbbytes;
  pHandle->pBuffer = (uint8_t*)avs_core_mem_alloc(avs_mem_type_heap, nbbytes);
  avs_core_mutex_create(&pHandle->lock);
  AVS_ASSERT(pHandle->pBuffer);
  /* RAZ by default */
  memset(pHandle->pBuffer, 0, nbbytes);

  return AVS_OK;
}
__STATIC_INLINE AVS_Result avs_bytes_buffer_Delete(Avs_bytes_buffer *pHandle);
__STATIC_INLINE AVS_Result avs_bytes_buffer_Delete(Avs_bytes_buffer *pHandle)
{
  avs_core_mutex_delete(&pHandle->lock);
  avs_core_mem_free(pHandle->pBuffer);
  return AVS_OK;
}
__STATIC_INLINE uint8_t *avs_bytes_buffer_GetConsumer(Avs_bytes_buffer *pHandle);
__STATIC_INLINE uint8_t *avs_bytes_buffer_GetConsumer(Avs_bytes_buffer *pHandle)
{
  return &pHandle->pBuffer[pHandle->consumPos  % pHandle->nbBytes];
}
__STATIC_INLINE uint8_t *avs_bytes_buffer_GetProducer(Avs_bytes_buffer *pHandle);
__STATIC_INLINE uint8_t *avs_bytes_buffer_GetProducer(Avs_bytes_buffer *pHandle)
{
  return &pHandle->pBuffer[pHandle->prodPos  % pHandle->nbBytes];
}
__STATIC_INLINE uint32_t avs_bytes_buffer_GetProducerSizeAvailable(Avs_bytes_buffer *pHandle);
__STATIC_INLINE uint32_t avs_bytes_buffer_GetProducerSizeAvailable(Avs_bytes_buffer *pHandle)
{
  return pHandle->nbBytes - pHandle->szConsumer;
}
__STATIC_INLINE void     avs_bytes_buffer_MovePtrConsumer(Avs_bytes_buffer *pHandle, uint32_t offset, uint8_t **ppBuffer);
__STATIC_INLINE void     avs_bytes_buffer_MovePtrConsumer(Avs_bytes_buffer *pHandle, uint32_t offset, uint8_t **ppBuffer)
{
  pHandle->szConsumer -= offset;
  AVS_ASSERT(pHandle->szConsumer >= 0);
  pHandle->consumPos += offset;
  *ppBuffer += offset;
  if (pHandle->consumPos >= pHandle->nbBytes)
  {
    pHandle->consumPos = 0;
    *ppBuffer = pHandle->pBuffer;
  }
}

__STATIC_INLINE void     avs_bytes_buffer_MovePtrProducer(Avs_bytes_buffer *pHandle, uint32_t offset, uint8_t **ppBuffer);
__STATIC_INLINE void     avs_bytes_buffer_MovePtrProducer(Avs_bytes_buffer *pHandle, uint32_t offset, uint8_t **ppBuffer)
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






/* Context struct */
typedef struct mp3_decoder_context
{
  TSpiritMP3Decoder MP3Decoder_Instance;          /* Decoder object pointer */
  TSpiritMP3Info    MP3_Info;                     /* Current Stream Info */
  Avs_bytes_buffer  payload;
  avs_task          *mp3Task;
  int8_t             mp3Runing;
} Mp3_decoder_context;



/* Buffer for input MP3 compressed stream */
/**
  * @brief  Callback function to supply the decoder with input MP3 bitsteram.
  * @param  pMP3CompressedData: pointer to the target buffer to be filled.
  * @param  nMP3DataSizeInChars: number of data to be read in bytes
  * @param  pUserData: optional parameter (not used in this version)
  * @retval return the number of chars placed into the pMP3CompressedData buffer
  */
static uint32_t MP3Decoder_ReadDataCallback(void *    pCompressedData,uint32_t  nDataSizeInChars,void *    pUserData );
static uint32_t MP3Decoder_ReadDataCallback(void *    pCompressedData,uint32_t  nDataSizeInChars,void *    pUserData )
{

  Mp3_decoder_context *pContext = (Mp3_decoder_context*)pUserData;
  uint32_t nBytes  = 0;

  Avs_bytes_buffer *pRingBuffer = &pContext->payload;
  if(avs_core_atomic_read((uint32_t *)(uint32_t)&pRingBuffer->szConsumer) == 0)
  {
    return 0;
  }
  avs_core_mutex_lock(&pRingBuffer->lock);

  uint8_t *pInput = avs_bytes_buffer_GetConsumer(pRingBuffer);
  uint8_t *pOut = (uint8_t *)pCompressedData;
  for(nBytes  = 0; (nBytes < nDataSizeInChars) && (pRingBuffer->szConsumer!= 0)  ; nBytes ++)
  {
    *pOut = *pInput;
    pOut++;
    avs_bytes_buffer_MovePtrConsumer(pRingBuffer, 1, &pInput);

  }
  avs_core_mutex_unlock(&pRingBuffer->lock);
  return nBytes ;
}
static void avs_mp3_task(const void *pCookie);
static void avs_mp3_task(const void *pCookie)
{
  AVS_audio_handle *pHandle = (AVS_audio_handle *)(uint32_t) pCookie;
  Mp3_decoder_context *pCtx = (Mp3_decoder_context*)pHandle->pMP3Context ;
  /* Wait audio MICRO initialized */
  while(!avs_core_atomic_read(&pHandle->runInRuning))
  {
    avs_core_task_delay(100);
  }
  avs_core_message_send(pHandle->pInstance, EVT_MP3_TASK_START, 0);


  /* Make sure the nb channels out is stereo MP3 always stereo */
  AVS_ASSERT(pHandle->synthesizerPipe.inBuffer.nbChannel == NB_CHANNEL_MP3);

  pCtx ->mp3Runing = 1;
  while(pCtx ->mp3Runing)
  {
    static int16_t buffer16bitsPCM[SIZE_BUFFER_PCM_IN_SAMPLE * NB_CHANNEL_MP3];
    /* Request for MP3_OUTPUT_PCM_BUFFER_SIZE MP3 decoding */
    int32_t nbSample = SpiritMP3Decode(&pCtx->MP3Decoder_Instance, buffer16bitsPCM, SIZE_BUFFER_PCM_IN_SAMPLE, &pCtx->MP3_Info );
    if(nbSample)
    {
      /* Check if the frequency is changed */
      if(pCtx->MP3_Info.nSampleRateHz != pHandle->pFactory->synthesizerSampleRate)
      {
        avs_core_change_mp3_frequency(pHandle->pInstance, pCtx->MP3_Info.nSampleRateHz);
      }


      /* The decoder has decoded nbSample PCM stereo samples */
      uint32_t index = 0;
      while(nbSample)
      {
        AVS_ASSERT(nbSample >= 0);
        if (pCtx->MP3_Info.IsGoodStream == 0)
        {
          /* BAD packet stream  */
        }
        /* Inject samples in the ring buffer */
        uint32_t nbInjected = avs_audio_inject_stream_buffer(pHandle, &buffer16bitsPCM[index * NB_CHANNEL_MP3], nbSample);
        /* If the returned nb samples is different this means that the buffer is full, let's sleep a bit and continue */
        if(nbInjected != nbSample)
        {
          avs_core_task_delay(SLEEP_CONSUM_TIME);
        }
        nbSample -= nbInjected  ;
        index    += nbInjected ;

      }
    }
    else
    {
      avs_core_task_delay(SLEEP_CONSUM_TIME);
    }
  }
  avs_core_message_send(pHandle->pInstance, EVT_MP3_TASK_DYING, 0);

}



/*

 Initialize  the mp3 decoder instance


*/
AVS_Result platform_MP3_decoder_init(AVS_audio_handle *pHandle);
AVS_Result platform_MP3_decoder_init(AVS_audio_handle *pHandle)
{
  /* Enable the CRC */
  Mp3_decoder_context *pCtx;
  pHandle->pMP3Context = avs_core_mem_alloc(avs_mem_type_heap, sizeof(Mp3_decoder_context));
  AVS_ASSERT(pHandle->pMP3Context );
  pCtx = (Mp3_decoder_context*)pHandle->pMP3Context ;
  avs_bytes_buffer_Create(&pCtx->payload, SIZE_MP3_RING_BUFFER);
  /* Initialize the decoder */
  SpiritMP3DecoderInit(&pCtx->MP3Decoder_Instance, (fnSpiritMP3ReadCallback*)MP3Decoder_ReadDataCallback, NULL, (void *)pCtx);

  AVS_VERIFY((pCtx->mp3Task = avs_core_task_create(MP3_TASK_NAME, avs_mp3_task, pHandle, MP3_TASK_STACK_SIZE, MP3_TASK_PRIORITY)) != NULL);
  if(!pCtx->mp3Task)
  {
    AVS_TRACE_ERROR("Create task %s", MP3_TASK_NAME);
    return AVS_ERROR;
  }
  return AVS_OK;
}


/*

 terminate the mp3 decoder instance


*/
AVS_Result platform_MP3_decoder_term(AVS_audio_handle *pHandle);
AVS_Result platform_MP3_decoder_term(AVS_audio_handle *pHandle)
{
  Mp3_decoder_context *pCtx = (Mp3_decoder_context*)pHandle->pMP3Context;

  pCtx->mp3Runing =  0;
  avs_core_task_delay(10); /* Leave enough time to terminate the thread */
  avs_core_task_delete(pCtx->mp3Task );


  avs_bytes_buffer_Delete(&pCtx->payload);

  if(pCtx)
  {
    avs_core_mem_free(pCtx);
  }
  pHandle->pMP3Context = 0;
  return AVS_OK;
}

/*

 this function apply a mp3 decompression, then feed the speaker stream
 if the stream is full, the function waits until the stream is consumed by the audio player
 the decoder assume mp3 packet are syntaxtcaly perfect and not cut in the sample middle


*/
AVS_Result platform_MP3_decoder_inject(AVS_audio_handle *pAHandle, uint8_t *pMp3Payload, uint32_t MP3PayloadSize);
AVS_Result platform_MP3_decoder_inject(AVS_audio_handle *pAHandle, uint8_t *pMp3Payload, uint32_t MP3PayloadSize)
{
  Mp3_decoder_context *pCtx;
  pCtx = (Mp3_decoder_context*)pAHandle->pMP3Context ;
  AVS_ASSERT(pMp3Payload);
  AVS_ASSERT(MP3PayloadSize < pCtx->payload.nbBytes);

  /* Waits for enough place in the ring buffer */
  while(1)
  {
    avs_core_mutex_lock(&pCtx->payload.lock);
    uint32_t szProd = avs_bytes_buffer_GetProducerSizeAvailable(&pCtx->payload);
    avs_core_mutex_unlock(&pCtx->payload.lock);
    if(szProd >= MP3PayloadSize)
    {
      break;
    }
    avs_core_task_delay(SLEEP_CONSUM_TIME);
  }

  avs_core_mutex_lock(&pCtx->payload.lock);
  uint8_t *pProd = avs_bytes_buffer_GetProducer(&pCtx->payload);
  for(uint32_t a = 0; a < MP3PayloadSize ; a++)
  {
    *pProd = *pMp3Payload;
    pMp3Payload++;
    avs_bytes_buffer_MovePtrProducer(&pCtx->payload, 1, &pProd);

  }
  avs_core_mutex_unlock(&pCtx->payload.lock);
  return AVS_OK;
}

/* Not used for now */
uint32_t platform_MP3_decoder_ioctl(AVS_audio_handle *pHandle, uint32_t code, uint32_t wparam, uint32_t lparam, ...);
uint32_t platform_MP3_decoder_ioctl(AVS_audio_handle *pHandle, uint32_t code, uint32_t wparam, uint32_t lparam, ...)
{
  return AVS_NOT_IMPL;
}


