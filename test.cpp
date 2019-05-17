#include <iostream>

void point_to_same_location(int* const & pt1, int*& pt2)
{
    pt2= pt1;
}

int main()
{
    int i1= 0;
    int i2= 0;
    int i3= 0;
    int* pt1= &i1;
    int* pt2= &i2;
    int* pt3= &i3;

    std::cout << "pt1=" << pt1
              << " pt2=" << pt2
              << " pt3=" << pt3
              << std::endl;

    point_to_same_location(pt1, pt2);
    point_to_same_location(pt2, pt3);

    std::cout << "pt1=" << pt1
              << " pt2=" << pt2
              << " pt3=" << pt3
              << std::endl;
}