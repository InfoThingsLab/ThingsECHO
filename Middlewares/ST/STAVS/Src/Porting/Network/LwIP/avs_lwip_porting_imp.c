/**
******************************************************************************
* @file    avs_lwip_porting_imp.c
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   This module implements the porting layer for cyclone
*          and ip
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



/* Init tls callback called from HTTP2 */

Http2Status avs_porting_http2_client_tls_init_Callback(Http2ClientContext *context, Http2TlsConfig *tlsConfig, Http2TlsContext *tlsContext)
{
  int_t ret;
  /* Debug message */
  avs_core_message_send(avs_core_get_instance(), EVT_START_TLS, 0);
  /* Set the fully qualified domain name of the server */
  ret = mbedtls_ssl_set_hostname(tlsContext, "h2spec-tn");

  /* Check return value */
  if(ret == 0)
  {
    return HTTP2_STATUS_OK;
  }
  else
  {
    return HTTP2_STATUS_FAILURE;
  }
}


/*

 HTTP2 porting layer, connect the client


*/

Http2Status avs_porting_http2_client_connect(AVS_instance_handle * pHandle);
Http2Status avs_porting_http2_client_connect(AVS_instance_handle * pHandle)
{
  struct sockaddr_in serverAddr;
  struct hostent *hostInfo;
  hostInfo = gethostbyname(pHandle->pFactory->urlEndPoint);
  if(hostInfo == 0)  
  {
    return HTTP2_STATUS_FAILURE;
  }
  avs_core_message_send(pHandle, EVT_HOSTNAME_RESOLVED, 0);
  /* Set remote IP address */
  serverAddr.sin_family = hostInfo->h_addrtype;
  memcpy(&serverAddr.sin_addr, hostInfo->h_addr, hostInfo->h_length);
  /* Set remote port number : 443 as default https port */
  serverAddr.sin_port = htons(AUTHENTICATION_SERVER_PORT);
  Http2Status err =  http2ClientConnect(pHandle->hHttpClient, &serverAddr, sizeof(serverAddr));
  if(err != HTTP2_STATUS_OK) 
  {
    return err;
  }
  return HTTP2_STATUS_OK;
}




/* Ethernet must be implemented in the board dependencies src 
Notice : Generate MISRA error MISRA C 2004 rule 5.6
Called from the TLS API  using a callback and a specific prototype 
*/

__weak err_t ethernetif_init(struct netif *netif)
{
  AVS_TRACE_ERROR("Error %s Not implemented", __FUNCTION__);
  AVS_Signal_Exeception(NULL,AVS_SIGNAL_EXCEPTION_GENERAL_ERROR);
  return 0;
}



/**
  * @brief  Returns the current time in milliseconds
  *         when LWIP_TIMERS == 1 and NO_SYS == 1
  * @param  None
  * @retval Time
  */
WEAK u32_t sys_now(void)
{
  return HAL_GetTick();
}
