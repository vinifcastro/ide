// g++ -g -Wall ide.cpp -o ide

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct Pilha{
    int valor;
    struct Pilha *proximo;
}Pilha;

void escreveoarquivo(){
    system("clear");
    FILE * x = fopen("codigo.txt", "r");
    char aux = fgetc(x);
    while(aux!=EOF){
        printf("%c", aux);
        aux = fgetc(x);
    }
}

int empilha_num(Pilha ** p, int n){

    Pilha *novo = NULL;
    novo = (Pilha *) malloc(sizeof(Pilha));
    if(novo){
        novo->proximo = (*p);
        novo->valor = n;
        (*p) = novo;
        return 0;
    }
    else return -1;
}

int desempilha_num(Pilha ** p){
    Pilha * aux = NULL;
    if(*p){
        aux = *p;
        *p = aux->proximo;
        return aux->valor;
        free(aux);
    }
    else return -1;
}

void printalinhasnaofechadas(Pilha * p){
    Pilha * aux = NULL;
    aux = p;
    while(1){
        if(aux == NULL) break;
        printf("%d ", aux->valor);
        aux = aux->proximo;
    }
}

void libera_pilha(Pilha ** fila){
    Pilha * aux;
    if (*fila == NULL){
        free(*fila);
        return;
    }
    aux = *fila;
    while((*fila)->proximo){
        *fila = (*fila)->proximo;
        free(aux);
        aux = *fila;
    }
    aux = *fila;
    free(aux);
}

int main() {
    char input = '.', aux = '|', anterior = '|';
    Pilha *parenteses = NULL, *maiormenor = NULL, *chaves = NULL;
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

        if ( (input == '{') || (input == '}') || (input == '<') || (input == '>') || (input == '(') || (input == ')') ){
            if( (input == '{') || (input == '<') || (input == '(') ){
                switch(input){
                    case '(':
                        empilha_num(&parenteses, i);
                        break;
                    case '<':
                        empilha_num(&maiormenor, i);
                        break;
                    default:
                        empilha_num(&chaves, i);
                        break;
                }
                    fprintf(backup, "%c", input);
            }
            else{
                switch(input){
                    case ')':
                        k= desempilha_num(&parenteses);
                        if(k == -1){ 
                            printf("\n\nErro de digitacao, voce tentou fechar um parenteses ')' sem abri-lo antes.\n\n");
                            input = '|';
                        }
                        system("pause");
                        break;
                    case '>':
                        k= desempilha_num(&maiormenor);
                        if(k == -1){ 
                            printf("\n\nErro de digitacao, voce tentou fechar um '>' sem abri-lo antes.\n\n");
                            input = '|';
                        }
                        system("pause");
                        break;
                    default:
                        k= desempilha_num(&chaves);
                        if(k == -1){ 
                            printf("\n\nErro de digitacao, voce tentou fechar uma chave '}' sem abri-la antes.\n\n");
                            input = '|';
                        }
                        system("pause");
                        break;
                }
                if(k != -1)
                    if (input != '|')
                        fprintf(backup, "%c", input);
            }
        }
        else{
            switch (input) {
                case '\n':
                    fprintf(backup, "\n");
                    i++;
                    fprintf(backup, "%03d - ", i);
                    break;
                case 127:
                    if( (anterior == '{') || (anterior == '(') || (anterior == '<') ){
                        switch(anterior){
                            case '{':
                                k = desempilha_num(&chaves);
                                break;
                            case '(':
                                k = desempilha_num(&parenteses);
                                break;
                            default:
                                k = desempilha_num(&maiormenor);
                                break;
                        }
                    }
                    if( (anterior == '}') || (anterior == ')') || (anterior == '>') ){
                        switch(anterior){
                            case '}':
                                empilha_num(&chaves, i-1);
                                break;
                            case ')':
                                empilha_num(&parenteses, i-1);
                                break;
                            default:
                                empilha_num(&maiormenor, i-1);
                                break;
                        }
                    }
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
        }
        anterior = input;
        fclose(backup);     
        
    } while (input != '|');


    if(parenteses != NULL || maiormenor != NULL || chaves != NULL) printf("\n\n\tERRO DE COMPILACAO:\n\n");
    
    if(parenteses != NULL){
        printf("\n\nParenteses '(' abertos e nao fechados nas linhas: ");
        printalinhasnaofechadas(parenteses);
        printf("\n\n");
    }
    if(maiormenor != NULL){
        printf("\n\nMaiores '<' abertos e nao fechados nas linhas: ");
        printalinhasnaofechadas(maiormenor);
        printf("\n\n");
    }
    if(chaves!= NULL){
        printf("\n\nChaves '{' abertos e nao fechados nas linhas: ");
        printalinhasnaofechadas(chaves);
        printf("\n\n");
    }

    if(parenteses == NULL && maiormenor == NULL && chaves == NULL) printf("\n\nCompilacao executada com sucesso.\n\n");

    libera_pilha(&parenteses);
    libera_pilha(&maiormenor);
    libera_pilha(&chaves);
    return 0;
}
