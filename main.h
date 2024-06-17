#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdbool.h>
#include <time.h>

typedef struct tm DataTempo;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int codigo;
    char nome[50];
    char endereco[50];
    char telefone[13];
} Cliente;

typedef struct {
    int codigo;
    char nome[50];
    char telefone[13];
    char cargo[50];
    float salario;
} Funcionario;

typedef struct {
    int codigo;
    DataTempo data_entrada;
    DataTempo data_saida;
    int quantidade_diarias;
    int codigo_cliente;
    int numero_quarto;
    bool com_baixa; // Usado para verificar se a estadia já teve baixa.
} Estadia;

typedef struct {
    int numero;
    int quantidade_hospedes;
    float valor_diaria;
    /* bool status;
     * O status está de acordo com o período solicitado de estadia,
     * portanto não justifica ter uma propriedade na estrutura dos quartos,
     * visto que esses parâmetros estão sendo usados para validar o status
     * dos quartos.
     * */
} Quarto;

typedef struct {
    Quarto *vetor;
    int tamanho;
} VetorQuartos;

void aviso(char *);

void menu();

void cadastrar_cliente();

void cadastrar_funcionario();

void cadastrar_quarto();

void cadastrar_estadia();

void baixa_estadia();

void pesquisar_cliente();

void pesquisar_funcionario();

void listar_estadias_cliente();

void pontos_fidelidade_cliente();

bool verificar_formato_data(Data);

DataTempo buscar_data(Data, int);

bool verificar_cliente_existente(char *);

Cliente buscar_cliente_pelo_nome(char *);

bool verificar_funcionario_existente(char *);

Funcionario buscar_funcionario_pelo_nome(char *);

bool verificar_quarto_existente(int);

Quarto buscar_quarto_pelo_numero(int);

VetorQuartos buscar_quartos_disponiveis(int, DataTempo, DataTempo);

bool verificar_quarto_vetor(VetorQuartos, int);

Quarto pesquisar_quarto_vetor(VetorQuartos, int);

void remover_quarto_vetor(VetorQuartos *, int);

#endif // MAIN_H_INCLUDED
