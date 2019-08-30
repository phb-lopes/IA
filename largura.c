/*
  main.c
  grafo_02

  Created by Paulo Henrique on 03/09/16.
  Copyright Â© 2016 Paulo Henrique. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define inf 999999999

//Estrutura das Arestas;
struct aresta{
    int rotulo;
    int peso;
    int destino;
    struct aresta* prox;
};

struct estimativa{
	int origem;
	int estima;
	int destino;
	struct estimativa *prox;
};

//Estrutura dos Vertices;
struct vertice{
    int num;
    int estimativa;
    int aberto;
    struct aresta *prox_aresta;
    struct vertice *prox_vertice;
};
int cont_total = 0;

struct estimativa *inserir_estimativa(struct estimativa * estimativa, int origem, int destino, int distancia){
	struct estimativa *v1;
	struct estimativa *aux;
	
	if(estimativa == NULL){
        v1 = (struct estimativa*)malloc(sizeof(struct estimativa));
        if(v1 == NULL)
            return estimativa;

        v1->origem = origem;
        v1->destino = destino;
        v1->estima = distancia;
        v1->prox = NULL;
        estimativa = v1;
        
        return estimativa;
    }else{ 
       	aux = (struct estimativa*)malloc(sizeof(struct estimativa));
       	
        if(aux == NULL)
        	return estimativa;
        
        aux->origem = origem;
        aux->destino = destino;
        aux->estima = distancia;

        aux->prox = estimativa;

        estimativa = aux;
    }
    return estimativa;
}

struct vertice *inserir_vertice(struct vertice *grafo,int num, int distancia){
    struct vertice *v1;
    struct vertice *aux;
    struct vertice *procura;
    //Se o Grafo for Nulo, aloca um vertice
    if(grafo == NULL){
        v1 = (struct vertice*)malloc(sizeof(struct vertice));
        if(v1 == NULL)
            return grafo;
        v1->num = num;
        v1->prox_vertice = NULL;
        v1->prox_aresta = NULL;
        v1->estimativa = distancia;
        v1->aberto = 0;
        grafo = v1;
    }else{
        //Se o Vertice j‡ existir, n‹o deixa adicionar outro vertice;
        for(procura = grafo; procura != NULL; procura = procura->prox_vertice){
            if(num == (procura)->num){
                printf("Vertice existente! Por favor insira outro\n");
                return grafo;
            }
        }
        //Se o Grafo n‹o tiver vazio, e o vertice n‹o existir no Grafo, aloca o vertice na ordem crescente;
        if(num < (grafo)->num){
            aux = grafo;
            v1 = (struct vertice*)malloc(sizeof(struct vertice));
            if(v1 == NULL)
                return grafo;
            
            v1->num = num;
            v1->prox_vertice = aux;
            v1->estimativa = distancia;
            v1->aberto = 0;
            aux->prox_aresta = NULL;
            grafo = v1;
            return grafo;
        }
        
        if(num > (grafo)->num){
            aux = grafo;
            v1 = (struct vertice*)malloc(sizeof(struct vertice));
            if(v1 == NULL)
                return grafo;
            v1->num = num;
            v1->estimativa = inf;
            v1->aberto = 0;
            v1->prox_aresta = NULL;
            
            while(aux->prox_vertice != NULL && num > aux->prox_vertice->num){
                aux = aux->prox_vertice;
            }
            v1->prox_vertice = aux->prox_vertice;
            aux->prox_vertice = v1;
        }
    }
    return grafo;
}

struct vertice *buscar_vertice(struct vertice *grafo, int origem){
    struct vertice *busca = grafo;
    //Busca um vertice no grafo, se o grafo for nulo, diz q o Grafo ta vazio;
    if(grafo == NULL){
        printf("Lista vazia!\n");
        return grafo;
    }
    //Se o numero desejado for o primeiro, retorna o proprio grafo;
    if(grafo->num == origem){
        return grafo;
    }
    //Se n‹o for o primeiro, vai de vertice em vertice na procura dele;
    while(busca->prox_vertice != NULL){
        if(busca->prox_vertice->num == origem){
            return busca->prox_vertice;
        }
        busca = busca->prox_vertice;
    }
    return NULL;
}

struct vertice *inserir_aresta(struct vertice *grafo, int origem, int destino, int valor_aresta, int peso){
    struct aresta *a1;
    struct aresta *a2;
    struct vertice *procurar_origem;
    struct vertice *procurar_destino;
    struct vertice *aux_procurar;
    aux_procurar = grafo;
    
    //Usa a fun‹o buscar_vertice pra saber se o vertice exite na origem e no destino que deseja inserir o vertice;
    
    procurar_origem = buscar_vertice(grafo, origem);
    if(procurar_origem == NULL){
        printf("Vertice de Origem inxistente!\n");
    }
    else{
        procurar_destino = buscar_vertice(grafo, destino);
        
        if(procurar_destino == NULL){
            printf("Vertice de Destino inxistente!\n");
        }else{
            if(procurar_origem->prox_aresta == NULL || procurar_origem->prox_aresta->destino > procurar_destino->num){
                
                //Se o vertice n‹o tiver arestas, insere Aresta aqui, como a primeira aresta;
                
                a1 = (struct aresta*)malloc(sizeof(struct aresta));
                if(a1 == NULL)
                    return grafo;
                
                a1->rotulo = valor_aresta;
                a1->peso = peso;
                a1->destino = procurar_destino->num;
                a1->prox = procurar_origem->prox_aresta;
                procurar_origem->prox_aresta = a1;
            }else{
                //Se j‡ existir uma aresta, a proxima ser‡ alocada na ordem Crescente;
                a1 = (struct aresta*)malloc(sizeof(struct aresta));
                if(a1 == NULL)
                    return grafo;
                
                a1->rotulo = valor_aresta;
                a1->peso = peso;
                a1->destino = procurar_destino->num;
                
                a2 = procurar_origem->prox_aresta;
                while(a2->prox != NULL && a2->prox->destino < procurar_destino->num)
                    a2 = a2->prox;
                
                a1->prox = a2->prox;
                a2->prox = a1;
            }
            
        }
    }
    
    return grafo;
}

struct vertice *remover_aresta(struct vertice *grafo, int num){
    struct vertice *i;
    struct aresta *v1_before,*v2_next,*aux;
    
    if(grafo == NULL)
        return grafo;
    //Aqui, faz a procura das Arestas com R—tulo que o usu‡rio solicitou, o For vai de vertice em Vertice;
    for(i = grafo; i != NULL ; i = i->prox_vertice){
        //Se esse i->prox_aresta for Nulo, significa que n‹o existe arestas no Vertice;
        if(i->prox_aresta != NULL) {
            v1_before = i->prox_aresta;
            //Enquanto o vertice n‹o for nulo, procura o rotulo;
            while(v1_before != NULL){
                //Se o rotulo for o primeiro que se deseja apagar, exclui a Aresta em quest‹o;
                if(v1_before->rotulo == num) {
                    printf("As arestas com Rotulo %d foram removidas do Vertice %d\n",v1_before->rotulo,i->num);
                    i->prox_aresta = i->prox_aresta->prox;
                    free(v1_before);
                    v1_before = i->prox_aresta;
                } else
                    break;
            }
            //Faz uma procura nas demais arestas;
            if(i->prox_aresta != NULL)
                for(v1_before = i->prox_aresta, v2_next = i->prox_aresta->prox; v2_next != NULL;){
                    if(v2_next->rotulo == num){
                        printf("As arestas com Rotulo %d foram removidas do Vertice %d\n",v2_next->rotulo,i->num);
                        v1_before->prox = v2_next->prox;
                        aux = v2_next;
                        v2_next = v2_next->prox;
                        free(aux);
                    }else
                        v2_next = v2_next->prox;
                }// Fecha o for de dentro;
        }// Fecha IF;
    }// Fecha outro for;
    return grafo;
}

struct vertice *remover_vertice(struct vertice *grafo, int num){
    struct vertice *before = NULL;
    struct vertice *next;
    struct vertice *v1,*i,*achei;
    struct aresta *v1_before,*v2_next,*aux;
    
    if(grafo == NULL)
        return grafo;
    v1 = grafo;
    
    achei = buscar_vertice(grafo, num);
    
    if(achei == NULL){
        printf("Vertice nao existe\n");
        return 0;
    }
    
    //Aqui, remove primeiro as Arestas com Liga›es para os Vertices;
    
    for(i = grafo; i != NULL ; i = i->prox_vertice){
        if(i->prox_aresta != NULL) {
            v1_before = i->prox_aresta;
            while(v1_before != NULL){
                if(v1_before->destino == num) {
                    printf("As arestas com Rotulo %d foram removidas do Vertice %d\n",v1_before->rotulo,i->num);
                    i->prox_aresta = i->prox_aresta->prox;
                    free(v1_before);
                    v1_before = i->prox_aresta;
                } else
                    break;
            }
            if(i->prox_aresta != NULL)
                for(v1_before = i->prox_aresta, v2_next = i->prox_aresta->prox; v2_next != NULL;){
                    if(v2_next->destino == num){
                        printf("As arestas com Rotulo %d foram removidas do Vertice %d\n",v2_next->rotulo,i->num);
                        v1_before->prox = v2_next->prox;
                        aux = v2_next;
                        v2_next = v2_next->prox;
                        free(aux);
                    } else
                        v2_next = v2_next->prox;
                }// Fecha o for de dentro;
        }// Fecha IF;
    }// Fecha outro for;
    
    //Depois de Remover as Arestas, remove o Vertice;
    next = grafo->prox_vertice;
    before = grafo;
    
    if(grafo->num == num){
        printf("O numero %d foi removido com sucesso\n",grafo->num);
        grafo = next;
        free(before);
        return grafo;
    }
    
    while(next != NULL && next->num != num){
        before = next;
        next = next->prox_vertice;
    }
    
    if (next == NULL){
        printf("\nNumero nÃ£o encontrado!\n");
        return grafo;
    }
    
    before->prox_vertice = next->prox_vertice;
    free(next);
    printf("\nO numero %d foi removido com sucesso\n",num);
    return grafo;
}

void imprimir(struct vertice *grafo){
    struct vertice *i;
    struct aresta *j;
    //Se o Grafo for nulo
    if(grafo == NULL) {
        printf("GRAFO VAZIO\n");
        return;
    }
    //Dois for, um pra varrer os vertices e um pras Arestas;
    printf("Vertices:");
    for(i = grafo; i != NULL ; i = i->prox_vertice){
        printf("\n%d->", i->num);
        for(j = i->prox_aresta; j != NULL; j = j->prox){
            printf("%d{p:%d}->",j->destino,j->peso);
        }
    }
    printf("\n");
}

int busca_largura(struct vertice *grafo, struct estimativa *estimativa){
    struct vertice *i;
    struct aresta *j;
    struct estimativa *d;

    // dois for pra percorrer as duas estruturas(vertice e arestas)
    for ( i = grafo; i != NULL ;i = i->prox_vertice){
        for(j = i->prox_aresta; j != NULL; j = j->prox){
            printf("Numero atual [%d] proximo -> [%d]\n",i->num,j->destino);
            //for pra procurar na estrutura com estimativas qual o valor
            for ( d = estimativa; d != NULL ; d = d->prox){
                //se a estimativa for 0, significa que chegou ao destino
                if(j->destino == d->destino){
                    printf("Destino final [%d]\n",d->destino);
                    cont_total = j->peso;
                    return 1;
                }
            }
        }
    }
    return 0;
}

int main(){
    
    setlocale(LC_ALL,"Portuguese");
    
    int x,cont_aresta = 0,cont_vertice = 0, num = 0,origem,destino,peso,rotulo,comecar,alvo, distancia;
    
    struct vertice *grafo;
    struct estimativa *estimativa;

    estimativa = NULL;
    grafo = NULL;
    
    do{
        printf("Digite:\n1 - Inserir Vertice\n2 - Inserir Aresta\n3 - Remover Vertice\n4 - Remover Aresta\n5 - Arvore de Natal\n6 - Busca em Largura\n7 - Adicionar Estimativas\n0 - Sair\n");
        scanf("%d",&x);
        
        switch(x){
            case 1:{
                printf("Digite o Vertice\n");
                scanf("%d",&num);
                printf("Digite a Distancia pro no final\n");
                scanf("%d",&distancia);
                grafo = inserir_vertice(grafo,num,distancia);
                cont_vertice++;
                
                imprimir(grafo);
                break;
            }
            case 2:{
                printf("Em qual Vertice deseja inserir?\n");
                scanf("%d",&origem);
                printf("Para qual vertice deseja ligar?\n");
                scanf("%d",&destino);
                printf("Qual o valor da aresta?\n");
                scanf("%d",&rotulo);
                printf("Qual o Peso?\n");
                scanf("%d",&peso);
                grafo = inserir_aresta(grafo, origem, destino, rotulo,peso);
                
                cont_aresta++;
                
                imprimir(grafo);
                
                break;
            }
            case 3:{
                printf("Qual Vertice deseja remover?\n");
                scanf("%d",&num);
                grafo = remover_vertice(grafo,num);
                imprimir(grafo);
                break;
            }
            case 4:{
                printf("Qual o rotulo da Aresta deseja remover?\n");
                scanf("%d",&num);
                grafo = remover_aresta(grafo,num);
                imprimir(grafo);
                break;
            }
            case 5:{
                printf("Arvore de NATAL\n");
                grafo = inserir_vertice(grafo, 0,16);
                grafo = inserir_vertice(grafo, 1,9);
                grafo = inserir_vertice(grafo, 2,8);
                grafo = inserir_vertice(grafo, 3,7);
                grafo = inserir_vertice(grafo, 4,6);
                grafo = inserir_vertice(grafo, 5,6);
                grafo = inserir_vertice(grafo, 6,6);
                grafo = inserir_vertice(grafo, 7,13);
                grafo = inserir_vertice(grafo, 8,5);
                grafo = inserir_vertice(grafo, 9,5);
                grafo = inserir_vertice(grafo, 10,8);
                grafo = inserir_vertice(grafo, 11,7);
                grafo = inserir_vertice(grafo, 12,0);
                grafo = inserir_vertice(grafo, 13,0);
                grafo = inserir_vertice(grafo, 14,0);
                grafo = inserir_vertice(grafo, 15,13);
                grafo = inserir_vertice(grafo, 16,2);
                grafo = inserir_vertice(grafo, 17,0);
                grafo = inserir_vertice(grafo, 18,1);
                grafo = inserir_vertice(grafo, 19,1);
                grafo = inserir_vertice(grafo, 20,0);
                grafo = inserir_vertice(grafo, 21,0);
                cont_vertice = 22;
 
                // UERN - NORTSHOPPING
                grafo = inserir_aresta(grafo, 0, 1, 0,1);
                // UERN - PONTE NEWTON
                grafo = inserir_aresta(grafo, 0, 2, 0,5);
                // ------ Fim do Nivel 01 ------

                
                // NORTSHOPPING - PONTE VELHA
                grafo = inserir_aresta(grafo, 1, 3, 0,5);
                // PONTE NEWTON - HOSPITAL
                grafo = inserir_aresta(grafo, 2, 4, 0,10);
                // PONTE NEWTON - CENTRO
                grafo = inserir_aresta(grafo, 2, 5, 0,5);
                // ------ Fim do Nivel 02 ------
                
                
                // PONTE VELHA - KM 6
                grafo = inserir_aresta(grafo, 3, 6, 0,30);
                // PONTE VELHA - BERNARDO1
                grafo = inserir_aresta(grafo, 3, 7, 0,1);
                // HOSPITAL - PRAÇA
                grafo = inserir_aresta(grafo, 4, 8, 0,12);
                // HOSPITAL - PRUDENTE
                grafo = inserir_aresta(grafo, 4, 9, 0,2);
                // CENTRO - DEODORO
                grafo = inserir_aresta(grafo, 5, 10, 0,4);
                // ------ Fim do Nivel 03 ------

                
                // KM6 - RODOVIARIA
                grafo = inserir_aresta(grafo, 6, 11, 0,2);
                // BERNARDO1 - MIDWAY1
                grafo = inserir_aresta(grafo, 7, 12, 0,15);
                // PRACA - MIDWAY2
                grafo = inserir_aresta(grafo, 8, 13, 0,6);
                // PRUDENTE - MIDWAY3
                grafo = inserir_aresta(grafo, 9, 14, 0,7);
                // DEODORO - BERNARDO2
                grafo = inserir_aresta(grafo, 10, 15, 0,3);
                // ------ Fim do Nivel 04 ------


                // RODOVIARIA - ARENA
                grafo = inserir_aresta(grafo, 11, 16, 0,3);
                // BERNARDO2 - MIDWAY 4
                grafo = inserir_aresta(grafo, 15, 17, 0,7);
                // ------ Fim do Nivel 05 ------

                // ARENA - SALGADO
                grafo = inserir_aresta(grafo, 16, 18, 0,1);
                // ARENA - PRUDENTE
                grafo = inserir_aresta(grafo, 16, 19, 0,1);
                // ------ Fim do Nivel 06 ------

                
                // SALGADO - MIDWAY 5
                grafo = inserir_aresta(grafo, 18, 20, 0,2);
                // PRUDENTE - MIDWAY 6
                grafo = inserir_aresta(grafo, 19, 21, 0,2);
                // ------ Fim do Nivel 07 ------

                cont_aresta = 21;
                comecar = 0;
                imprimir(grafo);

                
                // UERN - MIDWAY 1 (12)
                estimativa = inserir_estimativa(estimativa,0,12,16);
                // UERN - MIDWAY 2 (13)
                estimativa = inserir_estimativa(estimativa,0,13,16);
                // UERN - MIDWAY 3 (14)
                estimativa = inserir_estimativa(estimativa,0,14,16);
                // UERN - MIDWAY 4 (17)
                estimativa = inserir_estimativa(estimativa,0,17,15);
                // UERN - MIDWAY 5 (20)
                estimativa = inserir_estimativa(estimativa,0,20,16);
                // UERN - MIDWAY 6 (21)
                estimativa = inserir_estimativa(estimativa,0,21,16);
                // ------ Fim do Nivel 00 ------



         		// NORTSHOPPING - MIDWAY 1 (12)
                estimativa = inserir_estimativa(estimativa,1,12,9);
                // NORTSHOPPING - MIDWAY 5 (20)
                estimativa = inserir_estimativa(estimativa,1,20,9);
                // NORTSHOPPING - MIDWAY 6 (21)
                estimativa = inserir_estimativa(estimativa,1,21,9);
                // PONTE NEWTON - MIDWAY 2 (13)
                estimativa = inserir_estimativa(estimativa,2,13,8);
                // PONTE NEWTON - MIDWAY 3 (14)
                estimativa = inserir_estimativa(estimativa,2,14,8);
                // PONTE NEWTON - MIDWAY 4 (17)
                estimativa = inserir_estimativa(estimativa,2,17,8);
                // ------ Fim do Nivel 01 ------



                // PONTE VELHA - MIDWAY 1 (12)
                estimativa = inserir_estimativa(estimativa,3,12,7);
                // PONTE VELHA - MIDWAY 5 (20)
                estimativa = inserir_estimativa(estimativa,3,20,7);
                // PONTE VELHA - MIDWAY 6 (21)
                estimativa = inserir_estimativa(estimativa,3,21,7);
                // HOSPITAL - MIDWAY 2 (13)
                estimativa = inserir_estimativa(estimativa,4,13,6);
                // HOSPITAL - MIDWAY 3 (14)
                estimativa = inserir_estimativa(estimativa,4,14,6);
                // CENTRO - MIDWAY 4 (17)
                estimativa = inserir_estimativa(estimativa,5,17,6);              
                // ------ Fim do Nivel 02 ------



                // KM6 - MIDWAY 5 (20)
                estimativa = inserir_estimativa(estimativa,6,20,6);
                // KM6 - MIDWAY 6 (21)
                estimativa = inserir_estimativa(estimativa,6,21,6);
                // PRACA - MIDWAY 1 (12)
                estimativa = inserir_estimativa(estimativa,7,12,5);       
                // PRUDENTE - MIDWAY 2 (13)
                estimativa = inserir_estimativa(estimativa,8,13,5);
                // BERNARDO1 - MIDWAY 3 (14)
                estimativa = inserir_estimativa(estimativa,9,14,13);
                // DEODORO - MIDWAY 4 (17)
                estimativa = inserir_estimativa(estimativa,10,17,8);
                // ------ Fim do Nivel 03 ------



                // RODOVIARIA - MIDWAY 1 (12)
                estimativa = inserir_estimativa(estimativa,11,20,8);
                // RODOVIARIA - MIDWAY 2 (13)
                estimativa = inserir_estimativa(estimativa,11,21,8);
                
                // BERNARDO2 - MIDWAY 2 (13)
                estimativa = inserir_estimativa(estimativa,15,17,5);
                // ------ Fim do Nivel 04 ------



                // ARENA - MIDWAY 5 (20)
                estimativa = inserir_estimativa(estimativa,16,20,2);
                // ARENA - MIDWAY 5 (20)
                estimativa = inserir_estimativa(estimativa,16,21,2);
                // ------ Fim do Nivel 05 ------


                // SALGADO - MIDWAY 5 (20)
                estimativa = inserir_estimativa(estimativa,16,20,1);
                // PRUDENTE - MIDWAY 5 (20)
                estimativa = inserir_estimativa(estimativa,15,21,1);
                // ------ Fim do Nivel 06 ------

            	break;
            }
            case 6:{
                busca_largura(grafo,estimativa);
                break;
            }
            case 7:{
            	int num;
				printf("Qual a origem\n");
				scanf("%d",&origem);
				printf("Qual o destino\n");
				scanf("%d",&destino);
				printf("Qual a estimativa\n");
				scanf("%d",&num);
				estimativa = inserir_estimativa(estimativa,origem,destino,num);
            	break;
            }
            case 0:{
                printf("Obrigado!\n");
                return 0;
                break;
            }
                
        }
    }while(x>0);

    return 0;
}
