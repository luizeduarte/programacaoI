#include "calendario_lib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

struct compromisso{
	struct tm data;
	char *evento;
	struct compromisso *prox;
};

struct compromisso** inicializar_agenda(){
	struct compromisso **meses_v;
	meses_v = malloc(12 * sizeof(struct compromisso*));

	for (int i = 0; i<12; i++)
		meses_v[i] = NULL;

	return meses_v;
}

struct compromisso* criar_compromisso(){
	int mes, dia, horario, num_char;
	size_t tam_string = 1;

	struct compromisso *novo;
	novo = malloc(sizeof(struct compromisso));
	novo->evento = malloc(tam_string * sizeof(char));
	
	printf("Informe o dia, mes e horario do seu novo compromisso:\n");
	scanf("%d %d %d", &dia, &mes, &horario);
	getchar();

	novo->data.tm_mday = dia;
	novo->data.tm_mon = mes - 1;
	novo->data.tm_hour = horario;
	novo->prox = NULL;

	printf("Como deseja nomea-lo?\n");
	num_char = getline(&(novo->evento), &tam_string, stdin);
	novo->evento[num_char - 1] = '\0';

	return novo;
}

void adicionar_compromisso(struct compromisso *meses_v[]){
	struct compromisso *novo = criar_compromisso();
	struct compromisso *aux = meses_v[novo->data.tm_mon];

	//se aux for NULL, significa que é o primeiro compromisso agendado do mes
	if (aux == NULL){
		meses_v[novo->data.tm_mon] = novo;
		printf("Compromisso agendado!\n");
	}
	//faz a comparacao com o primeiro elemento caso seja necessario inserir antes dele
	else if ((novo->data.tm_mday <= aux->data.tm_mday) && (novo->data.tm_hour <= aux->data.tm_hour)){

		//checa se ja existe algum compromisso marcado para o mesmo horario, se nao, adiciona o novo compromisso
		if ((novo->data.tm_hour == aux->data.tm_hour) && (novo->data.tm_mday == aux->data.tm_mday)){
			printf("Ja existe compromisso marcado para este horario!\n");
			free(novo->evento);
			free(novo);
		} else {
			novo->prox = aux;
			meses_v[novo->data.tm_mon] = novo;
			printf("Compromisso agendado!\n");
		}

	} else {

		//encontra o lugar na lista onde se deve inserir o novo compromisso 
		while ((aux->prox != NULL) && (novo->data.tm_mday >= aux->prox->data.tm_mday) && (novo->data.tm_hour > aux->prox->data.tm_hour)){
			aux = aux->prox;
		}

		//checa se ja existe algum compromisso marcado para o mesmo horario, se nao, adiciona o novo compromisso
		if ((aux->prox != NULL) && (novo->data.tm_hour == aux->prox->data.tm_hour) && (novo->data.tm_mday == aux->prox->data.tm_mday)){
			printf("Ja existe compromisso marcado para este horario!\n");
			free(novo->evento);
			free(novo);
		} else {
			novo->prox = aux->prox;
			aux->prox = novo;
			printf("Compromisso agendado!\n");
		}
	}
}


void listar_compromissos(struct compromisso *meses_v[]){
	struct compromisso *aux;

	for (int i = 0; i<12; i++){
		aux = meses_v[i];

		if (aux != NULL){
			printf("\nMES %d:\n", i + 1);

			while (aux != NULL){
				printf ("\t \"%s\" no dia %d as %d horas\n", aux->evento, aux->data.tm_mday, aux->data.tm_hour);
				aux = aux->prox;
			}
		}
	}
}

struct compromisso* destruir_agenda(struct compromisso *meses_v[]){
	struct compromisso *aux, *atual;

	if (meses_v == NULL)
		return NULL;

	for (int i = 0; i<12; i++){
		aux = meses_v[i];
		atual = meses_v[i];

	/*usa um ponteiro aux para salvar a localizacao do prox
	elemento da lista enquanto usa um ponteiro "atual" para apontar a memoria que deseja desalocar*/
		while (aux != NULL){
			aux = aux->prox;
			free(atual->evento);
			free(atual);
			atual = aux;
		}
	}

	free(meses_v);

	return NULL;
}