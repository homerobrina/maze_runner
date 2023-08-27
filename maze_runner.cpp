#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <vector>

#include <chrono>
#include <thread>

#include <iostream>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows = 0;
int num_cols = 0;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

pos_t initial_pos, next_position, valid_pos;

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	
	// Abre o arquivo para leitura (fopen)
	FILE * maze_file;
	maze_file = fopen(file_name, "rt");

	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(maze_file, "%d", &num_rows);
	fscanf(maze_file, "%d", &num_cols);
	fgetc(maze_file);

	int i, j;

	// Aloca a matriz maze (malloc)
	// aloca um vetor de LIN ponteiros para linhas
	maze = (char**) malloc (num_rows * sizeof (char*));

	// aloca cada uma das linhas (vetores de COL inteiros)
	for (int i=0; i < num_rows; i++){
		maze[i] = (char*) malloc (num_cols * sizeof (char));
	}

	// percorre a matriz
	for (i=0; i < num_rows; i++){
		for (j=0; j < num_cols; j++){
			maze[i][j] = 0 ;        // acesso com sintaxe mais simples
		}
	}

	
	for (i = 0; i < num_rows; ++i) {
		for (j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			fscanf(maze_file, "%c", &maze[i][j]);
			// Se o valor for 'e' salvar o valor em initial_pos
			if(maze[i][j] == 'e'){
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
		fgetc(maze_file);
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {

	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
		// Marcar a posição atual com o símbolo '.'
		// Limpa a tela
		// Imprime o labirinto
	printf("posição: %d %d\n", pos.i, pos.j);
	maze[pos.i][pos.j] = '.';
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	system("clear");
	print_maze();
		
		/* Dado a posição atual, verifica quais sao as próximas posições válidas
			Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		 	e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		 	cada uma delas no vetor valid_positions
		 		- pos.i, pos.j+1
		 		- pos.i, pos.j-1
		 		- pos.i+1, pos.j
		 		- pos.i-1, pos.j
		 	Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	 	*/
	
	if(maze[pos.i][pos.j+1] == 's' || maze[pos.i][pos.j-1] == 's' || maze[pos.i+1][pos.j] == 's' || maze[pos.i-1][pos.j] == 's'){
		return true;
	}

	if(pos.j+1 < num_cols && maze[pos.i][pos.j+1] == 'x') {
		valid_pos.i = pos.i;
		valid_pos.j = pos.j+1;
		valid_positions.push(valid_pos);
	}
	if(0 <= pos.j-1 && maze[pos.i][pos.j-1] == 'x') {
		valid_pos.i = pos.i;
		valid_pos.j = pos.j-1;
		valid_positions.push(valid_pos);
	}
	if(pos.i+1 < num_rows && maze[pos.i+1][pos.j] == 'x') {
		valid_pos.i = pos.i+1;
		valid_pos.j = pos.j;
		valid_positions.push(valid_pos);
	}
	if(0 <= pos.i-1 && maze[pos.i-1][pos.j] == 'x') {
		valid_pos.i = pos.i-1;
		valid_pos.j = pos.j;
		valid_positions.push(valid_pos);
	}
	// Verifica se a pilha de posições nao esta vazia 
	// Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
	// Caso contrario, retornar falso
	if (!valid_positions.empty()) {
		next_position = valid_positions.top();
		valid_positions.pop();
		walk(next_position);
	}
 	return false;
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);
	print_maze();
	
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	
	printf("exit_found = %d\n", exit_found);

	// Tratar o retorno (imprimir mensagem)
	if(exit_found == true){
		printf("Saida encontrada!\n");
	} else
		printf("Saida não encontrada.\n");
	return 0;
}
