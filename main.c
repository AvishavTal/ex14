#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#include "symbol.h"
#include "macro.h"

#define LINE_LENGTH 81


void check_list_of_symbols();//to delete later
void check_list_of_macros();//to delete later

#include "file.h"
#include "pre_assembler.h"
#include "first_scan.h"
#include "second_scan.h"

file *init_files(file *files, int n_files, char **names);

void run_assembler(file *files, int n_files);

int main(int argc, char **argv){
//    FILE *ps= fopen("/home/avishav/Documents/study/cPrograming/seconde_chance/ex14/ps.as","r");
//    FILE *dest=fopen("/home/avishav/Documents/study/cPrograming/seconde_chance/ex14/ps.out","a");
    file ps= init_file("/home/avishav/Documents/study/cPrograming/seconde_chance/ex141/ps");
    pre_assembler(ps);
    return 0;
}

void run_assembler(file *files, int n_files) {
    int i=0;
    for (; i < n_files; ++i) {
        if (files[i]){

        }
    }
}

file *init_files(file *files, int n_files, char **names) {
    int i=0;
    files=(file *) malloc(sizeof(file)*n_files);
    for ( ; i < n_files; ++i) {
        init_file(names[i]);
    }
    return files;
}

void check_list_of_symbols(){//to delete later
    //list of symbols

    list symbol_list = create_empty_list();
    symbol new_symbol = init_symbol();
    char *name1 = "symbol1";
    update_symbol(new_symbol, name1, 5, 4, 3);
    add_to_tail(symbol_list, new_symbol);

    symbol new_symbol2 = init_symbol();
    char *name2 = "symbol2";
    update_symbol(new_symbol2, name2, 5, 4, 3);
    add_to_tail(symbol_list, new_symbol2);

    print_list(symbol_list);
}

void check_list_of_macros(){//to delete later
    //list of symbols

    list macro_list = create_empty_list();

    macro new_macro = init_macro();
    char *name1 = "macro1";
    set_macro_name(new_macro, name1);

    add_to_tail(macro_list, new_macro);

    macro new_macro2 = init_macro();
    char *name2 = "macro2";
    set_macro_name(new_macro2, name2);

    add_to_tail(macro_list, new_macro2);
    print_list(macro_list);
}