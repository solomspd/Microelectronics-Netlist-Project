#include<iostream>
#include<string>
#include<stack>
using namespace std;

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
//string checkstring(string str) {
//	int x=0;
//	for (int i = x; i < str.length(); i++) {
//		if (isalpha(str[i]) || checkop(str[i])) {
//			if (str[i + 1] != '^') { str.insert(i + 1, "^"); x = i + 2; }
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
	if (c == '^')
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
		if ((str[i] >= 'a' && str[i] <= 'z' && str[i+1]=='^')) 
			eq += str[i];
	// if not it was complemented
		else if  ((str[i] >= 'a' && str[i] <= 'z' && str[i + 1] != '^')) 
			eq = eq + str[i] + '^';
		

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
	// returning string
	return eq;


}
// changing from post to prefix
string prefinal(string pref,string group[100],int &ind)
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
			//Here where i try to make it an array of strings
			group[ind] = pref[i];
			ind++;
			group[ind] = temp2;
			ind++;
			group[ind] = temp1;
			ind++;
			string result = pref[i] + temp2 + temp1;
			//group[ind++] = result;

			spre.push(result);
		}
		//if not is encountered
		else if (pref[i] == '^') {
			string temp3 = spre.top();
			spre.pop();
			string res = temp3+ pref[i] ;
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
//			if (str[i + 1] != NULL && str[i + 1] == '^') {
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

int main()
{
	string group[100];
	int ind=0;
		string str = "a^&b|c^&d";
	string str1 = conv(str);
	cout << str1 << endl; // postfix str
	cout << prefinal(str1,group,ind) << endl; //correct prefix string
	
	for (int i =0; i < ind;i++) {
		cout << group[i];
	} //check
	cout << endl;
	system("pause");
	return 0;
}

