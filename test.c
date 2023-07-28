#include <stdlib.h>
#include<stdio.h>
#include <string.h>

char* inc(char *num, char* p)//https://stackoverflow.com/questions/891715/recursive-incrementer
{   
    if( *p>='0' && *p<='8' )
    {
        (*p)++;       //==> change
    }
    else if ( *p=='9' )
    {
        *p = '0';
        inc(num, --p);
    }

    return num;
}

int main(){
    char code_init_message[9];
    strncpy(code_init_message,"10",2);
    strncpy(code_init_message+2,"100032",6);
    printf("User code sent: %s",code_init_message);
    return 0;
}