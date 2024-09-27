#ifndef MBR_H
#define MBR_H

#include <stdint.h>

#define MAX_PARTICOES 4  // MBR suporta até 4 partições primárias

typedef struct {
    uint32_t start;    // Setor inicial da partição
    uint32_t end;      // Setor final da partição
    uint32_t sectors;  // Número de setores da partição
    double size;       // Tamanho em GB
    uint8_t id;        // ID do tipo de partição
    char bootable;     // Indica se a partição é bootável
} Particao;

typedef struct {
    Particao particoes[MAX_PARTICOES];  // Até 4 partições
    uint16_t signature;                 // Assinatura da MBR (2 bytes)
    uint64_t total_sectors;             // Total de setores no disco
    double total_size;                  // Tamanho total do disco em GB
} MBR;

// Funções para ler e imprimir MBR
void ler_mbr(const char *filename, MBR *mbr);
const char *tipos(uint8_t id);
void imprimir_mbr(const MBR *mbr);

#endif
