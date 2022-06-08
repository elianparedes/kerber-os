/* sampleCodeModule.c */
#include <kstdio.h>
#include <kstring.h>
#include <kctype.h>
#include <kstdlib.h>
#include <help.h>
#include <shell.h>

int main() {
	
	_clear_screen();

	_switch_screen_mode(0);

	_run(&shell);

	return 0xDEADBEEF;
}