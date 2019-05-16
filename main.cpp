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
#include<stack>



const int N = 1e3;

struct node
{
    std::string data;
    node *left;
    node *right;
};

node* tree;

/*

// READ THIS //

 NULL = -1
 AND = -2
 OR = -3
 NOT = -4

*/

struct mos {
    int* drn;
    int* bdy;
    int* snk;
};

std::vector<mos*> up_netlist, down_netlist;

int* Vdd, *gnd;
int uni1 = -3;
int nMOS = 0, pMOS = 0;
int inter = 1000;
int count = 0;
int inter1 = 1000;
bool up_down;
int top = -1;
node *arr[35];
std::string group[1000];

std::pair<int**, int**> get_ends(node*);
std::pair<int**, int**> create_and(std::pair<int**, int**>&, std::pair<int**, int**>&);
std::pair<int**, int**> create_or(std::pair<int**, int**>&, std::pair<int**, int**>&);
std::pair<int**, int**> create_not(std::pair<int**, int**>&);

bool checkop(std::string);
int order(char);
std::string conv(std::string);
void push(node*);
node* pop();
void create_expr_tree(std::string[]);

int main(){


    //memset(tree, -1, sizeof tree);
    for (int i = 0; i < 1000; i++) {
        group[i] = "`";
    }
    std::string str = "a\'&b\'|c\'&d";
    std::string str1 = conv(str);
    std::string str2 = str1;

    int gr = 0;

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

    create_expr_tree(group);

    tree = arr[0];

    Vdd = new int;
    *Vdd = -1;
    gnd = new int;
    *gnd = 0;
    int* y = new int;
    *y = -2;
    up_down = true;
    std::pair<int**, int**> up = get_ends(tree); //returns a pair of pointers that are the start and end wire for the generated pull up network
    up_down = false;
    std::pair<int**, int**> down = get_ends(tree);
    **up.first = *Vdd; //make start = Vdd
    **up.second = *y; //make end = y
    **down.first = *y;
    **down.second = *gnd;
    std::cout << "Vdd: " << **up.first << std::endl << "Gnd: " << **down.second << std::endl << std::endl << std::endl << "Netlist: " << std::endl << std::endl;
    int j = 0;
    for (auto i : up_netlist) {
        std::cout << 'M' << j++ << ' ' << *i->snk << ' ' << *i->bdy << ' ' << *i->drn << ' ' << *i->drn << ' ' << "PMOS" << std::endl; //printing netlist
    }
    for (auto i : down_netlist) {
        std::cout << 'M' << j++ << ' ' << *i->snk << ' ' << *i->bdy << ' ' << *i->drn << ' ' << *i->drn << ' ' << "NMOS" << std::endl;
    }

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
std::string conv(std::string str)
{
    std::stack<char> in;
    in.push(':');
    std::string eq;
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
std::pair<int**, int**> get_ends(node* ptr) {
    std::string parent =  ptr->data;//parent to evaluate

    node* child_1_loc = ptr->left;
    node* child_2_loc = ptr->right; //location of parents children


    std::pair<int**, int**> child_1_end, child_2_end; //will be the start and end wires that will be recursively returned

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

    std::pair<int**, int**> ret;

    if (c1 && c2) { //enter case if children not null and parent is one of &, | or '. meaning the children have their start and end wires ready
        switch (parent[0]) {
            case '&' : if (up_down) {
                    ret = create_and(child_1_end, child_2_end);
                } else {
                    ret = create_or(child_1_end, child_2_end);
                }
                break;
            case '|': if (up_down) {
                    ret = create_or(child_1_end, child_2_end);
                } else {
                    ret = create_and(child_1_end, child_2_end);
                }
                break;
            default: std::cout << "ERROR";
        }
    } else { //this creates the start and end wires that are to be combined
        mos* new_mos = new mos;
        new_mos->bdy = new int;
        *new_mos->bdy = parent[0];
        new_mos->drn = new int;
        *new_mos->drn= --uni1;
        new_mos->snk = new int;
        *new_mos->snk = --uni1;
        if (up_down) {
            up_netlist.push_back(new_mos); //push to the netlist so we can output it later and values are still changed in the netlist due to pointers
        } else {
            down_netlist.push_back(new_mos);
        }
        ret = {&new_mos->drn, &new_mos->snk}; //start and end of a single MOSFET
    }
    if (parent.back() == '\'') {
        ret = create_not(ret);
    }
    return ret;
}


std::pair<int**, int**> create_and(std::pair<int**, int**> &in1, std::pair<int**, int**> &in2) {
    **in2.first = **in1.second;
    *in2.first = *in1.second;
    in2.first = in1.second;
    return {in1.first, in2.second};
}

std::pair<int**, int**> create_or(std::pair<int**, int**> &in1, std::pair<int**, int**> &in2) {
    **in2.first = **in1.first;
    *in2.first = *in1.first;
    in2.first, in1.first;
    **in2.second = **in1.second;
    *in2.second = *in1.second;
    in2.second = in1.second;
    return {in1.first, in1.second};
}

std::pair<int**, int**> create_not(std::pair<int**, int**> &in) {
    int* inter = new int;
    *inter = --uni1;
    mos* new_mos = new mos;
    new_mos->bdy = *in.second;
    new_mos->drn = Vdd;
    new_mos->snk = inter;
    mos* new_mos_2 = new mos;
    new_mos_2->bdy = *in.second;
    new_mos_2->drn = inter;
    new_mos_2->snk = gnd;
    up_netlist.push_back(new_mos);
    down_netlist.push_back(new_mos_2);
    return {in.first, &inter};
}