//
// Created by solom on 17/05/19.
//

//
// Created by solom on 17/05/19.
//

//
// Created by solom on 16/05/19.
//

//
// Created by solom on 16/05/19.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <cmath>

using namespace std;



const int N = 1e3;

struct node
{
	std::string data;
	node *left;
	node *right;
};

struct circ {
	int data;
	circ* next;
};


struct mos {
	circ* drn;
	int bdy;
	circ* snk;
};


struct nmos_details {
	string type = "";
	string Rg = "3";
	string Vto = "1.4";
	string Rd = "8m";
	string Rs = "6m";
	string Rb = "10m";
	string Kp = "70";
	string Cgdmax = ".5n";
	string Cgdmin = ".12n";
	string Cgs = ".8n";
	string Cjo = ".24n";
	string Is = "24p";
	string ksubthres = ".1";
	string mfg = "Siliconix";
	string Vds = "12";
	string Ron = "20m";
	string Qg = "12n";

};

struct pmos_details {
	string type = "pchan";
	string Rg = "3";
	string Vto = "-1.3";
	string Rd = "16m";
	string Rs = "12m";
	string Rb = "20m";
	string Kp = "30";
	string Cgdmax = ".6n";
	string Cgdmin = ".14n";
	string Cgs = ".9n";
	string Cjo = ".28n";
	string Is = "28p";
	string ksubthres = ".1";
	string mfg = "Siliconix";
	string Vds = "-12";
	string Ron = "40m";
	string Qg = "14n";

};

struct pulse {

	int On_Vdd = 5;
	int Off_Vdd = 0;

	int Period;
	int half_Period;

};

struct voltageSource {

	int TransGate;
	const int gnd = 0;
	pulse PWL;

};


std::vector<mos*> up_netlist, down_netlist;

node* tree;
int Vdd, gnd;
int uni1 = -3;
int nMOS = 0, pMOS = 0;
int inter = 1000;
int count = 0;
int inter1 = 1000;
bool up_down;
int top = -1;
node *arr[35];
std::string group[1000];

std::pair<circ**, circ**> get_ends(node*);
std::pair<circ**, circ**> create_and(std::pair<circ**, circ**>&, std::pair<circ**, circ**>&);
std::pair<circ**, circ**> create_or(std::pair<circ**, circ**>&, std::pair<circ**, circ**>&);
std::pair<circ**, circ**> create_not(std::pair<circ**, circ**>&);
void voltageSources(vector<int> v_in);
void simulation(int max);
void printCMOS(vector<int> v_in, string exp);		

bool isOperator(char c);
int weightOperator(char c);
bool precedece(char a, char b);

bool checkop(std::string);
int order(char);
std::string conv(std::string);
string demorgan(string input);
void push(node*);
node* pop();
void create_expr_tree(std::string[]);

int main() {

	//input voltages
	vector <int> v_in;
	string input;
	
	//memset(tree, -1, sizeof tree);
	for (int i = 0; i < 1000; i++) {
		group[i] = "`";
	}

	cout << "Enter your logical expression!" << endl;
	cout << " for AND use & " <<  " for OR use | " 
		<< "for NOT use \' " << endl;
	
	std::string str;

	cin >> str;
	

	
	//to_lower(str);
	std::string str1 = conv(str);
	std::string str2 = str1;

int gr = 0;

	/*for (int i = 0; i < str2.length(); i++) {
		if (str2[i] == '&' || str2[i] == '|') { group[gr] = str2[i]; gr++; }
		else if (isalpha(str2[i])) {
			if (isalpha(str2[i])) { group[gr] = str2[i]; gr++; }
			else if (str2[i] == '\'') { group[gr].append(1, str2[i]); group[gr].append(1, str2[i + 1]); gr++; }
			else if (str2[i] == '&' || str2[i] == '|') { group[gr] = str2[i]; gr++; }
			else continue;
		}
		else if (str2[i] == '\'') continue;
	}
*/

	for (int i = 0; i < str2.length(); i++) {

		if (isalpha(str2[i]) || str2[i] == '&' || str2[i] == '|') {
			if (str2[i + 1] == '\'') { group[gr] = str2[i]; group[gr] += str2[i + 1];  gr++; }
			else { group[gr] = str2[i]; gr++; }
		}
		else continue;

	}

	
	create_expr_tree(group);

	tree = arr[0];

	Vdd = -1;
	gnd = 0;
	int y = -2;
	up_down = true;
	auto up = get_ends(tree); //returns a pair of pointers that are the start and end wire for the generated pull up network
	up_down = false;
	auto down = get_ends(tree);
	auto temp = *up.first;
	while (temp->next != nullptr) {
		temp = temp->next;
	}
	temp->data = Vdd; //make start = Vdd
	temp = *up.second;
	while (temp->next != nullptr) {
		temp = temp->next;
	}
	temp->data = y;
	temp = *down.first;
	while (temp->next != nullptr) {
		temp = temp->next;
	}
	temp->data = y; //make end = y
	temp = *down.second;
	while (temp->next != nullptr) {
		temp = temp->next;
	}

	cout << "*this is the generated netlist for the expression " << str << endl;

	temp->data = gnd;
	std::cout << "Vdd " << Vdd << ' ' << 0 << " 5v" << std::endl;
	int j = 0;
	for (auto i : up_netlist) {
		std::cout << 'M' << j++ << ' ';
		auto temp = i->snk;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		std::cout << temp->data << ' ' << i->bdy << ' ';
		v_in.push_back(i->bdy);
		temp = i->drn;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		std::cout << temp->data << ' ' << temp->data << ' ' << "mypmos" << std::endl; //printing netlist
	}

	for (auto i : down_netlist) {
		std::cout << 'M' << j++ << ' ';
		auto temp = i->drn;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		std::cout << temp->data << ' ' << i->bdy << ' ';
		//v_in.push_back(i->bdy);
		temp = i->snk;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		std::cout << temp->data << ' ' << temp->data << ' ' << "mynmos" << std::endl; //printing netlist
	}


	printCMOS(v_in, str);

	system("pause");

	return 0;

}


bool checkop(std::string x)
{
	switch (x[0]) {
	case '&':
	case '|':
	case '\'':
		return true;
	}
	return false;
}

//dictating the order of the operators
int order(char c)
{
	if (c == '\'')
		return 3;
	if (c == '&')
		return 2;
	else if (c == '|')
		return 1;
	else
		return -1;
}

// changing from infix to postfix

bool isOperator(char c) {

	if (c == '(' || c == ')' || c == '&' || c == '|' || c == '\'') return true;
	else return false;

}
int weightOperator(char c) {
	int w = 0;
	if (c == '(' || c == ')') w = 4;
	else if (c == '\'') w = 3;
	else if (c == '&') w = 2;
	else if (c == '|') w = 1;
	else w = 0;
	return w;
}
bool precedece(char a, char b) {

	if (weightOperator(a) > weightOperator(b)) return true;
	else return false;

}

string demorgan(string input) {

	string output = "";

	if (input[input.length() - 1] == '\'') {

		for (int i = 0; i < input.length() - 1; i++) {

			if (isalpha(input[i]) && input[i + 1] == '\'') {
				output += input[i];
			}
			else if (isalpha(input[i]) && input[i + 1] != '\'') {
				output += input[i];
				output += '\'';
			}
			else if (input[i] == '&') {
				output += '|';
			}
			else if (input[i] == '|') {
				output += '&';
			}
			else if (input[i + 1] == '\'') continue;

		}

		return output;

	}
	else return input;

}


string conv(string infix) {

	string postfix = "";
	string final;
	stack<char> order;

	for (int i = 0; i < infix.length(); i++) {
		if (!isOperator(infix[i])) {
			if (isalpha(infix[i]) && infix[i + 1] == '\'')
				postfix += infix[i];
			else if (isalpha(infix[i]) && infix[i + 1] != '\'')
			{
				postfix += infix[i];
				postfix += '\'';

			}
			
		}
		else if (infix[i] == '(')
		{
			order.push(infix[i]);
		}

		else if (infix[i] == ')')
		{
			while (!order.empty() && order.top() != '(') {
				postfix += order.top();
				order.pop();
			}
			order.pop();
		}
		else if (infix[i] == '\'') { continue; }
		else {
			while (!order.empty() && order.top() != '(' && precedece(order.top(), infix[i]))
			{
				postfix += order.top();
				order.pop();
			}
			order.push(infix[i]);
		}

	}
	while (!order.empty()) {
		postfix += order.top();
		order.pop();
	}

	//final = demorgan(postfix);

	return postfix;

}

//it is used for inseting an single element in//a tree, i.e. is pushing of single element.
void push(node *tree)
{
	top++;
	arr[top] = tree;
}

node *pop()
{
	top--;
	return (arr[top + 1]);
}

void create_expr_tree(std::string suffix[1000])
{
	std::string cur;
	node *new_parent, *child_1, *child_2;
	int is_var;
	cur = suffix[0];
	for (int i = 1; cur != "`"; i++)
	{
		is_var = checkop(cur);
		if (!is_var)
		{
			new_parent = new node;
			new_parent->data = cur;
			new_parent->left = nullptr;
			new_parent->right = nullptr;
			push(new_parent);
		}
		else
		{
			child_1 = pop();
			child_2 = pop();
			new_parent = new node;
			new_parent->data = cur;
			new_parent->left = child_2;
			new_parent->right = child_1;
			push(new_parent);
		}
		cur = suffix[i];
	}
}


//To better interpret this, suppose the view from the function opinion is just a tree with a parent and 2 children nodes with each node presenting a circuit (except the parent). The children return the their start and end wires to the parent. The parent then combines the 4 wires together down to 2 according to the parent's value. The parent then returns the 2 new wires.
std::pair<circ**, circ**> get_ends(node* ptr) {

	std::string parent = ptr->data;//parent to evaluate

	node* child_1_loc = ptr->left;
	node* child_2_loc = ptr->right; //location of parents children


	std::pair<circ**, circ**> child_1_end, child_2_end; //will be the start and end wires that will be recursively returned

	bool c1 = false;
	if (child_1_loc != nullptr) {
		child_1_end = get_ends(child_1_loc); //start and end wires of child 1
		c1 = true;
		std::string child_1 = child_1_loc->data; //value of child
	}

	bool c2 = false;
	if (child_2_loc != nullptr) {
		child_2_end = get_ends(child_2_loc); //start and end wires of child 2
		c2 = true;
		std::string child_2 = child_2_loc->data; //value of child
	}

	std::pair<circ**, circ**> ret;

	if (c1 && c2) { //enter case if children not null and parent is one of &, | or '. meaning the children have their start and end wires ready
		switch (parent[0]) {
		case '&': if (up_down) {
			ret = create_and(child_1_end, child_2_end);
		}
				  else {
			ret = create_or(child_1_end, child_2_end);
		}
				  break;
		case '|': if (up_down) {
			ret = create_or(child_1_end, child_2_end);
		}
				  else {
			ret = create_and(child_1_end, child_2_end);
		}
				  break;
		default: std::cout << "ERROR";
		}
	}
	else { //this creates the start and end wires that are to be combined
		mos* new_mos = new mos;
		new_mos->bdy = parent[0];
		new_mos->drn = new circ;
		(*new_mos->drn).data = --uni1;
		(*new_mos->drn).next = nullptr;
		new_mos->snk = new circ;
		(*new_mos->snk).data = --uni1;
		(*new_mos->snk).next = nullptr;
		if (up_down) {
			up_netlist.push_back(new_mos); //push to the netlist so we can output it later and values are still changed in the netlist due to pointers
		}
		else {
			down_netlist.push_back(new_mos);
		}
		ret = { &new_mos->drn, &new_mos->snk }; //start and end of a single MOSFET
	}
	if (parent.back() == '\'') {
		ret = create_not(ret);
	}
	return ret;
}


std::pair<circ**, circ**> create_and(std::pair<circ**, circ**> &in1, std::pair<circ**, circ**> &in2) {
	(*in2.first)->next = *in1.second;
	return { in1.first, in2.second };
}

std::pair<circ**, circ**> create_or(std::pair<circ**, circ**> &in1, std::pair<circ**, circ**> &in2) {
	(*in2.first)->next = *in1.first;
	(*in2.second)->next = *in1.second;
	return { in1.first, in2.second };
}

std::pair<circ**, circ**> create_not(std::pair<circ**, circ**> &in) {
	(*in.first)->data = Vdd;
	circ* inter = new circ;
	inter->data = --uni1;
	inter->next = nullptr;

	mos* new_mos = new mos;

	new_mos->bdy = (*in.second)->data;
	new_mos->drn = new circ;
	new_mos->drn->data = Vdd;
	new_mos->drn->next = nullptr;
	new_mos->snk = inter;

	mos* new_mos_2 = new mos;
	new_mos_2->bdy = (*in.second)->data;
	new_mos_2->drn = inter;
	new_mos_2->snk = new circ;
	new_mos_2->snk->data = gnd;
	new_mos_2->snk->next = nullptr;
	(*in.second)->next = nullptr;

	mos* new_mos_3 = new mos;
	new_mos_3->bdy = inter->data;
	new_mos_3->drn = new circ;
	new_mos_3->drn->data = --uni1;
	new_mos_3->drn->next = nullptr;
	new_mos_3->snk = new circ;
	new_mos_3->snk->data = --uni1;
	new_mos_3->snk->next = nullptr;

	up_netlist.push_back(new_mos);
	down_netlist.push_back(new_mos_2);
	if (up_down) {
		up_netlist.push_back(new_mos_3);
	}
	else {
		down_netlist.push_back(new_mos_3);
	}

	return { &new_mos_3->drn, &new_mos_3->snk };
}

void voltageSources(vector <int> v_in ) {

	vector <voltageSource> Vdds;
	vector <char> var;

	Vdds.resize(v_in.size());

	sort(v_in.begin(), v_in.end());
	v_in.erase(unique(v_in.begin(), v_in.end()), v_in.end());
	

	int maximumPeriod = pow(2, v_in.size());



	for (int i = 0; i < v_in.size(); i++) {



		Vdds[i].TransGate = v_in[i];

		Vdds[i].PWL.Period = maximumPeriod;
		Vdds[i].PWL.half_Period = maximumPeriod / 2;

		maximumPeriod = maximumPeriod / 2;

		Vdds[i].PWL.On_Vdd = 5;
		Vdds[i].PWL.Off_Vdd = 0;


	}


	for (int i = 0; i < v_in.size(); i++) {

		cout << 'V' << i << ' '
			<< Vdds[i].TransGate << ' ' << Vdds[i].gnd << ' ' << "pulse("
			<< Vdds[i].PWL.Off_Vdd << ' ' << Vdds[i].PWL.On_Vdd << ' '
			<< '0' << " 1n " << "1n " << Vdds[i].PWL.half_Period << 'm' << ' '
			<< Vdds[i].PWL.Period << 'm' << " )" << endl;
	}


}

void simulation(int max) {

	char unit = 'm';

	cout << ".tran " << max << unit << endl;

	cout << ".backanno" << endl << ".end" << endl;

}

void printCMOS(vector <int> v_in, string str ) {

	pmos_details mypmos;
	nmos_details mynmos;

	
	int maximumPeriod = pow(2, v_in.size());
	
	voltageSources(v_in);

	cout << "*models discription" << endl;

	//nmos
	cout << ".model mynmos VDMOS(" << mynmos.type << " Rg=" << mynmos.Rg << " Vto=" << mynmos.Vto << " Rd=" << mynmos.Rd << endl << "+ Rs=" << mynmos.Rs
		<< " Rb=" << mynmos.Rb << " Kp=" << mynmos.Kp << " Cgdmax=" << mynmos.Cgdmax << " Cgdmin=" << mynmos.Cgdmin << endl << '+' <<
		" Cgs=" << mynmos.Cgs << " Cjo=" << mynmos.Cjo << " Is=" << mynmos.Is << " ksubthres=" << mynmos.ksubthres << endl << '+' <<
		" mfg=" << mynmos.mfg << " Vds=" << mynmos.Vds << " Ron=" << mynmos.Ron << " Qg=" << mynmos.Qg << endl << endl;

	cout << ".model mypmos VDMOS(" << mypmos.type << " Rg=" << mypmos.Rg << " Vto=" << mypmos.Vto << " Rd=" << mypmos.Rd << endl << "+ Rs=" << mypmos.Rs
		<< " Rb=" << mypmos.Rb << " Kp=" << mypmos.Kp << " Cgdmax=" << mypmos.Cgdmax << " Cgdmin=" << mypmos.Cgdmin << endl << '+' <<
		" Cgs=" << mypmos.Cgs << " Cjo=" << mypmos.Cjo << " Is=" << mypmos.Is << " ksubthres=" << mypmos.ksubthres << endl << '+' <<
		" mfg=" << mypmos.mfg << " Vds=" << mypmos.Vds << " Ron=" << mypmos.Ron << " Qg=" << mypmos.Qg << endl << endl;

	simulation(maximumPeriod);

}



