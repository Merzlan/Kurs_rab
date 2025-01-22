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
            std::cout << "��� ��������� ����.\n";
        }
        else {
            std::cout << "��� ���������:\n";
            for (const auto& item : items) {
                std::cout << "- " << item->name << "\n";
            }
        }
    }
};

// ����� �����������
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

// ����� ������
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
        powerfulStrike("������ ����", 1, 3), stun("���������", 1, 5) {}

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
        std::cout << "�� �������� ������ " << level << "! ���� �������������� ��������.\n";
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
            std::cout << "�� �������� 10 �����. �������� �������: " << 3 - freeCoinsUsed << "\n";
            return true;
        }
        else {
            std::cout << "�� ��������� ����� ���������� �����.\n";
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
            std::cout << "����� " << skill.name << " ������� �� ������ " << skill.level << "!\n";
        }
        else {
            std::cout << "� ��� ������������ ����� ��� ��������� ������.\n";
        }
    }
};

// ����� �������
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

//������� ��� ������������� �����������
void Player::useSkill(Skill& skill, Monster& monster) {
    if (skill.currentCooldown > 0) {
        std::cout << "����� " << skill.name << " �� �����������. �������� �����: " << skill.currentCooldown << "\n";
        return;
    }

    if (skill.name == "������ ����") {
        int damage = (attack * 2) - monster.defense;
        if (damage < 0) damage = 0;
        monster.takeDamage(damage);
        std::cout << "�� ������������ ������ ���� � ������� " << damage << " �����!\n";
    }
    else if (skill.name == "���������") {
        int stunChance = 50 + (skill.level - 1) * 10;
        if (std::rand() % 100 < stunChance) {
            monster.attack = 0;
            std::cout << "�� �������� �������! �� ��������� ��������� ���.\n";
        }
        else {
            std::cout << "��������� �� ���������!\n";
        }
    }

    skill.currentCooldown = skill.cooldown;
}

// ������� ��� ������� �������
bool tryToEscape() {
    std::srand(std::time(0));
    int chance = std::rand() % 100;
    return chance < 25;
}

//������� ��� ���������� ���
void battle(Player& player, Monster& monster) {
    std::cout << "�� ��������� �������: " << monster.name << "!\n";
    while (player.isAlive() && monster.isAlive()) {
        std::cout << player.name << " (HP: " << player.health << ") vs " << monster.name << " (HP: " << monster.health << ")\n";
        std::cout << "1. ���������\n2. ������������ ����� ��������\n3. ������������ �����\n4. ���������� �������\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            int damage = player.attack - monster.defense;
            if (damage < 0) damage = 0;
            monster.takeDamage(damage);
            std::cout << "�� ������� " << damage << " �����!\n";
        }
        else if (choice == 2) {
            std::cout << "�������� �����:\n";
            std::cout << "1. ����� ����� �������� (" << player.inventory.getItemCount("����� ����� ��������") << " ��������)\n";
            std::cout << "2. ������� ����� �������� (" << player.inventory.getItemCount("������� ����� ��������") << " ��������)\n";
            std::cout << "3. ������� ����� �������� (" << player.inventory.getItemCount("������� ����� ��������") << " ��������)\n";
            int potionChoice;
            std::cin >> potionChoice;
            if (potionChoice == 1) {
                player.inventory.removeItem("����� ����� ��������");
                player.heal(20);
                std::cout << "�� ������������ ����� ����� �������� � ������������ 20 HP.\n";
            }
            else if (potionChoice == 2) {
                player.inventory.removeItem("������� ����� ��������");
                player.heal(50);
                std::cout << "�� ������������ ������� ����� �������� � ������������ 50 HP.\n";
            }
            else if (potionChoice == 3) {
                player.inventory.removeItem("������� ����� ��������");
                player.heal(100);
                std::cout << "�� ������������ ������� ����� �������� � ������������ 100 HP.\n";
            }
            else {
                std::cout << "�������� �����. ������� ����.\n";
            }
        }
        else if (choice == 3) {
            std::cout << "�������� �����:\n";
            std::cout << "1. ������ ���� (�������: " << player.powerfulStrike.level << ", �������: " << player.powerfulStrike.currentCooldown << ")\n";
            std::cout << "2. ��������� (�������: " << player.stun.level << ", �������: " << player.stun.currentCooldown << ")\n";
            int skillChoice;
            std::cin >> skillChoice;
            if (skillChoice == 1) {
                player.useSkill(player.powerfulStrike, monster);
            }
            else if (skillChoice == 2) {
                player.useSkill(player.stun, monster);
            }
            else {
                std::cout << "�������� �����. ������� ����.\n";
            }
        }
        else if (choice == 4) {
            if (tryToEscape()) {
                std::cout << "��� ������� �������!\n";
                return;
            }
            else {
                std::cout << "����� �� ������! ������ ������� ���.\n";
            }
        }
        else {
            std::cout << "�������� �����. ������� ����.\n";
        }

        if (monster.isAlive()) {
            int damage = monster.attack - player.defense;
            if (damage < 0) damage = 0;
            player.takeDamage(damage);
            std::cout << monster.name << " ����� ��� " << damage << " �����!\n";
        }

        player.reduceCooldowns();
    }

    if (player.isAlive()) {
        std::cout << "�� �������� " << monster.name << "!\n";
        player.gainExperience(monster.experience);
        player.addCoins(monster.coins);
        std::cout << "�� �������� " << monster.coins << " ����� � " << monster.experience << " �����.\n";
    }
    else {
        std::cout << "�� �������...\n";
    }
}

// ����� �������
class Location {
public:
    std::string name;
    std::string description;
    std::vector<std::string> monsters;

    Location(std::string n, std::string desc, std::vector<std::string> mons) : name(n), description(desc), monsters(mons) {}

    void showInfo() {
        std::cout << "�������: " << name << "\n";
        std::cout << "��������: " << description << "\n";
        std::cout << "�������: ";
        for (const auto& monster : monsters) {
            std::cout << monster << " ";
        }
        std::cout << "\n";
    }

    void explore(Player& player) {
        std::srand(std::time(0));
        int encounter = std::rand() % 4;

        switch (encounter) {
        case 0: {
            std::string potionType;
            int potionChance = std::rand() % 3;
            if (potionChance == 0) {
                potionType = "����� ����� ��������";
            }
            else if (potionChance == 1) {
                potionType = "������� ����� ��������";
            }
            else {
                potionType = "������� ����� ��������";
            }
            std::cout << "�� ����� " << potionType << ". ��� ��������� � ��� ���������.\n";
            player.inventory.addItem(new HealthPotion(potionType, 0, potionChance == 0 ? 20 : (potionChance == 1 ? 50 : 100)));
            break;
        }
        case 1:
            std::cout << "�� ������ �� �����.\n";
            break;
        default: {
            std::string monsterName = monsters[std::rand() % monsters.size()];
            Monster* monster = nullptr;
            if (monsterName == "������") {
                monster = new Monster("������", 30, 8, 2, 10, 20);
            }
            else if (monsterName == "�����") {
                monster = new Monster("�����", 20, 5, 1, 5, 15);
            }
            else if (monsterName == "����") {
                monster = new Monster("����", 25, 7, 3, 8, 18);
            }
            else if (monsterName == "���") {
                monster = new Monster("���", 50, 12, 5, 20, 40);
            }
            else if (monsterName == "������") {
                monster = new Monster("������", 40, 10, 4, 15, 35);
            }
            else if (monsterName == "����") {
                monster = new Monster("����", 45, 11, 6, 18, 38);
            }
            else if (monsterName == "������") {
                monster = new Monster("������", 100, 20, 10, 50, 100);
            }
            else if (monsterName == "������") {
                monster = new Monster("������", 90, 18, 8, 40, 90);
            }
            else if (monsterName == "�����") {
                monster = new Monster("�����", 95, 19, 9, 45, 95);
            }
            else if (monsterName == "������ ����") {
                monster = new Monster("������ ����", 200, 30, 15, 100, 500);
            }

            if (monster) {
                battle(player, *monster);
                delete monster;
            }
            break;
        }
        }
    }
};

// ����� ��������
class Shop {
public:
    void showMenu(Player& player) {
        std::cout << "����� ���������� � �������!\n";
        std::cout << "1. ������ ����� ����� �������� (10 �����)\n";
        std::cout << "2. ������ ������� ����� �������� (25 �����)\n";
        std::cout << "3. ������ ������� ����� �������� (50 �����)\n";
        std::cout << "4. �������� �����\n";
        std::cout << "5. �����\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            if (player.coins >= 10) {
                player.spendCoins(10);
                player.inventory.addItem(new HealthPotion("����� ����� ��������", 10, 20));
                std::cout << "�� ������ ����� ����� ��������. ��� ��������� � ��� ���������.\n";
            }
            else {
                std::cout << "� ��� ������������ �����.\n";
            }
        }
        else if (choice == 2) {
            if (player.coins >= 25) {
                player.spendCoins(25);
                player.inventory.addItem(new HealthPotion("������� ����� ��������", 25, 50));
                std::cout << "�� ������ ������� ����� ��������. ��� ��������� � ��� ���������.\n";
            }
            else {
                std::cout << "� ��� ������������ �����.\n";
            }
        }
        else if (choice == 3) {
            if (player.coins >= 50) {
                player.spendCoins(50);
                player.inventory.addItem(new HealthPotion("������� ����� ��������", 50, 100));
                std::cout << "�� ������ ������� ����� ��������. ��� ��������� � ��� ���������.\n";
            }
            else {
                std::cout << "� ��� ������������ �����.\n";
            }
        }
        else if (choice == 4) {
            std::cout << "�������� ����� ��� ���������:\n";
            std::cout << "1. ������ ���� (�������: " << player.powerfulStrike.level << ", ���������: " << 50 * player.powerfulStrike.level << " �����)\n";
            std::cout << "2. ��������� (�������: " << player.stun.level << ", ���������: " << 50 * player.stun.level << " �����)\n";
            int skillChoice;
            std::cin >> skillChoice;
            if (skillChoice == 1) {
                player.upgradeSkill(player.powerfulStrike);
            }
            else if (skillChoice == 2) {
                player.upgradeSkill(player.stun);
            }
            else {
                std::cout << "�������� �����.\n";
            }
        }
        else if (choice == 5) {
            std::cout << "�� ����� �� ��������.\n";
        }
        else {
            std::cout << "�������� �����.\n";
        }
    }
};

void showStats(Player& player) {
    std::cout << "���: " << player.name << "\n";
    std::cout << "�������: " << player.level << "\n";
    std::cout << "��������: " << player.health << "\n";
    std::cout << "�����: " << player.attack << "\n";
    std::cout << "������: " << player.defense << "\n";
    std::cout << "����: " << player.experience << "/" << player.getExperienceToNextLevel() << "\n";
    std::cout << "������: " << player.coins << "\n";
    std::cout << "����� ��������:\n";
    std::cout << "- �����: " << player.inventory.getItemCount("����� ����� ��������") << "\n";
    std::cout << "- �������: " << player.inventory.getItemCount("������� ����� ��������") << "\n";
    std::cout << "- �������: " << player.inventory.getItemCount("������� ����� ��������") << "\n";
    std::cout << "������:\n";
    std::cout << "- ������ ���� (�������: " << player.powerfulStrike.level << ", �������: " << player.powerfulStrike.currentCooldown << ")\n";
    std::cout << "- ��������� (�������: " << player.stun.level << ", �������: " << player.stun.currentCooldown << ")\n";
}


int main() {
    setlocale(LC_ALL, "Rus");
    std::string playerName;
    std::cout << "������� ��� ������ ���������: ";
    std::cin >> playerName;

    Player player(playerName);
    std::cout << "����� ���������� � ��������� RPG, " << player.name << "!\n";

    std::vector<Location> locations = {
        Location("��� �����", "������� ���, ������ ������� �������. ����� ����� �����������.", {"������", "�����", "����"}),
        Location("���� �����", "������� ����, ��� ������� ������� � ������� �������.", {"���", "������", "����"}),
        Location("�������� ����", "���������� ��������, ��� ���������� �������������� ��������.", {"������", "������", "�����"}),
        Location("������� ��� ������ ����", "��������� �������, ��� ��� ���� ����� � ������� ����.", {"������ ����"})
    };

    Shop shop;
    int currentLocation = 0;
    while (player.isAlive()) {
        locations[currentLocation].showInfo();
        std::cout << "1. �����������\n2. ���������� ��������������\n3. �������\n4. ������� � ��������� �������\n5. �������� ���������� ������\n6. ����������� ���������\n7. �����\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            locations[currentLocation].explore(player);
        }
        else if (choice == 2) {
            showStats(player);
        }
        else if (choice == 3) {
            shop.showMenu(player);
        }
        else if (choice == 4) {
            if (currentLocation == 0 && player.level >= 2) {
                currentLocation = 1;
                std::cout << "�� ������� � ������� 2!\n";
            }
            else if (currentLocation == 1 && player.level >= 3) {
                currentLocation = 2;
                std::cout << "�� ������� � ������� 3!\n";
            }
            else if (currentLocation == 2 && player.level >= 4) {
                currentLocation = 3;
                std::cout << "�� ������� � ������� �����! ������������� � ��������� �����!\n";
            }
            else {
                std::cout << "�� �� ������ ������� � ��������� �������. ��������� ����� ������� �������.\n";
            }
        }
        else if (choice == 5) {
            player.getFreeCoins();
        }
        else if (choice == 6) {
            player.inventory.showInventory();
        }
        else if (choice == 7) {
            break;
        }
        else {
            std::cout << "�������� �����. ���������� �����.\n";
        }

        // �������� �� ������ ��� ������
        if (currentLocation == 3 && player.level >= 4) {
            Monster boss("������ ����", 200, 30, 15, 100, 500);
            battle(player, boss);
            if (player.isAlive()) {
                std::cout << "�����������! �� �������� ������ ���� � ��������� ����!\n";
                break;
            }
            else {
                std::cout << "�� ������� � ����� � ������� ����...\n";
                break;
            }
        }
    }

    std::cout << "���� ���������. ������� �� ����!\n";
    return 0;
}