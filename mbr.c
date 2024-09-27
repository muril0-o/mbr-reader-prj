#include <stdio.h>
#include <string.h>
#include "mbr.h"

const char *tipos(uint8_t id) {
    switch (id) {
    case 0x0b:
        return "W95 FAT32";
    case 0x83:
        return "Linux";
    case 0x82:
        return "Linux Swap";
    case 0x07:
        return "HPFS/NTFS/exFAT";
    case 0x05:
        return "Extended";
    default:
        return "Desconhecido";
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

    // Verificar a assinatura
    uint16_t assinatura = ((uint16_t)bytes[510]) | ((uint16_t)bytes[511] << 8);
    if (assinatura != 0xAA55) {
        printf("Assinatura inválida, não é uma MBR.\n");
        return;
    }


    // Processar partições
    for (int i = 0; i < MAX_PARTICOES; i++) {
        uint32_t start = ((uint32_t)bytes[454 + (16 * i)] << 24) |
                         ((uint32_t)bytes[453 + (16 * i)] << 16) |
                         ((uint32_t)bytes[452 + (16 * i)] << 8) |
                         ((uint32_t)bytes[451 + (16 * i)]);
                         
        if (start == 0) {
            break; 
        }

        mbr->particoes[i].start = start;
        mbr->particoes[i].bootable = (bytes[446 + (16 * i)] == 128) ? '*' : ' ';
        mbr->particoes[i].sectors = ((uint32_t)bytes[458 + (16 * i)] << 24) |
                                    ((uint32_t)bytes[457 + (16 * i)] << 16) |
                                    ((uint32_t)bytes[456 + (16 * i)] << 8) |
                                    ((uint32_t)bytes[455 + (16 * i)]);

        mbr->particoes[i].size = (double)mbr->particoes[i].sectors / 2048.0; 
        mbr->particoes[i].id = bytes[450 + (16 * i)];
    }
}

void imprimir_mbr(const MBR *mbr) {
    printf("NAME        MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS\n");
    printf("/dev/sda    0:0    0 %.1lfG 0 disk\n", mbr->total_size);

    for (int i = 0; i < MAX_PARTICOES; i++) {
        if (mbr->particoes[i].start != 0) {
            printf("└─/dev/sda%d   0:%d    0 %.1lfG 0 part %s\n",
                   i + 1, i + 1, mbr->particoes[i].size, tipos(mbr->particoes[i].id));
        }
    }
    
    printf("Identificador do disco: 0x%x\n", mbr->signature);
}

