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

	/*
	time_t struct_time;
	_time(&struct_time, 0);
	printf("year: %d, month: %d, day: %d, hour: %d, minutes: %d\n ", struct_time.year, struct_time.month, struct_time.day,
	struct_time.hour, struct_time.minutes);
	*/
	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}