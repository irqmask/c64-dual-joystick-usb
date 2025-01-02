/**
 * @addtogroup UART
 * @brief UART driver.
 *
 * @{
 * @file    UART.c
 * @brief   UART driver.
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

// --- Definitions -------------------------------------------------------------

//! Calculation of UART baudrate
#define UART_UBRR_CALC(BAUD_,FREQ_) ((FREQ_)/((BAUD_)*16L)-1)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static char val2hex (uint8_t value)
{
    if (value <= 9) return '0' + value;
    if (value <= 15) return 'A'- 0x0A + value;
    return '-';
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize UART 8N1 without interrupts.
 *
 * @param[in] uBaud
 * Baudrate to configure.
 */
void uart_init_blk ( uint32_t baudrate )
{
    // 9600 Baud 8N1
    UCSRB = (1 << TXEN) | (1 << RXEN);  // UART TX und RX einschalten
    UCSRC = (1<<URSEL) | (1 << UCSZ1) | (1 << UCSZ0); // Asynchron 8N1

    UBRRH = (uint8_t)(UART_UBRR_CALC( baudrate, F_CPU ) >> 8);
    UBRRL = (uint8_t)UART_UBRR_CALC( baudrate, F_CPU );
}

/**
 * Send a char over the UART. Before sending a char, this function waits until
 * the UART is ready to send the next char.
 *
 * @param[in] cChar
 * Character to send.
 */
void uart_putchar_blk ( char cChar )
{
    if ( cChar == '\n' ) uart_putchar_blk('\r');

    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = cChar;
}

/**
 * If there is a received char in the UART, whis function will return it. If no
 * char has been received the null-character is received.
 *
 * @returns Received char, otherwise zero.
 */
char uart_getchar_blk ( void )
{
    // is char available?
    if ( !(UCSRA & (1 << RXC)) ) {
        // return NUL character if nothing has been received
        return '\0';
    }
    // return received char
    return UDR;
}

/**
 * Sends a string over the UART. Before sending a char, this function waits until
 * the UART is ready to send the next char.
 *
 * @param[in] pcString
 * String to send. String has to be terminated with a null character.
 */
void uart_putstring_blk ( const char *uart_string )
{
    while ( *uart_string != '\0' ) {
        uart_putchar_blk(*uart_string++);
    }
}

/**
 * Sends a hexadecimal representation of a byte over the UART. Before each
 * character is sent, this function waits until the UART is ready to send
 * the next char.
 *
 * @param[in] uValue
 * Byte-value to convert into hex and to be sent.
 */
void uart_puthex_blk (uint8_t value)
{
    uart_putchar_blk(val2hex(value >> 4));
    uart_putchar_blk(val2hex(value & 0x0F));
}

/**
 * Sends a hexadecimal dump of a memory region queued over the UART.
 * Before each character is sent, this function waits until the UART is ready
 * to send the next char.
 *
 * @param[in] puArray
 * Byte-pointer to memory region.
 * @param[in] uSize
 * Size in bytes of the memory region.
 */
void uart_hexdump_blk ( const uint8_t *array, uint8_t size)
{
    uint8_t ii;

    for (ii=0; ii<size; ii++) {
        uart_puthex_blk(array[ii]);
        if (ii>1 && (ii+1)%16==0) {
            uart_putchar_blk('\n');
        } else {
            uart_putchar_blk(' ');
        }
    }
    if (ii%16) uart_putchar_blk('\n');
}

/** @} */
