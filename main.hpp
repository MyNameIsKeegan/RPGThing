#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class statusEffect {
public:
    string name, desc;
    int atkmod, defmod, spdmod, magicmod, dur = 0;
    statusEffect(string name, string desc, int atkmod, int defmod, int spdmod, int magicmod, int dur):
    name(name), desc(desc), atkmod(atkmod), defmod(defmod), spdmod(spdmod), 
    magicmod(magicmod), dur(dur) {}
};

class character {
    public:
    sf::Sprite spr;
    sf::RectangleShape hpR;
    sf::RectangleShape hpG;
    sf::Text hpT;
    string name, desc;
    int totalHP, curHP, atk, def, spd, magic, xp, lvl, tempatk, tempdef, tempspd, tempmagic = 0;
    vector <statusEffect> statuses;
    character() : name("Name"), desc("Description"), atk(1), def(1), spd(1), magic(1), xp(0), 
    lvl(1), spr(), hpR({200, 25}), hpG({200, 25}), hpT() {
        hpR.setFillColor(sf::Color::Red);
        hpR.setOutlineColor(sf::Color::Transparent);
        hpG.setFillColor(sf::Color::Green);
        hpG.setOutlineColor(sf::Color::Transparent);
        hpT.setCharacterSize(25);
        hpT.setOutlineThickness(2);
        UpHP();
    };
    character(string name, string desc, int totalHP, int curHP, int atk, int def, int spd, int magic, int xp, int lvl): 
    name(name), desc(desc), totalHP(totalHP), 
    curHP(curHP), atk(atk), def(def), spd(spd), magic(magic), xp(xp), lvl(lvl), 
    hpR({200, 25}), hpG({200, 25}), hpT() {
        hpR.setFillColor(sf::Color::Red);
        hpR.setOutlineColor(sf::Color::Transparent);
        hpG.setFillColor(sf::Color::Green);
        hpG.setOutlineColor(sf::Color::Transparent);
        hpT.setCharacterSize(25);
        hpT.setOutlineThickness(2);
        UpHP();
    };
    void startTurn() {
        tempatk, tempdef, tempspd, tempmagic = 0;
        for (int i = statuses.size()-1; i >= 0; i--) {
            statuses[i].dur--;
            if (statuses[i].dur <= 0) {
                statuses.erase(statuses.begin() + i);
            }
            else {
                tempatk += statuses[i].atkmod;
                tempdef += statuses[i].defmod;
                tempspd += statuses[i].spdmod;
                tempmagic += statuses[i].magicmod;
            }
        }
    }
    void endTurn() {
        tempatk, tempdef, tempspd, tempmagic = 0;
        for (int i = statuses.size() - 1; i >= 0; i--) {
            if (statuses[i].dur <= 0) {
                statuses.erase(statuses.begin() + i);
            }
            else {
                tempatk += statuses[i].atkmod;
                tempdef += statuses[i].defmod;
                tempspd += statuses[i].spdmod;
                tempmagic += statuses[i].magicmod;
            }
        }
    }
    void DescChar() {
        cout<<"Name: "+this->name<<endl;
        cout<<this->desc<<endl;
    }
    void UpHP() { //Updates HP display text & HP bar.
        hpT.setString(to_string(this->curHP)+" / "+to_string(this->totalHP));
        hpG.setScale((float)this->curHP/this->totalHP, 1.f); //needs explicit convert
    }
    bool IsAlive() {
        if (this->curHP <= 0) return 0; else return 1;
    }
    virtual void Attack(character *other) = 0;
    virtual void Defend() = 0;
    virtual void Flee() = 0;
};

class player : public character {
    public:
    player(string name, string desc, int totalHP, int curHP, int atk, int def, int spd, int magic, int xp, int lvl) : 
    character(name, desc, totalHP, curHP, atk, def, spd, magic, xp, lvl) {};
    void Attack(character* other) {
        int damage = (this->atk + this->tempatk) + (rand() % 4) - other->def - other->tempdef;
        if (damage < 0) damage = 0;
        cout<<this->name<<" attacks "<<other->name<<"!"<<endl;
        if (damage >= (this->atk + this->tempatk)*3) {
            cout<<this->name<<" has a real jam goin' now!"<<endl;
            cout<<this->name<<" absolutely slams "<<other->name<<"!"<<endl;
        } else if (damage >= (this->atk + this->tempatk)*2) {
            cout<<this->name<<" damages "<<other->name<<"!"<<endl;
        } else if (damage >= this->atk + this->tempatk) {
            cout<<this->name<<" scratches "<<other->name<<"."<<endl;
        } else if (damage = 0) {
            cout<<this->name<<" screwed it up"<<"..."<<endl;
        }
        cout<<this->name<<" dealt "<<damage<<" damage!"<<endl;
        other->curHP -= damage;
        if (other->curHP < 0) other->curHP = 0;
        other->UpHP();
    }
    void Defend() {
        statuses.emplace_back("Defending", this->name + " is ready for the enemy's attack!", 0, 1, 0, 0, 1);
        cout << this->name << "is ready for the enemy's attack!" << endl;
    }
    void Flee() {
        cout << this->name << " tried to flee..." << endl;
        //eventually calculate against enemy stat
        if (rand() % 10 + this->spd + this->tempspd > 5) {
            cout << this->name << " ran!" << endl;
        } else {
            cout << this->name << " was too slow..." << endl;
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
    enemy(string name, string desc, int totalHP, int curHP, int atk, int def, int spd, int magic, int xp, int lvl) : 
    character(name, desc, totalHP, curHP, atk, def, spd, magic, xp, lvl){};
    enemy(string name, string desc, int lvl) :
    character(name, desc, 10*lvl, 10*lvl, lvl, lvl, lvl, lvl, 10*lvl, lvl){};
    void Attack(character* other) {
        int damage = (this->atk + this->tempatk) * (rand() % 3) - other->def - other->tempdef;
        cout<<this->name<<" attacks "<<other->name<<"!"<<endl;
        if (damage >= (this->atk + this->tempatk)*3) {
            cout<<this->name<<" has a real jam goin' now!"<<endl;
            cout<<this->name<<" absolutely slams "<<other->name<<"!"<<endl;
        } else if (damage >= (this->atk + this->tempatk)*2) {
            cout<<this->name<<" damages "<<other->name<<"!"<<endl;
        } else if (damage >= this->atk + this->tempatk) {
            cout<<this->name<<" scratches "<<other->name<<"."<<endl;
        } else if (damage = 0) {
            cout<<this->name<<" screwed it up"<<"..."<<endl;
        }
        cout<<this->name<<" dealt "<<damage<<" damage!"<<endl;
        other->curHP -= damage;
        if (other->curHP < 0) other->curHP = 0;
        other->UpHP();
    }
    void Defend() {
        cout << this->name << "is ready for the enemy's attack!" << endl;
    }
    void Flee() {
        cout << this->name << " tried to flee..." << endl;
        //eventually calculate against enemy stat
        if (rand() % 10 + this->spd + this->tempspd > 5) {
            cout << this->name << " ran!" << endl;
        }
        else {
            cout << this->name << " was too slow..." << endl;
        }
    }
};