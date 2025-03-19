#include "GLCD.h" 
#include <stdbool.h>
#define INITIAL_CAPACITY 100
#define INITIAL_GHOST_X 11
#define INITIAL_GHOST_Y 10
extern int ghost_current_state_x;
extern int ghost_current_state_y;
extern int current_state_x;       // Pac-Man's x position
extern int current_state_y;       // Pac-Man's y position
extern int cellSize;
extern int margin_top;
int ghost_ready = 1;
uint16_t ghost_color = Red;
int powerful_pacman = 0;
char previousCell = 'U';
int power_pills_time = -1;
int moves[200][2];
int step = 0;
void Ghost_respawn(){
	ghost_current_state_x = INITIAL_GHOST_X;
	ghost_current_state_y = INITIAL_GHOST_Y;
	DrawGhost(ghost_current_state_x * cellSize + cellSize/2, ghost_current_state_y * cellSize + cellSize/2 + margin_top, cellSize, ghost_color);
	ghost_ready = 1;
}
void ghost_eaten(){
	ghost_ready = 0;
	previousCell = 'U';
}
void DrawPreviousCell(){
	DrawFilledSquare(ghost_current_state_x * cellSize + cellSize/2, ghost_current_state_y * cellSize + cellSize/2 + margin_top, cellSize, Black);
	if(previousCell == 'p'){
		DrawPill(ghost_current_state_x, ghost_current_state_y, cellSize/2, 1);
		grid[ghost_current_state_y][ghost_current_state_x] = 'p';
	}
	else if(previousCell == 'P'){
		DrawPill(ghost_current_state_x, ghost_current_state_y, cellSize/2, 2);
		grid[ghost_current_state_y][ghost_current_state_x] = 'P';
	}
	else if(previousCell == 'D'){
		DrawDoor(ghost_current_state_x * cellSize + cellSize/2, ghost_current_state_y * cellSize + cellSize/2 + margin_top, cellSize, Grey);
				grid[ghost_current_state_y][ghost_current_state_x] = 'D';
	}else grid[ghost_current_state_y][ghost_current_state_x] = 'E';
}

void frightened_mode_ghost(){
	ghost_color = Blue2;
	DrawGhost(ghost_current_state_x * cellSize + cellSize/2, ghost_current_state_y * cellSize + cellSize/2 + margin_top, cellSize, ghost_color);
	powerful_pacman = 1;
	power_pills_time = 10;
}

void end_frightened_mode_ghost(){
	ghost_color = Red;
	DrawGhost(ghost_current_state_x * cellSize + cellSize/2, ghost_current_state_y * cellSize + cellSize/2 + margin_top, cellSize, ghost_color);
	powerful_pacman = 0;
}

void ghost_next_move(int ghost_next_state_x, int ghost_next_state_y){
		if(ghost_ready == 0) return;
		DrawGhost(ghost_next_state_x* cellSize + cellSize/2, ghost_next_state_y * cellSize + cellSize/2 + margin_top, 5 , ghost_color);
		DrawPreviousCell();
		ghost_current_state_x = ghost_next_state_x;
		ghost_current_state_y = ghost_next_state_y;
		previousCell = grid[ghost_current_state_y][ghost_current_state_x];
		if(grid[ghost_current_state_y][ghost_current_state_x] == 'M' && !powerful_pacman){
			pacman_eaten();
		}
		else if(grid[ghost_current_state_y][ghost_current_state_x] == 'M' && powerful_pacman){
			ghost_ready = 0;
			previousCell = 'U';			
		}else	grid[ghost_current_state_y][ghost_current_state_x] = 'G';
}

void chase_mode_ghost(){
	ghost_next_move(moves[step][0], moves[step][1]);
	step++;
}


void run_away() {
    int i;
    float h[4] = {0};          // Store heuristics for each neighbor
    int best_move = -1;        // Index of the best move
    int nx, ny;                // Coordinates of the neighboring cell
    static int previous_x = -1, previous_y = -1; // Track the previous position
		int neighbors[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // Possible moves
		if(grid[ghost_current_state_y - 1][ghost_current_state_x] == 'D' || previousCell =='D')
			ghost_next_move(ghost_current_state_x, ghost_current_state_y - 1);
		else for (i = 0; i < 4; i++) {
        nx = ghost_current_state_x + neighbors[i][0];
        ny = ghost_current_state_y + neighbors[i][1];
        // Wrap around horizontally if ghost moves beyond grid bounds
        if (nx == -1) {
            nx = COLS - 1; // Wrap to the far right
        } else if (nx == COLS) {
            nx = 0;        // Wrap to the far left
        }

        // Check boundaries, walls, and previous position
        if (nx < 0 || ny < 0 || nx >= COLS || ny >= ROWS || grid[ny][nx] == 'W' || (nx == previous_x && ny == previous_y) || grid[ny][nx] == 'U' ||grid[ny][nx] == 'D') {
            continue; // Skip invalid moves
        }

        // Calculate heuristic: distance to Pac-Man
        h[i] = abs(nx - current_state_x) + abs(ny - current_state_y);

        // Choose the move with the highest heuristic (farthest from Pac-Man)
        if (best_move == -1 || h[i] > h[best_move]) {
            best_move = i;
        }
    }

    // If no valid moves, do nothing
    if (best_move == -1) {
        return;
    }

    // Update ghost position and store the previous position
    previous_x = ghost_current_state_x;
    previous_y = ghost_current_state_y;

    // Move the ghost to the selected position
    nx = ghost_current_state_x + neighbors[best_move][0];
    ny = ghost_current_state_y + neighbors[best_move][1];
    ghost_next_move(nx, ny);
}



// Create a new heap
Heap* create_heap(int capacity) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    if (!heap) return NULL;

    heap->nodes = (Node**)malloc(sizeof(Node*) * capacity);
    if (!heap->nodes) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Push a node onto the heap
void heap_push(Heap* heap, Node* node) {
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = (Node**)realloc(heap->nodes, sizeof(Node*) * heap->capacity);
    }

    int idx = heap->size++;
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->nodes[parent]->f <= node->f) break;
        heap->nodes[idx] = heap->nodes[parent];
        idx = parent;
    }
    heap->nodes[idx] = node;
}

// Pop the smallest node from the heap
Node* heap_pop(Heap* heap) {
    if (heap->size == 0) return NULL;
    Node* min = heap->nodes[0];
    Node* last = heap->nodes[--heap->size];

    int idx = 0;
    while (idx * 2 + 1 < heap->size) {
        int left = idx * 2 + 1;
        int right = left + 1;
        int smallest = left;

        if (right < heap->size && heap->nodes[right]->f < heap->nodes[left]->f)
            smallest = right;

        if (last->f <= heap->nodes[smallest]->f) break;
        heap->nodes[idx] = heap->nodes[smallest];
        idx = smallest;
    }

    heap->nodes[idx] = last;
    return min;
}

// Heuristic function (Manhattan distance)
float heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void reconstruct_path(Node* end_node) {
    Node* current;
    int i = 0;

    while (end_node != NULL) {
        current = end_node;
        end_node = end_node->parent;

        // Check if end_node is NULL before accessing its members
        if (end_node != NULL) {
            moves[i][0] = end_node->x; // Store x-coordinate
            moves[i][1] = end_node->y; // Store y-coordinate
        }

        i++;
        free(current); // Free the current node
    }

    step = 0; // Reset step
}

void start_a_star_algorithm(){
a_star(current_state_x, current_state_y, ghost_current_state_x, ghost_current_state_y);
}

// A* Algorithm
void a_star(int start_x, int start_y, int goal_x, int goal_y) {
	int i;
    Heap* open_list = create_heap(INITIAL_CAPACITY);
    if (!open_list) return;
    Node* start_node = (Node*)malloc(sizeof(Node));
    if (!start_node) {
        free(open_list->nodes);
        free(open_list);
        return;
    }

    start_node->x = start_x;
    start_node->y = start_y;
    start_node->g = 0;
    start_node->h = heuristic(start_x, start_y, goal_x, goal_y);
    start_node->f = start_node->g + start_node->h;
    start_node->parent = NULL;

    heap_push(open_list, start_node);
    bool closed_list[ROWS][COLS] = {false};

    while (open_list->size > 0) {
        Node* current = heap_pop(open_list);

        // Goal reached
        if (current->x == goal_x && current->y == goal_y) {
            reconstruct_path(current);
            break;
        }

        closed_list[current->y][current->x] = true;

        // Explore neighbors
        int neighbors[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (i = 0; i < 4; i++) {
            int nx = current->x + neighbors[i][0];
            int ny = current->y + neighbors[i][1];
            // Check boundaries and obstacles
						if(nx == - 1 && grid[ny][nx + 1] != 'W')
								nx = COLS -1;
						else if(nx == COLS && grid[ny][nx - 1] != 'W')
								nx = 0;
            if (nx < 0 || ny < 0 || nx >= COLS || ny >= ROWS || grid[ny][nx] == 'W' || closed_list[ny][nx])
                continue;

            float g = current->g + 1;
            float h = heuristic(nx, ny, goal_x, goal_y);

            Node* neighbor = (Node*)malloc(sizeof(Node));
            if (!neighbor) continue;

            neighbor->x = nx;
            neighbor->y = ny;
            neighbor->g = g;
            neighbor->h = h;
            neighbor->f = g + h;
            neighbor->parent = current;

            heap_push(open_list, neighbor);
        }
    }

    // Cleanup
    for (i = 0; i < open_list->size; i++) {
        free(open_list->nodes[i]);
    }
    free(open_list->nodes);
    free(open_list);
}