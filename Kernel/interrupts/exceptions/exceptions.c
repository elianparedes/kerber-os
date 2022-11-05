// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <exceptions.h>
#include <interrupts.h>
#include <registers.h>

#define ZERO_EXCEPTION_ID           0
#define INVALID_OPCODE_EXCEPTION_ID 6
#define REG_SIZE                    16
#define REGISTERS_COUNT             18

static void zero_division_exception();
static void opcode_exception();

cpu_state_t registers;

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

static void show_registers(char *error_message, int exception_id) {
    gclear_screen(get_current_process()->g_context);

    printf("%s", error_message);
    printf(" - CODE=%d\n", exception_id);

    char reg_str[REG_SIZE + 3];
    char *registers_strings[] = {
        "R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ",
        "R9: ",  "R8: ",  "RDX: ", "RCX: ", "RBX: ", "RAX: ",
        "RSI: ", "RDI: ", "RBP: ", "RSP: ", "RIP: ", "RFLAGS: "};
    uint64_t regs_values[] = {registers.r15, registers.r14, registers.r13,
                              registers.r12, registers.r11, registers.r10,
                              registers.r9,  registers.r8,  registers.rdx,
                              registers.rcx, registers.rbx, registers.rax,
                              registers.rsi, registers.rdi, registers.rbp,
                              registers.rsp, registers.rip, registers.rflags};

    for (int i = 0; i < REGISTERS_COUNT; i++) {
        format_reg_str(reg_str, regs_values[i]);
        printf("%s", registers_strings[i]);
        puts(reg_str);
    }
}

void exceptionDispatcher(int exception) {
    copy_cpu_state(&registers, EXCEPTION);
    if (exception == ZERO_EXCEPTION_ID) {

        zero_division_exception();
    } else if (exception == INVALID_OPCODE_EXCEPTION_ID) {
        opcode_exception();
    }

    exit_process(0);
}

static void zero_division_exception() {
    show_registers("Divide by Zero Exception", ZERO_EXCEPTION_ID);
}

static void opcode_exception() {

    show_registers("Invalid Op Code Exception", INVALID_OPCODE_EXCEPTION_ID);
}
