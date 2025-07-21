#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct{
    char p[20];
    char dica[100];
}palavra;


char novoJogo(){
    char resposta;
    printf("Deseja adicionar um novo jogo? s/n: ");
    scanf(" %c", &resposta); 
    return resposta;
}

int verificarDiferenca(char *palavraAtual, char *palavraAnterior){
    if(strlen(palavraAtual) == strlen(palavraAnterior)){
        int cont = 0;
        for(int i =0; i< strlen(palavraAtual); i++){
            if(tolower(palavraAtual[i]) != tolower(palavraAnterior[i])){
                cont++;
            }
        }
        if(cont==1){
            return 1;
        }
    }
    return 0;
    
}
int main(){
    FILE *file = fopen("dicas.txt", "r+");

    if(file == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int jogo;
    fscanf(file,"indice atual: %03d\n", &jogo);
    while(1){
        int newGame = novoJogo();

        if(newGame == 's' || newGame == 'S'){
            jogo++;
            //organizando o indice do jogo
            rewind(file);
            fprintf(file, "indice atual: %03d\n", jogo);
            fseek(file, 0, SEEK_END);

            char palavraAnterior[20] = "";
            fprintf(file, "%d\n", jogo);
            for(int i =0; i<=4; i++){
                palavra p;
                if(i!=0){
                    printf("Digite a dica para a proxima palavra: ");
                    scanf(" %[^\n]", p.dica);
                }
                printf("Digite a palavra: ");
                scanf(" %[^\n]", p.p);
                if(i==0 || verificarDiferenca(p.p, palavraAnterior)){
                    printf("Palavra aceita!\n");
                    
                    if(i!=0){
                        fprintf(file,"%s\n", p.dica);
                    }
                    
                    fprintf(file,"%s\n", p.p);
                 }
                    
                else {
                    printf("A palavra deve ter uma letra de diferença com a anterior. Tente novamente\n");
                    i--;
                    continue;
                }
                strcpy(palavraAnterior, p.p);
            }
            fprintf(file, "#\n");
            
        }
        else{
            printf("Obrigado por adicionoar jogos!\n");
            fclose(file);
            break;
        }
    }


    return 0;
}