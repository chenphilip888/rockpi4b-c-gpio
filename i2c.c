/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

void wiringPiI2CWriteReg8( int fd, int addr, int reg, int data )
{
    unsigned char buf[2] = { reg, data };

    ioctl( fd, I2C_SLAVE, addr );
    write( fd, buf, 2 );
}

void set_backlight( int fd, int r, int g, int b )
{
    wiringPiI2CWriteReg8( fd, 0x62, 0, 0 );
    wiringPiI2CWriteReg8( fd, 0x62, 1, 0 );
    wiringPiI2CWriteReg8( fd, 0x62, 8, 0xaa );
    wiringPiI2CWriteReg8( fd, 0x62, 4, r );
    wiringPiI2CWriteReg8( fd, 0x62, 3, g );
    wiringPiI2CWriteReg8( fd, 0x62, 2, b );
}

void textCommand(int fd, int cmd)
{
    wiringPiI2CWriteReg8( fd, 0x3e, 0x80, cmd );
}

void setText(int fd, char * text)
{
    int i;

    textCommand( fd, 0x01 );        // clear display
    usleep( 5000);
    textCommand( fd, 0x08 | 0x04 ); // display on, no cursor
    textCommand( fd, 0x28 );        // 2 lines
    usleep( 5000 );
    for (i=0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            textCommand( fd, 0xc0 );
	} else {
            wiringPiI2CWriteReg8( fd, 0x3e, 0x40, text[i] );
	}
    }
}

int i2c_lcd_test ( void )
{
    int i;
    int fd;
    printf ("Rockpi4b i2c test\n") ;
  
    // 0x62 is rgb, 0x3e is text
    fd = open( "/dev/i2c-7", O_RDWR );
  
    textCommand( fd, 0x01 );        // clear display
    usleep( 5000 );
    textCommand( fd, 0x08 | 0x04 ); // display on, no cursor
    textCommand( fd, 0x28 );        // 2 lines
    usleep( 5000 );
  
    set_backlight( fd, 255, 0, 0 );
    for ( i=0; i<5; i++ )
    {
      set_backlight( fd, 255, 0, 0 );
      sleep( 1 );
      set_backlight( fd, 255, 255, 0 );
      sleep( 1 ); 
      set_backlight( fd, 0, 255, 0 );
      sleep( 1 ); 
      set_backlight( fd, 0, 255, 255 );
      sleep( 1 ); 
      set_backlight( fd, 0, 0, 255 );
      sleep( 1 ); 
      set_backlight( fd, 255, 0, 255 );
      sleep( 1 ); 
    }
    set_backlight( fd, 128, 255, 0 );
    setText( fd, "Hello world !\nIt works !\n" );

    close( fd );
    return 0;
}
