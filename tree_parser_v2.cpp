
//
// Created by solom on 06/05/19.
//

#include <iostream>
#include <cstring>
#include <vector>

const int N = 1e3;

int tree[N];

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

std::vector<mos*> netlist;

int* Vdd, *gnd;
int uni1 = 105;
int nMOS = 0, pMOS = 0;
int inter = 1000;
int count = 0;
int inter1 = 1000;

std::pair<int*, int*> get_ends(int = 0);
std::pair<int*, int*> create_and(std::pair<int*, int*>&, std::pair<int*, int*>&);
std::pair<int*, int*> create_or(std::pair<int*, int*>&, std::pair<int*, int*>&);
std::pair<int*, int*> create_not(std::pair<int*, int*>&);

int main(){


    memset(tree, -1, sizeof tree);
    Vdd = new int;
    gnd = new int;
    tree[0] = -2;
    tree[1] = 'a';
    tree[2] = 'b';
    int y = 111;
    std::pair<int*, int*> temp = get_ends();
    temp.first = Vdd;
    *temp.second = y;
    std::cout << *temp.first << std::endl << *temp.second << std::endl;
    int j = 0;
    for (auto i : netlist) {
        std::cout << 'M' << j++ << *i->bdy << ' ' << *i->drn << ' ' << *i->snk << ' ' << *i->snk << ' ' << "PMOS" << std::endl;
    }

    return 0;

}


std::pair<int*, int*> get_ends(int parent_loc) {
    int parent = tree[parent_loc];

    int child_1_loc = parent_loc * 2 + 1, child_2_loc = child_1_loc + 1;

    int child_1 = tree[child_1_loc];
    int child_2 = tree[child_2_loc];

    std::pair<int*, int*> child_1_end, child_2_end;
    if (child_1 != -1) {
        child_1_end = get_ends(child_1_loc);
    }

    if (child_2 != -1) {
        child_2_end = get_ends(child_2_loc);
    }

    std::pair<int*, int*> ret;

    if (parent < 0 && child_1 != -1 && child_2 != -1) {
        switch (parent) {
            case -2: ret = create_and(child_1_end, child_2_end);
                break;
            case -3: ret = create_or(child_1_end, child_2_end);
                break;
            case -4: ret = create_not(child_1_end);
                break;
            default: std::cout << "ERROR";
        }
    } else {
        mos* new_mos = new mos;
        new_mos->bdy = new int;
        *new_mos->bdy = parent;
        new_mos->drn = new int;
        *new_mos->drn= ++uni1;
        new_mos->snk = new int;
        *new_mos->snk = ++uni1;
        netlist.push_back(new_mos);
        ret = {new_mos->drn, new_mos->snk};
    }
    return ret;
}

std::pair<int*, int*> create_and(std::pair<int*, int*> &in1, std::pair<int*, int*> &in2) {
    delete in2.first;
    in2.first = in1.second;
    return {in1.first, in2.second};
}

std::pair<int*, int*> create_or(std::pair<int*, int*> &in1, std::pair<int*, int*> &in2) {
    delete in2.first;
    delete in2.second;
    in2.first = in1.first;
    in2.second = in1.second;
    return {in1.first, in1.second};
}

std::pair<int*, int*> create_not(std::pair<int*, int*> &in) {
    int* inter = new int;
    *inter = inter1;
    mos* new_mos = new mos;
    new_mos->bdy = in.second;
    new_mos->drn = Vdd;
    new_mos->snk = inter;
    mos* new_mos_2 = new mos;
    new_mos_2->bdy = in.second;
    new_mos_2->drn =  inter;
    new_mos_2->snk = gnd;
    return {in.first, inter};
}