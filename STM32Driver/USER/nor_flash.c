/*
 * COPYRIGHT (c) 2010 MACRONIX INTERNATIONAL CO., LTD
 * MX29-series Parallel Flash Low Level Driver (LLD) Sample Code
 *
 * Command functions source code.
 * This sample code provides a reference, not recommand for direct using.
 *
 * $Id: MX29_CMD.c,v 1.2 2010/04/14 05:53:47 benhuang Exp $
 */

#include "stm32f2xx.h"
#include "nor_flash.h"
#include "fsmc_sram.h"
#include "TypeDef.h"

const uint32_t SectorAddr[] = // Offset from BASE_ADDR of blocks
{
	0x000000L, 		//SA0 16 Kbytes
	0x004000L,      //SA1 8 Kbytes
	0x006000L,      //SA2 8 Kbytes
	0x008000L,      //SA3 32 Kbytes
	0x010000L,      //SA4 64 Kbytes
	0x020000L,      //SA5 64 Kbytes
	0x030000L,      //SA6 64 Kbytes
	0x040000L,      //SA7 64 Kbytes
	0x050000L,      //SA8 64 Kbytes
	0x060000L,      //SA9 64 Kbytes
	0x070000L,      //SA10 64 Kbytes
	0x080000L,      //SA11 64 Kbytes
	0x090000L,      //SA12 64 Kbytes
	0x0a0000L,      //SA13 64 Kbytes
	0x0b0000L,      //SA14 64 Kbytes
	0x0c0000L,      //SA15 64 Kbytes
	0x0d0000L,      //SA16 64 Kbytes
	0x0e0000L,      //SA17 64 Kbytes
	0x0f0000L,      //SA18 64 Kbytes
	0x100000L,      //SA19 64 Kbytes
	0x110000L,      //SA20 64 Kbytes
	0x120000L,      //SA21 64 Kbytes
	0x130000L,      //SA22 64 Kbytes
	0x140000L,      //SA23 64 Kbytes
	0x150000L,      //SA24 64 Kbytes
	0x160000L,      //SA25 64 Kbytes
	0x170000L,      //SA26 64 Kbytes
	0x180000L,      //SA27 64 Kbytes
	0x190000L,      //SA28 64 Kbytes
	0x1a0000L,      //SA29 64 Kbytes
	0x1b0000L,      //SA30 64 Kbytes
	0x1c0000L,      //SA31 64 Kbytes
	0x1d0000L,      //SA32 64 Kbytes
	0x1e0000L,      //SA33 64 Kbytes
	0x1f0000L       //SA34 64 Kbytes
};

/* Global variable */
static uint8_t Security_Mode = FALSE; // TRUE:  Flash enter security mode
                                  // FALSE: Flash is not in security mode
volatile uint32_t Flash_Timeout = FLASH_WordProgram_TO;
/*
 * Function:     Reset_OP
 * Arguments:    none
 * Return Value: SUCCESS
 * Description:  This function places the flash in the Read Array mode .
 *               In this mode the flash can be read as normal memory.
 *
 * Pseudo Code:
 *     Step 1: write command sequence
 */
uint8_t Reset_OP( void )
{
   /* Step 1: write reset command s */
   FlashWrite( ANY_ADDR, 0x00F0 ); /* 1st Cycle: write 0x00F0 to ANY address */
   return SUCCESS;

} /* EndFunction FlashReset */

/*
 * Function:     CFI_Read_OP
 * Arguments:    addr -> Address of CFI array
 *               CFI_data -> Returned CFI data
 * Return Value: SUCCESS
 * Description:  This function can be used to read the CFI value of the flash.
 *
 * Pseudo Code:
 *     Step 1: Send the CFI READ command
 *     Step 2: Read the CFI value
 *     Step 3: Assign the CFI value and go back to Read mode
 */
uint8_t CFI_Read_OP( uint32_t Addr, uint16_t *CFI_data )
{
    /* Step 1: Send the CFI READ command */
    FlashWrite( 0x55, 0x98 );    // 1st Cycle

    /* Step 2: Read the CFI value */
    *CFI_data = FlashRead( Addr );

    /* Step 3: Return the CFI value and go back Read mode */
    Reset_OP();               // Reset command

    return SUCCESS;
}

/*
 * Function:     Read_SiliconID_OP
 * Arguments:    SiliconID -> Returned silicon ID.
 * Return Value: SUCCESS
 * Description:  This function can be used to read the SiliconID of the flash.
 *
 * Pseudo Code:
 *     Step 1: Send the Automatic Select instruction
 *     Step 2: Read the SiliconId
 *     Step 3: Return the SiliconId and go back Read mode
 */
uint8_t Read_SiliconID_OP( uint16_t *SiliconID )
{

    /* Step 1: Send the Automatic Select command */
    FlashWrite( 0x555, 0xAA );        // 1st Cycle
    FlashWrite( 0x2AA, 0x55 );        // 2nd Cycle
    FlashWrite( 0x555, 0x90 );        // 3rd Cycle

    /* Step 2: Read the SiliconId */
    *SiliconID = FlashRead( 0x00 );    // 4th Cycle, A1 = 0, A0 = 0

    /* Step 3: Return the SiliconId and go back Read mode */
    Reset_OP();                       // Reset command

    return SUCCESS;
}


/*
 * Function:     Read_DeviceID_OP
 * Arguments:    DeviceIDNum -> Chose require Device ID type.
 *               DeviceID -> Returned Device ID.
 * Return Value: SUCCESS
 * Description:  This function can be used to read the Device ID of the flash.
 *
 * Pseudo Code:
 *     Step 1: Chose require Device ID type
 *     Step 2: Send the Automatic Select instruction
 *     Step 3: Read the DeviceID
 *     Step 4: Return the DeviceID and go back Read mode
 */
uint8_t Read_DeviceID_OP( uint8_t DeviceIDNum, uint16_t *DeviceID )
{
    uint8_t DeviceIDAddr;

    /* Step 1: Choose require Device ID type */
    switch( DeviceIDNum ){
        case 0:
            DeviceIDAddr = 0x01;
            break;
        case 1:
            DeviceIDAddr = 0x0E;
            break;
        case 2:
            DeviceIDAddr = 0x0F;
            break;
        default:
            DeviceIDAddr = 0x01;
            break;
    }

    /* Step 2: Send the Automatic Select command */
    FlashWrite( 0x555, 0xAA );             // 1st Cycle
    FlashWrite( 0x2AA, 0x55 );             // 2nd Cycle
    FlashWrite( 0x555, 0x90 );             // 3rd Cycle

    /* Step 3: Read the SliconId */
    *DeviceID = FlashRead( DeviceIDAddr ); // 4th Cycle

    /* Step 4: Return the SliconId and go back Read mode */
    Reset_OP();                            // Reset command

    return SUCCESS;
}


/*
 * Function:     Sector_Protect_Verify_OP
 * Arguments:    SectorAddr -> Sector address which would be verified
 *               ProtStatus -> Returned protect status.
 * Return Value: ERROR, SUCCESS
 * Description:  This function read Sector Protect Status.
 *
 * Pseudo Code:
 *     Step 1: Check address range
 *     Step 2: Send the Automatic Select instruction
 *     Step 3: Read the Protect Status
 *     Step 4: Return the Sector Protect Status and go back Read mode
 */
uint8_t Sector_Protect_Verify_OP( uint32_t SectorAddr, uint16_t *ProtStatus )
{
    /* Step 1: Check address range */
    if( SectorAddr > NOR_FLASH_SIZE ) return ERROR;

    /* Step 2: Send the Automatic Select command */
    FlashWrite( 0x555, 0xAA );        // 1st Cycle
    FlashWrite( 0x2AA, 0x55 );        // 2nd Cycle
    FlashWrite( 0x555, 0x90 );        // 3rd Cycle

    /* Step 3: Read the Protect Status */
    *ProtStatus = FlashRead( (SectorAddr&NOR_SECTOR_MASK) + 0x02 ); // 4th Cycle, read "00" -> unprotected,

    /* Step 4: Return the Protect Status and go back Read mode */
    Reset_OP();                       // Reset command

    return SUCCESS;
}


/*
 * Function:     Program_OP
 * Arguments:    Addr -> Programming address of flash
 *               Data -> Data to be programmed
 * Return Value: ERROR, SUCCESS, ERROR
 *               ERROR
 * Description:  Program data to flash array
 *
 * Pseudo Code:
 *     Step 1: Check address range
 *     Step 2: Check protection status
 *     Step 3: Send Program command
 *     Step 4: Send Program address & data
 *     Step 5: Follow Data Toggle Flow Chart until Program/Erase Controller has completed
 *     Step 6: Return to Read Mode.
 */
uint8_t Program_OP( uint32_t Addr, uint16_t Data )
{
    /* Step 1: Check address range */
    if( Addr > NOR_FLASH_SIZE ) return ERROR;

    /* Step 2: Check protection status */
    if( CheckSectorProtection( Addr ) == ERROR ) return ERROR;

    /* Step 3: Send Program command */
    FlashWrite( 0x555, 0xAA );        // 1st Cycle
    FlashWrite( 0x2AA, 0x55 );        // 2nd Cycle
    FlashWrite( 0x555, 0xA0 );        // 3rd Cycle

    /* Step 4: Send Program address & data  */
    FlashWrite( Addr, Data );                   // 4th Cycle

    /* Step 5: Follow Data Toggle Flow Chart until Program Controller has completed */
	Flash_Timeout = FLASH_WordProgram_TO;

    if( DataToggle() != (uint8_t)SUCCESS
#ifdef NoSynchronousIO
      | DataToggle() != (uint8_t)SUCCESS	   //DataToggle() != (ReturnMsg)Flash_InProgress
#endif
    )
    {
      /* Step 6: Return to Read Mode (if an error occurred) */
        Reset_OP();
        return ERROR;
    } /* EndIf */
    return SUCCESS;
}



/*
 * Function:     Chip_Erase_OP
 * Arguments:    none
 * Return Value: Flash_Sucess, Flash_EraseFailed
 * Description:  The Chip Erase operation is used erase all the data within the memory array
 *
 * Pseudo Code:
 *     Step 1: Send the Chip Erase command
 */
uint8_t Chip_Erase_OP( void )
{
    /* Step 1: Send the Chip Erase command*/
    FlashWrite( 0x555, 0xAA );        // 1st Cycle
    FlashWrite( 0x2AA, 0x55 );        // 2nd Cycle
    FlashWrite( 0x555, 0x80 );        // 3rd Cycle
    FlashWrite( 0x555, 0xAA );        // 4th Cycle
    FlashWrite( 0x2AA, 0x55 );        // 5th Cycle
    FlashWrite( 0x555, 0x10 );        // 6th Cycle

    /* Step 2: Follow Data Toggle Flow Chart until erase has completed */
	Flash_Timeout = FLASH_ChipErase_TO;

    if( DataToggle() != (uint8_t)SUCCESS
#ifdef NoSynchronousIO
      | DataToggle() != (uint8_t)SUCCESS    //DataToggle() != (ReturnMsg)Flash_InProgress
#endif
    ){
        /* Step 3: Return to Read Mode (if an error occurred) */
        Reset_OP();
        return ERROR;
    } /* EndIf */

    return SUCCESS;
}

/*
 * Function:     Sector_Erase_OP
 * Arguments:    Addr -> Sector address to be erased
 *               SC -> Count of sector adress to be erased
 * Return Value: SUCCESS, Flash_EraseFailed, ERROR
 * Description:  The Sector Erase operation is used erase sector data within the memory array
 *
 * Pseudo Code:
 *     Step 1: Check address range
 *     Step 2: Check protection status
 *     Step 3: Send the Sector Erase command
 *     Step 4: Send the Sector address to be erased
 *     Step 5: Follow Data Toggle Flow Chart until erase has completed
 *     Step 6: Return to Read Mode
 */
uint8_t Sector_Erase_OP( uint32_t Addr, uint16_t SC )
{
    uint16_t i;

    /* Step 1: Check address range */
    if( Addr > NOR_FLASH_SIZE ) return ERROR;

    /* Step 2: Check protection status */
    if( CheckSectorProtection( Addr ) == ERROR ) return ERROR;

    /* Step 3: Send the Sector Erase command*/
    FlashWrite( 0x555, 0xAA );            // 1st Cycle
    FlashWrite( 0x2AA, 0x55 );            // 2nd Cycle
    FlashWrite( 0x555, 0x80 );            // 3rd Cycle
    FlashWrite( 0x555, 0xAA );            // 4th Cycle
    FlashWrite( 0x2AA, 0x55 );            // 5th Cycle

    /* Step 4: Send the Sector address to be erased */
    for (i=0; i<SC; i++)
    {
        FlashWrite( Addr, 0x30 );
        /* Check for Erase Timeout Period 50us (is bit DQ3 set?) */
        if( ( FlashRead( Addr ) & 0x0008 ) != 0 )
            break;    // Cannot set any more blocks due to timeout
        Addr = Addr + 0x8000;     //??? to next sector address
    }

    /* Step 5: Follow Data Toggle Flow Chart until erase has completed */
	Flash_Timeout = FLASH_SectorErase_TO;

    if( DataToggle() != (uint8_t)SUCCESS
#ifdef NoSynchronousIO
      | DataToggle() != (uint8_t)SUCCESS  //DataToggle() != (ReturnMsg)Flash_InProgress
#endif
    ){
        /* Step 6: Return to Read Mode (if an error occurred) */
        Reset_OP();
        return ERROR;
    } /* EndIf */

    return SUCCESS;
}

/*
 * Function:     Erase_Suspend
 * Arguments:    none
 * Return Value: SUCCESS
 * Description:  This function suspends erase operation.
 *
 * Pseudo Code:
 *     Step 1: Send the Erase suspend command to the device
 */
uint8_t Erase_Suspend( void )
{
   /* Step 1: Send the Erase Suspend command */
   FlashWrite( ANY_ADDR, 0xB0 );

   return SUCCESS;

} /* EndFunction erase suspend */

/*
 * Function:     Erase_Resume
 * Arguments:    none
 * Return Value: SUCCESS
 * Description:  This function resume erase operation.
 *
 * Pseudo Code:
 *     Step 1: Send the Erase resume command to the device
 */
uint8_t Erase_Resume( void )
{

   /* Step 1: Send the Erase Resume command */
   FlashWrite( ANY_ADDR, 0x30 );

   return SUCCESS;

} /* EndFunction erase resume*/

/*
 * Function:     DataToggle
 * Arguments:    None.
 * Return Value: SUCCESS, ERROR, ERROR
 * Description:  Using data polling & toggle bit to detect program and erase operation
 *               completion
 *
 * Pseudo Code:
 *     Step 1: Polling data to detect DQ6 bit first time
 *     Step 2: Polling data to detect DQ6 bit second time
 *     Step 3: If DQ6 did not toggle between the two reads then return SUCCESS
 *     Step 4: Else if DQ5 is zero then operation is not yet complete, goto step 1
 *     Step 5: Else (DQ5 != 0), read DQ6 again
 *     Step 6: If DQ6 did not toggle between the last two reads then return SUCCESS
 *     Step 7: Else return ERROR
 */
uint8_t DataToggle( void )
{
    uint8_t tog_status1, tog_status2;    // toggle value read from any address
                                       // offset within the Flash Memory
    uint32_t cnt;
#ifndef NonSynchronousIO
		// by ouhs timeout
    while( cnt++ < Flash_Timeout ) {
      /* TimeOut: If, for some reason, the hardware fails then this loop exit
                  and the function return flash_OperationTimeOut.  */
#endif
      /* Step 1: Polling data to detect DQ6 bit fisrt time */
        tog_status2 = FlashRead( ANY_ADDR );    // Read DQ5 and DQ6 from the Flash (any address)

      /* Step 2: Polling data to detect DQ6 bit second time*/
        tog_status1 = FlashRead( ANY_ADDR );    // Read DQ6 from the Flash (any address)

      /* Step 3: If DQ6 did not toggle between the two reads then return SUCCESS */
        if( (tog_status1 & 0x0040) == (tog_status2 & 0x0040) )    // DQ6 == NO Toggle
            return SUCCESS;

      /* Step 4: Else if DQ5 is zero then operation is not yet complete */
        if( (tog_status2 & 0x0020) != 0x0020 )
#ifndef NonSynchronousIO
           continue;
#else
           return ERROR; //return Flash_InProgress;   // Return IO access
#endif

      /* Step 5: Else (DQ5 == 1), read DQ6 twice */
        tog_status1 = FlashRead( ANY_ADDR );    // Read DQ6 from the Flash (any address)
        tog_status2 = FlashRead( ANY_ADDR );

      /* Step 6: If DQ6 did not toggle between the last two reads then return SUCCESS */
        if( (tog_status2 & 0x0040) == (tog_status1 & 0x0040) )    // DQ6 == NO Toggle
            return SUCCESS;

      /* Step 7: Else return ERROR */
        else {
      /* DQ6 == Toggle here means fail */
            return ERROR;
        } //EndIf
#ifndef NonSynchronousIO
    } // EndWhile
    return ERROR;   // if exit from while loop then time out exceeded
#endif

} // EndFunction DataToggle

/*
 * Function:     CheckSectorProtection
 * Arguments:    Addr -> address to check
 * Return Value: SUCCESS:Flash_SectorUnprotected, ERROR:Flash_SectorProtected
 *               ERROR
 * Description:  Check sector is protected or not
 */
uint8_t CheckSectorProtection( uint32_t Addr )
{
    uint16_t status_reg;
    uint8_t rtMsg;

    if( Security_Mode == FALSE ){
        rtMsg = Sector_Protect_Verify_OP( (Addr&NOR_SECTOR_MASK), &status_reg );

        if( rtMsg == SUCCESS ){
            if( status_reg == 0 )
                return SUCCESS;
            else
                return ERROR;
        }else
            return rtMsg;
    }else
        return SUCCESS;  /* Always return unprotect state prevent
                                            unexpected exit from security mode */
}

/*
 * Basic function
 */

/*
 * Function:     FlashWrite
 * Arguments:    Addr -> address to write
 *               Value -> data to write
 * Return Value: None.
 * Description:  Write a data to flash
 */
void FlashWrite( uint32_t Addr, uint16_t Value )
{

    /* Please insert your flash write function */
    *(__IO uint16_t*)(NOR_FLASH_ADDR + AddrConv(Addr)) = Value;

}

/*
 * Function:     FlashRead
 * Arguments:    Addr -> address to read
 * Return Value: None.
 * Description:  Read a data from flash
 */
uint16_t FlashRead( uint32_t Addr )
{
    /* Please insert your flash read function */
    return *(__IO uint16_t*)(NOR_FLASH_ADDR + AddrConv(Addr));
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
uint8_t NorFlash_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
	for (; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
	{
		/* Write a half-word to the memory */
		Program_OP( WriteAddr, *pBuffer++ );

		/* Increment the address*/
		WriteAddr++;
	}
	return SUCCESS;
}

/**按字读取数据
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
uint8_t NorFlash_WReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
	ReadAddr+=NOR_FLASH_ADDR;
	for (; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
	{
		/* Read a half-word from the memory */
		*pBuffer++ = *(__IO uint16_t*) (ReadAddr);
		ReadAddr+=2;
	}
	return SUCCESS;
}

/**按字节读取数据
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
uint8_t NorFlash_BReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumBytesToRead)
{
	ReadAddr+=NOR_FLASH_ADDR;
	for (; NumBytesToRead != 0; NumBytesToRead--) /* while there is data to read */
	{
		/* Read a half-word from the memory */
		*pBuffer++ = *(__IO uint8_t*) (ReadAddr);
		ReadAddr++;
	}
	return SUCCESS;
}

