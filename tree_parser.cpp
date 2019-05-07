//
// Created by solom on 06/05/19.
//

#include <iostream>
#include <cstring>

const int N = 1e3;

int tree[N];

/*

// READ THIS //

 NULL = -1
 AND = -2
 OR = -3
 NOT = -4

*/



int Vdd, gnd;
int uni1 = 105;
int nMOS = 0, pMOS = 0;
int inter = 1000;

std::pair<int, int> get_ends(int = 0);
std::pair<int, int> create_and(std::pair<int, int>, std::pair<int, int>);
std::pair<int, int> create_or(std::pair<int, int>, std::pair<int, int>);
std::pair<int, int> create_not(std::pair<int, int>);

int main(){


    memset(tree, -1, sizeof tree);

    tree[0] = -2;
    tree[1] = 'a';
    tree[2] = 'b';
    std::pair<int, int> temp = get_ends();
    std::cout << temp.first << std::endl << temp.second << std::endl;
    return 0;

}


std::pair<int, int> get_ends(int parent_loc) {
    int parent = tree[parent_loc];

    int child_1_loc = parent_loc * 2 + 1, child_2_loc = child_1_loc + 1;

    int child_1 = tree[child_1_loc];
    int child_2 = tree[child_2_loc];

    std::pair<int, int> child_1_end, child_2_end;
    if (child_1 != -1) {
        child_1_end = get_ends(child_1_loc);
    }

    if (child_2 != -1) {
        child_2_end = get_ends(child_2_loc);
    }

    std::pair<int, int> ret;

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
        std::cout << "M" << nMOS++ << " " << uni1++ << " " << parent << " " << uni1 << " " << uni1 << " PMOS" << std::endl;
        ret = {uni1 - 1, uni1++};
    }
    return ret;
}

std::pair<int, int> create_and(std::pair<int, int> in1, std::pair<int, int> in2) {
    std::cout << "node " << inter++ << " " <<  in1.second << " " << in2.first << std::endl;
    return {in1.first, in2.second};
}

std::pair<int, int> create_or(std::pair<int, int> in1, std::pair<int, int> in2) {
    std::cout << "node " << inter++ << " " << in1.first << " " << in2.first << std::endl;
    std::cout << "node " << inter++ << " " << in1.second << " " << in2.second << std::endl;
    return {inter - 1, inter};
}

std::pair<int, int> create_not(std::pair<int, int> in) {
    std::cout << "M" << nMOS++ << ++inter << " " << in.second << Vdd << " NMOS" << std::endl;
    std::cout << "M" << nMOS++ << inter << " " << in.second << gnd << " NMOS" << std::endl;
    return {in.first, inter};
}