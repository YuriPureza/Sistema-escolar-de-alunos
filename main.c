#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char nome[100];
  float notas[3];
} t_aluno;

typedef struct {
  t_aluno *alunos;
  int quantidade;
  int capacidade;
} t_lista;

typedef struct {
  char nome[100];
  char genero;
  unsigned int idade;
} Pessoa;

t_lista *inicializa_lista();
void criar_aluno(t_aluno *aluno);
void exibir_aluno(t_aluno *aluno);
void inserir_aluno(t_lista *lista);
void listar_alunos(t_lista *lista);
void salvar_alunos_arquivo(char *caminho_arquivo, t_lista *lista);
void salvar_bin(char *caminho, int qtd, Pessoa pessoas[qtd]);
void ler_bin(char *caminho);
void salvar_txt(char *caminho);
void ler_txt(char *caminho);

int main(void) {
  t_lista *lista = inicializa_lista();
  inserir_aluno(lista);
  inserir_aluno(lista);
  inserir_aluno(lista);
  listar_alunos(lista);
  salvar_alunos_arquivo("./alunos.txt", lista);

  Pessoa pessoas[3] = {{.nome = "Marcus", .genero = 'M', .idade = 24},
                       {.nome = "Beatriz", .genero = 'F', .idade = 18},
                       {"Yuri", 'M', 22}};

  salvar_bin("./pessoas.bin", 3, pessoas);
  salvar_txt("./alunos.txt");
  ler_txt("./alunos.txt");
  ler_bin("./pessoas.bin");

  free(lista->alunos);
  free(lista);

  return 0;
}

t_lista *inicializa_lista() {
  t_lista *lista = malloc(sizeof(t_lista));
  if (lista != NULL) {
    lista->capacidade = 3;
    lista->alunos = malloc(sizeof(t_aluno) * lista->capacidade);
    lista->quantidade = 0;
  }
  return lista;
}

void criar_aluno(t_aluno *aluno) {
  printf("Introduza o nome do aluno: ");
  scanf("%[^\n]%*c", aluno->nome);

  printf("Introduza as notas do aluno (separadas por espaço): ");
  scanf("%f %f %f", &aluno->notas[0], &aluno->notas[1], &aluno->notas[2]);
  getchar();
}

void exibir_aluno(t_aluno *aluno) {
  printf("(nome: %s, notas: (%.2f, %.2f, %.2f))\n", aluno->nome,
         aluno->notas[0], aluno->notas[1], aluno->notas[2]);
}

void inserir_aluno(t_lista *lista) {
  if (lista == NULL) {
    printf("Erro ao inserir! Lista não inicializada.\n");
    return;
  }

  if (lista->quantidade == lista->capacidade) {
    lista->capacidade += 10;
    t_aluno *aux = realloc(lista->alunos, sizeof(t_aluno) * lista->capacidade);
    if (aux == NULL) {
      printf("Erro ao inserir! Não foi possível realocar!\n");
      return;
    }
    lista->alunos = aux;
  }

  criar_aluno(&lista->alunos[lista->quantidade]);
  lista->quantidade++;
}

void listar_alunos(t_lista *lista) {
  if (lista->quantidade == 0) {
    printf("Lista vazia!\n");
    return;
  }

  for (int i = 0; i < lista->quantidade; i++) {
    exibir_aluno(&lista->alunos[i]);
  }
}

void salvar_alunos_arquivo(char *caminho_arquivo, t_lista *lista) {
  FILE *saida = fopen(caminho_arquivo, "w");
  if (saida == NULL || lista == NULL) {
    printf("Erro ao salvar alunos no arquivo!\n");
    return;
  }

  for (int i = 0; i < lista->quantidade; i++) {
    fprintf(saida, "%s\t(%.2f\t%.2f\t%.2f)\n", lista->alunos[i].nome,
            lista->alunos[i].notas[0], lista->alunos[i].notas[1],
            lista->alunos[i].notas[2]);
  }

  fclose(saida);
}

void salvar_bin(char *caminho, int qtd, Pessoa pessoas[]) {
  FILE *output = fopen(caminho, "wb");
  if (!output) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  fwrite(pessoas, sizeof(Pessoa), qtd, output);
  fclose(output);
}

void ler_bin(char *caminho) {
  FILE *input = fopen(caminho, "rb");
  if (!input) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  Pessoa pessoa;
  while (!feof(input)) {
    fread(&pessoa, sizeof(Pessoa), 1, input);
    printf("(nome: %s, genero: %c, idade: %d)\n", pessoa.nome, pessoa.genero,
           pessoa.idade);
  }
  fclose(input);
}

void salvar_txt(char *caminho) {
  FILE *output = fopen(caminho, "w");
  if (!output) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  fprintf(output, "Marcus\t8\t9\t10\n");
  fprintf(output, "Beatriz\t7\t8\t9\n");
  fprintf(output, "Yuri\t3\t8\t9\n");
  fclose(output);
}

void ler_txt(char *caminho) {
  FILE *input = fopen(caminho, "r");
  if (!input) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  char nome[100];
  float notas[3];
  while (!feof(input)) {
    fscanf(input, "%[^\t]\t%f\t%f\t%f%*c", nome, &notas[0], &notas[1],
           &notas[2]);

    printf("nome: %s, notas: %.2f, %.2f, %.2f\n", nome, notas[0], notas[1],
           notas[2]);
  }
  fclose(input);
}
