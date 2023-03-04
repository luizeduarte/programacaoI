#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include "de-burguer.h"


void inicializa_ncurses(){
    initscr();            //inicializa a tela
    raw();                //desabilita o buffer
    noecho();             //nao mostra os caracteres digitados
    curs_set(FALSE);      //não mostra o cursor na tela 
    keypad(stdscr, TRUE); //habilita leitura de setas 
	start_color();		  //habilita cores
	init_pair(1, COLOR_BLACK, COLOR_RED); //cor hamburguer
	init_pair(2, COLOR_BLACK, COLOR_GREEN); //cor salada
	init_pair(3, COLOR_BLACK, COLOR_YELLOW); //cor queijo
	init_pair(4, COLOR_GREEN, COLOR_BLACK); //cor pontuacao
	init_pair(5, COLOR_WHITE, COLOR_MAGENTA); //cor pedidos entregues
	init_pair(6, COLOR_BLACK, COLOR_CYAN); //cor refrigerante
	init_pair(7, COLOR_RED, COLOR_BLACK);	//cor lixeira	
	init_pair(8, COLOR_BLACK, COLOR_WHITE); //cor pao
	init_pair(9, COLOR_BLACK, COLOR_MAGENTA); //cor fritas
}

void inicializa_cardapio(char* cardapio[]){
	//aloca memoria para os vetores de cada refeicao
	cardapio[X_Burguer] = malloc(sizeof(char)*4);
	cardapio[X_Salada] = malloc(sizeof(char)*4);
	cardapio[Combo_1] = malloc(sizeof(char)*6);
	cardapio[Combo_2] = malloc(sizeof(char)*6);
	cardapio[Vegetariano] = malloc(sizeof(char)*5);
	cardapio[Vegano] = malloc(sizeof(char)*3);

	//atribui as receitas
	cardapio[X_Burguer] = "PQHp";
	cardapio[X_Salada] = "PSHp";
	cardapio[Combo_1] = "RFPQHp";
	cardapio[Combo_2] = "RFPSHp";
	cardapio[Vegetariano] = "RFPQp";
	cardapio[Vegano] = "RFS";
}

void imprime_pedido(char* cardapio[], struct fila_clientes* fila){
	//imprime os primeiros 5 pedidos da fila 
	struct pedido* aux = fila->comeco;
	int col = 50, cont = 1, pedido, lin = 1;
	//struct ingredientes* auxIngredientes = refeicao->topo;
	mvprintw(0, 50, "Pedidos na fila:");

	while ((cont <= 5) && (aux != NULL)){
		pedido = aux->num_refeicao;

		switch(pedido){
			case 0:{
				for(int i = 0; i <= strlen(cardapio[aux->num_refeicao]) - 1; i++, lin++)
					imprime_cores(cardapio[0][i], lin, col);
			} break;

			case 1:{
				for(int i = 0; i <= strlen(cardapio[aux->num_refeicao]) - 1; i++, lin++)
					imprime_cores(cardapio[1][i], lin, col);
			} break;

			case 2:{
				for(int i = 0; i <= strlen(cardapio[aux->num_refeicao]) - 1; i++, lin++)
					imprime_cores(cardapio[2][i], lin, col);
			} break;
			
			case 3:{
				for(int i = 0; i <= strlen(cardapio[aux->num_refeicao]) - 1; i++, lin++)
					imprime_cores(cardapio[3][i], lin, col);
			} break;

			case 4:{
				for(int i = 0; i <= strlen(cardapio[aux->num_refeicao]) - 1; i++, lin++)
					imprime_cores(cardapio[4][i], lin, col);
			} break;
			
			case 5:{
				for(int i = 0; i <= strlen(cardapio[aux->num_refeicao]) - 1; i++, lin++)
					imprime_cores(cardapio[5][i], lin, col);
			} break;
		}

		lin = 1;
		cont++;
		col += 4;
		aux = aux->prox;
	}
}

int verifica_pedido(char* cardapio[], struct fila_clientes* fila, struct pilha* refeicao){

	if(refeicao->topo != NULL){
		int i = 0;
		struct ingredientes* aux = refeicao->topo;

		//enquanto o ingrediente no cardapio eh o mesmo no lanche, e nenhum dos dois chegou ao fim
		while ((aux != NULL) && (i < strlen(cardapio[fila->comeco->num_refeicao])) && 
					(cardapio[fila->comeco->num_refeicao][i] == aux->ingrediente)){
			i++;
			aux = aux->anterior;
		}
		//se ambos chegaram ao fim, sao iguais
		if ((aux == NULL) && (i == strlen(cardapio[fila->comeco->num_refeicao])))
			return 1;
	}
	return 0;
}

struct pedido* cria_pedido(int* num_clientes){
	(*num_clientes)++;
	struct pedido* novo = malloc(sizeof(struct pedido));
	novo->prox = NULL;
	novo->anterior = NULL;
	novo->cliente = (*num_clientes);
	novo->num_refeicao = rand()%6;

	return novo;
}

int fila_vazia(struct fila_clientes* fila){
	//verifica se a fila etsa vazia

	if ((fila->comeco == NULL) && (fila->fim == NULL)){
		return 1;
	}
	return 0;
}

void insere_fim(struct fila_clientes* fila, int* num_clientes){
	struct pedido* novo = cria_pedido(num_clientes);

	if (fila_vazia(fila) == 1){
		fila->comeco = novo;
		fila->fim = novo;
	} else {
		//adiciona o novo cliente e atualiza o ponteiro para o fim
		novo->anterior = fila->fim;
		fila->fim->prox = novo;
		fila->fim = novo;
	}
}

struct fila_clientes* inicializa_fila(int* num_clientes){
	//começa o jogo com tres pedidos ja na fila
	struct fila_clientes* fila;
	fila = malloc(sizeof(struct fila_clientes));
	fila->comeco = NULL;
	fila->fim = NULL;

	insere_fim(fila, num_clientes);
	insere_fim(fila, num_clientes);
	insere_fim(fila, num_clientes);

	return fila;
}

void retira_comeco(struct fila_clientes* fila){
	struct pedido* aux = fila->comeco;

	if (aux->prox != NULL){
		fila->comeco = aux->prox;
		aux->prox->anterior = NULL;
		aux->prox = NULL;
	} else {
		fila->comeco = NULL;
		fila->fim = NULL;
	}

	free(aux);
}

struct pilha* inicializa_pilha(){
	struct pilha* refeicao;
	refeicao = malloc(sizeof(struct pilha));
	refeicao->topo = NULL;

	return refeicao;
}

void push(struct pilha* refeicao, char ingrediente){
	struct ingredientes* novo = malloc(sizeof(struct ingredientes));
	novo->ingrediente = ingrediente;

	if (refeicao->topo == NULL){
		novo->anterior = NULL;
		refeicao->topo = novo;
	} else {
		novo->anterior = refeicao->topo;
		refeicao->topo = novo;
	}
}

void pop(struct pilha* refeicao){
	struct ingredientes* aux = refeicao->topo;

	refeicao->topo = aux->anterior;
	free(aux);
}

void destroi_refeicao(struct pilha* refeicao){
	while (refeicao->topo != NULL)
		pop(refeicao);
}


void imprime_cores(char ingre, int lin, int col){

	switch (ingre){
		case 'H':
			attron(COLOR_PAIR(1));
			mvprintw(lin, col, "[%c]", ingre);
			break;
		case 'P':
			attron(COLOR_PAIR(8));
			mvprintw(lin, col, "[%c]", ingre);
			break;
		case 'p':
			attron(COLOR_PAIR(8));
			mvprintw(lin, col, "[%c]", ingre);
			break;
		case 'Q':
			attron(COLOR_PAIR(3));
			mvprintw(lin, col, "[%c]", ingre);
			break;
		case 'S':
			attron(COLOR_PAIR(2));
			mvprintw(lin, col, "[%c]", ingre);
			break;
		case 'F':
			attron(COLOR_PAIR(9));
			mvprintw(lin, col, "[%c]", ingre);
			break;
		case 'R':
			attron(COLOR_PAIR(6));
			mvprintw(lin, col, "[%c]", ingre);
			break;
	}
	attroff(COLOR_PAIR(1));
	attroff(COLOR_PAIR(2));
	attroff(COLOR_PAIR(3));
	attroff(COLOR_PAIR(8));
	attroff(COLOR_PAIR(9));
	attroff(COLOR_PAIR(6));
}




void imprime_refeicao(struct pilha* refeicao){
	struct ingredientes* aux = refeicao->topo;
	int i = 1;

	mvprintw(0, 28, "Seu preparo atual:");

	while (aux != NULL){
		switch (aux->ingrediente){
			case 'H':
				attron(COLOR_PAIR(1));
				mvprintw(i, 32, "[%c]", aux->ingrediente);
				break;
			case 'P':
				attron(COLOR_PAIR(8));
				mvprintw(i, 32, "[%c]", aux->ingrediente);
				break;
			case 'p':
				attron(COLOR_PAIR(8));
				mvprintw(i, 32, "[%c]", aux->ingrediente);
				break;
			case 'Q':
				attron(COLOR_PAIR(3));
				mvprintw(i, 32, "[%c]", aux->ingrediente);
				break;
			case 'S':
				attron(COLOR_PAIR(2));
				mvprintw(i, 32, "[%c]", aux->ingrediente);
				break;
			case 'F':
				attron(COLOR_PAIR(9));
				mvprintw(i, 32, "[%c]", aux->ingrediente);
				break;
			case 'R':
				attron(COLOR_PAIR(6));
				mvprintw(i, 32, "[%c]", aux->ingrediente);
				break;
		}
		mvprintw(i, 32, "[%c]", aux->ingrediente);
		i++;
		aux = aux->anterior;
		attroff(COLOR_PAIR(1));
		attroff(COLOR_PAIR(2));
		attroff(COLOR_PAIR(3));
		attroff(COLOR_PAIR(8));
		attroff(COLOR_PAIR(9));
		attroff(COLOR_PAIR(6));
	}

	free(aux);
}


int verifica_direita(struct pilha* refeicao, struct fila_clientes* fila, int* pontos, int* pedidos_errados, int* uso_lixeira, struct locais elementos_mapa, char* cardapio[]){
//verifica se o personagem esta tentando "subir" em uma estacao, se sim, faz o que ela pede e retorna o inteiro 1, se nao retorna 0

	if ((elementos_mapa.chapeiro.lin == elementos_mapa.hamburguer.lin) && ((elementos_mapa.chapeiro.col + 1 == elementos_mapa.hamburguer.col[0]) 
			|| (elementos_mapa.chapeiro.col + 1 == elementos_mapa.hamburguer.col[1]) || (elementos_mapa.chapeiro.col + 1 == elementos_mapa.hamburguer.col[2]))){

		push(refeicao, elementos_mapa.hamburguer.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.pao_cima.lin) && ((elementos_mapa.chapeiro.col + 1 == elementos_mapa.pao_cima.col[0]) 
			|| (elementos_mapa.chapeiro.col + 1 == elementos_mapa.pao_cima.col[1]) || (elementos_mapa.chapeiro.col + 1 == elementos_mapa.pao_cima.col[2]))){

		push(refeicao, elementos_mapa.pao_cima.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.pao_baixo.lin) && ((elementos_mapa.chapeiro.col + 1 == elementos_mapa.pao_baixo.col[0]) 
			|| (elementos_mapa.chapeiro.col + 1 == elementos_mapa.pao_baixo.col[1]) || (elementos_mapa.chapeiro.col + 1 == elementos_mapa.pao_baixo.col[2]))){

		push(refeicao, elementos_mapa.pao_baixo.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.queijo.lin) && ((elementos_mapa.chapeiro.col + 1 == elementos_mapa.queijo.col[0]) 
			|| (elementos_mapa.chapeiro.col + 1 == elementos_mapa.queijo.col[1]) || (elementos_mapa.chapeiro.col + 1 == elementos_mapa.queijo.col[2]))){

		push(refeicao, elementos_mapa.queijo.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.salada.lin) && ((elementos_mapa.chapeiro.col + 1 == elementos_mapa.salada.col[0]) 
			|| (elementos_mapa.chapeiro.col + 1 == elementos_mapa.salada.col[1]) || (elementos_mapa.chapeiro.col + 1 == elementos_mapa.salada.col[2]))){

		push(refeicao, elementos_mapa.salada.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.fritas.lin) && ((elementos_mapa.chapeiro.col + 1 == elementos_mapa.fritas.col[0]) 
			|| (elementos_mapa.chapeiro.col + 1 == elementos_mapa.fritas.col[1]) || (elementos_mapa.chapeiro.col + 1 == elementos_mapa.fritas.col[2]))){

		push(refeicao, elementos_mapa.fritas.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.refrigerante.lin) && ((elementos_mapa.chapeiro.col + 1 == elementos_mapa.refrigerante.col[0]) 
			|| (elementos_mapa.chapeiro.col + 1 == elementos_mapa.refrigerante.col[1]) || (elementos_mapa.chapeiro.col + 1 == elementos_mapa.refrigerante.col[2]))){
		push(refeicao, elementos_mapa.refrigerante.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.lixeira.lin) && (elementos_mapa.chapeiro.col + 1 == elementos_mapa.lixeira.col)){
		(*uso_lixeira)++;
		destroi_refeicao(refeicao);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.entrega.lin) && (elementos_mapa.chapeiro.col + 1 == elementos_mapa.entrega.col)){
		if (verifica_pedido(cardapio, fila, refeicao) == 1){
				(*pontos) += 10;
				retira_comeco(fila);
		} else {
			(*pedidos_errados)++;
		}

		destroi_refeicao(refeicao);
		return 1;
	
	} else if (elementos_mapa.chapeiro.col + 1 >= elementos_mapa.parede_direita.col){  
		return 1; //se for tentar andar para a direita do limite maximo das colunas do mapa, a parede lateral
	}

	return 0;
}

int verifica_esquerda(struct pilha* refeicao, struct fila_clientes* fila, int* pontos, int* pedidos_errados, int* uso_lixeira, struct locais elementos_mapa, char* cardapio[]){
//verifica se o personagem esta tentando "subir" em uma estacao, se sim, faz o que ela pede e retorna o inteiro 1, se nao retorna 0

	if ((elementos_mapa.chapeiro.lin == elementos_mapa.hamburguer.lin) && ((elementos_mapa.chapeiro.col - 1 == elementos_mapa.hamburguer.col[0]) 
			|| (elementos_mapa.chapeiro.col - 1 == elementos_mapa.hamburguer.col[1]) || (elementos_mapa.chapeiro.col - 1 == elementos_mapa.hamburguer.col[2]))){

		push(refeicao, elementos_mapa.hamburguer.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.pao_cima.lin) && ((elementos_mapa.chapeiro.col - 1 == elementos_mapa.pao_cima.col[0]) 
			|| (elementos_mapa.chapeiro.col - 1 == elementos_mapa.pao_cima.col[1]) || (elementos_mapa.chapeiro.col - 1 == elementos_mapa.pao_cima.col[2]))){

		push(refeicao, elementos_mapa.pao_cima.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.pao_baixo.lin) && ((elementos_mapa.chapeiro.col - 1 == elementos_mapa.pao_baixo.col[0]) 
			|| (elementos_mapa.chapeiro.col - 1 == elementos_mapa.pao_baixo.col[1]) || (elementos_mapa.chapeiro.col - 1 == elementos_mapa.pao_baixo.col[2]))){

		push(refeicao, elementos_mapa.pao_baixo.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.queijo.lin) && ((elementos_mapa.chapeiro.col - 1 == elementos_mapa.queijo.col[0]) 
			|| (elementos_mapa.chapeiro.col - 1 == elementos_mapa.queijo.col[1]) || (elementos_mapa.chapeiro.col - 1 == elementos_mapa.queijo.col[2]))){

		push(refeicao, elementos_mapa.queijo.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.salada.lin) && ((elementos_mapa.chapeiro.col - 1 == elementos_mapa.salada.col[0]) 
			|| (elementos_mapa.chapeiro.col - 1 == elementos_mapa.salada.col[1]) || (elementos_mapa.chapeiro.col - 1 == elementos_mapa.salada.col[2]))){

		push(refeicao, elementos_mapa.salada.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.fritas.lin) && ((elementos_mapa.chapeiro.col - 1 == elementos_mapa.fritas.col[0]) 
			|| (elementos_mapa.chapeiro.col - 1 == elementos_mapa.fritas.col[1]) || (elementos_mapa.chapeiro.col - 1 == elementos_mapa.fritas.col[2]))){

		push(refeicao, elementos_mapa.fritas.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.refrigerante.lin) && ((elementos_mapa.chapeiro.col - 1 == elementos_mapa.refrigerante.col[0]) 
			|| (elementos_mapa.chapeiro.col - 1 == elementos_mapa.refrigerante.col[1]) || (elementos_mapa.chapeiro.col - 1 == elementos_mapa.refrigerante.col[2]))){

		push(refeicao, elementos_mapa.refrigerante.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.lixeira.lin) && (elementos_mapa.chapeiro.col - 1 == elementos_mapa.lixeira.col)){
		(*uso_lixeira)++;
		destroi_refeicao(refeicao);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin == elementos_mapa.entrega.lin) && (elementos_mapa.chapeiro.col - 1 == elementos_mapa.entrega.col)){
		if (verifica_pedido(cardapio, fila, refeicao) == 1){
				(*pontos) += 10;
				retira_comeco(fila);
		} else {
			(*pedidos_errados)++;
		}

		destroi_refeicao(refeicao);
		return 1;
	
	} else if (elementos_mapa.chapeiro.col - 1 <= 0){  
		return 1; //se for tentar andar para a direita do limite maximo das colunas do mapa, a parede lateral que fica na coluna 0
	}

	return 0;
}

int verifica_baixo(struct pilha* refeicao, struct fila_clientes* fila, int* pontos, int* pedidos_errados, int* uso_lixeira, struct locais elementos_mapa, char* cardapio[]){
//verifica se o personagem esta tentando "subir" em uma estacao, se sim, faz o que ela pede e retorna o inteiro 1, se nao retorna 0

	if ((elementos_mapa.chapeiro.lin + 1 == elementos_mapa.hamburguer.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.hamburguer.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.hamburguer.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.hamburguer.col[2]))){

		push(refeicao, elementos_mapa.hamburguer.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin + 1 == elementos_mapa.pao_cima.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.pao_cima.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.pao_cima.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.pao_cima.col[2]))){

		push(refeicao, elementos_mapa.pao_cima.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin + 1  == elementos_mapa.pao_baixo.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.pao_baixo.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.pao_baixo.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.pao_baixo.col[2]))){
		push(refeicao, elementos_mapa.pao_baixo.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin + 1 == elementos_mapa.queijo.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.queijo.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.queijo.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.queijo.col[2]))){

		push(refeicao, elementos_mapa.queijo.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin + 1 == elementos_mapa.salada.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.salada.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.salada.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.salada.col[2]))){

		push(refeicao, elementos_mapa.salada.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin + 1 == elementos_mapa.fritas.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.fritas.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.fritas.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.fritas.col[2]))){

		push(refeicao, elementos_mapa.fritas.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin + 1 == elementos_mapa.refrigerante.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.refrigerante.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.refrigerante.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.refrigerante.col[2]))){

		push(refeicao, elementos_mapa.refrigerante.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin + 1 == elementos_mapa.lixeira.lin) && (elementos_mapa.chapeiro.col == elementos_mapa.lixeira.col)){
		(*uso_lixeira)++;
		destroi_refeicao(refeicao);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin + 1 == elementos_mapa.entrega.lin) && (elementos_mapa.chapeiro.col == elementos_mapa.entrega.col)){
		if (verifica_pedido(cardapio, fila, refeicao) == 1){
				(*pontos) += 10;
				retira_comeco(fila);
		} else {
			(*pedidos_errados)++;
		}

		destroi_refeicao(refeicao);
		return 1;
	
	} else if (elementos_mapa.chapeiro.lin + 1 >= elementos_mapa.parede_baixo.lin){  
		return 1; //se for tentar andar para abaixo do limite maximo das linhas do mapa, a parede horizontal
	}

	return 0;
}

int verifica_cima(struct pilha* refeicao, struct fila_clientes* fila, int* pontos, int* pedidos_errados, int* uso_lixeira, struct locais elementos_mapa, char* cardapio[]){
//verifica se o personagem esta tentando "subir" em uma estacao, se sim, faz o que ela pede e retorna o inteiro 1, se nao retorna 0

	if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.hamburguer.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.hamburguer.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.hamburguer.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.hamburguer.col[2]))){

		push(refeicao, elementos_mapa.hamburguer.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.pao_cima.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.pao_cima.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.pao_cima.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.pao_cima.col[2]))){

		push(refeicao, elementos_mapa.pao_cima.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.pao_baixo.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.pao_baixo.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.pao_baixo.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.pao_baixo.col[2]))){

		push(refeicao, elementos_mapa.pao_baixo.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.queijo.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.queijo.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.queijo.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.queijo.col[2]))){

		push(refeicao, elementos_mapa.queijo.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.salada.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.salada.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.salada.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.salada.col[2]))){

		push(refeicao, elementos_mapa.salada.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.fritas.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.fritas.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.fritas.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.fritas.col[2]))){

		push(refeicao, elementos_mapa.fritas.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.refrigerante.lin) && ((elementos_mapa.chapeiro.col == elementos_mapa.refrigerante.col[0]) 
			|| (elementos_mapa.chapeiro.col == elementos_mapa.refrigerante.col[1]) || (elementos_mapa.chapeiro.col == elementos_mapa.refrigerante.col[2]))){

		push(refeicao, elementos_mapa.refrigerante.ingrediente);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.lixeira.lin) && (elementos_mapa.chapeiro.col == elementos_mapa.lixeira.col)){
		(*uso_lixeira)++;
		destroi_refeicao(refeicao);
		return 1;

	} else if ((elementos_mapa.chapeiro.lin - 1 == elementos_mapa.entrega.lin) && (elementos_mapa.chapeiro.col == elementos_mapa.entrega.col)){
		if (verifica_pedido(cardapio, fila, refeicao) == 1){
				(*pontos) += 10;
				retira_comeco(fila);
		} else {
			(*pedidos_errados)++;
		}

		destroi_refeicao(refeicao);
		return 1;
	
	} else if ((elementos_mapa.chapeiro.lin - 1 <= 0) && (elementos_mapa.chapeiro.col != elementos_mapa.entrega.col)){  
		return 1; //se for tentar andar para acima do limite maximo das linhas do mapa, a parede horizontal que fica na linha 0
	}

	return 0;
}

void inicializa_elem_mapa(struct locais* elementos_mapa){

	//lixeira
	elementos_mapa->lixeira.simbolo = 'o';
	elementos_mapa->lixeira.lin = 5;
	elementos_mapa->lixeira.col = 25;

	//ponto de entrega
	elementos_mapa->entrega.simbolo = '@';
	elementos_mapa->entrega.lin = 0;
	elementos_mapa->entrega.col = 14;

	//chapeiro
	elementos_mapa->chapeiro.simbolo = '&';
	elementos_mapa->chapeiro.lin = 2;
	elementos_mapa->chapeiro.col = 23;

	//hamburguer
	elementos_mapa->hamburguer.simbolo[0] = '[';
	elementos_mapa->hamburguer.simbolo[1] =  'H';
	elementos_mapa->hamburguer.simbolo[2] = ']';
	elementos_mapa->hamburguer.lin = 7;
	elementos_mapa->hamburguer.col[0] = 7;
	elementos_mapa->hamburguer.col[1] = 8;
	elementos_mapa->hamburguer.col[2] = 9;
	elementos_mapa->hamburguer.ingrediente = 'H';

	//fritas
	elementos_mapa->fritas.simbolo[0] = '[';
	elementos_mapa->fritas.simbolo[1] = 'F';
	elementos_mapa->fritas.simbolo[2] = '[';
	elementos_mapa->fritas.lin = 4;
	elementos_mapa->fritas.col[0] = 2;
	elementos_mapa->fritas.col[1] = 3;
	elementos_mapa->fritas.col[2] = 4;
	elementos_mapa->fritas.ingrediente = 'F';

	//pao cima
	elementos_mapa->pao_cima.simbolo[0] = '[';
	elementos_mapa->pao_cima.simbolo[1] = 'P';
	elementos_mapa->pao_cima.simbolo[2] = '[';
	elementos_mapa->pao_cima.lin = 7;
	elementos_mapa->pao_cima.col[0] = 22;
	elementos_mapa->pao_cima.col[1] = 23;
	elementos_mapa->pao_cima.col[2] = 24;
	elementos_mapa->pao_cima.ingrediente = 'P';

	//pao baixo
	elementos_mapa->pao_baixo.simbolo[0] = '[';
	elementos_mapa->pao_baixo.simbolo[1] = 'p';
	elementos_mapa->pao_baixo.simbolo[2] = '[';
	elementos_mapa->pao_baixo.lin = 7;
	elementos_mapa->pao_baixo.col[0] = 2;
	elementos_mapa->pao_baixo.col[1] = 3;
	elementos_mapa->pao_baixo.col[2] = 4;
	elementos_mapa->pao_baixo.ingrediente = 'p';

	//queijo
	elementos_mapa->queijo.simbolo[0] = '[';
	elementos_mapa->queijo.simbolo[1] = 'Q';
	elementos_mapa->queijo.simbolo[2] = '[';
	elementos_mapa->queijo.lin = 7;
	elementos_mapa->queijo.col[0] = 12;
	elementos_mapa->queijo.col[1] = 13;
	elementos_mapa->queijo.col[2] = 14;
	elementos_mapa->queijo.ingrediente = 'Q';

	//salada
	elementos_mapa->salada.simbolo[0] = '[';
	elementos_mapa->salada.simbolo[1] = 'S';
	elementos_mapa->salada.simbolo[2] = '[';
	elementos_mapa->salada.lin = 7;
	elementos_mapa->salada.col[0] = 17;
	elementos_mapa->salada.col[1] = 18;
	elementos_mapa->salada.col[2] = 19;
	elementos_mapa->salada.ingrediente = 'S';

	//refrigerante
	elementos_mapa->refrigerante.simbolo[0] = '[';
	elementos_mapa->refrigerante.simbolo[1] = 'R';
	elementos_mapa->refrigerante.simbolo[2] = '[';
	elementos_mapa->refrigerante.lin = 2;
	elementos_mapa->refrigerante.col[0] = 2;
	elementos_mapa->refrigerante.col[1] = 3;
	elementos_mapa->refrigerante.col[2] = 4;
	elementos_mapa->refrigerante.ingrediente = 'R';

	//paredes 
	elementos_mapa->parede_direita.col = 26;
	elementos_mapa->parede_direita.simbolo = '|';

	elementos_mapa->parede_baixo.lin = 8;
	elementos_mapa->parede_baixo.simbolo = '-';
}

void pontuacao(int* pontos){
	attron(COLOR_PAIR(4));
	mvprintw(10, 0, "PONTOS: %d", (*pontos));
	attroff(COLOR_PAIR(4));
	mvprintw(11, 0, "Aperte f1 para sair.");
	mvprintw(12, 0, "Dica: Faça os pedidos de baixo para cima.");
}

void imprime_tela(char* cardapio[], struct fila_clientes* fila, struct locais* elementos_mapa, struct pilha* refeicao, int* pontos){
	//limpa tela
	clear();

	imprime_refeicao(refeicao);
	pontuacao(pontos);
	imprime_pedido(cardapio, fila);

	int i;
	//imprime parede esquerda
	for (i = 0; i <= 8; i++){
		mvprintw(i, 0, "|");
	}

	//imprime parede direita
	for (i = 0; i <= 8; i++){
		mvprintw(i, 26, "|");
	}

	//imprime parede baixo
	for (i = 0; i <= 26; i++){
		mvprintw(8, i, "-");
	}

	//imprime parede cima
	for (i = 0; i <= 26; i++){
		mvprintw(0, i, "-");
	}

	//elementos do mapa
	attron(COLOR_PAIR(7));
	mvprintw(5, 25, "o");
	attron(COLOR_PAIR(5));
	mvprintw(0, 13, " @ ");
	attroff(COLOR_PAIR(5));

	mvprintw(elementos_mapa->chapeiro.lin, elementos_mapa->chapeiro.col, "&");
	attron(COLOR_PAIR(1));
	mvprintw(7, 7, "[H]");

	attron(COLOR_PAIR(9));
	mvprintw(4, 2, "[F]");

	attron(COLOR_PAIR(8));
	mvprintw(7, 22, "[P]");

	attron(COLOR_PAIR(8));
	mvprintw(7, 2, "[p]");

	attron(COLOR_PAIR(3));
	mvprintw(7, 12, "[Q]");

	attron(COLOR_PAIR(2));
	mvprintw(7, 17, "[S]");

	attron(COLOR_PAIR(6));
	mvprintw(2, 2, "[R]");

	attroff(COLOR_PAIR(1));
	attroff(COLOR_PAIR(2));
	attroff(COLOR_PAIR(3));
	attroff(COLOR_PAIR(4));
	attroff(COLOR_PAIR(6));
	attroff(COLOR_PAIR(7));
	attroff(COLOR_PAIR(8));
	attroff(COLOR_PAIR(9));

	refresh();
}


void regras(){
    char a;
    a = '.';
    while (a != ' '){
        printw("-------------------------------- Bem vindo ao jogo De-Burguer! --------------------------------\n");
        printw("--> O objetivo do jogo eh fazer o maior numero de pedidos possivel sem errar.\n");
        printw("--> Para isso, voce deve usar as setas do teclado para se movimentar e coletar os ingredientes.\n");
        printw("--> Cada pedido vale 10 pontos, sendo 8 pedidos o maximo.\n");
        printw("--> Voce pode errar 3 pedidos e usar a lixeira 5 vezes.\n\n");
		printw("--> Para melhor jogabilidade, recomendamos usar o terminal no modo tela cheia.\n");
        printw("--> Aperte espaco 2x para continuar.\n");
        printw("-----------------------------------------------------------------------------------------------\n");
        a = getch();
    }
}


void animacao_inicio(){

    printw(" ______   _______             _______  __   __  ______    _______  __   __  _______  ______   \n");refresh(); usleep(150000);
    printw("|      | |       |           |  _    ||  | |  ||    _ |  |       ||  | |  ||       ||    _ |  \n");refresh(); usleep(150000);
    printw("|  _    ||    ___|   ____    | |_|   ||  | |  ||   | ||  |    ___||  | |  ||    ___||   | ||  \n");refresh(); usleep(150000);
    printw("| | |   ||   |___   |____|   |       ||  |_|  ||   |_||_ |   | __ |  |_|  ||   |___ |   |_||_ \n");refresh(); usleep(150000);
    printw("| |_|   ||    ___|           |  _   | |       ||    __  ||   ||  ||       ||    ___||    __  |\n");refresh(); usleep(150000);
    printw("|       ||   |___            | |_|   ||       ||   |  | ||   |_| ||       ||   |___ |   |  | |\n");refresh(); usleep(150000);
    printw("|______| |_______|           |_______||_______||___|  |_||_______||_______||_______||___|  |_|\n\n");refresh(); usleep(150000);
    printw("-------------------------------Desenvolvido por Eduarda e Luize ------------------------------\n");refresh(); usleep(150000);
    usleep(2000000);
	clear();
}

void game_over(){
    clear();
	printw(" _______  _______  __   __  _______        _______  __   __  _______  ______      ___      ____  \n");
	printw("|       ||   _   ||  |_|  ||       |      |       ||  | |  ||       ||    _ |    |   |    |    | \n");
	printw("|    ___||  |_|  ||       ||    ___|      |   _   ||  |_|  ||    ___||   | ||    |___|   |    _| \n");
	printw("|   | __ |       ||       ||   |___       |  | |  ||       ||   |___ |   |_||_    ___    |   |   \n");
	printw("|   ||  ||       ||       ||    ___|      |  |_|  ||       ||    ___||    __  |  |   |   |   |   \n");
	printw("|   |_| ||   _   || ||_|| ||   |___       |       | |     | |   |___ |   |  | |  |___|   |   |_  \n");
	printw("|_______||__| |__||_|   |_||_______|      |_______|  |___|  |_______||___|  |_|           |____| \n\n");
    printw("                          Pressione qualquer tecla para sair");
    getch();
    endwin();
    exit(0);
}

void game_won(){
	clear();
	printw(" __   __  _______  _______  _______    _______  _______  __    _  __   __  _______  __   __  __       ___   ____   \n");
	printw("|  | |  ||       ||       ||       |  |       ||   _   ||  |  | ||  | |  ||       ||  | |  ||  |     |   | |    |  \n");
	printw("|  |_|  ||   _   ||       ||    ___|  |    ___||  |_|  ||   |_| ||  |_|  ||   _   ||  | |  ||  |     |___| |_    | \n");
	printw("|       ||  | |  ||       ||   |___   |   | __ |       ||       ||       ||  | |  ||  |_|  ||  |      ___    |   | \n");
	printw("|       ||  |_|  ||      _||    ___|  |   ||  ||       ||  _    ||       ||  |_|  ||       ||__|     |   |   |   | \n");
	printw(" |     | |       ||     |_ |   |___   |   |_| ||   _   || | |   ||   _   ||       ||       | __      |___|  _|   | \n");
	printw("  |___|  |_______||_______||_______|  |_______||__| |__||_|  |__||__| |__||_______||_______||__|           |____|  \n\n");
	printw("                                   Pressione qualquer tecla para sair");
    getch();
    endwin();
    exit(0);
}

void free_ponteiros(struct pilha* refeicao, struct fila_clientes* fila){
	if ((refeicao->topo != NULL) || (fila->comeco != NULL)){
		destroi_refeicao(refeicao);

		while (fila->comeco != NULL){
			retira_comeco(fila);
		}
	}

	free(refeicao);
	free(fila);
}