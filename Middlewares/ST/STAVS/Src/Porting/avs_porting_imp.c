/**
******************************************************************************
* @file    avs_porting_imp.c
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   This file provides the Audio monitoring interface API
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
#include "avs_porting.h"



typedef uint32_t(*DRV_PROC)(void) ;


/* Call to have the importunity to inspect the strut and manage default standard values */
void avs_default_instance_solver(AVS_Instance_Factory *pFactory)
{
  avs_init_default_string(&pFactory->clientId, "fake id : init me");
  avs_init_default_string(&pFactory->clientSecret, "fake id:  init me");
  avs_init_default_string(&pFactory->productId, "STAVS Application");
  avs_init_default_string(&pFactory->hostName, "STAVS Application");
  avs_init_default_interger(&pFactory->synthesizerSampleChannels, 2);
  avs_init_default_interger(&pFactory->synthesizerSampleRate, 24000);
  avs_init_default_interger(&pFactory->recognizerSampleRate, 16000);
  avs_init_default_interger(&pFactory->useAuxAudio, 0);
  avs_init_default_interger(&pFactory->auxAudioSampleChannels, 1);
  avs_init_default_interger(&pFactory->auxAudioSampleRate, 16000);
  avs_init_default_string(&pFactory->alexaKeyWord, "ALEXA");
  avs_init_default_string(&pFactory->toolChainID, TOOLCHAIN);


  avs_init_default_string(&pFactory->redirectUri, "https://stavs.com/grant_me");
  avs_init_default_string(&pFactory->urlNtpServer, "0.fr.pool.ntp.org");
  avs_init_default_string(&pFactory->cpuID, "STM32F(X)");
  avs_init_default_string(&pFactory->urlEndPoint, "avs-alexa-na.amazon.com");
  avs_init_default_interger(&pFactory->memDTCMSize, BOARD_DTCM_SIZE);
  avs_init_default_interger(&pFactory->memPRAMSize, BOARD_PRAM_SIZE);




}

/* Call to have the opportunity to inspect the struct and manage default standard values */
void avs_default_audio_solver(AVS_Instance_Factory *pFactory)
{

}



/* Just a function doing nothing */
static uint32_t dummy_function(void);
static uint32_t dummy_function(void)
{
  return AVS_NOT_IMPL;
}
static void myPuts(const char *pString);
static void myPuts(const char *pString)
{
  while(*pString)
  {
#ifdef __GNUC__
	int __io_putchar(int ch);

    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
       set to 'Yes') calls __io_putchar() */
    __io_putchar(*pString);
#else
	FILE * fd = NULL; /* Avoid warning*/
    fputc(*pString, fd);
#endif /* __GNUC__ */

    pString++;
  }
}
static void default_puts(const char *pString, uint32_t flags);
static void default_puts(const char *pString, uint32_t flags)
{
  myPuts(pString);
  if(flags)
  {
    myPuts("\r\n"); /* Assumes always CRLF */
}
}



AVS_porting_audio    drv_audio;
AVS_porting_instance drv_instance;
AVS_porting_sys    drv_sys;


AVS_Result avs_deletePorting(void)
{
  return AVS_OK;
}


AVS_Result avs_createPorting(void)
{
  AVS_Result error;
  /* Warning, i use an hack to init the driver with dummy function in order to limit the check of null before to call the driver fn */
  /* Example if(drv_audio.platform_init) drv_audio.platform_init(NULL);the aims is to limit the code generation */

  DRV_PROC *pDriver;
  int32_t nb = sizeof(drv_audio) / sizeof(DRV_PROC);
  pDriver = (DRV_PROC*)(uint32_t)&drv_audio;
  /* Init the driver audio */

  for (int32_t cpt = 0; cpt < nb; cpt++)
  {
    pDriver[cpt] = dummy_function;
  }
  /* Init the driver instance */

  nb = sizeof(drv_instance) / sizeof(DRV_PROC);
  pDriver = (DRV_PROC*)(uint32_t)&drv_instance;
  for (int32_t cpt = 0; cpt < nb; cpt++)
  {
    pDriver[cpt] = dummy_function;
  }


  nb = sizeof(drv_sys) / sizeof(DRV_PROC);
  pDriver = (DRV_PROC*)(uint32_t)&drv_sys;
  for (int32_t cpt = 0; cpt < nb; cpt++)
  {
    pDriver[cpt] = dummy_function;
  }

  /* Now we can initialize some functions rarely overloaded by the platform in order to ease the porting */

  drv_sys.platform_Sys_puts = default_puts;

  /* Now we can call the porting  initialization, it is up to the platform to fill the CB or not, since we have a real function in place doing nothing */
  error = drv_platform_Init();
  if (error != AVS_OK)
  {
    return  error;
  }

  /* We can now initialize the hardware */
  return drv_sys.platform_Sys_init();
}
