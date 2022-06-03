#include <stdklib.h>
#include <kstring.h>s

extern int _read(int fd, char * buffer, size_t count);
extern int _write(int fd, char * buffer, size_t count);

char getchar(){
    char * buffer=NULL;
    _read(STDIN, buffer, 1);
    return *buffer;
}

int putchar(int character){
    _write(STDOUT, &character, 1);
    return character;
}

int puts(const char * str){
    size_t length= strlen(str);
    return _write(STDOUT, str, length);
}

size_t strlen(const char * str){
    int i=0;
    while (str[i] != '\0'){
        i++;
    }
    return i;
}

//--https://www.geeksforgeeks.org/implement-itoa/--
char* itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}
//-------------------------------------------------

//--https://iq.opengenus.org/how-printf-and-scanf-function-works-in-c-internally/--
int printf (char * str, ...)
{
	va_list vl;
	int i = 0, j=0;
		char buff[100]={0}, tmp[20];
		va_start( vl, str ); 
		while (str && str[i])
		{
		  	if(str[i] == '%')
		  	{
 		    i++;
 		    switch (str[i]) 
 		    {
	 		    case 'c': 
	 		    {
	 		        buff[j] = (char)va_arg( vl, int );
	 		        j++;
	 		        break;
	 		    }
	 		    case 'd': 
	 		    {
                    itoa(va_arg( vl, int ), tmp, 10);
	 		        strcpy(&buff[j], tmp);
	 		        j += strlen(tmp);
		           break;
                }
		        case 'x': 
		        {   
		           itoa(va_arg( vl, int ), tmp, 16);
		           strcpy(&buff[j], tmp);
		           j += strlen(tmp);
		           break;
		        }
        	}
     	} 
     	else 
	    {
	       	buff[j] =str[i];
	       	j++;
	    }
	    i++;
	} 
    _write(1,buff,j);
    va_end(vl);
    return j;
}

//-------------------------------------------------