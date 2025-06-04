// programa_b.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    pid_t pid;
    unsigned long addr;
    int valor;

    printf("PID do processo: ");
    scanf("%d", &pid);

    printf("Endereço da variável (em hexa): ");
    scanf("%lx", &addr);

    char mem_path[64];
    sprintf(mem_path, "/proc/%d/mem", pid);

    int mem = open(mem_path, O_RDONLY);
    if(mem == -1) {
        perror("open");
        return 1;
    }

    if(pread(mem, &valor, sizeof(valor), addr) == -1) {
        perror("pread");
        close(mem);
        return 1;
    }

    printf("Valor lido: %d\n", valor);

    close(mem);
    return 0;
}
