#include "uart.h"
#define CODE 0x23

int uart0_filestream = -1;


int openUartFileStream() {
    struct termios options;

    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        return -1;
    }

    return 1;
}

int sendMessageToUart(int subcode_command) {
    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;

    p_tx_buffer = &tx_buffer[0];

    // get arduino adress
    *p_tx_buffer++ = 0x01;

    // get comunication code
    *p_tx_buffer++ = CODE;
    *p_tx_buffer++ = subcode_command;

    // matricula
    *p_tx_buffer++ = 0x05;
    *p_tx_buffer++ = 0x05;
    *p_tx_buffer++ = 0x02;
    *p_tx_buffer++ = 0x01;

    short crc16 = calcula_CRC(tx_buffer, (p_tx_buffer - &tx_buffer[0]));

    *p_tx_buffer++ = crc16;
    *p_tx_buffer++ = crc16 >> 8;

    if (uart0_filestream != -1)
    {
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if(count < 0) {
            return 1;
        }
        return -1;
    }

    return -1;
}

float readFromUart() {
    if (uart0_filestream != -1)
    {
        unsigned char rx_buffer[256];

        int rx_length = read(uart0_filestream, (void*)rx_buffer , 255);
        if (rx_length < 0)
            return -1;
        else if (rx_length == 0)
            return -1;
        else
        {
            rx_buffer[rx_length] = '\0';
            if (compare_CRC(rx_buffer, rx_length)) {
                float send_command;
                memcpy(&send_command, rx_buffer + 3, 4);
                return send_command;
            }
        }
    }
    return -1;
}

void closeUart() {
    close(uart0_filestream);
}

float get_temperature(int type) {

    float temperaturaFromUart;
    openUartFileStream();
    sendMessageToUart(type);
    usleep(1000000);
    temperaturaFromUart = readFromUart();

    closeUart();

    return temperaturaFromUart;
}