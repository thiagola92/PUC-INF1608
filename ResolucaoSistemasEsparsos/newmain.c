#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
		add_valor(contador, contador, contador+1, m);
		add_valor(contador, contador + 1, 0.5, m);
		add_valor(contador, contador + 2, 0.5, m);
		add_valor(contador + 1, contador, 0.5, m);
		add_valor(contador + 2, contador, 0.5, m);
	}
}

void preecnher_matriz_2(Matriz* m) {
	int contador = 0;

	for (contador = 0; contador < m->numero_de_linhas; contador++) {
		add_valor(contador, contador, contador + 1, m);
		add_valor(contador, contador + 1, 0.5, m);
		add_valor(contador, contador + 2, 0.5, m);
		add_valor(contador + 1, contador, 0.5, m);
		add_valor(contador + 2, contador, 0.5, m);
		if (contador < m->numero_de_linhas/2) {
			add_valor(contador, (contador+1) * 2 - 1, 0.5, m);
			add_valor((contador+1) * 2 - 1, contador, 0.5, m);
		}
	}
}

void copia(Matriz* m, Matriz* m2) {
	int contador = 0;
	int contador2 = 0;

	cria_matriz(m->numero_de_linhas, m2);

	for (contador = 0; contador < m->numero_de_linhas; contador++)
		for (contador2 = 0; contador2 < m->i[contador].numero_de_colunas; contador2++)
			add_valor(contador, m->i[contador].j[contador2].numero_da_coluna, m->i[contador].j[contador2].valor, m2);

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
	printf("-----------------------------------------------\n");
}

void adicao_vetores(Matriz* v1, Matriz* v2, Matriz* v3) {
	
	int contador = 0;
	int posicao = -1;

	copia(v1, v3);

	for (contador = 0; contador < v2->i[0].numero_de_colunas; contador++) {
		posicao = busca_binaria(0, v2->i[0].j[contador].numero_da_coluna, v3);
		if (posicao == -1)
			add_valor(0, v2->i[0].j[contador].numero_da_coluna, v2->i[0].j[contador].valor, v3);
		else
			add_valor(0, v2->i[0].j[contador].numero_da_coluna, v3->i[0].j[posicao].valor + v2->i[0].j[contador].valor, v3);
	}
}

void multiplicacao_matriz_vetor(Matriz* m, Matriz* v1, Matriz* v2) {
	int contadorLinha = 0;
	int contadorColuna = 0;
	int contador;

	int busca;

	double soma;

	cria_matriz(m->numero_de_linhas, v2);

	for (contadorLinha = 0; contadorLinha < m->numero_de_linhas; contadorLinha++) {

		soma = 0;

		for (contadorColuna = 0; contadorColuna < m->i[contadorLinha].numero_de_colunas; contadorColuna++) {

			busca = busca_binaria(0, m->i[contadorLinha].j[contadorColuna].numero_da_coluna, v1);

			if (busca == -1)
				continue;

			soma = soma + (m->i[contadorLinha].j[contadorColuna].valor * v1->i[0].j[busca].valor);
		}

		if (soma != 0)
			add_valor(contadorLinha, 0, soma, v2);
	}

}

void multiplicacao_escalar_vetor(double x, Matriz* v1, Matriz* v2) {
	int contadorLinha = 0;
	int contadorColuna = 0;

	int resposta_busca = -1;

	cria_matriz(v1->numero_de_linhas, v2);

	for (contadorLinha = 0; contadorLinha < v1->numero_de_linhas; contadorLinha++) {
		for (contadorColuna = 0; contadorColuna < v1->i[contadorLinha].numero_de_colunas; contadorColuna++) {
			add_valor(contadorLinha, v1->i[contadorLinha].j[contadorColuna].numero_da_coluna, v1->i[contadorLinha].j[contadorColuna].valor * x, v2);
		}
	}
}

double multiplicacao_vetor_vetor(Matriz* v1, Matriz* v2) {

	Matriz v3;

	multiplicacao_matriz_vetor(v1, v2, &v3);

	if (v3.i[0].numero_de_colunas != 0)
		return v3.i[0].j[0].valor;
	return 0;
}

void ConjugateGradient(Matriz* A, Matriz* b, Matriz* x, double tol) {

	int iteracoes = 0;

	double alfa, beta, alfaNum, alfaDen, betaNum;

	Matriz Ax;
	Matriz menosAx;
	Matriz r;
	Matriz d;
	Matriz Ad;
	Matriz alfad;
	Matriz alfaAd;
	Matriz x1;
	Matriz menosalfaAd;
	Matriz r1;
	Matriz betad;
	Matriz d1;

	multiplicacao_matriz_vetor(A, x, &Ax);

	// d = r = b - Ax
	multiplicacao_escalar_vetor(-1, &Ax, &menosAx);
	adicao_vetores(b, &menosAx, &r);
	copia(&r, &d);

	for (iteracoes = 1; iteracoes < A->numero_de_linhas; iteracoes++) {

		// Se todos os numeros tiverem dentro da tolerancia
		for (int i = 0; i < r.i[0].numero_de_colunas; i++) {
			if (r.i[0].j[i].valor > tol) {
				//printf("%f \n", r.i[0].j[i].valor);
				break;
			}

			if (i + 1 == r.i[0].numero_de_colunas) {
				printf("Gradiente Conjugado \t iteracoes: %d \n\n\n", iteracoes);
				return;
			}
		}


		// alfa
		alfaNum = multiplicacao_vetor_vetor(&r, &r);
		multiplicacao_matriz_vetor(A, &d, &Ad);
		alfaDen = multiplicacao_vetor_vetor(&d, &Ad);
		alfa = alfaNum / alfaDen;

		// x k+1
		multiplicacao_escalar_vetor(alfa, &d, &alfad);
		adicao_vetores(x, &alfad, &x1);

		//r k+1
		multiplicacao_escalar_vetor(alfa, &Ad, &alfaAd);
		multiplicacao_escalar_vetor(-1, &alfaAd, &menosalfaAd);
		adicao_vetores(&r, &menosalfaAd, &r1);

		// beta
		betaNum = multiplicacao_vetor_vetor(&r1, &r1);
		beta = betaNum / alfaNum;

		// d k+1
		multiplicacao_escalar_vetor(beta, &d, &betad);
		adicao_vetores(&r1, &betad, &d1);

		copia(&x1, x);
		copia(&r1, &r);
		copia(&d1, &d);
	}

	printf("Gradiente Conjugado \t iteracoes: %d \n\n\n", iteracoes);
}

void main(void) {
	Matriz A;
	Matriz b;
	Matriz x;

	double tol = 0.00001;

	int tamanho=5;
	printf("Tamanho da matriz: %d\n", tamanho);
	printf("Tolerancia: %g\n\n", tol);

	
	cria_matriz(tamanho, &A);
	cria_matriz(tamanho, &b);
	cria_matriz(tamanho, &x);

	// Importante para descobrir b
	for (int i = 0; i < tamanho; i++)
		add_valor(0, i, 1, &x);

	preecnher_matriz(&A);

	print(&A);

	//preecnher_matriz_2(&A);

	multiplicacao_matriz_vetor(&A, &x, &b);

	// Importante pois você quer x zerado
	for (int i = 0; i < tamanho; i++)
		add_valor(0, i, 0, &x);

	// A, b, x, tolerancia
	ConjugateGradient(&A, &b, &x, tol);

	print(&x);

	return;
}

