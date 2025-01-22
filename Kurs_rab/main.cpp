#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <locale.h>

// Класс предмета
class Item {
public:
    std::string name;
    int value;

    Item(std::string n, int v) : name(n), value(v) {}
};

// Класс зелья здоровья
class HealthPotion : public Item {
public:
    int healAmount;

    HealthPotion(std::string n, int v, int heal) : Item(n, v), healAmount(heal) {}
};

// Класс инвентаря
class Inventory {
public:
    std::vector<Item*> items; 
};

int main() {
    setlocale(LC_ALL, "Rus");
    return 0;
}