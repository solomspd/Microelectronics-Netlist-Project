#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <time.h>
#include <iomanip>


using namespace std;

struct pmos {
	
	int drain;
	char gate;
	int source;

};

void pullup_circut (string input) {

	vector<pmos> c;
	vector <char> op;

	pmos var = {  0, ' ', 0};
	int s, d; 
	srand(time(NULL));

	for (int i = 0; i < input.length(); i++) {
		s = input[i] + 36 ;
		d = input[i] - 36;
		
		if (isalpha(input[i])) {
			var = { d, input[i], s};
			c.push_back(var);
		}
		else {
			op.push_back(input[i]);
		}
	}
	//before
	//for (int i = 0; i < c.size(); i++) {
	//	cout << 'M' << i << ' ' << c[i].drain << ' ' << c[i].gate << ' ' << c[i].source << ' ' << c[i].source << endl;
	//}


	for (int i = 0; i < op.size(); i++) {
		if (op[i] == '&') {
			c[i + 1].source = c[i].drain ;
		}
		else if (op[i] == '|') {
			c[i+1].source = c[i].source;
			c[i+1].drain = c[i ].drain;
		}
	}

	cout << "name " << "drain " << "gate " << "source " << "body " << endl << endl;

	cout << "Vdd " << c[0].source << endl;

	for (int i = 0; i < c.size(); i++) {
		cout << 'M' << i << ' ' << c[i].drain << ' ' << c[i].gate << ' ' << c[i].source << ' ' << c[i].source << endl;
	}

	cout <<  "OUTPUT " << c[c.size() - 1].drain << endl;
}


int main() {

	string s;

	cin >> s;

	pullup_circut(s);



	system("pause");
	return 0;
}