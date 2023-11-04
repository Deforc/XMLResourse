#include <iomanip>
#include <algorithm>
#include "Tree.h"

void Tree::load(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }

    std::string line;
    std::stack<std::weak_ptr<Node>> tagStack;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        size_t indent = line.find_first_not_of("    ");
        line = line.substr(indent);
        size_t pos = 0;
        while (pos < line.length()) {
            if (line[pos] == '<') {
                size_t closingBracket = line.find('>', pos + 1);
                std::string tag = line.substr(pos + 1, closingBracket - pos - 1);

                if (tag[0] == '/') {
                    tagStack.pop();
                } else {
                    size_t valueStart = line.find('\"', closingBracket + 1) + 1;
                    size_t valueEnd = line.find('\"', valueStart);
                    std::string value = line.substr(valueStart, valueEnd - valueStart);
                    if(tagStack.empty())
                    {
                        if(root != nullptr)
                            throw std::runtime_error("You are already have HEAD");
                        root = std::make_shared<Node>(tag, value);
                        tagStack.push(root);
                    } else {
                        auto newNode = std::make_shared<Node>(tag, value);
                        if (std::shared_ptr<Node> stackTop = tagStack.top().lock())
                        {
                            stackTop->addChild(newNode);
                            tagStack.push(newNode);
                        }
                    }
                }

                pos = closingBracket + 1;
            } else {
                pos++;
            }
        }
    }

    treeIter.updateList(this);

    if(!tagStack.empty())
        throw std::runtime_error("The tree is not specified correctly!");

    file.close();
}

void Tree::save(const std::string &filename) {
    std::ofstream file(filename);
    saveNode(file, root, 0);
    file.close();
}

void Tree::saveNode(std::ofstream &file, const std::weak_ptr<Node>& node, int indentLevel)  {
    std::string indent(indentLevel * 4, ' ');
    if (std::shared_ptr<Node> curNode = node.lock()) {
        file << indent << "<" << curNode->getName() << ">" << " value = \"" << curNode->getValue() << "\" ";
        for (const auto& child : curNode->getChildren()) {
            file << std::endl;
            saveNode(file, child, indentLevel + 1);
        }

        if (!curNode->getChildren().empty()) {
            file << std::endl << indent;
        }

        file << "</" << curNode->getName() << ">";
    }


}


void Tree::print() {
    std::stack<std::pair <int, std::weak_ptr<Node>>> nodes;
    nodes.emplace(0, root);

    while (!nodes.empty()) {
        auto currentTopNode = nodes.top();
        if (std::shared_ptr<Node> currentNode = currentTopNode.second.lock()){
            std::string nodeOutput = "(" + currentNode->getName() + ") " + currentNode->getValue();
            std::cout << std::string(currentTopNode.first, ' ') << nodeOutput << std::endl;
            nodes.pop();
            std::vector<std::weak_ptr<Node>> tmpNodes = currentNode->getChildren();
            std::reverse(tmpNodes.begin(), tmpNodes.end());
            for (const auto& child : tmpNodes)
                if(std::shared_ptr<Node> childNode = child.lock())
                    nodes.emplace(currentTopNode.first + nodeOutput.length(), childNode);
        } else nodes.pop();
    }
}

void Tree::for_each(std::function<void (std::weak_ptr<Node>)> callback) {
    callback(root);
    root->for_each(callback);
}

void Tree::Iterator::updateList(Tree* tree) {
    tree->for_each([&](std::weak_ptr<Node> node) ->void { iteratorList.push_back(node); } );
}

void Tree::Iterator::addNewElemInIterList(std::list<std::weak_ptr<Node>>::iterator iterator, const std::weak_ptr<Node>& newNode) {
    iteratorList.insert(inListFinder(iterator), newNode);
}

void Tree::Iterator::eraseElemFromIterList(std::list<std::weak_ptr<Node>>::iterator iterator) {
    iteratorList.erase(inListFinder(iterator));
}
