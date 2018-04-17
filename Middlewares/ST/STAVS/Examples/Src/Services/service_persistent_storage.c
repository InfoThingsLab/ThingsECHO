/**
******************************************************************************
* @file    service_persistent.c
* @author  MCD Application Team
* @version V1.0.0
* @date    20-02-2018
* @brief   persistent storage
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
 * this code is provided as an example , is is not a production code
 *
 ******************************************************************************/

/******************************************************************************
 *
 * This code manages the persistent stoarage of the token
 * this code store the token if flash
 ******************************************************************************/



#include "service.h"

#define       TOKEN_FIRST_CHARS "Atzr|"

uint32_t     flash_base_adress ;
uint32_t     flash_base_sector ;
uint32_t     flash_sector_size ;


/*
  returns the base address sector and size of the flash storage 
  the default implementation is done for a stm32F769I-disco
  for other board it should be overloaded in init_platform.c

*/

__weak void Get_Board_Flash_Storage(uint32_t *pOffset,uint32_t *pSector,uint32_t *pSectorSize);
__weak void Get_Board_Flash_Storage(uint32_t *pOffset,uint32_t *pSector,uint32_t *pSectorSize)
{
  if(pOffset != 0)
  {
     *pOffset = 0x081E0000;
  }
  if(pSector != 0)
  {
     *pSector = FLASH_SECTOR_23;
  }
  if(pSectorSize != 0)
  {
     *pSectorSize = 128;
  }
}



   
static Persistant_t *service_persistent_get_storage(void);
static Persistant_t *service_persistent_get_storage(void)
{
  return (Persistant_t*)flash_base_adress;
}

AVS_Result service_persistent_check_empty(void);
AVS_Result service_persistent_check_empty(void)
{
  
  uint8_t *pByte = (uint8_t *)flash_base_adress;
  for(int32_t a = 0; a < flash_sector_size*1024 ; a++)
  {
    if(pByte[a] != 0xFF)
    {
      AVS_TRACE_ERROR("Check  flash not erased : %08x = %x", &pByte[a], pByte[a]);
      return AVS_ERROR;
    }
  }
  return AVS_OK;
}


static AVS_Result  service_persistent_erase_storage(void);
static AVS_Result  service_persistent_erase_storage(void)
{
  FLASH_EraseInitTypeDef EraseInitStruct = {0};
  uint32_t SectorError = 0;
  
  uint32_t EraseTryCount=10;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */   
  
  
  while(EraseTryCount) 
  {
    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector       = flash_base_sector;
    EraseInitStruct.Banks        = FLASH_BANK_2;
    EraseInitStruct.NbSectors    = 1;
    
    
    HAL_FLASH_Unlock();
    SCB_InvalidateDCache();
    HAL_StatusTypeDef result = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
    HAL_FLASH_Lock();
    if(result == HAL_OK)  
    {
      if(service_persistent_check_empty()==AVS_OK)
      {
        break;
      }
    }
    EraseTryCount--;
    AVS_TRACE_INFO("Re-try to erase the flash");
  }
  
  if(EraseTryCount ==0)
  {
      AVS_TRACE_ERROR("Cannot erase the flash");
      return AVS_ERROR;
  }
  return AVS_OK;  
}


/**
 * @brief Write token from flash
 * @note  BE CAREFULL :the sector number is hard coded below
 *        and this address must be in lin with scatter file content
 * @param[in] context Pointer to the AVS context
 * @return Error code
 **/



AVS_Result service_persistent_write_storage(Persistant_t *pPersistent);
AVS_Result service_persistent_write_storage(Persistant_t *pPersistent)
{
  uint32_t Address ;
  uint64_t Data;
  pPersistent->signature = PERSISTENT_SIGNATURE;


  AVS_TRACE_DEBUG("going to overwrite flash memory");

  /* Unlock the flash */
  HAL_StatusTypeDef status = HAL_FLASH_Unlock();
  if (status != HAL_OK)
  {
    AVS_TRACE_ERROR("HAL_FLASH_Unlock failed :%d", status);
    return AVS_ERROR;
  }
  
  if(service_persistent_erase_storage() != AVS_OK) 
  {
    return AVS_ERROR;
  }

  uint32_t *pStorage = (uint32_t *)pPersistent;
  uint32_t szStorage = (sizeof(*pPersistent)/4) + 1;
  

  /* Write the storage in the flash sector */
  HAL_FLASH_Unlock();
  
  AVS_Result ret = AVS_OK;
  for (int32_t i = 0; i < szStorage; i++)
  {
    Address = flash_base_adress + 4 * i;
    Data = pStorage[i];
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data) != HAL_OK)
    {
      ret = AVS_ERROR;
      break;
    }
  }
  HAL_FLASH_Lock();
  if(ret != AVS_OK)
  {
     AVS_TRACE_ERROR("HAL_FLASH_Program failed while writing at 0x%x", Address);
     return AVS_ERROR;
  }
  
  if(memcmp((const void *)(uint32_t)service_persistent_get_storage(),(const void *)(uint32_t)pStorage,szStorage*sizeof(uint32_t)) != 0)
  {
    AVS_TRACE_ERROR("Write Persistent: Verify error");
    return AVS_ERROR;
    
  }

  AVS_TRACE_DEBUG("Persistent: Refresh OK");
  return  AVS_OK;
}


AVS_Result service_persistent_init_default(void)
{
  Get_Board_Flash_Storage(&flash_base_adress,&flash_base_sector,&flash_sector_size);
  AVS_ASSERT(flash_base_adress != 0);
  AVS_ASSERT(flash_base_sector != 0);
  
  if(service_persistent_get_storage()->root_ca_Store[0] == 0xFF)
  {
    uint32_t assetSize;
    
    const void *pData = service_assets_load("default_amazon_root_ca",&assetSize,0);
    if(pData)
    {
      service_persistent_storage(AVS_PERSIST_SET,AVS_PERSIST_ITEM_CA_ROOT,(char_t *)(uint32_t)pData,assetSize);
    }
    service_assets_free(pData);
  }
  return AVS_OK;
}


uint32_t service_persistent_storage(AVS_Persist_Action action,Avs_Persist_Item item ,char_t *pItem,uint32_t itemSize)
{
  
  if((action == AVS_PERSIST_GET) && (item == AVS_PERSIST_ITEM_TOKEN))
  {
    Persistant_t *pPersistent = service_persistent_get_storage();
    if(pPersistent->signature != PERSISTENT_SIGNATURE)
    {
      AVS_TRACE_DEBUG("Flash storage empty");
      return AVS_ERROR;
    }
    AVS_TRACE_DEBUG("Persistent: Looking from token in Flash");
    /* Check that a token is stored in flash */
    if (strncmp((char_t *) pPersistent->tokenStore, TOKEN_FIRST_CHARS, sizeof(TOKEN_FIRST_CHARS) - 1) )
    {
      AVS_TRACE_DEBUG("Flash doesn't contain a valid token yet");
      return AVS_ERROR;
    }
    int32_t rlen = strlen((char_t *) pPersistent->tokenStore);
    if ( (pItem == NULL) || (itemSize <= rlen ) )
    {
      AVS_TRACE_DEBUG("null buffer, or buffer too small");
      return AVS_ERROR;
    }
    /* Read the token from flash */
    strcpy(pItem, (void *) pPersistent->tokenStore);
    AVS_TRACE_DEBUG("Token read from Flash : %s", pItem);
    return AVS_OK;
  }
  
  if((action == AVS_PERSIST_SET) && (item == AVS_PERSIST_ITEM_TOKEN))
  {
    AVS_TRACE_DEBUG("Persistent: Write token in Flash");
    
    /* Save the storage in memory */
    Persistant_t *pPersistent = pvPortMalloc(sizeof(Persistant_t ));
    AVS_ASSERT(pPersistent);
    memcpy(pPersistent,service_persistent_get_storage(),sizeof(Persistant_t ));
    /* copy update the storage  */
    memcpy(pPersistent->tokenStore,pItem,itemSize);
    AVS_ASSERT(itemSize < sizeof(pPersistent->tokenStore));
    /* Re-write the storage */
    AVS_Result ret = service_persistent_write_storage(pPersistent);
    /* clean-up */
    vPortFree(pPersistent);
    return ret;
  }
  
  
  if((action == AVS_PERSIST_GET_POINTER) && (item == AVS_PERSIST_ITEM_CA_ROOT))
  {
    AVS_TRACE_DEBUG("Read CA Root");
    
    /* Save the storage in memory */
    Persistant_t *pPersistent = service_persistent_get_storage();
    if(pPersistent->signature != PERSISTENT_SIGNATURE)
    {
      AVS_TRACE_DEBUG("Flash storage empty");
      return AVS_ERROR;
    }
    if(pPersistent->root_ca_Store[0] == 0xFF) 
    {
      AVS_TRACE_DEBUG("Flash Store empty");
      return AVS_ERROR;
    }
    return (uint32_t)pPersistent->root_ca_Store;
  }
  
  if((action == AVS_PERSIST_SET) && (item == AVS_PERSIST_ITEM_CA_ROOT))
  {
    AVS_TRACE_DEBUG("Persistent: Write Item in Flash");
    
    /* Save the storage in memory */
    Persistant_t *pPersistent = pvPortMalloc(sizeof(Persistant_t ));
    AVS_ASSERT(pPersistent);
    memcpy(pPersistent,service_persistent_get_storage(),sizeof(Persistant_t ));
    /* copy update the storage  */
    memcpy(pPersistent->root_ca_Store,pItem,itemSize);
    pPersistent->root_ca_Store[itemSize] = 0;
    itemSize++;
    AVS_ASSERT(itemSize < sizeof(pPersistent->root_ca_Store));
    /* Re-write the storage */
    AVS_Result ret = service_persistent_write_storage(pPersistent);
    /* clean-up */
    vPortFree(pPersistent);
    return ret;
  }
    
  
  return AVS_NOT_IMPL;
}



