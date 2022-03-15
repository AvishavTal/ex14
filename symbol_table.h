/*
** Created by Avishav & Sapir on March 7,2022
*/

#ifndef EX14_SYMBOL_TABLE_H
#define EX14_SYMBOL_TABLE_H

#include "symbol.h"
#include "errors.h"
#include "boolean.h"
#include "file.h"

typedef struct symbol_table *symbol_table;

/**
 * Returns new symbol_table with default values
 * @return new symbol_table with default values
 */
symbol_table init_symbol_table();

/**
 * Add to the table new symbol if the received name is legal name and not exist in the table already
 * @param symbols table of symbols to which we want to add a symbol
 * @param symbol_name wanted name for the new symbol
 * @param address new symbol address
 * @param is_entry a flag that symbolizes whether the symbol is entry
 * @param is_extern a flag that symbolizes whether the symbol is extern
 * @param is_data a flag that symbolizes whether the symbol is data
 * @param is_code a flag that symbolizes whether the symbol is code
 * @param err error code is saved here if an error occurred
 */
void add_symbol(symbol_table symbols, char *symbol_name, unsigned long address, boolean is_entry,boolean is_extern, boolean is_data, boolean is_code, error *err);

/**
 * Returns the symbol by name
 * @param symbols table of symbols in which to search for the name
 * @param symbol_name wanted symbol's name
 * @return the symbol by name
 */
symbol get_symbol_by_name(symbol_table symbols, char *symbol_name);

/**
 * Gets legal symbol and push it to the table
 * @param to_update table of symbols where we want to add the symbol
 * @param to_push symbol we wand to add
 */
void push_legal_symbol(symbol_table to_update, symbol to_push);

/**
 * Delete all the symbols in the table and the table itself
 * @param to_delete wanted table to delete
 */
void delete_symbol_table(symbol_table to_delete);

/**
 * Print to file all the symbols defined as entry in the table
 * @param dest file to print to
 * @param to_print table of symbols
 */
void print_entries(FILE *dest,symbol_table to_print);

/**
 * Print to file all the symbols defined as extern in the table
 * @param dest file to print to
 * @param to_print table of symbols
 */
void print_externals(FILE *dest,symbol_table to_print);

/**
 * Check if the needed symbol already exist in the symbols table
 * @param symbols table of symbols
 * @param name symbol's name
 * @param is_extern value of a flag that symbolizes whether the symbol is extern
 * @param err error code is saved here if an error occurred
 * @return true if this symbol already exist in the table, false otherwise
 */
boolean double_definition(symbol_table symbols, char *name, boolean is_extern, error *err);

/**
 * Check if the received name is legal name - contains only alphanumeric characters, start with alphabetic letter, length no longer than MAX_NAME_LENGTH
 * @param name the symbol name
 * @return true if legal name, false otherwise
 */
boolean is_legal_name(char *name);

/**
 * Push all data symbols to the end of the table
 * @param symbols table of symbols
 * @param final_ic number of instruction counter after first scan
 */
void update_addresses_of_data_symbols(symbol_table symbols,unsigned long final_ic);

#endif /* EX14_SYMBOL_TABLE_H */