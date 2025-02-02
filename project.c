#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

// Define the user_information structure
typedef struct {
    char name[100];
    char password[100];
    char email[100];
    int score;
} user_information;

typedef struct {
    int y;
    int x;
}location;

typedef struct {
    location loc;
    int health;//adad salamati bein 0 ta 100
    int weapons[4];//list selah haii ke dare
    int foods;//tedad ghaza hash
    int score;
    int color;//0 = red | 1 = blue | 2 = green
    int magics[4];
    int weapon_typ;
}character_information;

//character_information characinf;

typedef struct {
    int length;
    int width;
    location wall[28];//hadeaghal tol ba arze otagh 7*7 e pas mohitesh 28 e
    int n_w;// number of windows
    location win[28];//makane panjere ha
    location pillar[3];
    location door[4];
}floor_informatoin;


// Function prototypes
void print_menu(int highlight);
void get_info();
void new_user(user_information *user);
void login(user_information *user);
void game_menu(user_information *user);
void profile(user_information *user);
void scoreboard();
int check_duplicate(const char *name, const char *email);
void new_game();
void load_game();
void guest_user(user_information *user);
void game_setting();
void game_difficulty();
void character_color();
void print_map();
//void print_character(int random_room,struct roominf[9]);
int g_m=0;//game difficulty
int c_c=0;//characte color   0=red   1=blue   2=green

int main() {
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    user_information current_user = {"", "", "", 0}; // Initialize user information

    int choice;
    int highlight = 0;

    while(1) {
        print_menu(highlight);
        choice = getch();
        switch(choice) {
            case KEY_UP:
                if(highlight == 0)
                    highlight = 6;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight == 6)
                    highlight = 0;
                else
                    highlight++;
                break;
            case 10: // Enter key
                switch(highlight) {
                    case 0:
                        clear();
                        guest_user(&current_user);
                        break;
                    case 1:
                        clear();
                        new_user(&current_user);
                        break;
                    case 2:
                        clear();
                        login(&current_user);
                        break;
                    case 3:
                        clear();
                        game_menu(&current_user);
                        break;
                    case 4:
                        clear();
                        profile(&current_user);
                        break;
                    case 5:
                        clear();
                        scoreboard();
                        break;
                    case 6:
                        clear();
                        game_setting();
                        break;
                }
                break;
            case 'q':
                endwin();
                exit(0);
            default:
                break;
        }
    }

    endwin();
    return 0;
}

void print_menu(int highlight) {
    clear();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    noecho();
    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    mvprintw(1, 9, "Welcome");
    attroff(COLOR_PAIR(1));

    char *choices[] = {
        "Guest user",
        "New User",
        "User Login",
        "Game Menu",
        "Profile",
        "Scoreboard",
        "Game setting"
    };

    for(int i = 0; i < 7; i++) {
        if(highlight == i) {
            attron(COLOR_PAIR(3));
            mvprintw(2 + i*2, 2, "%s", choices[i]);
            attroff(COLOR_PAIR(3));
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(2 + i*2, 1, "%s", choices[i]);
            attroff(COLOR_PAIR(2));
        }
    }

    mvprintw(16, 1, "Press 'q' to Quit");
    refresh();
}

void new_user(user_information *user) {
    here:
    move(3,0);
    clrtoeol();
    mvprintw(0, 0, "New User");
    noecho();
    mvprintw(3, 0, "Enter your name:");
    echo();
    move(3, 16);
    scanw("%s", user->name);
    noecho();

    // Check for duplicate name or email
    if (check_duplicate(user->name, user->email)) {
        mvprintw(5, 0, "Name already exists.");
        refresh();
        goto here;
    }

    while(1) {
        mvprintw(5, 0, "Enter your password:");
        echo();
        move(5, 21);
        clrtoeol();
        scanw("%s", user->password);
        noecho();
        int c = 0, s = 0, num = 0, i = 0;

        while(user->password[i] != '\0') {
            if(user->password[i] >= 'a' && user->password[i] <= 'z'){
                s++;
            }
            if(user->password[i] >= 'A' && user->password[i] <= 'Z'){
                c++;
            }
            if(user->password[i] >= '0' && user->password[i] <= '9'){
                num++;
            }
            i++;
        }

        if(i < 6 || num == 0 || c == 0 || s == 0){
            start_color();
            init_pair(9, COLOR_RED, COLOR_BLACK);
            attron(COLOR_PAIR(9) | A_BOLD);
            mvprintw(6, 0, "password ghabele ghabol nist, password bayad hadeaghal daraye 7 character va horofe bozorg (A,B...) va horoofe kochik (a,b..) va adad bashad.");
            attroff(COLOR_PAIR(9) | A_BOLD);
        } else {
            break;
        }
    }
    move(6, 0);
    clrtoeol();
    while (1) {
        here2:
        mvprintw(7, 0, "Enter your email:");
        echo();
        move(7, 17);
        clrtoeol();
        scanw("%s", user->email);
        noecho();
        if (check_duplicate(user->name, user->email)) {
        mvprintw(8, 0, "email already exists.");
        refresh();
        goto here2;
    }

        int i = 0, at = 0, dot = 0, valid = 1;

        while (user->email[i] != '\0') {
            if (user->email[i] == '@')
                at++;
            if (user->email[i] == '.')
                dot++;
            i++;
        }

        if (at != 1 || dot != 1) {
            start_color();
            init_pair(9, COLOR_RED, COLOR_BLACK);
            attron(COLOR_PAIR(9) | A_BOLD);
            mvprintw(8, 0, "Email be form AAA@BBB.CCC nist, dobare vared konid.");
            attroff(COLOR_PAIR(9) | A_BOLD);
            valid = 0;
        } else {
            valid = 1;
        }

        if (valid)
            break;
    }
    move(8,0);
    clrtoeol();
    start_color();
    init_pair(8, COLOR_WHITE, COLOR_RED);
    attron(COLOR_PAIR(8) | A_BLINK);
    mvprintw(2, 5, "You login successfully");
    attroff(COLOR_PAIR(8) | A_BLINK);
    mvprintw(12, 0, "Press Enter to go to Main Menu.");
    
    // Save user info to file
    FILE *file = fopen("inf.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s %d\n", user->name, user->password, user->email, user->score);
        fclose(file);
    }

    refresh();
    int ch = getch();
    if (ch == 10) { // Enter key
        clear();
        return;
    }
    clear();
}

void guest_user(user_information *user)
{
    clear();
    echo();
    mvprintw(0, 0, "Guest User");
    attron(A_BOLD);
    mvprintw(2, 0, "Enter your name:");
    move(2, 16);
    attroff(A_BOLD);
    scanw("%s", user->name);
    noecho();
    strcpy(user->email, "Guest user"); // Set email to "Guest user"
    user->password[0] = '\0'; // Set password to an empty string
    user->score = 0; // Optionally reset score
    mvprintw(4, 0, "Welcome, %s! You are logged in as a guest user.", user->name);
    refresh();
    getch();
    clear();
}



void login(user_information *user) {
    mvprintw(0, 0, "User Login");
    mvprintw(3, 0, "Enter your name:");
    echo();
    char name[100];
    move(3, 16);
    scanw("%s", name);

    mvprintw(5, 0, "Enter your password:");
    echo();
    char password[100];
    move(5, 21);
    clrtoeol();
    scanw("%s", password);
    noecho();

    // Check credentials
    FILE *file = fopen("inf.txt", "r");
    if (file != NULL) {
        char file_name[100], file_password[100], file_email[100];
        int file_score;
        int found = 0;
        while (fscanf(file, "%s %s %s %d", file_name, file_password, file_email, &file_score) != EOF) {
            if (strcmp(name, file_name) == 0 && strcmp(password, file_password) == 0) {
                strcpy(user->name, file_name);
                strcpy(user->password, file_password);
                strcpy(user->email, file_email);
                user->score = file_score;
                found = 1;
                break;
            }
        }
        fclose(file);

        if (found) {
            mvprintw(7, 0, "Login successful!");
        } else {
            mvprintw(7, 0, "Incorrect name or password.");
        }
    } else {
        mvprintw(7, 0, "No users registered.");
    }
    
    refresh();
    getch();
    clear();
}

void game_menu(user_information *user) {
    clear();
    if(user->name[0] == '\0') { 
        clear();
        start_color();
        init_pair(4, COLOR_RED, COLOR_BLACK);
        attron(A_BOLD | COLOR_PAIR(4));
        mvprintw(4, 4, "You have not logged in before!");
        attroff(A_BOLD | COLOR_PAIR(4));
        refresh();
        getch();
        return;
    }
    mvprintw(0, 0, "Game Menu");
    int h = 0; // Declare h outside the loop to retain its value across iterations
    while(1) {
        clear();
        mvprintw(0, 0, "Game Menu");
        echo();
        if(h == 0) {
            start_color();
            init_pair(10, COLOR_BLACK, COLOR_RED);
            attron(COLOR_PAIR(10));
            mvprintw(2, 0, "New game");
            attroff(COLOR_PAIR(10));
            init_pair(11, COLOR_RED, COLOR_BLACK);
            attron(COLOR_PAIR(11));
            mvprintw(4, 0, "Load game");
            attroff(COLOR_PAIR(11));
        } else {
            start_color();
            init_pair(10, COLOR_BLACK, COLOR_RED);
            attron(COLOR_PAIR(10));
            mvprintw(4, 0, "Load game");
            attroff(COLOR_PAIR(10));
            init_pair(11, COLOR_RED, COLOR_BLACK);
            attron(COLOR_PAIR(11));
            mvprintw(2, 0, "New game");
            attroff(COLOR_PAIR(11));
        }
        refresh();

        int choise = getch();
        if(choise == KEY_UP) {
            if(h == 0) {
                h = 1;
            } else {
                h--;
            }
        } else if(choise == KEY_DOWN) {
            if(h == 1) {
                h = 0;
            } else {
                h++;
            }
        } else if(choise == 'q') {
            break;
        } else if(choise == 10) { // Enter key
            if(h == 0) {
                new_game();
            } else {
                load_game();
            }
        }
    }
    refresh();
}

void profile(user_information *user) {
    if(user->name[0] == '\0') { 
        clear();
        start_color();
        init_pair(4, COLOR_RED, COLOR_BLACK);
        attron(A_BOLD | COLOR_PAIR(4));
        mvprintw(4, 4, "You have not logged in before!");
        attroff(A_BOLD | COLOR_PAIR(4));
        refresh();
        getch();
        return;
    } else {
        clear();
        mvprintw(0, 0, "Profile");
        mvprintw(2, 0, "Your name:");
        attron(A_BOLD);
        mvprintw(2, 12, "%s", user->name);
        attroff(A_BOLD);

        mvprintw(4, 0, "Your email:");
        attron(A_BOLD);
        mvprintw(4, 12, "%s", user->email);
        attroff(A_BOLD);

        mvprintw(6, 0, "Your score:");
        attron(A_BOLD);
        mvprintw(6, 12, "%d", user->score);
        attroff(A_BOLD);

        mvprintw(8, 0, "Press Enter to go to Main Menu.");
        refresh();
        int ch = getch();
        if (ch == 10) { 
            clear();
            return;
        }
        clear();
    }
}

void scoreboard() {
    mvprintw(0, 0, "Scoreboard - Press any key to return...");
    refresh();
    getch();
    clear();
}

void game_setting() {
    clear();
    start_color();
    mvprintw(0, 0, "Game setting");
    init_pair(54, COLOR_RED, COLOR_BLACK);
    init_pair(55, COLOR_BLACK, COLOR_RED);
    int h = 0;
    while (1) {
        clear();
        mvprintw(0, 0, "Game setting");
        if (h == 0) {
            attron(COLOR_PAIR(55));
            mvprintw(2, 0, "Select game difficulty");
            attroff(COLOR_PAIR(55));
            attron(COLOR_PAIR(54));
            mvprintw(4, 0, "Character color");
            attroff(COLOR_PAIR(54));
        } else {
            attron(COLOR_PAIR(55));
            mvprintw(4, 0, "Character color");
            attroff(COLOR_PAIR(55));
            attron(COLOR_PAIR(54));
            mvprintw(2, 0, "Select game difficulty");
            attroff(COLOR_PAIR(54));
        }
        int choise = getch();
        if (choise == KEY_UP) {
            h = (h == 0) ? 1 : 0;
        } else if (choise == KEY_DOWN) {
            h = (h == 1) ? 0 : 1;
        } else if (choise == 10) { // کلید Enter
            if (h == 0) {
                game_difficulty();
            } else {
                character_color();
            }
        } else if (choise == 'q') {
            break;
        }
        refresh();
    }
    clear();
    refresh();
}

void game_difficulty() {
    clear();
    start_color();
    mvprintw(0, 0, "Selcet game difficulty");
    init_pair(54, COLOR_RED, COLOR_BLACK);
    init_pair(55, COLOR_BLACK, COLOR_RED);
    int h = 0;
    while (1) {
        clear();
        mvprintw(0, 0, "Select game difficulty");
        if (h == 0) {
            attron(COLOR_PAIR(55));
            mvprintw(2, 0, "Easy");
            attroff(COLOR_PAIR(55));
            attron(COLOR_PAIR(54));
            mvprintw(4, 0, "Medium");
            mvprintw(6,0,"Hard");
            attroff(COLOR_PAIR(54));
        } else if(h==1) {
            attron(COLOR_PAIR(54));
            mvprintw(2, 0, "Easy");
            attroff(COLOR_PAIR(54));
            attron(COLOR_PAIR(55));
            mvprintw(4, 0, "Medium");
            attroff(COLOR_PAIR(55));
            attron(COLOR_PAIR(54));
            mvprintw(6, 0, "Hard");
            attroff(COLOR_PAIR(54));
        }else{
            attron(COLOR_PAIR(54));
            mvprintw(2, 0, "Easy");
            mvprintw(4, 0, "Medium");
            attroff(COLOR_PAIR(54));
            attron(COLOR_PAIR(55));
            mvprintw(6, 0, "Hard");
            attroff(COLOR_PAIR(55));
        }
        int choise = getch();
        if (choise == KEY_UP) {
            if(h==0){
                h=2;
            }else{
                h--;
            }
        } else if (choise == KEY_DOWN) {
            if(h==2){
                h=0;
            }else{
                h++;
            }
        } else if (choise == 10) { // کلید Enter
            if (h == 0) {
                g_m = 0;
                break;
            } else if(h==1){
                g_m=1;
                break;
            }else{
                g_m=2;
                break;
            }
        } else if (choise == 'q') {
            break;
        }
        refresh();
    }
    clear();
    refresh();
}

void character_color() {
    clear();
    start_color();
    mvprintw(0, 0, "Selcet character color!");
    init_pair(54, COLOR_RED, COLOR_BLACK);
    init_pair(55, COLOR_BLUE, COLOR_BLACK);
    init_pair(56, COLOR_GREEN, COLOR_BLACK);
    int h = 0;
    while (1) {
        clear();
        mvprintw(0, 0, "Selcet character color!");
        if (h == 0) {
            attron(COLOR_PAIR(54));
            mvprintw(2, 2, "Red");
            attroff(COLOR_PAIR(54));
            attron(COLOR_PAIR(55));
            mvprintw(4, 0, "Blue");
            attroff(COLOR_PAIR(55));
            attron(COLOR_PAIR(56));
            mvprintw(6, 0, "Green");
            attroff(COLOR_PAIR(56));
        } else if(h==1) {
            attron(COLOR_PAIR(54));
            mvprintw(2, 0, "Red");
            attroff(COLOR_PAIR(54));
            attron(COLOR_PAIR(55));
            mvprintw(4, 2, "Blue");
            attroff(COLOR_PAIR(55));
            attron(COLOR_PAIR(56));
            mvprintw(6, 0, "Green");
            attroff(COLOR_PAIR(56));
        }else{
            attron(COLOR_PAIR(54));
            mvprintw(2, 0, "Red");
            attroff(COLOR_PAIR(54));
            attron(COLOR_PAIR(55));
            mvprintw(4, 0, "Blue");
            attroff(COLOR_PAIR(55));
            attron(COLOR_PAIR(56));
            mvprintw(6, 2, "Green");
            attroff(COLOR_PAIR(56));
        }
        int choise = getch();
        if (choise == KEY_UP) {
            if(h==0){
                h=2;
            }else{
                h--;
            }
        } else if (choise == KEY_DOWN) {
            if(h==2){
                h=0;
            }else{
                h++;
            }
        } else if (choise == 10) { // کلید Enter
            if (h == 0) {
                c_c = 0;
                break;
            } else if(h==1){
                c_c=1;
                break;
            }else{
                c_c=2;
                break;
            }
        } else if (choise == 'q') {
            break;
        }
        refresh();
    }
    clear();
    refresh();
}

int check_duplicate(const char *name, const char *email) {
    FILE *file = fopen("inf.txt", "r");
    if (file != NULL) {
        char file_name[100], file_email[100];
        while (fscanf(file, "%s %*s %s %*d", file_name, file_email) != EOF) {
            if (strcmp(name, file_name) == 0 || strcmp(email, file_email) == 0) {
                fclose(file);
                return 1; // Duplicate found
            }
        }
        fclose(file);
    }
    return 0; // No duplicates
}

void new_game()
{
    clear();
    mvprintw(0, 0, "New game");
    print_map();
    refresh();
    //break;
}

void load_game()
{
    clear();
    mvprintw(0, 0, "Load game - Press any key to return...");
    refresh();
    getch();
    clear();
    //break;
}

struct rooms_inf {
    int x_n_w; //x sorth east
    int y_n_w;
    int x_s_e;
    int y_s_e;
};
struct rooms_inf roominf[9];

struct corridor_inf{
    int x_s;//x start
    int y_s;//y start
    int x_e;//x end
    int y_e;//y end
};
struct corridor_inf coorrinf[11];

struct trap
{
    int x;
    int y;
};
struct trap trapinf[1000];

struct magic{
    int magic_type;// 1 = health   || 2 = speed  || 3 = damage 
    int x;
    int y;
};

struct magic magicinf[4];

struct weapon{
    int weapon_type;// 1 = gorze || 2 = khanjar || 3 = aasaye jadoii || 4 = tire aadi || 5 = shamshir   
    int x;
    int y;
};

struct weapon weaponinf[4];

struct food{
    int x;
    int y;
};

struct food foodinf[5];

void print_map() {
    clear();
    start_color();
    init_pair(14, COLOR_YELLOW, COLOR_BLACK);
    srand(time(0));

    character_information characinf;
    int n_r = (rand() % 1) + 9; // تعداد اتاق‌ها (۶ تا ۱۰)
    int grid_size = 3; 
    int cell_width = 35;
    int cell_height = 11;
    int cell_status[9] = {0}; // افزایش اندازه برای قرار دادن تا ۹ سلول

    for (int ii = 0; ii < n_r; ii++) {
        int cell_index;
        do {
            cell_index = rand() % 9; // اجازه انتخاب از ۹ سلول
        } while (cell_status[cell_index] == 1); 

        cell_status[cell_index] = 1;

        int n_wx = (cell_index % grid_size) * cell_width;
        int n_wy = (cell_index / grid_size) * cell_height;

        int random_n = (rand() % 5) + 6; // عرض اتاق (۶ تا ۱۰)
        int random_n2 = (rand() % 5) + 6; // طول اتاق (۶ تا ۱۰)

        floor_informatoin floor;
        floor.width = random_n;
        floor.length = random_n2;
        roominf[ii].x_n_w = n_wx;
        roominf[ii].y_n_w = n_wy;
        roominf[ii].x_s_e = n_wx + random_n2;
        roominf[ii].y_s_e = n_wy + random_n;
        attron(COLOR_PAIR(14));
        for (int i = 0; i < floor.length; i++) {
            move(n_wy, n_wx + i + 1);
            printw("_");
        }
        for (int i = 0; i < floor.length; i++) {
            move(n_wy + floor.width, n_wx + i);
            printw("_");
        }
        for (int i = 0; i < floor.width; i++) {
            move(n_wy + i + 1, n_wx);
            printw("|");
        }
        for (int i = 0; i < floor.width; i++) {
            move(n_wy + i + 1, n_wx + floor.length);
            printw("|");
        }
        attroff(COLOR_PAIR(14));

        init_pair(15, COLOR_BLUE, COLOR_BLACK);
        attron(COLOR_PAIR(15) | A_BOLD);
        for (int i = n_wx + 1; i < n_wx + floor.length; i++) {
            for (int j = n_wy + 1; j < n_wy + floor.width; j++) {
                mvprintw(j, i, ".");
            }
        }
        attroff(COLOR_PAIR(15) | A_BOLD);
    }
    //chap rah haye bein otagh ha
    //1 2 3 
    //4 5 6
    //7 8 9
    int yek;
    int dow;
    int swe;
    int chahr;
    int panj;
    int shish;
    int haft;
    int hasht;
    int nooh;
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w < 6  && roominf[i].y_n_w < 6){
                yek = i;
                break;
            }
        }
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w > 30 && roominf[i].x_n_w <68 && roominf[i].y_n_w < 6){
            dow = i;
            break;
        }
    }
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w > 63 && roominf[i].y_n_w < 6){
            swe = i;
            break;
        }
    }
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w < 6  && roominf[i].y_n_w > 10 && roominf[i].y_n_w <20 ){
            chahr = i;
            break;
        }
    }
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w > 26  && roominf[i].x_n_w <63 && roominf[i].y_n_w > 10 && roominf[i].y_n_w < 20){
            panj = i;
            break;
        }
    }
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w > 61 && roominf[i].y_n_w > 10 && roominf[i].y_n_w < 20){
            shish = i;
            break;
        }
    }
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w < 6  && roominf[i].y_n_w > 20 ){
            haft = i;
            break;
        }
    }
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w > 24  && roominf[i].x_n_w <61 && roominf[i].y_n_w > 20){
            hasht = i;
            break;
        }
    }
    for(int i=0;i<12;i++){
        if(roominf[i].x_n_w > 40 && roominf[i].y_n_w > 20){
            nooh = i;
            break;
        }
    }
    // chap nahaii
    int yy = (roominf[yek].y_n_w + roominf[yek].y_s_e + roominf[dow].y_n_w + roominf[dow].y_s_e) / 4;
    mvprintw(yy,roominf[yek].x_s_e,"=");
    mvprintw(yy,roominf[dow].x_n_w,"=");
    for(int i=roominf[yek].x_s_e + 1; i<roominf[dow].x_n_w ;i++){
        mvprintw(yy,i,"#");
    }
    coorrinf[0].y_e = yy;
    coorrinf[0].y_s = yy;
    coorrinf[0].x_s = roominf[yek].x_s_e + 1;
    coorrinf[0].x_e = roominf[dow].x_n_w - 1;
    int yy2 = (roominf[swe].y_n_w + roominf[swe].y_s_e + roominf[dow].y_n_w + roominf[dow].y_s_e) / 4;
    mvprintw(yy2,roominf[dow].x_s_e,"=");
    mvprintw(yy2,roominf[swe].x_n_w,"=");
    for(int i=roominf[dow].x_s_e + 1; i<roominf[swe].x_n_w ;i++){
        mvprintw(yy2,i,"#");
    }
    coorrinf[1].y_e = yy2;
    coorrinf[1].y_s = yy2;
    coorrinf[1].x_s = roominf[dow].x_s_e + 1;
    coorrinf[1].x_e = roominf[swe].x_n_w - 1;
    int xx = (roominf[yek].x_n_w + roominf[yek].x_s_e + roominf[chahr].x_n_w + roominf[chahr].x_s_e) / 4;
    mvprintw(roominf[yek].y_s_e,xx,"=");
    mvprintw(roominf[chahr].y_n_w,xx,"=");
    for(int i=roominf[yek].y_s_e + 1; i<roominf[chahr].y_n_w ;i++){
        mvprintw(i,xx,"#");
    }
    coorrinf[2].x_e = xx;
    coorrinf[2].x_s = xx;
    coorrinf[2].y_s = roominf[yek].y_s_e + 1;
    coorrinf[2].y_e = roominf[chahr].y_n_w - 1;
    int yy3 = (roominf[chahr].y_n_w + roominf[chahr].y_s_e + roominf[panj].y_n_w + roominf[panj].y_s_e) / 4;
    mvprintw(yy3,roominf[chahr].x_s_e,"=");
    mvprintw(yy3,roominf[panj].x_n_w,"=");
    for(int i=roominf[chahr].x_s_e + 1; i<roominf[panj].x_n_w ;i++){
        mvprintw(yy3,i,"#");
    }
    coorrinf[3].y_e = yy3;
    coorrinf[3].y_s = yy3;
    coorrinf[3].x_s = roominf[chahr].x_s_e + 1;
    coorrinf[3].x_e = roominf[panj].x_n_w - 1;
    int yy4 = (roominf[panj].y_n_w + roominf[panj].y_s_e + roominf[shish].y_n_w + roominf[shish].y_s_e) / 4;
    mvprintw(yy4,roominf[panj].x_s_e,"=");
    mvprintw(yy4,roominf[shish].x_n_w,"=");
    for(int i=roominf[panj].x_s_e + 1; i<roominf[shish].x_n_w ;i++){
        mvprintw(yy4,i,"#");
    }
    coorrinf[4].y_e = yy4;
    coorrinf[4].y_s = yy4;
    coorrinf[4].x_s = roominf[panj].x_s_e + 1;
    coorrinf[4].x_e = roominf[shish].x_n_w - 1;
    int xx2 = (roominf[shish].x_n_w + roominf[shish].x_s_e + roominf[nooh].x_n_w + roominf[nooh].x_s_e) / 4;
    mvprintw(roominf[shish].y_s_e,xx2,"=");
    mvprintw(roominf[nooh].y_n_w,xx2,"=");
    for(int i=roominf[shish].y_s_e + 1; i<roominf[nooh].y_n_w ;i++){
        mvprintw(i,xx2,"#");
    }
    coorrinf[5].x_e = xx2;
    coorrinf[5].x_s = xx2;
    coorrinf[5].y_s = roominf[shish].y_s_e + 1;
    coorrinf[5].y_e = roominf[nooh].y_n_w - 1;
    int yy5 = (roominf[haft].y_n_w + roominf[haft].y_s_e + roominf[hasht].y_n_w + roominf[hasht].y_s_e) / 4;
    mvprintw(yy5,roominf[haft].x_s_e,"=");
    mvprintw(yy5,roominf[hasht].x_n_w,"=");
    for(int i=roominf[haft].x_s_e + 1; i<roominf[hasht].x_n_w ;i++){
        mvprintw(yy5,i,"#");
    }
    coorrinf[6].y_e = yy5;
    coorrinf[6].y_s = yy5;
    coorrinf[6].x_s = roominf[haft].x_s_e + 1;
    coorrinf[6].x_e = roominf[hasht].x_n_w - 1;
    int yy6 = (roominf[hasht].y_n_w + roominf[hasht].y_s_e + roominf[nooh].y_n_w + roominf[nooh].y_s_e) / 4;
    mvprintw(yy6,roominf[hasht].x_s_e,"=");
    mvprintw(yy6,roominf[nooh].x_n_w,"=");
    for(int i=roominf[hasht].x_s_e + 1; i<roominf[nooh].x_n_w ;i++){
        mvprintw(yy6,i,"#");
    }
    coorrinf[7].y_e = yy6;
    coorrinf[7].y_s = yy6;
    coorrinf[7].x_s = roominf[hasht].x_s_e + 1;
    coorrinf[7].x_e = roominf[nooh].x_n_w - 1;
    // bein 3 6 
    int randd = rand();
    randd = randd%2;
    if(randd == 0){ // chap mikonim
        int xx3 = (roominf[shish].x_n_w + roominf[shish].x_s_e + roominf[swe].x_n_w + roominf[swe].x_s_e) / 4;
        mvprintw(roominf[swe].y_s_e,xx3,"=");
        mvprintw(roominf[shish].y_n_w,xx3,"=");
        for(int i=roominf[swe].y_s_e + 1; i<roominf[shish].y_n_w ;i++){
            mvprintw(i,xx3,"#");
        }
        coorrinf[8].x_e = xx3;
        coorrinf[8].x_s = xx3;
        coorrinf[8].y_s = roominf[swe].y_s_e + 1;
        coorrinf[8].y_e = roominf[shish].y_n_w - 1;
    }else{
        coorrinf[8].x_e = -1;
        coorrinf[8].x_s = -1;
        coorrinf[8].y_s = -1;
        coorrinf[8].y_e = -1;
    }
    int rand2 = rand();
    rand2 = rand2 % 4;
    if(rand2 == 0){// bein  2 5
        int xx4 = (roominf[dow].x_n_w + roominf[dow].x_s_e + roominf[panj].x_n_w + roominf[panj].x_s_e) / 4;
        mvprintw(roominf[dow].y_s_e,xx4,"=");
        mvprintw(roominf[panj].y_n_w,xx4,"=");
        for(int i=roominf[dow].y_s_e + 1; i<roominf[panj].y_n_w ;i++){
            mvprintw(i,xx4,"#");
        }
        coorrinf[9].x_e = xx4;
        coorrinf[9].x_s = xx4;
        coorrinf[9].y_s = roominf[dow].y_s_e + 1;
        coorrinf[9].y_e = roominf[panj].y_n_w - 1;
        coorrinf[10].x_e = -1;
        coorrinf[10].x_s = -1;
        coorrinf[10].y_s = -1;
        coorrinf[10].y_e = -1;
    }else if(rand2 == 1){// bein 5 8
        int xx5 = (roominf[hasht].x_n_w + roominf[hasht].x_s_e + roominf[panj].x_n_w + roominf[panj].x_s_e) / 4;
        mvprintw(roominf[panj].y_s_e,xx5,"=");
        mvprintw(roominf[hasht].y_n_w,xx5,"=");
        for(int i=roominf[panj].y_s_e + 1; i<roominf[hasht].y_n_w ;i++){
            mvprintw(i,xx5,"#");
        }
        coorrinf[10].x_e = xx5;
        coorrinf[10].x_s = xx5;
        coorrinf[10].y_s = roominf[panj].y_s_e + 1;
        coorrinf[10].y_e = roominf[hasht].y_n_w - 1;
        coorrinf[9].x_e = -1;
        coorrinf[9].x_s = -1;
        coorrinf[9].y_s = -1;
        coorrinf[9].y_e = -1;

    }else if(rand2 == 2){ // ham 2 5 ham 5 8
        int xx6 = (roominf[dow].x_n_w + roominf[dow].x_s_e + roominf[panj].x_n_w + roominf[panj].x_s_e) / 4;
        mvprintw(roominf[dow].y_s_e,xx6,"=");
        mvprintw(roominf[panj].y_n_w,xx6,"=");
        for(int i=roominf[dow].y_s_e + 1; i<roominf[panj].y_n_w ;i++){
            mvprintw(i,xx6,"#");
        }
        int xx7 = (roominf[hasht].x_n_w + roominf[hasht].x_s_e + roominf[panj].x_n_w + roominf[panj].x_s_e) / 4;
        mvprintw(roominf[panj].y_s_e,xx7,"=");
        mvprintw(roominf[hasht].y_n_w,xx7,"=");
        for(int i=roominf[panj].y_s_e + 1; i<roominf[hasht].y_n_w ;i++){
            mvprintw(i,xx7,"#");
        }
        coorrinf[9].x_e = xx6;
        coorrinf[9].x_s = xx6;
        coorrinf[9].y_s = roominf[dow].y_s_e + 1;
        coorrinf[9].y_e = roominf[panj].y_n_w - 1;
        coorrinf[10].x_e = xx7;
        coorrinf[10].x_s = xx7;
        coorrinf[10].y_s = roominf[panj].y_s_e + 1;
        coorrinf[10].y_e = roominf[hasht].y_n_w - 1;
    }else{
        coorrinf[10].x_e = -1;
        coorrinf[10].x_s = -1;
        coorrinf[10].y_s = -1;
        coorrinf[10].y_e = -1;
        coorrinf[9].x_e = -1;
        coorrinf[9].x_s = -1;
        coorrinf[9].y_s = -1;
        coorrinf[9].y_e = -1;
    }
    // entekhab otaghe ganj
    int random_room  = rand(); 
    random_room = (random_room % 9);
    //tedad tale ha
    int tool = roominf[random_room].x_s_e - roominf[random_room].x_n_w - 2;
    int arz = roominf[random_room].y_s_e - roominf[random_room].y_n_w - 2;
    int trap_num = (rand() % (tool * arz) );
    trap_num = trap_num % 8 ;
    int trapii = trap_num ;
    for(int i=0;i<trap_num;i++){
        int xrandd = (rand() % (tool-1));
        int yrandd = (rand() % (arz-1));
        init_pair(19,COLOR_RED,COLOR_BLACK);
        attron(COLOR_PAIR(19));
        mvprintw((yrandd + roominf[random_room].y_n_w + 1),(xrandd + roominf[random_room].x_n_w + 1),"^");
        trapinf[i].x = (xrandd + roominf[random_room].x_n_w + 1);
        trapinf[i].y = (yrandd + roominf[random_room].y_n_w + 1);
    }
    attroff(COLOR_PAIR(29));
    // entekhab otagh sehr 
    int random_room_magic = (rand() % 9);
    while(random_room_magic == random_room){
        random_room_magic = (rand() % 9);
    }
    int magic_number = (rand() % 4) + 1 ;
    magicinf[0].magic_type = 1;
    magicinf[1].magic_type = 2;
    magicinf[2].magic_type = 3;
    int magictypenumb = (rand()%2) + 1;
    magicinf[3].magic_type = magictypenumb;
    //chap sehr ha
    int x[4];
    int y[4];
    int nun = 0;
    int tol = roominf[random_room_magic].x_s_e - roominf[random_room_magic].x_n_w ;
    int aarz = roominf[random_room_magic].y_s_e - roominf[random_room_magic].y_n_w;
    while(nun != 4){
        int tt = (rand() % (tol - 2)) + 1;
        int aa = (rand() % (aarz - 2)) + 1;
        tt += roominf[random_room_magic].x_n_w;
        aa += roominf[random_room_magic].y_n_w;
        int find = 0;
        for(int i=0;i<4;i++){
            if(tt == x[i]){
                find = 1;
            }
        } 
        for(int i=0;i<4;i++){
            if(aa == y[i]){
                find = 1;
            }
        }
        if(find == 0){
            x[nun] = tt;
            y[nun] = aa;
            magicinf[nun].x = tt;
            magicinf[nun].y = aa;
            nun ++;
        }
    }
    //chap
    for(int i=0;i<4;i++){
        init_pair(77,COLOR_CYAN,COLOR_BLACK);
        attron(COLOR_PAIR(77) | A_BOLD);
        if(magicinf[i].magic_type == 1){
            mvprintw(magicinf[i].y,magicinf[i].x,"H");
        }else if(magicinf[i].magic_type == 2){
            mvprintw(magicinf[i].y,magicinf[i].x,"S");
        }else{
            mvprintw(magicinf[i].y,magicinf[i].x,"D");
        }
    }
    attroff(A_BOLD | COLOR_PAIR(77));
    // moshakhas kardan tale ha bedon chap anha
    int iii = 0;
    for(int i = 0; i < 9; i++) {
        if(i != random_room_magic && i != random_room) {
            iii++;
            int toll = roominf[i].x_s_e - roominf[i].x_n_w;
            int arz = roominf[i].y_s_e - roominf[i].y_n_w;
            int tedad_tale = (rand() % (toll * arz));
            tedad_tale = tedad_tale % 7 + 1; 
            for(int j = 0; j < tedad_tale; j++) {
                int tolss = (rand() % (toll - 2)) + 1;
                int arzs = (rand() % (arz - 2)) + 1;
                trapinf[iii + j + trapii].x = tolss + roominf[i].x_n_w;
                trapinf[iii + j + trapii].y = arzs + roominf[i].y_n_w;
            }
        }
    }
    // entekhab aslahe
    // 4 ta az 5 noe entekhab mishavand
    int ntypew = (rand() % 4) + 1;
    int iui = 0;
    for(int i = 1; i < 5; i++) {
        if(i != ntypew) {
            weaponinf[iui].weapon_type = i;
            iui++;
        }
    } 

    //mokhtasate aslahe ha
    int weloc[4];
    int nuwe = 0;
    while(nuwe != 4) {
        int map_s = (rand() % 9);
        int find = 0;
        for(int i = 0; i < 4; i++) {
            if(weloc[i] == map_s) {
                find = 1;
            }
        }
        if(find == 0) {
            int towl = roominf[map_s].x_s_e - roominf[map_s].x_n_w;
            int arwz = roominf[map_s].y_s_e - roominf[map_s].y_n_w;
            towl = ((rand() % (towl - 1)) + 1);
            arwz = ((rand() % (arwz - 1)) + 1);
            weaponinf[nuwe].x = roominf[map_s].x_n_w + towl;
            weaponinf[nuwe].y = roominf[map_s].y_n_w + arwz;
            nuwe++;
        }
    }

    // chap aslahe
    init_pair(34, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(34) | A_BOLD);
    for(int i = 0; i < 4; i++) {
        if(weaponinf[i].weapon_type == 1) { //gorz
            mvprintw(weaponinf[i].y, weaponinf[i].x, "/");
        } else if(weaponinf[i].weapon_type == 2) { //khanjar
            mvprintw(weaponinf[i].y, weaponinf[i].x, "}");
        } else if(weaponinf[i].weapon_type == 3) { //asaye jadoii
            mvprintw(weaponinf[i].y, weaponinf[i].x, "|");
        } else if(weaponinf[i].weapon_type == 4) { // tire aadi
            mvprintw(weaponinf[i].y, weaponinf[i].x, "*");
        } else { // shamshir
            mvprintw(weaponinf[i].y, weaponinf[i].x, "~");
        }
    }
    attroff(COLOR_PAIR(34) | A_BOLD);
    //chap ghaza dar otagh haye: yek swe panj haft nooh
    foodinf[0].x = roominf[yek].x_n_w + 1;
    foodinf[0].y = roominf[yek].y_n_w + 1;
    foodinf[1].x = roominf[swe].x_n_w + 1;
    foodinf[1].y = roominf[swe].y_n_w + 1;
    foodinf[2].x = roominf[panj].x_n_w + 1;
    foodinf[2].y = roominf[panj].y_n_w + 1;
    foodinf[3].x = roominf[haft].x_n_w + 1;
    foodinf[3].y = roominf[haft].y_n_w + 1;
    foodinf[4].x = roominf[nooh].x_n_w + 1;
    foodinf[4].y = roominf[nooh].y_n_w + 1;
    init_pair(99,COLOR_GREEN,COLOR_BLACK);
    attron(COLOR_PAIR(99) | A_ITALIC | A_BOLD);
    for(int i=0;i<5;i++){
        mvprintw(foodinf[i].y,foodinf[i].x,"$");
    }
    attroff(COLOR_PAIR(99) | A_ITALIC | A_BOLD);
    //print character 
    // tabe joda nasakhtam chon bug mikhordm
    int kodom_otagh;//dar otaghe ganj nabashad
    int ty=0;
    while(ty == 0){
        kodom_otagh = rand()%9;
        if(kodom_otagh == random_room){
            ty = 0;
        }else{
            ty = 1;
        }
    }
    characinf.loc.x = roominf[kodom_otagh].x_s_e - 1;
    characinf.loc.y = roominf[kodom_otagh].y_n_w + 1;
    int prev_x = characinf.loc.x;
    int prev_y = characinf.loc.y;
    int kodoom=0; // 0 = .       1 = #    2 = =
    init_pair(432,COLOR_BLUE,COLOR_BLACK);
    int weapon_num = 0;//tedad selah haii ke dare
    int magic_num = 0;
    characinf.health = 100;
    characinf.score = 0;
    characinf.weapon_typ = 0;
    while (1) {
        curs_set(0);
        start_color();
        if(kodoom == 0){
            attron(A_BOLD | COLOR_PAIR(15));
            mvprintw(prev_y, prev_x, ".");
            attroff(A_BOLD | COLOR_PAIR(15));
        }else if(kodoom == 1){
            mvprintw(prev_y, prev_x, "#");
        }else{
            mvprintw(prev_y, prev_x, "=");
        }
        refresh();
        mvprintw(characinf.loc.y, characinf.loc.x, "@");
        refresh();
        prev_x = characinf.loc.x;
        prev_y = characinf.loc.y;
        char choice;
        scanf(" %c", &choice); // Vared kardan-e yek character
        // switch (choice) {
        //     case KEY_UP:
        //         characinf.loc.y--;
        //         break;
        //     case KEY_DOWN:
        //         characinf.loc.y++;
        //         break;
        //     case KEY_RIGHT:
        //         characinf.loc.x++;
        //         break;
        //     case KEY_LEFT:
        //         characinf.loc.x--;
        //         break;
        //     case 'q':
        //         return;
        // }
        if (choice == 'w') { // Baraye KEY_UP
            characinf.loc.y--;
        } else if (choice == 's') { // Baraye KEY_DOWN
            characinf.loc.y++;
        } else if (choice == 'd') { // Baraye KEY_RIGHT
            characinf.loc.x++;
        } else if (choice == 'a') { // Baraye KEY_LEFT
            characinf.loc.x--;
        } else if (choice == 'q') {
            return;
        }
        mvprintw(33,103,"                               ");
        int in_bounds = 0;
        for (int i = 0; i < 9; i++) {
            if (characinf.loc.x > roominf[i].x_n_w && characinf.loc.x < roominf[i].x_s_e &&
                characinf.loc.y > roominf[i].y_n_w && characinf.loc.y < roominf[i].y_s_e) {
                in_bounds = 1;
                break;
            }
        }

        for (int corrn = 0; corrn < 11; corrn++) {
            if (coorrinf[corrn].x_e != -1) {
                if (coorrinf[corrn].x_e == coorrinf[corrn].x_s) {
                    if (characinf.loc.x >= coorrinf[corrn].x_s && characinf.loc.x <= coorrinf[corrn].x_e &&
                        characinf.loc.y >= (coorrinf[corrn].y_s - 1) && characinf.loc.y <= (coorrinf[corrn].y_e + 1)) {
                        in_bounds = 1;
                        break;
                    }
                } else {
                    if (characinf.loc.x >= (coorrinf[corrn].x_s - 1) && characinf.loc.x <= (coorrinf[corrn].x_e + 1) &&
                        characinf.loc.y >= coorrinf[corrn].y_s && characinf.loc.y <= coorrinf[corrn].y_e) {
                        in_bounds = 1;
                        break;
                    }
                }
            }
        }
        kodoom = 0;
        for (int corrn = 0; corrn < 11; corrn++) {
            if (coorrinf[corrn].x_e != -1) {
                if (coorrinf[corrn].y_e == coorrinf[corrn].y_s) {
                    if((prev_x == coorrinf[corrn].x_s - 1  || prev_x == coorrinf[corrn].x_e + 1) && prev_y == coorrinf[corrn].y_s){
                        kodoom = 2;
                    }else if(prev_y == coorrinf[corrn].y_s && prev_x >= coorrinf[corrn].x_s && prev_x <= coorrinf[corrn].x_e ){
                        kodoom = 1;
                    }
                } else {//x hashon barabar
                    if((prev_y == coorrinf[corrn].y_s - 1  || prev_y == coorrinf[corrn].y_e + 1) && prev_x == coorrinf[corrn].x_s){
                        kodoom = 2;
                    }else if(prev_x == coorrinf[corrn].x_s && prev_y >= coorrinf[corrn].y_s && prev_y <= coorrinf[corrn].y_e){
                        kodoom = 1;
                    }
                }
            }
        }

        if (!in_bounds) {
            characinf.loc.x = prev_x;
            characinf.loc.y = prev_y;
        }
        //ro tale ha ast ya na?
        for(int i=0;i<1000;i++){
            if(trapinf[i].x == characinf.loc.x && trapinf[i].y == characinf.loc.y){
                characinf.health = characinf.health - 5;
                //COLOR_RED  19
                attron(COLOR_PAIR(19));
                mvprintw(33,103,"!!YOU WENT ON THE TRAP!!");
                mvprintw(prev_y,prev_x,"@");
                attroff(COLOR_PAIR(19));
                trapinf[i].x = -1;
                trapinf[i].y = -1;
                break;
            }
        }
        //ro ghaza ha ast ya na?
        for(int i=0;i<5;i++){
            if(foodinf[i].x == characinf.loc.x && foodinf[i].y == characinf.loc.y){
                characinf.foods ++;
                mvprintw(33,107,"!!YOU GOT FOOD!!");
                foodinf[i].x = -1;
                foodinf[i].y = -1;
            }
        }
        //sela ha        
        for(int i=0;i<4;i++){
            if(weaponinf[i].x == characinf.loc.x && weaponinf[i].y == characinf.loc.y){
                characinf.weapons[weapon_num] = weaponinf[i].weapon_type;
                weapon_num ++;
                mvprintw(33,107,"!!WEAPON ADDED!!");
                weaponinf[i].x = -1;
                weaponinf[i].y = -1;
            }
        }
        //telesm ha
        for(int i=0;i<4;i++){
            if(magicinf[i].x == characinf.loc.x && weaponinf[i].y == characinf.loc.y){
                characinf.magics[magic_num] = magicinf[i].magic_type;
                magic_num ++;
                magicinf[i].x = -1;
                magicinf[i].y = -1;
            }
        }
        //chap safheye etela'at dar samte CHAP safhe
        ///////////////////////////////////////////   chap           ////////////////////////////////
        ///////////////////////////////////////////   chap           ////////////////////////////////
        attron(A_BOLD | COLOR_PAIR(19));
        for(int i=92;i<141;i++){
            mvprintw(0,i,".");
            //mvprintw(0,i,".");
            //mvprintw(35,i,".");
            mvprintw(35,i,".");
        }
        for(int i=1;i<36;i++){
            //mvprintw(i,92,".");
            mvprintw(i,92,".");
            //mvprintw(i,140,".");
            mvprintw(i,140,".");
        }
        attroff(COLOR_PAIR(19));
        user_information user;
        mvprintw(1,93,"Your score : %d",characinf.score);
        int heal = characinf.health;
        mvprintw(3,93,"Your health : %d",heal);
        mvprintw(5,93,"Number of your foods: %d (Press e to use food)",characinf.foods);
        //list aslahe ha
        int x2 = 93,y2 =8;
        move(y2,x2);
        mvprintw(7,110,"Your weapons list :");
        move(7,113);
        for(int i=0;i<weapon_num;i++){
            if(weaponinf[i].weapon_type == 1){
                mvprintw(y2,x2,"GORZ ");
                x2+=7;
            }else if(weaponinf[i].weapon_type == 2){
                mvprintw(y2,x2,"KHANJAR ");
                x2+=10;
            }else if(weaponinf[i].weapon_type == 3){
                mvprintw(y2,x2,"ASAYE JADOII ");
                x2+=15;
            }else if(weaponinf[i].weapon_type == 4){
                mvprintw(y2,x2,"TIRE A'DI ");
                x2+=12;
            }else{
                mvprintw(y2,x2,"SHAMSHIR ");
                x2+=10;
            }
        }
        attroff(A_BOLD);
        //estefade az ghaza
        if (choice == 101) { // barabre e bod;
            if (characinf.foods == 0) {
                mvprintw(33, 107, "!!YOU HAVE NO FOOD TO EAT!!");
                refresh(); // Refresh safhe
                goto here;
            }
            if (characinf.health == 100) {
                mvprintw(33, 109, "!!YOUR HEALTH IS FULL!!");
                refresh(); // Refresh safhe
                goto here;
            }
            mvprintw(33, 106, "!!YOU ATE ONE OF THE FOODS!!");
            characinf.health += 5;
            characinf.foods--;
        }
        here:
        //ebtekhab selah 
        mvprintw(9,93,"Press c to chose weapon");
        if(choice == 'c'){
            onja:
            mvprintw(9,93,"Selcet the weapon number from the specified list");
            int choice2;
            scanf("%d",&choice2);
            if(choice2 > (weapon_num + 1)){
                mvprintw(10,93,"Please select correct number");
                goto onja;
            }
            if(choice2 == 1){
                characinf.weapon_typ = characinf.weapons[0];
            }else if(choice2 == 2){
                characinf.weapon_typ = characinf.weapons[1];
            }else if(choice2 == 3){
                characinf.weapon_typ = characinf.weapons[2];
            }else if(choice2 == 4){
                characinf.weapon_typ = characinf.weapons[3];
            }else{
                mvprintw(10,93,"Please select correct number");
                goto onja;
            }
        }
        if(characinf.weapon_typ==0){
            mvprintw(12,93,"You do not have a weapon in your hand right now!");
        }else{
            if (characinf.weapon_typ == 1){
                mvprintw(12,93,"You weapon : GORZ");
            }else if(characinf.weapon_typ == 2){
                mvprintw(12,93,"You weapon : KHANJAR");
            }else if (characinf.weapon_typ == 3){
                mvprintw(12,93,"You weapon : ASAYE JADOII");
            }else if(characinf.weapon_typ == 4){
                mvprintw(12,93,"You weapon : TIRE A'DI");
            }else{
                mvprintw(12,93,"You weapon : SHAMSHIR");
            }
            
        }
        

    }



    refresh();
    getch();
    clear();
}




// void print_character(int n_wx, int n_wy, int floor_length, int floor_width) {
//     start_color();
//     character_information character;
//     character.loc.x = n_wx + 1;
//     character.loc.y = n_wy + 1;
//     init_pair(44, COLOR_CYAN, COLOR_BLACK);
//     attron(COLOR_PAIR(44) | A_BOLD);
//     echo();
//     mvprintw(character.loc.y, character.loc.x, "#");
//     while (true) {
//         echo();
//         int choice = getch();
//         if (choice == KEY_UP) {
//             if (character.loc.y - 1 > n_wy) {
//                 mvprintw(character.loc.y, character.loc.x, "."); 
//                 character.loc.y--;
//             }
//         } else if (choice == KEY_DOWN) {
//             if (character.loc.y + 1 < n_wy + floor_width) {
//                 mvprintw(character.loc.y, character.loc.x, ".");
//                 character.loc.y++;
//             }
//         } else if (choice == KEY_RIGHT) {
//             if (character.loc.x + 1 < n_wx + floor_length) {
//                 mvprintw(character.loc.y, character.loc.x, ".");
//                 character.loc.x++;
//             }
//         } else if (choice == KEY_LEFT) {
//             if (character.loc.x - 1 > n_wx) {
//                 mvprintw(character.loc.y, character.loc.x, ".");
//                 character.loc.x--;
//             }
//         } else if (choice == 'q') {
//             break;
//         }
//         mvprintw(character.loc.y, character.loc.x, "#");
//         move(0,0);
//         refresh();
//     }
//     attroff(COLOR_PAIR(44) | A_BOLD);
//     clear();
//     noecho();
// }
