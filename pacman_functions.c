#include "GLCD.h" 

extern int	current_state_x;
extern int current_state_y;
extern int ghost_ready;
int is_closed = 1;
int move(int x, int y){
	if(grid[current_state_y + y][current_state_x + x] == 'G' && !powerful_pacman){
		DrawFilledSquare(current_state_x * cellSize + cellSize/2, current_state_y * cellSize + cellSize/2 + margin_top, cellSize, Black);
		pacman_eaten();
		return 1;
	}else if(grid[current_state_y + y][current_state_x + x] != 'W' && grid[current_state_y][current_state_x] != 'D' && current_state_x + x >=0 && current_state_x + x < COLS){
		if(x>0 && is_closed)
			DrawPacManRight((current_state_x + x) * cellSize + cellSize/2, (current_state_y + y) * cellSize + cellSize/2 + margin_top, 5, Yellow);
		else if(x<0 && is_closed)
			DrawPacManLeft((current_state_x + x) * cellSize + cellSize/2, (current_state_y + y) * cellSize + cellSize/2 + margin_top, 5, Yellow);
		else DrawFilledCircle((current_state_x + x) * cellSize + cellSize/2, (current_state_y + y) * cellSize + cellSize/2 + margin_top, 5, Yellow);
		DrawFilledSquare(current_state_x * cellSize + cellSize/2, current_state_y * cellSize + cellSize/2 + margin_top, cellSize, Black);
		grid[current_state_y][current_state_x] = 'E';		
		current_state_x += x ;
		current_state_y += y ;
		if(grid[current_state_y][current_state_x] == 'G' && powerful_pacman){
			check_points();	
			ghost_eaten();
		}else check_points();
		grid[current_state_y][current_state_x] = 'M';
		return 1;
	}else if(current_state_x + x == COLS){
		DrawPacManRight(0* cellSize + cellSize/2, current_state_y * cellSize + cellSize/2 + margin_top, 5 , Yellow);
		DrawFilledSquare(current_state_x * cellSize + cellSize/2, current_state_y * cellSize + cellSize/2 + margin_top, cellSize, Black);
		if(grid[current_state_y][current_state_x + x] == 'G'&& powerful_pacman)	
			ghost_eaten();
		current_state_x = 0 ;
		check_points();
		grid[current_state_y][current_state_x] = 'M';
		return 1;
	}else if(current_state_x + x < 0){
		DrawPacManLeft((COLS - 1) * cellSize + cellSize/2, current_state_y * cellSize + cellSize/2 + margin_top, 5 , Yellow);
		DrawFilledSquare(current_state_x * cellSize + cellSize/2, current_state_y * cellSize + cellSize/2 + margin_top, cellSize, Black);
		current_state_x = COLS ;
		check_points();
		grid[current_state_y][current_state_x] = 'M';
		return 1;
}
	return 0;
	}

int move_down(){
return move(0, 1);
}

int move_up(){
return move(0, -1);
}
int move_right(){	
	return move(1, 0);
}     

int move_left(){
	return move(-1, 0);
}

