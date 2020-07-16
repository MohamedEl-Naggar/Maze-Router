#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
struct pin 
{
	int x;
	int y;
	int layer;
};
void batata()
{
	vector<vector<pin>> net;
	string input;

	ifstream ifile("input.txt");
	int nett = 0;
	while (getline(ifile, input))
	{
		cout << input << endl;
		int count = 0;
		input = input.substr(input.find('('));
		for (int i = 0; i < input.length(); i++)
			if (input[i] == '(')
				count++;
		vector<pin> tempv;
		for (int i = 0; i < count; i++)
		{
			string curr = input.substr(input.find('(')+1, input.find(')') - input.find('(')-1);
			input = input.substr(input.find(')') + 1);
			//cout << curr << endl;
			int layer = stoi(curr.substr(0, curr.find(',')));
			curr = curr.substr(curr.find(',')+1);
			int x = stoi(curr.substr(0, curr.find(',')));
			curr = curr.substr(curr.find(',')+1);
			int y = stoi(curr.substr(1));
			pin temppin;
			temppin.layer = layer;
			temppin.x = x;
			temppin.y = y;
			tempv.push_back(temppin);
			if (i == count - 1)
				net.push_back(tempv);
		}

		nett++;
	}
	for (int i = 0; i < net.size(); i++)
	{
		for (int j = 0; j < net[i].size(); j++)
			cout << net[i][j].layer << ' ' << net[i][j].x << ' ' << net[i][j].y << endl;
		cout << endl << endl;
	}
		
}

int main()
{
	batata();
	system("pause");
}