#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <locale.h>

// ����� ��������
class Item {
public:
    std::string name;
    int value;

    Item(std::string n, int v) : name(n), value(v) {}
};

// ����� ����� ��������
class HealthPotion : public Item {
public:
    int healAmount;

    HealthPotion(std::string n, int v, int heal) : Item(n, v), healAmount(heal) {}
};

// ����� ���������
class Inventory {
public:
    std::vector<Item*> items; 
};

int main() {
    setlocale(LC_ALL, "Rus");
    return 0;
}