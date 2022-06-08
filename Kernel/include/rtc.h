#ifndef _RTC_H_
#define _RTC_H_

#include <stdint.h>

typedef struct time{
    uint64_t year;
    uint64_t month;
    uint64_t day;
    uint64_t hour;
    uint64_t minutes;
    uint64_t seconds;
} time_t;

/**
 * Maneja los valores que requiere el registro de RTC para obtener la unidad de tiempo descripta
 **/
typedef enum {SEC = 0, MIN = 2, HOUR = 4,DAY_WEEK = 6 , DAY_MONTH = 7 , MONTH = 8, YEAR = 9} TIME;

/**
 * Indica en que base se desea obtener el resultado solicitado
 **/
typedef enum {BCD=0,DECIMAL} BASE_MODE;

/**
 * Indica el modo en que se quiere la hora
 **/
typedef enum {HS_12=0,HS_24} HS_MODE;

/**
 * @param utc_offset setea el horario local de la computadora. Por DEFAULT es 0
 **/
void set_UTC_offset(int utc_offset);

/**
 * @param base_mode setea la base en que se recibe la respuesta del tiempo solicitado. Por DEFAULT es DECIMAL
 **/
void set_base_mode(BASE_MODE base_mode);

/**
 * @param base_mode setea el formato de hora a utilizar. Por DEFAULT es HS_24
 **/
void set_hs_mode(HS_MODE hs_mode);

uint64_t get_seconds();

uint64_t get_minutes();

uint64_t get_hour();

uint64_t get_day_week();

uint64_t get_day_month();

uint64_t get_month();

uint64_t get_year();

void get_struct_time();

#endif /* _RTC_H_ */