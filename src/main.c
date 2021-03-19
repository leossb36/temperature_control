#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "crc16.h"


void stop_process(int signal) {
    printf("Programa interrompido!\nFinalizando processos...\n");
    exit(0);
}

int main(int argc, const char * argv[]) {

    signal(SIGINT, stop_process);
    signal(SIGKILL, stop_process);

    int uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }

    struct termios options;

    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;

    p_tx_buffer = &tx_buffer[0];

    *p_tx_buffer++ = 0x01;
    *p_tx_buffer++ = 0x23;
    *p_tx_buffer++ = 0xC1;
    *p_tx_buffer++ = 0x05;
    *p_tx_buffer++ = 0x05;
    *p_tx_buffer++ = 0x02;
    *p_tx_buffer++ = 0x01;

    short crc16 = calcula_CRC(tx_buffer, (p_tx_buffer - &tx_buffer[0]));

    *p_tx_buffer++ = crc16;
    *p_tx_buffer++ = crc16 >> 8;

    printf("Buffers de memória criados!\n");

    if (uart0_filestream != -1)
    {
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);

    //----- CHECK FOR ANY RX BYTES -----
    if (uart0_filestream != -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void*)rx_buffer , 255);      //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0)
        {
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
        }
        else if (rx_length == 0)
        {
            printf("Nenhum dado disponível.\n"); //No data waiting
        }
        else
        {
            //Bytes received
            float temperatura;

            memcpy(&temperatura, rx_buffer + 3, 4);

            rx_buffer[rx_length] = '\0';
            printf("%i Bytes lidos : %f\n", rx_length, temperatura);
        }
    }

    close(uart0_filestream);
   return 0;
}
