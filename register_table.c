/*
** Created by Avishav & Sapir on March 10,2022
*/

#include <string.h>
#include "register_table.h"
#include "string_manipulations.h"

#define REG_TABLE_SIZE 16

struct reg {
    char *name;
    int reg_code;
    boolean valid_index :1; /*true if this register can be index, false otherwise*/
};

regyster get_register_by_name(char *name){
    int i;
    static struct reg registers[]={
            {"r0",0,false},
            {"r1",1,false},
            {"r2",2,false},
            {"r3",3,false},
            {"r4",4,false},
            {"r5",5,false},
            {"r6",6,false},
            {"r7",7,false},
            {"r8",8,false},
            {"r9",9,false},
            {"r10",10,true},
            {"r11",11,true},
            {"r12",12,true},
            {"r13",13,true},
            {"r14",14,true},
            {"r15",15,true}
    };
    name=trim_whitespace(name);
    for (i=0 ; i < REG_TABLE_SIZE ; ++i){
        if(!strcmp(registers[i].name,name)){
            return registers+i;
        }
    }
    return NULL;
}

int get_reg_code(regyster reg){
    return reg->reg_code;
}

boolean get_is_valid_index(regyster reg){
    return reg->valid_index;
}

boolean is_valid_index(regyster reg){
    return reg->valid_index;
}
