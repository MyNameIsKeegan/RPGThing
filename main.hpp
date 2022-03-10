#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

sf::Font btnFont;
sf::Text batDisp("",btnFont,25); //temporary implementation, probably

const sf::Vector2f spriteSize (sf::Sprite spr) {return {spr.getTexture()->getSize().x * spr.getScale().x, spr.getTexture()->getSize().y * spr.getScale().y};};

enum inputState {overworld, combat, combatChoose, targetChoose};

class statusEffect {
    public:
    string name, desc;
    int atkmod = 0, defmod = 0, spdmod = 0, magicmod = 0, dur = 0;
    statusEffect(string name, string desc, int atkmod, int defmod, int spdmod, int magicmod, int dur):
    name(name), desc(desc), atkmod(atkmod), defmod(defmod), spdmod(spdmod), magicmod(magicmod), dur(dur) {}
};

class character {
    public:
    sf::Sprite spr;
    string name, desc;
    bool isActive = 1;
    int maxHP = 0, curHP = 0, atk = 0, def = 0, spd = 0, magic = 0, xp = 0, lvl = 0, tempatk = 0, tempdef = 0, tempspd = 0, tempmagic = 0;
    vector <statusEffect> statuses;
    character(): spr(), name("Name"), desc("Description"), atk(1), def(1), spd(1), magic(1), xp(0), lvl(1) {}
    character(string name, string desc, int maxHP, int curHP, int atk, int def, int spd, int magic, int xp, int lvl): 
    spr(), name(name), desc(desc), maxHP(maxHP), curHP(curHP), atk(atk), def(def), spd(spd), magic(magic), xp(xp), lvl(lvl) {}
    void upStat() {
        this->tempatk = 0, this->tempdef = 0, this->tempspd = 0, this->tempmagic = 0;
        for (const auto& i : this->statuses) {
            this->tempatk += i.atkmod;
            this->tempdef += i.defmod;
            this->tempspd += i.spdmod;
            this->tempmagic += i.magicmod;
        }
    }
    void startTurn() {
        this->statuses.erase(remove_if(this->statuses.begin(), this->statuses.end(), [](const statusEffect &status){return status.dur <= 0;}), this->statuses.end());
        for (auto& i : this->statuses) i.dur--;
        upStat();
    }
    bool IsAlive() {
        if (this->curHP<= 0) return 1; else return 0;
        //return this->curHP <= 0;
    }
    virtual void Attack(character* other) = 0;
    virtual void Defend() = 0;
    virtual void Flee(character* other) = 0;
};

class player : public character {
    public: //Some of the string conditionals could be ternary 
    player(string name, string desc, int maxHP, int curHP, int atk, int def, int spd, int magic, int xp, int lvl) : 
    character(name, desc, maxHP, curHP, atk, def, spd, magic, xp, lvl) {}
    void Attack(character* other) { //separate from character
        //test with different formulas
        int damage = (this->atk + this->tempatk) + (rand() % 4) - other->def - other->tempdef;
        if (damage < 0) damage = 0;
        //probably adjust this conditional later
        if (damage >= (this->atk + this->tempatk)*3) { //critical
            batDisp.setString(this->name+" has a real jam goin' now!");
            batDisp.setString(batDisp.getString()+"\n"+this->name+" absolutely slams "+other->name+"!");
        } else if (damage >= (this->atk + this->tempatk)*2) { //neutral
            batDisp.setString(this->name+" damages "+other->name+"!");
        } else if (damage >= this->atk + this->tempatk) { //bad
            batDisp.setString(this->name+" scratches "+other->name+".");
        } else if (damage == 0) { //miss
            batDisp.setString(this->name+" screwed it up"+"...");
        }
        batDisp.setString(batDisp.getString()+"\n"+this->name+" dealt "+to_string(damage)+" damage!");
        other->curHP -= damage;
        if (other->curHP < 0) other->curHP = 0;
    }
    void Defend() {
        statuses.emplace_back("Defending", this->name + " is ready for the enemy's attack!", 0, 1, 0, 0, 1);
        batDisp.setString(this->name+" is ready for the enemy's\nattack!");
        upStat();
    }
    void Flee(character* other) {
        batDisp.setString(this->name+" tried to flee...");
        //test with different rand values
        if (rand() % 10 + this->spd + this->tempspd > rand() % 8 + other->spd + other->tempspd) {
            batDisp.setString(batDisp.getString()+"\n"+this->name+" ran!");
        } else {
            batDisp.setString(batDisp.getString()+"\n"+this->name+" was too slow...");
        }
    }
    void LvlUp(int lvl) {
        int points = 5*lvl;
        while (points > 0) {
            cout << "You have " << points << " points to spend!" << "\n" << "1. Attack: " << this->atk << "\n" << "2. Defense: " << this->def << "\n" << "3. Speed: " << this->spd << "\n" << "4. Magic: " << this->magic << endl;
        }
    }
};

class enemy : public character {
    public:
    enemy(string name, string desc, int maxHP, int curHP, int atk, int def, int spd, int magic, int xp, int lvl) : 
    character(name, desc, maxHP, curHP, atk, def, spd, magic, xp, lvl) {}
    enemy(string name, string desc, int lvl) :
    character(name, desc, 10*lvl, 10*lvl, lvl, lvl, lvl, lvl, 10*lvl, lvl) {}
    void Attack(character* other) {
        //test with different formulas
        int damage = (this->atk + this->tempatk) * (rand() % 3) - other->def - other->tempdef;
        cout<<this->name<<" attacks "<<other->name<<"!"<<endl;
        if (damage >= (this->atk + this->tempatk)*3) {
            cout<<this->name<<" has a real jam goin' now!"<<endl;
            cout<<this->name<<" absolutely slams "<<other->name<<"!"<<endl;
        } else if (damage >= (this->atk + this->tempatk)*2) {
            cout<<this->name<<" damages "<<other->name<<"!"<<endl;
        } else if (damage >= this->atk + this->tempatk) {
            cout<<this->name<<" scratches "<<other->name<<"."<<endl;
        } else if (damage == 0) {
            cout<<this->name<<" screwed it up"<<"..."<<endl;
        }
        cout<<this->name<<" dealt "<<damage<<" damage!"<<endl;
        other->curHP -= damage;
        if (other->curHP < 0) other->curHP = 0;
    }
    void Defend() {
        statuses.emplace_back("Defending", this->name + " is ready for the enemy's attack!", 0, 1, 0, 0, 1);
        batDisp.setString(this->name+" is ready for the enemy's\nattack!");
        upStat();
    }
    void Flee(character* other) {
        batDisp.setString(this->name+" tried to flee...");
        //test with different rand values
        if (rand() % 10 + this->spd + this->tempspd > rand() % 8 + other->spd + other->tempspd) {
            batDisp.setString(batDisp.getString()+"\n"+this->name+" ran!");
        } else {
            batDisp.setString(batDisp.getString()+"\n"+this->name+" was too slow...");
        }
    }
};

class combatEncounter {
    public:
    vector <enemy*> enemies;
    combatEncounter() {}
    void addEnemy(enemy* thing) {
        this->enemies.emplace_back(thing);
    }
};