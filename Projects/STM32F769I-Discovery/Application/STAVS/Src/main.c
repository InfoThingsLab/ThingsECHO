/**
******************************************************************************
* @file    main.c
* @author  MCD Application Team
* @version V1.0.0
* @date    04-Aout-2017
* @brief   main application file
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

#include "service.h"
#include "platform_init.h"
#include "cmsis_os.h"



#ifdef AVS_TEST_AUTO_START
#define TEST_AUTO_START         1
#else
#define TEST_AUTO_START         0

#endif

uint32_t        bAutoStartTest = TEST_AUTO_START;



static void service_main_task(void  const * argument)
{
  services_Init();
  if(bAutoStartTest)
  {
    /* Wait NTP synchronization */
    while(AVS_Set_Sync_Time(hInstance) != AVS_OK)
    {
      vTaskDelay(100);
    }
    service_endurance_start();
  }
  while(1) vTaskDelay(1000000);
}





int main()
{

  platform_Init();
  osThreadDef(defaultTask, service_main_task, osPriorityNormal, 0, 1000);
  osThreadCreate(osThread(defaultTask), NULL);
  osKernelStart();

  /* Never reach this code */
  while (1);

  /* Return status code */
  /* Return 0; */
}



