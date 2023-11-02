#include "Tree.h"

int main() {
    Tree tree;

    tree.load("..\\input_tree.xml");
    tree.save("..\\output_tree.xml");

    return 0;
}

