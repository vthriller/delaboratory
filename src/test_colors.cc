#include "convert_color.h"
#include <iostream>

void test_rgb_2_lab(deValue r, deValue g, deValue b)
{
    std::cout << "rgb -> lab" << std::endl;
    deValue v[8];

    v[0]=r;
    v[1]=g;
    v[2]=b;
    v[3]=0;

    std::cout << "R: " << v[0] << std::endl;
    std::cout << "G: " << v[1] << std::endl;
    std::cout << "B: " << v[2] << std::endl;

    rgb2lab(v);

    std::cout << "L: " << v[4] << std::endl;
    std::cout << "A: " << v[5] << std::endl;
    std::cout << "B: " << v[6] << std::endl;
}

void test_rgb_2_xyz(deValue r, deValue g, deValue b)
{
    std::cout << "rgb -> xyz" << std::endl;
    deValue v[8];

    v[0]=r;
    v[1]=g;
    v[2]=b;
    v[3]=0;

    std::cout << "R: " << v[0] << std::endl;
    std::cout << "G: " << v[1] << std::endl;
    std::cout << "B: " << v[2] << std::endl;

    rgb2xyz(v);

    std::cout << "X: " << v[4] << std::endl;
    std::cout << "Y: " << v[5] << std::endl;
    std::cout << "Z: " << v[6] << std::endl;
}

void test_xyz_2_rgb(deValue x, deValue y, deValue z)
{
    std::cout << "xyz -> rgb" << std::endl;
    deValue v[8];

    v[0]=x;
    v[1]=y;
    v[2]=z;
    v[3]=0;

    std::cout << "X: " << v[0] << std::endl;
    std::cout << "Y: " << v[1] << std::endl;
    std::cout << "Z: " << v[2] << std::endl;

    xyz2rgb(v);

    std::cout << "R: " << v[4] << std::endl;
    std::cout << "G: " << v[5] << std::endl;
    std::cout << "B: " << v[6] << std::endl;
}

void test_lab_2_xyz(deValue l, deValue a, deValue b)
{
    std::cout << "lab -> xyz" << std::endl;
    deValue v[8];

    v[0]=l;
    v[1]=a;
    v[2]=b;
    v[3]=0;

    std::cout << "L: " << v[0] << std::endl;
    std::cout << "A: " << v[1] << std::endl;
    std::cout << "B: " << v[2] << std::endl;

    lab2xyz(v);

    std::cout << "X: " << v[4] << std::endl;
    std::cout << "Y: " << v[5] << std::endl;
    std::cout << "Z: " << v[6] << std::endl;
}


int main()
{

    test_rgb_2_lab(100, 100, 100);
    test_rgb_2_xyz(100, 100, 100);

    test_lab_2_xyz(44, 0, 0);

    return 0;
}
