#include<iostream>
#include<string>
#include<stack>
using namespace std;

int order(char c)
{
	if (c == '^')
		return 3;
	 if (c == '&')
		return 2;
	else if (c == '|')
		return 1;
	else
		return -1;
}
string conv(string str)
{
	stack<char> in;
	in.push(':');
	string eq;
	for (int i = 0; i < str.length(); i++)
	{
		if ((str[i] >= 'a' && str[i] <= 'z' && str[i+1]=='^')) 
			eq += str[i];
	
		else if  ((str[i] >= 'a' && str[i] <= 'z' && str[i + 1] != '^')) 
			eq = eq + str[i] + '^';
		

		else if (str[i] == '(')
			in.push('(');

		else if (str[i] == ')')
		{
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
		else {
			while (in.top() != ':' && order(str[i]) <= order(in.top()))
			{
				char c = in.top();
				in.pop();
				eq += c;
			}
			if (isalpha(str[i]) ||str[i]=='&' || str[i]=='|')
			in.push(str[i]);
		}

	}
	while (in.top() != ':')
	{
		char c = in.top();
		in.pop();
		eq += c;
	}
	return eq;
	cout << eq << endl;

}
bool checkop(char x)
{
	switch (x) {
	case '&':
	case '|':
	case '^':
		return true;
	}
	return false;
}
string prefinal(string pref)
{
	stack<string> spre;
	int length = pref.size();
	for (int i = 0; i < length; i++) {
		if (pref[i] == '&' || pref[i] == '|') {
			string temp1 = spre.top();
			spre.pop();
			string temp2 = spre.top();
			spre.pop();
			string result = pref[i] + temp2 + temp1;

			spre.push(result);
		}
		//THIS THE CONDITION HAVING THE PROBLEM
		else if (pref[i] == '^') {
			string temp3 = spre.top();
			spre.pop();
			string res = pref[i]+temp3;
			spre.push(res);
		}
		//IT ENDS HERE

		else
			spre.push(string(1, pref[i]));

	}

	return spre.top();
}

int main()
{
	string str = "(a&b)";
	string str1 = conv(str);
	cout << str1 << endl;
	cout << prefinal(str1) << endl;
	system("pause");
	return 0;
}

