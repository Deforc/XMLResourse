#include <iomanip>
#include <algorithm>
#include "Tree.h"

void Tree::load(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    std::string strtest = filename.substr(filename.length()-4, 4);
    if(filename.substr(filename.length()-4, 4) != ".xml")
        throw std::runtime_error("Loaded file isn`t \".xml\" format");

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

    treeIterList.updateList(this);

    if(!tagStack.empty())
        throw std::runtime_error("The tree is not specified correctly!");

    file.close();
}

void Tree::save(const std::string &filename) const{
    std::ofstream file(filename);
    saveNode(file, root, 0);
    file.close();
}

void Tree::saveNode(std::ofstream &file, const std::weak_ptr<Node>& node, int indentLevel) const {
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


void Tree::print() const{
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

std::list<std::weak_ptr<Node>>::iterator Tree::findByName(std::string name) {
    return std::find_if(treeIterList.begin(), treeIterList.end(),
                        [&](const std::weak_ptr<Node>& ptr)
                        {return ptr.lock()->getName() == name;});
}

std::list<std::weak_ptr<Node>>::iterator Tree::findByValue(std::string value) {
    return std::find_if(treeIterList.begin(), treeIterList.end(),
                        [&](const std::weak_ptr<Node>& ptr)
                        { return ptr.lock()->getValue() == value; });
}

std::list<std::weak_ptr<Node>>::iterator Tree::add(std::string name, std::string value, std::list<std::weak_ptr<Node>>::iterator addIter) {
    auto newNode = std::make_shared<Node>(name, value);
    if(std::shared_ptr<Node> addToNode = addIter->lock())
    {
        addToNode->addChild(newNode);
        return treeIterList.addNewElemInIterList(addIter, newNode);
    } else {
        throw std::runtime_error("Failed to add this element");
    }

}

bool Tree::erase(std::list<std::weak_ptr<Node>>::iterator delIter) {
    if (delIter == treeIterList.begin()) {
        return false;
    }
    std::weak_ptr<Node> parent;
    for(const auto& nodeInList : treeIterList)
    {
        if(std::shared_ptr<Node> curNodeInList = nodeInList.lock()) {
            for(const auto& child : curNodeInList->getChildren())
                if(delIter->lock() == child.lock())
                {
                    parent = curNodeInList;
                    goto next;
                }
        }
    }

    next:
    if(std::shared_ptr<Node> deletingNode = delIter->lock())
    {
        for(const auto& delIterChild : deletingNode->getChildren())
        {
            if(std::shared_ptr<Node> delIterParent = parent.lock())
                delIterParent->addChild(delIterChild.lock());
        }
        //----Почистить детей удаляемого node----
        //---------------------------------------
        treeIterList.eraseElemFromIterList(delIter);
        if(std::shared_ptr<Node> delIterParent = parent.lock())
            delIterParent->deleteNodeFromChildren(delIter->lock());
    } else return false;

    return  true;
}

void Tree::IteratorManager::updateList(Tree* tree) {
    tree->for_each([&](std::weak_ptr<Node> node) ->void { iteratorList.push_back(node); } );
}

std::list<std::weak_ptr<Node>>::iterator Tree::IteratorManager::addNewElemInIterList(std::list<std::weak_ptr<Node>>::iterator iterator, const std::weak_ptr<Node>& newNode) {
   return iteratorList.insert(inListFinder(iterator), newNode);
}

void Tree::IteratorManager::eraseElemFromIterList(std::list<std::weak_ptr<Node>>::iterator iterator) {
    iteratorList.erase(inListFinder(iterator));
}
