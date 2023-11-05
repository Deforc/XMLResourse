#ifndef XMLRESOURSE_XMLRESOURSE_H
#define XMLRESOURSE_XMLRESOURSE_H
#include "Tree.h"

class XMLResourse {
public:
    static std::unique_ptr<XMLResourse> create (const std::string& path);
    void load(const std::string& path);
    void save(const std::string& path) const;
    std::list<std::weak_ptr<Node>>::iterator findByValue(std::string value);
    std::list<std::weak_ptr<Node>>::iterator findByName(std::string name);
    std::list<std::weak_ptr<Node>>::iterator add(std::string name, std::string value, std::list<std::weak_ptr<Node>>::iterator addIter) ;
    bool erase(std::list<std::weak_ptr<Node>>::iterator delIter);
    void print() const;
    void for_each(const std::function<void (std::weak_ptr<Node>)>& callback);
private:
    Tree tree;
    XMLResourse(const std::string& path);
};


#endif //XMLRESOURSE_XMLRESOURSE_H
