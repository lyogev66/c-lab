/* This code is public domain -- Will Hartung 4/9/09 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr =(char*) malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr =(char*) realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

char* strcasestr(char* str1, const char* str2)
{
	char* p1 = str1;
    const char* p2 = str2;
    char* r = *p2 == 0 ? str1 : 0;

    while(*p1 != 0 && *p2 != 0)
    {
        if (tolower(*p1)==tolower(*p2)) {
            if (r==0)
                r=p1;
            p2++;
		}
		else {
            p2 = str2;
            if (tolower(*p1)==tolower(*p2)) {
                r = p1;
                p2++;
            }
            else
				r=0;
		}
        p1++ ;
    }
    return *p2 == 0 ? r : 0;
}
/*
int main(int argc, char** args) {
    char *buf = NULL; 
    int bufSize = 0; 

    printf("%d\n", bufSize);
    int charsRead =  getline(&buf, &bufSize, stdin);

    printf("'%s'", buf);
    printf("%d\n", bufSize);
    return 0;
}
*/
