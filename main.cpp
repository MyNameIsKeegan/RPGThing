#include "main.hpp"
#include "graphic.hpp"

int main() {
    int winx = 800, winy = 800;
    sf::RenderWindow window(sf::VideoMode(winx,winy), "Name");
    sf::Texture dude;
    sf::Texture baddude;
    menu* activeMenu = nullptr;
    menu* battle = new menu;
    menu* battleChoose = new menu;
    menu* targetMenu = new menu;
    srand(time(NULL));
    player *activePlayer = nullptr;
    player *user = new player("Fennel", "This is a person.", 10, 10, 1, 1, 1, 1, 0, 1);
    player *user2 = new player("Friend Dummy", "They're a dummy.", 10, 10, 1, 1, 1, 1, 0, 1);
    enemy *activeEnemy = nullptr;
    enemy *badguy = new enemy("Bad Dude", "Not a good dude.", 1);
    enemy *badguy2 = new enemy("Cardboard Cutout Slime", "Is this a joke?", 1);
    window.setVerticalSyncEnabled(true);

    inputState curState;
    bool combatSet = 0;
    bool refreshCombat = 0;
    vector <bar*> hpBars;
    vector <bar*> hpBarsE;

    vector <player*> players = {user, user2};
    vector <enemy*> enemies;
    combatEncounter* test = new combatEncounter;
    test->addEnemy(badguy);
    test->addEnemy(badguy2);
    enemies = test->enemies;

    activeMenu = battleChoose;
    curState = combatChoose;

    //Asset loading
    //Fonts
    btnFont.loadFromFile("Assets/Manjari-Thin.otf");
    //Textures
    dude.loadFromFile("Assets/FennelIdleBack.png");
    baddude.loadFromFile("Assets/Slime.png");

    //Character sprite setup
    user->spr.setTexture(dude);
    user2->spr.setTexture(dude);
    badguy->spr.setTexture(baddude);                                                                                 
    badguy2->spr.setTexture(baddude);

    //Battle output display
    batDisp.setPosition({400, 400});
    batDisp.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        if (curState == combat || curState == combatChoose) {
            if (!combatSet) {
                battle->btnmap.emplace_back(new button("Attack!", 100, 50, 150, 700, 234, 32, 26));
                battle->btnmap.emplace_back(new button("Defend.", 100, 50, 355, 700, 121, 227, 241));
                battle->btnmap.emplace_back(new button("Flee...", 100, 50, 550, 700, 255, 254, 122));
                for (size_t i = 0; i < battle->btnmap.size(); i++) {
                    battle->btnmap[i]->btn.setOutlineThickness(2);
                    battle->btnmap[i]->label.setFont(btnFont);
                    battle->btnmap[i]->label.setCharacterSize(25);
                    battle->btnmap[i]->link('r', battle->btnmap[i < battle->btnmap.size()-1 ? i+1 : 0]);
                }
                battle->activeBtn = battle->btnmap[0];
                battle->activeBtn->SetActive();
                for (size_t i = 0; i < players.size(); i++) {
                    hpBars.emplace_back(new bar(200, 25, 255, 0, 0, 0, 255, 0));
                    players[i]->spr.setScale(6, 6);
                    players[i]->spr.setPosition({(float)winx/(players.size()+1)/2*(i*2+2)-spriteSize(players[i]->spr).x/2,558});
                    hpBars[i]->base.setPosition({players[i]->spr.getPosition().x+spriteSize(players[i]->spr).x/2-100.f, players[i]->spr.getPosition().y+100.f});
                    hpBars[i]->fill.setPosition(hpBars[i]->base.getPosition());
                    hpBars[i]->label.setFont(btnFont);
                    hpBars[i]->label.setPosition(hpBars[i]->base.getPosition().x+10.f, hpBars[i]->base.getPosition().y-5.f);
                    hpBars[i]->update(players[i]->curHP, players[i]->maxHP);
                    battleChoose->btnmap.emplace_back(new button(to_string(i), spriteSize(players[i]->spr), players[i]->spr.getPosition(), 107, 88, 69));
                }
                for (size_t i = 0; i < battleChoose->btnmap.size(); i++) battleChoose->btnmap[i]->link('r', battleChoose->btnmap[i < battleChoose->btnmap.size()-1 ? i+1 : 0]);
                battleChoose->activeBtn = battleChoose->btnmap[0];
                battleChoose->activeBtn->SetActive();
                for (size_t i = 0; i < enemies.size(); i++) {
                    hpBarsE.emplace_back(new bar(200, 25, 255, 0, 0, 0, 255, 0));
                    enemies[i]->spr.setScale(6, 6);
                    enemies[i]->spr.setPosition({(float)winx/(enemies.size()+1)/2*(i*2+2)-spriteSize(enemies[i]->spr).x/2,20});
                    hpBarsE[i]->base.setPosition({enemies[i]->spr.getPosition().x+spriteSize(enemies[i]->spr).x/2-100.f, enemies[i]->spr.getPosition().y+spriteSize(enemies[i]->spr).y+10.f});
                    hpBarsE[i]->fill.setPosition(hpBarsE[i]->base.getPosition());
                    hpBarsE[i]->label.setFont(btnFont);
                    hpBarsE[i]->label.setPosition(hpBarsE[i]->base.getPosition().x+10.f, hpBarsE[i]->base.getPosition().y-5.f);
                    hpBarsE[i]->update(enemies[i]->curHP, enemies[i]->maxHP);
                    targetMenu->btnmap.emplace_back(new button(to_string(i), spriteSize(enemies[i]->spr), enemies[i]->spr.getPosition(), 107, 88, 69));
                }
                for (size_t i = 0; i < targetMenu->btnmap.size(); i++) targetMenu->btnmap[i]->link('r', targetMenu->btnmap[i < targetMenu->btnmap.size()-1 ? i+1 : 0]);
                targetMenu->activeBtn = targetMenu->btnmap[0];
                targetMenu->activeBtn->SetActive();
                combatSet = true;
            }
            /*if (refreshCombat == true) {
                cout<<"Combat is being refreshed!\n";
                battleChoose->btnmap.clear();
                battleChoose->activeBtn = nullptr;
                for (size_t i = 0; i < players.size(); i++) {
                    if (players[i]->isActive) {
                        cout<<players[i]->name+" is active!\n";
                        battleChoose->btnmap.emplace_back(new button(to_string(i), spriteSize(players[i]->spr), players[i]->spr.getPosition(), 107, 88, 69));
                    }
                }
                for (size_t i = 0; i < battleChoose->btnmap.size(); i++) battleChoose->btnmap[i]->link('r', battleChoose->btnmap[i < battleChoose->btnmap.size()-1 ? i+1 : 0]);
                battleChoose->activeBtn = battleChoose->btnmap[0];
                battleChoose->activeBtn->SetActive();
                refreshCombat = false;
            }*/
            /*if (none_of(players.begin(), players.end(), [](character* i){return i->IsAlive();}) || none_of(enemies.begin(), enemies.end(), [](character* i){return i->IsAlive();})) {
                batDisp.setString("Yuo are the weiner!"); //Some sort of xp thing eventually
                curState == overworld;
            }*/
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {
                    case sf::Keyboard::Enter:
                        switch (curState) {
                            case overworld:
                            break;
                            case combatChoose:
                                if (players[stoi(activeMenu->actButton())]->isActive) {
                                    activePlayer = players[stoi(activeMenu->actButton())];
                                    curState = combat;
                                    activeMenu = battle;
                                } else {
                                    cout<<"Player is inactive!\n";
                                }
                            break;
                            case targetChoose:
                                activeEnemy = enemies[stoi(activeMenu->actButton())];
                                activePlayer->Attack(activeEnemy);
                                activePlayer->isActive = 0;
                                hpBarsE[stoi(activeMenu->actButton())]->update(activeEnemy->curHP, activeEnemy->maxHP);
                                curState = combatChoose;
                                activeMenu = battleChoose;
                                refreshCombat = true;
                            break;
                            case combat:
                                if (activeMenu->actButton() == "Attack!") {
                                    curState = targetChoose;
                                    activeMenu = targetMenu;
                                } else if (activeMenu->actButton() == "Defend.") {
                                    activePlayer->isActive = 0;
                                    activePlayer->Defend();
                                } else if (activeMenu->actButton() == "Flee...") {
                                    activePlayer->isActive = 0;
                                    activePlayer->Flee(badguy);
                                } 
                            break;
                        }
                        break;
                    case sf::Keyboard::Right:
                        activeMenu->navigate('r');
                        break;
                    case sf::Keyboard::Left:
                        activeMenu->navigate('l');
                        break;
                    case sf::Keyboard::Down:
                        //debug
                        if (curState == combat) {
                            user->upStat();
                            cout << "Attack: " << user->tempatk << endl;
                            cout << "Defense: " << user->tempdef << endl;
                            cout << "Speed: " << user->tempspd << endl;
                            cout << "Magic: " << user->tempmagic << endl;
                            for (const auto &i : user->statuses) cout << i.name << ": " << i.dur << endl;
                        }
                        break;
                    case sf::Keyboard::W:
                        //debug
                        if (curState == combat) {
                            cout << "Starting turn!" << endl;
                            user->startTurn();
                        }
                        break;
                }
            }
        }
        window.clear();
        if (curState == combat || curState == combatChoose || curState == targetChoose) {
            for (size_t i = 0; i < players.size(); i++) {
                window.draw(players[i]->spr);
                if (curState == combatChoose) window.draw(battleChoose->btnmap[i]->btn);
                window.draw(hpBars[i]->base);
                window.draw(hpBars[i]->fill);
                window.draw(hpBars[i]->label);
            }
            for (size_t i = 0; i < enemies.size(); i++) {
                window.draw(enemies[i]->spr);
                if (curState == targetChoose) window.draw(targetMenu->btnmap[i]->btn);
                window.draw(hpBarsE[i]->base);
                window.draw(hpBarsE[i]->fill);
                window.draw(hpBarsE[i]->label);
            }
            for (const auto &i : battle->btnmap) {
                window.draw(i->btn);
                window.draw(i->label);
            }
            window.draw(batDisp);
        }
        window.display();
    }
    return 0;
}

/*
Objects created by combat should be smart pointers, or manually deleted if possible
Converting btnmap to a vector of smart pointers causes conversion issues with activeButton (a raw pointer)
Unclear if making activeButton into smart pointer makes sense, and doing so causes errors in the navigation function anyways

Should implement diff RNG at some point (xoshiro256**).

Global vars should be reduced/eliminated.
Two current ones are for the battle display text, so that it may be accessed by combat functions.
This could be eliminated by passing text* into functions, or depending on eventual fate of combat functions.
*/