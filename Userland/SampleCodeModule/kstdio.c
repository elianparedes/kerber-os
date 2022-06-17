#include <kstdio.h>

#include <kstdlib.h>
#include <stdarg.h>

char getchar(){
    char buffer;
    _read(STDIN, &buffer, 1);
    return buffer;
}

int putchar(int character){
    _write(STDOUT, &character, 1);
    return character;
}

int puts(const char * str){
    size_t length= strlen(str);
	char new_str[length+2];
	int i;
	for (i=0; i < length; i++){
		new_str[i]=str[i];
	}
	new_str[i++]='\n';
	new_str[i]='\0';
    return _write(STDOUT, new_str, length+1);
}

/* Implementation by https://iq.opengenus.org/how-printf-and-scanf-function-works-in-c-internally/ */
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
		           uitoa(va_arg( vl, uint64_t ), tmp, 16);
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

/* Implementation by https://iq.opengenus.org/how-printf-and-scanf-function-works-in-c-internally/-- */
int scanf (char * str, ...)
{
    va_list vl;
    int i = 0, j=0, ret = 0;
    char buff[100] = {0}, tmp[20], c;
    c='\0';
    char *out_loc;
    while(c != '\n') 
    {
        _read(STDIN, &c ,1);
        buff[i] = c;
 	    i++;
 	}
 	va_start( vl, str );
 	i = 0;
 	while (str && str[i])
 	{
 	    if (str[i] == '%') 
 	    {
 	       i++;
 	       switch (str[i]) 
 	       {
 	           case 'c': 
 	           {
	 	           *(char *)va_arg( vl, char* ) = buff[j];
	 	           j++;
	 	           ret ++;
	 	           break;
 	           }
 	           case 'd': 
 	           {
	 	           *(int *)va_arg( vl, int* ) =strtol(&buff[j], &out_loc, 10);
	 	           j+=out_loc -&buff[j];
	 	           ret++;
	 	           break;
 	            }
 	            case 'x': 
 	            {
	 	           *(int *)va_arg( vl, int* ) =strtol(&buff[j], &out_loc, 16);
	 	           j+=out_loc -&buff[j];
	 	           ret++;
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
    va_end(vl);
    return ret;
}