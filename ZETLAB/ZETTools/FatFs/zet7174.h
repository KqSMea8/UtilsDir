#ifndef _ZET7174_H_ 
#define _ZET7174_H_ 

#include <SPI/libMPSSE_spi.h>

/*----------------------------------------------------------------------------*/
/* Defines */

// размер сектора SD
#define SECT_SIZE 512
// в пакете
#define DATA_SIZE 2048 // 4096
#define FRAME_SIZE (DATA_SIZE + 64)
#define RESERV_SIZE 48
#define CRC_SIZE (FRAME_SIZE - 4 - 2) // 4 пустый и 2 на CRC
#define FREQ 30000000 // 30MHz
#define TX_RX_READY_TIMEOUT 1000 // ms 
// типы пакетов
#define TEST_PACKET 0
#define MODBUS_PACKET 1
#define SD_READ_SECTOR 2
#define SD_WRITE_SECTOR 3
// ... 
// статусы
#define STATUS_OK 0
// ... 

/*----------------------------------------------------------------------------*/
/* Data Types */

/*----------------------------------------------------------------------------*/
/* Macros */

/*----------------------------------------------------------------------------*/
/* Variables */

/*----------------------------------------------------------------------------*/
/* Functions */

FT_HANDLE FT232_SPI_Init(BYTE Drive, uint32 freq);
    void FT232_SPI_Restart(FT_HANDLE ftHandle);
FT_STATUS FT232_SPI_Close(FT_HANDLE ftHandle);
unsigned char FT232_SPI_WaitReady(FT_HANDLE ftHandle);
FT_STATUS FT232_PacketTransmit(FT_HANDLE ftHandle, 
    unsigned char f_type, unsigned char f_status, unsigned short f_size, unsigned char *f_reserv, unsigned char *d_buf);
unsigned char FT232_PacketReceive(FT_HANDLE ftHandle, 
    unsigned char *f_type, unsigned char *f_status, unsigned short f_size, unsigned char *f_reserv, unsigned char *d_buf);
unsigned char FT232_SD_SectorWrite(FT_HANDLE ftHandle, DWORD sect, unsigned char sect_num, unsigned char *buf);
unsigned char FT232_SD_SectorRead(FT_HANDLE ftHandle, DWORD sect, unsigned char sect_num, unsigned char *buf);
void Blackfin_Restart(FT_HANDLE ftHandle);

/*----------------------------------------------------------------------------*/

#endif /* #ifndef _ZET7174_H_ */
