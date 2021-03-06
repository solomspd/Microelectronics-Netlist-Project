//
// Created by solom on 16/05/19.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <string>

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
int uni1 = -2;
int nMOS = 0, pMOS = 0;
int inter = 1000;
int count = 0;
int inter1 = 1000;
bool up_down;

std::pair<int**, int**> get_ends(node*);
std::pair<int**, int**> create_and(std::pair<int**, int**>&, std::pair<int**, int**>&);
std::pair<int**, int**> create_or(std::pair<int**, int**>&, std::pair<int**, int**>&);
std::pair<int**, int**> create_not(std::pair<int**, int**>&);

int main(){


    //memset(tree, -1, sizeof tree);
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


//To better interpret this, suppose the view from the function opinion is just a tree with a parent and 2 children nodes with each node presenting a circuit (except the parent). The children return the their start and end wires to the parent. The parent then combines the 4 wires together down to 2 according to the parent's value. The parent then returns the 2 new wires.
std::pair<int**, int**> get_ends(node* ptr) {
    std::string parent =  ptr->data;//parent to evaluate

    node* child_1_loc = ptr->left;
    node* child_2_loc = ptr->right; //location of parents children

    std::string child_1 = child_1_loc->data; //value of child
    std::string child_2 = child_2_loc->data; //value of child

    std::pair<int**, int**> child_1_end, child_2_end; //will be the start and end wires that will be recursively returned

    if (child_1_loc != nullptr) {
        child_1_end = get_ends(child_1_loc); //start and end wires of child 1
    }

    if (child_2_loc != nullptr) {
        child_2_end = get_ends(child_2_loc); //start and end wires of child 2
    }

    std::pair<int**, int**> ret;

    if (child_1_loc != nullptr && child_2_loc != nullptr) { //enter case if children not null and parent is one of &, | or '. meaning the children have their start and end wires ready
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
    if (parent[parent.length()-1] == '\'') {
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
    *inter = inter1;
    mos* new_mos = new mos;
    new_mos->bdy = *in.second;
    new_mos->drn = Vdd;
    new_mos->snk = inter;
    mos* new_mos_2 = new mos;
    new_mos_2->bdy = *in.second;
    new_mos_2->drn =  inter;
    new_mos_2->snk = gnd;
    up_netlist.push_back(new_mos);
    down_netlist.push_back(new_mos_2);
    return {in.first, &inter};
}