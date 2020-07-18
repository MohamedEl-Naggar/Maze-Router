#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

const int NUMBER_LAYERS = 3;
const int N = 50;

struct pin {
	int layer;
	int x;
	int y;
};

bool sortbysec(const pair<int, int>& a,
	const pair<int, int>& b)
{
	return (a.second < b.second);
}
vector<vector<pin>> batata()
{
	vector<vector<pin>> net;
	string input;

	ifstream ifile("input.txt");
	int nett = 0;
	vector< pair <int, int> > vect;
	while (getline(ifile, input))
	{
		int minx = INT_MAX, miny = INT_MAX, minl = INT_MAX;
		int maxx = INT_MIN, maxy = INT_MIN, maxl = INT_MIN;
		//cout << input << endl;
		int count = 0;
		input = input.substr(input.find('('));
		for (int i = 0; i < input.length(); i++)
			if (input[i] == '(')
				count++;
		vector<pin> tempv;
		for (int i = 0; i < count; i++)
		{
			string curr = input.substr(input.find('(') + 1, input.find(')') - input.find('(') - 1);
			input = input.substr(input.find(')') + 1);
			//cout << curr << endl;
			int layer = stoi(curr.substr(0, curr.find(',')));
			curr = curr.substr(curr.find(',') + 1);
			int x = stoi(curr.substr(0, curr.find(',')));
			curr = curr.substr(curr.find(',') + 1);
			int y = stoi(curr.substr(1));
			pin temppin;
			temppin.layer = layer;
			temppin.x = x;
			temppin.y = y;
			minx = x < minx ? x : minx;
			miny = y < miny ? y : miny;
			minl = layer > minl ? layer : minl;
			maxx = x > maxx ? x : maxx;
			maxy = y > maxy ? y : maxy;
			maxl = layer < maxl ? layer : maxl;
			tempv.push_back(temppin);
			if (i == count - 1)
				net.push_back(tempv);
		}
		int volume = (maxx - minx) * (maxy - miny) * (maxl - minl);
		vect.push_back(make_pair(nett, volume));
		nett++;

	}
	sort(vect.begin(), vect.end(), sortbysec);
	for (int i = 0; i < net.size(); i++)
	{
		for (int j = 0; j < net[vect[i].first].size(); j++)
			;// cout << net[vect[i].first][j].layer << ' ' << net[vect[i].first][j].x << ' ' << net[vect[i].first][j].y << endl;
		// cout << endl << endl;
	}

	return net;
}

void maze_routing(vector<vector<pin>> nets)
{
	int*** cells;
	cells = new int** [NUMBER_LAYERS];
	for (int i = 0; i < NUMBER_LAYERS; i++)
		cells[i] = new int* [N];
	for (int i = 0; i < NUMBER_LAYERS; i++)
		for (int j = 0; j < N; j++)
			cells[i][j] = new int [N] ;
	// 0 means clear
	// -1 means start
	// 1000000000 means blocked
	// other numbers are distances

	int number_nets = nets.size();

	// clear the array
	for (int i = 0; i < NUMBER_LAYERS; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				cells[i][j][k] = 0;

	// mark the pins as blocked cells
	for (int i = 0; i < number_nets; i++)
		for (int j = 0; j < nets[i].size(); j++)
		{
			nets[i][j].layer--;
			nets[i][j].x--;
			nets[i][j].y--;
			cells[nets[i][j].layer][nets[i][j].x][nets[i][j].y] = 1000000000;
		}

	for (int i = 0; i < number_nets; i++)
	{
		vector<pin>* targets;
		targets = new vector<pin>;

		// add first target pin to be a target
		targets->push_back(nets[i][0]);

		for (int j = 0; j < nets[i].size() - 1; j++)
		{
			pin pin_start = nets[i][j + 1];
			pin pin_target = nets[i][j];

			for (int a = 0; a < NUMBER_LAYERS; a++)
				for (int b = 0; b < N; b++)
					for (int c = 0; c < N; c++)
						if (cells[a][b][c] != 1000000000)
							cells[a][b][c] = 0;

			cells[pin_start.layer][pin_start.x][pin_start.y] = -1; // mark current start pin

			pin p = pin_start;


			for (int k = 0; k < targets->size(); k++)
				cells[targets->at(k).layer][targets->at(k).x][targets->at(k).y] = 0; // unblock current target pins

			// breadth first search vector
			vector<pin>* bfs_cells;
			bfs_cells = new vector<pin>;

			// search for a route (apply Lee's algorithm)
			int cur_num = 0;
			pin found_target;

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
						bfs_cells->push_back(p_temp);
					}
					break;
				case NUMBER_LAYERS - 1:
					if (cells[NUMBER_LAYERS - 2][cur_x][cur_y] == 0)
					{
						cells[NUMBER_LAYERS - 2][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = p.layer - 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				default:
					if (cells[cur_layer + 1][cur_x][cur_y] == 0)
					{
						cells[cur_layer + 1][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = p.layer + 1;
						bfs_cells->push_back(p_temp);
					}
					if (cells[cur_layer - 1][cur_x][cur_y] == 0)
					{
						cells[cur_layer - 1][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = p.layer - 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				}

				//through x-axis
				if (added_x == 1)
					switch (cur_x) {
				case 0:
					if (cells[cur_layer][1][cur_y] == 0)
					{
						cells[cur_layer][1][cur_y] = cur_num + added_x;
						pin p_temp = p;
						p_temp.x = 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				case N - 1:
					if (cells[cur_layer][N - 2][cur_y] == 0)
					{
						cells[cur_layer][N - 2][cur_y] = cur_num + added_x;
						pin p_temp = p;
						p_temp.x = p.x - 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				default:
					if (cells[cur_layer][cur_x + 1][cur_y] == 0)
					{
						cells[cur_layer][cur_x + 1][cur_y] = cur_num + added_x;
						pin p_temp = p;
						p_temp.x = p.x + 1;
						bfs_cells->push_back(p_temp);
					}
					if (cells[cur_layer][cur_x - 1][cur_y] == 0)
					{
						cells[cur_layer][cur_x - 1][cur_y] = cur_num + added_x;
						pin p_temp = p;
						p_temp.x = p.x - 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				}
				else
				//through y-axis
					switch (cur_y) {
				case 0:
					if (cells[cur_layer][cur_x][1] == 0)
					{
						cells[cur_layer][cur_x][1] = cur_num + added_y;
						pin p_temp = p;
						p_temp.y = 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				case N - 1:
					if (cells[cur_layer][cur_x][N - 2] == 0)
					{
						cells[cur_layer][cur_x][N - 2] = cur_num + added_y;
						pin p_temp = p;
						p_temp.y = p.y - 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				default:
					if (cells[cur_layer][cur_x][cur_y + 1] == 0)
					{
						cells[cur_layer][cur_x][cur_y + 1] = cur_num + added_y;
						pin p_temp = p;
						p_temp.y = p.y + 1;
						bfs_cells->push_back(p_temp);
					}
					if (cells[cur_layer][cur_x][cur_y - 1] == 0)
					{
						cells[cur_layer][cur_x][cur_y - 1] = cur_num + added_y;
						pin p_temp = p;
						p_temp.y = p.y - 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				}

				// check if reached target
				// break loop if reached target
				bool reached_target = false;
				for (int k = 0; k < targets->size(); k++)
					if (cells[targets->at(k).layer][targets->at(k).x][targets->at(k).y] != 0)
					{
						reached_target = true;
						found_target = targets->at(k);
						break;
					}
				if (reached_target)
					break;
				
				// next cell
				p = bfs_cells->at(0);
				bfs_cells->erase(bfs_cells->begin());
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
					if (cells[1][cur_x][cur_y] < cur_sol_num && cells[1][cur_x][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				case NUMBER_LAYERS - 1:
					if (cells[NUMBER_LAYERS - 2][cur_x][cur_y] < cur_sol_num && cells[NUMBER_LAYERS - 2][cur_x][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = found_target.layer - 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				default:
					if (cells[cur_layer + 1][cur_x][cur_y] < cur_sol_num && cells[cur_layer + 1][cur_x][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = found_target.layer + 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					if (cells[cur_layer - 1][cur_x][cur_y] < cur_sol_num && cells[cur_layer - 1][cur_x][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = found_target.layer - 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				}

				//through x-axis
				switch (cur_x) {
				case 0:
					if (cells[cur_layer][1][cur_y] < cur_sol_num && cells[cur_layer][1][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.x = 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				case N - 1:
					if (cells[cur_layer][N - 2][cur_y] < cur_sol_num && cells[cur_layer][N - 2][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.x = found_target.x - 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				default:
					if (cells[cur_layer][cur_x + 1][cur_y] < cur_sol_num && cells[cur_layer][cur_x + 1][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.x = found_target.x + 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					if (cells[cur_layer][cur_x - 1][cur_y] < cur_sol_num && cells[cur_layer][cur_x - 1][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.x = found_target.x - 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				}

				//through y-axis
				switch (cur_y) {
				case 0:
					if (cells[cur_layer][cur_x][1] < cur_sol_num && cells[cur_layer][cur_x][1] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.y = 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				case N - 1:
					if (cells[cur_layer][cur_x][N - 2] < cur_sol_num && cells[cur_layer][cur_x][N - 2] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.y = found_target.y - 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				default:
					if (cells[cur_layer][cur_x][cur_y + 1] < cur_sol_num && cells[cur_layer][cur_x][cur_y + 1] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.y = found_target.y + 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					if (cells[cur_layer][cur_x][cur_y - 1] < cur_sol_num && cells[cur_layer][cur_x][cur_y - 1] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.y = found_target.y - 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				}

				cur_sol_num = cells[targets->back().layer][targets->back().x][targets->back().y];
				found_target = targets->back();
			}	
		}
		// connect cells in targets and last start value
		// mark as blocked
		for (int k = 0; k < targets->size(); k++)
			cells[targets->at(k).layer][targets->at(k).x][targets->at(k).y] = 1000000000; // block target pins since the wire is finalized

		// momo bonus lw adreen
		// while true loop can go infinitely if no routes exist momo
		// the other while loop also can go infinitely momo
	}

	for (int i = 0; i < NUMBER_LAYERS; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				if (cells[i][j][k] == 1000000000)
					cout << i << " " << j << " " << k << endl;

}

int main()
{
	maze_routing(batata());
}
