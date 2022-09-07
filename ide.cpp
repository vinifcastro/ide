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
    char input = '.', aux = '|', anterior = '|', z;
    Pilha *parenteses = NULL, *chaves = NULL;
    FILE* backup = NULL, * help = NULL;
    int i = 1, j, k, erro = 0;
    backup = fopen("codigo.txt", "w");
    fprintf(backup, "\n\n\tIDE EM C - PARA PARAR DE DIGITAR O CODIGO UTILIZE O CARACTERE '|'.\n%03d - ", i);
    fclose(backup);
    escreveoarquivo();
    do {
        backup = fopen("codigo.txt", "a+");
        printf("%c", anterior);
        escreveoarquivo();
        input = getch();

        if ( (input == '{') || (input == '}') || (input == '(') || (input == ')') ){
            if( (input == '{')|| (input == '(') ){
                switch(input){
                    case '(':
                        empilha_num(&parenteses, i);
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
                            system("clear");
                            printf("\n\nErro de digitacao, voce tentou fechar um parenteses ')' sem abri-lo antes.\n\n");
                            erro++;
                            input = '|';
                        }
                        break;
                    default:
                        k= desempilha_num(&chaves);
                        if(k == -1){ 
                            system("clear");
                            printf("\n\nErro de digitacao, voce tentou fechar uma chave '}' sem abri-la antes.\n\n");
                            erro++;
                            input = '|';
                        }
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
                    if(anterior != '{' && anterior != '>' && anterior != '}' && anterior != '\n' && anterior != ';'){
                        system("clear");
                        printf("\nErro na linha %d, faltou um ';'.\n", i);
                        erro++;
                        input = '|';
                    }
                    fprintf(backup, "\n");
                    i++;
                    fprintf(backup, "%03d - ", i);
                    break;
                case 127:
                    if( (anterior == '{') || (anterior == '(') ){
                        switch(anterior){
                            case '{':
                                k = desempilha_num(&chaves);
                                break;
                            default:
                                k = desempilha_num(&parenteses);
                                break;
                        }
                    }
                    if( (anterior == '}') || (anterior == ')') ){
                        switch(anterior){
                            case '}':
                                empilha_num(&chaves, i-1);
                                break;
                            default:
                                empilha_num(&parenteses, i-1);
                                break;
                        }
                    }
                    fseek(backup,-2,SEEK_END);
                    aux = fgetc(backup);
                    help = fopen("help.txt", "w");
                    if(aux == '-'){
                        fseek(backup,-7,SEEK_END);
                        i--;
                    }
                    else fseek(backup,-1,SEEK_END);
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
                case 'i':
                    if(anterior == '\n' || anterior == ';'){
                        system("clear");
                        z = 'z';
                        while(z != 'n' && z != 'N' && z != 's' && z != 'S'){
                            printf("Deseja escrever a funcao 'if' [s/n]?\n");
                            scanf("%c%*c", &z);
                            if(z != 'n' && z != 'N' && z != 's' && z != 'S') printf("Opção selecionada invalida!\n");
                        }

                        if(z == 'n' || z == 'N') fprintf(backup, "%c", input);
                        else{
                            fprintf(backup, "if(");
                            empilha_num(&parenteses, i);
                            input = '(';
                        }
                    }
                    else {
                        if (input != '|')
                            fprintf(backup, "%c", input);
                    }
                    break;
                case 'b':
                    if(anterior == '\n' || anterior == ';'){
                        system("clear");
                        z = 'z';
                        while(z != 'n' && z != 'N' && z != 's' && z != 'S'){
                            printf("Deseja escrever a funcao 'break' [s/n]?\n");
                            scanf("%c%*c", &z);
                            if(z != 'n' && z != 'N' && z != 's' && z != 'S') printf("Opção selecionada invalida!\n");
                        }

                        if(z == 'n' || z == 'N') fprintf(backup, "%c", input);
                        else{
                            fprintf(backup, "break;");
                            input = ';';
                        }
                    }
                    else {
                        if (input != '|')
                            fprintf(backup, "%c", input);
                    }
                    break;
                case 's':
                    if(anterior == '\n' || anterior == ';'){
                        system("clear");
                        z = 'z';
                        while(z != 'n' && z != 'N' && z != 's' && z != 'S'){
                            printf("Deseja escrever a funcao 'switch' [s/n]?\n");
                            scanf("%c%*c", &z);
                            if(z != 'n' && z != 'N' && z != 's' && z != 'S') printf("Opção selecionada invalida!\n");
                        }

                        if(z == 'n' || z == 'N') fprintf(backup, "%c", input);
                        else{
                            fprintf(backup, "switch(");
                            empilha_num(&parenteses, i);
                            input = '(';
                        }
                    }
                    else {
                        if (input != '|')
                            fprintf(backup, "%c", input);
                    }
                    break;
                case 'e':
                    if(anterior == '\n' || anterior == ';' || anterior == '}'){
                        system("clear");
                        z = 'z';
                        while(z != 'n' && z != 'N' && z != 's' && z != 'S'){
                            printf("Deseja escrever a funcao 'else' [s/n]?\n");
                            scanf("%c%*c", &z);
                            if(z != 'n' && z != 'N' && z != 's' && z != 'S') printf("Opção selecionada invalida!\n");
                        }

                        if(z == 'n' || z == 'N') fprintf(backup, "%c", input);
                        else{
                            fprintf(backup, "else {");
                            empilha_num(&chaves, i);
                            input = '{';
                        }
                    }
                    else {
                        if (input != '|')
                            fprintf(backup, "%c", input);
                    }
                    break;
                case 'f':
                    if(anterior == '\n' || anterior == ';'){
                        system("clear");
                        z = 'z';
                        while(z != 'n' && z != 'N' && z != 's' && z != 'S'){
                            printf("Deseja escrever a funcao 'for' [s/n]?\n");
                            scanf("%c%*c", &z);
                            if(z != 'n' && z != 'N' && z != 's' && z != 'S') printf("Opção selecionada invalida!\n");
                        }

                        if(z == 'n' || z == 'N') fprintf(backup, "%c", input);
                        else{
                            fprintf(backup, "for(");
                            empilha_num(&parenteses, i);
                            input = '(';
                        }
                    }
                    else {
                        if (input != '|')
                            fprintf(backup, "%c", input);
                    }
                    break;
                case 'r':
                    if(anterior == '\n' || anterior == ';'){
                        system("clear");
                        z = 'z';
                        while(z != 'n' && z != 'N' && z != 's' && z != 'S'){
                            printf("Deseja escrever a funcao 'return' [s/n]?\n");
                            scanf("%c%*c", &z);
                            if(z != 'n' && z != 'N' && z != 's' && z != 'S') printf("Opção selecionada invalida!\n");
                        }

                        if(z == 'n' || z == 'N') fprintf(backup, "%c", input);
                        else{
                            fprintf(backup, "return");
                        }
                    }
                    else {
                        if (input != '|')
                            fprintf(backup, "%c", input);
                    }
                    break;
                case 'w':
                    if(anterior == '\n' || anterior == ';'){
                        system("clear");
                        z = 'z';
                        while(z != 'n' && z != 'N' && z != 's' && z != 'S'){
                            printf("Deseja escrever a funcao 'while' [s/n]?\n");
                            scanf("%c%*c", &z);
                            if(z != 'n' && z != 'N' && z != 's' && z != 'S') printf("Opção selecionada invalida!\n");
                        }

                        if(z == 'n' || z == 'N') fprintf(backup, "%c", input);
                        else{
                            fprintf(backup, "while(");
                            empilha_num(&parenteses, i);
                            input = '(';
                        }
                    }
                    else {
                        if (input != '|')
                            fprintf(backup, "%c", input);
                    }
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


    if(parenteses != NULL || chaves != NULL) printf("\n\n\tERRO DE COMPILACAO:\n\n");
    
    if(parenteses != NULL){
        printf("\n\nParenteses '(' abertos e nao fechados nas linhas: ");
        printalinhasnaofechadas(parenteses);
        printf("\n\n");
    }
    if(chaves!= NULL){
        printf("\n\nChaves '{' abertos e nao fechados nas linhas: ");
        printalinhasnaofechadas(chaves);
        printf("\n\n");
    }

    if(parenteses == NULL && chaves == NULL && erro == 0 ) printf("\n\nCompilacao executada com sucesso.\n\n");

    libera_pilha(&parenteses);
    libera_pilha(&chaves);
    return 0;
}