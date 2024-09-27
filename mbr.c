#include <stdio.h>
#include <string.h>
#include "mbr.h"

const char *tipos(uint8_t id) {
    switch (id) {
    case 0x0b:
        return "W95 FAT32";
    case 0x83:
        return "Linux";
    default:
        return "não encontrado";
    }
}

void ler_mbr(const char *nome_arquivo, MBR *mbr) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    uint8_t bytes[512];
    fread(bytes, 1, 512, arquivo);
    fclose(arquivo);

    uint32_t total_setores = 0;
    for (int i = 0; i < MAX_PARTICOES; i++) {
        mbr->particoes[i].start = ((uint32_t)bytes[457 + (16 * i)] << 24) |
                                   ((uint32_t)bytes[456 + (16 * i)] << 16) |
                                   ((uint32_t)bytes[455 + (16 * i)] << 8) |
                                   ((uint32_t)bytes[454 + (16 * i)]);

        if (mbr->particoes[i].start == 0) {
            break; 
        }

        mbr->particoes[i].bootable = (bytes[446 + (16 * i)] == 128) ? '*' : ' ';
        mbr->particoes[i].sectors = ((uint32_t)bytes[461 + (16 * i)] << 24) |
                                     ((uint32_t)bytes[460 + (16 * i)] << 16) |
                                     ((uint32_t)bytes[459 + (16 * i)] << 8) |
                                     ((uint32_t)bytes[458 + (16 * i)]);

        mbr->particoes[i].end = mbr->particoes[i].start + mbr->particoes[i].sectors - 1;
        mbr->particoes[i].size = (double)mbr->particoes[i].sectors / 2048.0; 
        mbr->particoes[i].id = bytes[450 + (16 * i)];

        total_setores += mbr->particoes[i].sectors;
    }

    mbr->total_sectors = total_setores;
    mbr->total_size = (double)total_setores / 2048.0; 

    mbr->signature = ((uint32_t)bytes[0x1bb] << 24) |
                     ((uint32_t)bytes[0x1ba] << 16) |
                     ((uint32_t)bytes[0x1b9] << 8) |
                     ((uint32_t)bytes[0x1b8]);
}

void imprimir_mbr(const MBR *mbr) {
    printf("\n[\nDisco: /dev/sda: %.0lf GB\n%lu bytes\n%lu setores\n]\n", mbr->total_size, mbr->total_sectors * 512, mbr->total_sectors);
    printf("Identificador do disco: 0x%x\n", mbr->signature);

    for (int i = 0; i < MAX_PARTICOES; i++) {
        if (mbr->particoes[i].start != 0) {
            printf("Dispositivo: /dev/sda%d | Boot: %c  | Início: %7u | Fim: %8u | Setores: %8u | Tamanho: %3.0lfG | ID: %2x | Tipo: %s\n",
                   i + 1, mbr->particoes[i].bootable,
                   mbr->particoes[i].start, mbr->particoes[i].end,
                   mbr->particoes[i].sectors, mbr->particoes[i].size,
                   mbr->particoes[i].id, tipos(mbr->particoes[i].id));
        }
    }
}
