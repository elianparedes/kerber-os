#include <kstdio.h>
#include <inforeg.h>

cpu_state_t registers;

static void format_reg_str(char * dest , uint64_t reg){
	dest[0] = '0';
	dest[1] = 'x';
	char aux[REG_SIZE];
	itoa(reg,aux,16);
	int len = strlen(aux);
	for(int i = REG_SIZE+2-len , j = 0; i < REG_SIZE+2 ; i++ , j++){
		dest[i] = aux[j];
	}
	for(int i = 2 ; i < REG_SIZE+2-len; i++){
		dest[i] = '0';
	}

	dest[REG_SIZE+2] = '\0';
}

static void show_registers(){
	char reg_str[REG_SIZE+3];
	char * registers_strings[] = {"R15: ","R14: ","R13: ","R12: ","R11: ","R10: ","R9: ","R8: ", "RDX: " , "RCX: ", "RBX: ", "RAX: ", 
	"RSI: ", "RDI: ", "RBP: ", "RSP: ", "RIP: ", "RFLAGS: "};
	uint64_t regs_values[] = 
	{registers.r15  ,registers.r14  ,registers.r13  ,registers.r12 , registers.r11 , registers.r10 , registers.r9 ,
	registers.r8 , registers.rdx , registers.rcx , registers.rbx , registers.rax , registers.rsi , registers.rdi ,
	registers.rbp , registers.rsp , registers.rip , registers.rflags};

	for(int i = 0 ; i < REGISTERS_COUNT ; i++){
		format_reg_str(reg_str,regs_values[i]);
		printf(registers_strings[i]);
		puts(reg_str);
	}
}

void inforeg(){
    int succeed;
	succeed = _copy_cpu_state(&registers,KBD_PRINT_REG);

	if(!succeed){
		puts("No snapshot to show");
		return;
	}
	show_registers();
}