//
// Created by avishav on 28.2.2022.
//
#include <stdio.h>
#include <string.h>
#include "first_scan.h"
#include "symbol_table.h"
#include "string_manipulations.h"
#include "symbol.h"
#include "file.h"
#include "instruction.h"
#include "parser.h"
#include "first_and_second_scan_constants.h"

#define MIN_DC 0
#define true 1;
#define false 0;
#define COMMENT_DELIMITERS ";"

int check_if_label(char *first_word_in_line);
char* get_next_word_in_line(char *line, char* delim){
    char *result;

    result = strtok(line,delim);
    trim_whitespace(result);

    return result;
}
int get_base_address(int address){

}

void add_data_or_string_label_to_symbol_table(symbol_table symbol_table,char *label_name, int DC ){
    long base_address = (int)(140/16) * 16;
    long offset = DC - base_address;

    add_symbol(symbol_table, label_name, DC, base_address, offset, false, false, true, false,error *error1);//todo error, how to update DC
}

int add_data_or_string_to_data_image(data_image data_image, /*......*/ ){

}

void first_scan(file source) {
    symbol_table symbol_table = init_symbol_table();
    data_image data_image = init_data_image();
    char line[LINE_LENGTH], *first_word_in_line, *second_word_in_line;
    FILE *src, *dest;
    int IC = MIN_IC, DC = MIN_DC;
    int is_there_error = false;
    int is_label = false;
    char *label_name;

    first_word_in_line = get_next_word_in_line(line, " \t");
    second_word_in_line = get_next_word_in_line(line + strlen(strtok(line," \t")), " \t");

    src= fopen(get_name_am(source),"r");//todo add open file check
    dest= fopen(get_name_ob(source),"a");


    while ((fgets(line,LINE_LENGTH,src))!=NULL){
        char* get_next_word_in_line(char *line, char* delim);


        /* If this line is a blank line or a comment line */
        if(first_word_in_line == NULL || first_word_in_line == COMMENT_DELIMITERS || is_entry_def(first_word_in_line)){
            IC++;
        }
        else{
            if(check_if_label(first_word_in_line)){//todo
                is_label = true;
                strcpy(label_name, strtok(first_word_in_line,":")); /* label_name will be the name witout the : */

                /* if both label and data */
                if(is_string_def(second_word_in_line) || is_data_def(second_word_in_line)) {
                    //todo check in notebook what is the base address and offset of this label
                    add_data_or_string_label_to_symbol_table(symbol_table, label_name, DC );
                    DC += add_data_or_string_to_data_image();//todo
                }
            }
            else { /* if not label */
                /* if data or string line*/
                if (is_string_def(first_word_in_line) || is_data_def(first_word_in_line)) {
                    DC += add_data_or_string_to_data_image();
                }
                    /* if external line */
                else if (is_extern_def(first_word_in_line)) {
                    add_symbol(symbol_table, second_word_in_line, 0, 0,
                               0, false, true, false, false,error *error1);//todo error
                }
                    /* its instraction line */
                else {
                    if (is_label) {
                        add_code_label_to_symbol_table(symbol_table, label_name, IC);
                    }
                    IC += add_instraction(); // inside the func check if the instraction name exist, check if operand num correct and if the miun(?) type correct
                }
            }
        }
    } /* We have completed the transition over the entire file  */

    if(!(print_errors())){ // check + print if there is errors - if not continue
        ICF = IC;
        DCF = DC;
        update_symbol_table(ICF);//todo
    }

}



/*
void second_scan(file source){

    while ((fgets(line,LINE_LENGTH,src))!=NULL){
        first_word_in_line=strtok(line," \t");
        trim_whitespace(first_word_in_line);
        if(is_symbol_def(first_word_in_line)){
            first_word_in_line= strtok(NULL," \t");
            trim_whitespace(first_word_in_line);
        }
        if (!is_extern_def(first_word_in_line) && !is_string_def(first_word_in_line) && !is_data_def(first_word_in_line)){
            if (is_entry_def(first_word_in_line)){
                char *symbol_name;
                symbol to_update;
                symbol_name= strtok(NULL," \t");
                to_update= get_symbol_by_name(symbol_name);
                if (to_update!=NULL){
                    mark_entry(to_update);
                } else{
                    //todo print symbol dosent exist error

                }
            } else{
                instruction temp_instruction;
                temp_instruction= init_instruction(line, table, &ic);
                print_instruction(dest,temp_instruction);//todo error check
            }
        }
    }
}
*/