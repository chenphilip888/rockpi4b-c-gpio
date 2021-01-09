/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

char *readLine(int fd) {
    static unsigned char line[256];
    int size;
    int i;
    char *ptr = line;
    while(1) {
	size = read(fd, (unsigned char *)ptr, 255);
	while (size > 0) {
	    if (*ptr == '\n') {
		ptr++;
	        *ptr = '\0';
	        return line;
	    }
            ptr++;
	    size--;
	}
    }
}

int uart_test( void ) {
    struct termios tty;
    int k;
    int fd;
 
    fd = open("/dev/ttyS4", O_RDWR | O_NOCTTY );
    tcgetattr(fd, &tty);

    tty.c_cflag = (B19200 | CLOCAL | CREAD | CS8);
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
 
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        fprintf (stderr, "error %d from tcsetattr", errno);
        return -1;
    }

    tcflush(fd, TCIFLUSH);

    for ( k = '0'; k <= '~'; k++ ) {
	char i[3] = "9\r\n";
	i[0] = k;
        write(fd, &i[0], sizeof i);
	char *j = readLine(fd);
        printf("%s", j);
    }
    printf("\n");
    close(fd);
    return 0;
}
