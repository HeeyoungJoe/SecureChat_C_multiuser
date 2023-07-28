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
    char num[10]="001";
    printf("%s",inc(num,num+2));
    return 0;
}