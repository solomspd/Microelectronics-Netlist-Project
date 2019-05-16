#include<iostream>
#include<string>
#include<stack>
using namespace std;

bool checkop(string x)
{
	switch (x[0]) {
	case '&':
	case '|':
	case '\'':
		return true;
	}
	return false;
}
//string checkstring(string str) {
//	int x=0;
//	for (int i = x; i < str.length(); i++) {
//		if (isalpha(str[i]) || checkop(str[i])) {
//			if (str[i + 1] != '\'') { str.insert(i + 1, "\'"); x = i + 2; }
//			else {
//				str.erase(i + 1, 1); x = i + 1;
//			}
//		}
//	}
//	return str;
//}
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
string conv(string str)
{
	stack<char> in;
	in.push(':');
	string eq;
	for (int i = 0; i < str.length(); i++)
	{
		//if complemented input left as it is
		if ((str[i] >= 'a' && str[i] <= 'z' && str[i + 1] == '\''))
			eq += str[i];
		// if not it was complemented
		else if ((str[i] >= 'a' && str[i] <= 'z' && str[i + 1] != '\''))
			eq = eq + str[i] + '\'';


		else if (str[i] == '(')
			in.push('(');

		else if (str[i] == ')')
		{
			//adding all characters to a temporary string "en"
			while (in.top() != ':' && in.top() != '(')
			{
				char c = in.top();
				in.pop();
				eq += c;
			}
			if (in.top() == '(')
			{
				char c = in.top();
				in.pop();
			}
		}
		//making sure the required order of precedence is followed
		else {
			while (in.top() != ':' && order(str[i]) <= order(in.top()))
			{
				char c = in.top();
				in.pop();
				eq += c;
			}
			if (isalpha(str[i]) || str[i] == '&' || str[i] == '|')
				in.push(str[i]);
		}

	}
	while (in.top() != ':')
	{
		char c = in.top();
		in.pop();
		eq += c;
	}
	// returning string
	return eq;


}
// changing from post to prefix
string prefinal(string pref)
{
	stack<string> spre;
	int length = pref.size();
	for (int i = 0; i < length; i++) {
		//if operator and or or are encountered
		if (pref[i] == '&' || pref[i] == '|') {
			string temp1 = spre.top();
			spre.pop();
			string temp2 = spre.top();
			spre.pop();
			string result = pref[i] + temp2 + temp1;


			spre.push(result);
		}
		//if not is encountered
		else if (pref[i] == '\'') {
			string temp3 = spre.top();
			spre.pop();
			string res = temp3 + pref[i];
			//group[ind] = res;
		//	ind++;
			spre.push(res);
		}

		else {
			spre.push(string(1, pref[i]));
			//group[ind++] = pref[i];
		}
	}
	//returning final string
	return spre.top();
}
//string manuip(string str) {
//	string arr[100],arr1[100];
//	//for (int i = 0; i < 100; i++) arr[i] = "";
//	int index = 0;
//	string eq;
//	for (int i = 0; i < str.length(); i++) {
//		if (isalpha(str[i]) || checkop(str[i]) || str[i] == '(') 
//			arr[index] += str[i]; 
//		else if (str[i] == ')') {
//			arr[index] += str[i];
//			if (str[i + 1] != NULL && str[i + 1] == '\'') {
//				arr1[index]=checkstring(arr[index]);
//				index++;
//			}
//			else continue;
//		}
//		else continue;
//	}
//	for (int i = 0; i < index; i++) {
//		eq += arr1[i];
//	}
//	
//	return eq;
//}

struct node
{
    string data;
    node *left;
    node *right;
};

char postfix[35];
int top = -1;
node *arr[35];

int r(char inputchar)
{ //for checking symbol is operand or operator
    if (inputchar == '+' || inputchar == '-' || inputchar == '*' || inputchar
                                                                    == '/')
        return (-1);
    else if (inputchar >= 'a' || inputchar <= 'z')
        return (1);
    else if (inputchar >= 'A' || inputchar <= 'Z')
        return (1);
    else
        return (-99); //for error
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

void create_expr_tree(string suffix[1000])
{
    string symbol;
    node *newl, *ptr1, *ptr2;
    int flag; //flag=-1 when operator and flag=1 when operand
    symbol = suffix[0]; //Read the first symbol from the postfix expr.
    for (int i = 1; symbol != "`"; i++)
    { //continue till end of the expr.
        flag = checkop(symbol); //check symbol is operand or operator.
        if (!flag)//if symbol is operand.
        {
            newl = new node;
            newl->data = symbol;
            newl->left = 0;
            newl->right = 0;
            push(newl);
        }
        else
        { //If the symbol is operator//pop two top elements.
            ptr1 = pop();
            ptr2 = pop();
            newl = new node;
            newl->data = symbol;
            newl->left = ptr2;
            newl->right = ptr1;
            push(newl);
        }
        symbol = suffix[i];
    }
}

void preOrder(node *tree)
{
    if (tree != NULL)
    {
        cout << tree->data;
        preOrder(tree->left);
        preOrder(tree->right);
    }
}

int main()
{
	string group[1000];
	for (int i = 0; i < 1000; i++) {
	    group[i] = "`";
	}
	int gr = 0;
	string str = "a\'&b\'|c\'&d";
	string str1 = conv(str);
	cout << str1 << endl; // postfix str
	string str2 = str1;
	//cout << prefinal(str1) << endl; //correct prefix string

	//Changing the output string to an array of strings
	for (int i = 0; i < str2.length(); i++) {
		if (str2[i] == '&' || str2[i] == '|') { group[gr] = str2[i]; gr++; }
		else if (isalpha(str2[i])) {
			if (isalpha(str2[i + 1])) { group[gr] = str2[i]; gr++; }
			else if (str2[i + 1] == '\'') { group[gr].append(1,str2[i]); group[gr].append(1, str2[i+1]); gr++; }
			else if (str2[i + 1] == '&' || str2[i + 1] == '|') { group[gr] = str2[i]; gr++; }
			else continue;
		}
		else if (str2[i] == '\'') continue;
	}
	cout << endl;

	create_expr_tree(group);



	return 0;
}

