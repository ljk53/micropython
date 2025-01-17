/**
*
 * ESP-Drone Firmware
 *
 * Copyright 2019-2020  Espressif Systems (Shanghai)
 * Copyright (c) 2012 Bitcraze AB
 *
 * eprintf.c: Memory-friendly ultra-limited embedded implementation of printf
 */

#include <stdarg.h>

#ifndef	__EPRINTF_H__
#define __EPRINTF_H__

/**
 * putc function pointer definition
 */
typedef int (*putc_t)(int c);

/**
 * Light printf implementation
 * @param[in] putcf Putchar function to be used by Printf
 * @param[in] fmt Format string
 * @param[in] ... Parameters to print
 * @return the number of character printed
 */
int eprintf(putc_t putcf, char * fmt, ...)
    __attribute__ (( format(printf, 2, 3) ));

/**
 * Light printf implementation
 * @param[in] putcf Putchar function to be used by Printf
 * @param[in] fmt Format string
 * @param[in] ap Parameters to print
 * @return the number of character printed
 */
int evprintf(putc_t putcf, char * fmt, va_list ap);

#endif //__EPRINTF_H__
