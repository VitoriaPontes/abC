//.h necessário para que o main reconheça a lista.c

struct palavra {
    char *nome;
    char **soletrado;
    int tam;
};

typedef struct palavra Palavra;

struct elemento {
    struct palavra info;
    struct elemento *prox;
};

typedef struct elemento Elemento;

typedef struct lista{
    Elemento *inicio;
    int qtd;
} Lista;

Lista *criar();
int inserirInicio (Lista *, Palavra);
int acessarValor(Lista *, int, Palavra*);
void destruir(Lista *);
int vazia (Lista *);
Palavra criarPalavra(char *, char **, int);

// para controle de testes:
int exibir(Lista *);
