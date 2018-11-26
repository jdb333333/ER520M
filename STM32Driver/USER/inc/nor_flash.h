
#ifndef __NOR_FLASH_H
#define __NOR_FLASH_H

/* Target Flash Device: MX29XXXX */
#define  FlashSID   0xC2
#define  FlashDID0  0x2249     //MX29LV160D top boot: 0x22C4, bottom boot: 0x2249

// word模式下字库地址 和 数据空间地址
#define HZK16_ADDR			0
#define HZK16_SIZE			0x40000		//128Kx16bit  实际占用:0---0x3FC60/2
#define HZK24_ADDR 			0x40000
#define HZK24_SIZE			0x80000		//256Kx16bit  实际占用:0x40000/2---0xBF140/2

#define	NOR_FLASH_DATA_ADDR	0xc0000
#define	NOR_FLASH_DATA_SIZE	0xA0000		//640Kx16bit=1280KB	 用于存储数据
#define NOR_FLASH_SIZE		0x100000

#define FLASH_WordProgram_TO	500000		// 360us: 360*120/7=6171   ???
#define FLASH_SectorErase_TO	34000000	// 2s: 2000000*120/7
#define FLASH_ChipErase_TO		550000000	// 32s

//#define  NonSynchronousIO	1

#define		AddrConv(X)   	((X)<<1)
//#define		Shift(X)      	((X)<<1)
#define		DATA_MASK 		0xFFFF


/* Program Parameter */
#define  ANY_ADDR   0x0000
#define  MIN_SECTOR_SIZE  0x00002000    // Minimum sector size ( 8KB, 16KB, 64KB, 128KB ) // 0x10000  64KB
#define  NOR_SECTOR_MASK  0xFFFFE000    // 8KB sector mask																//


/* Basic Function */
void FlashWrite( uint32_t Addr, uint16_t Value );
uint16_t FlashRead( uint32_t Addr );
uint8_t NorFlash_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
uint8_t NorFlash_WReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

/* Utility Function */
void NorFlash_Init(void);
uint8_t CheckSectorProtection( uint32_t Addr );
uint8_t DataToggle( void );

/* Flash Command */
uint8_t Reset_OP( void );
uint8_t CFI_Read_OP( uint32_t Addr, uint16_t *CFI_data );
uint8_t Read_SiliconID_OP( uint16_t *SiliconID );
uint8_t Read_DeviceID_OP( uint8_t DeviceIDNum, uint16_t *DeviceID );
uint8_t Sector_Protect_Verify_OP( uint32_t SectorAddr, uint16_t *ProStatus );
uint8_t Program_OP( uint32_t Addr, uint16_t Data );
uint8_t Chip_Erase_OP( void );
uint8_t Sector_Erase_OP( uint32_t Addr, uint16_t SC );
uint8_t Erase_Suspend( void );
uint8_t Erase_Resume( void );

#define FlashReadWord(Addr)     *(__IO uint16_t*)(Addr)
#define FlashReadByte(Addr)     *(__IO uint8_t*)(Addr)
#endif 	//__NOR_FLASH_H

