#include "main.hpp"

int main() {
    srand(time(NULL));
    cout<<"Type your name: "<<endl;
    string username;
    getline(cin, username);
    character *user = new player(username, "This is a person.", 10, 10, 1, 1, 1, 1, 0, 1); 
    character *badguy = new enemy("Bad Dude", "Not a good dude.", 1);
    int winx = 800, winy = 800;
    //sf::Vector2i localmousepos = {0,0};
    sf::RenderWindow window(sf::VideoMode(winx,winy), "Name");
    window.setVerticalSyncEnabled(true);
    
    int activeBtn = 0;

    //Battle menu button setup
    //This will be class once I make better button selection logic
    //Selected button is shown with semi-transparent overlay
    //Might directly change button's color values later
    sf::RectangleShape overlayBtn({100, 50});
    overlayBtn.setPosition({50, 700});
    overlayBtn.setFillColor(sf::Color(0, 0, 0, 200));
    overlayBtn.setOutlineColor(sf::Color::Transparent);
    sf::RectangleShape atkBtn({100, 50});
    atkBtn.setPosition({50, 700});
    atkBtn.setFillColor(sf::Color(234, 32, 26));
    atkBtn.setOutlineColor(sf::Color::White);
    atkBtn.setOutlineThickness(2);
    sf::Font btnFont;
    btnFont.loadFromFile("Assets/Manjari-Thin.otf");
    sf::Text atkBtnText("Attack!", btnFont, 25);
    atkBtnText.setPosition({55, 705});
    atkBtnText.setFillColor(sf::Color::Black);
    atkBtnText.setOutlineColor(sf::Color::White);
    atkBtnText.setOutlineThickness(2);
    sf::RectangleShape defBtn({100, 50});
    defBtn.setPosition({160, 700});
    defBtn.setFillColor(sf::Color(121, 227, 241));
    defBtn.setOutlineColor(sf::Color::White);
    defBtn.setOutlineThickness(2);
    sf::Text defBtnText("Defend!", btnFont, 25);
    defBtnText.setPosition({165,705});
    defBtnText.setFillColor(sf::Color::Black);
    defBtnText.setOutlineColor(sf::Color::White);
    defBtnText.setOutlineThickness(2);
    sf::RectangleShape runBtn({100, 50});
    runBtn.setPosition({270, 700});
    runBtn.setFillColor(sf::Color(255, 254, 122));
    runBtn.setOutlineColor(sf::Color::White);
    runBtn.setOutlineThickness(2);
    sf::Text runBtnText("Flee!", btnFont, 25);
    runBtnText.setPosition({275,705});
    runBtnText.setFillColor(sf::Color::Black);
    runBtnText.setOutlineColor(sf::Color::White);
    runBtnText.setOutlineThickness(2);

    //Texture setup
    sf::Texture dude;
    dude.loadFromFile("Assets/dude.png");
    sf::Texture baddude;
    baddude.loadFromFile("Assets/baddude.png");
    
    //Character sprite setup
    user->spr.setTexture(dude);
    badguy->spr.setTexture(baddude);

    //Battle sprite positioning
    user->spr.setScale(4, 4);
    user->spr.setPosition(0, winy-64);
    badguy->spr.setScale(4, 4);
    badguy->spr.setPosition(winx-64, 0);
    
    //Battle hp positioning
    user->hpR.setPosition({50, 675});
    user->hpG.setPosition(user->hpR.getPosition()); //reference to red's pos?
    badguy->hpR.setPosition({500, 100});
    badguy->hpG.setPosition(badguy->hpR.getPosition());
    
    //Battle hp text setup
    user->hpT.setFont(btnFont);
    badguy->hpT.setFont(btnFont);
    user->hpT.setPosition({50, 650});
    badguy->hpT.setPosition({500, 50});

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyReleased) {
                //Battle input, make into function later
                if (event.key.code == sf::Keyboard::Enter) {
                    switch (activeBtn) {
                        case 0:
                            user->Attack(badguy);
                            break;
                        case 1:
                            user->Defend();
                            break;
                        case 2:
                            user->Flee();
                            break;
                    }
                } else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {
                    if (activeBtn >= 2) {
                        activeBtn = 0;
                    } else {
                        activeBtn++;
                    }
                    overlayBtn.setPosition(activeBtn*110.f+50.f, 700.f);
                } else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
                    if (activeBtn <= 0) {
                        activeBtn = 2;
                    } else {
                        activeBtn--;
                    }
                    overlayBtn.setPosition(activeBtn*110.f+50.f, 700.f);
                } else if (event.key.code == sf::Keyboard::S) {
                    //debug
                    for (int i = 0; i < user->statuses.size(); i++) {
                        cout << "Attack: " << user->tempatk << endl;
                        cout << "Defense: " << user->tempdef << endl;
                        cout << "Speed: " << user->tempspd << endl;
                        cout << "Magic: " << user->tempmagic << endl;
                        cout << i << ". " << user->statuses[i].name << endl;
                    }
                }
            }
            /*if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    localmousepos.x = event.mouseButton.x;
                    localmousepos.y = event.mouseButton.y;
                    cout << event.mouseButton.x+", "+event.mouseButton.y<< endl;
                    if (atkBtn.getLocalBounds().contains(localmousepos.x, localmousepos.y)) {
                        atkBtn.setFillColor(sf::Color::Green);
                    }
                }
            }*/
        }
        window.clear();
        window.draw(atkBtn);
        window.draw(atkBtnText);
        window.draw(defBtn);
        window.draw(defBtnText);
        window.draw(runBtn);
        window.draw(runBtnText);
        window.draw(overlayBtn);
        window.draw(user->spr);
        window.draw(badguy->spr);
        window.draw(user->hpR);
        window.draw(user->hpG);
        window.draw(badguy->hpR);
        window.draw(badguy->hpG);
        window.draw(user->hpT);
        window.draw(badguy->hpT);
        window.display();
    }

    return 0;
}