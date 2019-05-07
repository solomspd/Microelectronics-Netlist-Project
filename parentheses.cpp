#include<iostream>
#include<string>
#include<stack>
using namespace std;


void tocirc(string str1) {
	stack<char> s1;
	char x1, x2;
	for (int i = str1.length()-1; i >= 0; i--) {
		s1.push(str1[i]);
	}
	for (int i=0;i<str1.length();i++){
		if (str1[i] == '&') {
			x1 = s1.top();
			s1.pop();
			x2 = s1.top();
			s1.pop();
			//apply the and mosfet
		}
		else if (str1[i] == '|') {
			x1 = s1.top();
			s1.pop();
			x2 = s1.top();
			s1.pop();
			//apply the or mosfet
		}
	}
}
int order(char c)
{
	 if (c == '&')
		return 2;
	else if (c == '|')
		return 1;
	else
		return -1;
}

// The main function to convert infix expression 
//to postfix expression 
string conv(string str)
{
	stack<char> in;
	in.push(':');
	string eq;
	for (int i = 0; i < str.length(); i++)
	{
		
		if ((str[i] >= 'a' && str[i] <= 'z'))
			eq += str[i];
 
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
			in.push(str[i]);
		}

	}
	//Pop all the remaining elements from the stack 
	while (in.top() != ':')
	{
		char c = in.top();
		in.pop();
		eq += c;
	}
	return eq;
	cout << eq << endl;

}

int main()
{
	string str,str1;
	string str1= conv(str);
	tocirc(str1);
	return 0;
}
 
