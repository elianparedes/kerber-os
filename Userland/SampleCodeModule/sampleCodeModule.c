/* sampleCodeModule.c */
#include <kstdio.h>
#include <kstring.h>
#include <kctype.h>
#include <kstdlib.h>

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {
	//All the following code may be removed 
	*v = 'X';
	*(v+1) = 0x74;
	printf("Ingrese dos numeros separados por un espacio \n");
	int n1=1;
	int n2=2;
	scanf("%d %d", &n1, &n2);
	printf("Sus numeros ingresados son: %d y %d", n1, n2);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}