#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

int tamanho = 100;

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

Linha* cria_matriz() {
	int contador = 0;
	Linha* m;

	m = (Linha*)malloc(sizeof(Linha)*tamanho);

	for (contador = 0; contador < tamanho; contador++)
		m[contador].numero_de_colunas = 0;

	return m;
}

int busca_binaria(int linha, int coluna, Linha* m) {
	int posicao_inicial = 0;
	int posicao_final = 0;

	int meio;

	if (linha >= tamanho || linha < 0 || coluna >= tamanho || coluna < 0 || m[linha].numero_de_colunas <= 0)
		return -1;

	posicao_inicial = 0;
	posicao_final = m[linha].numero_de_colunas - 1;

	while (posicao_inicial < posicao_final) {

		meio = (posicao_final - posicao_inicial + 1) / 2;
		meio = posicao_inicial + meio;

		if (m[linha].j[meio].numero_da_coluna == coluna)
			return meio;
		if (m[linha].j[meio].numero_da_coluna > coluna)
			posicao_final = meio - 1;
		if (m[linha].j[meio].numero_da_coluna < coluna)
			posicao_inicial = meio + 1;
	}

	if (m[linha].j[posicao_inicial].numero_da_coluna == coluna)
		return posicao_inicial;

	return -1;
}

void add_valor(int linha, int coluna, double valor, Linha* m) {
	int resposta_da_busca_binaria = -1;
	int contador = 0;

	if (linha >= tamanho || coluna >= tamanho)
		return;

	resposta_da_busca_binaria = busca_binaria(linha, coluna, m);

	if (resposta_da_busca_binaria != -1) {

		if (valor == 0) {

			for (; resposta_da_busca_binaria + 1 < m[linha].numero_de_colunas; resposta_da_busca_binaria++) {
				m[linha].j[resposta_da_busca_binaria].numero_da_coluna = m[linha].j[resposta_da_busca_binaria + 1].numero_da_coluna;
				m[linha].j[resposta_da_busca_binaria].valor = m[linha].j[resposta_da_busca_binaria + 1].valor;
			}

			m[linha].numero_de_colunas--;
			m[linha].j = (Coluna*)realloc(m[linha].j, sizeof(Coluna) * m[linha].numero_de_colunas);

			return;
		}

		m[linha].j[resposta_da_busca_binaria].valor = valor;

		return;

	}

	if (valor == 0)
		return;

	if (m[linha].numero_de_colunas == 0) {
		m[linha].numero_de_colunas = 1;
		m[linha].j = (Coluna*)malloc(sizeof(Coluna));
		m[linha].j->numero_da_coluna = coluna;
		m[linha].j->valor = valor;
	}
	else {
		m[linha].numero_de_colunas++;
		m[linha].j = (Coluna*)realloc(m[linha].j, sizeof(Coluna) * m[linha].numero_de_colunas);

		contador = m[linha].numero_de_colunas;

		do {
			if ((contador - 2) < 0) {
				m[linha].j[contador - 1].numero_da_coluna = coluna;
				m[linha].j[contador - 1].valor = valor;

				break;

				//
			}

			if (m[linha].j[contador - 2].numero_da_coluna < coluna) {
				m[linha].j[contador - 1].numero_da_coluna = coluna;
				m[linha].j[contador - 1].valor = valor;

				break;
			}

			m[linha].j[contador - 1].numero_da_coluna = m[linha].j[contador - 2].numero_da_coluna;
			m[linha].j[contador - 1].valor = m[linha].j[contador - 2].valor;

			contador--;
		} while (1);
	}
}

void preecnher_matriz(Linha* m) {
	int contador = 0;

	for (contador = 0; contador < tamanho; contador++) {
		add_valor(contador, contador, contador + 1, m);
		add_valor(contador, contador + 1, 0.5, m);
		add_valor(contador, contador + 2, 0.5, m);
		add_valor(contador + 1, contador, 0.5, m);
		add_valor(contador + 2, contador, 0.5, m);
	}
}

void copia(Linha* m, Linha* m2) {
	int contador = 0;
	int contador2 = 0;

	m2 = cria_matriz();

	for (contador = 0; contador < tamanho; contador++)
		for (contador2 = 0; contador2 < m[contador].numero_de_colunas; contador2++)
			add_valor(contador, m[contador].j[contador2].numero_da_coluna, m[contador].j[contador2].valor, m2);

}

void print(Linha* x) {
	int i, j;

	for (i = 0; i < tamanho; i++) {
		if (x[i].numero_de_colunas != 0) {
			printf("[ ");
			for (j = 0; j < tamanho; j++) {
				if (busca_binaria(i, j, x) == -1)
					printf("%f ", (double)0);
				else
					printf("%f ", x[i].j[busca_binaria(i, j, x)].valor);
			}
			printf("] \n");
		}
	}
	printf("-------------------------------------------\n");
}

void adicao_vetores(Linha* v1, Linha* v2, Linha* v3) {

	int contador = 0;
	int posicao = -1;

	copia(v1, v3);

	for (contador = 0; contador < v2[0].numero_de_colunas; contador++) {
		posicao = busca_binaria(0, v2[0].j[contador].numero_da_coluna, v3);
		if (posicao == -1)
			add_valor(0, v2[0].j[contador].numero_da_coluna, v2[0].j[contador].valor, v3);
		else
			add_valor(0, v2[0].j[contador].numero_da_coluna, v3[0].j[posicao].valor + v2[0].j[contador].valor, v3);
	}
}

void multiplicacao_matriz_vetor(Linha* m, Linha* v1, Linha* v2) {
	int contadorLinha = 0;
	int contadorColuna = 0;
	int contador;

	double soma;

	int pos_da_busca_1;
	int pos_da_busca_2;

	Linha* temp;

	v2 = cria_matriz();
	temp = cria_matriz();

	for (contadorLinha = 0; contadorLinha < tamanho; contadorLinha++) {
		for (contadorColuna = 0; contadorColuna < v1[contadorLinha].numero_de_colunas; contadorColuna++)
			add_valor(v1[contadorLinha].j[contadorColuna].numero_da_coluna, contadorLinha, v1[contadorLinha].j[contadorColuna].valor, v2);
	}

	copia(v2, temp);

	for (contadorLinha = 0; contadorLinha < tamanho; contadorLinha++) {

		for (contadorColuna = 0; contadorColuna < tamanho; contadorColuna++) {
			soma = 0;

			for (contador = 0; contador < tamanho; contador++) {
				pos_da_busca_1 = busca_binaria(contadorLinha, contador, m);

				if (pos_da_busca_1 == -1) {
					add_valor(contadorLinha, contadorColuna, soma, v2);
					continue;
				}

				pos_da_busca_2 = busca_binaria(contador, contadorColuna, temp);

				if (pos_da_busca_2 == -1) {
					add_valor(contadorLinha, contadorColuna, soma, v2);
					continue;
				}

				soma = soma + (m[contadorLinha].j[pos_da_busca_1].valor * temp[contador].j[pos_da_busca_2].valor);
				add_valor(contadorLinha, contadorColuna, soma, v2);
			}
		}
	}

	temp = cria_matriz();

	for (contadorLinha = 0; contadorLinha < tamanho; contadorLinha++) {
		for (contadorColuna = 0; contadorColuna < v2[contadorLinha].numero_de_colunas; contadorColuna++)
			add_valor(v2[contadorLinha].j[contadorColuna].numero_da_coluna, contadorLinha, v2[contadorLinha].j[contadorColuna].valor, temp);
	}

	copia(temp, v2);
}

void multiplicacao_escalar_vetor(double x, Linha* v1, Linha* v2) {
	int contadorLinha = 0;
	int contadorColuna = 0;

	int resposta_busca = -1;

	v2 = cria_matriz();

	for (contadorLinha = 0; contadorLinha < tamanho; contadorLinha++) {
		for (contadorColuna = 0; contadorColuna < tamanho; contadorColuna++) {
			resposta_busca = busca_binaria(contadorLinha, contadorColuna, v1);

			if (resposta_busca != -1)
				add_valor(contadorLinha, contadorColuna, v1[contadorLinha].j[resposta_busca].valor * x, v2);
		}
	}
}

double multiplicacao_vetor_vetor(Linha* v1, Linha* v2) {

	Linha* v3 = cria_matriz();

	multiplicacao_matriz_vetor(v1, v2, v3);

	if (v3[0].numero_de_colunas != 0)
		return v3[0].j[0].valor;
	return 0;
}

void ConjugateGradient(int n, Linha* A, Linha* b, Linha* x, double tol) {

	int iteracoes = 0;

	double alfa, beta, alfaNum, alfaDen, betaNum;

	Linha* Ax = cria_matriz();
	Linha* menosAx = cria_matriz();
	Linha* r = cria_matriz();
	Linha* d = cria_matriz();
	Linha* Ad = cria_matriz();
	Linha* alfad = cria_matriz();
	Linha* alfaAd = cria_matriz();
	Linha* x1 = cria_matriz();
	Linha* menosalfaAd = cria_matriz();
	Linha* r1 = cria_matriz();
	Linha* betad = cria_matriz();
	Linha* d1 = cria_matriz();


	// d = r = b - Ax
	multiplicacao_matriz_vetor(A, x, Ax);
	multiplicacao_escalar_vetor(-1, Ax, menosAx);
	adicao_vetores(b, menosAx, r);
	copia(r, d);

	for (iteracoes = 1; iteracoes < n; iteracoes++) {

		// Se todos os numeros tiverem dentro da tolerancia
		for (int i = 0; i < r[0].numero_de_colunas; i++) {
			if (r[0].j[i].valor > tol) {
				printf("%f \n", r[0].j[i].valor);
				break;
			}

			if (i + 1 == r[0].numero_de_colunas) {
				printf("Gradiente Conjugado para uma matriz de tamanho %d, iteracoes: %d\n", n, iteracoes);
				return;
			}
		}


		// alfa
		alfaNum = multiplicacao_vetor_vetor(r, r);
		multiplicacao_matriz_vetor(A, d, Ad);
		alfaDen = multiplicacao_vetor_vetor(d, Ad);
		alfa = alfaNum / alfaDen;

		// x k+1
		multiplicacao_escalar_vetor(alfa, d, alfad);
		adicao_vetores(x, alfad, x1);

		//r k+1
		multiplicacao_escalar_vetor(alfa, Ad, alfaAd);
		multiplicacao_escalar_vetor(-1, alfaAd, menosalfaAd);
		adicao_vetores(r, menosalfaAd, r1);

		// beta
		betaNum = multiplicacao_vetor_vetor(r1, r1);
		beta = betaNum / alfaNum;

		// d k+1
		multiplicacao_escalar_vetor(beta, d, betad);
		adicao_vetores(r1, betad, d1);

		copia(x1, x);
		copia(r1, r);
		copia(d1, d);
	}

	printf("Gradiente Conjugado para uma matriz de tamanho %d, iteracoes: %d\n", n, iteracoes);
}

void GC(void) {
	Linha* m1;
	Linha* v1;
	Linha* v2;

	printf("Diga o tamanho da matriz: ");
	scanf("%d", &tamanho);

	m1 = cria_matriz();
	v1 = cria_matriz();
	v2 = cria_matriz();

	preecnher_matriz(m1);

	print(m1);

	for (int i = 0; i < tamanho; i++)
		add_valor(0, i, 1, v2);

	print(v2);

	multiplicacao_matriz_vetor(m1, v2, v1);

	print(v1);

	for (int i = 0; i < tamanho; i++)
		add_valor(0, i, 0, v2);

	// numero de linha, A, b, x
	//ConjugateGradient(tamanho, m1, v1, v2, 0.1);

	//print(v2);

	return;
}


int main(void) {

	GC();

	return;
}

