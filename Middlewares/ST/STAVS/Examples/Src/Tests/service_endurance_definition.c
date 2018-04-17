
/**
******************************************************************************
* @file    service_endurance_definition.c.c
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

#include "service.h"




/* Include test item objects */
#include "service_test_delay.c"
#include "service_test_sequence.c"
#include "service_test_disconnection.c"
#include "service_test_wakeup.c"
#include "service_test_check_player.c"



#define TEST_WAKEUP              {"Wakeup",1,service_endurance_wakeup,{{EVT_ENDURANCE_TEST_START,"Alexa_wav",0,0},},1,0},
#define TEST_DELAY(Label,nbSec)  {Label,nbSec,service_endurance_delay,{{0,0,0,0},},0,0},


/* Test list for the test session */

Test_item_t tUnitTest[30] =
{
  TEST_WAKEUP
  {
    "Media Player", 60, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "sing_song_wav",0,0},
      {EVT_PLAYER_START,0,0,0},
    }, 1,0},

  TEST_DELAY("Wait some time", 1)
  TEST_WAKEUP
  {
    "Volume up", 10, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "vol_up_wav",0,0},
      {EVT_CHANGE_VOLUME,0,0,0},

    }, 1,0},


  TEST_DELAY("Wait some time", 1)
  TEST_WAKEUP
  {
    "Volume down", 10, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "vol_dn_wav",0,0},
      {EVT_CHANGE_VOLUME,0,0,0},

    }, 1,0},

  TEST_DELAY("Wait some time", 1)
  TEST_WAKEUP
  {
    "Mute", 10, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "mute_wav",0,0},
      {EVT_CHANGE_MUTE,0,0,0},

    }, 1,0},

  TEST_DELAY("Wait some time", 1)
  TEST_WAKEUP

  {
    "unMute", 10, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "unmute_wav",0,0},
      {EVT_CHANGE_MUTE,0,0,0},
    }, 1,0},

  {"Wait Player end", 5 * 60, service_check_player, {{0}}, 1},
  TEST_WAKEUP
  {
    "Multi-turn-timer", 1 * 60, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "set_timer_wav",0,0}, {EVT_STOP_REC,0,0,0},
      {EVT_START_SPEAK,0,0,0}, {EVT_STOP_SPEAK,0,0,0},
      {EVT_START_REC, "six_sec_wav",0,0},
      {EVT_ENDURANCE_ADD_ALARM,0,0,0},
      {EVT_ENDURANCE_ALARM_RING_START,0,0,0},
      {EVT_ENDURANCE_ALARM_RING_STOP,0,0,0}
    }, 1,0},

  TEST_WAKEUP
  {
    "Set Alarm", 20, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "set_alarm_2pm_wav",0,0},
      {EVT_ENDURANCE_ADD_ALARM,0,0,0},
      {EVT_STOP_SPEAK,0,0,0}

    }, 1,0},

  TEST_DELAY("Wait some time", 10)
  TEST_WAKEUP

  {
    "Cancel Alarm", 20, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "cancel_alarm_2PM_wav",0,0},
      {EVT_ENDURANCE_DEL_ALARM,0,0,0},
    }, 1,0},

  TEST_DELAY("Wait some time", 10)
  TEST_WAKEUP
  {
    "Joke", 50, service_endurance_sequence,
    {
      {EVT_ENDURANCE_TEST_START,0,0,0},
      {EVT_START_REC, "tell_me_a_joke_wav",0,0},
      {EVT_STOP_REC,0,0,0},
      {EVT_STOP_SPEAK,0,0,0}
    }, 3,0},

  TEST_WAKEUP
  {"Net Disconnection", 5 * 60, service_endurance_disconnection, {{0,0,0,0}}, 1,0},
  {0}

};

