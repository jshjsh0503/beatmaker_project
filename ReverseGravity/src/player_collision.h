int get_tile_at(int tx, int ty);
void check_spike_collision(void);
int is_wall_tile(int t);
int check_wall_collision(void);
void resolve_horizontal_collision(void);
void resolve_vertical_tile_collision(void);
void check_goal_reach(void);