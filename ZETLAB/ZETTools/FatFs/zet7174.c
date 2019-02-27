/*=============================================================================
    Project: 		ZET7174
    Author:			Василевич М.
    Version: 		1.0
    Description:	Модуль работы с ZET7174 через FT232H	
======= Update History =========================================================
   Date			Author		Rev		Reason Description 
28.02.2012		Василевич	1.0		Создание
==============================================================================*/
           
#include <windows.h>
#include "ZET7174.h"

/*-------------------------------------------------------------------------*/
/* Defines */

/*----------------------------------------------------------------------------*/
/* Data Types */

/*-------------------------------------------------------------------------*/
/* Constant */

const unsigned char CRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};
const unsigned char CRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};

/*-------------------------------------------------------------------------*/
/* Variables */

unsigned char Rx_Buf[64 * 1024];
unsigned char Tx_Buf[64 * 1024];
DWORD FrameCounter = 0;

/*-------------------------------------------------------------------------*/
/* Functions */

FT_STATUS FT232_SPI_Transmit(FT_HANDLE ftHandle, unsigned char *buf, DWORD sz);
DWORD FT232_SPI_Receive(FT_HANDLE ftHandle, unsigned char *buf, DWORD sz);

//---------------------------------------------------------------------------
// расчёт CRC16 из проекта Zet7xxx
unsigned short CRC16(unsigned char *pBuff, int len)
{
    unsigned char	Hi = 0xFF;
    unsigned char	Lo = 0xFF;
    unsigned		TableIdx;
    
    while(len--)
    {
        TableIdx = Hi ^ *pBuff++;
        
        Hi = Lo ^ CRCHi[TableIdx];
        Lo = CRCLo[TableIdx];
    }
    return (Hi << 8 | Lo);
}

//---------------------------------------------------------------------------
// инициализация устройства
// freq - частота обмена по SPI, не более 30МГц
FT_HANDLE FT232_SPI_Init(BYTE Drive, uint32 freq)
{
  FT_HANDLE ftHandle = NULL;
  FT_STATUS status;
  ChannelConfig channelConf; 

    channelConf.ClockRate = freq; 
    channelConf.LatencyTimer = 255; 
    channelConf.configOptions = SPI_CONFIG_OPTION_MODE2 | SPI_CONFIG_OPTION_CS_DBUS3 | SPI_CONFIG_OPTION_CS_ACTIVELOW; 
    channelConf.Pin = 0x00000000;
    // открываем первое попавшееся устройство
    status = SPI_OpenChannel(Drive,&ftHandle); 
    //if (status != FT_OK) printf("Device open error!");
    status = SPI_InitChannel(ftHandle,&channelConf); 
    //if (status != FT_OK) printf("Init channel error!");
    // все GPIO на вход 
    FT_WriteGPIO(ftHandle, 0x00, 0x00);
    // инициализируем интерфейс Blackfin
    Blackfin_Restart(ftHandle);
    // сброс счётчика пакетов
    FrameCounter = 0;

  return ftHandle;
}

//---------------------------------------------------------------------------
// Остановка устройства
FT_STATUS FT232_SPI_Close(FT_HANDLE ftHandle)
{
	return SPI_CloseChannel(ftHandle);
}

//---------------------------------------------------------------------------
// передача данных на устройство
// buf - указатель на буфер данных
// sz - размер данных в байтах
FT_STATUS FT232_SPI_Transmit(FT_HANDLE ftHandle, unsigned char *buf, DWORD sz)
{
    FT_STATUS status = FT_OK;
    DWORD real_sz = 0;

    status = SPI_Write(ftHandle, buf, sz, &real_sz,  
        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE); 
    if (status != FT_OK)
    {
        //printf("Write error!\n");
        FT232_SPI_Restart(ftHandle);
    }

    return status;
}

//---------------------------------------------------------------------------
// приём данных с устройства
// buf - указатель на буфер данных
// sz - размер данных в байтах
DWORD FT232_SPI_Receive(FT_HANDLE ftHandle, unsigned char *buf, DWORD sz)
{
    FT_STATUS status = FT_OK;
    DWORD real_sz = 0;

    status = SPI_Read(ftHandle, buf, sz, &real_sz, 
        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
    if (status != FT_OK)
    {
        //printf("Read error!\n");
        FT232_SPI_Restart(ftHandle);
    }

    return real_sz;
}

//---------------------------------------------------------------------------
// перезапуск машины состояний обмена в Blackfin (дёргает одним из GPIO)
void Blackfin_Restart(FT_HANDLE ftHandle)
{
    // передёргиваем пин 29 FT232H
    Sleep(10);
    FT_WriteGPIO(ftHandle, 0x20, 0x20); // Set D5
    Sleep(10);
    FT_WriteGPIO(ftHandle, 0x20, 0x00); // Clear D5
    Sleep(10);
}

//---------------------------------------------------------------------------
// перезапуск интерфейса в случае ошибки обмена
void FT232_SPI_Restart(FT_HANDLE ftHandle)
{
    Blackfin_Restart(ftHandle);
    // ... 
    // м.б. имеет смысл добавит переподключение FT232H
}

//---------------------------------------------------------------------------
// ждёт готовности устройства к ответу
// 0x00 - Ок, 0xFF - таймаут
unsigned char FT232_SPI_WaitReady(FT_HANDLE ftHandle)
{
    unsigned char GPIO_test = 0;
    time_t t = GetTickCount64();

    do
    {
        FT_ReadGPIO(ftHandle, &GPIO_test);
        if ((GetTickCount64() - t) > TX_RX_READY_TIMEOUT)
            return 0xFF;
    }
    while (!(GPIO_test & 0x04));

    return 0x00;
}

//---------------------------------------------------------------------------
// формирует и отправляет пакет
// f_type, f_status, f_size, d_buf - см. описание структуры пакетов
FT_STATUS FT232_PacketTransmit(FT_HANDLE ftHandle, 
    unsigned char f_type, unsigned char f_status, unsigned short f_size, unsigned char *f_reserv, unsigned char *d_buf)
{
    unsigned char *ptr = Tx_Buf;
    unsigned short CRC;

    // начальное поле
    memset(ptr, 0x00, 2);
    ptr += 2;
    // префикс
    *ptr++ = 0xAA; 
    *ptr++ = 0x55;
    // счётчик
    memcpy(ptr, &FrameCounter, sizeof(FrameCounter));
    ptr += sizeof(FrameCounter);
    // тип
    *ptr++ = f_type;
    // статус
    *ptr++ = f_status;
    // размер
    memcpy(ptr, &f_size, sizeof(f_size));
    ptr += sizeof(f_size);
    // данные
    memcpy(ptr, d_buf, f_size);
    ptr += DATA_SIZE;
    // резерв
    memcpy(ptr, f_reserv, RESERV_SIZE);
    ptr += RESERV_SIZE;
    // CRC16
    CRC = CRC16(&Tx_Buf[2], CRC_SIZE);
    memcpy(ptr, &CRC, sizeof(CRC));
    ptr += 2;
    // конечное поле
    memset(ptr, 0x00, 2);
    // инкремент счётчика пакетов
    FrameCounter++;

    return FT232_SPI_Transmit(ftHandle, Tx_Buf, FRAME_SIZE);
}

//---------------------------------------------------------------------------
// принимает и разбирает пакет
// f_type, f_status, f_size, d_buf - см. описание структуры пакетов
// 1 - ошибка, 0 - Ок
unsigned char FT232_PacketReceive(FT_HANDLE ftHandle, 
    unsigned char *f_type, unsigned char *f_status, unsigned short f_size, unsigned char *f_reserv, unsigned char *d_buf)
{
    unsigned short CRC;
    unsigned char ret = 1;

    if (FRAME_SIZE == FT232_SPI_Receive(ftHandle, Rx_Buf, FRAME_SIZE))
    {
        // префикс
        if ((Rx_Buf[2] == 0xAA) && (Rx_Buf[3] == 0x55))
        {
            // CRC
            memcpy(&CRC, &Rx_Buf[FRAME_SIZE - 2 - 2], sizeof(CRC));
            if (CRC16(&Rx_Buf[2], CRC_SIZE) == CRC)
            {
                // тип
                *f_type = Rx_Buf[8];
                // статус
                *f_status = Rx_Buf[9];
                // размер
                memcpy(&f_size, &Rx_Buf[10], sizeof(f_size));
                // данные
                if (f_size <= DATA_SIZE)
                {			
                    memcpy(d_buf, &Rx_Buf[12], f_size);
                    ret = 0;
                }
                // резерв
                memcpy(f_reserv, &Rx_Buf[FRAME_SIZE - 2 - 2 - 48], 48);
            }
        }
    }

    return ret;
}
//---------------------------------------------------------------------------
// запись SD-карты
// sect - номер сектора, sect_num - количество секторов (ограничено, см. описание структуры пакетов), buf - буфер
// 1 - ошибка, 0 - Ок
unsigned char FT232_SD_SectorWrite(FT_HANDLE ftHandle, DWORD sect, unsigned char sect_num, unsigned char *buf)
{
    unsigned char reserv_buf[RESERV_SIZE];
    unsigned char pack_type, dev_st;

    if ((sect_num > DATA_SIZE/SECT_SIZE) || (!sect_num))
        return 1;

    // номер начального сектора
    memcpy(reserv_buf, &sect, 4);
    // передача
    FT232_PacketTransmit(ftHandle, SD_WRITE_SECTOR, STATUS_OK, SECT_SIZE*sect_num, reserv_buf, buf);
    // ждём ответа
    if (FT232_SPI_WaitReady(ftHandle))
    {
        FT232_SPI_Restart(ftHandle);
        return 1;
    }
    // приём
    if (FT232_PacketReceive(ftHandle, &pack_type, &dev_st, SECT_SIZE*sect_num, reserv_buf, buf))
    {
        FT232_SPI_Restart(ftHandle);
        return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------
// чтение SD-карты
// sect - номер сектора, sect_num - количество секторов (ограничено, см. описание структуры пакетов), buf - буфер
// 1 - ошибка, 0 - Ок
unsigned char FT232_SD_SectorRead(FT_HANDLE ftHandle, DWORD sect, unsigned char sect_num, unsigned char *buf)
{
    unsigned char reserv_buf[RESERV_SIZE];
    unsigned char pack_type, dev_st;

    if ((sect_num > DATA_SIZE/SECT_SIZE) || (!sect_num))
        return 1;

    // номер начального сектора
    memcpy(reserv_buf, &sect, 4);
    // передача
    FT232_PacketTransmit(ftHandle, SD_READ_SECTOR, STATUS_OK, SECT_SIZE*sect_num, reserv_buf, buf);
    // ждём ответа
    if (FT232_SPI_WaitReady(ftHandle))
    {
        FT232_SPI_Restart(ftHandle);
        return 1;
    }
    // приём
    if (FT232_PacketReceive(ftHandle, &pack_type, &dev_st, SECT_SIZE*sect_num, reserv_buf, buf))
    {
        FT232_SPI_Restart(ftHandle);
        return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------
