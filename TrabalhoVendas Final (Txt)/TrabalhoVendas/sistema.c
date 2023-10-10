#include <stdio.h>
#include <stdlib.h>

struct Product /* Definição dos produtos*/
{
    int code;
    double price;
    char description[50];
};

struct Employee /* Definição dos vendedores*/
{
    int id;
    char name[50];
};

struct Sale /* Definição das vendas*/
{
    int employee;
    int product;
    int qtd;
};

struct Product products[100];     /*Define o maximo que armazena de produtos*/
struct Employee employees[100];   /*Define o maximo que armazena de vendedores*/
struct Sale sales[100];           /*Define o maximo que armazena de vendas*/

int n_products;      /* Numero total de produtos*/
int n_employees;     /* Numero total de vendedores*/
int n_sales;         /* Numero total de vendas*/

double totalVendas[100];                 /* Armazena o valor total de vendas por vendedor*/
double ValorTotalPorProduto[100];        /* Armazena o valor total por produto*/

double ValorTotal = 0;     /* Valor total de vendas no periodo*/

void Obter(struct Product products[], struct Employee employees[], struct Sale sales[], int *n_products, int *n_employees, int *n_sales)
{
    FILE *file_products;
    file_products = fopen("produtos.txt", "r"); /* le o arquivo txt*/

    if (file_products == NULL)
    {
        printf("Erro ao abrir o arquivo\n"); /* Erro caso nao exista o arquivo*/
        exit(1);
    }

    fscanf(file_products, "%d\n", n_products); /* Obtem o tamanho do file */

    for (int i = 0; i < *n_products; i++) /* for do tamanho do file */
    {
        fscanf(file_products, "%d\n %lf\n %s\n", &products[i].code, &products[i].price, &products[i].description); /* armazena as informacoes */
    }

    fclose(file_products); /* Salva o arquivo */

    FILE *file_employees;
    file_employees = fopen("vendedores.txt", "r");

    if (file_employees == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    fscanf(file_employees, "%d\n", n_employees);

    for (int i = 0; i < *n_employees; i++)
    {
        fscanf(file_employees, "%d\n %s\n", &employees[i].id, &employees[i].name);
    }

    fclose(file_employees);

    FILE *file_sales;
    file_sales = fopen("vendas.txt", "r");

    if (file_sales == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    fscanf(file_sales, "%d\n", n_sales);

    for (int i = 0; i < *n_sales; i++)
    {
        fscanf(file_sales, "%d %d %d", &sales[i].employee, &sales[i].product, &sales[i].qtd);
    }

    fclose(file_sales);
}

int main()
{
    Obter(products, employees, sales, &n_products, &n_employees, &n_sales); /* Obtem os dados necessarios*/
    
    
    FILE *totais_File;
    totais_File = fopen("totais.txt", "a");         /* Abre o arquivo de totais e seleciona para alterar*/

    if (totais_File == NULL)
    {
        printf("Erro ao abrir o arquivo totais.txt\n");         /* Erro se nao conseguir criar arquivo*/
        exit(1);
    }

fprintf(totais_File, "\nLog de Vendas:\n");
    for (int i = 0; i < n_sales; i++)                    /* Imprime o Log de vendas no file*/
    {
        fprintf(totais_File, "[%d] %d %d %d\n", i, sales[i].employee, sales[i].product, sales[i].qtd);
    }

 fprintf(totais_File, "\nCatalogo de Produtos:\n");
    for (int i = 0; i < n_products; i++)                /* Imprime o catalogo de produtos no file*/
    {
        fprintf(totais_File, "[%d] %d %.2lf %s\n", i, products[i].code, products[i].price, products[i].description);
    }

fprintf(totais_File, "\nLista de Vendedores:\n");
    for (int i = 0; i < n_employees; i++)               /* Imprime a Lista de vendedores no file*/
    {
        fprintf(totais_File, "[%d] %d %s\n", i, employees[i].id, employees[i].name);
    }

for (int i = 0; i < n_sales; i++)
    {
        double ValorVendido = products[sales[i].product - 1].price * sales[i].qtd;    /* Calcula o valor vendido e soma a um total*/
        ValorTotal += ValorVendido; 
    }


    fprintf(totais_File, "\nTotal geral vendido: %.2lf\n", ValorTotal);        /* Imprime o total vendido*/

for (int i = 0; i < n_products; i++) {

    ValorTotalPorProduto[i] = 0.0;              /* Define o total vendido de cada produto como 0*/

}

for (int i = 0; i < n_sales; i++)
    {
        double ValorVendidoProduto = products[sales[i].product - 1].price * sales[i].qtd;       /* Calcula o valor vendido do produto em uma venda*/
        ValorTotalPorProduto[sales[i].product - 1] += ValorVendidoProduto;                      /* Soma esse valor ao valor total vendido do produto*/
    }

    fprintf(totais_File, "\nTotal de vendas de cada produto:\n");

    for (int i = 0; i < n_products; i++)                           /* Imprime o total vendido por produto*/
    {
        fprintf(totais_File, "Produto %d (%s): %.2lf\n", products[i].code, products[i].description, ValorTotalPorProduto[i]);
    }

for (int i = 0; i < n_employees; i++) {
        totalVendas[i] = 0.0;                                     /* Define o total vendido po vendedor como 0*/
    }

    for (int i = 0; i < n_sales; i++) {
        double valorVendido = products[sales[i].product - 1].price * sales[i].qtd;             /* Calcula o valor vendido por um vendedor em uma venda*/      
        totalVendas[sales[i].employee - 1] += valorVendido;                                    /* Soma esse valor ao valor total vendido pelo vendedor*/
    }

    fprintf(totais_File, "\nTotal de vendas de cada vendedor:\n");

    for (int i = 0; i < n_employees; i++) {                      /* Imprime o total vendido por vendedor*/
        fprintf(totais_File, "Vendedor %d (%s) : %.2lf\n", employees[i].id, employees[i].name, totalVendas[i]);
    }

    fclose(totais_File);

    return 0;
}