#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_CLIENTES 100
#define MAX_FUNCIONARIOS 50
#define MAX_QUARTOS 30
#define MAX_ESTADIAS 200

typedef struct {
    int codigo;
    char nome[100];
    char endereco[200];
    char telefone[15];
} Cliente;

typedef struct {
    int codigo;
    char nome[100];
    char telefone[15];
    char cargo[50];
    float salario;
} Funcionario;

typedef struct {
    int numero;
    int quantidadeHospedes;
    float valorDiaria;
    char status[10];
} Quarto;

typedef struct {
    int codigo;
    time_t dataEntrada;
    time_t dataSaida;
    int quantidadeDiarias;
    int codigoCliente;
    int numeroQuarto;
} Estadia;

Cliente clientes[MAX_CLIENTES];
Funcionario funcionarios[MAX_FUNCIONARIOS];
Quarto quartos[MAX_QUARTOS];
Estadia estadias[MAX_ESTADIAS];

int clienteCount = 0;
int funcionarioCount = 0;
int quartoCount = 0;
int estadiaCount = 0;

void inicializarDados();
void salvarDados();
void carregarDados();
bool cadastrarCliente(char nome[], char endereco[], char telefone[]);
bool cadastrarFuncionario(char nome[], char telefone[], char cargo[], float salario);
bool cadastrarQuarto(int numero, int quantidadeHospedes, float valorDiaria);
bool cadastrarEstadia(int codigoCliente, int quantidadeHospedes, time_t dataEntrada, time_t dataSaida);
float darBaixaEstadia(int codigoEstadia);
Cliente* pesquisarCliente(char *nome);
Funcionario* pesquisarFuncionario(char *nome);
void mostrarEstadiasCliente(int codigoCliente);
int calcularPontosFidelidade(int codigoCliente);
void mostrarMenu();
int calcularDiarias(time_t dataEntrada, time_t dataSaida);

void inicializarDados() {
    carregarDados();
}

void salvarDados() {
    FILE *file;

    file = fopen("clientes.txt", "wb");
    fwrite(&clienteCount, sizeof(int), 1, file);
    fwrite(clientes, sizeof(Cliente), clienteCount, file);
    fclose(file);

    file = fopen("funcionarios.txt", "wb");
    fwrite(&funcionarioCount, sizeof(int), 1, file);
    fwrite(funcionarios, sizeof(Funcionario), funcionarioCount, file);
    fclose(file);

    file = fopen("quartos.txt", "wb");
    fwrite(&quartoCount, sizeof(int), 1, file);
    fwrite(quartos, sizeof(Quarto), quartoCount, file);
    fclose(file);

    file = fopen("estadias.txt", "wb");
    fwrite(&estadiaCount, sizeof(int), 1, file);
    fwrite(estadias, sizeof(Estadia), estadiaCount, file);
    fclose(file);
}

void carregarDados() {
    FILE *file;

    file = fopen("clientes.txt", "rb");
    if (file != NULL) {
        fread(&clienteCount, sizeof(int), 1, file);
        fread(clientes, sizeof(Cliente), clienteCount, file);
        fclose(file);
    }

    file = fopen("funcionarios.txt", "rb");
    if (file != NULL) {
        fread(&funcionarioCount, sizeof(int), 1, file);
        fread(funcionarios, sizeof(Funcionario), funcionarioCount, file);
        fclose(file);
    }

    file = fopen("quartos.txt", "rb");
    if (file != NULL) {
        fread(&quartoCount, sizeof(int), 1, file);
        fread(quartos, sizeof(Quarto), quartoCount, file);
        fclose(file);
    }

    file = fopen("estadias.txt", "rb");
    if (file != NULL) {
        fread(&estadiaCount, sizeof(int), 1, file);
        fread(estadias, sizeof(Estadia), estadiaCount, file);
        fclose(file);
    }
}

bool cadastrarCliente(char nome[], char endereco[], char telefone[]) {
    for (int i = 0; i < clienteCount; i++) {
        if (strcmp(clientes[i].nome, nome) == 0) {
            return false; // Cliente já existe
        }
    }
    clientes[clienteCount].codigo = clienteCount + 1;
    strcpy(clientes[clienteCount].nome, nome);
    strcpy(clientes[clienteCount].endereco, endereco);
    strcpy(clientes[clienteCount].telefone, telefone);
    clienteCount++;
    salvarDados();
    return true;
}

bool cadastrarFuncionario(char nome[], char telefone[], char cargo[], float salario) {
    for (int i = 0; i < funcionarioCount; i++) {
        if (strcmp(funcionarios[i].nome, nome) == 0) {
            return false; // Funcionário já existe
        }
    }
    funcionarios[funcionarioCount].codigo = funcionarioCount + 1;
    strcpy(funcionarios[funcionarioCount].nome, nome);
    strcpy(funcionarios[funcionarioCount].telefone, telefone);
    strcpy(funcionarios[funcionarioCount].cargo, cargo);
    funcionarios[funcionarioCount].salario = salario;
    funcionarioCount++;
    salvarDados();
    return true;
}

bool cadastrarQuarto(int numero, int quantidadeHospedes, float valorDiaria) {
    for (int i = 0; i < quartoCount; i++) {
        if (quartos[i].numero == numero) {
            return false; // Quarto já existe
        }
    }
    quartos[quartoCount].numero = numero;
    quartos[quartoCount].quantidadeHospedes = quantidadeHospedes;
    quartos[quartoCount].valorDiaria = valorDiaria;
    strcpy(quartos[quartoCount].status, "desocupado");
    quartoCount++;
    salvarDados();
    return true;
}

bool cadastrarEstadia(int codigoCliente, int quantidadeHospedes, time_t dataEntrada, time_t dataSaida) {
    Cliente *cliente = NULL;
    for (int i = 0; i < clienteCount; i++) {
        if (clientes[i].codigo == codigoCliente) {
            cliente = &clientes[i];
            break;
        }
    }
    if (cliente == NULL) return false; // Cliente não encontrado

    Quarto *quarto = NULL;
    for (int i = 0; i < quartoCount; i++) {
        if (quartos[i].quantidadeHospedes >= quantidadeHospedes && strcmp(quartos[i].status, "desocupado") == 0) {
            quarto = &quartos[i];
            break;
        }
    }
    if (quarto == NULL) return false; // Quarto adequado não encontrado

    for (int i = 0; i < estadiaCount; i++) {
        if (estadias[i].numeroQuarto == quarto->numero &&
            ((dataEntrada >= estadias[i].dataEntrada && dataEntrada < estadias[i].dataSaida) ||
             (dataSaida > estadias[i].dataEntrada && dataSaida <= estadias[i].dataSaida))) {
            return false; // Conflito de reservas
        }
    }

    estadias[estadiaCount].codigo = estadiaCount + 1;
    estadias[estadiaCount].dataEntrada = dataEntrada;
    estadias[estadiaCount].dataSaida = dataSaida;
    estadias[estadiaCount].quantidadeDiarias = calcularDiarias(dataEntrada, dataSaida);
    estadias[estadiaCount].codigoCliente = codigoCliente;
    estadias[estadiaCount].numeroQuarto = quarto->numero;
    strcpy(quarto->status, "ocupado");
    estadiaCount++;
    salvarDados();
    return true;
}

float darBaixaEstadia(int codigoEstadia) {
    Estadia *estadia = NULL;
    for (int i = 0; i < estadiaCount; i++) {
        if (estadias[i].codigo == codigoEstadia) {
            estadia = &estadias[i];
            break;
        }
    }
    if (estadia == NULL) return -1; // Estadia não encontrada

    Quarto *quarto = NULL;
    for (int i = 0; i < quartoCount; i++) {
        if (quartos[i].numero == estadia->numeroQuarto) {
            quarto = &quartos[i];
            break;
        }
    }
    if (quarto == NULL) return -1; // Quarto não encontrado

    float valorTotal = estadia->quantidadeDiarias * quarto->valorDiaria;
    strcpy(quarto->status, "desocupado");
    
    // Marcar estadia como "removida"
    estadia->codigo = -1;
    salvarDados();
    return valorTotal;
}

Cliente* pesquisarCliente(char *nome) {
    for (int i = 0; i < clienteCount; i++) {
        if (strcmp(clientes[i].nome, nome) == 0) {
            return &clientes[i];
        }
    }
    return NULL;
}

Funcionario* pesquisarFuncionario(char *nome) {
    for (int i = 0; i < funcionarioCount; i++) {
        if (strcmp(funcionarios[i].nome, nome) == 0) {
            return &funcionarios[i];
        }
    }
    return NULL;
}

void mostrarEstadiasCliente(int codigoCliente) {
    for (int i = 0; i < estadiaCount; i++) {
        if (estadias[i].codigoCliente == codigoCliente) {
            printf("Estadia %d: Quarto %d, Entrada %s, Saída %s, Diárias %d\n",
                    estadias[i].codigo,
                    estadias[i].numeroQuarto,
                    ctime(&estadias[i].dataEntrada),
                    ctime(&estadias[i].dataSaida),
                    estadias[i].quantidadeDiarias);
        }
    }
}

int calcularPontosFidelidade(int codigoCliente) {
    int pontos = 0;
    for (int i = 0; i < estadiaCount; i++) {
        if (estadias[i].codigoCliente == codigoCliente) {
            pontos += estadias[i].quantidadeDiarias * 10;
        }
    }
    return pontos;
}

void mostrarMenu() {
    printf("1. Cadastrar Cliente\n");
    printf("2. Cadastrar Funcionario\n");
    printf("3. Cadastrar Quarto\n");
    printf("4. Cadastrar Estadia\n");
    printf("5. Dar Baixa em Estadia\n");
    printf("6. Pesquisar Cliente\n");
    printf("7. Pesquisar Funcionario\n");
    printf("8. Mostrar Estadias Cliente\n");
    printf("9. Calcular Pontos Fidelidade\n");
    printf("0. Sair\n");
}

int calcularDiarias(time_t dataEntrada, time_t dataSaida) {
    struct tm *tmEntrada = localtime(&dataEntrada);
    struct tm *tmSaida = localtime(&dataSaida);
    int diarias = 0;
    if (tmSaida->tm_year == tmEntrada->tm_year) {
        diarias = tmSaida->tm_yday - tmEntrada->tm_yday;
    } else {
        diarias = (tmSaida->tm_yday + (365 * (tmSaida->tm_year - tmEntrada->tm_year))) - tmEntrada->tm_yday;
    }
    if (tmEntrada->tm_hour < 14) {
        diarias++; // Entrada antes das 14h, ganha uma diária adicional
    }
    if (tmSaida->tm_hour > 12) {
        diarias++; // Saída após as 12h, paga uma diária adicional
    }
    return diarias;
}

int main() {
    inicializarDados();
    int opcao;
    do {
        mostrarMenu();
        scanf("%d", &opcao);
        getchar(); // Limpeza do buffer

        char nome[100];
        char endereco[200];
        char telefone[15];
        char cargo[50];
        int quantidadeHospedes;
        float salario, valorDiaria;
        int numero, codigo, codigoEstadia;
        time_t dataEntrada, dataSaida;
        struct tm tmData;
        char dataStr[20];

        switch (opcao) {
            case 1:
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                printf("Endereco: ");
                fgets(endereco, sizeof(endereco), stdin);
                endereco[strcspn(endereco, "\n")] = '\0';
                printf("Telefone: ");
                fgets(telefone, sizeof(telefone), stdin);
                telefone[strcspn(telefone, "\n")] = '\0';
                if (!cadastrarCliente(nome, endereco, telefone)) {
                    printf("Erro ao cadastrar cliente. Cliente já existe.\n");
                } else {
                    printf("Cliente cadastrado com sucesso!\n");
                }
                break;
            case 2:
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                printf("Telefone: ");
                fgets(telefone, sizeof(telefone), stdin);
                telefone[strcspn(telefone, "\n")] = '\0';
                printf("Cargo: ");
                fgets(cargo, sizeof(cargo), stdin);
                cargo[strcspn(cargo, "\n")] = '\0';
                printf("Salario (ex: 2500.50): ");
                scanf("%f", &salario);
                getchar();
                if (!cadastrarFuncionario(nome, telefone, cargo, salario)) {
                    printf("Erro ao cadastrar funcionário. Funcionário já existe.\n");
                } else {
                    printf("Funcionário cadastrado com sucesso!\n");
                }
                break;
            case 3:
                printf("Número: ");
                scanf("%d", &numero);
                printf("Quantidade de hóspedes: ");
                scanf("%d", &quantidadeHospedes);
                printf("Valor da diária: ");
                scanf("%f", &valorDiaria);
                getchar();
                if (!cadastrarQuarto(numero, quantidadeHospedes, valorDiaria)) {
                    printf("Erro ao cadastrar quarto. Quarto já existe.\n");
                } else {
                    printf("Quarto cadastrado com sucesso!\n");
                }
                break;
            case 4:
                printf("Código do cliente: ");
                scanf("%d", &codigo);
                printf("Quantidade de hóspedes: ");
                scanf("%d", &quantidadeHospedes);
                getchar(); // Limpeza do buffer

                printf("Data de entrada (dd/mm/yyyy): ");
                fgets(dataStr, sizeof(dataStr), stdin);
                sscanf(dataStr, "%d/%d/%d", &tmData.tm_mday, &tmData.tm_mon, &tmData.tm_year);
                tmData.tm_mon -= 1;
                tmData.tm_year -= 1900;
                tmData.tm_hour = 14;
                tmData.tm_min = 0;
                tmData.tm_sec = 0;
                dataEntrada = mktime(&tmData);

                printf("Data de saída (dd/mm/yyyy): ");
                fgets(dataStr, sizeof(dataStr), stdin);
                sscanf(dataStr, "%d/%d/%d", &tmData.tm_mday, &tmData.tm_mon, &tmData.tm_year);
                tmData.tm_mon -= 1;
                tmData.tm_year -= 1900;
                tmData.tm_hour = 12;
                tmData.tm_min = 0;
                tmData.tm_sec = 0;
                dataSaida = mktime(&tmData);

                if (dataEntrada >= dataSaida) {
                    printf("Erro: Data de entrada deve ser anterior à data de saída.\n");
                } else if (!cadastrarEstadia(codigo, quantidadeHospedes, dataEntrada, dataSaida)) {
                    printf("Erro ao cadastrar estadia. Cliente não existe ou quarto não disponível.\n");
                } else {
                    printf("Estadia cadastrada com sucesso!\n");
                }
                break;
            case 5:
                printf("Código da estadia: ");
                scanf("%d", &codigoEstadia);
                float valorTotal = darBaixaEstadia(codigoEstadia);
                if (valorTotal < 0) {
                    printf("Erro ao dar baixa na estadia. Estadia não encontrada.\n");
                } else {
                    printf("Valor total a ser pago: %.2f\n", valorTotal);
                }
                break;
            case 6:
                printf("Nome do cliente: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                Cliente *cliente = pesquisarCliente(nome);
                if (cliente) {
                    printf("Cliente encontrado: %s, %s, %s\n", cliente->nome, cliente->endereco, cliente->telefone);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            case 7:
                printf("Nome do funcionário: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                Funcionario *funcionario = pesquisarFuncionario(nome);
                if (funcionario) {
                    printf("Funcionário encontrado: %s, %s, %s, %.2f\n", funcionario->nome, funcionario->telefone, funcionario->cargo, funcionario->salario);
                } else {
                    printf("Funcionário não encontrado.\n");
                }
                break;
            case 8:
                printf("Código do cliente: ");
                scanf("%d", &codigo);
                mostrarEstadiasCliente(codigo);
                break;
            case 9:
                printf("Código do cliente: ");
                scanf("%d", &codigo);
                int pontos = calcularPontosFidelidade(codigo);
                printf("Pontos de fidelidade: %d\n", pontos);
                break;
            case 0:
                salvarDados();
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
