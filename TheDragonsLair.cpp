#include <iostream>
#include <string>
#include <vector>
#include <random>

using namespace std;

// Item class
class Item {
public:
    string name;
    int attackBonus;
    int defenseBonus;
    int healthBonus;

    Item(string name, int attackBonus, int defenseBonus, int healthBonus) {
        this->name = name;
        this->attackBonus = attackBonus;
        this->defenseBonus = defenseBonus;
        this->healthBonus = healthBonus;
    }
};

// Character class
class Character {
public:
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int experience;
    int level;
    vector<Item> inventory;

    Character(string name) {
        this->name = name;
        this->health = 100;
        this->maxHealth = 100;
        this->attack = 10;
        this->defense = 5;
        this->experience = 0;
        this->level = 1;
    }

    void levelUp() {
        level++;
        maxHealth += 10;
        health = maxHealth;
        attack += 2;
        defense += 1;
    }

    void useItem(Item item) {
        health += item.healthBonus;
        attack += item.attackBonus;
        defense += item.defenseBonus;
        inventory.erase(find(inventory.begin(), inventory.end(), item));
    }

    void attack(Character& target) {
        int damage = attack - target.defense;
        target.health -= max(damage, 0);
        cout << name << " attacks " << target.name << " for " << max(damage, 0) << " damage.\n";
    }
};

// Enemy class
class Enemy : public Character {
public:
    Enemy(string name, int health, int attack, int defense) {
        this->name = name;
        this->health = health;
        this->maxHealth = health;
        this->attack = attack;
        this->defense = defense;
    }
};

// Environment class
class Environment {
public:
    string description;
    vector<Enemy> enemies;
    vector<Item> items;

    Environment(string description) {
        this->description = description;
    }

    void encounter(Character& player) {
        if (!enemies.empty()) {
            Enemy enemy = enemies.back();
            enemies.pop_back();
            cout << "You encounter a " << enemy.name << "!\n";
            while (player.health > 0 && enemy.health > 0) {
                player.attack(enemy);
                if (enemy.health <= 0) {
                    cout << "You defeated the " << enemy.name << "!\n";
                    player.experience += enemy.level * 10;
                    if (player.experience >= player.level * 100) {
                        player.levelUp();
                        cout << player.name << " leveled up to level " << player.level << "!\n";
                    }
                    break;
                }
                enemy.attack(player);
                if (player.health <= 0) {
                    cout << "You were defeated by the " << enemy.name << ".\n";
                    break;
                }
            }
        } else {
            cout << "The area is empty.\n";
        }
    }
};

// Game class
class Game {
public:
    Character player;
    Environment currentEnvironment;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> distrib;

    Game() : gen(rd()), distrib(1, 100) {}

    void start() {
        string playerName;
        cout << "Enter your name: ";
        getline(cin, playerName);
        player = Character(playerName);

        currentEnvironment = Environment("The Dragon's Lair");
        currentEnvironment.enemies.push_back(Enemy("Dragon", 100, 20, 10));
        currentEnvironment.items.push_back(Item("Sword of Might", 5, 0, 0));
        currentEnvironment.items.push_back(Item("Shield of Protection", 0, 5, 0));
        currentEnvironment.items.push_back(Item("Potion of Healing", 0, 0, 20));

        cout << "Welcome, " << player.name << "!\n";
        cout << "You are a brave adventurer, ready to face the Dragon's Lair.\n";
    }

    void update() {
        currentEnvironment.encounter(player);
        if (player.health <= 0) {
            end();
        }
    }

    void render() {
        cout << "Your health: " << player.health << "/" << player.maxHealth << endl;
        cout << "Your attack: " << player.attack << endl;
        cout << "Your defense: " << player.defense << endl;
        cout << "Your level: " << player.level << endl;
        cout << "Your inventory:\n";
        for (Item item : player.inventory) {
            cout << "- " << item.name << endl;
        }
        cout << endl;
    }

    void end() {
        cout << "Game Over.\n";
    }
};

int main() {
    Game game;
    game.start();

    while (!game.isOver()) {
        game.update();
        game.render();
    }

    game.end();

    return 0;
}
