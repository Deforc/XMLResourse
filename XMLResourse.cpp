#include "XMLResourse.h"
#include <utility>

XMLResourse::XMLResourse(const std::string &path) {
    tree = Tree();
    try {
        tree.load(path);
    } catch (...) {
        tree = Tree();
    }
}

std::unique_ptr<XMLResourse> XMLResourse::create(const std::string &path) {
    return std::unique_ptr<XMLResourse>(new XMLResourse(path));
}

void XMLResourse::load(const std::string& path) {
    Tree saveTree = tree;
    try {
        tree.load(path);
    } catch (...) {
        tree = saveTree;
    }
}

void XMLResourse::save(const std::string &path) const{
    tree.save(path);
}

std::list<std::weak_ptr<Node>>::iterator XMLResourse::findByValue(std::string value) {
    if(tree.findByValue(value) == tree.end())
        std::cout << "Be careful, there is no such Container with this value!" << std::endl;
    return tree.findByValue(std::move(value));
}

std::list<std::weak_ptr<Node>>::iterator XMLResourse::findByName(std::string name) {
    if(tree.findByName(name) == tree.end())
        std::cout << "Be careful, there is no such Container with this name!" << std::endl;
    return tree.findByName(std::move(name));
}

std::list<std::weak_ptr<Node>>::iterator
XMLResourse::add(std::string name, std::string value, std::list<std::weak_ptr<Node>>::iterator addIter) {

    try {
        return tree.add(std::move(name), std::move(value), addIter);
    } catch (...) {
        std::cout << "Failed to add new element in tree! " << std::endl << "An iterator is returned to the end of the tree";
        return tree.end();
    }

}

bool XMLResourse::erase(std::list<std::weak_ptr<Node>>::iterator delIter) {
    return tree.erase(delIter);
}

void XMLResourse::print() const{
    tree.print();
}

void XMLResourse::for_each(const std::function<void(std::weak_ptr<Node>)>& callback) {
    tree.for_each(callback);
}
