#include <stdio.h>
#include <time.h>
#define CSV_FILE "temperature_control.csv"


void createCSVFile() {
    FILE *file = fopen(CSV_FILE, "r");

    if(file == NULL) {
        file = fopen(CSV_FILE, "w");
        fprintf(file, "timestamp, Temperatura Interna, Temperatura Referencial, Temperatura Externa, Ventoinha, Resistor\n");
    }
    fclose(file);
}

void writeOnCSVFile(float internal, float referencial, float external, float resistor, float fan) {
    createCSVFile();

    FILE *file = fopen(CSV_FILE, "a");

    char timestamp[50];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(timestamp,"%02d-%02d-%d %02d:%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(file, "%s,%.2f,%.2f,%.2f,%.2f,%.2f\n", timestamp, internal, referencial, external, fan, resistor);

    fclose(file);
}