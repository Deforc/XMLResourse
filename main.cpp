#include "XMLResourse.h"

int main() {
    std::unique_ptr<XMLResourse> xmlResourse = XMLResourse::create("..\\input_tree.xml");

    std::cout << xmlResourse->findByName("FuelType")->lock()->getName() << std::endl;
    std::cout << xmlResourse->findByValue("Petrol")->lock()->getValue() << std::endl;

    auto iter1 = xmlResourse->findByName("Engine");

    xmlResourse->add("FuelConsumption", "5.7", iter1);

    auto iter2 = xmlResourse->findByName("ValvesNumber");
    xmlResourse->erase(iter2);
    xmlResourse->save("..\\output_tree.xml");
    return 0;

}

