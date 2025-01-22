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

// Класс способности
class Skill {
public:
    std::string name;
    int level;
    int cooldown;
    int currentCooldown;

    Skill(std::string n, int lvl, int cd) : name(n), level(lvl), cooldown(cd), currentCooldown(0) {}

    void reduceCooldown() {
        if (currentCooldown > 0) currentCooldown--;
    }

    void upgrade() {
        level++;
    }
};

// Класс игрока
class Player {
public:
    std::string name;
    int health;
    int attack;
    int defense;
    int level;
    int experience;
    int coins;
    int freeCoinsUsed;
    Inventory inventory;
    Skill powerfulStrike;
    Skill stun;

    Player(std::string n)
        : name(n), health(100), attack(10), defense(5), level(1), experience(0), coins(0), freeCoinsUsed(0),
        powerfulStrike("Мощный удар", 1, 3), stun("Оглушение", 1, 5) {}
};


int main() {
    setlocale(LC_ALL, "Rus");
    return 0;
}