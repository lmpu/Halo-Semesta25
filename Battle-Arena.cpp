#include <bits/stdc++.h>
using namespace std;

class Character {
protected:
    string name;
    int health;
    int attack;
    int defense;
    int mana;

public:
    Character(string n, int h, int a, int d, int m)
        : name(n), health(h), attack(a), defense(d), mana(m) {}

    virtual ~Character() {}

    string getName() { return name; }
    int getHealth() { return health; }
    int getMana() { return mana; }
    bool isAlive() { return health > 0; }

    void takeDamage(int dmg) {
        int actualDmg = max(dmg - defense, 0);
        health -= actualDmg;
        cout << name << " takes " << actualDmg << " damage! Health: " << health << endl;
    }

    void heal(int amount) {
        health += amount;
        cout << name << " heals " << amount << " HP. Current Health: " << health << endl;
    }

    virtual void performAction(Character& target) = 0; // to be implemented by derived classes
};

class Warrior : public Character {
public:
    Warrior(string n) : Character(n, 120, 25, 10, 20) {}

    void performAction(Character& target) override {
        int choice = rand() % 2; // 0 = attack, 1 = special
        if (choice == 0 || mana < 10) {
            // Normal attack
            int crit = (rand() % 100 < 20) ? 2 : 1; // 20% chance for critical
            cout << name << " attacks " << target.getName() << "!\n";
            target.takeDamage(attack * crit);
            if (crit == 2) cout << "Critical Hit!\n";
        } else {
            // Special ability: Power Strike
            cout << name << " uses Power Strike!\n";
            target.takeDamage(attack + 20);
            mana -= 10;
            cout << name << "'s mana left: " << mana << endl;
        }
    }
};

class Mage : public Character {
public:
    Mage(string n) : Character(n, 80, 15, 5, 50) {}

    void performAction(Character& target) override {
        int choice = rand() % 2; // 0 = attack, 1 = spell
        if (choice == 0 || mana < 15) {
            cout << name << " attacks " << target.getName() << "!\n";
            target.takeDamage(attack);
        } else {
            cout << name << " casts Fireball!\n";
            target.takeDamage(30);
            mana -= 15;
            cout << name << "'s mana left: " << mana << endl;
        }
    }
};

class BattleArena {
    vector<Character*> players;
    vector<string> battleLog;

public:
    void addPlayer(Character* c) { players.push_back(c); }

    void startBattle() {
        cout << "Battle Start!\n";
        int turn = 0;
        while (aliveCount() > 1) {
            Character* attacker = players[turn % players.size()];
            if (!attacker->isAlive()) { turn++; continue; }

            // Choose random target
            Character* target = nullptr;
            do {
                target = players[rand() % players.size()];
            } while (target == attacker || !target->isAlive());

            attacker->performAction(*target);

            if (!target->isAlive())
                battleLog.push_back(target->getName() + " has been defeated!");

            turn++;
            cout << "---------------------------------\n";
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        for (auto p : players)
            if (p->isAlive())
                cout << p->getName() << " wins the battle!\n";

        cout << "\nBattle Log:\n";
        for (auto &log : battleLog) cout << log << endl;
    }

private:
    int aliveCount() {
        int count = 0;
        for (auto p : players) if (p->isAlive()) count++;
        return count;
    }
};

int main() {
    srand(time(0));
    BattleArena arena;

    Warrior* w1 = new Warrior("Thor");
    Mage* m1 = new Mage("Merlin");
    Warrior* w2 = new Warrior("Conan");
    Mage* m2 = new Mage("Gandalf");

    arena.addPlayer(w1);
    arena.addPlayer(m1);
    arena.addPlayer(w2);
    arena.addPlayer(m2);

    arena.startBattle();

    // Clean up
    delete w1; delete m1; delete w2; delete m2;

    return 0;
}
