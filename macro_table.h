/*
* Created by Avishav & Sapir on March 2, 2022
*/

#ifndef EX14_MACRO_TABLE_H
#define EX14_MACRO_TABLE_H

#include "macro.h"

typedef struct macro_table *macro_table;
macro_table init_macro_table();
void delete_macro_table(macro_table to_delete);
macro get_macro_by_name(macro_table macros, char *name);
void push_macro(macro_table table, macro new_macro);

#endif