/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void pwm_open( void )
{
    FILE *fp;
    bool isdir;
    char path[100] = "";
    strcpy( path, "/sys/class/pwm/pwmchip0/pwm0" );
    struct stat st = {0};
    if ( !stat(path, &st) ) {
        isdir = S_ISDIR( st.st_mode );
    }
    if ( !isdir ) {
        fp = fopen( "/sys/class/pwm/pwmchip0/export", "w" );
        fprintf( fp, "%d", 0 );
        fclose( fp );
    }
}

void pwm_polarity( void )
{
    FILE *fp;
    fp = fopen( "/sys/class/pwm/pwmchip0/pwm0/polarity", "w" );
    fputs( "normal", fp );
    fclose( fp );
}

void pwm_enable( void )
{
    FILE *fp;
    fp = fopen( "/sys/class/pwm/pwmchip0/pwm0/enable", "w" ); 
    fprintf( fp, "%d", 1 );
    fclose( fp );
}

void pwm_stop( void )
{
    FILE *fp;
    fp = fopen( "/sys/class/pwm/pwmchip0/pwm0/enable", "w" );
    fprintf( fp, "%d", 0 );
    fclose( fp );
}

void pwm_close( void )
{
    FILE *fp;
    fp = fopen( "/sys/class/pwm/pwmchip0/unexport", "w" );
    fprintf( fp, "%d", 0 );
    fclose( fp );
}

int pwm_freq( int freq )
{
    FILE *fp;
    int pwm_period;
    pwm_period = 1000000000.0 / freq;
    fp = fopen( "/sys/class/pwm/pwmchip0/pwm0/period", "w" ); 
    fprintf( fp, "%d", pwm_period );
    fclose( fp );
    return ( pwm_period );
}

void pwm_duty( float duty, int pwm_period )
{
    FILE *fp;
    int dutycycle;
    dutycycle = duty * pwm_period; 
    fp = fopen( "/sys/class/pwm/pwmchip0/pwm0/duty_cycle", "w" );
    fprintf( fp, "%d", dutycycle );
    fclose( fp );
}

int pwm_led_test( void )
{
    int i;
    int x;
    int pwm_period;
    pwm_open();
    pwm_period = pwm_freq( 60 );
    pwm_duty( 0, pwm_period );
    pwm_polarity();
    pwm_enable();

    for ( i = 0; i < 10; i++ ) {
        for ( x = 0; x <= 100; x = x + 5 ) {
            pwm_duty( x/100.0, pwm_period );
            usleep( 60000 );
        }
        for ( x = 100; x >= 0; x = x - 5 ) {
            pwm_duty( x/100.0, pwm_period );
            usleep( 60000 );
        }
    }

    pwm_stop();
    pwm_close();
    return 0;
}

void tong( int note, int duration )
{
    int pwm_period;
    int extra;
    extra = duration / 5;
    if ( note == 0 ) {
        usleep( duration );
        usleep( extra ); 
    } else {
        pwm_period = pwm_freq( note );
        pwm_duty( 0.5, pwm_period );
        pwm_polarity();
        pwm_enable();
        usleep( duration );
        pwm_stop();
        usleep( extra );
    }
}

int tongsong( void )
{
    int i;
    static int melody[8] = {262, 196, 196, 220, 196, 0, 247, 262};
    static int noteDurations[8] = {4, 8, 8, 4, 4, 4, 4, 4};
    int noteDuration;

    pwm_open();

    for ( i = 0; i < 8; i++ ) {
        noteDuration = 1500000 / noteDurations[i];
        tong( melody[i], noteDuration );
    }

    pwm_close();
    return 0;
}

int servo( void )
{
    int i;
    int pwm_period;

    pwm_open();
    pwm_period = pwm_freq( 50 );
    pwm_duty( 0.05, pwm_period );
    pwm_polarity();
    pwm_enable();

    for ( i = 0; i < 3; i++ ) {
        pwm_duty( 0.05, pwm_period );
        printf( "0 degree\n" );
        sleep( 1 );
        pwm_duty( 0.1, pwm_period );
        printf( "90 degree\n" );
        sleep( 1 );
        pwm_duty( 0.15, pwm_period );
        printf( "180 degree\n" );
        sleep( 1 );
        pwm_duty( 0.1, pwm_period );
        printf( "90 degree\n" );
        sleep( 1 );
    }

    pwm_stop();
    pwm_close();
    return 0;
}
