/**
******************************************************************************
* @file    service_os.c
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   Service Os abstraction
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
/******************************************************************************
*
* this code is provided as an example , It is not a production code
*
******************************************************************************/

#include "service.h"


task_t *task_Create(const char_t *pName, os_pthread  pCb, void *pCookie, size_t stackSize, osPriority  priority)
{
  
  task_t *pTask = (task_t *)pvPortMalloc(sizeof(task_t)) ;
  pTask->osThread.name = (char_t *)(uint32_t)pName;
  pTask->osThread.pthread = pCb;
  pTask->osThread.tpriority = priority;
  pTask->osThread.stacksize = stackSize;
  pTask->osThread.instances = 0;
  pTask->id = osThreadCreate(&pTask->osThread,pCookie);
  if(pTask->id ==0)
  {
    vPortFree(pTask);
    pTask = NULL;
  }
  AVS_ASSERT(pTask != 0);
  return pTask;
}
/* Delete a task */
 
 void task_Delete(task_t *task)
{
  if((task != 0) && (task->id != 0)) 
  {
    osThreadTerminate(task->id);
    task->id = 0;
  }
  vPortFree(task);
}

int8_t mutex_Create(mutex_t *mutex, const char *pName)
{
  mutex->id     = osRecursiveMutexCreate(&mutex->m);  
  if((mutex->id!= 0)  && (pName != 0))
  {
    vQueueAddToRegistry (mutex->id, pName);
  }
  
  AVS_ASSERT(mutex->id!= 0);
  return (int8_t)  ((mutex->id != 0) ? TRUE : FALSE);

}


void mutex_Delete(mutex_t *mutex)
{
  AVS_ASSERT(mutex != 0);
  AVS_ASSERT(mutex->id != 0);
  if(mutex->id != 0)
  {
    vQueueUnregisterQueue(mutex->id);
  }
  osMutexDelete(mutex->id);

}

 
 void mutex_Lock(mutex_t  *mutex)
{
  
  AVS_ASSERT(mutex != 0);
  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
  {
    osRecursiveMutexWait(mutex->id,  osWaitForever);
  }
}

/* Unlock  a recursive mutex */

void mutex_Unlock(mutex_t *mutex)
{
  AVS_ASSERT(mutex != 0);
  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
  {
    osRecursiveMutexRelease(mutex->id);
  }
  
}
