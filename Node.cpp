#include "Node.h"

void Node::addChild(std::unique_ptr<Node> child) {
    children.push_back(std::move(child));
}

std::vector<Node *> Node::getChildren() {
    std::vector<Node*> returnedChildren ;
    returnedChildren.reserve(children.size());
    for (const auto & i : children)
    {
        returnedChildren.push_back(i.get());
    }
    return returnedChildren;
}

Node *Node::getLastChild() {
    if(!children.empty())
        return children.back().get();
    else
        return nullptr;
}