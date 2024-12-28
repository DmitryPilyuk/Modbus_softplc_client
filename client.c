/*
 * Copyright © Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#ifndef _MSC_VER
#include <sys/time.h>
#include <unistd.h>
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <modbus.h>

#define G_MSEC_PER_SEC 1000

static uint32_t gettime_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint32_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

enum
{
    TCP,
    RTU
};

/* Tests based on PI-MBUS-300 documentation */
int main(int argc, char *argv[])
{
    modbus_t *ctx;
    double elapsed;
    uint32_t start;
    uint32_t end;
    int rc;
    int bit_val;
    if (argc > 1)
    {
        if (strcmp(argv[1], "on") == 0)
        {
            bit_val = ON;
            printf("Enable led\n");
        }
        else if (strcmp(argv[1], "off") == 0)
        {
            bit_val = OFF;
            printf("Disable led\n");
        }
        else
        {
            printf("Usage:\n  %s [on|off] \n",
                   argv[0]);
            exit(1);
        }
    }
    else
    {
        printf("Usage:\n  %s [on|off] \n",
               argv[0]);
        exit(1);
    }

    ctx = modbus_new_tcp("10.0.2.16", 502);
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                                  MODBUS_ERROR_RECOVERY_PROTOCOL);

    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    start = gettime_ms();
    rc = modbus_write_bit(ctx, 0, bit_val);
    if (rc == -1)
    {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
        return -1;
    }
    end = gettime_ms();
    elapsed = end - start;
    printf("* %.3f ms\n", elapsed);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}