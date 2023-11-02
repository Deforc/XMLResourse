#ifndef XMLRESOURSE_TREE_H
#define XMLRESOURSE_TREE_H

#include <fstream>
#include <stack>

#include "Node.h"

class Tree {
private:
    std::unique_ptr<Node> root;
public:
    Tree() : root(nullptr) {}

    void load(const std::string& filename);

    void save(const std::string& filename);

private:
    void saveNode(std::ofstream& file, Node* node, int indentLevel);
};

#endif //XMLRESOURSE_TREE_H
