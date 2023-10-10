#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

struct Product /* Definição dos produtos*/
{
    int code;
    double price;
    wchar_t description[50];
};

struct Employee /* Definição dos vendedores*/
{
    int id;
    wchar_t name[50];
};

struct Sale /* Definição das vendas*/
{
    int employee;
    int product;
    int qtd;
};

int n_products;
int n_employees;
int n_sales;

struct Product products[100];
struct Employee employees[100];
struct Sale sales[100];

double ValorTotal;
double valorvendido;

void Obter(){
    FILE *file_products;
    file_products = fopen("produtos.txt", "r, ccs=UTF-8"); /* Lê o arquivo txt */

    if (file_products == NULL)
    {
        printf("Erro ao abrir o arquivo\n"); /* Erro caso nao exista o arquivo*/
        exit(1);
    }

    fwscanf(file_products, L"%d\n", &n_products);  /* Obtem o tamanho do file*/

    for (int i = 0; i < n_products; i++) /* for do tamanho do file*/
    {
        fwscanf(file_products, L"%d\n %lf\n", &products[i].code, &products[i].price);     /* armazena as informações*/
        fgetws(products[i].description, sizeof(products[i].description) / sizeof(wchar_t), file_products);
    }

    fclose(file_products); /* Salva o arquivo */

    FILE *file_employees;
    file_employees = fopen("vendedores.txt", "r, ccs=UTF-8");

    if (file_employees == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    fwscanf(file_employees, L"%d\n", &n_employees);

    for (int i = 0; i < n_employees; i++)
    {
        fwscanf(file_employees, L"%d\n", &employees[i].id);
        fgetws(employees[i].name, sizeof(employees[i].name) / sizeof(wchar_t), file_employees);
    }

    fclose(file_employees);

    FILE *file_sales;
    file_sales = fopen("vendas.txt", "r");

    if (file_sales == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    fscanf(file_sales, "%d\n", &n_sales);

    for (int i = 0; i < n_sales; i++)
    {
        fscanf(file_sales, "%d %d %d", &sales[i].employee, &sales[i].product, &sales[i].qtd);
    }

    fclose(file_sales);
}

void Imprimir(){
    FILE *totais_File;
    totais_File = fopen("totais.txt", "a");

}

void Produtos(){
    printf("\n-------------------\n");
    for (int i = 0; i < n_products; i++) /* for do tamanho do file*/
    {
        wprintf(L"%-3d %.2lf %s", products[i].code, products[i].price, products[i].description); /* Imprime as informações */
    }
    printf("\n-------------------\n");
}

void Vendedores(){
    printf("\n-----------\n");
    for (int i = 0; i < n_employees; i++)
    {
        wprintf(L"%d %s", employees[i].id, employees[i].name);
        printf("\n");
    }
    printf("-----------\n");
}

void Vendas(){
    printf("\n------\n");
    for (int i = 0; i < n_sales; i++)
    {
        wprintf(L"%d %d %d\n", sales[i].employee, sales[i].product, sales[i].qtd);
    }
    printf("------\n");
}

void TotalVendas(){
    for (int i = 0; i < n_sales; i++)        /* faz um for do tamanho do file de vendas*/
    {
        double valorvendido = products[sales[i].product - 1].price * sales[i].qtd;   /* Salva o valor vendido a uma var*/
        ValorTotal += valorvendido;               /* soma a nossa var*/
    }
    printf("\n---------------------------------\n");
    printf("Valor Total de Vendas: R$%.2lf\n", ValorTotal);  /* Imprime o valor certo*/
    printf("---------------------------------\n");
}

void TotalProdutos(){
    double TotalProduto[n_products];

    for (int i = 0; i < n_products; i++)
    {
        TotalProduto[i] = 0.0;
    }

    for (int i = 0; i < n_sales; i++)
    {
        int produtoIndex = sales[i].product - 1;

        if (produtoIndex >= 0 && produtoIndex < n_products)
        {
            double valorvendido = products[produtoIndex].price * sales[i].qtd;
            TotalProduto[produtoIndex] += valorvendido;
        }
    }
    printf("\n----------------------------------\n");
    printf("Valor Total de Vendas por Produto:\n");
    for (int i = 0; i < n_products; i++)
    {
        wprintf(L"Produto %-3dR$%.2lf\n", products[i].code, TotalProduto[i]);
    }
    printf("----------------------------------\n");
}

void TotalVendedor(){
    /*Inicialize um array para rastrear as vendas de cada vendedor*/
    double totalVendas[n_employees];

    /*Define o total de vendas como 0*/
    for (int i = 0; i < n_employees; i++) {
        totalVendas[i] = 0.0;
    }

    /*Calcula o valor e soma*/
    for (int i = 0; i < n_sales; i++) {
        double valorVendido = products[sales[i].product - 1].price * sales[i].qtd;
        int vendedor = sales[i].employee - 1;
        totalVendas[vendedor] += valorVendido;
    }

    printf("\n---------------------------------------\n");
    printf("Valor Total de Vendas de Cada Vendedor:\n");
    for (int i = 0; i < n_employees; i++)
    {
    while (iswspace(employees[i].name[0])) {
        wcscpy(employees[i].name, employees[i].name + 1);
    }
    int len = wcslen(employees[i].name);
    while (len > 0 && iswspace(employees[i].name[len - 1])) {
        employees[i].name[len - 1] = L'\0';
        len--;
    }

    wprintf(L"Vendedor %d (%ls): R$%.2lf\n", employees[i].id, employees[i].name, totalVendas[i]);
    }
    printf("---------------------------------------\n");
}

int main() {
    setlocale(LC_CTYPE, "Portuguese_Brazil");
    setlocale(LC_NUMERIC, "C");
    Obter();
    Imprimir();

    int opcao;
    while (1) {
        printf("\nEscolha o que deseja consultar:\n");
        printf("1. Produtos\n");
        printf("2. Vendedores\n");
        printf("3. Vendas\n");
        printf("4. Valor total das vendas\n");
        printf("5. Total de produtos vendidos\n");
        printf("6. Total vendido por vendedor\n");
        wprintf(L"Digite qualquer outro número para sair: ");
        
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                Produtos();
                break;
            case 2:
                Vendedores();
                break;
            case 3:
                Vendas();
                break;
            case 4:
                TotalVendas();
                break;
            case 5:
                TotalProdutos();
                break;
            case 6:
                TotalVendedor();
                break;
            default:
                printf("Saindo do programa.\n");
                return 0;
        }
    }
}