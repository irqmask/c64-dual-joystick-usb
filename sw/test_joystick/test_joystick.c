/*
 * c64_joystick_usb.c
 *
 * Created: 23.05.2015 14:36:28
 *  Author: christian
 */ 


#include <avr/io.h>
#include <util/delay.h>

#include "joystick.h"
#include "uart.h"

int main (void)
{
    joy_init();
    uart_init_blk(9600);
    
    uart_putstring_blk("\nJoystick Test V0.1\n");
    uart_putstring_blk("------------------\n");
    
    while(1) {
        joy_update();
        
        if (joy_event(0)) {
            uart_putstring_blk("1: "); 
            if (joy_1_is_up()) uart_putstring_blk("up ");
            else  uart_putstring_blk("   ");
            if (joy_1_is_down()) uart_putstring_blk("down ");
            else  uart_putstring_blk("     ");
            if (joy_1_is_left()) uart_putstring_blk("left ");
            else  uart_putstring_blk("     ");
            if (joy_1_is_right()) uart_putstring_blk("right ");
            else  uart_putstring_blk("      ");
            if (joy_1_is_fireing()) uart_putstring_blk("fire ");
            else  uart_putstring_blk("     ");
            
            uart_putstring_blk(" - 2: ");
            if (joy_2_is_up()) uart_putstring_blk("up ");
            else  uart_putstring_blk("   ");
            if (joy_2_is_down()) uart_putstring_blk("down ");
            else  uart_putstring_blk("     ");
            if (joy_2_is_left()) uart_putstring_blk("left ");
            else  uart_putstring_blk("     ");
            if (joy_2_is_right()) uart_putstring_blk("right ");
            else  uart_putstring_blk("      ");
            if (joy_2_is_fireing()) uart_putstring_blk("fire ");
            else  uart_putstring_blk("     ");
            uart_putstring_blk("\n");
        }
        _delay_ms(1);        
    }
}