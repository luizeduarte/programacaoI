#include "calendario_lib.h"
#include <stdio.h>

int main(){
	int sel = 0;
	struct compromisso **meses_v = inicializar_agenda();

	while (sel != 3){
		printf("\nSelecione o que deseja fazer:\n 1.adicionar um compromisso\n 2.listar os compromissos\n 3.sair\n");
		scanf("%d", &sel);

		if (sel == 1)
			adicionar_compromisso(meses_v);
		else if (sel == 2)
			listar_compromissos(meses_v);
	}

	destruir_agenda(meses_v);
	return 0;
}