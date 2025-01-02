/**
 * @addtogroup UART
 * @brief UART driver.
 *
 * Contains functions to send and receive data over the UART interface.
 *
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    uart.h
 * @brief   UART driver.
 *///---------------------------------------------------------------------------
#ifndef _UART_H_
#define _UART_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void uart_init_blk ( uint32_t baudrate );

void uart_putchar_blk ( char cChar );

char uart_getchar_blk ( void );

void uart_putstring_blk ( const char *uart_string );

void uart_puthex_blk (uint8_t value);

void uart_hexdump_blk ( const uint8_t *array, uint8_t size);

#endif // _UART_H_
/** @} */
