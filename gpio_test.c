/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <stdio.h>
#include <stdlib.h>
int uart_test( void );
int led_test( void );
int button_test( void );
int pwm_led_test( void );
int tongsong( void );
int servo( void );
int i2c_lcd_test ( void );
int ssd1306_test( void );

char test_menu( void )
{
    char ch;

    printf( "--- select a test ---\r\n" );
    printf( "1. uart test\r\n2. led test\r\n3. button test\r\n4. pwm led test\r\n5. i2c lcd test\r\n6. tongsong\r\n7. servo\r\n8. spi oled test\r\nq. quit\r\n");
    ch = getchar();
    getchar();
    if ( ch == 'q' )
        printf( "Goodbye !\r\n" );
    return ch;
}

int main() {
    char ch;
    char test_item;

    test_item = '\0';
    while( test_item != 'q' ) {
        test_item = test_menu();
	if ( test_item == '1' ) {
	    uart_test();
	} else if ( test_item == '2' ) {
            led_test();
        } else if ( test_item == '3' ) {
            button_test();
        } else if ( test_item == '4' ) {
            pwm_led_test();
        } else if ( test_item == '5' ) {
            i2c_lcd_test();
            printf( "press x to exit i2c lcd test\r\n" );
            ch = getchar();
	    getchar();
        } else if ( test_item == '6' ) {
            tongsong();
        } else if ( test_item == '7' ) {
            servo();
        } else if ( test_item == '8' ) {
            ssd1306_test();
        }
    }

    return 0;
}
