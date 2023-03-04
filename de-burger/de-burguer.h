#define X_Burguer 0
#define X_Salada 1
#define Combo_1 2
#define Combo_2 3
#define Vegetariano 4
#define Vegano 5



struct estacao{
	char simbolo[3];
	int lin, col[3];
	char ingrediente;
};

struct elem_mapa{ 
	char simbolo;
	int lin, col;
};

struct locais{
	struct elem_mapa parede_direita, parede_baixo, lixeira, entrega, chapeiro;
	struct estacao hamburguer, pao_cima, pao_baixo, queijo, salada, fritas, refrigerante;
};

struct pedido{ //fila de pedidos
	int cliente;
	int num_refeicao;
	struct pedido* prox;
	struct pedido* anterior;
};

struct fila_clientes{ //ponteiros da fila
	struct pedido* fim;
	struct pedido* comeco;
};

struct ingredientes{ //struct dos ingredientes que compoem a pilha de ingredientes
	char ingrediente;
	struct ingredientes* anterior;
};

struct pilha{	//struct da pilha que compoe a ingredientes
	struct ingredientes* topo;
};

void inicializa_ncurses();

void inicializa_cardapio(char* cardapio[]);

int verifica_pedido(char* cardapio[], struct fila_clientes* fila, struct pilha* refeicao);

void imprime_pedido(char* cardapio[], struct fila_clientes* fila);

struct pedido* cria_pedido(int* num_clientes);

int fila_vazia(struct fila_clientes* fila);

void insere_fim(struct fila_clientes* fila, int* num_clientes);

struct fila_clientes* inicializa_fila(int* num_clientes);

void retira_comeco(struct fila_clientes* fila);

struct pilha* inicializa_pilha();

void push(struct pilha* refeicao, char ingrediente);

void pop(struct pilha* refeicao);

void destroi_refeicao(struct pilha* refeicao);

void imprime_cores(char ingre, int lin, int col);

void imprime_refeicao(struct pilha* refeicao);

int verifica_direita(struct pilha* refeicao, struct fila_clientes* fila, int* pontos, int* pedidos_errados, int* uso_lixeira, struct locais elementos_mapa, char* cardapio[]);

int verifica_esquerda(struct pilha* refeicao, struct fila_clientes* fila, int* pontos, int* pedidos_errados, int* uso_lixeira, struct locais elementos_mapa, char* cardapio[]);

int verifica_baixo(struct pilha* refeicao, struct fila_clientes* fila, int* pontos, int* pedidos_errados, int* uso_lixeira, struct locais elementos_mapa, char* cardapio[]);

int verifica_cima(struct pilha* refeicao, struct fila_clientes* fila, int* pontos, int* pedidos_errados, int* uso_lixeira, struct locais elementos_mapa, char* cardapio[]);

void inicializa_elem_mapa(struct locais* elementos_mapa);

void pontuacao(int* pontos);

void imprime_tela(char* cardapio[], struct fila_clientes* fila, struct locais* elementos_mapa, struct pilha* refeicao, int* pontos);

void regras();

void animacao_inicio();

void game_over();

void game_won();

void free_ponteiros(struct pilha* refeicao, struct fila_clientes* fila);