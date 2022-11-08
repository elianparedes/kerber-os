// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <inforeg.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <kstring.h>

static void format_reg_str(char *dest, uint64_t reg) {
    dest[0] = '0';
    dest[1] = 'x';
    char aux[REG_SIZE];
    itoa(reg, aux, 16);
    int len = strlen(aux);
    for (int i = REG_SIZE + 2 - len, j = 0; i < REG_SIZE + 2; i++, j++) {
        dest[i] = aux[j];
    }
    for (int i = 2; i < REG_SIZE + 2 - len; i++) {
        dest[i] = '0';
    }

    dest[REG_SIZE + 2] = '\0';
}

static void show_registers(cpu_state_t registers) {
    char reg_str[REG_SIZE + 3];
    char *registers_strings[] = {
        "R15:    ", "R14:    ", "R13:    ", "R12:    ", "R11:    ", "R10:    ",
        "R9:     ", "R8:     ", "RDX:    ", "RCX:    ", "RBX:    ", "RAX:    ",
        "RSI:    ", "RDI:    ", "RBP:    ", "RSP:    ", "RIP:    ", "RFLAGS: "};
    uint64_t regs_values[] = {registers.r15, registers.r14, registers.r13,
                              registers.r12, registers.r11, registers.r10,
                              registers.r9,  registers.r8,  registers.rdx,
                              registers.rcx, registers.rbx, registers.rax,
                              registers.rsi, registers.rdi, registers.rbp,
                              registers.rsp, registers.rip, registers.rflags};

    printf("Last snapshot taken:\n");
    for (int i = 0; i < REGISTERS_COUNT; i++) {
        format_reg_str(reg_str, regs_values[i]);
        printf("%s",registers_strings[i]);
        puts(reg_str);
    }
}

int inforeg(int argc, char *argv[]) {
    int succeed;
    cpu_state_t aux;
    succeed = _copy_cpu_state(&aux, KBD_PRINT_REG);

    if (!succeed) {
        printf("No snapshot to show.\n"
               "Type \"help inforeg\" for more information.\n");
        return -1;
    }

    show_registers(aux);

    return 0;
}