/* =============================================================
   mary_jackson_quiz.c  (versao 2 — completa)
   Programa em C — RODA NO TERMINAL, nao no navegador.

   Recursos desta versao:
     - Menu principal com 4 opcoes
     - Biografia resumida
     - Linha do tempo formatada
     - Quiz com 10 perguntas, em ORDEM ALEATORIA a cada partida
     - Validacao de entrada (nao trava se digitar letra/lixo)
     - Explicacao da resposta certa apos cada pergunta
     - Pontuacao final com classificacao
     - Opcao de jogar novamente sem fechar o programa

   Como compilar e rodar (Windows / MSYS2 ou MinGW):
     gcc mary_jackson_quiz.c -o quiz -Wall -Wextra
     .\quiz

   Linux / Mac:
     gcc mary_jackson_quiz.c -o quiz -Wall -Wextra
     ./quiz
   ============================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_PERGUNTAS   10
#define NUM_EVENTOS      9
#define NUM_FATOS         6
#define TAM_ENTRADA      16

/* ---------- estruturas de dados ---------- */

typedef struct {
    char pergunta[220];
    char opcoes[4][100];
    int  resposta_certa;   /* indice de 0 a 3 */
    char explicacao[220];
} Pergunta;

typedef struct {
    char ano[12];
    char evento[160];
} EventoLinha;

/* ---------- protótipos ---------- */

void exibir_cabecalho(void);
void exibir_menu(void);
void exibir_biografia(void);
void exibir_linha_do_tempo(void);
void iniciar_quiz(void);
void embaralhar_perguntas(Pergunta lista[], int tamanho);
int  ler_opcao_menu(int minimo, int maximo);
int  ler_resposta_quiz(void);
void pausar(void);
void limpar_tela_logica(void);

/* ---------- dados fixos ---------- */

static const char *fatos[NUM_FATOS] = {
    "Nasceu em 9 de abril de 1921, em Hampton, Virginia.",
    "Formou-se com honras maximas em Matematica e Ciencias Fisicas pelo Hampton Institute, em 1942.",
    "Comecou na NASA em 1951 como 'computadora humana' na unidade segregada West Area Computing.",
    "Em 1958, tornou-se a primeira engenheira negra da historia da NASA.",
    "Em 1979, aceitou reduzir de cargo para se tornar gerente do Programa Federal da Mulher em Langley.",
    "Faleceu em 11 de fevereiro de 2005, em Hampton, Virginia, aos 83 anos."
};

static const EventoLinha linha_do_tempo[NUM_EVENTOS] = {
    {"1921",     "Nascimento em Hampton, Virginia."},
    {"1942",     "Formatura no Hampton Institute (Matematica e Ciencias Fisicas)."},
    {"1951",     "Entra para o NACA (futura NASA), na West Area Computing."},
    {"1953-57",  "Passa a trabalhar no tunel de vento supersonico com Kazimierz Czarnecki."},
    {"1958",     "Torna-se a primeira engenheira negra da NASA."},
    {"1958-79",  "Vinte anos analisando dados de voo e tunel de vento."},
    {"1979",     "Vira gerente do Programa Federal da Mulher em Langley."},
    {"1985",     "Aposenta-se apos 34 anos de carreira na NASA."},
    {"2005",     "Falece em Hampton, Virginia, aos 83 anos."}
};

/* pool de 10 perguntas do quiz */
static Pergunta banco_perguntas[NUM_PERGUNTAS] = {
    {
        "Em que ano Mary Jackson nasceu?",
        {"1911", "1921", "1931", "1941"},
        1,
        "Ela nasceu em 9 de abril de 1921, em Hampton, Virginia."
    },
    {
        "Em que instituicao ela se formou em 1942?",
        {"Hampton Institute", "MIT", "Universidade de Virginia", "Howard University"},
        0,
        "Formou-se com honras maximas no Hampton Institute, em Matematica e Ciencias Fisicas."
    },
    {
        "Em que ano ela entrou para o NACA (futura NASA)?",
        {"1945", "1951", "1958", "1962"},
        1,
        "Ela comecou em 1951, na unidade segregada West Area Computing."
    },
    {
        "Qual era o nome da unidade onde ela comecou a trabalhar?",
        {"Mission Control", "West Area Computing", "Flight Dynamics Lab", "Propulsion Division"},
        1,
        "West Area Computing era o setor reservado a matematicas negras, chefiado por Dorothy Vaughan."
    },
    {
        "Quem foi o engenheiro que a convidou para trabalhar no tunel de vento?",
        {"Werner von Braun", "Kazimierz Czarnecki", "Christopher Kraft", "John Glenn"},
        1,
        "Kazimierz Czarnecki sugeriu que ela se tornasse engenheira."
    },
    {
        "O que ela precisou fazer para poder cursar as disciplinas exigidas?",
        {"Mudar de cidade", "Pedir autorizacao especial a prefeitura de Hampton", "Pagar uma multa", "Prestar um novo vestibular"},
        1,
        "Ela teve que pedir permissao para assistir aulas numa escola ainda segregada."
    },
    {
        "Em que ano ela se tornou a primeira engenheira negra da NASA?",
        {"1948", "1958", "1968", "1978"},
        1,
        "A promocao aconteceu em 1958."
    },
    {
        "Em 1979, que cargo ela aceitou (com reducao de nivel) para ajudar outras pessoas?",
        {"Diretora de voo", "Gerente do Programa Federal da Mulher", "Piloto de testes", "Chefe de imprensa"},
        1,
        "Ela passou a recrutar e promover mulheres e minorias dentro da NASA."
    },
    {
        "Depois de quantos anos de carreira ela se aposentou, em 1985?",
        {"20 anos", "27 anos", "34 anos", "40 anos"},
        2,
        "Foram 34 anos de carreira dedicados a NASA."
    },
    {
        "Em que ano a sede da NASA em Washington foi renomeada em homenagem a ela?",
        {"2005", "2016", "2021", "2024"},
        2,
        "Em fevereiro de 2021, o predio passou a se chamar Mary W. Jackson NASA Headquarters."
    }
};

/* ---------- main ---------- */

int main(void) {
    int opcao;
    int continuar = 1;

    srand((unsigned int) time(NULL));

    while (continuar) {
        exibir_cabecalho();
        exibir_menu();
        opcao = ler_opcao_menu(1, 4);

        switch (opcao) {
            case 1:
                exibir_biografia();
                pausar();
                break;
            case 2:
                exibir_linha_do_tempo();
                pausar();
                break;
            case 3:
                iniciar_quiz();
                pausar();
                break;
            case 4:
                printf("\nObrigado por conhecer a historia de Mary W. Jackson. Ate a proxima!\n\n");
                continuar = 0;
                break;
            default:
                break;
        }
    }

    return 0;
}

/* ---------- implementacao ---------- */

void exibir_cabecalho(void) {
    printf("\n==================================================\n");
    printf("   MARY WINSTON JACKSON — DOSSIE INTERATIVO (NASA)\n");
    printf("==================================================\n");
}

void exibir_menu(void) {
    printf("\n1 - Ver biografia resumida\n");
    printf("2 - Ver linha do tempo\n");
    printf("3 - Iniciar quiz (10 perguntas)\n");
    printf("4 - Sair\n\n");
}

void exibir_biografia(void) {
    int i;
    printf("\n--- BIOGRAFIA RESUMIDA ---\n\n");
    for (i = 0; i < NUM_FATOS; i++) {
        printf(" %d. %s\n", i + 1, fatos[i]);
    }
    printf("\nEm 1958, Mary Jackson tornou-se a primeira engenheira negra\n");
    printf("da historia da NASA, apos anos de estudo noturno e a luta para\n");
    printf("frequentar aulas em uma escola ainda segregada.\n");
}

void exibir_linha_do_tempo(void) {
    int i;
    printf("\n--- LINHA DO TEMPO ---\n\n");
    for (i = 0; i < NUM_EVENTOS; i++) {
        printf(" [%-8s] %s\n", linha_do_tempo[i].ano, linha_do_tempo[i].evento);
    }
}

/* embaralha o array de perguntas com o algoritmo de Fisher-Yates */
void embaralhar_perguntas(Pergunta lista[], int tamanho) {
    int i, j;
    Pergunta temp;

    for (i = tamanho - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = lista[i];
        lista[i] = lista[j];
        lista[j] = temp;
    }
}

void iniciar_quiz(void) {
    Pergunta perguntas_da_rodada[NUM_PERGUNTAS];
    int acertos = 0;
    int i, resposta;
    double percentual;

    memcpy(perguntas_da_rodada, banco_perguntas, sizeof(banco_perguntas));
    embaralhar_perguntas(perguntas_da_rodada, NUM_PERGUNTAS);

    printf("\n--- QUIZ: MARY WINSTON JACKSON ---\n");
    printf("Responda digitando um numero de 1 a 4.\n");

    for (i = 0; i < NUM_PERGUNTAS; i++) {
        printf("\nPergunta %d de %d\n", i + 1, NUM_PERGUNTAS);
        printf("%s\n", perguntas_da_rodada[i].pergunta);
        printf(" 1 - %s\n", perguntas_da_rodada[i].opcoes[0]);
        printf(" 2 - %s\n", perguntas_da_rodada[i].opcoes[1]);
        printf(" 3 - %s\n", perguntas_da_rodada[i].opcoes[2]);
        printf(" 4 - %s\n", perguntas_da_rodada[i].opcoes[3]);

        resposta = ler_resposta_quiz();

        if (resposta - 1 == perguntas_da_rodada[i].resposta_certa) {
            printf(">> Correto!\n");
            acertos++;
        } else {
            printf(">> Errado. A resposta certa era: %s\n",
                   perguntas_da_rodada[i].opcoes[perguntas_da_rodada[i].resposta_certa]);
        }
        printf("   %s\n", perguntas_da_rodada[i].explicacao);
    }

    percentual = (acertos * 100.0) / NUM_PERGUNTAS;

    printf("\n==================================================\n");
    printf(" RESULTADO: %d de %d perguntas corretas (%.0f%%)\n", acertos, NUM_PERGUNTAS, percentual);

    if (percentual == 100.0) {
        printf(" Perfeito! Voce conhece muito bem a historia de Mary Jackson.\n");
    } else if (percentual >= 70.0) {
        printf(" Muito bom! Voce conhece bem a trajetoria dela.\n");
    } else if (percentual >= 40.0) {
        printf(" Legal! Vale revisar a linha do tempo para fixar mais detalhes.\n");
    } else {
        printf(" Vale a pena revisitar a biografia e a linha do tempo (opcoes 1 e 2).\n");
    }
    printf("==================================================\n");
}

/* le uma opcao de menu valida entre 'minimo' e 'maximo', sem travar com entrada invalida */
int ler_opcao_menu(int minimo, int maximo) {
    char buffer[TAM_ENTRADA];
    int valor;
    int valido;

    do {
        printf("Escolha uma opcao: ");
        valido = 0;

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            /* entrada padrao terminou (EOF) — encerra o programa em vez de
               ficar preso em loop infinito pedindo uma opcao para sempre */
            printf("\nEntrada encerrada. Fechando o programa.\n");
            exit(0);
        }

        if (sscanf(buffer, "%d", &valor) == 1) {
            if (valor >= minimo && valor <= maximo) {
                valido = 1;
            }
        }

        if (!valido) {
            printf("Opcao invalida. Digite um numero entre %d e %d.\n", minimo, maximo);
        }
    } while (!valido);

    return valor;
}

/* le uma resposta do quiz (1 a 4), sem travar com entrada invalida */
int ler_resposta_quiz(void) {
    return ler_opcao_menu(1, 4);
}

void pausar(void) {
    char buffer[TAM_ENTRADA];
    printf("\nPressione ENTER para voltar ao menu...");
    fgets(buffer, sizeof(buffer), stdin);
}