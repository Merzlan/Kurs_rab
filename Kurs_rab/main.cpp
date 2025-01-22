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
    void addItem(Item* item) {
        items.push_back(item);
    }

    void removeItem(const std::string& itemName) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->name == itemName) {
                items.erase(it);
                break;
            }
        }
    }

    int getItemCount(const std::string& itemName) {
        int count = 0;
        for (const auto& item : items) {
            if (item->name == itemName) {
                count++;
            }
        }
        return count;
    }

    void showInventory() {
        if (items.empty()) {
            std::cout << "Ваш инвентарь пуст.\n";
        }
        else {
            std::cout << "Ваш инвентарь:\n";
            for (const auto& item : items) {
                std::cout << "- " << item->name << "\n";
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Rus");
    return 0;
}