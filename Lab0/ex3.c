#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BIT(n) (1 << (n))

//challenge 1
void print_bits(int n) {
    //iterar do bit 31 até o 0
    for (int i = 31; i >= 0; i--) {
        int bit = (n >> i) & 1;
        printf("%d", bit);
        if (i % 8 == 0 && i != 0) printf(" "); // Espaço para legibilidade
    }
    printf("\n");
}

int main() {
    int n = 0;
    printf("n=%d\n", n);
    n = n | 1;
    printf("n|1=%d\n", n);

    printf("0b101^0b10=%d\n", 0b101^0b10);
    printf("0xF+1=%d\n", 0xF+1);

    int k = 0xE;
    printf("k=%d\n", k);
    printf("Bit 3 of k is %d\n", (k >> 3) & 1);

    //challenge 1
    int meu_n = 10; // 10 em binário: 0000 1010
    printf("Bits de %d: ", meu_n);
    print_bits(meu_n);

    //challenge 2
    char cmd[20];
    unsigned int v1, v2;

    while (scanf("%s %u %u", cmd, &v1, &v2) != EOF) {

        if (strcmp(cmd, "and") == 0) {
            //AND
            printf("%u\n", v1 & v2);
        } 
        else if (strcmp(cmd, "or") == 0) {
            //OR 
            printf("%u\n", v1 | v2);
        } 
        else if (strcmp(cmd, "set") == 0) {
            // Define o bit para 1
            printf("%u\n", v1 | (1 << v2));
        } 
        else if (strcmp(cmd, "unset") == 0) {
            // Define o bit para 0
            printf("%u\n", v1 & ~(1 << v2));
        } 
        else if (strcmp(cmd, "byte") == 0) {
            // Extrai o byte
            printf("%u\n", (v1 >> (v2 * 8)) & 0xFF);
        }
    }
return 0;
}