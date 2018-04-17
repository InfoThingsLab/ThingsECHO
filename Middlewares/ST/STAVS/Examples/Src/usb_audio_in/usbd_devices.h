/**
  ******************************************************************************
  * @file    audio_devices.h
  * @author  MCD Application Team
  * @version V1.2.0RC2
  * @date    20-02-2018
  * @brief   Abstraction of boared specific devices.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_DEVICES_H
#define __AUDIO_DEVICES_H

#ifdef __cplusplus
extern "C" {
#endif

#define USE_USB_AUDIO_RECORDING
#define USE_USB_AUDIO_CLASS_10
#define USE_USB_FS_INTO_HS
#define USE_USB_FS
#define USE_AUDIO_RECORDING_VOLUME_CONTROL


/* list of frequencies*/
/* #define USB_AUDIO_CONFIG_FREQ_16_K   16000 */


#define USB_IRQ_PREPRIO 0xA
#define USB_FIFO_WORD_SIZE  320
#define USBD_AUDIO_CONFIG_PLAY_RES_BIT                0x10 /* 16 bit per sample */
#define USBD_AUDIO_CONFIG_PLAY_RES_BYTE               0x02 /* 2 bytes */


#define  USBD_AUDIO_CONFIG_PLAY_BUFFER_SIZE (1024 * 2)



/*record session : list of terminal and unit id for audio function */
/* must be greater than the highest interface number(to avoid request destination confusion */
#define USB_AUDIO_CONFIG_RECORD_TERMINAL_INPUT_ID     0x011
#define USB_AUDIO_CONFIG_RECORD_UNIT_FEATURE_ID       0x015
#define USB_AUDIO_CONFIG_RECORD_TERMINAL_OUTPUT_ID    0x013

#ifdef USB_AUDIO_DEEPER_RECORD_BUFFER_SIZE
#define  USBD_AUDIO_CONFIG_RECORD_BUFFER_SIZE         (1024 * 2 * 8)
#else
#define  USBD_AUDIO_CONFIG_RECORD_BUFFER_SIZE         (1024 * 2)
#endif

/*record session : audio description */
#define USBD_AUDIO_CONFIG_RECORD_CHANNEL_COUNT        0x02 /* stereo */
#define USBD_AUDIO_CONFIG_RECORD_CHANNEL_MAP          0x03 /* (USBD_AUDIO_CONFIG_CHANNEL_LEFT_FRONT|USBD_AUDIO_CONFIG_CHANNEL_RIGHT_FRONT) */


#define USBD_AUDIO_CONFIG_RECORD_RES_BIT              0x10 /* 16 bit per sample */
#define USBD_AUDIO_CONFIG_RECORD_RES_BYTE             0x02 /* 2 bytes */


#define USB_AUDIO_CONFIG_RECORD_FREQ_COUNT           1 /* 1 frequence */
#define USB_AUDIO_CONFIG_RECORD_FREQ_MAX             USB_AUDIO_CONFIG_RECORD_DEF_FREQ
#define USB_AUDIO_CONFIG_RECORD_DEF_FREQ             USB_AUDIO_CONFIG_FREQ_16_K /* to set by user */


#define USBD_AUDIO_CONFIG_RECORD_MAX_PACKET_SIZE ((uint16_t)(AUDIO_MS_MAX_PACKET_SIZE(USB_AUDIO_CONFIG_RECORD_FREQ_MAX,\
    USBD_AUDIO_CONFIG_RECORD_CHANNEL_COUNT,\
    USBD_AUDIO_CONFIG_RECORD_RES_BYTE)))


#define USBD_AUDIO_CONFIG_RECORD_SA_INTERFACE            0x01 /* AUDIO STREAMING INTERFACE NUMBER FOR RECORD SESSION */
#define USB_AUDIO_CONFIG_RECORD_EP_IN                    0x81

#define   AUDIO_MS_PACKET_SIZE(freq,channel_count,res_byte) (((uint32_t)((freq) /1000))* (channel_count) * (res_byte))
#define   AUDIO_MS_MAX_PACKET_SIZE(freq,channel_count,res_byte) AUDIO_MS_PACKET_SIZE((freq)+999,(channel_count),(res_byte))


/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
}
#endif
#endif  /* __AUDIO_USER_DEVICES_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
