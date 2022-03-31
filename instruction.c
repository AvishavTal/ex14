/*
* Created by Avishav & Sapir on March 1, 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"
#include "word.h"
#include "operator_table.h"
#include "string_manipulations.h"
#include "errors.h"
#include "ARE.h"
#include "register_table.h"
#include "system_errors.h"
#include "symbol.h"

#define MAX_N_WORDS 6

/*indexes of the words in the word list of the instruction*/
#define OP_WORD_INDEX 0
#define OPERANDS_INFO_WORD_INDEX 1
#define SOURCE_ADDRESS_WORD_INDEX 2
#define SOURCE_OFFSET_WORD_INDEX 3
#define DEST_ADDRESS_WORD_INDEX 4
#define DEST_OFFSET_WORD_INDEX 5

/*Indexes in the arguments list*/
#define DEST_ARG_INDEX 1
#define SOURCE_ARG_INDEX 0

/*number of arguments required*/
#define REQUIRED_TWO 2
#define REQUIRED_ONE 1
#define NO_ARGS_REQUIRED 0

#define IMMEDIATE_SIGN '#'

#define LEFT_BRAKET '['
#define RIGHT_BRAKET ']'

#define MAX_IMMEDIATE 32767
#define MIN_IMMEDIATE -32768

/*numbers of words required for each addressing mode*/
#define NUM_OF_WORDS_FOR_IMMEDIATE 1
#define NUM_OF_WORDS_FOR_DIRECT 2
#define NUM_OF_WORDS_FOR_INDEX 2
#define NUM_OF_WRDS_FOR_REGISTER_DIRECT 0


struct instruction{
    int n_words;/*number of words in the machine code*/
    word *words;
    operator op;
    addressing_mode dest_addressing;
    addressing_mode source_addressing;

};

void set_operator(instruction to_set, char *name, unsigned long *ic, error *err);

void set_args(instruction to_set, symbol_table symbols, char **arguments_list, unsigned long n_args, unsigned long *ic,
              error *err);

void
build_second_word(instruction to_set, symbol_table symbols, char *source, char *dest, unsigned long *ic, error *err);

addressing_mode recognize_addressing_mode(char *operand);

void set_addressing_modes(instruction to_set, char *source, char *dest, error *err);

int is_immediate(char *operand);

int is_index(char *operand);

int is_register_direct(char *operand);


void set_register_direct_operand(instruction to_set, char *operand, int is_dest, error *err);

void
build_operand(instruction to_set, symbol_table symbols, char *operand_str, int is_dest, unsigned long *ic, error *err);

void set_immediate_operand(instruction to_set, char *operand_str, int is_dest, unsigned long *ic, error *err);

void
set_direct_operand(instruction to_set, symbol_table symbols, char *operand_str, int is_dest, unsigned long *ic, error *err);

void set_index_operand(instruction to_set, symbol_table symbols, char *operand_str, int is_dest, unsigned long *ic, error *err);

int in_range(int immediate);

void break_to_label_and_reg(char *operan_str, char **label, char **reg_name, error *err);

void init_instruction_words(instruction to_set);

void update_n_words_helper(instruction to_set, addressing_mode mode);

void set_n_words(instruction to_set);

void print_instruction(FILE *dest, instruction to_print){
    int i=0;
    for (; i<MAX_N_WORDS ; ++i) {
        if(to_print->words[i]){
            print_word(dest,to_print->words[i]);
        }
    }
}

instruction init_instruction(char *line, symbol_table symbols, unsigned long ic, error *err) {
    instruction result;
    char *opname;
    line=trim_whitespace(line);
    opname= str_tok(line," \t");
    result=(instruction) calloc(1, sizeof(struct instruction));
    if (is_allocation_succeeded(result)){
        init_instruction_words(result);
        set_operator(result, opname, &ic, err);
        if(*err==NOT_ERROR){
            char **arguments;
            unsigned long op_len,arguments_len;
            op_len= strlen(opname);
            arguments=(char **) malloc(sizeof(char *));
            if(is_allocation_succeeded(arguments)) {
                arguments_len = split(line + op_len + 1, &arguments, ", \t");
                set_args(result, symbols, arguments, arguments_len, &ic, err);
                set_n_words(result);
                free(arguments);
            }
        }
    }
    return result;
}

int get_n_words(instruction instruction1){
    return instruction1->n_words;
}


void init_instruction_words(instruction to_set) {
    to_set->words=(word *) calloc(MAX_N_WORDS, sizeof(word));
    is_allocation_succeeded(to_set->words);
}

void delete_instruction(instruction to_delete){
    int i=0;
    for ( ; i <MAX_N_WORDS ;i++) {
        if(to_delete->words[i]!=NULL){
            delete_word(to_delete->words[i]);
        }
    }
    free(to_delete->words);
    free(to_delete);
}


void set_args(instruction to_set, symbol_table symbols, char **arguments_list, unsigned long n_args, unsigned long *ic,
              error *err) {
    int required_number_of_operands;
    required_number_of_operands= get_n_operands(to_set->op);
    if(n_args>required_number_of_operands){
        *err=TOO_MANY_ARGS;
    } else if (n_args<required_number_of_operands) {
        *err = TOO_FEW_ARGS;
    } else if (required_number_of_operands!=NO_ARGS_REQUIRED) {
        char *source=NULL,*dest=NULL;

        if (required_number_of_operands==REQUIRED_ONE){
            dest=*arguments_list;
        } else if(required_number_of_operands==REQUIRED_TWO){
            source=*arguments_list;
            dest=*(arguments_list+1);
        }
        set_addressing_modes(to_set,source,dest,err);
        build_second_word(to_set,symbols,source,dest,ic,err);
        build_operand(to_set,symbols,source,0,ic,err);
        build_operand(to_set,symbols,dest,1,ic,err);
        }


    }

void
build_operand(instruction to_set, symbol_table symbols, char *operand_str, int is_dest, unsigned long *ic, error *err) {
    if (operand_str){
        addressing_mode mode;
        mode=to_set->source_addressing;
        if(is_dest){
            mode=to_set->dest_addressing;
        }
        switch (mode) {
            case IMMEDIATE:
                set_immediate_operand(to_set, operand_str, is_dest, ic, err);
                break;
            case DIRECT:
                set_direct_operand(to_set, symbols, operand_str, is_dest, ic, err);
                break;
            case INDEX:
                set_index_operand(to_set, symbols, operand_str, is_dest, ic, err);
                break;
            case REGISTER_DIRECT:
                set_register_direct_operand(to_set, operand_str, is_dest, err);
                break;
        }
    }
}

void
set_index_operand(instruction to_set, symbol_table symbols, char *operand_str, int is_dest, unsigned long *ic, error *err) {
    char *label,*reg_name;
    regyster regi;
    break_to_label_and_reg(operand_str,&label,&reg_name,err);
    set_direct_operand(to_set,symbols,label,is_dest,ic,err);
    set_register_direct_operand(to_set,reg_name,is_dest,err);
    if((regi=get_register_by_name(reg_name))!=NULL){
        if(!is_valid_index(regi)){
            *err=INVALID_REGISTER_FOR_INDEX;
        }
    }
}

void break_to_label_and_reg(char *operan_str, char **label, char **reg_name, error *err) {
    char *end;
    operan_str= trim_whitespace(operan_str);
    *label= strtok(operan_str,"[");
    *reg_name= strtok(NULL,"]");
    end= strtok(NULL," \t");
    if(end){
        *err=ILLEGAL_OPERAND;
    }
}

void
set_direct_operand(instruction to_set, symbol_table symbols, char *operand_str, int is_dest, unsigned long *ic, error *err) {
    if(symbols){
        symbol symbol_operand;
        symbol_operand= get_symbol_by_name(symbols,operand_str);
        if(!symbol_operand){
            *err=UNDEFINED_SYMBOL;
        } else{
            word address_word,offset_word;
            are are1;
            unsigned long symbol_address,symbol_offset;
            address_word=init_word();
            offset_word=init_word();
            symbol_address= get_symbol_base_address(symbol_operand);
            symbol_offset= get_symbol_offset(symbol_operand);
            set_immediate(address_word,symbol_address);
            set_immediate(offset_word,symbol_offset);
            are1=RELOCATABLE;
            if (get_is_extern_symbol(symbol_operand)){
                are1=EXTERNAL;
            }
            set_are(address_word,are1);
            set_are(offset_word,are1);
            set_address(address_word,*ic);
            (*ic)++;
            set_address(offset_word,*ic);
            (*ic)++;
            if (get_is_extern_symbol(symbol_operand)){
                add_to_base_required(symbol_operand,get_address(address_word));
                add_to_offset_required(symbol_operand,get_address(offset_word));
            }
            if(is_dest){
                to_set->words[DEST_ADDRESS_WORD_INDEX]=address_word;
                to_set->words[DEST_OFFSET_WORD_INDEX]=offset_word;
            } else{
                to_set->words[SOURCE_ADDRESS_WORD_INDEX]=address_word;
                to_set->words[SOURCE_OFFSET_WORD_INDEX]=offset_word;
            }
        }
    }
}

void set_immediate_operand(instruction to_set, char *operand_str, int is_dest, unsigned long *ic, error *err) {
    int immediate;
    operand_str= trim_whitespace(operand_str);
    operand_str++; /*delete the # sign*/
    if (!str_to_int(operand_str,&immediate)){
        *err=NOT_INT;
    } else if(!in_range(immediate)){
        *err=NOT_IN_RANGE_IMMEDIATE;
    } else{
        word immediate_word;
        immediate_word=init_word();
        set_address(immediate_word,*ic);
        (*ic)++;
        set_immediate(immediate_word,immediate);
        set_are(immediate_word,ABSOLUTE);
        if(is_dest){
            to_set->words[DEST_ADDRESS_WORD_INDEX]=immediate_word;
        } else{
            to_set->words[SOURCE_ADDRESS_WORD_INDEX]=immediate_word;
        }
    }
}

int in_range(int immediate) {
    return (immediate>MIN_IMMEDIATE)&&(immediate<MAX_IMMEDIATE);
}


void set_register_direct_operand(instruction to_set, char *operand, int is_dest, error *err) {
    regyster reg;
    reg= get_register_by_name(operand);
    if (reg==NULL){
        *err=INVALID_REGISTER_NAME;
    } else{
        int register_code;
        register_code=get_reg_code(reg);
        if (is_dest){
            set_dest_register(to_set->words[OPERANDS_INFO_WORD_INDEX], register_code);
        } else{
            set_src_register(to_set->words[OPERANDS_INFO_WORD_INDEX], register_code);
        }
    }
}



void
build_second_word(instruction to_set, symbol_table symbols, char *source, char *dest, unsigned long *ic, error *err) {
    int funct;
    to_set->words[OPERANDS_INFO_WORD_INDEX]=init_word();
    funct= get_funct(to_set->op);
    set_address(to_set->words[OPERANDS_INFO_WORD_INDEX],*ic);
    (*ic)++;
    set_funct(to_set->words[OPERANDS_INFO_WORD_INDEX],funct);
    set_are(to_set->words[OPERANDS_INFO_WORD_INDEX],ABSOLUTE);
    set_src_addressing(to_set->words[OPERANDS_INFO_WORD_INDEX],to_set->source_addressing);
    set_dest_addressing(to_set->words[OPERANDS_INFO_WORD_INDEX],to_set->dest_addressing);
}

void set_addressing_modes(instruction to_set, char *source, char *dest, error *err) {
    to_set->dest_addressing = recognize_addressing_mode(dest);
    to_set->source_addressing = recognize_addressing_mode(source);
    if(!is_legal_dest_addressing_mode(to_set->op,to_set->dest_addressing) ||
       ((source!=NULL)&&(!is_legal_source_addressing_mode(to_set->op,to_set->source_addressing)))){
        *err=ILLEGAL_ADDRESSING;
    }
}

void set_n_words(instruction to_set) {
    int n_args_required;
    n_args_required= get_n_operands(to_set->op);
    to_set->n_words=1;
    if (n_args_required!=NO_ARGS_REQUIRED){
        to_set->n_words++;/*for the operands information word*/
        update_n_words_helper(to_set,to_set->dest_addressing);
        if(n_args_required==REQUIRED_TWO){
            update_n_words_helper(to_set,to_set->source_addressing);
        }
    }
}

void update_n_words_helper(instruction to_set, addressing_mode mode) {
    switch (mode) {
        case IMMEDIATE:
            to_set->n_words+=NUM_OF_WORDS_FOR_IMMEDIATE;
            break;
        case DIRECT:
            to_set->n_words+=NUM_OF_WORDS_FOR_DIRECT;
            break;
        case INDEX:
            to_set->n_words+=NUM_OF_WORDS_FOR_INDEX;
            break;
        case REGISTER_DIRECT:
            to_set->n_words+=NUM_OF_WRDS_FOR_REGISTER_DIRECT;
            break;
    }
}

addressing_mode recognize_addressing_mode(char *operand) {
    addressing_mode result;
    result=DEFAULT;
    if(operand){
        if (is_immediate(operand)){
            result=IMMEDIATE;
        } else if(is_index(operand)){
            result=INDEX;
        } else if(is_register_direct(operand)){
            result=REGISTER_DIRECT;
        } else{
            result=DIRECT;
        }
    }
    return result;
}

int is_register_direct(char *operand) {
    return get_register_by_name(operand)!=NULL;
}

int is_index(char *operand) {
    int result=0,len,left_diteceted=0,i=0;
    operand=trim_whitespace(operand);
    len= strlen(operand);
    while ((result==0)&&(i<len)){
        if ((operand[i]==RIGHT_BRAKET)&&(left_diteceted)){
            result=1;
        }
        if(operand[i]==LEFT_BRAKET){
            left_diteceted=1;
        }
        i++;
    }
    return result;
}

int is_immediate(char *operand) {
    trim_whitespace(operand);
    return *operand==IMMEDIATE_SIGN;
}

void set_operator(instruction to_set, char *name, unsigned long *ic, error *err) {
    to_set->op= get_operator_by_name(name);
    if(to_set->op==NULL){
        *err=UNDEFINED_OPERATOR;
    } else{
        int opcode;
        to_set->words[OP_WORD_INDEX]=init_word();
        opcode= get_opcode(to_set->op);
        set_address(to_set->words[OP_WORD_INDEX], *ic);
        (*ic)++;
        set_opcode(to_set->words[OP_WORD_INDEX], opcode);
        set_are(to_set->words[OP_WORD_INDEX], ABSOLUTE);
    }
}