#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <locale.h>

#define HORAS 3600
#define DIAS 86400

FILE *dados_clientes;
FILE *dados_funcionarios;
FILE *dados_estadias;
FILE *dados_quartos;

int main() {
    char opcao;

    setlocale(LC_ALL, "UTF-8");

    dados_clientes = fopen("dados_clientes.dat", "rb+");
    if (dados_clientes == NULL) dados_clientes = fopen("dados_clientes.dat", "wb+");

    dados_funcionarios = fopen("dados_funcionarios.dat", "rb+");
    if (dados_funcionarios == NULL) dados_funcionarios = fopen("dados_funcionarios.dat", "wb+");

    dados_estadias = fopen("dados_estadias.dat", "rb+");
    if (dados_estadias == NULL) dados_estadias = fopen("dados_estadias.dat", "wb+");

    dados_quartos = fopen("dados_quartos.dat", "rb+");
    if (dados_quartos == NULL) dados_quartos = fopen("dados_quartos.dat", "wb+");

    do {
        system("cls");
        menu();

        printf("\nSelecione a opção\n");
        opcao = getch();
        fflush(stdin);

        system("cls");

        switch (opcao - '0') {
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

        }

        getch();
    } while (opcao != '0');

    return 0;
}

void aviso(char *mensagem) {
    printf("\n%s\n", mensagem);
}

void menu() {
    printf("0 - Sair\n");
    printf("1 - Cadastrar cliente\n");
    printf("2 - Cadastrar funcionário\n");
    printf("3 - Cadastrar quarto\n");
    printf("4 - Cadastrar estadia\n");
    printf("5 - Dar baixa na estadia\n");
    printf("6 - Pesquisar cliente\n");
    printf("7 - Pesquisar funcionário\n");
    printf("8 - Listar estadias de um cliente\n");
    printf("9 - Exibir pontos de fidelidade um cliente\n");
}

void cadastrar_cliente() {
    Cliente cliente;

    printf("Cadastro de cliente\n\n");

    printf("Nome: ");
    gets(cliente.nome);
    if (verificar_cliente_existente(cliente.nome))
        return aviso("Já existe um cliente com esse nome.");

    printf("Endereço: ");
    gets(cliente.endereco);

    printf("Telefone: ");
    gets(cliente.telefone);

    fseek(dados_clientes, 0, SEEK_END);
    cliente.codigo = ftell(dados_clientes) / sizeof(Cliente) + 1;
    fwrite(&cliente, sizeof(Cliente), 1, dados_clientes);
    fflush(dados_clientes);
}

void cadastrar_funcionario() {
    Funcionario funcionario;
    printf("Cadastro de funcionário\n\n");

    printf("Nome: ");
    gets(funcionario.nome);
    if (verificar_funcionario_existente(funcionario.nome))
        return aviso("Já existe um funcionário com esse nome.");

    printf("Telefone: ");
    gets(funcionario.telefone);

    printf("\nSelecione um cargo\n");
    printf("  1 - Recepcionista\n");
    printf("  2 - Auxiliar de limpeza\n");
    printf("  3 - Garçom\n");
    printf("  4 - Gerente\n");
    switch (getch() - '0') {
        case 1:
            strcpy(funcionario.cargo, "Recepcionista");
            break;
        case 2:
            strcpy(funcionario.cargo, "Auxiliar de limpeza");
            break;
        case 3:
            strcpy(funcionario.cargo, "Garçom");
            break;
        case 4:
            strcpy(funcionario.cargo, "Gerente");
            break;
        default:
            return aviso("Opção de cargo inválida.");
    }
    printf("Cargo: %s\n\n", funcionario.cargo);

    printf("Salário: ");
    scanf("%f", &funcionario.salario);

    fseek(dados_funcionarios, 0, SEEK_END);
    funcionario.codigo = ftell(dados_funcionarios) / sizeof(Funcionario) + 1;
    fwrite(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);
    fflush(dados_funcionarios);
}

void cadastrar_quarto() {
    Quarto quarto;
    printf("Cadastro de quarto\n\n");

    printf("Numero do quarto: ");
    scanf("%d", &quarto.numero);
    if (verificar_quarto_existente(quarto.numero))
        return aviso("Já existe um quarto com esse número.");

    printf("Quantidade de hóspedes: ");
    scanf("%d", &quarto.quantidade_hospedes);

    printf("Valor diária: ");
    scanf("%f", &quarto.valor_diaria);

    fflush(stdin);

    fseek(dados_quartos, 0, SEEK_END);
    fwrite(&quarto, sizeof(Quarto), 1, dados_quartos);
    fflush(dados_quartos);
}

void cadastrar_estadia() {
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
        return aviso("Não foi encontrado um cliente com esse nome.");
    cliente = buscar_cliente_pelo_nome(nome_cliente);
    estadia.codigo_cliente = cliente.codigo;

    printf("Data de entrada (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
    if (!verificar_formato_data(data))
        return aviso("Data inválida.");
    estadia.data_entrada = buscar_data(data, 14);

    printf("Data de saida (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
    if (!verificar_formato_data(data))
        return aviso("Data inválida.");
    estadia.data_saida = buscar_data(data, 12);
    if (difftime(mktime(&estadia.data_saida), mktime(&estadia.data_entrada)) / HORAS < 22)
        return aviso("A data de saída deve ser no mínimo um dia após a data de entrada.");

    printf("Quantidade de hóspedes: ");
    scanf("%d", &quantidade_hospedes);

    quartos_disponiveis = buscar_quartos_disponiveis(quantidade_hospedes, estadia.data_entrada, estadia.data_saida);
    printf("Quartos disponíveis:\t");
    if (quartos_disponiveis.tamanho == 0)
        return aviso("Não existe quartos disponíveis.");

    for (i = 0; i < quartos_disponiveis.tamanho; ++i) {
        printf("%d\t", quartos_disponiveis.vetor[i].numero);
        if ((i + 1) % 5 == 0 && i != quartos_disponiveis.tamanho - 1) printf("\n\t\t\t");
    }
    printf("\n");

    printf("Número do quarto: ");
    scanf("%d", &estadia.numero_quarto);
    if (!verificar_quarto_vetor(quartos_disponiveis, estadia.numero_quarto))
        return aviso("Não foi encontrado um quarto desocupado com esse número.");

    estadia.quantidade_diarias = (difftime(mktime(&estadia.data_saida), mktime(&estadia.data_entrada))
                                  + (estadia.data_entrada.tm_hour - estadia.data_saida.tm_hour) * HORAS) / DIAS;

    estadia.com_baixa = false;

    fseek(dados_estadias, 0, SEEK_END);
    estadia.codigo = ftell(dados_estadias) / sizeof(Estadia) + 1;
    fwrite(&estadia, sizeof(Estadia), 1, dados_estadias);
    fflush(dados_estadias);
}

void baixa_estadia() {
    Estadia estadia;
    int codigo;

    printf("Baixa de estadia\n\n");

    printf("Código da estadia: ");
    scanf("%d", &codigo);

    fseek(dados_estadias, 0, SEEK_SET);
    fread(&estadia, sizeof(Estadia), 1, dados_estadias);

    while (!feof(dados_estadias)) {
        if (estadia.codigo == codigo) {
            if (estadia.com_baixa)
                return aviso("Esta estadia já teve baixa.");
            if (!estadia.com_baixa) {
                estadia.com_baixa = true;
                fseek(dados_estadias, sizeof(Estadia) * (estadia.codigo - 1), SEEK_SET);
                fwrite(&estadia, sizeof(Estadia), 1, dados_estadias);
                fflush(dados_estadias);
                return aviso("Baixa realizada com sucesso.");
            }
        }
        fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    }

    return aviso("Estadia com esse código não foi encontrada.");
}

void pesquisar_cliente() {
    Cliente cliente;
    char nome[50];

    printf("Pesquisa de cliente\n\n");

    printf("Nome: ");
    gets(nome);

    if (!verificar_cliente_existente(nome))
        printf("\nNão foi encontrado um cliente com esse nome.\n");
    else {
        cliente = buscar_cliente_pelo_nome(nome);

        printf("\nNome: %s\n", cliente.nome);
        printf("Telefone: %s\n", cliente.telefone);
        printf("Endereço: %s\n", cliente.endereco);
        printf("Código: %d\n", cliente.codigo);
    }
}

void pesquisar_funcionario() {
    Funcionario funcionario;
    char nome[50];

    printf("Pesquisa de funcionário\n\n");

    printf("Nome: ");
    gets(nome);

    if (!verificar_funcionario_existente(nome))
        printf("\nNão foi encontrado um funcionário com esse nome.\n");
    else {
        funcionario = buscar_funcionario_pelo_nome(nome);

        printf("\nNome: %s\n", funcionario.nome);
        printf("Telefone: %s\n", funcionario.telefone);
        printf("Cargo: %s\n", funcionario.cargo);
        printf("Salário: R$%.2f\n", funcionario.salario);
        printf("Código: %d\n", funcionario.codigo);
    }
}

void listar_estadias_cliente() {
    Cliente cliente;
    Estadia estadia;
    bool existe;
    char nome[50];
    char data[32];

    printf("Lista de estadias de um cliente\n\n");

    printf("Nome: ");
    gets(nome);

    if (!verificar_cliente_existente(nome))
        return aviso("Não foi encontrado um cliente com esse nome.");
    cliente = buscar_cliente_pelo_nome(nome);
    printf("Código do cliente: %d\n", cliente.codigo);

    fseek(dados_estadias, 0, SEEK_SET);
    fread(&estadia, sizeof(Estadia), 1, dados_estadias);

    while (!feof(dados_estadias)) {
        if (estadia.codigo_cliente == cliente.codigo) {
            existe = true;

            strftime(data, 32, "%d/%m/%Y", &estadia.data_entrada);
            printf("\nData de entrada: %s\n", data);
            strftime(data, 32, "%d/%m/%Y", &estadia.data_saida);
            printf("Data de saída: %s\n", data);
            printf("Número do quarto: %d\n", estadia.numero_quarto);
            printf("Quantidade de diárias: %d\n", estadia.quantidade_diarias);
            printf("Com baixa: %s\n", estadia.com_baixa ? "Sim" : "Não");
            printf("Código da estadia: %d\n", estadia.codigo);
        };
        fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    }

    if (!existe)
        printf("Este cliente não possui estadias cadastradas");
}

void pontos_fidelidade_cliente() {
    Cliente cliente;
    Estadia estadia;
    char nome[50];
    int pontos = 0;

    printf("Pontos de fidelidade um cliente\n\n");

    printf("Nome: ");
    gets(nome);

    if (!verificar_cliente_existente(nome))
        return aviso("Não foi encontrado um cliente com esse nome.");
    cliente = buscar_cliente_pelo_nome(nome);

    fseek(dados_estadias, 0, SEEK_SET);
    fread(&estadia, sizeof(Estadia), 1, dados_estadias);

    while (!feof(dados_estadias)) {
        if (estadia.codigo_cliente == cliente.codigo) {
            pontos += estadia.quantidade_diarias * 10;
        };
        fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    }

    printf("\nPontos de fidelidade: %d", pontos);
}

bool verificar_formato_data(Data data) {
    DataTempo dataTempo = buscar_data(data, 0);
    return mktime(&dataTempo) != -1;
}

DataTempo buscar_data(Data data, int horas) {
    DataTempo dataTempo;

    dataTempo.tm_mday = data.dia;
    dataTempo.tm_mon = data.mes - 1;
    dataTempo.tm_year = data.ano - 1900;
    dataTempo.tm_hour = horas;
    dataTempo.tm_min = 0;

    return dataTempo;
}

bool verificar_cliente_existente(char *nome) {
    Cliente cliente;
    bool existe = false;

    fseek(dados_clientes, 0, SEEK_SET);
    fread(&cliente, sizeof(Cliente), 1, dados_clientes);

    while (!feof(dados_clientes) && !existe) {
        if (strcmpi(cliente.nome, nome) == 0) existe = true;
        fread(&cliente, sizeof(Cliente), 1, dados_clientes);
    }

    return existe;
}

Cliente buscar_cliente_pelo_nome(char *nome) {
    Cliente cliente;

    fseek(dados_clientes, 0, SEEK_SET);
    fread(&cliente, sizeof(Cliente), 1, dados_clientes);

    while (!feof(dados_clientes)) {
        if (strcmpi(cliente.nome, nome) == 0) break;
        fread(&cliente, sizeof(Cliente), 1, dados_clientes);
    }

    return cliente;
}

bool verificar_funcionario_existente(char *nome) {
    Funcionario funcionario;
    bool existe = false;

    fseek(dados_funcionarios, 0, SEEK_SET);
    fread(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);

    while (!feof(dados_funcionarios) && !existe) {
        if (strcmpi(funcionario.nome, nome) == 0) existe = true;
        fread(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);
    }

    return existe;
}

Funcionario buscar_funcionario_pelo_nome(char *nome) {
    Funcionario funcionario;

    fseek(dados_funcionarios, 0, SEEK_SET);
    fread(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);

    while (!feof(dados_funcionarios)) {
        if (strcmpi(funcionario.nome, nome) == 0) break;
        fread(&funcionario, sizeof(Funcionario), 1, dados_funcionarios);
    }

    return funcionario;
}

bool verificar_quarto_existente(int numero) {
    Quarto quarto;
    bool existe = false;

    fseek(dados_quartos, 0, SEEK_SET);
    fread(&quarto, sizeof(Quarto), 1, dados_quartos);

    while (!feof(dados_quartos) && !existe) {
        if (quarto.numero == numero) existe = true;
        fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    }

    return existe;
}

Quarto buscar_quarto_pelo_numero(int numero) {
    Quarto quarto;

    fseek(dados_quartos, 0, SEEK_SET);
    fread(&quarto, sizeof(Quarto), 1, dados_quartos);

    while (!feof(dados_quartos)) {
        if (quarto.numero == numero) break;
        fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    }

    return quarto;
}

VetorQuartos buscar_quartos_disponiveis(int hospedes, DataTempo entrada, DataTempo saida) {
    Estadia estadia;
    Quarto quarto;
    VetorQuartos quartos;
    int quantidade = 0;

    fseek(dados_quartos, 0, SEEK_END);
    quartos.tamanho = ftell(dados_quartos) / sizeof(Quarto);
    quartos.vetor = malloc(ftell(dados_quartos));

    fseek(dados_quartos, 0, SEEK_SET);
    fread(&quarto, sizeof(Quarto), 1, dados_quartos);

    while (!feof(dados_quartos)) {
        quartos.vetor[quantidade++] = quarto;
        fread(&quarto, sizeof(Quarto), 1, dados_quartos);
    }

    fseek(dados_estadias, 0, SEEK_SET);
    fread(&estadia, sizeof(Estadia), 1, dados_estadias);

    while (!feof(dados_estadias)) {
        quarto = pesquisar_quarto_vetor(quartos, estadia.numero_quarto);

        if (difftime(mktime(&estadia.data_entrada), mktime(&saida)) / DIAS <= 0 &&
            difftime(mktime(&entrada), mktime(&estadia.data_saida)) / DIAS <= 0 &&
            !estadia.com_baixa)
            remover_quarto_vetor(&quartos, estadia.numero_quarto);
        fread(&estadia, sizeof(Estadia), 1, dados_estadias);
    }

    return quartos;
}

bool verificar_quarto_vetor(VetorQuartos vetorQuartos, int numero) {
    int i;

    for (i = 0; i < vetorQuartos.tamanho; ++i)
        if (vetorQuartos.vetor[i].numero == numero)
            return true;
    return false;
}

Quarto pesquisar_quarto_vetor(VetorQuartos vetorQuartos, int numero) {
    Quarto quarto;
    int i;

    for (i = 0; i < vetorQuartos.tamanho; ++i)
        if (vetorQuartos.vetor[i].numero == numero) {
            quarto = vetorQuartos.vetor[i];
            break;
        }

    return quarto;
}

void remover_quarto_vetor(VetorQuartos *vetorQuartos, int numero) {
    int i;
    bool achou;

    if (vetorQuartos->vetor[vetorQuartos->tamanho - 1].numero == numero)
        achou = true;
    else {
        for (i = 0; i < vetorQuartos->tamanho - 1 && !achou; ++i)
            if (vetorQuartos->vetor[i].numero == numero)
                achou = true;

        for (i = i - 1; i < vetorQuartos->tamanho - 1; ++i)
            vetorQuartos->vetor[i] = vetorQuartos->vetor[i + 1];
    }

    if (achou) {
        --vetorQuartos->tamanho;
        realloc(vetorQuartos->vetor, vetorQuartos->tamanho * sizeof(Quarto));
    }
}
