#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

// Se você quiser ver os prints que mostram etapa por etapa de uma certa função, coloque o debug dela como true

#define DEBUG_add_valor false
#define DEBUG_busca_binaria false
#define DEBUG_adicao_vetores false
#define DEBUG_multiplicacao_matriz_vetor false
#define DEBUG_copia false

// Lembrete: linha 1 coluna 1 é M[0][0]

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

/******************* FUNÇÕES *******************/

void cria_linhas(int n, Matriz* m);
void add_valor(int linha, int coluna, double valor, Matriz* m);
void preencher_matriz(Matriz* m);
void adicao_vetores(Matriz* v1, Matriz v2, Matriz *v3);
int busca_binaria(int linha, int coluna, Matriz* m);
void multiplicacao_matriz_vetor(Matriz* m, Matriz* v1, Matriz* v2);
void multiplicacao_escalar_vetor(double x, Matriz* v1, Matriz* v2);
double multiplicacao_vetor_vetor(Matriz* v1, Matriz* v2);
void copia(Matriz* a, Matriz* b);
void transposta(Matriz* a, Matriz* b);
void print(Matriz* x);


void ConjugateGradient(int n, Matriz* A, Matriz* b, Matriz* x);



/***********************************************/

/*
 * Cria n linhas na matriz m
 * n >> numero de linhas
 * m >> matriz que vai receber esse numero de linhas
*/
void cria_linhas(int n, Matriz* m)
{
	int contador;

	m->i = (Linha*)malloc(sizeof(Linha)*n);

	// Começa dizendo que não tem nenhuma coluna onde não tenha zero
	for (contador = 0; contador < n; contador++)
		m->i[contador].numero_de_colunas = 0;

	m->numero_de_linhas = n;
}

/*
 * Adiciona um elemento a matriz
 * linha >> linha onde o elemento vai ser adicionado
 * coluna >> coluna onde o elemento vai ser adicionado
 * valor >> valor do elemento vai ser adicionado
 * m >> matriz que vai ter o elemento adicionado
 * Caso tente adicionar um elemento que já existe, vai trocar o valor daquele elemento
*/
void add_valor(int linha, int coluna, double valor, Matriz* m)
{
	int resposta_da_busca_binaria;
	int contador;

	if (DEBUG_add_valor)
		printf("#Add valor \n");

	// Apenas verificando se não esta tentando adicionar elemento em uma linha/coluna maior que a da matriz
	if (linha >= m->numero_de_linhas)
	{
		if (DEBUG_add_valor) {
			printf("Nao existe a linha %d que voce deseja botar elemento \n", linha);
			printf("#Fim do add valor \n\n");
		}

		return;
	} else if (coluna >= m->numero_de_linhas)
	{
		if (DEBUG_add_valor) {
			printf("Nao existe a coluna %d que voce deseja botar elemento \n", coluna);
			printf("#Fim do add valor \n\n");
		}

		return;
	}

	if (DEBUG_add_valor)
		printf("Tentando inserir o valor %f em m[%d][%d]: \n", valor, linha, coluna);

	// Se a coluna já existir você só vai querer atualizar o elemento dela
	resposta_da_busca_binaria = busca_binaria(linha, coluna, m);
	if (resposta_da_busca_binaria != -1) {

		if (valor == 0) {

			if (DEBUG_add_valor) {
				printf("Tratando para quando um dos elementos se torna 0 \n");
				printf("#Fim do add valor \n\n");
			}

			// Esse loop vai passar esse elemento com 0 pro final da coluna para que possamos depois dar realloc
			for (; resposta_da_busca_binaria+1 < m->i[linha].numero_de_colunas; resposta_da_busca_binaria++) {
				m->i[linha].j[resposta_da_busca_binaria].numero_da_coluna = m->i[linha].j[resposta_da_busca_binaria+1].numero_da_coluna;
				m->i[linha].j[resposta_da_busca_binaria].valor = m->i[linha].j[resposta_da_busca_binaria+1].valor;
			}

			m->i[linha].numero_de_colunas--;
			m->i[linha].j = (Coluna*)realloc(m->i[linha].j, sizeof(Coluna) * m->i[linha].numero_de_colunas);

			return;
		}

		m->i[linha].j[resposta_da_busca_binaria].valor = valor;

		if (DEBUG_add_valor) {
			printf("Voce alterou o valor da linha %d e coluna %d para %f \n", linha, coluna, valor);
			printf("#Fim do add valor \n\n");
		}

		return;
	}

	// Caso seja 0 você não precisa adicionar no vetor, nós sabemos que toda parte da matriz sem numero é zero
	if (valor == 0) {

		if (DEBUG_add_valor) {
			printf("Voce tentou adicionar um elemento com zero, isso nao eh necessario \n");
			printf("#Fim do add valor \n\n");
		}

		return;
	}

	// Caso não tenha nenhuma coluna com valor e caso já tenha
	if (m->i[linha].numero_de_colunas == 0)
	{
		m->i[linha].numero_de_colunas = 1;
		m->i[linha].j = (Coluna*)malloc(sizeof(Coluna));
		m->i[linha].j->numero_da_coluna = coluna;
		m->i[linha].j->valor = valor;


		if (DEBUG_add_valor) {
			// Informando como foi adicionado e onde
			printf("- Foi o primeiro elemento do vetor a ser adicionado \n");
			printf("- Numero de linha adicionada: %d \n", linha);
			printf("- Numero de colunas com nao zero nessa linha: %d \n", m->i[linha].numero_de_colunas);
			printf("- Numero da coluna adicionada: %d \n", m->i[linha].j->numero_da_coluna);
			printf("- Valor nessa linha e coluna: %f \n", m->i[linha].j->valor);
		}

	} else {
		m->i[linha].numero_de_colunas++;
		m->i[linha].j = (Coluna*)realloc(m->i[linha].j, sizeof(Coluna) * m->i[linha].numero_de_colunas);

		// Agora nos queremos que fique em ordem de colunas então temos que adicionar no local certo
		contador = m->i[linha].numero_de_colunas;
		do {
			// Caso a coluna adicionada seja a menor de todas
			if ( (contador - 2) < 0) {
				m->i[linha].j[contador-1].numero_da_coluna = coluna;
				m->i[linha].j[contador-1].valor = valor;


				if (DEBUG_add_valor) {
					// Informando como foi adicionado e onde
					printf("- Se tornou o primeiro da lista \n");
					printf("- Numero de linha adicionada: %d \n", linha);
					printf("- Numero de colunas com nao zero nessa linha: %d \n", m->i[linha].numero_de_colunas);
					printf("- Numero da coluna adicionada: %d \n", m->i[linha].j[contador - 1].numero_da_coluna);
					printf("- Valor nessa linha e coluna: %f \n", m->i[linha].j[contador - 1].valor);
				}

				break;
			}

			// Caso a coluna da esquerda seja menor, já pode salvar
			if (m->i[linha].j[contador-2].numero_da_coluna < coluna) {
				m->i[linha].j[contador-1].numero_da_coluna = coluna;
				m->i[linha].j[contador-1].valor = valor;


				if (DEBUG_add_valor) {
					// Informando como foi adicionado e onde
					printf("- Ele ficou ou no meio de alguma coluna ou eh a ultima \n");
					printf("- Numero de linha adicionada: %d \n", linha);
					printf("- Numero de colunas com nao zero nessa linha: %d \n", m->i[linha].numero_de_colunas);
					printf("- Numero da coluna adicionada: %d \n", m->i[linha].j[contador - 1].numero_da_coluna);
					printf("- Valor nessa linha e coluna: %f \n", m->i[linha].j[contador - 1].valor);
				}

				break;
			} else {
				
				// Caso a coluna do contador-2 seja maior, você tem que mover a coluna do contador-2 para direita e continuar procurando onde botar a coluna nova
				m->i[linha].j[contador-1].numero_da_coluna = m->i[linha].j[contador-2].numero_da_coluna;
				m->i[linha].j[contador-1].valor = m->i[linha].j[contador-2].valor;

				contador--;
			}

			// Essa condicao do while não é recomendada
		} while (1);
	}

	if (DEBUG_add_valor) {
		printf("#Fim do add valor \n\n");
	}
}

// Preenche a matriz conforme o enunciado do trabalho
void preencher_matriz(Matriz* m)
{
	int contador;		// Vai ser usado para percorrer toda a matriz

	// Conforme o enunciado manda
	for (contador = 0; contador < m->numero_de_linhas; contador++) {
		add_valor(contador, contador, contador, m);
		add_valor(contador, contador+1, 0.5, m);
		add_valor(contador, contador+2, 0.5, m);
		add_valor(contador+1, contador, 0.5, m);
		add_valor(contador+2, contador, 0.5, m);
	}
}

/*
 * Faz a busca por um elemento da matriz
 * linha >> linha na qual vai buscar e aplicar busca binaria
 * coluna >> coluna que vai buscar
 * m >> matriz onde vai ocorrer a busca
 * Return >> Retorna onde esta aquela coluna dentro do vetor, ou seja, retorna o indice , ex: m->i[linha]->j[INDICE QUE RETORNA]
*/
int busca_binaria(int linha, int coluna, Matriz* m)
{
	int posicao_inicial;		// Vamos aplicar busca binaria dessa parte do vetor
	int posicao_final;			// até essa parte do vetor

	int meio;

	if (DEBUG_busca_binaria)
		printf("#Busca binaria \n");

	// Caso esteja tentando procurar por alguma linha ou coluna que não exista
	if (linha >= m->numero_de_linhas || linha < 0) {

		if (DEBUG_busca_binaria) {
			printf("Essa linha nao existe nessa matriz \n");
			printf("#Fim da busca binaria \n\n");
		}

		return -1;
	} else if (coluna >= m->numero_de_linhas || coluna < 0) {

		if (DEBUG_busca_binaria) {
			printf("Essa coluna nao existe nessa matriz \n");
			printf("#Fim da busca binaria \n\n");
		}

		return -1;
	} else if (m->i[linha].numero_de_colunas <= 0) {

		if (DEBUG_busca_binaria) {
			printf("Nao existe coluna nessa linha \n");
			printf("#Fim da busca binaria \n\n");
		}

		return -1;
	}

	posicao_inicial = 0;										// Inicio do vetor
	posicao_final = m->i[linha].numero_de_colunas - 1;			// Final do vetor (se a linha tem 5 colunas então o final é 4)

	if (DEBUG_busca_binaria)
		printf("O vetor comeca na posicao 0 e vai ate a posicao %d \n", posicao_final);

	while (posicao_inicial < posicao_final) {

		// Descobrindo o meio entre a posicao inicial e final que estamos fazendo busca binaria, fiz em duas linhas para ficar mais organizado
		meio = (posicao_final - posicao_inicial + 1) / 2;
		meio = posicao_inicial + meio;
		
		if (DEBUG_busca_binaria) {
			printf("-Busca entre [%d] e [%d] \n", posicao_inicial, posicao_final);
			printf("-O meio e [%d] \n", meio);
		}

		// Se a coluna do meio for a que estamos procurando, finalizar retornando essa posicao. Caso isso não seja verdade mudar posicao final ou inicial de forma a fazer a busca binaria progredir
		if (m->i[linha].j[meio].numero_da_coluna == coluna) {

			if (DEBUG_busca_binaria)
				printf("#Fim da busca binaria \n\n");

			return meio;
		}
		else if (m->i[linha].j[meio].numero_da_coluna > coluna)
			posicao_final = meio - 1;
		else if (m->i[linha].j[meio].numero_da_coluna < coluna)
			posicao_inicial = meio + 1;

	}

	// Vai sair do loop quando posicao_inicial == posicao_final, ai checaremos se essa posicao é a coluna procurada, se for retorna a posicao_incial ou posicao_final
	if (m->i[linha].j[posicao_inicial].numero_da_coluna == coluna) {

		if (DEBUG_busca_binaria) {
			printf("posicao %d <----> coluna %d \n", posicao_inicial, m->i[linha].j[posicao_inicial].numero_da_coluna);
			printf("#Fim da busca binaria \n\n");
		}

		return posicao_inicial;
	} else {

		if (DEBUG_busca_binaria) {
			printf("Coluna %d nao existe \n", coluna);
			printf("#Fim da busca binaria \n\n");
		}

		return -1;
	}
}

/*
 * Adicao de vetores trata duas matrizes como se fossem um vetor.
 * Bota a soma deles em outro terceiro vetor passado
 * v1 >> primeiro vetor
 * v2 >> segundo vetor
 * v3 >> vetor com o resultado da adicao de ambos
*/
void adicao_vetores(Matriz* v1, Matriz* v2, Matriz *v3)
{
	int contador;
	int posicao;

	if (DEBUG_adicao_vetores)
		printf("#Adicao vetores \n");

	// Criando o vetor novo que vai ser a soma dos dois
	cria_linhas(v1->numero_de_linhas, v3);

	if (DEBUG_adicao_vetores)
		printf("copiar %d elementos \n", v1->i[0].numero_de_colunas);

	// Copia o primeiro vetor
	for (contador = 0; contador != v1->i[0].numero_de_colunas; contador++)
		add_valor(0, v1->i[0].j[contador].numero_da_coluna, v1->i[0].j[contador].valor , v3);

	if (DEBUG_adicao_vetores)
		printf("somar %d elementos \n", v2->i[0].numero_de_colunas);

	// Agora se a coluna existir somar se não, adicionar
	for (contador = 0; contador != v2->i[0].numero_de_colunas; contador++) {

		posicao = busca_binaria(0, v2->i[0].j[contador].numero_da_coluna, v3);

		if (posicao == -1) {

			add_valor(0, v2->i[0].j[contador].numero_da_coluna, v2->i[0].j[contador].valor, v3);

			if (DEBUG_adicao_vetores)
				printf("0 + %f = %f \n", v2->i[0].j[contador].valor, v3->i[0].j[contador].valor);

		} else {
			add_valor(0, v2->i[0].j[contador].numero_da_coluna, v1->i[0].j[posicao].valor + v2->i[0].j[contador].valor, v3);

			if (DEBUG_adicao_vetores)
				printf("%f + %f = %f \n", v1->i[0].j[posicao].valor, v2->i[0].j[contador].valor, v3->i[0].j[contador].valor);

		}
	}

	if (DEBUG_adicao_vetores)
		printf("#Fim adicao vetores \n\n");
}

/*
* Subtracao de vetores trata duas matrizes como se fossem um vetor.
* Bota a subtracao deles em outro terceiro vetor passado
* v1 >> primeiro vetor
* v2 >> segundo vetor
* v3 >> vetor com o resultado da subtracao de ambos
*/
void subtrai_vetores(Matriz* v1, Matriz* v2, Matriz *v3)
{
	int contador;
	int posicao;

	if (DEBUG_adicao_vetores)
		printf("#Adicao vetores \n");

	// Criando o vetor novo que vai ser a soma dos dois
	cria_linhas(v1->numero_de_linhas, v3);

	if (DEBUG_adicao_vetores)
		printf("copiar %d elementos \n", v1->i[0].numero_de_colunas);

	// Copia o primeiro vetor
	for (contador = 0; contador != v1->i[0].numero_de_colunas; contador++)
		add_valor(0, v1->i[0].j[contador].numero_da_coluna, v1->i[0].j[contador].valor, v3);

	if (DEBUG_adicao_vetores)
		printf("somar %d elementos \n", v2->i[0].numero_de_colunas);

	// Agora se a coluna existir somar se não, adicionar
	for (contador = 0; contador != v2->i[0].numero_de_colunas; contador++) {

		posicao = busca_binaria(0, v2->i[0].j[contador].numero_da_coluna, v3);

		if (posicao == -1) {

			add_valor(0, v2->i[0].j[contador].numero_da_coluna, -(v2->i[0].j[contador].valor), v3);

			if (DEBUG_adicao_vetores)
				printf("0 + %f = %f \n", v2->i[0].j[contador].valor, v3->i[0].j[contador].valor);

		}
		else {
			add_valor(0, v2->i[0].j[contador].numero_da_coluna, v1->i[0].j[posicao].valor - v2->i[0].j[contador].valor, v3);

			if (DEBUG_adicao_vetores)
				printf("%f + %f = %f \n", v1->i[0].j[posicao].valor, v2->i[0].j[contador].valor, v3->i[0].j[contador].valor);

		}
	}

	if (DEBUG_adicao_vetores)
		printf("#Fim adicao vetores \n\n");
}

/*
 * Multiplicação de matriz por vetor
 * Bota o resultado no segundo vetor passado
 * m >> matriz que vai ser usada
 * v1 >> vetor que vai multiplicar a matriz
 * v3 >> vetor que vai armazenar o resultado
 * OBS: Na verdade multiplica matriz por matriz mas quando a matriz só tem uma linha, não é nada mais que um vetor.
 * /\ Entenda isso como um "Você pode usar para multiplicar matriz por matriz tranquilamente"
 * OBS2: Ordem faz diferença no resultado
 */
void multiplicacao_matriz_vetor(Matriz* m, Matriz* v1, Matriz* v2)
{
	int contadorLinha;	// Contador da linha
	int contadorColuna;	// Contador da coluna
	int contador;		// Contador que vai de 0 até numero de linha/coluna para pegar os elementos

	double soma;		// Armazena a soma de um dos elementos que vai ficar numa posicao [i][j] do novo vetor

	int pos_da_busca_1;	// Guarda a posicao da coluna que vai retornar da busca binaria
	int pos_da_busca_2;	// Guarda a posicao da coluna que vai retornar da busca binaria (para a segunda matriz)
	
	if (DEBUG_multiplicacao_matriz_vetor)
		printf("#Multiplicacao matriz por vetor \n");

	// Vai linha por linha e coluna por coluna para fazer o calculo de matriz por vetor
	for (contadorLinha = 0; contadorLinha < m->numero_de_linhas; contadorLinha++) {

		if (DEBUG_multiplicacao_matriz_vetor)
			printf("-----Linha: %d \n", contadorLinha);

		for (contadorColuna = 0; contadorColuna < m->numero_de_linhas; contadorColuna++) {

			if (DEBUG_multiplicacao_matriz_vetor)
				printf("-----Coluna: %d \n", contadorColuna);

			soma = 0;

			for (contador = 0; contador < m->numero_de_linhas; contador++) {

				if (DEBUG_multiplicacao_matriz_vetor) {
					printf("-Andou uma coluna do primeiro \n");
					printf("-Andou uma linha do segundo \n");
					printf("-Para %d \n", contador);
					printf("-Estado do somatorio: %f \n", soma);
				}

				pos_da_busca_1 = busca_binaria(contadorLinha, contador, m);

				// Caso seja zero o valor do elemento, a multiplicação da zero
				if (pos_da_busca_1 == -1) {
					add_valor(contadorLinha, contadorColuna, soma, v2);

					if (DEBUG_multiplicacao_matriz_vetor) {
						printf("-Um dos elementos eh 0 (1) \n");
						printf("-Somatorio %f \n", soma);
					}

					continue;
				}

				pos_da_busca_2 = busca_binaria(contador, contadorColuna, v1);

				// Caso seja zero o valor do elemento, a multiplicação da zero
				if (pos_da_busca_2 == -1) {
					add_valor(contadorLinha, contadorColuna, soma, v2);

					if (DEBUG_multiplicacao_matriz_vetor) {
						printf("-Um dos elementos eh 0 (2) \n");
						printf("-Somatorio %f \n", soma);
					}

					continue;
				}


				// Caso ambos não sejam zero, multiplicar e somar ao somatorio para botar naquela linha e coluna
				soma = soma + (m->i[contadorLinha].j[pos_da_busca_1].valor * v1->i[contador].j[pos_da_busca_2].valor);
				add_valor(contadorLinha, contadorColuna, soma,v2);

				if (DEBUG_multiplicacao_matriz_vetor) {
					printf("%f \n", m->i[contadorLinha].j[pos_da_busca_1].valor);
					printf("%f \n", v1->i[contador].j[pos_da_busca_2].valor);
					printf("-Nenhum dos elementos eh 0 \n");
					printf("-Somatorio %f \n", soma);
				}
			}

		}
	}

	if (DEBUG_multiplicacao_matriz_vetor) {
		printf("#Fim da multiplicacao matriz por vetor \n");
	}

}

/*
 * Multiplica um vetor por um escalar
 * x >> O valor pelo qual você vai multiplica tudo no vetor
 * v1 >> Vetor que vai ser usado como base para multiplicar
 * v2 >> Resultado vai ser botado nesse vetor
 * OBS: É nada mais que multiplicar um escalar por uma matriz.
 * /\ Sim, você pode usar para fazer escalar vezes matriz
*/
void multiplicacao_escalar_vetor(double x, Matriz* v1, Matriz* v2)
{
	int contadorLinha;
	int contadorColuna;

	int resposta_busca;

	double multplicacao_temp;

	for (contadorLinha = 0; contadorLinha < v1->numero_de_linhas; contadorLinha++) {

		for (contadorColuna = 0; contadorColuna < v1->numero_de_linhas; contadorColuna++) {

			// Resposta da busca binaria, se for 0 retorna -1, se não a posicao do vetor
			resposta_busca = busca_binaria(contadorLinha, contadorColuna, v1);

			// Caso não seja zero, salvar no segundo vetor
			if (resposta_busca != -1) {
				multplicacao_temp = v1->i[contadorLinha].j[resposta_busca].valor * x;
				add_valor(contadorLinha, contadorColuna, multplicacao_temp, v2);
			}

		}

	}
}

/*
 * Multiplicação de vetor por outro vetor que resulta em escalar
 * v1 >> primeiro vetor
 * v2 >> segundo vetor
 * Retorna >> O escalar
 */
double multiplicacao_vetor_vetor(Matriz* v1, Matriz* v2) {

	Matriz v2T;
	Matriz v3;

	cria_linhas(v2->numero_de_linhas, &v2T);
	cria_linhas(v2->numero_de_linhas, &v3);


	transposta(v2, &v2T);

	multiplicacao_matriz_vetor(v1, &v2T, &v3);

	if (v3.i[0].numero_de_colunas != 0) 
		return v3.i[0].j[0].valor;
	else
		return 0;

}

/*
 * Copia uma matriz em outra
 * a >> matriz a qual vai ter os elementos copiados
 * b >> matriz que vai receber a copia
*/
void copia(Matriz* a, Matriz* b) {

	int i, j;		// Contadores

	if (a->numero_de_linhas != b->numero_de_linhas) {
		printf("!!!Erro as matrizes/vetores não são do mesmo tamanho");
		return;
	}

	for (i = 0; i < a->numero_de_linhas; i++) {


		for (j = 0; j < a->i[i].numero_de_colunas; j++) {

			if (DEBUG_copia) {
				printf("-Copiando i[%d] j[%d] \n", i, j);
				printf("-Numero da coluna: %d \n", a->i[i].j[j].numero_da_coluna);
				printf("-Valor: %f \n", a->i[i].j[j].valor);
			}

			add_valor(i, a->i[i].j[j].numero_da_coluna, a->i[i].j[j].valor, b);
		}

	}

}

void transposta(Matriz* a, Matriz* b) {

	int i, j;

	for (i = 0; i < a->numero_de_linhas; i++) {

		for (j = 0; j < a->i[i].numero_de_colunas; j++) {
			add_valor(a->i[i].j[j].numero_da_coluna, i, a->i[i].j[j].valor, b);
		}

	}

}

/*
 *Printa a matriz
*/
void print(Matriz* x) {

	int i, j;

	for (i = 0; i < x->numero_de_linhas; i++) {

		if (x->i[i].numero_de_colunas != 0) {

			printf("[ ");

			for (j = 0; j < x->numero_de_linhas; j++) {

				if (busca_binaria(i, j, x) == -1) {
					printf("%f ", 0);
				}
				else {
					printf("%f ", x->i[i].j[busca_binaria(i, j, x)].valor);
				}

			}

			printf("] \n");
		}

	}

}

void filtro(Matriz* x) {
	int i;
	for (i = 1; i < x->numero_de_linhas; i++) {
		x->i[i].numero_de_colunas = 0;
	}
}

/*
int main (void) {
	Matriz m;				// Matriz que o programa vai interagir
	Matriz v1;				// Vetor 1
	Matriz v2;				// Vetor 2
	Matriz v3;				// Vetor 3

	/////////////////////////// FIM DE VARIAVEIS /////////////////////////////////

	
	printf("---------------------------------------------------\n");
	printf("\t Comecando a execucao do programa. \n");
	printf("Diga o tamanho da matriz quadrada: \n");
	scanf("%d", &m.numero_de_linhas);
	getchar();
	printf("Tamanho: %d x %d \n", m.numero_de_linhas, m.numero_de_linhas);

	cria_linhas(m.numero_de_linhas, &m);

	printf("---------------------------------------------------\n");
	printf("\t Preenchendo a matriz conforme o enunciado. \n");

	preencher_matriz(&m);

	printf("Terminou de preencher a matriz \n");

	////////////////////////////////////TESTES//////////////////////////////////////////

	printf("---------------------------------------------------\n");
	printf("\t Testando busca binaria em elementos aleatorios. \n");
	printf("Se retorna -1 eh porque nao encontrou. \n\n");
	printf("Buscando a posicao do vetor do elemento %dx%d --> %d \n\n", m.numero_de_linhas, m.numero_de_linhas, busca_binaria(m.numero_de_linhas-1, m.numero_de_linhas-1, &m));
	printf("Buscando a posicao do vetor do elemento 1x1 --> %d \n\n", busca_binaria(0, 0, &m));
	printf("Buscando a posicao do vetor do elemento 1x%d --> %d \n\n", m.numero_de_linhas, busca_binaria(0, m.numero_de_linhas-1, &m));

	printf("---------------------------------------------------\n");
	printf("\t Testando a soma de vetores(nesse caso vetores sao matrizes com uma linha). \n");

	cria_linhas(3, &v1);
	cria_linhas(3, &v2);
	cria_linhas(3, &v3);

	printf("Um vetor vai ser [ 1 2 3 ] \n");
	add_valor(0, 0, 1, &v1);
	add_valor(0, 1, 2, &v1);
	add_valor(0, 2, 3, &v1);

	printf("O outro vai ser [ 0 4 -3 ] \n");
	add_valor(0, 1, 4, &v2);
	add_valor(0, 2, 5, &v2);
	add_valor(0, 2, 0, &v2);
	add_valor(0, 2, -3, &v2);

	adicao_vetores(&v1, &v2, &v3);

	printf("Resultado = \n");
	printf("[%f,%f,%f] \n", v3.i[0].j[0].valor, v3.i[0].j[1].valor, v3.i[0].j[2].valor);
	printf("numero de colunas n zero: %d \n", v3.i[0].numero_de_colunas);

	printf("---------------------------------------------------\n");
	printf("\t Testando a multiplicacao de matriz por vetor. \n");

	printf("Uma matriz com \n [ 1 2 3 ] \n [ 4 5 6 ] \n [ 7 8 9 ] \n");
	add_valor(1, 0, 1, &v1);
	add_valor(1, 1, 2, &v1);
	add_valor(1, 2, 3, &v1);
	add_valor(1, 0, 4, &v1);
	add_valor(1, 1, 5, &v1);
	add_valor(1, 2, 6, &v1);
	add_valor(2, 0, 7, &v1);
	add_valor(2, 1, 8, &v1);
	add_valor(2, 2, 9, &v1);

	printf("Um vetor com \n");
	printf(" [ 2 ] \n");
	printf(" [ 2 ] \n");
	printf(" [ 0 ] \n");
	add_valor(0, 0, 2, &v2);
	add_valor(1, 0, 2, &v2);
	add_valor(2, 0, 0, &v2);
	add_valor(0, 1, 0, &v2);
	add_valor(1, 1, 0, &v2);
	add_valor(2, 1, 0, &v2);
	add_valor(0, 2, 0, &v2);
	add_valor(1, 2, 0, &v2);
	add_valor(2, 2, 0, &v2);

	multiplicacao_matriz_vetor(&v1, &v2, &v3);

	printf("Resultado = \n");
	printf(" [ %f %f %f] \n [ %f %f %f] \n [ %f %f %f] \n", v3.i[0].j[0].valor, v3.i[0].j[1].valor, v3.i[0].j[2].valor, v3.i[1].j[0].valor, v3.i[1].j[1].valor, v3.i[1].j[2].valor, v3.i[2].j[0].valor, v3.i[2].j[1].valor, v3.i[2].j[2].valor);

	printf("---------------------------------------------------\n");
	printf("\t Testando a multiplicacao de escalar por vetor. \n");

	printf("Vetor com [ 1 2 0 ] \n");
	add_valor(0, 0, 1, &v1);
	add_valor(0, 1, 2, &v1);
	add_valor(0, 2, 0, &v1);
	add_valor(1, 0, 0, &v1);
	add_valor(1, 1, 0, &v1);
	add_valor(1, 2, 0, &v1);
	add_valor(2, 0, 0, &v1);
	add_valor(2, 1, 0, &v1);
	add_valor(2, 2, 0, &v1);

	printf("Escalar = 3 \n");

	add_valor(0, 0, 0, &v2);
	add_valor(0, 1, 0, &v2);
	add_valor(0, 2, 0, &v2);
	add_valor(1, 0, 0, &v2);
	add_valor(1, 1, 0, &v2);
	add_valor(1, 2, 0, &v2);
	add_valor(2, 0, 0, &v2);
	add_valor(2, 1, 0, &v2);
	add_valor(2, 2, 0, &v2);

	multiplicacao_escalar_vetor(3, &v1, &v2);

	printf("Resultado = \n");
	printf(" [ %f %f %f ] \n", v2.i[0].j[0].valor, v2.i[0].j[1].valor, v2.i[0].j[2].valor);

	printf("---------------------------------------------------\n");
	printf("\t Testando produto escalar entre vetores. \n");

	printf("Vetor com [ 1 2 0 ] \n");
	add_valor(0, 0, 1, &v1);
	add_valor(0, 1, 2, &v1);
	add_valor(0, 2, 0, &v1);
	add_valor(1, 0, 0, &v1);
	add_valor(1, 1, 0, &v1);
	add_valor(1, 2, 0, &v1);
	add_valor(2, 0, 0, &v1);
	add_valor(2, 1, 0, &v1);
	add_valor(2, 2, 0, &v1);

	printf("Um vetor com [ 2 2 3] \n");
	add_valor(0, 0, 2, &v2);
	add_valor(0, 1, 0, &v2);
	add_valor(0, 2, 0, &v2);
	add_valor(1, 0, 2, &v2);
	add_valor(1, 1, 0, &v2);
	add_valor(1, 2, 0, &v2);
	add_valor(2, 0, 3, &v2);
	add_valor(2, 1, 0, &v2);
	add_valor(2, 2, 0, &v2);

	add_valor(0, 0, 0, &v3);
	add_valor(0, 1, 0, &v3);
	add_valor(0, 2, 0, &v3);
	add_valor(1, 0, 0, &v3);
	add_valor(1, 1, 0, &v3);
	add_valor(1, 2, 0, &v3);
	add_valor(2, 0, 0, &v3);
	add_valor(2, 1, 0, &v3);
	add_valor(2, 2, 0, &v3);

	multiplicacao_matriz_vetor(&v1, &v2, &v3);

	printf("Resultado = \n");
	printf(" [ %f ] \n", v3.i[0].j[0].valor);
	
	printf("---------------------------------------------------\n");
	printf("\t Testando copia vetores. \n");

	cria_linhas(3, &v1);
	cria_linhas(3, &v2);

	printf("Um vetor vai ser [ 1 2 3 ] \n [ 4 4 4 ] \n");
	add_valor(0, 0, 1, &v1);
	add_valor(0, 1, 2, &v1);
	add_valor(0, 2, 3, &v1);
	add_valor(1, 0, 4, &v1);
	add_valor(1, 1, 4, &v1);
	add_valor(1, 2, 4, &v1);

	copia(&v1, &v2);

	printf("Resultado = [ %f %f %f ] \n", v2.i[0].j[0].valor, v2.i[0].j[1].valor, v2.i[0].j[2].valor);
	printf("Resultado = [ %f %f %f ] \n", v2.i[1].j[0].valor, v2.i[1].j[1].valor, v2.i[1].j[2].valor);

	printf("---------------------------------------------------\n");
	printf("\t Testando transposta. \n");

	cria_linhas(3, &v1);
	cria_linhas(3, &v2);

	printf("Um vetor vai ser \n [ 1 2 3 ] \n [ 4 4 4 ] \n");
	add_valor(0, 0, 1, &v1);
	add_valor(0, 1, 2, &v1);
	add_valor(0, 2, 3, &v1);
	add_valor(1, 0, 4, &v1);
	add_valor(1, 1, 4, &v1);
	add_valor(1, 2, 4, &v1);

	transposta(&v1, &v2);

	printf("Resultado = [ %f %f %f ] \n", v2.i[0].j[0].valor, v2.i[0].j[1].valor, v2.i[0].j[2].valor);
	printf("Resultado = [ %f %f %f ] \n", v2.i[1].j[0].valor, v2.i[1].j[1].valor, v2.i[1].j[2].valor);
	printf("Resultado = [ %f %f %f ] \n", v2.i[2].j[0].valor, v2.i[2].j[1].valor, v2.i[2].j[2].valor);
	
	printf("---------------------------------------------------\n");
	printf("\t Testando vetor vezes vetor. \n");

	cria_linhas(3, &v1);
	cria_linhas(3, &v2);

	printf("Um vetor vai ser [ 1 2 3 ] \n");
	add_valor(0, 0, 1, &v1);
	add_valor(0, 1, 2, &v1);
	add_valor(0, 2, 3, &v1);

	printf("Outro vetor vai ser [ 2 2 2 ] \n");
	add_valor(0, 0, 2, &v2);
	add_valor(0, 1, 2, &v2);
	add_valor(0, 2, 2, &v2);

	printf("Resultado = %f \n", multiplicacao_vetor_vetor(&v1, &v2));

	/////////////////////////////////FIM DOS TESTES//////////////////////////	
	printf("---------------------------------------------------\n");
	printf("\t Testando gradiente conjugado \n");

	cria_linhas(3, &v1);
	cria_linhas(3, &v2);
	cria_linhas(3, &v3);

	add_valor(0, 0, 1, &v1);
	add_valor(0, 1, -1, &v1);
	add_valor(0, 2, 0, &v1);
	add_valor(1, 0, -1, &v1);
	add_valor(1, 1, 2, &v1);
	add_valor(1, 2, 1, &v1);
	add_valor(2, 0, 0, &v1);
	add_valor(2, 1, 1, &v1);
	add_valor(2, 2, 2, &v1);

	print(&v1);
	printf("-------------A \n");

	add_valor(0, 0, 0, &v2);
	add_valor(0, 1, 2, &v2);
	add_valor(0, 2, 3, &v2);

	print(&v2);
	printf("-------------b \n");

	ConjugateGradient(v1.numero_de_linhas, &v1, &v2, &v3);

	print(&v3);
	printf("-------------final \n");

	return 0;
}
*/

void ConjugateGradient(int n, Matriz* A, Matriz* b, Matriz* x)
{

	int k;

	double alfa, beta, alfaNum, alfaDen, betaNum;

	Matriz d;
	Matriz d1;   // proximo vetor d
	Matriz r;
	Matriz r1;   // proximo vetor r
	Matriz x1;   // proximo vetor x
	Matriz w;    // vetor que recebe Ax
	Matriz w1;	  // vetor multiplica Adk
	Matriz temp; // vetor temporario
	Matriz temp2; // vetor temporario2
	int iteracoes = 0;

	cria_linhas(n, &d);  //Criando vetor d
	cria_linhas(n, &d1); //Criando vetor d1
	cria_linhas(n, &r); //Criando vetor r
	cria_linhas(n, &r1); //Criando vetor r1
	cria_linhas(n, &x1); //Criando vetor x1
	cria_linhas(n, &w); //Criando vetor w
	cria_linhas(n, &w1); //Criando vetor w1
	cria_linhas(n, &temp); //Criando vetor temp
	cria_linhas(n, &temp2); //Criando vetor temp


	multiplicacao_matriz_vetor(A, x, &w); //multipica Ax, salva em w

	print(&w);
	printf("-------------w \n");

	subtrai_vetores(b, &w, &temp2); //diminui b-w  --> b - Ax , salva em w

	print(&temp2);
	printf("-------------temp2 \n");
	copia(&temp2, &w);
	printf("-------------w \n");

	copia(&w, &r); // copia r = w 

	print(&r);
	printf("-------------r \n");

	copia(&r, &d); // copia d = r

	print(&d);
	printf("-------------d \n");
	
	for (k = 0; k < n; k++, iteracoes++)
	{

		if (r.i[0].numero_de_colunas == 0)
			break;

		alfaNum = multiplicacao_vetor_vetor(&r, &r); // rT*r ---> numerador de alfa

		printf("%f \n", alfaNum);
		printf("-------------AlfaNum \n");

		transposta(&d, &temp2);
		multiplicacao_matriz_vetor(A, &temp2, &w1); // Ad ---> matriz A * dk, denominador de alfa e salva no vetor w1
		transposta(&w1, &temp2);
		copia(&temp2, &w1);
		filtro(&w1);

		print(&w1);
		printf("------------w1 \n");

		alfaDen = multiplicacao_vetor_vetor(&d, &w1); // dkT * Adk --> multiplicao no denominador de alfa
				
		printf("%f \n", alfaDen);
		printf("-------------alfaDen \n");

		alfa = alfaNum / alfaDen; // calculo do alfa

		printf("%f \n", alfa);
		printf("-------------alfa \n");

		// inicio do calculo de Xk+1

		multiplicacao_escalar_vetor(alfa, &d, &temp); //alfa * d --> multiplica alfa pelo vetor d 

		print(&temp);
		printf("------------temp \n");

		adicao_vetores(x, &temp, &x1); // Xk + alfaDk  ---> soma de vetores

		print(&x1);
		printf("-----------x1 \n");

		multiplicacao_escalar_vetor(alfa, &w1, &temp); // alfa * Adk 

		print(&temp);
		printf("-----------temp \n");

		subtrai_vetores(x, &temp, &r1); // Rk+1 = Rk - alfa*Adk

		print(&r1);
		printf("-----------r1 \n");

		betaNum = multiplicacao_vetor_vetor(&r1, &r1); // Rk+1(transposto) * Rk+1

		printf("%f \n", betaNum);
		printf("-----------betaNum \n");

		beta = betaNum / alfaNum; // denominador de Beta eh o mesmo numerador de alfa

		printf("%f \n", beta);
		printf("-----------beta \n");

		multiplicacao_escalar_vetor(beta, &d, &temp); // beta * dk

		print(&temp);
		printf("-----------temp \n");

		adicao_vetores(&r1, &temp, &d1); // Rk+1 + beta * dk 

		print(&d1);
		printf("-----------d1 \n");

		copia(&r1, &r);
		copia(&x1, x);
		copia(&d1, &d);

	}



}