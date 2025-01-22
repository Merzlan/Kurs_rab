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

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= getExperienceToNextLevel()) {
            levelUp();
        }
    }

    void levelUp() {
        level++;
        attack += 5;
        defense += 3;
        health = 100;
        experience = 0;
        std::cout << "Вы достигли уровня " << level << "! Ваши характеристики улучшены.\n";
    }

    int getExperienceToNextLevel() const {
        return 100 + (level - 1) * 50;
    }

    bool isAlive() {
        return health > 0;
    }

    void addCoins(int amount) {
        coins += amount;
    }

    void spendCoins(int amount) {
        coins -= amount;
    }

    bool getFreeCoins() {
        if (freeCoinsUsed < 3) {
            coins += 10;
            freeCoinsUsed++;
            std::cout << "Вы получили 10 монет. Осталось попыток: " << 3 - freeCoinsUsed << "\n";
            return true;
        }
        else {
            std::cout << "Вы исчерпали лимит бесплатных монет.\n";
            return false;
        }
    }

    void useSkill(Skill& skill, class Monster& monster);

    void reduceCooldowns() {
        powerfulStrike.reduceCooldown();
        stun.reduceCooldown();
    }

    void upgradeSkill(Skill& skill) {
        if (coins >= 50 * skill.level) {
            coins -= 50 * skill.level;
            skill.upgrade();
            std::cout << "Навык " << skill.name << " улучшен до уровня " << skill.level << "!\n";
        }
        else {
            std::cout << "У вас недостаточно монет для улучшения навыка.\n";
        }
    }
};

// Класс монстра
class Monster {
public:
    std::string name;
    int health;
    int attack;
    int defense;
    int coins;
    int experience;

    Monster(std::string n, int h, int a, int d, int c, int exp) : name(n), health(h), attack(a), defense(d), coins(c), experience(exp) {}

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    bool isAlive() {
        return health > 0;
    }
};

//Функция для использлвания способности
void Player::useSkill(Skill& skill, Monster& monster) {
    if (skill.currentCooldown > 0) {
        std::cout << "Навык " << skill.name << " на перезарядке. Осталось ходов: " << skill.currentCooldown << "\n";
        return;
    }

    if (skill.name == "Мощный удар") {
        int damage = (attack * 2) - monster.defense;
        if (damage < 0) damage = 0;
        monster.takeDamage(damage);
        std::cout << "Вы использовали Мощный удар и нанесли " << damage << " урона!\n";
    }
    else if (skill.name == "Оглушение") {
        int stunChance = 50 + (skill.level - 1) * 10;
        if (std::rand() % 100 < stunChance) {
            monster.attack = 0;
            std::cout << "Вы оглушили монстра! Он пропустит следующий ход.\n";
        }
        else {
            std::cout << "Оглушение не сработало!\n";
        }
    }

    skill.currentCooldown = skill.cooldown;
}

// Функция для попытки убежать
bool tryToEscape() {
    std::srand(std::time(0));
    int chance = std::rand() % 100;
    return chance < 25;
}

//Функция для проведения боя
void battle(Player& player, Monster& monster) {
    std::cout << "Вы встретили монстра: " << monster.name << "!\n";
    while (player.isAlive() && monster.isAlive()) {
        std::cout << player.name << " (HP: " << player.health << ") vs " << monster.name << " (HP: " << monster.health << ")\n";
        std::cout << "1. Атаковать\n2. Использовать зелье здоровья\n3. Использовать навык\n4. Попытаться убежать\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            int damage = player.attack - monster.defense;
            if (damage < 0) damage = 0;
            monster.takeDamage(damage);
            std::cout << "Вы нанесли " << damage << " урона!\n";
        }
        else if (choice == 2) {
            std::cout << "Выберите зелье:\n";
            std::cout << "1. Малое зелье здоровья (" << player.inventory.getItemCount("Малое зелье здоровья") << " осталось)\n";
            std::cout << "2. Среднее зелье здоровья (" << player.inventory.getItemCount("Среднее зелье здоровья") << " осталось)\n";
            std::cout << "3. Большое зелье здоровья (" << player.inventory.getItemCount("Большое зелье здоровья") << " осталось)\n";
            int potionChoice;
            std::cin >> potionChoice;
            if (potionChoice == 1) {
                player.inventory.removeItem("Малое зелье здоровья");
                player.heal(20);
                std::cout << "Вы использовали малое зелье здоровья и восстановили 20 HP.\n";
            }
            else if (potionChoice == 2) {
                player.inventory.removeItem("Среднее зелье здоровья");
                player.heal(50);
                std::cout << "Вы использовали среднее зелье здоровья и восстановили 50 HP.\n";
            }
            else if (potionChoice == 3) {
                player.inventory.removeItem("Большое зелье здоровья");
                player.heal(100);
                std::cout << "Вы использовали большое зелье здоровья и восстановили 100 HP.\n";
            }
            else {
                std::cout << "Неверный выбор. Пропуск хода.\n";
            }
        }
        else if (choice == 3) {
            std::cout << "Выберите навык:\n";
            std::cout << "1. Мощный удар (Уровень: " << player.powerfulStrike.level << ", Кулдаун: " << player.powerfulStrike.currentCooldown << ")\n";
            std::cout << "2. Оглушение (Уровень: " << player.stun.level << ", Кулдаун: " << player.stun.currentCooldown << ")\n";
            int skillChoice;
            std::cin >> skillChoice;
            if (skillChoice == 1) {
                player.useSkill(player.powerfulStrike, monster);
            }
            else if (skillChoice == 2) {
                player.useSkill(player.stun, monster);
            }
            else {
                std::cout << "Неверный выбор. Пропуск хода.\n";
            }
        }
        else if (choice == 4) {
            if (tryToEscape()) {
                std::cout << "Вам удалось сбежать!\n";
                return;
            }
            else {
                std::cout << "Побег не удался! Монстр атакует вас.\n";
            }
        }
        else {
            std::cout << "Неверный выбор. Пропуск хода.\n";
        }

        if (monster.isAlive()) {
            int damage = monster.attack - player.defense;
            if (damage < 0) damage = 0;
            player.takeDamage(damage);
            std::cout << monster.name << " нанес вам " << damage << " урона!\n";
        }

        player.reduceCooldowns();
    }

    if (player.isAlive()) {
        std::cout << "Вы победили " << monster.name << "!\n";
        player.gainExperience(monster.experience);
        player.addCoins(monster.coins);
        std::cout << "Вы получили " << monster.coins << " монет и " << monster.experience << " опыта.\n";
    }
    else {
        std::cout << "Вы погибли...\n";
    }
}


int main() {
    setlocale(LC_ALL, "Rus");
    return 0;
}