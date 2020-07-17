#include <vector>
using namespace std;

const int NUMBER_LAYERS = 3;
const int N = 100;

struct pin {
	int layer;
	int x;
	int y;
};

void maze_routing(vector<vector<pin>> nets)
{
	int cells[NUMBER_LAYERS][N][N];
	// 0 means clear
	// -1 means start
	// 1000000000 means blocked
	// other numbers are distances

	int number_nets = nets.size;

	// clear the array
	for (int i = 0; i < NUMBER_LAYERS; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				cells[i][j][k] = 0;

	// mark the pins as blocked cells
	for (int i = 0; i < number_nets; i++)
		for (int j = 0; j < nets[i].size; j++)
			cells[nets[i][j].layer][nets[i][j].x][nets[i][j].y] = 1000000000;

	for (int i = 0; i < number_nets; i++)
	{
		vector<pin> targets;
		pin found_target;

		for (int j = 0; j < nets[i].size - 1; j++)
		{
			pin pin_start = nets[i][j + 1];
			pin pin_target = nets[i][j];
			cells[pin_start.layer][pin_start.x][pin_start.y] = -1; // mark current start pin

			pin p = pin_start;

			// add target pin to be a target
			targets.push_back(pin_target);
			for (int i = 0; i < targets.size; i++)
				cells[targets[i].layer][targets[i].x][targets[i].y] = 0; // unblock current target pins

			// breadth first search vector
			vector<pin> bfs_cells;

			// search for a route (apply Lee's algorithm)
			int cur_num = 0;

			// filling
			while (true)
			{
				// wave propagation

				// initialize seach parameters
				int cur_layer = p.layer;
				int cur_x = p.x;
				int cur_y = p.y;
				int added_x = (cur_layer % 2 == 0) ? 1 : 10;
				int added_y = (cur_layer % 2 == 1) ? 1 : 10;

				// through layers
				switch (cur_layer) {
				case 0:
					if (cells[1][cur_x][cur_y] == 0)
					{
						cells[1][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				case NUMBER_LAYERS - 1:
					if (cells[NUMBER_LAYERS - 2][cur_x][cur_y] == 0)
					{
						cells[NUMBER_LAYERS - 2][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = p.layer - 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				default:
					if (cells[cur_layer + 1][cur_x][cur_y] == 0)
					{
						cells[cur_layer + 1][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = p.layer + 1;
						bfs_cells.push_back(p_temp);
					}
					if (cells[cur_layer - 1][cur_x][cur_y] == 0)
					{
						cells[cur_layer - 1][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = p.layer - 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				}

				//through x-axis
				switch (cur_x) {
				case 0:
					if (cells[cur_layer][1][cur_y] == 0)
					{
						cells[cur_layer][1][cur_y] = cur_num + added_x;
						pin p_temp = p;
						p_temp.x = 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				case N - 1:
					if (cells[cur_layer][N - 2][cur_y] == 0)
					{
						cells[cur_layer][N - 2][cur_y] = cur_num + added_x;
						pin p_temp = p;
						p_temp.x = p.x - 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				default:
					if (cells[cur_layer][cur_x + 1][cur_y] == 0)
					{
						cells[cur_layer][cur_x + 1][cur_y] = cur_num + added_x;
						pin p_temp = p;
						p_temp.x = p.x + 1;
						bfs_cells.push_back(p_temp);
					}
					if (cells[cur_layer][cur_x - 1][cur_y] == 0)
					{
						cells[cur_layer][cur_x - 1][cur_y] = cur_num + added_x;
						pin p_temp = p;
						p_temp.x = p.x - 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				}

				//through y-axis
				switch (cur_y) {
				case 0:
					if (cells[cur_layer][cur_x][1] == 0)
					{
						cells[cur_layer][cur_x][1] = cur_num + added_y;
						pin p_temp = p;
						p_temp.y = 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				case N - 1:
					if (cells[cur_layer][cur_x][N - 2] == 0)
					{
						cells[cur_layer][cur_x][N - 2] = cur_num + added_y;
						pin p_temp = p;
						p_temp.y = p.y - 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				default:
					if (cells[cur_layer][cur_x][cur_y + 1] == 0)
					{
						cells[cur_layer][cur_x][cur_y + 1] = cur_num + added_y;
						pin p_temp = p;
						p_temp.y = p.y + 1;
						bfs_cells.push_back(p_temp);
					}
					if (cells[cur_layer][cur_x][cur_y - 1] == 0)
					{
						cells[cur_layer][cur_x][cur_y - 1] = cur_num + added_y;
						pin p_temp = p;
						p_temp.y = p.y - 1;
						bfs_cells.push_back(p_temp);
					}
					break;
				}

				// check if reached target
				// break loop if reached target
				bool reached_target = false;
				for (int i = 0; i < targets.size; i++)
					if (cells[targets[i].layer][targets[i].x][targets[i].y] != 0)
					{
						reached_target = true;
						found_target = targets[i];
						break;
					}
				if (reached_target)
					break;
				
				// next cell
				p = bfs_cells[0];
				bfs_cells.erase(bfs_cells.begin());
				cur_num = cells[p.layer][p.x][p.y];
			}

			// when a solution is found
			// add cells to the targets vector

			// retracing
			int cur_sol_num = cells[found_target.layer][found_target.x][found_target.y];

			while (cur_sol_num != -1)
			{
				int cur_layer = found_target.layer;
				int cur_x = found_target.x;
				int cur_y = found_target.y;
				bool next_cell_found = false;

				// through layers
				switch (cur_layer) {
				case 0:
					if (cells[1][cur_x][cur_y] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				case NUMBER_LAYERS - 1:
					if (cells[NUMBER_LAYERS - 2][cur_x][cur_y] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = found_target.layer - 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				default:
					if (cells[cur_layer + 1][cur_x][cur_y] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = found_target.layer + 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					if (cells[cur_layer - 1][cur_x][cur_y] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = found_target.layer - 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				}

				//through x-axis
				switch (cur_x) {
				case 0:
					if (cells[cur_layer][1][cur_y] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.x = 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				case N - 1:
					if (cells[cur_layer][N - 2][cur_y] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.x = found_target.x - 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				default:
					if (cells[cur_layer][cur_x + 1][cur_y] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.x = found_target.x + 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					if (cells[cur_layer][cur_x - 1][cur_y] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.x = found_target.x - 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				}

				//through y-axis
				switch (cur_y) {
				case 0:
					if (cells[cur_layer][cur_x][1] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.y = 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				case N - 1:
					if (cells[cur_layer][cur_x][N - 2] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.y = found_target.y - 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				default:
					if (cells[cur_layer][cur_x][cur_y + 1] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.y = found_target.y + 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					if (cells[cur_layer][cur_x][cur_y - 1] < cur_sol_num && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.y = found_target.y - 1;
						targets.push_back(p_temp);
						next_cell_found = true;
					}
					break;
				}

				cur_sol_num = cells[targets.back().layer][targets.back().x][targets.back().y];
			}	
		}
		// connect cells in targets and last start value
		// mark as blocked
		for (int i = 0; i < targets.size; i++)
			cells[targets[i].layer][targets[i].x][targets[i].y] = 1000000000; // block target pins since the wire is finalized
		// momo bonus lw adreen
		// while true loop can go infinitely if no routes exist momo
		// the other while loop also can go infinitely momo
	}
}

int main()
{
	vector<vector<pin>> nets;
	maze_routing(nets);
}