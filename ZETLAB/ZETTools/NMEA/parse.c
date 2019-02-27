/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: parse.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/**
 * \file parse.h
 * \brief Functions of a low level for analysis of
 * packages of NMEA stream.
 *
 * \code
 * ...
 * ptype = nmea_pack_type(
 *     (const char *)parser->buffer + nparsed + 1,
 *     parser->buff_use - nparsed - 1);
 * 
 * if(0 == (node = malloc(sizeof(nmeaParserNODE))))
 *     goto mem_fail;
 * 
 * node->pack = 0;
 * 
 * switch(ptype)
 * {
 * case GPGGA:
 *     if(0 == (node->pack = malloc(sizeof(nmeaGPGGA))))
 *         goto mem_fail;
 *     node->packType = GPGGA;
 *     if(!nmea_parse_GPGGA(
 *         (const char *)parser->buffer + nparsed,
 *         sen_sz, (nmeaGPGGA *)node->pack))
 *     {
 *         free(node);
 *         node = 0;
 *     }
 *     break;
 * case GPGSA:
 *     if(0 == (node->pack = malloc(sizeof(nmeaGPGSA))))
 *         goto mem_fail;
 *     node->packType = GPGSA;
 *     if(!nmea_parse_GPGSA(
 *         (const char *)parser->buffer + nparsed,
 *         sen_sz, (nmeaGPGSA *)node->pack))
 *     {
 *         free(node);
 *         node = 0;
 *     }
 *     break;
 * ...
 * \endcode
 */

#include <string.h>
#include <stdio.h>

#include "tok.h"
#include "parse.h"
#include "context.h"
#include "gmath.h"
#include "units.h"

int _nmea_parse_time(const char *buff, int buff_sz, nmeaTIME *res)
{
    int success = 0;

    switch(buff_sz)
    {
    case sizeof("hhmmss") - 1:
        success = (3 == nmea_scanf(buff, buff_sz,
            "%2d%2d%2d", &(res->hour), &(res->min), &(res->sec)
            ));
        break;
    case sizeof("hhmmss.s") - 1:
    case sizeof("hhmmss.ss") - 1:
    case sizeof("hhmmss.sss") - 1:
        success = (4 == nmea_scanf(buff, buff_sz,
            "%2d%2d%2d.%d", &(res->hour), &(res->min), &(res->sec), &(res->hsec)
            ));
        break;
    default:
//        nmea_error("Parse of time error (format error)!");
        success = 0;
        break;
    }

    return (success?0:-1);        
}

/**
 * \brief Define packet type by header (nmeaPACKTYPE).
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @return The defined packet type
 * @see nmeaPACKTYPE
 */
int nmea_pack_type(const char *buff, int buff_sz)
{
    NMEA_ASSERT(buff);

    if(buff_sz < 5)
        return GPNON;

    if(buff[0] != '$' || buff[1] != 'G')                     /* $G */
        return GPNON;

    if(buff[2] != 'P' && buff[2] != 'L' && buff[2] != 'N')    /* $G[PLN] */
        return GPNON;

    if(buff[3] == 'G' && buff[4] == 'G' && buff[5] == 'A')    /* $G[PLN]GGA */
        return GPGGA;
    if(buff[3] == 'G' && buff[4] == 'S' && buff[5] == 'A')    /* $G[PLN]GSA */
        return GPGSA;
    if(buff[3] == 'G' && buff[4] == 'S' && buff[5] == 'V')    /* $G[PLN]GSV */
        return GPGSV;
    if(buff[3] == 'R' && buff[4] == 'M' && buff[5] == 'C')    /* $G[PLN]RMC */
        return GPRMC;
    if(buff[3] == 'V' && buff[4] == 'T' && buff[5] == 'G')    /* $G[PLN]RMC */
        return GPVTG;

    return GPNON;
}

/**
 * \brief Find tail of packet ("\r\n") in buffer and check control sum (CRC).
 * @param buff a constant character pointer of packets buffer.
 * @param buff_sz buffer size.
 * @param res_crc a integer pointer for return CRC of packet (must be defined).
 * @return Number of bytes to packet tail.
 */
int nmea_find_tail(const char *buff, int buff_sz, int *res_crc)
{
    static const int tail_sz = 3 /* *[CRC] */ + 2 /* \r\n */;

    const char *end_buff = buff + buff_sz;
    int nread = 0;
    int crc = 0;

    NMEA_ASSERT(buff && res_crc);

    *res_crc = -1;

    for(;buff < end_buff; ++buff, ++nread)
    {
        if(('$' == *buff) && nread)
        {
            buff = 0;
            break;
        }
        else if('*' == *buff)
        {
            if(buff + tail_sz <= end_buff && '\r' == buff[3] && '\n' == buff[4])
            {
                *res_crc = nmea_atoi(buff + 1, 2, 16);
                nread = buff_sz - (int)(end_buff - (buff + tail_sz));
                if(*res_crc != crc)
                {
                    *res_crc = -1;
                    buff = 0;
                }
            }

            break;
        }
        else if(nread)
            crc ^= (int)*buff;
    }

    if(*res_crc < 0 && buff)
        nread = 0;

    return nread;
}

/**
 * \brief Parse GGA packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int nmea_parse_GPGGA(const char *buff, int buff_sz, nmeaGPGGA *pack)
{
    char time_buff[NMEA_TIMEPARSE_BUF];
    char talker;

    NMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(nmeaGPGGA));

//    nmea_trace_buff(buff, buff_sz);

    if(15 != nmea_scanf(buff, buff_sz,
        "$G%CGGA,%s,%f,%C,%f,%C,%d,%d,%f,%f,%C,%f,%C,%f,%d*",
        &talker,
        &(time_buff[0]),
        &(pack->lat), &(pack->ns), &(pack->lon), &(pack->ew),
        &(pack->sig), &(pack->satinuse), &(pack->HDOP), &(pack->elv), &(pack->elv_units),
        &(pack->diff), &(pack->diff_units), &(pack->dgps_age), &(pack->dgps_sid)) )
        {
// 	      nmea_error("GPGGA parse error!");
    	  return 0;
 	   	}

    if(0 != _nmea_parse_time(&time_buff[0], (int)strlen(&time_buff[0]), &(pack->utc)))
    {
//        nmea_error("GPGGA time parse error!");
        return 0;
    }

    return 1;
}

/**
 * \brief Parse GSA packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int nmea_parse_GPGSA(const char *buff, int buff_sz, nmeaGPGSA *pack)
{
    char talker;

    NMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(nmeaGPGSA));

//    nmea_trace_buff(buff, buff_sz);

    if(18 != nmea_scanf(buff, buff_sz,
        "$G%CGSA,%C,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f*",
        &talker,
        &(pack->fix_mode), &(pack->fix_type),
        &(pack->sat_prn[0]), &(pack->sat_prn[1]), &(pack->sat_prn[2]), &(pack->sat_prn[3]), &(pack->sat_prn[4]), &(pack->sat_prn[5]),
        &(pack->sat_prn[6]), &(pack->sat_prn[7]), &(pack->sat_prn[8]), &(pack->sat_prn[9]), &(pack->sat_prn[10]), &(pack->sat_prn[11]),
        &(pack->PDOP), &(pack->HDOP), &(pack->VDOP)))
    {
        //nmea_error("GPGSA parse error!");
        return 0;
    }

    return 1;
}

/**
 * \brief Parse GSV packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int nmea_parse_GPGSV(const char *buff, int buff_sz, nmeaGPGSV *pack)
{
    int nsen, nsat;

    NMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(nmeaGPGSV));

//    nmea_trace_buff(buff, buff_sz);

    nsen = nmea_scanf(buff, buff_sz,
        "$G%CGSV,%d,%d,%d,"
        "%d,%d,%d,%d,"
        "%d,%d,%d,%d,"
        "%d,%d,%d,%d,"
        "%d,%d,%d,%d*",
        &(pack->talker),
        &(pack->pack_count), &(pack->pack_index), &(pack->sat_count),
        &(pack->id[0]), &(pack->elv[0]), &(pack->azimuth[0]), &(pack->sig[0]),
        &(pack->id[1]), &(pack->elv[1]), &(pack->azimuth[1]), &(pack->sig[1]),
        &(pack->id[2]), &(pack->elv[2]), &(pack->azimuth[2]), &(pack->sig[2]),
        &(pack->id[3]), &(pack->elv[3]), &(pack->azimuth[3]), &(pack->sig[3]));

    nsat = (pack->pack_index - 1) * NMEA_MAXSAT_GSV;
    nsat = (nsat + NMEA_MAXSAT_GSV > pack->sat_count)?pack->sat_count - nsat:NMEA_MAXSAT_GSV;
    nsat = nsat * 4 + 4 /* first three sentence`s */;

    if(	nsen < nsat || nsen > (NMEA_MAXSAT_GSV * 4 + 4) || 
		pack->sat_count > NMEA_MAXSAT || 
		pack->pack_index > NMEA_MAXSAT / NMEA_MAXSAT_GSV || 
		pack->pack_count > NMEA_MAXSAT / NMEA_MAXSAT_GSV)
    {
 //       nmea_error("GPGSV parse error!");
        return 0;
    }

    return 1;
}

/**
 * \brief Parse RMC packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int nmea_parse_GPRMC(const char *buff, int buff_sz, nmeaGPRMC *pack)
{
    int nsen;
    char time_buff[NMEA_TIMEPARSE_BUF];
    char talker;

    NMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(nmeaGPRMC));

//    nmea_trace_buff(buff, buff_sz);

    nsen = nmea_scanf(buff, buff_sz,
        "$G%CRMC,%s,%C,%f,%C,%f,%C,%f,%f,%2d%2d%2d,%f,%C,%C*",
        &talker,
        &(time_buff[0]),
        &(pack->status), &(pack->lat), &(pack->ns), &(pack->lon), &(pack->ew),
        &(pack->speed), &(pack->direction),
        &(pack->utc.day), &(pack->utc.mon), &(pack->utc.year),
        &(pack->declination), &(pack->declin_ew), &(pack->mode));

    if(nsen != 14 && nsen != 15)
    {
  //      nmea_error("GPRMC parse error!");
        return 0;
    }

    if(0 != _nmea_parse_time(&time_buff[0], (int)strlen(&time_buff[0]), &(pack->utc)))
    {
   //     nmea_error("GPRMC time parse error!");
        return 0;
    }

    if(pack->utc.year < 90)
        pack->utc.year += 100;
    pack->utc.mon -= 1;

    return 1;
}

/**
 * \brief Parse VTG packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int nmea_parse_GPVTG(const char *buff, int buff_sz, nmeaGPVTG *pack)
{
    char talker;

    NMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(nmeaGPVTG));

//    nmea_trace_buff(buff, buff_sz);

    if(9 != nmea_scanf(buff, buff_sz,
        "$G%CVTG,%f,%C,%f,%C,%f,%C,%f,%C*",
        &talker,
        &(pack->dir), &(pack->dir_t),
        &(pack->dec), &(pack->dec_m),
        &(pack->spn), &(pack->spn_n),
        &(pack->spk), &(pack->spk_k)))
    {
//        nmea_error("GPVTG parse error!");
        return 0;
    }

    if( pack->dir_t != 'T' ||
        pack->dec_m != 'M' ||
        pack->spn_n != 'N' ||
        pack->spk_k != 'K')
    {
//        nmea_error("GPVTG parse error (format error)!");
        return 0;
    }

    return 1;
}

/**
 * \brief Fill nmeaINFO structure by GGA packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void nmea_GPGGA2info(nmeaGPGGA *pack, nmeaINFO *info)
{
    NMEA_ASSERT(pack && info);

    info->utc.hour = pack->utc.hour;
    info->utc.min = pack->utc.min;
    info->utc.sec = pack->utc.sec;
    info->utc.hsec = pack->utc.hsec;
    info->sig = pack->sig;
    info->HDOP = pack->HDOP;
    info->elv = pack->elv;
    info->lat = ((pack->ns == 'N')?pack->lat:-(pack->lat));
    info->lon = ((pack->ew == 'E')?pack->lon:-(pack->lon));
    info->smask |= GPGGA;
}

/**
 * \brief Fill nmeaINFO structure by GSA packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void nmea_GPGSA2info(nmeaGPGSA *pack, nmeaINFO *info)
{
    int i, j;
    int nuse = 0;

    NMEA_ASSERT(pack && info);

    info->fix = pack->fix_type;
    info->PDOP = pack->PDOP;
    info->HDOP = pack->HDOP;
    info->VDOP = pack->VDOP;


    for(i = 0; i < info->satinfo.inview; ++i)
    {
        for(j = 0; j < NMEA_MAXSAT_GSA; ++j)
        {
             if(pack->sat_prn[j] && pack->sat_prn[j] == info->satinfo.sat[i].id)
                info->satinfo.sat[i].in_use = 1;
        }
        if(info->satinfo.sat[i].in_use)
            nuse++;
    }

    info->satinfo.inuse = nuse;
    info->smask |= GPGSA;
}

/**
 * \brief Fill nmeaINFO structure by GSV packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void nmea_GPGSV2info(nmeaGPGSV *pack, nmeaINFO *info)
{
    int isat, isi, nsat;
    int ntalker = 0;

    NMEA_ASSERT(pack && info);

    if(pack->pack_index > pack->pack_count ||
        pack->pack_index * NMEA_MAXSAT_GSV > NMEA_MAXSAT)
        return;

    if(pack->pack_index < 1)
        pack->pack_index = 1;

    if(pack->talker == NMEA_TALKER_GP)
    {
        info->satinfo.inview_GP = pack->sat_count;
        ntalker = 0;
    }
    if(pack->talker == NMEA_TALKER_GL)
    {
        info->satinfo.inview_GL = pack->sat_count;
        /* Sats offset for GLONASS talker */
        ntalker = info->satinfo.inview_GP;
        /* Check GSV output form (combined or separated) */
        if((pack->pack_count > (info->satinfo.inview_GL + 3) / 4) && (pack->pack_index > (info->satinfo.inview_GP + 3) / 4))
        {
            /* MNP-M7'like combined form:
                $GPGSV,5,1,10,..
                $GPGSV,5,2,10,..
                $GPGSV,5,3,10,..
                $GLGSV,5,4,7,..   -- GLONASS pack starts from index 4 instead of 1
                $GLGSV,5,5,7,..
            */
            pack->pack_index -= (info->satinfo.inview_GP + 3) / 4;
        }
    }
    info->satinfo.inview = info->satinfo.inview_GP + info->satinfo.inview_GL;

    nsat = (pack->pack_index - 1) * NMEA_MAXSAT_GSV;
    nsat = (nsat + NMEA_MAXSAT_GSV > pack->sat_count)?pack->sat_count - nsat:NMEA_MAXSAT_GSV;

    for(isat = 0; isat < nsat; ++isat)
    {
        isi = ntalker + (pack->pack_index - 1) * NMEA_MAXSAT_GSV + isat;
        info->satinfo.sat[isi].id = pack->id[isat];
        info->satinfo.sat[isi].elv = pack->elv[isat];
        info->satinfo.sat[isi].azimuth = pack->azimuth[isat];
        info->satinfo.sat[isi].sig = pack->sig[isat];
    }
    for(isi = 0; isi < info->satinfo.inview; ++isi)
        info->satinfo.sat[isi].in_use = 0;

    info->smask |= GPGSV;
}

/**
 * \brief Fill nmeaINFO structure by RMC packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void nmea_GPRMC2info(nmeaGPRMC *pack, nmeaINFO *info)
{
    NMEA_ASSERT(pack && info);

    if('A' == pack->status)
    {
        if(NMEA_SIG_BAD == info->sig)
            info->sig = NMEA_SIG_MID;
        if(NMEA_FIX_BAD == info->fix)
            info->fix = NMEA_FIX_2D;
    }
    else if('V' == pack->status)
    {
        info->sig = NMEA_SIG_BAD;
        info->fix = NMEA_FIX_BAD;
    }

    info->utc = pack->utc;
    info->lat = ((pack->ns == 'N')?pack->lat:-(pack->lat));
    info->lon = ((pack->ew == 'E')?pack->lon:-(pack->lon));
    info->speed = pack->speed * NMEA_TUD_KNOTS;
    info->direction = pack->direction;
    info->smask |= GPRMC;
}

/**
 * \brief Fill nmeaINFO structure by VTG packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void nmea_GPVTG2info(nmeaGPVTG *pack, nmeaINFO *info)
{
    NMEA_ASSERT(pack && info);

    info->direction = pack->dir;
    info->declination = pack->dec;
    info->speed = pack->spk;
    info->smask |= GPVTG;
}
