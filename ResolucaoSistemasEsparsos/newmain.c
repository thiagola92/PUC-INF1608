#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

typedef struct coluna
{

	int numero_da_coluna;		// Diz qual é essa coluna (se você sabe em que linha esta, então isso vai te dizer a posicao exata da matriz. ex: M[i][numero_da_coluna])
	double valor;				// O valor que tem nesse local, ex: M[i][j] >> 2.543

} Coluna;

typedef struct linha
{

	int numero_de_colunas;		// Diz quantas colunas tem nessa linha SEM O VALOR 0
	Coluna* j;					// Vai em um vetor que representa uma coluna, depois você tem que ver que coluna é essa j[10].numero_da_coluna >> pode ser a coluna 100 e terem 91 colunas vazias antes dessa

} Linha;

typedef struct matriz
{

	int numero_de_linhas;		// Diz quantas linhas tem a matriz
	Linha* i;					// Para ir para uma certa linha basta falar i[numero da linha]

} Matriz;

void cria_matriz(int n, Matriz* m) {
	int contador = 0;

	m->i = (Linha*)malloc(sizeof(Linha)*n);

	for (contador = 0; contador < n; contador++)
		m->i[contador].numero_de_colunas = 0;

	m->numero_de_linhas = n;
}

int busca_binaria(int linha, int coluna, Matriz* m) {
	int posicao_inicial = 0;
	int posicao_final = 0;

	int meio;

	if (linha >= m->numero_de_linhas || linha < 0 || coluna >= m->numero_de_linhas || coluna < 0 || m->i[linha].numero_de_colunas <= 0)
		return -1;

	posicao_inicial = 0;
	posicao_final = m->i[linha].numero_de_colunas - 1;

	while (posicao_inicial < posicao_final) {

		meio = (posicao_final - posicao_inicial + 1) / 2;
		meio = posicao_inicial + meio;

		if (m->i[linha].j[meio].numero_da_coluna == coluna)
			return meio;
		if (m->i[linha].j[meio].numero_da_coluna > coluna)
			posicao_final = meio - 1;
		if (m->i[linha].j[meio].numero_da_coluna < coluna)
			posicao_inicial = meio + 1;
	}

	if (m->i[linha].j[posicao_inicial].numero_da_coluna == coluna)
		return posicao_inicial;

	return -1;
}

void add_valor(int linha, int coluna, double valor, Matriz* m) {
	int resposta_da_busca_binaria = -1;
	int contador = 0;

	if (linha >= m->numero_de_linhas || coluna >= m->numero_de_linhas)
		return;

	resposta_da_busca_binaria = busca_binaria(linha, coluna, m);

	if (resposta_da_busca_binaria != -1) {

		if (valor == 0) {

			for (; resposta_da_busca_binaria + 1 < m->i[linha].numero_de_colunas; resposta_da_busca_binaria++) {
				m->i[linha].j[resposta_da_busca_binaria].numero_da_coluna = m->i[linha].j[resposta_da_busca_binaria + 1].numero_da_coluna;
				m->i[linha].j[resposta_da_busca_binaria].valor = m->i[linha].j[resposta_da_busca_binaria + 1].valor;
			}

			m->i[linha].numero_de_colunas--;
			m->i[linha].j = (Coluna*)realloc(m->i[linha].j, sizeof(Coluna) * m->i[linha].numero_de_colunas);

			return;
		}

		m->i[linha].j[resposta_da_busca_binaria].valor = valor;

		return;

	}

	if (valor == 0)
		return;

	if (m->i[linha].numero_de_colunas == 0) {
		m->i[linha].numero_de_colunas = 1;
		m->i[linha].j = (Coluna*)malloc(sizeof(Coluna));
		m->i[linha].j->numero_da_coluna = coluna;
		m->i[linha].j->valor = valor;
	}
	else {
		m->i[linha].numero_de_colunas++;
		m->i[linha].j = (Coluna*)realloc(m->i[linha].j, sizeof(Coluna) * m->i[linha].numero_de_colunas);

		contador = m->i[linha].numero_de_colunas;

		do {
			if ((contador - 2) < 0) {
				m->i[linha].j[contador - 1].numero_da_coluna = coluna;
				m->i[linha].j[contador - 1].valor = valor;

				break;

				//
			}

			if (m->i[linha].j[contador - 2].numero_da_coluna < coluna) {
				m->i[linha].j[contador - 1].numero_da_coluna = coluna;
				m->i[linha].j[contador - 1].valor = valor;

				break;
			}

			m->i[linha].j[contador - 1].numero_da_coluna = m->i[linha].j[contador - 2].numero_da_coluna;
			m->i[linha].j[contador - 1].valor = m->i[linha].j[contador - 2].valor;

			contador--;
		} while (1);
	}
}

void preecnher_matriz(Matriz* m) {
	int contador = 0;

	for (contador = 0; contador < m->numero_de_linhas; contador++) {
		add_valor(contador, contador, contador, m);
		add_valor(contador, contador + 1, 0.5, m);
		add_valor(contador, contador + 2, 0.5, m);
		add_valor(contador + 1, contador, 0.5, m);
		add_valor(contador + 2, contador, 0.5, m);
	}
}

void copia(Matriz* m, Matriz* m2) {
	int contador = 0;
	int contador2 = 0;

	cria_matriz(m->numero_de_linhas, m2);

	for (contador = 0; contador < m->numero_de_linhas; contador++)
		for (contador2 = 0; contador2 < m->i[contador].numero_de_colunas; contador2++)
			add_valor(contador, m->i[contador].j[contador].numero_da_coluna, m->i[contador].j[contador].valor, m2);

}

void print(Matriz* x) {
	int i, j;

	for (i = 0; i < x->numero_de_linhas; i++) {
		if (x->i[i].numero_de_colunas != 0) {
			printf("[ ");
			for (j = 0; j < x->numero_de_linhas; j++) {
				if (busca_binaria(i, j, x) == -1)
					printf("%f ", (double)0);
				else
					printf("%f ", x->i[i].j[busca_binaria(i, j, x)].valor);

			}
			printf("] \n");
		}
	}
}

void adicao_vetores(Matriz* v1, Matriz* v2, Matriz* v3) {
	
	int contador = 0;
	int posicao = -1;

	copia(v1, v3);

	for (contador = 0; contador < v2->i[0].numero_de_colunas; contador++) {
		posicao = busca_binaria(0, v2->i[0].j[contador].numero_da_coluna, v3);
		if (posicao == -1)
			add_valor(0, v2->i[0].j[contador].numero_da_coluna, v2->i[0].j[contador].valor, v3);
		if (contador != -1)
			add_valor(0, v2->i[0].j[contador].numero_da_coluna, v1->i[0].j[contador].valor + v2->i[0].j[contador].valor, v3);
	}
}

void multiplicacao_matriz_vetor(Matriz* m, Matriz* v1, Matriz* v2) {
	int contadorLinha = 0;
	int contadorColuna = 0;
	int contador;

	double soma;

	int pos_da_busca_1;
	int pos_da_busca_2;

	cria_matriz(m->numero_de_linhas, v2);

	for (contadorLinha = 0; contadorLinha < v1->numero_de_linhas; contadorLinha++) {
		for (contadorColuna = 0; contadorColuna < v1->i[contadorLinha].numero_de_colunas; contadorColuna)
			add_valor(v1->i[contadorLinha].j[contadorColuna].numero_da_coluna, contadorLinha, v1->i[contadorLinha].j[contadorColuna].valor, v2);
	}

	for (contadorLinha = 0; contadorLinha < m->numero_de_linhas; contadorLinha++) {
		
		for (contadorColuna = 0; contadorColuna < m->numero_de_linhas; contadorColuna++) {
			soma = 0;

			for (contador = 0; contador < m->numero_de_linhas; contador++) {
				pos_da_busca_1 = busca_binaria(contadorLinha, contador, m);

				if (pos_da_busca_1 == -1) {
					add_valor(contadorLinha, contadorColuna, soma, v2);
					continue;
				}

				pos_da_busca_2 = busca_binaria(contador, contadorColuna, v1);

				if (pos_da_busca_2 == -1) {
					add_valor(contadorLinha, contadorColuna, soma, v2);
					continue;
				}

				soma = soma + (m->i[contadorLinha].j[pos_da_busca_1].valor * v1->i[contador].j[pos_da_busca_2].valor);
				add_valor(contadorLinha, contadorColuna, soma, v2);
			}
		}
	}
}

void multiplicacao_escalar_vetor(double x, Matriz* v1, Matriz* v2) {
	int contadorLinha = 0;
	int contadorColuna = 0;

	int resposta_busca = -1;

	cria_matriz(v1->numero_de_linhas, v2);

	for (contadorLinha = 0; contadorLinha < v1->numero_de_linhas; contadorLinha++) {
		for (contadorColuna = 0; contadorColuna < v1->numero_de_linhas; contadorColuna++) {
			resposta_busca = busca_binaria(contadorLinha, contadorColuna, v1);

			if (resposta_busca != -1)
				add_valor(contadorLinha, contadorColuna, v1->i[contadorLinha].j[resposta_busca].valor, v2);
		}
	}
}

double multiplicacao_vetor_vetor(Matriz* v1, Matriz* v2, Matriz* v3) {

	multiplicacao_matriz_vetor(v1, v2, v3);

	if (v3->i[0].numero_de_colunas != 0)
		return v3->i[0].j[0].valor;
	return 0;
}

int main(void) {
	Matriz m1;
	Matriz v1;
	Matriz v2;

	cria_matriz(3, &m1);
	cria_matriz(3, &v1);

	add_valor(0, 0, 1, &m1);
	add_valor(0, 1, 2, &m1);
	add_valor(0, 2, 3, &m1);
	add_valor(1, 0, 1, &m1);
	add_valor(1, 1, 2, &m1);
	add_valor(1, 2, 3, &m1);
	add_valor(2, 0, 1, &m1);
	add_valor(2, 1, 2, &m1);
	add_valor(2, 2, 3, &m1);

	print(&m1);
	printf("-\n");

	add_valor(0, 0, 3, &v1);
	add_valor(0, 1, 2, &v1);
	add_valor(0, 2, 1, &v1);

	print(&v1);
	printf("-\n");

	adicao_vetores(&m1, &v1, &v2);

	print(&v2);
	printf("-\n");


	return;
}

