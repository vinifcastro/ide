#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void escreveoarquivo(){
    FILE * x = fopen("codigo.txt", "r");
    char aux = fgetc(x);
    while(aux!=EOF){
        printf("%c", aux);
        aux = fgetc(x);
    }
}

int main() {
    char input, aux = '|', ant = '|';
    FILE* backup = NULL, * help = NULL;
    int i = 1, j, k;
    backup = fopen("codigo.txt", "w");
    fprintf(backup, "IDE EM C - PARA PARAR DE DIGITAR O CODIGO UTILIZE O CARACTERE '|'.\n%03d - ", i);
    fclose(backup);
    do {
        system(CLEAR);
        escreveoarquivo();
        backup = fopen("codigo.txt", "a+");
        input = _getch();
        switch (input) {
            case 13:
                fprintf(backup, "\n");
                i++;
                fprintf(backup, "%03d - ", i);
                break;
            case 8:
                help = fopen("help.txt", "w");
                fseek(backup,-1-i,SEEK_END);
                k = ftell(backup);
                fseek(backup,0,SEEK_SET);
                for(j=0; j<k; j++) {
                    aux = fgetc(backup);
                    fprintf(help, "%c", aux);
                }
                fclose(help);
                fclose(backup);

                backup = fopen("codigo.txt", "w");
                help = fopen("help.txt", "r");
                aux = fgetc(help);
                while(aux != EOF){ 
                    fprintf(backup, "%c", aux);
                    aux = fgetc(help);
                }
                fclose(backup);
                backup = fopen("codigo.txt", "a+");
                fclose(help);
                remove("help.txt");
                break;
            default:
                if (input != '|')
                    fprintf(backup, "%c", input);
                break;
        }
        
        fclose(backup);
        ant = input;
    } while (input != '|');

    return 0;
}
