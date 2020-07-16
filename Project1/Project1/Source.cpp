#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;
struct pin 
{
	int x;
	int y;
	int layer;
};
bool sortbysec(const pair<int, int>& a,
	const pair<int, int>& b)
{
	return (a.second < b.second);
}
void batata()
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
			minx = x < minx ? x : minx;
			miny = y < miny ? y : miny;
			minl = layer > minl ?layer : minl;
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
	for (int i = 0;i < net.size(); i++)
	{
		for (int j = 0; j < net[vect[i].first].size(); j++)
			cout << net[vect[i].first][j].layer << ' ' << net[vect[i].first][j].x << ' ' << net[vect[i].first][j].y << endl;
		cout << endl << endl;
	}
		
}

int main()
{
	batata();
	system("pause");
}