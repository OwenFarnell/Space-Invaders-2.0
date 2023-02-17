#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <conio.h>
#include <windows.h>

using namespace std;

const int WIDTH = 80;
const int HEIGHT = 30;

class Spaceship {
public:
    int x, y, vx, vy, health, weapon_upgrade, shield_upgrade;
    Spaceship() {
        x = WIDTH / 2;
        y = HEIGHT - 2;
        vx = vy = health = 0;
        weapon_upgrade = shield_upgrade = 0;
    }
};

class Alien {
public:
    int x, y, vx, vy, health;
    Alien(int _x, int _y, int _vx, int _vy) {
        x = _x;
        y = _y;
        vx = _vx;
        vy = _vy;
        health = 1;
    }
};

class Level {
public:
    int level_num, num_aliens;
    vector<Alien> aliens;
    Level(int _level_num, int _num_aliens) {
        level_num = _level_num;
        num_aliens = _num_aliens;
        aliens.reserve(num_aliens);
    }
};

int score = 0;
int lives = 3;
int level = 1;
Spaceship ship;

void draw_ship() {
    cout << " ";
    for (int i = 0; i < ship.health; i++) {
        cout << "+";
    }
    cout << endl;
}

void draw_aliens(Level l) {
    for (int i = 0; i < l.aliens.size(); i++) {
        cout << "X";
    }
    cout << endl;
}

void draw_level(Level l) {
    cout << "Level " << l.level_num << endl;
    draw_aliens(l);
    draw_ship();
    cout << "Score: " << score << " Lives: " << lives << endl;
}

void generate_aliens(Level& l) {
    srand(time(NULL));
    int x = 10;
    int y = 4;
    int vx = 1;
    int vy = 0;
    for (int i = 0; i < l.num_aliens; i++) {
        Alien a(x, y, vx, vy);
        l.aliens.push_back(a);
        x += 5;
        if (x >= WIDTH - 5) {
            x = 10;
            y += 2;
        }
    }
}

void update_ship() {
    ship.x += ship.vx;
    if (ship.x < 0) {
        ship.x = 0;
    }
    if (ship.x >= WIDTH) {
        ship.x = WIDTH - 1;
    }
}

void update_alien(Level& l) {
    for (int i = 0; i < l.aliens.size(); i++) {
        l.aliens[i].x += l.aliens[i].vx;
        if (l.aliens[i].x < 0) {
            l.aliens[i].x = 0;
            l.aliens[i].vx = 1;
            l.aliens[i].y++;
        }
        if (l.aliens[i].x >= WIDTH) {
            l.aliens[i].x = WIDTH - 1;
            l.aliens[i].vx = -1;
            l.aliens[i].y++;
        }
        if (l.aliens[i].y >= HEIGHT - 2) {
            lives--;
            l.aliens.erase(l.aliens.begin() + i);
            i--;
        }
    }
}

void collision_detection(Level& l) {
    for (int i = 0; i < l.aliens.size(); i++) {
        if (l.aliens[i].y == ship.y && abs(l.aliens[i].x - ship.x) < 2) {
            lives--;
            l.aliens.erase(l.aliens.begin() + i);
            i--;
        }
    }
}

void update_game(Level& l) {
    update_ship();
    update_alien(l);
    collision_detection(l);
}

void handle_input(char c) {
    switch (c) {
    case 'a':
        ship.vx = -1;
        break;
    case 'd':
        ship.vx = 1;
        break;
    default:
        break;
    }
}

void upgrade_ship() {
    int choice;
    do {
        cout << "Choose an upgrade:\n1. Weapon Upgrade\n2. Shield Upgrade\n3. Exit Upgrade Menu" << endl;
        cin >> choice;
        switch (choice) {
        case 1:
            ship.weapon_upgrade++;
            score -= 10;
            break;
        case 2:
            ship.health++;
            ship.shield_upgrade++;
            score -= 10;
            break;
        case 3:
            break;
        default:
            break;
        }
    } while (choice != 3);
}

void check_score() {
    if (score >= 10 && score % 10 == 0) {
        upgrade_ship();
    }
}

bool game_over() {
    if (lives <= 0) {
        return true;
    }
    if (level > 3) {
        return true;
    }
    return false;
}

void reset_game() {
    score = 0;
    lives = 3;
    level = 1;
    ship.health = 0;
    ship.weapon_upgrade = 0;
    ship.shield_upgrade = 0;
    ship.x = WIDTH / 2;
    ship.y = HEIGHT - 2;
}

int main() {
    while (true) {
        reset_game();
        while (!game_over()) {
            Level l(level, level * 10);
            generate_aliens(l);
            while (l.aliens.size() > 0 && lives > 0) {
                system("cls");
                draw_level(l);
                if (_kbhit()) {
                    char c = _getch();
                    handle_input(c);
                }
                update_game(l);
                check_score();
                Sleep(50);
            }
            level++;
        }
        system("cls");
        cout << "Game Over!" << endl;
        cout << "Final Score: " << score << endl;
        cout << "Press 'r' to restart or any other key to quit." << endl;
        char c = _getch();
        if (c != 'r') {
            break;
        }
    }
    return 0;
}
