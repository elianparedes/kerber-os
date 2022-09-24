#include <kctype.h>

int isspace(int c) {
    return c == ' ';
}

int isdigit(int c) {
    return c >= '0' && c <= '9';
}

int isalpha(int c) {
    return islower(c) || isupper(c);
}

int islower(int c) {
    return c >= 'a' && c <= 'z';
}

int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}