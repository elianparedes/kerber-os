#include <kstring.h>

char * strchr (char * str, int character ){
    for (int i=0; str[i] != '\0'; i++){
        if (str[i] == character){
            return str+i;
            }
        }
    return NULL;
}

char *strcpy(char *dest, const char *src){
    for (int i=0; src[i] != '\0'; i++){
        dest[i] = src[i];
    }
    return dest;
}

int strcmp(const char *str1, const char *str2){
    while (*str1 && (*str1==*str2)){
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

//--https://www.geeksforgeeks.org/implement-itoa/--
void swap (char * c1, char * c2){
    char aux= *c1;
    *c1= *c2;
    *c2=aux;
}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(str+start, str+end);
        start++;
        end--;
    }
}
//-------------------------------------------------