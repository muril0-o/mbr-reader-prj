#ifndef MBR_H
#define MBR_H

#include <stdint.h>

#define MAX_PARTICOES 4

typedef struct {
    uint32_t start;
    uint32_t end;
    uint32_t sectors;
    double size;
    uint8_t id;
    char bootable;
} Particao;

typedef struct {
    Particao particoes[MAX_PARTICOES];
    uint32_t signature;
    uint64_t total_sectors;
    double total_size;
} MBR;

void ler_mbr(const char *filename, MBR *mbr);
const char *tipos(uint8_t id);
void imprimir_mbr(const MBR *mbr);

#endif 
