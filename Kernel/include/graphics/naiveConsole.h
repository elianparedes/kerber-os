#ifndef _NAIVE_CONSOLE_H_
#define _NAIVE_CONSOLE_H_

#include <stdint.h>

/**
 * @brief Prints a string on screen and moves the current cursor.
 * @deprecated
 * @param string
 */
void ncPrint(const char *string);

/**
 * @brief Prints a char on screen and moves the current cursor.
 *
 * @param character
 */
void ncPrintChar(char character);

/**
 * @brief Creates a line break on screen and moves the current
 * cursor to the next line.
 *
 */
void ncNewline();

/**
 * @brief Converts decimal value to string and prints it on screen.
 *
 * @param value decimal number
 */
void ncPrintDec(uint64_t value);

/**
 * @brief Converts hexadecimal value to string and prints it on screen.
 *
 * @param value hexadecimal number
 */
void ncPrintHex(uint64_t value);

/**
 * @brief Converts binary value to string and prints it on screen.
 *
 * @param value hexadecimal number
 */
void ncPrintBin(uint64_t value);

/**
 * @brief Converts base-n value to string and prints it on screen.
 *
 * @param value base-n number
 * @param base the n base of the number to print
 */
void ncPrintBase(uint64_t value, uint32_t base);

/**
 * @brief Clears the screen and resets the current cursor.
 *
 */
void ncClear();

#endif /* _NAIVE_CONSOLE_H_ */