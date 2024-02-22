#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdbool.h>
#include "macro.h"
#include "DIO.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <avr/interrupt.h>//to use bilt in functions in interrupt
#include <stdio.h>
#include <math.h>
#include <float.h>
#define NOTPRESSED 0xff
volatile unsigned char seconds_counter=0;
unsigned char minutes_counter=0,hours_counter=0;
unsigned char value,first_digit,second_digit;
int main(void)
{
	keypad_inti();
	LCD_inti();
	seven_seg_inti('B');
	// intlize them to be output pins << to connect with cmmon cathod of each 7segment
	SET_BIT(DDRC,0);
	SET_BIT(DDRC,1);
	SET_BIT(DDRC,2);
	SET_BIT(DDRC,3);
	SET_BIT(DDRC,4);
	SET_BIT(DDRC,5);
	LCD_send_string("press 1 to");
	LCD_move_cursore(2,1);
	LCD_send_string("set clock");
	timer2_overflow_init_interrupt();
	
	
	while(1)
	{
		value=keypad_check_press();
		if (value!=NOTPRESSED) //mean i pressed on keypad
		{
			if (value=='1')
			{
				
				LCD_clear_screen();
				LCD_send_string("hours= __");
				LCD_move_cursore(1,8);
				_delay_ms(500);
				do
				{
					first_digit=keypad_check_press();
					
				} while (first_digit==NOTPRESSED);
				LCD_send_data(first_digit);
				_delay_ms(300);
				do
				{
					second_digit=keypad_check_press();
					
				} while (second_digit==NOTPRESSED);
				LCD_send_data(second_digit);
				_delay_ms(300);
				hours_counter=(second_digit-48)+10*(first_digit-48); //count number which user enterd
				LCD_clear_screen();
				
				LCD_send_string("minutes= __");
				LCD_move_cursore(1,10);
				do
				{
					first_digit=keypad_check_press();
					
				} while (first_digit==NOTPRESSED);
				LCD_send_data(first_digit);
				_delay_ms(300);
				do
				{
					second_digit=keypad_check_press();
					
				} while (second_digit==NOTPRESSED);
				LCD_send_data(second_digit);
				_delay_ms(300);
				minutes_counter=(second_digit-48)+10*(first_digit-48);
				LCD_clear_screen();
				
				LCD_send_string("seconds= __");
				LCD_move_cursore(1,10);
				
				do
				{
					first_digit=keypad_check_press();
					
				} while (first_digit==NOTPRESSED);
				LCD_send_data(first_digit);
				_delay_ms(300);
				do
				{
					second_digit=keypad_check_press();
					
				} while (second_digit==NOTPRESSED);
				LCD_send_data(second_digit);
				_delay_ms(300);
				seconds_counter=(second_digit-48)+10*(first_digit-48);
				LCD_clear_screen();
				LCD_send_string("press 1 to");
				LCD_move_cursore(2,1);
				LCD_send_string("set clock");
			}
			else // number which enterd by user is not 1
			{
				LCD_clear_screen();
				LCD_send_string("wrong choice");
				_delay_ms(1000);
				LCD_clear_screen();
				LCD_send_string("press 1 to");
				LCD_move_cursore(2,1);
				LCD_send_string("set clock");
			}
		}
		//code which work defult if user not press any button on keypad
		
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		CLEAR_BIT(PORTC,0);
		seven_seg_write_commka('B',seconds_counter%10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		CLEAR_BIT(PORTC,1);
		seven_seg_write_commka('B',seconds_counter/10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		CLEAR_BIT(PORTC,2);
		seven_seg_write_commka('B',minutes_counter%10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		CLEAR_BIT(PORTC,3);
		seven_seg_write_commka('B',minutes_counter/10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,5);
		CLEAR_BIT(PORTC,4);
		seven_seg_write_commka('B',hours_counter%10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,4);
		CLEAR_BIT(PORTC,5);
		seven_seg_write_commka('B',hours_counter/10);
		_delay_ms(5);
		
		if (seconds_counter==60)
		{
			seconds_counter=0;
			minutes_counter++;
		}
		if (minutes_counter==60)
		{
			minutes_counter=0;
			hours_counter++;
		}
		if (hours_counter==24)
		{
			hours_counter=0;
		}
	}
	
}

ISR(TIMER2_OVF_vect)
{
	seconds_counter++;
	
}

