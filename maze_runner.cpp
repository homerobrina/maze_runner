#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <vector>

#include <chrono>
#include <thread>

#include <iostream>

// Matriz de char representnado o labirinto
char** maze;

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
	maze = (char**) malloc (num_rows * sizeof (char*));

	for (int i=0; i < num_rows; i++){
		maze[i] = (char*) malloc (num_cols * sizeof (char));
	}

	// percorre a matriz
	for (i=0; i < num_rows; i++){
		for (j=0; j < num_cols; j++){
			maze[i][j] = 0 ;
		}
	}

	
	for (i = 0; i < num_rows; ++i) {
		for (j = 0; j < num_cols; ++j) {
			fscanf(maze_file, "%c", &maze[i][j]);
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

	printf("posição: %d %d\n", pos.i, pos.j);
	maze[pos.i][pos.j] = 'O';
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	system("clear");
	print_maze();
	maze[pos.i][pos.j] = '.';
	
	// if((pos.j+1 < num_cols && maze[pos.i][pos.j+1] == 's') ||
	//    (0 <= pos.j-1 && maze[pos.i][pos.j-1] == 's') ||
	//    (pos.i+1 < num_rows && maze[pos.i+1][pos.j] == 's') ||
	//    (0 <= pos.i-1 && maze[pos.i-1][pos.j] == 's')){
	// 	return true;
	// }

	if(pos.j+1 < num_cols && maze[pos.i][pos.j+1] == 's'){
		maze[pos.i][pos.j+1] = 'O';
		system("clear");
		print_maze();
		return true;
	} else if(pos.j+1 < num_cols && maze[pos.i][pos.j+1] == 'x') {
		valid_pos.i = pos.i;
		valid_pos.j = pos.j+1;
		valid_positions.push(valid_pos);
	}

	if(0 <= pos.j-1 && maze[pos.i][pos.j-1] == 's'){
		maze[pos.i][pos.j-1] = 'O';
		system("clear");
		print_maze();
		return true;
	} else if(0 <= pos.j-1 && maze[pos.i][pos.j-1] == 'x') {
		valid_pos.i = pos.i;
		valid_pos.j = pos.j-1;
		valid_positions.push(valid_pos);
	}
	
	if(pos.i+1 < num_rows && maze[pos.i+1][pos.j] == 's'){
		maze[pos.i+1][pos.j] = 'O';
		system("clear");
		print_maze();
		return true;
	} else if(pos.i+1 < num_rows && maze[pos.i+1][pos.j] == 'x') {
		valid_pos.i = pos.i+1;
		valid_pos.j = pos.j;
		valid_positions.push(valid_pos);
	}
	
	if(0 <= pos.i-1 && maze[pos.i-1][pos.j] == 's'){
		maze[pos.i-1][pos.j] = 'O';
		system("clear");
		print_maze();
		return true;
	} else if(0 <= pos.i-1 && maze[pos.i-1][pos.j] == 'x') {
		valid_pos.i = pos.i-1;
		valid_pos.j = pos.j;
		valid_positions.push(valid_pos);
	}

	while (!valid_positions.empty()) {
		next_position = valid_positions.top();
		valid_positions.pop();
		if(walk(next_position)){
			return true;
		}
	}
	return false;
}

int main(int argc, char* argv[]) {

	pos_t initial_pos = load_maze(argv[1]);
	print_maze();
	
	bool exit_found = walk(initial_pos);
	
	if(exit_found){
		printf("Saida encontrada!\n");
	} else
		printf("Saida não encontrada.\n");
	return 0;
}
