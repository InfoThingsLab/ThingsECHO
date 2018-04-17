/**
  ******************************************************************************
  * @file    hal_usb_interface_extension
  * @author  MCD Application Team
  * @version V1.1.0RC2
  * @date    12-June-2017
  * @brief   This file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_USB_INTERFACE_EXTENSION
#define __HAL_USB_INTERFACE_EXTENSION

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_ll_usb.h"
#ifdef USE_USB_FS_INTO_HS
#define USB_OTG_BASE_ADDRESS  USB_OTG_HS
#else
#ifdef USE_USB_FS
#define USB_OTG_BASE_ADDRESS  USB_OTG_FS
#endif
#ifdef USE_USB_HS
#define USB_OTG_BASE_ADDRESS  USB_OTG_HS
#endif
#endif

/* MACRO ------------------------------------------------------------------*/
#define USB_DIEPCTL(ep_addr) ((USB_OTG_INEndpointTypeDef *)((uint32_t)USB_OTG_BASE_ADDRESS + USB_OTG_IN_ENDPOINT_BASE   \
                              + ((ep_addr)&0x7FU)*USB_OTG_EP_REG_SIZE))->DIEPCTL
#define USB_DOEPCTL(ep_addr) ((USB_OTG_OUTEndpointTypeDef *)((uint32_t)USB_OTG_BASE_ADDRESS +  \
                              USB_OTG_OUT_ENDPOINT_BASE + (ep_addr)*USB_OTG_EP_REG_SIZE))->DOEPCTL

#define USB_CLEAR_INCOMPLETE_IN_EP(ep_addr)     if((((ep_addr) & 0x80U) == 0x80U)){  \
    USB_DIEPCTL(ep_addr) |= (USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK);  \
  };

#define USB_CLEAN_EP_AFTER_CLOSE(ep_addr)\
  if((((ep_addr) & 0x80U) == 0x80U))\
  {\
    USB_DIEPCTL(ep_addr)&= ~ (USB_OTG_DIEPCTL_USBAEP|   \
                              USB_OTG_DIEPCTL_MPSIZ|    \
                              USB_OTG_DIEPCTL_TXFNUM|  \
                              USB_OTG_DIEPCTL_EPTYP);    \
  }\
  else\
  {\
    USB_DOEPCTL(ep_addr)&= ~(USB_OTG_DOEPCTL_USBAEP| \
                             USB_OTG_DOEPCTL_MPSIZ); \
  } ;

#define USB_SOF_NUMBER() ((((USB_OTG_DeviceTypeDef *)((uint32_t )USB_OTG_BASE_ADDRESS + USB_OTG_DEVICE_BASE))->DSTS&USB_OTG_DSTS_FNSOF)>>USB_OTG_DSTS_FNSOF_Pos)



#define IS_ISO_IN_INCOMPLETE_EP(ep_addr,current_sof, transmit_soffn) ((USB_DIEPCTL(ep_addr)&USB_OTG_DIEPCTL_EPENA_Msk)&&\
    (((((uint32_t)current_sof)&0x01U) == ((USB_DIEPCTL((ep_addr))&USB_OTG_DIEPCTL_EONUM_DPID_Msk)>>USB_OTG_DIEPCTL_EONUM_DPID_Pos))\
     ||((current_sof)== ((uint32_t)((transmit_soffn)+2)&0x7FFU))))

#ifdef __cplusplus
}
#endif

#endif  /* __HAL_USB_INTERFACE_EXTENSION*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
