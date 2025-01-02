/**
 * @file joystick.h
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>

// --- Definitions -------------------------------------------------------------

#define JOY1_UP_PIN     PINB
#define JOY1_UP_BIT     PB1
#define JOY1_DOWN_PIN   PINB
#define JOY1_DOWN_BIT   PB3
#define JOY1_LEFT_PIN   PINB
#define JOY1_LEFT_BIT   PB4
#define JOY1_RIGHT_PIN  PINB
#define JOY1_RIGHT_BIT  PB5
#define JOY1_FIRE_PIN   PINB
#define JOY1_FIRE_BIT   PB2

#define JOY1_PORTB_MASK ((1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5))
#define JOY1_PORTD_MASK 0

#define JOY2_UP_PIN     PIND
#define JOY2_UP_BIT     PD3
#define JOY2_DOWN_PIN   PIND
#define JOY2_DOWN_BIT   PD6
#define JOY2_LEFT_PIN   PIND
#define JOY2_LEFT_BIT   PD7
#define JOY2_RIGHT_PIN  PINB
#define JOY2_RIGHT_BIT  PB0
#define JOY2_FIRE_PIN   PIND
#define JOY2_FIRE_BIT   PD5

#define JOY2_PORTB_MASK (1<<PB0)
#define JOY2_PORTD_MASK ((1<<PD3) | (1<<PD5) | (1<<PD6) | (1<<PD7))

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void joy_init           (void);
void joy_update         (void);
bool joy_event          (uint8_t port);

bool joy_1_is_up        (void);
bool joy_1_is_down      (void);
bool joy_1_is_left      (void);
bool joy_1_is_right     (void);
bool joy_1_is_fireing   (void);

bool joy_2_is_up        (void);
bool joy_2_is_down      (void);
bool joy_2_is_left      (void);
bool joy_2_is_right     (void);
bool joy_2_is_fireing   (void);

#endif /* JOYSTICK_H_ */
