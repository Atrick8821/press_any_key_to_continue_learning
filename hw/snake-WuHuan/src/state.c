#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t* new = (game_state_t*)malloc(sizeof(game_state_t));
  new->num_rows = 18;
  new->num_snakes = 1;
  new->snakes = (snake_t*)malloc(sizeof(snake_t));
  new->snakes[0].tail_row = 2;
  new->snakes[0].tail_col = 2;
  new->snakes[0].head_row = 2;
  new->snakes[0].head_col = 4;
  new->snakes[0].live = true;
  new->board = (char**)malloc(sizeof(char*) * 18);
  for(int i = 0; i < 18; i++)
  {
	new->board[i] = (char*)malloc(sizeof(char) * 21);
  }
  for(int i = 0; i < 18; i++)
  {
	if(i ==  0 || i == 17)
	{
		for(int j = 0; j < 20; j++)
		{
			new->board[i][j] = '#';
		}
	}
	else
	{
		for(int j = 0; j < 20; j++)
		{
			new->board[i][j] = (j == 0 || j == 19) ? '#' : ' ';
		}
	}
	new->board[i][20] = '\0';
  }
  new->board[new->snakes[0].tail_row][new->snakes[0].tail_col] = 'd';
  new->board[new->snakes[0].head_row][new->snakes[0].head_col] = 'D';
  new->board[new->snakes[0].tail_row][new->snakes[0].tail_col + 1] = '>';
  new->board[2][9] = '*';

  return new;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
	free(state->snakes);
	for(int i = 0; i < state->num_rows; i++)
	{
		free(state->board[i]);
	}
	free(state->board);
	free(state);
	return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
	for(int i = 0; i < state->num_rows; i++)
	{
		for(int j = 0; j < strlen(state->board[i]); j++)
		{
			fprintf(fp, "%c", state->board[i][j]);
		}
		fprintf(fp, "\n");
	}
	return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
	char list[] = "wasd";
	for(int i = 0; i < strlen(list); i++)
	{
		if (c == list[i])
		{
			return true;
		}
	}
	return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
	char list[] = "WASDx";
	for(int i = 0; i < strlen(list); i++)
	{
		if (c == list[i])
		{
			return true;
		}
	}
	return false;
}


/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  	char list[] = "wasd^<v>WASDx";
	for(int i = 0; i < strlen(list); i++)
	{
		if (c == list[i])
		{
			return true;
		}
	}
	return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  switch(c)
  {
	  case '^':return 'w';
	  case '<':return 'a';
	  case 'v':return 's';
	  case '>':return 'd';
	  default:return '?';
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
	switch(c)
	{
		case 'W':return '^';
		case 'A':return '<';
		case 'S':return 'v';
		case 'D':return '>';
		default:return '?';
	}
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
	switch(c)
	{
		case 'v':case 's':case 'S':
			return cur_row + 1;
		case '^':case 'w':case 'W':
			return cur_row - 1;
		default:return cur_row;
	}
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
	switch(c)
	{
		case '>':case 'd':case 'D':
			return cur_col + 1;
		case '<':case 'a':case 'A':
			return cur_col - 1;
		default:return cur_col;
	}
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
	unsigned int row = state->snakes[snum].head_row;
	unsigned int col = state->snakes[snum].head_col;
	unsigned int next_row = get_next_row(row, state->board[row][col]);
	unsigned int next_col = get_next_col(col, state->board[row][col]);
	return get_board_at(state, next_row, next_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
	unsigned int row = state->snakes[snum].head_row;
	unsigned int col = state->snakes[snum].head_col;
	unsigned int next_row = get_next_row(row, state->board[row][col]);
	unsigned int next_col = get_next_col(col, state->board[row][col]);
	char c = get_board_at(state, row, col);
	set_board_at(state, next_row, next_col, c);
	set_board_at(state, row, col, head_to_body(c));
	state->snakes[snum].head_row = next_row;
	state->snakes[snum].head_col = next_col;

	return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
	unsigned int row = state->snakes[snum].tail_row;
	unsigned int col = state->snakes[snum].tail_col;
	char c =get_board_at(state, row, col);
	unsigned int next_row = get_next_row(row, c);
	unsigned int next_col = get_next_col(col, c);
	set_board_at(state, row, col, ' ');
	c = body_to_tail(get_board_at(state, next_row, next_col));
	set_board_at(state, next_row, next_col, c);
	state->snakes[snum].tail_row = next_row;
	state->snakes[snum].tail_col = next_col;

	return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
	for(unsigned int i = 0; i < state->num_snakes; i++)
	{
		if(state->snakes[i].live == false)
		{
			continue;
		}
		char c = next_square(state, i);
		if (c == '#' || is_snake(c))
		{
			set_board_at(state, state->snakes[i].head_row,
					state->snakes[i].head_col, 'x');
			state->snakes[i].live = false;
			continue;
		}
		else if(c == '*')
		{
			update_head(state, i);
			add_food(state);
		}
		else
		{
			update_head(state, i);
			update_tail(state, i);
		}
	}
	return;
}

/* Task 5 */
game_state_t* load_board(FILE* fp) {
  // TODO: Implement this function.
	if (fp == NULL)
	{
		return NULL;
	}
	game_state_t* state = (game_state_t*)malloc(sizeof(game_state_t));
	unsigned num = 0;
	int c;
	while((c = getc(fp)) != EOF)
	{
		num += (c == '\n') ? 1 : 0;
	}
	state->num_rows = num;
	state->board = (char**)malloc(sizeof(char*) * num);
	rewind(fp);
	for(int i = 0; i < num; i++)
	{
		int num = 0;
		long pos;
		if ((pos = ftell(fp)) == -1l)
		{
			return NULL;
		}
		int c;
		while((c = getc(fp)) != '\n')
		{
			num++;
		}
		fseek(fp, pos, SEEK_SET);
		state->board[i] = (char*)malloc(sizeof(char) * (num + 2));
		fgets(state->board[i], num + 2, fp);
		state->board[i][num] = '\0';

	}
	state->num_snakes = 0;
	state->snakes = NULL;
	return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
	int num = -1;
	for(unsigned int i = 0; i < state->num_rows; i++)
	{
		for(unsigned int j = 0; j < strlen(state->board[0]); j++)
		{
			if (is_tail(get_board_at(state, i, j)))
			{
				num++;
				if (num == snum)
				{
					state->snakes[snum].tail_row = i;
					state->snakes[snum].tail_col = j;
					unsigned int row = i;
					unsigned int col = j;
					char c = get_board_at(state, row, col);
					while (!is_head(c))
					{
						row = get_next_row(row, c);
						col = get_next_col(col, c);
						c = get_board_at(state, row, col);
					}
					state->snakes[snum].head_row = row;
					state->snakes[snum].head_col = col;
					state->snakes[snum].live = true;
					return;
				}
			}
		}
	}
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
	state->num_snakes = 0;
	for(unsigned int i = 0; i < state->num_rows; i++)
	{	
		for(unsigned int j = 0; j < strlen(state->board[i]); j++)
		{
			if (is_head(get_board_at(state, i, j)))
			{
				state->num_snakes++;
			}
		}
	}
	state->snakes = (snake_t*)malloc(sizeof(snake_t) * state->num_snakes);
	for(unsigned int i = 0; i < state->num_snakes; i++)
	{
		find_head(state, i);
	}
  return state;
}
