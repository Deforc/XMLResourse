#include "Node.h"

void Node::addChild(std::shared_ptr<Node> child) {
    children.push_back(std::move(child));
}

std::vector<std::weak_ptr<Node>> Node::getChildren() {
    std::vector<std::weak_ptr<Node>> returnedChildren;
    returnedChildren.reserve(children.size());
    for (const auto & i : children)
    {
        returnedChildren.push_back(i);
    }
    return returnedChildren;
}

void Node::for_each(std::function<void(std::weak_ptr<Node>)> callback) {
    for (const auto& child : children) {
        callback(child);
        child->for_each(callback);
    }
}



