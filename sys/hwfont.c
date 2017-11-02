#include <sys/hwfont.h>
#include <sys/defs.h>
#include <sys/homework.h>
#include "stdarg.h"

static int x=0, y=0;
int color = 12;

void scrollUp(){
	uint8_t *VGA = (uint8_t *)0xA0000;
	for (int i=0; i<320*16*11; i++){
		VGA[i] = VGA[i+320*16];
	}
	for (int i=320*16*11; i<320*16*12; i++){
		VGA[i] = 0;
	}
}

void hwfont(const char *fmt, ...){
	const char *temp1;
	va_list valist;
	va_start(valist, fmt);

	for (temp1 = fmt; *temp1; temp1+=1){
		if (x == 40*8){
			x = 0;
			y += 16;
		}
		if (y == 12*16){
			y = 11*16;
			scrollUp();			
		}

		if(*temp1 == '\n'){
			x = 0;
			y += 16;
			continue;
		}
		else if(*temp1 != '%'){
			PrintACharacter(*temp1, x, y, color);
			x += 8;
		}
		else {
			temp1 += 1;
			if (*temp1 == 'c') {
				PrintACharacter(va_arg(valist, int), x, y, color);
				x+= 8;
			}
			else if (*temp1 == 'd') {
				int input = va_arg(valist, int);
                                int a, i=0; char intStr[10];
                                while(input > 0){
                                        a = input % 10;
                                        intStr[i++] = (char)a + '0';
                                        input = input/10;
                                }
                                intStr[i] = '\0';
                                i--;

                                char printStr[10];
                                int k=0;
                                while(i>=0){
                                        printStr[k++] = intStr[i--];
                                }
                                printStr[k] = '\0';

                                for(i=0; printStr[i]!='\0'; i++){
					if (x == 8*40){x=0; y+=16;}
					PrintACharacter(printStr[i], x, y, color);
					x += 8;
                                }
			}
			else if (*temp1 == 's') {
				char *input = va_arg(valist, char*);
                                while(*input){
					if (x == 8*40){x=0; y+=16;}
					PrintACharacter(*input, x, y, color);
					x += 8;
                                        input += 1;
                                }
			}
			else if (*temp1 == 'x') {
				unsigned int input= va_arg(valist, int);
                                int a, i=0;
                                char str[1024];
                                while(input > 0){
                                        a = input%16;
                                        input = input/16;
                                        if(a<10) str[i++] = (char)(a+48);
                                        else if(a==10) str[i++] = 'a';
                                        else if(a==11) str[i++] = 'b';
                                        else if(a==12) str[i++] = 'c';
                                        else if(a==13) str[i++] = 'd';
                                        else if(a==14) str[i++] = 'e';
                                        else  str[i++] = 'f';
                                }
                                i--;

				char revStr[1024]; int k=0;
                                while(i>=0){
                                        revStr[k++] = str[i--];
                                }
                                revStr[k] = '\0';

                                for(i=0; revStr[i] != '\0'; i++){
					if (x == 8*40){x=0; y+=16;}
					PrintACharacter(revStr[i], x, y, color);
					x += 8;
                                }
			}
			else if (*temp1 == 'p'){
				unsigned int input= va_arg(valist, int);
                                int a, i=0;
                                char str[1024];
                                while(input > 0){
                                        a = input%16;
                                        input = input/16;
                                        if(a<10) str[i++] = (char)(a+48);
                                        else if(a==10) str[i++] = 'a';
                                        else if(a==11) str[i++] = 'b';
                                        else if(a==12) str[i++] = 'c';
                                        else if(a==13) str[i++] = 'd';
                                        else if(a==14) str[i++] = 'e';
                                        else  str[i++] = 'f';
                                }
				str[i++] = 'x'; str[i++] = '0';
				i--;

                                char revStr[1024]; int k=0;
                                while(i>=0){
                                        revStr[k++] = str[i--];
                                }
                                revStr[k] = '\0';

                                for(i=0; revStr[i] != '\0'; i++){
					if (x == 8*40) {x=0; y+=16;}
					PrintACharacter(revStr[i], x, y, color);
					x += 8;
                                }
			}

		}
	}
}
