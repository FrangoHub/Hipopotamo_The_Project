#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct funcionario {
    char nome[50];
    char aprovacao_string[10];
    float salario;
    float valor_pago_hora_extra;
    float hora_extra;
    float hora_semanal;
    float hora_mensal;
    float valor_hora_extra;
    int dia_semana;
    int feriado;
    int aprovacao;
};

int main() {
    FILE *arquivo;
    int n, opcao;

    time_t tempo = time(NULL);
    struct tm *hora_local = localtime(&tempo);
    int agora = hora_local->tm_hour * 60 + hora_local->tm_min;
    int limite = 13 * 60 + 0;
    
    printf("Data e hora local: %02d/%02d/%d %02d:%02d:%02d\n",
           hora_local->tm_mday,
           hora_local->tm_mon + 1,
           hora_local->tm_year + 1900,
           hora_local->tm_hour,
           hora_local->tm_min,
           hora_local->tm_sec);
    printf("O que você deseja fazer?\n1 - Registrar\n2 - Listar\n3 - Fechar\n");
    scanf("%d", &opcao);

    switch (opcao) {

    // ===================================================
    //                 CASE 1 - REGISTRAR
    // ===================================================
    case 1:
        if(agora <= limite){
        printf("Você não pode registrar funcionários nesse horário tente amanhã antes das 13:00");
    }
        else{
        arquivo = fopen("funcionarios.txt", "a");
        if (!arquivo) {
            printf("Erro ao abrir o arquivo!\n");
            return 1;
        }

        while (1) {
            printf("Digite quantos funcionários deseja registrar: ");
            if (scanf("%d", &n) == 1 && n > 0) break;

            printf("Entrada inválida! Digite um inteiro > 0.\n");
            while (getchar() != '\n');
        }

        struct funcionario *f = malloc(n * sizeof(struct funcionario));
        if (!f) {
            printf("Erro ao alocar memória.\n");
            fclose(arquivo);
            return 1;
        }

        for (int i = 0; i < n; i++) {

            printf("\nDigite o nome do funcionário %d: ", i + 1);
            scanf(" %[^\n]", f[i].nome);

            while (1) {
                printf("Digite o salário mensal: ");
                if (scanf("%f", &f[i].salario) == 1 && f[i].salario > 0) break;
                printf("Entrada inválida! Digite um número inteiro positivo.\n");
                while (getchar() != '\n');
            }

            while (1) {
                printf("Digite as horas extras desejadas: ");
                if (scanf("%f", &f[i].hora_extra) == 1 && f[i].hora_extra >= 0) break;
                printf("Entrada inválida! Digite um valor numérico inteiro positivo.\n");
                while (getchar() != '\n');
            }

            while (1) {
                printf("Horas semanais trabalhadas: ");
                if (scanf("%f", &f[i].hora_semanal) == 1 && f[i].hora_semanal >= 0) break;
                printf("Entrada inválida! Digite um valor numérico inteiro positivo.\n");
                while (getchar() != '\n');
            }

            while (1) {
                printf("Dia da semana (1=Dom ... 7=Sábado): ");
                if (scanf("%d", &f[i].dia_semana) == 1 &&
                    f[i].dia_semana >= 1 && f[i].dia_semana <= 7)
                    break;

                printf("Entrada inválida! Digite um número entre 1 e 7.\n");
                while (getchar() != '\n');
            }

            while (1) {
                printf("Esse dia é feriado? (sim=1, não=0): ");
                if (scanf("%d", &f[i].feriado) == 1 &&
                    (f[i].feriado == 0 || f[i].feriado == 1))
                    break;

                printf("Entrada inválida! Digite 0 ou 1.\n");
                while (getchar() != '\n');
            }

            // ============================
            //       CÁLCULOS
            // ============================

            // Aqui o correto é dividir por 4 semanas
            f[i].hora_mensal = f[i].hora_semanal * 4;

            float valor_hora = f[i].salario / f[i].hora_mensal;

            if (f[i].dia_semana == 7 || f[i].feriado == 1)
                f[i].valor_hora_extra = valor_hora * 2.0;
            else
                f[i].valor_hora_extra = valor_hora * 1.5;

            f[i].valor_pago_hora_extra = f[i].valor_hora_extra * f[i].hora_extra;

            printf("\nValor total a ser pago pelas horas extras de %s é R$ %.2f\n",
                   f[i].nome, f[i].valor_pago_hora_extra);

            while (1) {
                printf("Você deseja aprovar (sim=1, não=0): ");
                if (scanf("%d", &f[i].aprovacao) == 1 &&
                    (f[i].aprovacao == 0 || f[i].aprovacao == 1))
                    break;

                printf("Entrada inválida! Digite 0 ou 1.\n");
                while (getchar() != '\n');
            }

            strcpy(f[i].aprovacao_string,
                   f[i].aprovacao ? "Aprovado" : "Negado");

            // Salvando no arquivo
            fprintf(arquivo,
                    "%s|%.2f|%.2f|%.2f|%s\n",
                    f[i].nome,
                    f[i].salario,
                    f[i].hora_extra,
                    f[i].valor_pago_hora_extra,
                    f[i].aprovacao_string);
        }

        free(f);
        fclose(arquivo);
        printf("\nRegistros salvos com sucesso!\n");
        break;
}
    // ===================================================
    //                 CASE 2 - LISTAR
    // ===================================================
    case 2:
        arquivo = fopen("funcionarios.txt", "r");
        if (!arquivo) {
            printf("Nenhum funcionário cadastrado.\n");
            break;
        }

        printf("\n--- LISTA DE FUNCIONÁRIOS ---\n");

        struct funcionario x;

        while (fscanf(arquivo,
                      "%49[^|]|%f|%f|%f|%9s\n",
                      x.nome,
                      &x.salario,
                      &x.hora_extra,
                      &x.valor_pago_hora_extra,
                      x.aprovacao_string) == 5)
        {
            printf("Nome: %s\nSalário: %.2f\nHoras extras: %.2f\nValor pago: %.2f\nStatus: %s\n\n",
                   x.nome, x.salario, x.hora_extra, x.valor_pago_hora_extra, x.aprovacao_string);
        }

        fclose(arquivo);
        break;

    case 3:
        printf("Programa encerrado.\n");
        break;

    default:
        printf("Opção inválida.\n");
    }

    return 0;
}
