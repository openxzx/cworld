/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2019 Semtech

Description:
    Host specific functions to address the LoRa concentrator registers through
    a SPI interface.
    Single-byte read/write and burst read/write.
    Could be used with multiple SPI ports in parallel (explicit file descriptor)

License: Revised BSD License, see LICENSE.TXT file include in the project
*/


/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdint.h>     /* C99 types */
#include <stdio.h>      /* printf fprintf */
#include <stdlib.h>     /* malloc free */
#include <unistd.h>     /* lseek, close */
#include <fcntl.h>      /* open */
#include <string.h>     /* memset */

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "mpsse.h"
#include "loragw_sx1302_spi.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#if DEBUG_SPI == 1
    #define DEBUG_MSG(str)                fprintf(stderr, str)
    #define DEBUG_PRINTF(fmt, args...)    fprintf(stderr,"%s:%d: "fmt, __FUNCTION__, __LINE__, args)
    #define CHECK_NULL(a)                if(a==NULL){fprintf(stderr,"%s:%d: ERROR: NULL POINTER AS ARGUMENT\n", __FUNCTION__, __LINE__);return LGW_SPI_ERROR;}
#else
    #define DEBUG_MSG(str)
    #define DEBUG_PRINTF(fmt, args...)
    #define CHECK_NULL(a)                if(a==NULL){return LGW_SPI_ERROR;}
#endif

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define READ_ACCESS     0x00
#define WRITE_ACCESS    0x80

/* parameters for a FT2232H */
#define VID		0x0403
#define PID		0x6010

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */

/* SPI initialization and configuration */
int lgw_sx1302_spi_open(const char * spidev_path, void **spi_target_ptr) {
    struct mpsse_context *mpsse = NULL;
    int a=0, b=0;

    CHECK_NULL(spidev_path);

    /* check input variables */
    CHECK_NULL(spi_target_ptr); /* cannot be null, must point on a void pointer (*spi_target_ptr can be null) */

    /* try to open the first available FTDI device matching VID/PID parameters */
    mpsse = OpenIndex(VID, PID, SPI0, SIX_MHZ, MSB, IFACE_A, NULL, NULL, 0);
    if (mpsse == NULL) {
        DEBUG_MSG("ERROR: MPSSE OPEN FUNCTION RETURNED NULL\n");
        return LGW_SPI_ERROR;
    }
    if (mpsse->open != 1) {
        DEBUG_MSG("ERROR: MPSSE OPEN FUNCTION FAILED\n");
        return LGW_SPI_ERROR;
    }

    /* toggle pin ADBUS5 of the FT2232H */
    /* On the Semtech reference board, it resets the SX1301 */
    a = PinHigh(mpsse, GPIOL1);
    b = PinLow(mpsse, GPIOL1);
    if ((a != MPSSE_OK) || (b != MPSSE_OK)) {
        DEBUG_MSG("ERROR: IMPOSSIBLE TO TOGGLE GPIOL1/ADBUS5\n");
        return LGW_SPI_ERROR;
    }

    DEBUG_PRINTF("SPI port opened and configured ok\ndesc: %s\nPID: 0x%04X\nVID: 0x%04X\nclock: %d\nLibmpsse version: 0x%02X\n", \
        GetDescription(mpsse), GetPid(mpsse), GetVid(mpsse), GetClock(mpsse), Version());

    *spi_target_ptr = (void *)mpsse;

    DEBUG_MSG("Note: SPI port opened and configured ok\n");
    return LGW_SPI_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* SPI release */
int lgw_sx1302_spi_close(void *spi_target) {
    struct mpsse_context *mpsse = spi_target;

    /* check input variables */
    CHECK_NULL(spi_target);

    Close(mpsse);

    /* close return no status, assume success (0_o) */
    return LGW_SPI_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Simple write */
int lgw_sx1302_spi_w(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t data) {
    struct mpsse_context *mpsse = spi_target;
    uint8_t out_buf[4];
    uint8_t command_size;
    int a, b, c;

    /* check input variables */
    CHECK_NULL(spi_target);

    /* prepare frame to be sent */
    out_buf[0] = spi_mux_target;
    out_buf[1] = WRITE_ACCESS | ((address >> 8) & 0x7F);
    out_buf[2] =                ((address >> 0) & 0xFF);
    out_buf[3] = data;
    command_size = 4;

    /* MPSSE transaction */
    a = Start(mpsse);
    b = FastWrite(mpsse, (char *)out_buf, command_size);
    c = Stop(mpsse);

    /* determine return code */
    if ((a != MPSSE_OK) || (b != MPSSE_OK) || (c != MPSSE_OK)) {
        DEBUG_MSG("ERROR: SPI WRITE FAILURE\n");
        return LGW_SPI_ERROR;
    } else {
        DEBUG_MSG("Note: SPI write success\n");
        return LGW_SPI_SUCCESS;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Simple read */
int lgw_sx1302_spi_r(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t *data) {
    struct mpsse_context *mpsse = spi_target;
    uint8_t out_buf[5] = {0};
    uint8_t *in_buf = NULL;
    uint8_t command_size;
    int a, b;

    /* check input variables */
    CHECK_NULL(spi_target);
    CHECK_NULL(data);

    /* prepare frame to be sent */
    out_buf[0] = spi_mux_target;
    out_buf[1] = READ_ACCESS | ((address >> 8) & 0x7F);
    out_buf[2] =               ((address >> 0) & 0xFF);
    command_size = 5;

    /* MPSSE transaction */
    a = Start(mpsse);
    in_buf = (uint8_t *)Transfer(mpsse, (char *)out_buf, command_size);
    b = Stop(mpsse);

    /* determine return code */
    if ((in_buf == NULL) || (a != MPSSE_OK) || (b != MPSSE_OK)) {
        DEBUG_MSG("ERROR: SPI READ FAILURE\n");
        return LGW_SPI_ERROR;
    } else {
        DEBUG_MSG("Note: SPI read success\n");
        DEBUG_PRINTF("Note: SPI read success: %02X\n", in_buf[command_size - 1]);
        *data = in_buf[command_size - 1];
		free(in_buf);
        return LGW_SPI_SUCCESS;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Burst (multiple-byte) write */
int lgw_sx1302_spi_wb(void *spi_target, uint8_t spi_mux_target, uint16_t address, const uint8_t *data, uint16_t size) {
    struct mpsse_context *mpsse = spi_target;
    uint8_t command[3];
    uint8_t command_size;
    uint8_t *out_buf = NULL;
    int size_to_do, buf_size, chunk_size, offset;
    int a, b, c;
    int i;

    /* check input parameters */
    CHECK_NULL(spi_target);
    CHECK_NULL(data);
    if (size == 0) {
        DEBUG_MSG("ERROR: BURST OF NULL LENGTH\n");
        return LGW_SPI_ERROR;
    }

    /* prepare command byte */
    command[0] = spi_mux_target;
    command[1] = WRITE_ACCESS | ((address >> 8) & 0x7F);
    command[2] =                ((address >> 0) & 0xFF);
    command_size = 3;
    size_to_do = size + command_size;

    /* allocate data buffer */
    buf_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
    out_buf = malloc(buf_size);
    if (out_buf == NULL) {
        DEBUG_MSG("ERROR: MALLOC FAIL\n");
        return LGW_SPI_ERROR;
    }

    /* start MPSSE transaction */
    a = Start(mpsse);
    for (i = 0; size_to_do > 0; ++i) {
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;

        if (i == 0) {
            /* first chunk, need to append the address */
            out_buf[0] = command[0];
            out_buf[1] = command[1];
            out_buf[2] = command[2];

            memcpy(out_buf + command_size, data, chunk_size - command_size);
        } else {
            /* following chunks, just copy the data */
            offset = (i * LGW_BURST_CHUNK) - command_size;
            memcpy(out_buf, data + offset, chunk_size);
        }
        b = FastWrite(mpsse, (char *)out_buf, chunk_size);
        size_to_do -= chunk_size; /* subtract the quantity of data already transferred */
    }
    c = Stop(mpsse);

    /* deallocate data buffer */
    free(out_buf);

    /* determine return code (only the last FastWrite is checked) */
    if ((a != MPSSE_OK) || (b != MPSSE_OK) || (c != MPSSE_OK)) {
        DEBUG_MSG("ERROR: SPI BURST WRITE FAILURE\n");
        return LGW_SPI_ERROR;
    } else {
        DEBUG_MSG("Note: SPI burst write success\n");
        return LGW_SPI_SUCCESS;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Burst (multiple-byte) read */
int lgw_sx1302_spi_rb(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t *data, uint16_t size) {
    struct mpsse_context *mpsse = spi_target;
    uint8_t chunk_buf[LGW_BURST_CHUNK + 1];
    uint8_t command[3];
    uint8_t command_size;
    int size_to_do, chunk_size, offset;
    int a = 0, b = 0, c = 0, d = 0;
    int i;

    /* check input parameters */
    CHECK_NULL(spi_target);
    CHECK_NULL(data);
    if (size == 0) {
        DEBUG_MSG("ERROR: BURST OF NULL LENGTH\n");
        return LGW_SPI_ERROR;
    }

    /* prepare command byte */
    command[0] = spi_mux_target;
    command[1] = READ_ACCESS | ((address >> 8) & 0x7F);
    command[2] =               ((address >> 0) & 0xFF);
    command_size = 3;
    size_to_do = size;

    /* start MPSSE transaction */
    a = Start(mpsse);
    b = FastWrite(mpsse, (char *)command, command_size);
    for (i = 0; size_to_do > 0; ++i) {
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
        offset = i * LGW_BURST_CHUNK;
        c = FastRead(mpsse, (char *)(chunk_buf), chunk_size + 1);
        size_to_do -= chunk_size; /* subtract the quantity of data already transferred */
        memcpy(data + offset, chunk_buf + 1, chunk_size);
    }
    d = Stop(mpsse);

    /* determine return code (only the last FastRead is checked) */
    if ((a != MPSSE_OK) || (b != MPSSE_OK) || (c != MPSSE_OK) || (d != MPSSE_OK)) {
        DEBUG_MSG("ERROR: SPI BURST READ FAILURE\n");
        return LGW_SPI_ERROR;
    } else {
        DEBUG_MSG("Note: SPI burst read success\n");
        return LGW_SPI_SUCCESS;
    }
}

/* --- EOF ------------------------------------------------------------------ */
