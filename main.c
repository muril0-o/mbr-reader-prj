#include <stdio.h>
#include "mbr.h"

int main() {
    MBR mbr;

    // Lê os dados da MBR a partir de um arquivo binário
    ler_mbr("mbr.bin", &mbr);

    // Verifica se a assinatura da MBR está correta
    if (mbr.signature != 0xAA55) {
        printf("Erro: Assinatura inválida, não é uma MBR.\n");
        return 1;
    }

    // Exibe as informações lidas da MBR
    imprimir_mbr(&mbr);

    return 0;
}
