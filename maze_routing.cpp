#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

const int NUMBER_LAYERS = 3;
const int N = 50;

struct pin									//a struct to define the pin and its attributes 
{
	int x;									//the x coordiante of the pin
	int y;									//the y coordinate of the pin
	int layer;								//the laye in which the pin resides 
	string net_name;						//the name of the net that the pin belongs to
};

bool sortbysec(const pair<int, int>& a,		// a function used for sorting later on
	const pair<int, int>& b)
{
	return (a.second < b.second);
}
vector<vector<pin>> getting_input()								//the main function for extracting the input and sorting the nets ascendingly to be handeled 
{
	vector<vector<pin>> net;				//defining the vector that conatins the nets which contain the pins
	string input;

	ifstream ifile("input.txt");			//opening the file"input.txt" to get the input from it
	int nett = 0;
	vector< pair <int, int> > vect;
	while (getline(ifile, input))			//handeling each line from the file seperatly to get the nets, pins, and it attributes 
	{
		int minx = INT_MAX, miny = INT_MAX, minl = INT_MAX;
		int maxx = INT_MIN, maxy = INT_MIN, maxl = INT_MIN;
		//cout << input << endl;
		//string net_name = input.substr(input.find('n'), input.find('(') - 1);
		//cout << net_name << endl;
		int count = 0;
		string net_name = input.substr(input.find('n'), input.find('(') - 1);
		input = input.substr(input.find('('));		//dividing the input to handel it with the start of '('
		for (int i = 0; i < input.length(); i++)		//this loop is used to determine the number of pins in each net and saving it in count
			if (input[i] == '(')
				count++;
		vector<pin> tempv;

		for (int i = 0; i < count; i++)				// a lopp to handle each pin
		{
			string curr = input.substr(input.find('(') + 1, input.find(')') - input.find('(') - 1);    //dividing the input to get only the attributes of the pin we are workin on atm
			input = input.substr(input.find(')') + 1);
			//cout << curr << endl;
			int layer = stoi(curr.substr(0, curr.find(',')));			//the layer is the first number of the attributes until we hit a comma
			curr = curr.substr(curr.find(',') + 1);
			int x = stoi(curr.substr(0, curr.find(',')));				//the x coordiante is the second number of the attributes until we hit a comma
			curr = curr.substr(curr.find(',') + 1);
			int y = stoi(curr.substr(1));								//the y coordiante is the third number of the attributes until we hit a comma

			pin temppin;									// a temporary vector of type pin to arrange the nets according to the size of the wire needed for the pins
			temppin.layer = layer;
			temppin.x = x;
			temppin.y = y;
			temppin.net_name = net_name;
			minx = x < minx ? x : minx;						//getting the borders of the smallest box that contains all the pins in all the layers
			miny = y < miny ? y : miny;
			minl = layer > minl ? layer : minl;
			maxx = x > maxx ? x : maxx;
			maxy = y > maxy ? y : maxy;
			maxl = layer < maxl ? layer : maxl;
			tempv.push_back(temppin);
			if (i == count - 1)
				net.push_back(tempv);
		}
		int volume = (maxx - minx) * (maxy - miny) * (maxl - minl);		//getting the volume of said box
		vect.push_back(make_pair(nett, volume));
		nett++;

	}
	sort(vect.begin(), vect.end(), sortbysec);					//sorting each net by the volume of the box that contains the pins
	//for (int i = 0;i < net.size(); i++)
	//{
	//	for (int j = 0; j < net[vect[i].first].size(); j++)
	//		cout << net[vect[i].first][j].layer << ' ' << net[vect[i].first][j].x << ' ' << net[vect[i].first][j].y << endl << net[vect[i].first][j].net_name<< endl;			//printing the nets and the attributes of each pin ascendingly
	//	cout << endl << endl;
	//}
	return net;
}

void maze_routing(vector<vector<pin>> nets)
{
	vector<vector<pin>> output_nets; // array holding the final nets with all the connected pins
	output_nets.resize(nets.size());

	int*** cells; // 3d array to model the layers and pins
	cells = new int** [NUMBER_LAYERS];
	for (int i = 0; i < NUMBER_LAYERS; i++)
		cells[i] = new int* [N];
	for (int i = 0; i < NUMBER_LAYERS; i++)
		for (int j = 0; j < N; j++)
			cells[i][j] = new int[N];
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

			// clear the cells with unused distances
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

				// filling through layers
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
					if (cells[cur_layer - 1][cur_x][cur_y] == 0)
					{
						cells[cur_layer - 1][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = p.layer - 1;
						bfs_cells->push_back(p_temp);
					}
					if (cells[cur_layer + 1][cur_x][cur_y] == 0)
					{
						cells[cur_layer + 1][cur_x][cur_y] = cur_num + 10;
						pin p_temp = p;
						p_temp.layer = p.layer + 1;
						bfs_cells->push_back(p_temp);
					}
					break;
				}

				if (added_x == 1)
					// filling through x-axis
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
					// filling through y-axis
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

			int cur_sol_num = cells[found_target.layer][found_target.x][found_target.y];

			while (cur_sol_num != -1)
			{

				int cur_layer = found_target.layer;
				int cur_x = found_target.x;
				int cur_y = found_target.y;
				bool next_cell_found = false;

				// retracing
				// add retraced cells to the array of targets

				// retracing through layers
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
					if (cells[cur_layer - 1][cur_x][cur_y] < cur_sol_num && cells[cur_layer - 1][cur_x][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = found_target.layer - 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					if (cells[cur_layer + 1][cur_x][cur_y] < cur_sol_num && cells[cur_layer + 1][cur_x][cur_y] != 0 && !next_cell_found)
					{
						pin p_temp = found_target;
						p_temp.layer = found_target.layer + 1;
						targets->push_back(p_temp);
						next_cell_found = true;
					}
					break;
				}

				if (cur_layer % 2 == 0)
					// retracing through x-axis
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
				else
					// retracing through y-axis
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
		// connect cells in targets
		// mark target cells as blocked since the wire is finalized
		for (int k = 0; k < targets->size(); k++)
			cells[targets->at(k).layer][targets->at(k).x][targets->at(k).y] = 1000000000; // blocking

		for (int k = 0; k < targets->size(); k++)
			output_nets[i].push_back(targets->at(k));
		cout << output_nets[i][0].net_name << endl;

		// momo bonus lw adreen
		// while true loop can go infinitely if no routes exist momo
		// the other while loop also can go infinitely momo
	}

	for (int i = 0; i < NUMBER_LAYERS; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				if (cells[i][j][k] == 1000000000)
					cout << i << " " << j << " " << k << " " << endl;

	ofstream ofile("output.txt");
	for (int i = 0; i < output_nets.size(); i++)
	{
		ofile << output_nets[i][0].net_name;
		for (int j = 0; j < output_nets[i].size(); j++)
			ofile << " (" << output_nets[i][j].layer + 1 << ", " << output_nets[i][j].x + 1 << ", " << output_nets[i][j].y + 1 << ")";
		ofile << endl;
	}
}

int main()
{
	maze_routing(getting_input());
}
