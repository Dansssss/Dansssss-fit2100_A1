#include <stdlib.h>
#include <string.h>

char* gets(char *buf){ 
    int c, stdin, EOF; 
    char *s; 

    FLOCKFILE(stdin); ORIENT(stdin, -1); 
 
    for (s = buf; (c = __sgetc(stdin)) != '\n';) 
        if (c == EOF) 
            if (s == buf) {
                 FUNLOCKFILE(stdin); return (NULL); 
            } else 
                break; 
        else 
            *s++ = c; *s = 0; 
        FUNLOCKFILE(stdin); return (buf); 
} 
