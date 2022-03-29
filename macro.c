//
// Created by Sapir on 28.2.2022.
//

#include "macro.h"
#include "macro_body.h"
#include "linked_list.h"
#include <stdlib.h>
#include "string_manipulations.h"
#define MEM_SIZE 8192
struct macro{
    char *name;
    macro_body body;
};

/* create new macro and reset all variables */
macro init_macro(){
    macro new_macro = (macro)malloc(sizeof(struct macro));
    new_macro->name = NULL;
    new_macro->body = init_body();
    return new_macro;
}

char *get_macro_name(macro macro){
    return macro->name;
}
void set_macro_name(macro macro, char *name){
    trim_whitespace(name);
    macro->name=(char *) malloc(strlen(name)+1);
    strcpy(macro->name,name);
}

macro_body get_macro_body(macro macro){
    return macro->body;
}
void set_macro_body(macro macro, macro_body body){
    macro->body=body;
}

/*append line to the body of the macro*/
void append_line(macro macro, char *line){
    append_line_to_body(macro->body,line);
}


void delete_macro(macro to_delete){
    free(to_delete->name);
    delete_body(to_delete->body);
    free(to_delete);
}

void macro_call(FILE *dest,macro to_print){
    print_body(dest,to_print->body);
}
