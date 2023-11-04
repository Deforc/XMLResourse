#ifndef XMLRESOURSE_TREE_H
#define XMLRESOURSE_TREE_H

#include <fstream>
#include <stack>
#include <list>

#include "Node.h"

class Tree {

public:

    class IteratorManager {
    public:
        IteratorManager() { iteratorList = {}; }

        void updateList(Tree* tree);
        std::list<std::weak_ptr<Node>>::iterator addNewElemInIterList(std::list<std::weak_ptr<Node>>::iterator iterator, const std::weak_ptr<Node>& newNode);
        void eraseElemFromIterList(std::list<std::weak_ptr<Node>>::iterator iterator);

        std::list<std::weak_ptr<Node>>::iterator begin () { return iteratorList.begin(); }
        std::list<std::weak_ptr<Node>>::iterator end () { return iteratorList.end(); }

        std::list<std::weak_ptr<Node>>::iterator inListFinder(std::list<std::weak_ptr<Node>>::iterator iter) {
            return std::find_if(iteratorList.begin(), iteratorList.end(),
                                [&](const std::weak_ptr<Node>& ptr)
                                {return ptr.lock() == iter->lock();});
        }

    private:
        std::list<std::weak_ptr<Node>> iteratorList;
    };

    Tree() : root(nullptr) { }

    void load(const std::string& filename);

    void save(const std::string& filename);
    void print();
    void for_each(std::function<void (std::weak_ptr<Node>)> callback);

    std::list<std::weak_ptr<Node>>::iterator findByName(std::string name);
    std::list<std::weak_ptr<Node>>::iterator findByValue(std::string value);
    std::list<std::weak_ptr<Node>>::iterator add(std::string name, std::string value, std::list<std::weak_ptr<Node>>::iterator addIter) ;
    bool erase(std::list<std::weak_ptr<Node>>::iterator delIter);
private:
    std::shared_ptr<Node> root;
    IteratorManager treeIterList = IteratorManager();
    void saveNode(std::ofstream& file, const std::weak_ptr<Node>& node, int indentLevel);
};

#endif //XMLRESOURSE_TREE_H
