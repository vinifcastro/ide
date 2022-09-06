// g++ -g -Wall ide.cpp -o ide

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

void escreveoarquivo(){
    system("clear");
    FILE * x = fopen("codigo.txt", "r");
    char aux = fgetc(x);
    while(aux!=EOF){
        printf("%c", aux);
        aux = fgetc(x);
    }
}

int main() {
    char input = '.', aux = '|', anterior = '|';
    FILE* backup = NULL, * help = NULL;
    int i = 1, j, k;
    backup = fopen("codigo.txt", "w");
    fprintf(backup, "\n\n\tIDE EM C - PARA PARAR DE DIGITAR O CODIGO UTILIZE O CARACTERE '|'.\n%03d - ", i);
    fclose(backup);
    escreveoarquivo();
    do {
        backup = fopen("codigo.txt", "a+");
        printf("%c", anterior);
        escreveoarquivo();
        input = getch();
        
        switch (input) {
            case '\n':
                fprintf(backup, "\n");
                i++;
                fprintf(backup, "%03d - ", i);
                break;
            case 127:
                help = fopen("help.txt", "w");
                fseek(backup,-1,SEEK_END);
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
        anterior = input;
        fclose(backup);     
        
    } while (input != '|');

    return 0;
}
