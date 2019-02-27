#include "diskio.h"

#include "zet7174.h"

#include "ffconf.h"

#include "eth.h"

#define MAX_SECTOR_COUNT DATA_SIZE / _MAX_SS

static FT_HANDLE ft_handles[_VOLUMES];

fatfs_eth_read_func* p_fatfs_eth_read_func = NULL;
fatfs_eth_write_func* p_fatfs_eth_write_func = NULL;
work_class_ptr p_work_class = NULL;



DSTATUS disk_initialize (BYTE Drive)
{
	if(Drive < ETH_MOUNT_FROM)		//spi
		ft_handles[Drive] = FT232_SPI_Init( Drive, FREQ );

    return 0;
}

DSTATUS disk_close (BYTE Drive)
{
	if(Drive < ETH_MOUNT_FROM && ft_handles[Drive] != NULL)		//spi
		FT232_SPI_Close( ft_handles[Drive] );

	return 0;
}

DSTATUS disk_status (BYTE Drive)
{
    return 0;
}

DRESULT disk_read (BYTE Drive, BYTE* Buffer, DWORD SectorNumber, BYTE SectorCount)
{
	int sector_readed = 0;
	int curr_sector_read = 0;

	do
	{
		

		if(Drive < ETH_MOUNT_FROM)	// spi
		{
			curr_sector_read = (SectorCount - sector_readed <= MAX_SECTOR_COUNT) ? SectorCount - sector_readed : MAX_SECTOR_COUNT;
			FT232_SD_SectorRead( ft_handles[Drive], SectorNumber, curr_sector_read & 0xFF, Buffer );
		}			
		else						// eth
		{
			curr_sector_read = 1;
 
			if(p_fatfs_eth_read_func && p_work_class)		// if function inited
				p_fatfs_eth_read_func(Drive, SectorNumber, Buffer, p_work_class);
		}

		Buffer += curr_sector_read * _MAX_SS;
		SectorNumber += curr_sector_read;
		sector_readed += curr_sector_read;
	}
	while(sector_readed < SectorCount);
	
   

    return RES_OK;
}

#if	_READONLY == 0
DRESULT disk_write (BYTE Drive, const BYTE* Buffer, DWORD SectorNumber, BYTE SectorCount)
{
	int sector_writed = 0;
	int curr_sector_write = 0;

	do
	{
		if(Drive < ETH_MOUNT_FROM)	// spi
		{
			curr_sector_write = (SectorCount - sector_writed <= MAX_SECTOR_COUNT) ? SectorCount - sector_writed : MAX_SECTOR_COUNT;
			FT232_SD_SectorWrite( ft_handles[Drive], SectorNumber, curr_sector_write & 0xFF, (BYTE*)Buffer );
		}
		else
		{
			curr_sector_write = 1;
			if(p_fatfs_eth_write_func && p_work_class)		// if function inited
				p_fatfs_eth_write_func(Drive, SectorNumber, Buffer, p_work_class);
		}
		Buffer += curr_sector_write * _MAX_SS;
		SectorNumber += curr_sector_write;
		sector_writed += curr_sector_write;
	}
	while(sector_writed < SectorCount);

    return RES_OK;
}
#endif

DRESULT disk_ioctl (BYTE Drive, BYTE Command, void* Buffer)
{
    return RES_OK;
}

DWORD get_fattime()
{
    return 0;
}
