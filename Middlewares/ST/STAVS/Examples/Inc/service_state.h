/**
******************************************************************************
* @file    application_state.h
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   keep the application  state
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
#ifndef _application_state_
#define _application_state_
#include "avs.h"

extern AVS_Instance_Factory sInstanceFactory;
extern AVS_Handle               hInstance;

const void *assets_load(const char *pName, uint32_t *pSize, uint32_t flags);
void  assets_free(const void *pRes);



#define EVT_ENDURANCE_MSG                 ((AVS_Event)(EVT_USER + 1))
#define EVT_ENDURANCE_TEST_START          ((AVS_Event)(EVT_USER + 2))
#define EVT_ENDURANCE_ALARM_RING_START    ((AVS_Event)(EVT_USER + 3))
#define EVT_ENDURANCE_ALARM_RING_STOP     ((AVS_Event)(EVT_USER + 4))
#define EVT_ENDURANCE_ADD_ALARM           ((AVS_Event)(EVT_USER + 5))
#define EVT_ENDURANCE_DEL_ALARM           ((AVS_Event)(EVT_USER + 6))
#define EVT_ENDURANCE_TASK_START          ((AVS_Event)(EVT_USER + 7))
#define EVT_ENDURANCE_TASK_DYING          ((AVS_Event)(EVT_USER + 8))
#define EVT_NETWORK_SIM_TASK_START        ((AVS_Event)(EVT_USER + 9))
#define EVT_NETWORK_SIM_TASK_DYING        ((AVS_Event)(EVT_USER + 10))



typedef enum t_player_evt
{
  EVT_PLAYER_ERROR = EVT_USER + 100,
  EVT_PLAYER_START,
  EVT_PLAYER_BUFFERING,
  EVT_PLAYER_STOPPED,
  EVT_PLAYER_PAUSED,
  EVT_PLAYER_RESUMED,
  EVT_PLAYER_REPORT,
  EVT_PLAYER_MP3_TASK_START,
  EVT_PLAYER_MP3_TASK_DYING,
  EVT_PLAYER_TASK_START,
  EVT_PLAYER_TASK_DYING,
  EVT_PLAYER_OPEN_STREAM,
  EVT_PLAYER_CLOSE_STREAM
} player_evt_t;


typedef struct application_state
{
  uint32_t       evt;
  uint32_t         evt_param;
  char             tIpString[30];
  char             tTimeString[30];
  char             tVersionString[150];
  char             tFrontEndInfo[150];

  char             tTimeZone[20];
  char             tSysInfo[120];
  char             tBuffInfo[160];
  AVS_TIME         lastEvtTime;
  AVS_TIME         curTime;
  char             tInfoTest[256];
  uint32_t         testPassedCount;
  uint32_t         testErrorCount;
  uint32_t         testTimeoutCount;
  uint32_t         testRetryCount;
  uint32_t         testLoops;
  AVS_TIME         testTimeStart;
  uint32_t         bufAuxPlayer;

} Application_State;

extern Application_State    gAppState;

#endif /* _application_state_ */


