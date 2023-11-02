#include "Tree.h"

void Tree::load(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }

    std::string line;
    std::stack<Node*> tagStack;

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
                        root = std::make_unique<Node>(tag, value);
                        tagStack.push(root.get());
                    } else {
                        auto newNode = std::make_unique<Node>(tag, value);
                        tagStack.top()->addChild(std::move(newNode));
                        tagStack.push(tagStack.top()->getLastChild());
                    }
                }

                pos = closingBracket + 1;
            } else {
                pos++;
            }
        }
    }

    if(!tagStack.empty())
        throw std::runtime_error("The tree is not specified correctly!");

    file.close();
}

void Tree::save(const std::string &filename) {
    std::ofstream file(filename);
    saveNode(file, root.get(), 0);
    file.close();
}

void Tree::saveNode(std::ofstream &file, Node *node, int indentLevel)  {
    std::string indent(indentLevel * 4, ' ');
    file << indent << "<" << node->getName() << ">" << " value = \"" << node->getValue() << "\" ";

    for (auto child : node->getChildren()) {
        file << std::endl;
        saveNode(file, child, indentLevel + 1);
    }

    if (!node->getChildren().empty()) {
        file << std::endl << indent;
    }

    file << "</" << node->getName() << ">";
}
