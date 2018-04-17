
/**
******************************************************************************
* @file    service_endurance_test.h
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   test some functions and log events
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

#ifndef _service_endurance_test_
#define _service_endurance_test_
#define ENDURANCE_NETWORK_DELAY_DISCONNECTED     (30*1000)                        /* Bad Network simulation, stays disconnected 30 secs */
#define ENDURANCE_NETWORK_DELAY_CONNECTED        (30*1000)                        /* Bad Network simulation, stays connected 30 secs */

#define MAX_SEQUENCE                           30
#define MAX_STACK                              20


typedef enum action_t
{
  START_TEST,
  EVENT_TEST,
  CHECK_TEST,
  STOP_TEST
} Action_t;

typedef struct t_evt_keeper
{
  uint32_t   evt;
  uint32_t   pparam;
} evt_keeper_t;

typedef struct dialogue_t
{
  uint32_t evt;
  const char_t *pResName;
  uint32_t param;
  const void     *pData;
} Dialogue_t;


typedef struct  test_item_t
{

  char_t    *pTestName;
  uint32_t timeout;
  AVS_Result    (*test)(AVS_Handle handle, struct test_item_t *pItem, Action_t action, uint32_t evt, uint32_t param);
  Dialogue_t     sequence[MAX_SEQUENCE];
  uint32_t      iIndexResult;
  AVS_TIME        start_time;
} Test_item_t;

extern uint32_t bSimulateBadNetwork;
extern Test_item_t tUnitTest[30];


uint32_t        service_endurance_get_state(void);
void            service_endurance_stop(void);
void            service_endurance_start(void);
uint32_t        service_endurance_enable_network_sim( uint32_t state);
void            service_endurance_get_current_test_name(char_t *pName, uint32_t maxlen);
void            service_endurance_say(AVS_Handle handle, const void *pText, uint32_t mode, const char_t *pText2Say);
uint32_t        service_endurance_check_timeout(AVS_Handle handle, Test_item_t *pItem);
AVS_Result      service_endurance_send_msg(AVS_Handle handle, Test_item_t *pItem, AVS_Result retCode, const char_t *pFormat, ...);
void            service_endurance_start_capture(AVS_Handle handle);
#endif /* _service_endurance_test_ */

