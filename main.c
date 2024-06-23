/************************************************************************
 * Objetivo: Sistema de gerenciamento de um hotel                       *             
 * Autores: Julia Oliveira, Santhiago Takaesu, Kelvin Mendes.           *
 * Versão: 1.0                                                          *
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define HORAS 3600
#define DIAS 86400

// Definição dos arquivos de dados
FILE *dados_clientes;
FILE *dados_funcionarios;
FILE *dados_estadias;
FILE *dados_quartos;

// Definições das estruturas de dados
typedef struct tm DataTempo;

typedef struct{
    int dia;
    int mes;
    int ano;
} Data;

typedef struct{
    int codigo;
    char nome[50];
    char endereco[50];
    char telefone[11];
} Cliente;

typedef struct{
    int codigo;
    char nome[50];
    char telefone[11];
    char cargo[50];
    float salario;
} Funcionario;

typedef struct{
    int codigo;
    DataTempo data_entrada;
    DataTempo data_saida;
    int quantidade_diarias;
    int codigo_cliente;
    int numero_quarto;
    bool com_baixa; // Usado para verificar se a estadia já teve baixa.
} Estadia;

typedef struct{
    int numero;
    int quantidade_hospedes;
    float valor_diaria;
    bool status;
} Quarto;

typedef struct{
    Quarto *vetor;
    int tamanho;
} VetorQuartos;

// Declaração das funções
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
bool validar_endereco(const char *endereco);
bool validar_telefone(const char *telefone);

// Programa principal
int main(){
    char opcao;

    dados_clientes = fopen("dados_clientes.txt", "rb+");
    if (dados_clientes == NULL)
        dados_clientes = fopen("dados_clientes.txt", "wb+");

    dados_funcionarios = fopen("dados_funcionarios.txt", "rb+");
    if (dados_funcionarios == NULL)
        dados_funcionarios = fopen("dados_funcionarios.txt", "wb+");

    dados_estadias = fopen("dados_estadias.txt", "rb+");
    if (dados_estadias == NULL)
        dados_estadias = fopen("dados_estadias.txt", "wb+");

    dados_quartos = fopen("dados_quartos.txt", "rb+");
    if (dados_quartos == NULL)
        dados_quartos = fopen("dados_quartos.txt", "wb+");

    do{
        system("cls");
        menu();
        printf("\nSelecione a opcao\n");
        opcao = getchar();
        fflush(stdin);
        system("cls");
        switch (opcao - '0'){
        case 0:
            fclose(dados_clientes);
            fclose(dados_funcionarios);
            fclose(dados_estadias);
            fclose(dados_quartos);
            break;
        case 1:
            cadastrar_cliente();
            break;
        case 2:
            cadastrar_funcionario();
            break;
        case 3:
            cadastrar_quarto();
            break;
        case 4:
            cadastrar_estadia();
            break;
        case 5:
            baixa_estadia();
            break;
        case 6:
            pesquisar_cliente();
            break;
        case 7:
            pesquisar_funcionario();
            break;
        case 8:
            listar_estadias_cliente();
            break;
        case 9:
            pontos_fidelidade_cliente();
            break;
        } getchar();
    } while (opcao != '0');
    return 0;
}

// Função para exibir um aviso na tela
void aviso(char *mensagem){
    printf("\n%s\n", mensagem);
}

// Função para exibir o menu principal
void menu(){
    printf("0 - Sair\n");
    printf("1 - Cadastrar cliente\n");
    printf("2 - Cadastrar funcionario\n");
    printf("3 - Cadastrar quarto\n");
    printf("4 - Cadastrar estadia\n");
    printf("5 - Dar baixa na estadia\n");
    printf("6 - Pesquisar cliente\n");
    printf("7 - Pesquisar funcionario\n");
    printf("8 - Listar estadias de um cliente\n");
    printf("9 - Exibir pontos de fidelidade um cliente\n");
}

// Função para cadastrar um novo cliente
void cadastrar_cliente(){
    Cliente cliente;
    printf("Cadastro de cliente\n\n");

    printf("Nome: ");
    gets(cliente.nome);
    if (verificar_cliente_existente(cliente.nome)){
        return aviso("Ja existe um cliente com esse nome.");
    }
    printf("Endereco: ");
    gets(cliente.endereco);
    if (!validar_endereco(cliente.endereco)){
        return aviso("Endereco invalido. Deve conter letras e numeros.");
    }
    printf("Telefone: ");
    gets(cliente.telefone);
    if (!validar_telefone(cliente.telefone)){
       return aviso("Telefone invalido. Deve conter exatamente 11 digitos.");
    }
    fseek(dados_clientes, 0, SEEK_END);
    cliente.codigo = ftell(dados_clientes) / sizeof(Cliente) + 1;
    fwrite(&cliente, sizeof(Cliente), 1, dados_clientes);
    fflush(dados_clientes);
    printf("Cliente cadastrado com sucesso\n");
}

// Função para cadastrar um novo funcionário
void cadastrar_funcionario(){
    Funcionario funcionario;
    printf("Cadastro de funcionario\n\n");
    
    printf("Nome: ");
    gets(funcionario.nome);
    if (verificar_funcionario_existente(funcionario.nome))
        return aviso("Ja existe um funcionario com esse nome.");
    printf("Telefone: ");
    gets(funcionario.telefone);
    printf("\nSelecione um cargo\n");
    printf("  1 - Recepcionista\n");
    printf("  2 - Auxiliar de limpeza\n");
    printf("  3 - Garcom\n");
    printf("  4 - Gerente\n");
    switch (getchar() - '0'){
    case 1:
        strcpy(funcionario.cargo, "Recepcionista");
        break;
    case 2:
        strcpy(funcionario.cargo, "Auxiliar de limpeza");
        break;
    case 3:
        strcpy(funcionario.cargo, "Garcom");
        break;
    case 4:
        strcpy(funcionario.cargo, "Gerente");
        break;
    default:
        return aviso("Opcao de cargo invalida.");
    }
    printf("Cargo: %s\n\n", funcionario.cargo);
    printf("Salario: ");
    scanf("%f", &funcionario.salario);
    fseek(dados_funcionarios, 0, SEEK_END);
    funcionario.codigo = ftell(dados_funcionarios) / sizeof(Funcionario) + 1;
    fwrite(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);
    fflush(dados_funcionarios);
    printf("Funcionario cadastrado com sucesso\n");
}

// Função para cadastrar um novo quarto
void cadastrar_quarto(){
    Quarto quarto;
    printf("Cadastro de quarto\n\n");
    printf("Numero do quarto: ");
    scanf("%d", &quarto.numero);
    if (verificar_quarto_existente(quarto.numero))
        return aviso("Ja existe um quarto com esse numero.");
    printf("Quantidade de hospedes: ");
    scanf("%d", &quarto.quantidade_hospedes);
    printf("Valor diaria: ");
    scanf("%f", &quarto.valor_diaria);
    fflush(stdin);
    fseek(dados_quartos, 0, SEEK_END);
    fwrite(&quarto, sizeof(Quarto), 1, dados_quartos);
    fflush(dados_quartos);
    printf("Quarto cadastrado com sucesso\n");
}

// Função para cadastrar uma nova estadia
void cadastrar_estadia(){
    Estadia estadia;
    Cliente cliente;
    Data data;
    VetorQuartos quartos_disponiveis;
    char nome_cliente[50];
    int quantidade_hospedes;
    int i;
    printf("Cadastro de estadia\n\n");
    printf("Nome do cliente: ");
    gets(nome_cliente);
    if (!verificar_cliente_existente(nome_cliente))
        return aviso("Nao foi encontrado um cliente com esse nome.");
    cliente = buscar_cliente_pelo_nome(nome_cliente);
    estadia.codigo_cliente = cliente.codigo;

    printf("Data de entrada (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
    if (!verificar_formato_data(data))
        return aviso("Data invalida.");
    estadia.data_entrada = buscar_data(data, 14);

    printf("Data de saida (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
    if (!verificar_formato_data(data))
        return aviso("Data invalida.");
    estadia.data_saida = buscar_data(data, 12);
    if (difftime(mktime(&estadia.data_saida), mktime(&estadia.data_entrada)) / HORAS < 22)
        return aviso("A data de saida deve ser no minimo um dia apos a data de entrada.");

    printf("Quantidade de hospedes: ");
    scanf("%d", &quantidade_hospedes);

    quartos_disponiveis = buscar_quartos_disponiveis(quantidade_hospedes, estadia.data_entrada, estadia.data_saida);
    printf("Quartos disponiveis:\t");
    if (quartos_disponiveis.tamanho == 0)
        return aviso("Nao existe quartos disponiveis.");
    for (i = 0; i < quartos_disponiveis.tamanho; ++i){
        printf("%d\t", quartos_disponiveis.vetor[i].numero);
        if ((i + 1) % 5 == 0 && i != quartos_disponiveis.tamanho - 1)
            printf("\n\t\t\t");
    }
    printf("\n");

    printf("Numero do quarto: ");
    scanf("%d", &estadia.numero_quarto);
    if (!verificar_quarto_vetor(quartos_disponiveis, estadia.numero_quarto))
        return aviso("Nao foi encontrado um quarto desocupado com esse numero.");

    estadia.quantidade_diarias = (difftime(mktime(&estadia.data_saida), mktime(&estadia.data_entrada)) + (estadia.data_entrada.tm_hour - estadia.data_saida.tm_hour) * HORAS) / DIAS;
    estadia.com_baixa = false;
    fseek(dados_estadias, 0, SEEK_END);
    estadia.codigo = ftell(dados_estadias) / sizeof(Estadia) + 1;
    fwrite(&estadia, sizeof(Estadia), 1, dados_estadias);
    fflush(dados_estadias);
    printf("Estadia cadastrada com sucesso\n");
}

// Função para dar baixa em uma estadia
void baixa_estadia(){
    Estadia estadia;
    int codigo;
    printf("Baixa de estadia\n\n");
    printf("Codigo da estadia: ");
    scanf("%d", &codigo);
    fseek(dados_estadias, 0, SEEK_SET);
    fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    while (!feof(dados_estadias)){
        if (estadia.codigo == codigo){
            if (estadia.com_baixa)
                return aviso("Esta estadia ja teve baixa.");
            if (!estadia.com_baixa){
                estadia.com_baixa = true;
                fseek(dados_estadias, sizeof(Estadia) * (estadia.codigo - 1), SEEK_SET);
                fwrite(&estadia, sizeof(Estadia), 1, dados_estadias);
                fflush(dados_estadias);
                return aviso("Baixa realizada com sucesso.");
            }
        } 
        fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    } 
    return aviso("Estadia com esse codigo nao foi encontrada.");
}

// Função para pesquisar um cliente pelo nome
void pesquisar_cliente(){
    Cliente cliente;
    char nome[50];
    printf("Pesquisa de cliente\n\n");
    printf("Nome: ");
    gets(nome);
    if (!verificar_cliente_existente(nome))
        printf("\nNao foi encontrado um cliente com esse nome.\n");
    else{
        cliente = buscar_cliente_pelo_nome(nome);
        printf("\nNome: %s\n", cliente.nome);
        printf("Telefone: %s\n", cliente.telefone);
        printf("Endereco: %s\n", cliente.endereco);
        printf("Codigo: %d\n", cliente.codigo);
    }
}

// Função para pesquisar um funcionário pelo nome
void pesquisar_funcionario(){
    Funcionario funcionario;
    char nome[50];
    printf("Pesquisa de funcionario\n\n");
    printf("Nome: ");
    gets(nome);
    if (!verificar_funcionario_existente(nome))
        printf("\nNao foi encontrado um funcionario com esse nome.\n");
    else{
        funcionario = buscar_funcionario_pelo_nome(nome);
        printf("\nNome: %s\n", funcionario.nome);
        printf("Telefone: %s\n", funcionario.telefone);
        printf("Cargo: %s\n", funcionario.cargo);
        printf("Salario: R$%.2f\n", funcionario.salario);
        printf("Codigo: %d\n", funcionario.codigo);
    }
}

// Função para listar as estadias de um cliente
void listar_estadias_cliente(){
    Cliente cliente;
    Estadia estadia;
    bool existe;
    char nome[50];
    char data[32];
    printf("Lista de estadias de um cliente\n\n");
    printf("Nome: ");
    gets(nome);

    if (!verificar_cliente_existente(nome))
        return aviso("Nao foi encontrado um cliente com esse nome.");
    cliente = buscar_cliente_pelo_nome(nome);
    printf("Codigo do cliente: %d\n", cliente.codigo);

    fseek(dados_estadias, 0, SEEK_SET);
    fread(&estadia, sizeof(Estadia), 1, dados_estadias);

    while (!feof(dados_estadias)){
        if (estadia.codigo_cliente == cliente.codigo){
            existe = true;
            strftime(data, 32, "%d/%m/%Y", &estadia.data_entrada);
            printf("\nData de entrada: %s\n", data);
            strftime(data, 32, "%d/%m/%Y", &estadia.data_saida);
            printf("Data de saida: %s\n", data);
            printf("Numero do quarto: %d\n", estadia.numero_quarto);
            printf("Quantidade de diarias: %d\n", estadia.quantidade_diarias);
            printf("Com baixa: %s\n", estadia.com_baixa ? "Sim" : "Nao");
            printf("Codigo da estadia: %d\n", estadia.codigo);
        };
        fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    }
    if (!existe)
        printf("Este cliente nao possui estadias cadastradas");
}

// Função para exibir os pontos de fidelidade de um cliente
void pontos_fidelidade_cliente(){
    Cliente cliente;
    Estadia estadia;
    char nome[50];
    int pontos = 0;
    printf("Pontos de fidelidade um cliente\n\n");
    printf("Nome: ");
    gets(nome);

    if (!verificar_cliente_existente(nome))
        return aviso("Nao foi encontrado um cliente com esse nome.");
    cliente = buscar_cliente_pelo_nome(nome);

    fseek(dados_estadias, 0, SEEK_SET);
    fread(&estadia, sizeof(Estadia), 1, dados_estadias);

    while (!feof(dados_estadias)){
        if (estadia.codigo_cliente == cliente.codigo)
        {
            pontos += estadia.quantidade_diarias * 10;
        };
        fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    }
    printf("\nPontos de fidelidade: %d", pontos);
}

// Função para verificar o formato da data
bool verificar_formato_data(Data data){
    DataTempo dataTempo = buscar_data(data, 0);
    return mktime(&dataTempo) != -1;
}

// Função para buscar uma data
DataTempo buscar_data(Data data, int horas){
    DataTempo dataTempo;
    dataTempo.tm_mday = data.dia;
    dataTempo.tm_mon = data.mes - 1;
    dataTempo.tm_year = data.ano - 1900;
    dataTempo.tm_hour = horas;
    dataTempo.tm_min = 0;
    return dataTempo;
}

// Função para verificar se um cliente existe
bool verificar_cliente_existente(char *nome){
    Cliente cliente;
    bool existe = false;
    fseek(dados_clientes, 0, SEEK_SET);
    fread(&cliente, sizeof(Cliente), 1, dados_clientes);
    while (!feof(dados_clientes) && !existe){
        if (strcmpi(cliente.nome, nome) == 0)
            existe = true;
        fread(&cliente, sizeof(Cliente), 1, dados_clientes);
    }
    return existe;
}

// Função para buscar um cliente pelo nome
Cliente buscar_cliente_pelo_nome(char *nome){
    Cliente cliente;
    fseek(dados_clientes, 0, SEEK_SET);
    fread(&cliente, sizeof(Cliente), 1, dados_clientes);
    while (!feof(dados_clientes)){
        if (strcmpi(cliente.nome, nome) == 0)
            break;
        fread(&cliente, sizeof(Cliente), 1, dados_clientes);
    }
    return cliente;
}

// Função para verificar se um funcionário existe
bool verificar_funcionario_existente(char *nome){
    Funcionario funcionario;
    bool existe = false;
    fseek(dados_funcionarios, 0, SEEK_SET);
    fread(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);
    while (!feof(dados_funcionarios) && !existe){
        if (strcmpi(funcionario.nome, nome) == 0)
            existe = true;
        fread(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);
    }
    return existe;
}

// Função para buscar um funcionário pelo nome
Funcionario buscar_funcionario_pelo_nome(char *nome){
    Funcionario funcionario;
    fseek(dados_funcionarios, 0, SEEK_SET);
    fread(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);
    while (!feof(dados_funcionarios)){
        if (strcmpi(funcionario.nome, nome) == 0)
            break;
        fread(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);
    }
    return funcionario;
}

// Função para verificar se um quarto existe
bool verificar_quarto_existente(int numero){
    Quarto quarto;
    bool existe = false;
    fseek(dados_quartos, 0, SEEK_SET);
    fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    while (!feof(dados_quartos) && !existe){
        if (quarto.numero == numero)
            existe = true;
        fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    }
    return existe;
}

// Função para buscar um quarto pelo número
Quarto buscar_quarto_pelo_numero(int numero){
    Quarto quarto;
    fseek(dados_quartos, 0, SEEK_SET);
    fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    while (!feof(dados_quartos)){
        if (quarto.numero == numero)
            break;
        fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    }
    return quarto;
}

// Função para buscar quartos disponíveis
VetorQuartos buscar_quartos_disponiveis(int hospedes, DataTempo entrada, DataTempo saida){
    Estadia estadia;
    Quarto quarto;
    VetorQuartos quartos;
    int quantidade = 0;
    fseek(dados_quartos, 0, SEEK_END);
    quartos.tamanho = ftell(dados_quartos) / sizeof(Quarto);
    quartos.vetor = malloc(ftell(dados_quartos));
    fseek(dados_quartos, 0, SEEK_SET);
    fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    while (!feof(dados_quartos)){
        quartos.vetor[quantidade++] = quarto;
        fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    }
    fseek(dados_estadias, 0, SEEK_SET);
    fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    while (!feof(dados_estadias)){
        quarto = pesquisar_quarto_vetor(quartos, estadia.numero_quarto);
        if (difftime(mktime(&estadia.data_entrada), mktime(&saida)) / DIAS <= 0 &&
            difftime(mktime(&entrada), mktime(&estadia.data_saida)) / DIAS <= 0 &&
            !estadia.com_baixa)
            remover_quarto_vetor(&quartos, estadia.numero_quarto);
        fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    }
    return quartos;
}

// Função para verificar se um quarto existe em um vetor
bool verificar_quarto_vetor(VetorQuartos vetorQuartos, int numero){
    int i;
    for (i = 0; i < vetorQuartos.tamanho; ++i)
        if (vetorQuartos.vetor[i].numero == numero)
            return true;
    return false;
}

// Função para pesquisar um quarto em um vetor
Quarto pesquisar_quarto_vetor(VetorQuartos vetorQuartos, int numero){
    Quarto quarto;
    int i;
    for (i = 0; i < vetorQuartos.tamanho; ++i)
        if (vetorQuartos.vetor[i].numero == numero){
            quarto = vetorQuartos.vetor[i];
            break;
        }
    return quarto;
}

// Função para remover um quarto de um vetor
void remover_quarto_vetor(VetorQuartos *vetorQuartos, int numero){
    int i;
    bool achou;
    if (vetorQuartos->vetor[vetorQuartos->tamanho - 1].numero == numero)
        achou = true;
    else{
        for (i = 0; i < vetorQuartos->tamanho - 1 && !achou; ++i)
            if (vetorQuartos->vetor[i].numero == numero)
                achou = true;
        for (i = i - 1; i < vetorQuartos->tamanho - 1; ++i)
            vetorQuartos->vetor[i] = vetorQuartos->vetor[i + 1];
    }
    if (achou){
        --vetorQuartos->tamanho;
        realloc(vetorQuartos->vetor, vetorQuartos->tamanho * sizeof(Quarto));
    }
}

// Função para verificar se uma string contém pelo menos uma letra e um número
bool validar_endereco(const char *endereco) {
    int tem_letra = 0, tem_numero = 0;
    for (int i = 0; endereco[i] != '\0'; i++) {
        if (isalpha(endereco[i])) tem_letra = 1;
        if (isdigit(endereco[i])) tem_numero = 1;
    }
    return tem_letra && tem_numero;
}

// Função para verificar se uma string contém exatamente 11 dígitos
bool validar_telefone(const char *telefone) {
    if (strlen(telefone) != 11) return 0;
    for (int i = 0; telefone[i] != '\0'; i++) {
        if (!isdigit(telefone[i])) return 0;
    }
    return 1;
}