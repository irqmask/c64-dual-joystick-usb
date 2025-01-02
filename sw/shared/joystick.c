/**
 * @file joystick.c
 */

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

#include "joystick.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t g_portB, g_old_portB;
static uint8_t g_portD, g_old_portD;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void joy_init (void)
{
    // set direction to input
    DDRB &= ~(JOY1_PORTB_MASK | JOY2_PORTB_MASK);
    DDRD &= ~(JOY1_PORTD_MASK | JOY2_PORTD_MASK);

    // activate pull-up resistors
    PORTB |= (JOY1_PORTB_MASK | JOY2_PORTB_MASK);
    PORTD |= (JOY1_PORTD_MASK | JOY2_PORTD_MASK);
    g_portB = g_old_portB = PINB & (JOY1_PORTB_MASK | JOY2_PORTB_MASK);
    g_portD = g_old_portD = PIND & (JOY1_PORTD_MASK | JOY2_PORTD_MASK);
}

void joy_update (void)
{
    g_old_portB = g_portB;
    g_old_portD = g_portD;
    g_portB = PINB & (JOY1_PORTB_MASK | JOY2_PORTB_MASK);
    g_portD = PIND & (JOY1_PORTD_MASK | JOY2_PORTD_MASK);
}

bool joy_event (uint8_t port)
{
    bool retval = false;
    
    if (port == 1) {
        retval =  ((g_portB ^ g_old_portB) & JOY1_PORTB_MASK) | 
                  ((g_portD ^ g_old_portD) & JOY1_PORTD_MASK);
    } else if (port == 2) {
        retval =  ((g_portB ^ g_old_portB) & JOY2_PORTB_MASK) | 
                  ((g_portD ^ g_old_portD) & JOY2_PORTD_MASK);
    } else {
        retval = ((g_portB != g_old_portB) || (g_portD != g_old_portD));
    }

    return retval;
}

bool joy_1_is_up (void)
{
    return !((JOY1_UP_PIN & (1<<JOY1_UP_BIT)) != 0);
}

bool joy_1_is_down (void)
{
    return !((JOY1_DOWN_PIN & (1<<JOY1_DOWN_BIT)) != 0);
}

bool joy_1_is_left (void)
{
    return !((JOY1_LEFT_PIN & (1<<JOY1_LEFT_BIT)) != 0);
}

bool joy_1_is_right (void)
{
    return !((JOY1_RIGHT_PIN & (1<<JOY1_RIGHT_BIT)) != 0);
}

bool joy_1_is_fireing (void)
{
    return !((JOY1_FIRE_PIN & (1<<JOY1_FIRE_BIT)) != 0);
}


bool joy_2_is_up (void)
{
    return !((JOY2_UP_PIN & (1<<JOY2_UP_BIT)) != 0);
}

bool joy_2_is_down (void)
{
    return !((JOY2_DOWN_PIN & (1<<JOY2_DOWN_BIT)) != 0);
}

bool joy_2_is_left (void)
{
    return !((JOY2_LEFT_PIN & (1<<JOY2_LEFT_BIT)) != 0);
}

bool joy_2_is_right (void)
{
    return !((JOY2_RIGHT_PIN & (1<<JOY2_RIGHT_BIT)) != 0);
}

bool joy_2_is_fireing (void)
{
    return !((JOY2_FIRE_PIN & (1<<JOY2_FIRE_BIT)) != 0);
}

