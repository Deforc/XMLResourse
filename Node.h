#ifndef XMLRESOURSE_NODE_H
#define XMLRESOURSE_NODE_H

#include <iostream>
#include <memory>
#include <vector>
#include <functional>


class Node {
public:

    Node(std::string  name, std::string  value) : name(std::move(name)), value(std::move(value)) {}

    void addChild(std::shared_ptr<Node> child) ;
    std::vector<std::weak_ptr<Node>> getChildren () const;
    std::string getName() const { return  name; }
    std::string getValue() const { return value; }
    void deleteNodeFromChildren(const std::weak_ptr<Node>& deletingNode);
    void for_each(const std::function<void (std::weak_ptr<Node>)>& callback);


private:
    std::string name;
    std::string value;
    std::vector<std::shared_ptr<Node>> children;
};





#endif //XMLRESOURSE_NODE_H
