#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;
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
void getting_input()								//the main function for extracting the input and sorting the nets ascendingly to be handeled 
{
	vector<vector<pin>> net;				//defining the vector that conatins the nets which contain the pins
	string input;

	ifstream ifile("input.txt");			//opening the file"input.txt" to get the input from it
	int nett = 0;
	vector< pair <int, int> > vect;
	vector<string>netnames;
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
			string curr = input.substr(input.find('(')+1, input.find(')') - input.find('(')-1);    //dividing the input to get only the attributes of the pin we are workin on atm
			input = input.substr(input.find(')') + 1);
			//cout << curr << endl;
			int layer = stoi(curr.substr(0, curr.find(',')));			//the layer is the first number of the attributes until we hit a comma
			curr = curr.substr(curr.find(',')+1);						
			int x = stoi(curr.substr(0, curr.find(',')));				//the x coordiante is the second number of the attributes until we hit a comma
			curr = curr.substr(curr.find(',')+1);
			int y = stoi(curr.substr(1));								//the y coordiante is the third number of the attributes until we hit a comma
			
			pin temppin;									// a temporary vector of type pin to arrange the nets according to the size of the wire needed for the pins
			temppin.layer = layer;
			temppin.x = x;
			temppin.y = y;
			minx = x < minx ? x : minx;						//getting the borders of the smallest box that contains all the pins in all the layers
			miny = y < miny ? y : miny;
			minl = layer > minl ?layer : minl;
			maxx = x > maxx ? x : maxx;
			maxy = y > maxy ? y : maxy;
			maxl = layer < maxl ? layer : maxl;
			tempv.push_back(temppin);
			if (i == count - 1)
				net.push_back(tempv);
		}
		int volume = (maxx - minx) * (maxy - miny) * (maxl - minl);		//getting the volume of said box
		vect.push_back(make_pair(nett, volume));
		netnames.push_back(net_name);
		nett++;
		
	}
	sort(vect.begin(), vect.end(), sortbysec);					//sorting each net by the volume of the box that contains the pins
	/*for (int i = 0;i < net.size(); i++)
	{
		cout << netnames[vect[i].first] << endl;
		for (int j = 0; j < net[vect[i].first].size(); j++)
			cout << net[vect[i].first][j].layer << ' ' << net[vect[i].first][j].x << ' ' << net[vect[i].first][j].y << endl;//<< net[vect[i].first][j].net_name<< endl;			//printing the nets and the attributes of each pin ascendingly 
		cout << endl << endl;
	}*/
		
}

int main()
{
	getting_input();
}