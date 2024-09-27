#include <stdio.h>
#include "mbr.h"

int main() {
    MBR mbr;
    ler_mbr("mbr.bin", &mbr);
    imprimir_mbr(&mbr);

    return 0;
}
