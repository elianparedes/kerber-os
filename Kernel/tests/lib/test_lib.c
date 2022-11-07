// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib/linked_list.h>
#include <assert.h>
#include <stdlib.h>
#include <lib.h>

void test_linked_list(){
    
    list_ptr list = new_linked_list(NULL);
    add(list,"Alfredo");
    add(list,"Carlota");
    add(list,"Beatriz");
    add(list,"Dario");

    to_begin(list);

    while (hasNext(list))
    {
        puts((char *)next(list));
    }
    
    add(list,"Emilio");
    add(list,"Fernanda");

    to_begin(list);

    puts("____________________________________");
    while (hasNext(list))
    {
        puts((char *)next(list));
    }
}