#ifndef XMLRESOURSE_NODE_H
#define XMLRESOURSE_NODE_H

#include <iostream>
#include <memory>
#include <vector>

class Node {
public:
    Node(std::string  name, std::string  value) : name(std::move(name)), value(std::move(value)) {}

    void addChild(std::unique_ptr<Node> child) ;
    std::vector<Node*> getChildren () ;
    Node* getLastChild() ;
    std::string getName() { return  name; }
    std::string getValue() { return value; }

private:
    std::string name;
    std::string value;
    std::vector<std::unique_ptr<Node>> children;
};


#endif //XMLRESOURSE_NODE_H
