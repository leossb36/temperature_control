#ifndef CRC16_H_
#define CRC16_H_

#include <stdint.h>

typedef struct cell {
    short crc;
    uint8_t bytes[2];
} bytesCRC;

short CRC16(short crc, char data);
short calcula_CRC(unsigned char *commands, int size);
int compare_CRC(unsigned char *mensagem, int size);


#endif /* CRC16_H_ */
