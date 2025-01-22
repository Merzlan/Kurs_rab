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

// Класс локации
class Location {
public:
    std::string name;
    std::string description;
    std::vector<std::string> monsters;

    Location(std::string n, std::string desc, std::vector<std::string> mons) : name(n), description(desc), monsters(mons) {}

    void showInfo() {
        std::cout << "Локация: " << name << "\n";
        std::cout << "Описание: " << description << "\n";
        std::cout << "Монстры: ";
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
                potionType = "Малое зелье здоровья";
            }
            else if (potionChance == 1) {
                potionType = "Среднее зелье здоровья";
            }
            else {
                potionType = "Большое зелье здоровья";
            }
            std::cout << "Вы нашли " << potionType << ". Оно добавлено в ваш инвентарь.\n";
            player.inventory.addItem(new HealthPotion(potionType, 0, potionChance == 0 ? 20 : (potionChance == 1 ? 50 : 100)));
            break;
        }
        case 1:
            std::cout << "Вы ничего не нашли.\n";
            break;
        default: {
            std::string monsterName = monsters[std::rand() % monsters.size()];
            Monster* monster = nullptr;
            if (monsterName == "Гоблин") {
                monster = new Monster("Гоблин", 30, 8, 2, 10, 20);
            }
            else if (monsterName == "Слайм") {
                monster = new Monster("Слайм", 20, 5, 1, 5, 15);
            }
            else if (monsterName == "Волк") {
                monster = new Monster("Волк", 25, 7, 3, 8, 18);
            }
            else if (monsterName == "Орк") {
                monster = new Monster("Орк", 50, 12, 5, 20, 40);
            }
            else if (monsterName == "Скелет") {
                monster = new Monster("Скелет", 40, 10, 4, 15, 35);
            }
            else if (monsterName == "Паук") {
                monster = new Monster("Паук", 45, 11, 6, 18, 38);
            }
            else if (monsterName == "Дракон") {
                monster = new Monster("Дракон", 100, 20, 10, 50, 100);
            }
            else if (monsterName == "Гигант") {
                monster = new Monster("Гигант", 90, 18, 8, 40, 90);
            }
            else if (monsterName == "Демон") {
                monster = new Monster("Демон", 95, 19, 9, 45, 95);
            }
            else if (monsterName == "Король Тьмы") {
                monster = new Monster("Король Тьмы", 200, 30, 15, 100, 500);
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

// Класс магазина
class Shop {
public:
    void showMenu(Player& player) {
        std::cout << "Добро пожаловать в магазин!\n";
        std::cout << "1. Купить малое зелье здоровья (10 монет)\n";
        std::cout << "2. Купить среднее зелье здоровья (25 монет)\n";
        std::cout << "3. Купить большое зелье здоровья (50 монет)\n";
        std::cout << "4. Улучшить навык\n";
        std::cout << "5. Выйти\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            if (player.coins >= 10) {
                player.spendCoins(10);
                player.inventory.addItem(new HealthPotion("Малое зелье здоровья", 10, 20));
                std::cout << "Вы купили малое зелье здоровья. Оно добавлено в ваш инвентарь.\n";
            }
            else {
                std::cout << "У вас недостаточно монет.\n";
            }
        }
        else if (choice == 2) {
            if (player.coins >= 25) {
                player.spendCoins(25);
                player.inventory.addItem(new HealthPotion("Среднее зелье здоровья", 25, 50));
                std::cout << "Вы купили среднее зелье здоровья. Оно добавлено в ваш инвентарь.\n";
            }
            else {
                std::cout << "У вас недостаточно монет.\n";
            }
        }
        else if (choice == 3) {
            if (player.coins >= 50) {
                player.spendCoins(50);
                player.inventory.addItem(new HealthPotion("Большое зелье здоровья", 50, 100));
                std::cout << "Вы купили большое зелье здоровья. Оно добавлено в ваш инвентарь.\n";
            }
            else {
                std::cout << "У вас недостаточно монет.\n";
            }
        }
        else if (choice == 4) {
            std::cout << "Выберите навык для улучшения:\n";
            std::cout << "1. Мощный удар (Уровень: " << player.powerfulStrike.level << ", Стоимость: " << 50 * player.powerfulStrike.level << " монет)\n";
            std::cout << "2. Оглушение (Уровень: " << player.stun.level << ", Стоимость: " << 50 * player.stun.level << " монет)\n";
            int skillChoice;
            std::cin >> skillChoice;
            if (skillChoice == 1) {
                player.upgradeSkill(player.powerfulStrike);
            }
            else if (skillChoice == 2) {
                player.upgradeSkill(player.stun);
            }
            else {
                std::cout << "Неверный выбор.\n";
            }
        }
        else if (choice == 5) {
            std::cout << "Вы вышли из магазина.\n";
        }
        else {
            std::cout << "Неверный выбор.\n";
        }
    }
};

void showStats(Player& player) {
    std::cout << "Имя: " << player.name << "\n";
    std::cout << "Уровень: " << player.level << "\n";
    std::cout << "Здоровье: " << player.health << "\n";
    std::cout << "Атака: " << player.attack << "\n";
    std::cout << "Защита: " << player.defense << "\n";
    std::cout << "Опыт: " << player.experience << "/" << player.getExperienceToNextLevel() << "\n";
    std::cout << "Монеты: " << player.coins << "\n";
    std::cout << "Зелья здоровья:\n";
    std::cout << "- Малое: " << player.inventory.getItemCount("Малое зелье здоровья") << "\n";
    std::cout << "- Среднее: " << player.inventory.getItemCount("Среднее зелье здоровья") << "\n";
    std::cout << "- Большое: " << player.inventory.getItemCount("Большое зелье здоровья") << "\n";
    std::cout << "Навыки:\n";
    std::cout << "- Мощный удар (Уровень: " << player.powerfulStrike.level << ", Кулдаун: " << player.powerfulStrike.currentCooldown << ")\n";
    std::cout << "- Оглушение (Уровень: " << player.stun.level << ", Кулдаун: " << player.stun.currentCooldown << ")\n";
}


int main() {
    setlocale(LC_ALL, "Rus");
    std::string playerName;
    std::cout << "Введите имя вашего персонажа: ";
    std::cin >> playerName;

    Player player(playerName);
    std::cout << "Добро пожаловать в текстовую RPG, " << player.name << "!\n";

    std::vector<Location> locations = {
        Location("Лес Теней", "Мрачный лес, полный опасных существ. Здесь легко заблудиться.", {"Гоблин", "Слайм", "Волк"}),
        Location("Горы Ужаса", "Высокие горы, где обитают сильные и опасные монстры.", {"Орк", "Скелет", "Паук"}),
        Location("Лабиринт Тьмы", "Загадочный лабиринт, где скрываются могущественные существа.", {"Дракон", "Гигант", "Демон"}),
        Location("Тронный Зал Короля Тьмы", "Финальная локация, где вас ждет битва с Королем Тьмы.", {"Король Тьмы"})
    };

    Shop shop;
    int currentLocation = 0;
    while (player.isAlive()) {
        locations[currentLocation].showInfo();
        std::cout << "1. Исследовать\n2. Посмотреть характеристики\n3. Магазин\n4. Перейти в следующую локацию\n5. Получить бесплатные монеты\n6. Просмотреть инвентарь\n7. Выйти\n";
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
                std::cout << "Вы перешли в локацию 2!\n";
            }
            else if (currentLocation == 1 && player.level >= 3) {
                currentLocation = 2;
                std::cout << "Вы перешли в локацию 3!\n";
            }
            else if (currentLocation == 2 && player.level >= 4) {
                currentLocation = 3;
                std::cout << "Вы перешли в локацию босса! Приготовьтесь к финальной битве!\n";
            }
            else {
                std::cout << "Вы не можете перейти в следующую локацию. Требуется более высокий уровень.\n";
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
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }

        // Проверка на победу над боссом
        if (currentLocation == 3 && player.level >= 4) {
            Monster boss("Король Тьмы", 200, 30, 15, 100, 500);
            battle(player, boss);
            if (player.isAlive()) {
                std::cout << "Поздравляем! Вы победили Короля Тьмы и завершили игру!\n";
                break;
            }
            else {
                std::cout << "Вы погибли в битве с Королем Тьмы...\n";
                break;
            }
        }
    }

    std::cout << "Игра завершена. Спасибо за игру!\n";
    return 0;
}