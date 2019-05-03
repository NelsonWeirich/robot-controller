#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BAUDRATE B9600
#define PORT "/dev/ttyACM0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

FILE *file;
int fileLen;
char *tmpbuffer;
void open_port(void);
void read_port(void);
void send_port(void);
int fd=0;
struct termios oldtp, newtp;
//char sendcmd1[256]="\0";
char buffer[512];

void main () {
	open_port();
	sleep(1);
//	read_port();
    while(1)
	send_port();
}

void send_port(void) {
    printf("enter write\n");
    int n;
 // //   sem_wait(&len);
 //    file = fopen("sample.txt", "r");
 //
 //    //get file size
 //
 //    fseek(file, 0, SEEK_END);
 //    fileLen = ftell(file);
 //    fseek(file, 0, SEEK_SET);
 //
 //    tmpbuffer = (char *)malloc(fileLen + 1);
 //
 //    //read file contents
 //    printf("Start send\n");
 //    fread(tmpbuffer, fileLen, 1, file);
 //    fclose(file);

    n = write(fd, "opop\r"/*tmpbuffer*/, fileLen + 1);
    if (n < 0) {
            fputs("write() of bytes failed!\n", stderr);
    }
    else {
            printf("String sent successfully %d\n", n);
    }
    close(fd);
}

void open_port(void) {
    fd = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY );
	printf("Enviando para %d\n",fd);
    if (fd < 0) {
        perror(PORT);
    }
    else
        fcntl(fd, F_SETFL,0);

    tcgetattr(fd, &oldtp); /* save current serial port settings */
    bzero(&newtp, sizeof(newtp));

    newtp.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

    newtp.c_iflag = IGNPAR | ICRNL;

    newtp.c_oflag = 0;

    newtp.c_lflag = ICANON;

    newtp.c_cc[VINTR]    = 0;     /* Ctrl-c */
    newtp.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    newtp.c_cc[VERASE]   = 0;     /* del */
    newtp.c_cc[VKILL]    = 0;     /* @ */
    //newtp.c_cc[VEOF]     = 4;     /* Ctrl-d */
    newtp.c_cc[VEOF]     = 0;     /* Ctrl-d */
    newtp.c_cc[VTIME]    = 0;     /* inter-character timer unused */
    newtp.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
    newtp.c_cc[VSWTC]    = 0;     /* '\0' */
    newtp.c_cc[VSTART]   = 0;     /* Ctrl-q */
    newtp.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    newtp.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    newtp.c_cc[VEOL]     = 0;     /* '\0' */
    newtp.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    newtp.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    newtp.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    newtp.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    newtp.c_cc[VEOL2]    = 0;     /* '\0' */
}
