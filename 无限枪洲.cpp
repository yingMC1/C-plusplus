// 无限枪洲 - 最终版（击杀数共享，金钱私有，仅合作模式）
// 编译: g++ -std=c++17 -O2 -s -static -o 无限枪洲.exe main.cpp -lws2_32

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <conio.h>
#include <random>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <cstring>
#include <direct.h>
#include <errno.h>
#include <algorithm>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace chrono;

inline void wca(HANDLE h, const char *s) {
    DWORD w;
    WriteConsoleA(h, s, (long long)strlen(s), &w, NULL);
}

// ========== 常量 ==========
const long long MS = 40;
const long long VR = 12;
const long long EH = 55;
const long long ED = 14;
const long long PD = 15;
const long long ES = 5, HS = 3, TO = 5000;
const long long NET_PORT = 8888;
const long long CONSOLE_COLS = 80;
const long long CONSOLE_ROWS = 30;
const long long SYNC_INTERVAL = 50;
const long long MAX_SHOOT_RANGE = 5;
const double MISS_PROB = 0.4;

// ========== 游戏状态 ==========
long long mp[MS][MS] = {0};
long long px = MS / 2, py = MS / 2;
long long am = 0, hp = 100, kc = 0, te = 0;
long long sa = 20, sm = 3, money = 200;
bool ps = false, vic = false, eva = false, hea = false;
steady_clock::time_point evT, heT, emT;

// ========== 护甲 ==========
long long armorLevel = 0, armorDura = 0, armorMaxDura = 0, armorReduce = 0;

// ========== 双缓冲 ==========
HANDLE bf[2];
long long cb = 0;
COORD sz = {CONSOLE_COLS, CONSOLE_ROWS};
char msgBuf[32] = {0};
long long mc = 7;
steady_clock::time_point me;

// ========== 敌人 ==========
struct En {
    long long x, y, hp;
    bool live, awake;
    steady_clock::time_point lst;
};
vector<En> es;
mutex esMutex;

// ========== 网络 ==========
bool net = false;
bool netReady = false;
bool isHost = false;
SOCKET sk = INVALID_SOCKET;
sockaddr_in pr;
long long prX = -1, prY = -1, prH = 100, prAm = 0, prKc = 0;
bool prL = false;
steady_clock::time_point lr;
atomic<bool> st(false);
thread rt;
thread stt;
string lip;
mutex mtx;
string savePath = "C:\\doiu\\zi.dat";
bool gameRunning = false;
atomic<bool> mapSynced(false);
bool hostDisconnected = false;

// ========== 颜色 ==========
enum {
    BK = 0,
    DR = 4,
    DG = 2,
    DY = 6,
    GY = 8,
    r = 12,
    G = 10,
    Y = 14,
    CYN = 11,
    BL = 9,
    W = 15,
    PURPLE = 13
};

// ========== 快速随机 ==========
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count() ^
               (uint64_t)GetCurrentProcessId());
inline long long fastRand(long long mod) {
    return (long long)(rng() % (unsigned long long)max(1LL, mod));
}
inline double fastRandDouble() {
    return (double)rng() / (double)UINT64_MAX;
}

inline void sc(HANDLE h, long long c) {
    SetConsoleTextAttribute(h, (WORD)c);
}
inline void got(HANDLE h, long long x, long long y) {
    COORD c = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(h, c);
}
inline void show(const char *s, long long c, long long ms = 1500) {
    strncpy(msgBuf, s, 31);
    msgBuf[31] = 0;
    mc = c;
    me = steady_clock::now() + milliseconds(ms);
}

// ========== 视线 ==========
inline bool los(long long x1, long long y1, long long x2, long long y2) {
    long long dx = abs(x2 - x1), dy = abs(y2 - y1);
    long long sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1;
    long long err = dx - dy, x = x1, y = y1;
    while (!(x == x2 && y == y2)) {
        if ((x != x1 || y != y1) && mp[y][x] == 1) return false;
        long long e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
    return true;
}

// ========== 视野缓存 ==========
bool visible[MS][MS];
long long last_px = -1, last_py = -1;

void computeVisibility() {
    if (px == last_px && py == last_py) return;
    last_px = px;
    last_py = py;

    long long xmin = max(0LL, px - VR), xmax = min(MS - 1, px + VR);
    long long ymin = max(0LL, py - VR), ymax = min(MS - 1, py + VR);

    for (long long y = ymin; y <= ymax; ++y)
        for (long long x = xmin; x <= xmax; ++x) visible[y][x] = false;
    visible[py][px] = true;

    const long long dirs[8][2] = {{1, 0},  {1, 1},   {0, 1},  {-1, 1},
                                  {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
    for (long long d = 0; d < 8; ++d) {
        long long dx = dirs[d][0], dy = dirs[d][1];
        long long x = px + dx, y = py + dy;
        for (long long step = 1;
             step <= VR && x >= 0 && x < MS && y >= 0 && y < MS; ++step) {
            visible[y][x] = true;
            if (mp[y][x] == 1) break;
            x += dx;
            y += dy;
        }
    }
    for (long long y = ymin; y <= ymax; ++y)
        for (long long x = xmin; x <= xmax; ++x)
            if (!visible[y][x] && (abs(x - px) + abs(y - py) <= VR)) {
                long long dx = x - px, dy = y - py;
                long long sx = (dx > 0) ? 1 : -1, sy = (dy > 0) ? 1 : -1;
                long long ax = abs(dx), ay = abs(dy);
                long long err = ax - ay, tx = px, ty = py;
                bool blocked = false;
                while (!(tx == x && ty == y)) {
                    if ((tx != px || ty != py) && mp[ty][tx] == 1) {
                        blocked = true;
                        break;
                    }
                    long long e2 = 2 * err;
                    if (e2 > -ay) {
                        err -= ay;
                        tx += sx;
                    }
                    if (e2 < ax) {
                        err += ax;
                        ty += sy;
                    }
                }
                if (!blocked) visible[y][x] = true;
            }
}

// ========== 护甲 ==========
struct ArmorInfo {
    long long cost, maxDura, reduce;
};
ArmorInfo armorTable[5] = {
    {0, 0, 0}, {100, 50, 4}, {200, 75, 7}, {350, 100, 10}, {550, 125, 13}};

inline long long applyDamage(long long dmg) {
    if (armorLevel > 0 && armorDura > 0) {
        long long reduced = max(1LL, dmg - armorReduce);
        armorDura -= max(1LL, dmg / 2);
        if (armorDura <= 0) {
            armorLevel = 0;
            armorDura = 0;
            armorReduce = 0;
            show("护甲破碎！", r);
        }
        return reduced;
    }
    return dmg;
}

void safeMkdir(const char *path) {
    if (_mkdir(path) != 0 && errno != EEXIST) {}
}

// ========== 存档加密 ==========
void saveData() {
    safeMkdir("C:\\doiu");
    ofstream of(savePath, ios::binary);
    if (of) {
        long long aMax = (armorLevel > 0) ? armorTable[armorLevel].maxDura : 0;
        long long aRed = (armorLevel > 0) ? armorTable[armorLevel].reduce : 0;
        long long checksum =
            sa ^ sm ^ money ^ armorLevel ^ armorDura ^ aMax ^ aRed;
        of.write((char *)&sa, sizeof(sa));
        of.write((char *)&sm, sizeof(sm));
        of.write((char *)&money, sizeof(money));
        of.write((char *)&armorLevel, sizeof(armorLevel));
        of.write((char *)&armorDura, sizeof(armorDura));
        of.write((char *)&aMax, sizeof(aMax));
        of.write((char *)&aRed, sizeof(aRed));
        of.write((char *)&checksum, sizeof(checksum));
    }
}
void loadData() {
    ifstream f(savePath, ios::binary);
    if (f) {
        long long aMax, aRed, checksum;
        f.read((char *)&sa, sizeof(sa));
        f.read((char *)&sm, sizeof(sm));
        f.read((char *)&money, sizeof(money));
        f.read((char *)&armorLevel, sizeof(armorLevel));
        f.read((char *)&armorDura, sizeof(armorDura));
        f.read((char *)&aMax, sizeof(aMax));
        f.read((char *)&aRed, sizeof(aRed));
        f.read((char *)&checksum, sizeof(checksum));
        long long calc = sa ^ sm ^ money ^ armorLevel ^ armorDura ^ aMax ^ aRed;
        if (calc != checksum) {
            sa = 20;
            sm = 3;
            money = 200;
            armorLevel = 0;
            armorDura = 0;
            armorMaxDura = 0;
            armorReduce = 0;
        } else {
            armorMaxDura = aMax;
            armorReduce = aRed;
            if (sa < 0 || sa > 9999) sa = 20;
            if (sm < 0 || sm > 999) sm = 3;
            if (money < 0 || money > 999999) money = 200;
            if (armorLevel < 0 || armorLevel > 4) armorLevel = 0;
            if (armorDura < 0) armorDura = 0;
        }
    } else {
        sa = 20;
        sm = 3;
        money = 200;
        armorLevel = 0;
        armorDura = 0;
        armorMaxDura = 0;
        armorReduce = 0;
    }
}

// ========== 商店 ==========
void shop(HANDLE h) {
    system("cls");
    sc(h, Y);
    got(h, 30, 3);
    wca(h, "+==================+");
    sc(h, CYN);
    got(h, 30, 4);
    wca(h, "|     黑 市     |");
    sc(h, Y);
    got(h, 30, 5);
    wca(h, "+==================+");

    char buf[64];
    sc(h, W);
    got(h, 25, 7);
    sprintf(buf, "金钱:%lld", money);
    WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    sc(h, W);
    got(h, 25, 8);
    sprintf(buf, "仓库子弹:%lld", sa);
    WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    sc(h, W);
    got(h, 25, 9);
    sprintf(buf, "医疗包:%lld", sm);
    WriteConsoleA(h, buf, strlen(buf), NULL, NULL);

    sc(h, Y);
    got(h, 25, 11);
    wca(h, "1. 1级护甲");
    sc(h, W);
    got(h, 37, 11);
    wca(h, "100金");
    sc(h, G);
    got(h, 43, 11);
    wca(h, "(耐久50减伤4)");

    sc(h, Y);
    got(h, 25, 12);
    wca(h, "2. 2级护甲");
    sc(h, W);
    got(h, 37, 12);
    wca(h, "200金");
    sc(h, G);
    got(h, 43, 12);
    wca(h, "(耐久75减伤7)");

    sc(h, Y);
    got(h, 25, 13);
    wca(h, "3. 3级护甲");
    sc(h, W);
    got(h, 37, 13);
    wca(h, "350金");
    sc(h, G);
    got(h, 43, 13);
    wca(h, "(耐久100减伤10)");

    sc(h, Y);
    got(h, 25, 14);
    wca(h, "4. 4级护甲");
    sc(h, W);
    got(h, 37, 14);
    wca(h, "550金");
    sc(h, G);
    got(h, 43, 14);
    wca(h, "(耐久125减伤13)");

    sc(h, Y);
    got(h, 25, 15);
    wca(h, "5. 子弹x10");
    sc(h, W);
    got(h, 37, 15);
    wca(h, "5金");

    sc(h, Y);
    got(h, 25, 16);
    wca(h, "6. 医疗包x1");
    sc(h, W);
    got(h, 37, 16);
    wca(h, "20金");

    sc(h, r);
    got(h, 25, 17);
    wca(h, "7. 返回");

    sc(h, G);
    got(h, 30, 19);
    wca(h, "选择 (1-7):");
    long long ch = 0;
    while (!ch) {
        if (_kbhit()) {
            char c = _getch();
            if (c >= '1' && c <= '7') ch = c - '0';
        }
        Sleep(30);
    }
    if (ch == 7) return;
    if (ch >= 1 && ch <= 4) {
        long long cost = armorTable[ch].cost;
        if (money >= cost) {
            money -= cost;
            armorLevel = ch;
            armorMaxDura = armorTable[ch].maxDura;
            armorDura = armorMaxDura;
            armorReduce = armorTable[ch].reduce;
            show("购买护甲成功", G);
        } else show("金钱不足", r);
    } else if (ch == 5) {
        if (money >= 5) {
            money -= 5;
            sa += 10;
            show("购买子弹x10", G);
        } else show("金钱不足", r);
    } else if (ch == 6) {
        if (money >= 20) {
            money -= 20;
            sm++;
            show("购买医疗包x1", G);
        } else show("金钱不足", r);
    }
    saveData();
    Sleep(800);
}
void toggleArmor() {
    if (armorLevel > 0) {
        armorLevel = armorDura = armorReduce = 0;
        show("护甲已脱下", Y);
    } else if (armorMaxDura > 0) show("护甲已穿上", G);
    else show("没有护甲", r);
}

// ========== 地图生成 ==========
void genMap(long long seed) {
    rng.seed(seed);
    for (long long i = 0; i < MS; ++i)
        for (long long j = 0; j < MS; ++j) mp[i][j] = 0;
    for (long long i = 0; i < MS; ++i)
        mp[i][0] = mp[i][MS - 1] = mp[0][i] = mp[MS - 1][i] = 1;
    for (long long r = 0; r < 6; ++r) {
        long long w = fastRand(5) + 3, hh = fastRand(5) + 3,
                  sx = fastRand(MS - w - 2) + 1, sy = fastRand(MS - hh - 2) + 1;
        for (long long i = sy; i < sy + hh; ++i)
            for (long long j = sx; j < sx + w; ++j) mp[i][j] = 1;
        if (w > 2) mp[sy][sx + w / 2] = 0;
    }
    for (long long i = 0; i < 150; ++i) {
        long long x = fastRand(MS - 2) + 1, y = fastRand(MS - 2) + 1;
        if (mp[y][x] == 0 && abs(x - px) + abs(y - py) > 8) mp[y][x] = 1;
    }
    for (long long c = 0; c < 6; ++c) {
        long long x, y;
        do {
            x = fastRand(MS - 2) + 1;
            y = fastRand(MS - 2) + 1;
        } while (mp[y][x]);
        mp[y][x] = 2;
    }
    for (long long c = 0; c < 3; ++c) {
        long long x, y;
        do {
            x = fastRand(MS - 2) + 1;
            y = fastRand(MS - 2) + 1;
        } while (mp[y][x]);
        mp[y][x] = 5;
    }
    for (long long c = 0; c < 8; ++c) {
        long long x, y;
        do {
            x = fastRand(MS - 2) + 1;
            y = fastRand(MS - 2) + 1;
        } while (mp[y][x] || abs(x - px) + abs(y - py) < 12);
        mp[y][x] = 3;
    }
    mp[MS - 2][MS - 2] = 4;
    mp[py][px] = 0;
    for (long long i = -2; i <= 2; ++i)
        for (long long j = -2; j <= 2; ++j) {
            long long ny = py + i, nx = px + j;
            if (ny >= 0 && ny < MS && nx >= 0 && nx < MS && mp[ny][nx] == 1)
                mp[ny][nx] = 0;
        }
}
void gen() {
    long long seed =
        (long long)chrono::steady_clock::now().time_since_epoch().count() ^
        (long long)GetCurrentThreadId();
    genMap(seed);
}
void initE() {
    es.clear();
    auto now = steady_clock::now();
    te = 0;
    for (long long i = 0; i < MS; ++i)
        for (long long j = 0; j < MS; ++j)
            if (mp[i][j] == 3) {
                es.push_back({j, i, EH, true, false, now});
                te++;
            }
    kc = 0;
}

// ========== 射击（合作模式，不可伤害队友） ==========
void shoot(long long dx, long long dy) {
    if (am <= 0) {
        show("弹药不足!", r);
        return;
    }
    long long x = px + dx, y = py + dy;
    long long step = 0;
    while (x >= 0 && x < MS && y >= 0 && y < MS) {
        step++;
        if (step > MAX_SHOOT_RANGE && fastRandDouble() < MISS_PROB) {
            am--;
            show("子弹偏离", GY);
            return;
        }
        if (mp[y][x] == 1) {
            am--;
            show("墙壁", GY);
            return;
        }
        bool hitEnemy = false;
        lock_guard<mutex> lock(esMutex);
        for (auto &e : es)
            if (e.live && e.x == x && e.y == y) {
                e.hp -= PD;
                if (e.hp <= 0) {
                    e.live = false;
                    mp[y][x] = 0;
                    kc++;        // 击杀数增加（共享）
                    money += 10; // 金钱私有
                    show("击杀+1 +10金", G);
                    if (net) {
                        char buf[64];
                        sprintf(buf, "ENEMY_DEAD %d %d", (int)x, (int)y);
                        sendto(sk, buf, strlen(buf), 0, (sockaddr *)&pr,
                               sizeof(pr));
                    }
                } else {
                    show("击中敌人", r);
                }
                am--;
                for (auto &e2 : es)
                    if (!e2.awake && abs(e2.x - px) + abs(e2.y - py) <= 12)
                        e2.awake = true;
                hitEnemy = true;
                break;
            }
        if (hitEnemy) { return; }
        // 检查队友（合作模式禁止伤害）
        if (net) {
            long long pX = -1, pY = -1;
            bool pL = false;
            {
                lock_guard<mutex> lock(mtx);
                pX = prX;
                pY = prY;
                pL = prL;
            }
            if (pL && pX == x && pY == y) {
                am--;
                show("队友不能伤害", r);
                return;
            }
        }
        x += dx;
        y += dy;
        if (abs(x - px) > 20) break;
    }
    am--;
    show("未命中", GY);
    if (isHost || !net) {
        lock_guard<mutex> lock(esMutex);
        for (auto &e2 : es)
            if (!e2.awake && abs(e2.x - px) + abs(e2.y - py) <= 12)
                e2.awake = true;
    }
}

// ========== 敌人AI ==========
long long enemyInterval = 700;

void enemy() {
    if (!isHost && net) return; // 只有主机运行AI
    static auto lastUpdate = steady_clock::now();
    auto now = steady_clock::now();

    if (duration_cast<milliseconds>(now - lastUpdate).count() < enemyInterval)
        return;
    lastUpdate = now;

    lock_guard<mutex> lock(esMutex);
    for (auto &e : es) {
        if (!e.live) continue;
        long long dist = abs(e.x - px) + abs(e.y - py);
        if (!e.awake && dist <= 10 && los(e.x, e.y, px, py)) {
            e.awake = true;
            show("敌人惊醒！", Y, 800);
        }
        if (!e.awake) {
            if (fastRand(100) < 6) {
                long long dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
                long long d = fastRand(4), nx = e.x + dirs[d][0],
                          ny = e.y + dirs[d][1];
                if (nx >= 0 && nx < MS && ny >= 0 && ny < MS &&
                    mp[ny][nx] == 0) {
                    mp[e.y][e.x] = 0;
                    e.x = nx;
                    e.y = ny;
                    mp[e.y][e.x] = 3;
                }
            }
            continue;
        }
        // 追玩家
        long long dx = 0, dy = 0;
        if (px > e.x) dx = 1;
        else if (px < e.x) dx = -1;
        if (py > e.y) dy = 1;
        else if (py < e.y) dy = -1;
        long long nx = e.x + dx, ny = e.y + dy;
        if (nx >= 0 && nx < MS && ny >= 0 && ny < MS && mp[ny][nx] == 0 &&
            !(nx == px && ny == py)) {
            mp[e.y][e.x] = 0;
            e.x = nx;
            e.y = ny;
            mp[e.y][e.x] = 3;
        } else if (dx != 0) {
            nx = e.x + dx;
            ny = e.y;
            if (nx >= 0 && nx < MS && ny >= 0 && ny < MS && mp[ny][nx] == 0 &&
                !(nx == px && ny == py)) {
                mp[e.y][e.x] = 0;
                e.x = nx;
                e.y = ny;
                mp[e.y][e.x] = 3;
            }
        } else if (dy != 0 && !(e.x == px && e.y == py)) {
            nx = e.x;
            ny = e.y + dy;
            if (nx >= 0 && nx < MS && ny >= 0 && ny < MS && mp[ny][nx] == 0) {
                mp[e.y][e.x] = 0;
                e.x = nx;
                e.y = ny;
                mp[e.y][e.x] = 3;
            }
        }
        // 近战碰撞
        if (abs(e.x - px) <= 1 && abs(e.y - py) <= 1 &&
            !(e.x == px && e.y == py)) {
            long long dmg = applyDamage(ED);
            hp -= dmg;
            char tmp[32];
            sprintf(tmp, "撞击-%d", (int)dmg);
            show(tmp, r);
            if (hp <= 0) return;
            if (eva || hea) {
                eva = hea = false;
                show("打断", r);
            }
        }
        // 远程射击（视线或距离≤3）
        if (duration_cast<milliseconds>(now - e.lst).count() >= 2500 &&
            dist <= 8) {
            bool canShoot = los(e.x, e.y, px, py) || dist <= 3;
            if (canShoot) {
                e.lst = now;
                long long dmg = applyDamage(ED);
                hp -= dmg;
                char tmp[32];
                sprintf(tmp, "射击-%d", (int)dmg);
                show(tmp, r);
                if (hp <= 0) return;
                if (eva || hea) {
                    eva = hea = false;
                    show("打断", r);
                }
            }
        }
    }
}

// ========== 网络同步 ==========
void sendFullSync() {
    if (!net || !gameRunning || !isHost) return;
    stringstream ss;
    ss << "SYNC ";
    ss << px << " " << py << " " << hp << " " << am << " " << kc << " ";
    lock_guard<mutex> lock(esMutex);
    ss << es.size() << " ";
    for (auto &e : es) {
        ss << e.x << " " << e.y << " " << e.hp << " " << (e.live ? 1 : 0) << " "
           << (e.awake ? 1 : 0) << " ";
    }
    ss << "MAP ";
    for (long long y = 0; y < MS; y++) {
        for (long long x = 0; x < MS; x++) { ss << mp[y][x] << " "; }
    }
    string data = ss.str();
    sendto(sk, data.c_str(), (long long)data.length(), 0, (sockaddr *)&pr,
           sizeof(pr));
}

void sendSync() {
    if (!net || !gameRunning) return;
    stringstream ss;
    ss << "POS " << px << " " << py << " " << hp << " " << am << " " << kc;
    string data = ss.str();
    sendto(sk, data.c_str(), (long long)data.length(), 0, (sockaddr *)&pr,
           sizeof(pr));
}

void syncThread() {
    while (!st && gameRunning) {
        if (netReady && prL) {
            sendSync();
            static long long syncCounter = 0;
            if (++syncCounter % 5 == 0 && isHost) { sendFullSync(); }
        }
        Sleep(SYNC_INTERVAL);
    }
}

void recvData() {
    char buf[4096];
    sockaddr_in from;
    int fl = sizeof(from);
    char ipaddr[32];
    strcpy(ipaddr, inet_ntoa(pr.sin_addr));

    u_long mode = 1;
    ioctlsocket(sk, FIONBIO, &mode);

    while (!st) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sk, &fds);
        timeval tv = {0, 100};

        if (select(0, &fds, NULL, NULL, &tv) > 0 && FD_ISSET(sk, &fds)) {
            long long len =
                recvfrom(sk, buf, sizeof(buf) - 1, 0, (sockaddr *)&from, &fl);
            if (len > 0) {
                buf[len] = 0;
                char senderIP[32];
                strcpy(senderIP, inet_ntoa(from.sin_addr));
                if (strcmp(senderIP, ipaddr) != 0) continue;

                string data(buf);
                stringstream ss(data);
                string cmd;
                ss >> cmd;

                if (cmd == "POS") {
                    long long x, y, h, a, k;
                    ss >> x >> y >> h >> a >> k;
                    lock_guard<mutex> lock(mtx);
                    prX = x;
                    prY = y;
                    prH = h;
                    prAm = a;
                    prKc = k;
                    prL = (h > 0);
                    lr = steady_clock::now();
                    netReady = true;
                } else if (cmd == "HIT") {
                    long long dmg;
                    ss >> dmg;
                    lock_guard<mutex> lock(mtx);
                    if (prL) {
                        long long actual = applyDamage(dmg);
                        prH -= actual;
                        if (prH <= 0) {
                            prL = false;
                            show("队友被击败", Y, 2000);
                            vic = true;
                        }
                    }
                } else if (cmd == "SYNC" && isHost == false) {
                    long long x, y, h, a, k, ecnt;
                    ss >> x >> y >> h >> a >> k >> ecnt;

                    lock_guard<mutex> lock(esMutex);
                    es.clear();
                    for (long long i = 0; i < ecnt && i < 20; i++) {
                        long long ex, ey, eh, el, eaw;
                        ss >> ex >> ey >> eh >> el >> eaw;
                        En e;
                        e.x = ex;
                        e.y = ey;
                        e.hp = eh;
                        e.live = (el == 1);
                        e.awake = (eaw == 1);
                        e.lst = steady_clock::now();
                        es.push_back(e);
                    }
                    te = ecnt;
                    string mapTag;
                    ss >> mapTag;
                    if (mapTag == "MAP") {
                        for (long long y = 0; y < MS; y++) {
                            for (long long x = 0; x < MS; x++) {
                                ss >> mp[y][x];
                            }
                        }
                        mapSynced = true;
                    }
                    netReady = true;
                } else if (cmd == "PICKUP") {
                    long long tx, ty, type;
                    ss >> tx >> ty >> type;
                    lock_guard<mutex> lock(esMutex);
                    if (mp[ty][tx] == type) { mp[ty][tx] = 0; }
                } else if (cmd == "ENEMY_DEAD") {
                    long long ex, ey;
                    ss >> ex >> ey;
                    // 所有玩家都处理：击杀数共享，金钱私有（已在击杀者本地增加）
                    lock_guard<mutex> lock(esMutex);
                    for (auto &e : es) {
                        if (e.x == ex && e.y == ey && e.live) {
                            e.live = false;
                            mp[ey][ex] = 0;
                            kc++; // 共享击杀数，让客户端也能看到击杀进度
                            show("敌人被队友击杀", G);
                            break;
                        }
                    }
                } else if (cmd == "EVAC") {
                    long long status;
                    ss >> status;
                    lock_guard<mutex> lock(mtx);
                    if (status == 1) {
                        eva = true;
                        evT = steady_clock::now();
                        show("队友撤离中", G);
                    } else if (status == 2) {
                        show("队友撤离成功！", G, 2000);
                        vic = true;
                    }
                }
            }
        }
        // 检测主机断开（仅客户端）
        if (!isHost && net && !st) {
            if (duration_cast<milliseconds>(steady_clock::now() - lr).count() >
                TO) {
                lock_guard<mutex> lock(mtx);
                prL = false;
                netReady = false;
                if (!st) {
                    show("主机已断开，游戏结束", r, 3000);
                    hostDisconnected = true;
                    vic = true;
                }
            }
        }
        Sleep(10);
    }

    u_long mode0 = 0;
    ioctlsocket(sk, FIONBIO, &mode0);
}

// ========== 渲染 ==========
void draw(HANDLE h, long long cm) {
    const long long mapW = VR * 2 + 1;
    const long long mapH = VR * 2 + 1;
    long long sx = px - VR, sy = py - VR;
    COORD mapStart = {0, 0}, uiStart = {(SHORT)(mapW + 2), 0};
    DWORD w;

    FillConsoleOutputCharacter(h, ' ', (DWORD)(mapW * mapH), mapStart, &w);
    FillConsoleOutputAttribute(h, 7, (DWORD)(mapW * mapH), mapStart, &w);

    long long uiW = sz.X - (mapW + 2);
    if (uiW > 0) {
        FillConsoleOutputCharacter(h, ' ', (DWORD)(uiW * sz.Y), uiStart, &w);
        FillConsoleOutputAttribute(h, 7, (DWORD)(uiW * sz.Y), uiStart, &w);
    }

    computeVisibility();

    lock_guard<mutex> lock(esMutex);
    static bool enemyHere[MS][MS] = {false};
    memset(enemyHere, 0, sizeof(enemyHere));
    for (auto &e : es)
        if (e.live) enemyHere[e.y][e.x] = true;

    for (long long y = 0; y < mapH; ++y) {
        got(h, 0, y);
        long long wy = sy + y;
        for (long long x = 0; x < mapW; ++x) {
            long long wx = sx + x;
            if (wx < 0 || wx >= MS || wy < 0 || wy >= MS) {
                sc(h, GY);
                wca(h, "#");
                continue;
            }
            if (!visible[wy][wx]) {
                sc(h, GY);
                wca(h, "?");
                continue;
            }
            if (enemyHere[wy][wx]) {
                sc(h, r);
                wca(h, "E");
            } else if (wx == px && wy == py) {
                sc(h, G);
                wca(h, "@");
            } else {
                long long pX = -1, pY = -1, pL = 0;
                if (net) {
                    lock_guard<mutex> lock(mtx);
                    pX = prX;
                    pY = prY;
                    pL = prL;
                }
                if (net && pL && pX == wx && pY == wy) {
                    sc(h, BL);
                    wca(h, "♦");
                } else {
                    char ch = '?';
                    long long color = W;
                    switch (mp[wy][wx]) {
                    case 0:
                        ch = '.';
                        color = GY;
                        break;
                    case 1:
                        ch = '#';
                        color = Y;
                        break;
                    case 2:
                        ch = '$';
                        color = Y;
                        break;
                    case 4:
                        ch = 'G';
                        color = G;
                        break;
                    case 5:
                        ch = '+';
                        color = CYN;
                        break;
                    default:
                        ch = '?';
                        color = W;
                    }
                    sc(h, color);
                    WriteConsoleA(h, &ch, 1, NULL, NULL);
                }
            }
        }
    }

    long long uiX = mapW + 2;
    long long ln = 0;
    char buf[64];

    sc(h, CYN);
    got(h, uiX, ln);
    wca(h, "+============+");
    ln++;
    sc(h, CYN);
    got(h, uiX, ln);
    wca(h, "|  状 态 栏  |");
    ln++;
    sc(h, CYN);
    got(h, uiX, ln);
    wca(h, "+============+");
    ln++;
    ln++;

    if (hp > 70) sc(h, G);
    else if (hp > 40) sc(h, Y);
    else sc(h, r);
    got(h, uiX, ln);
    sprintf(buf, "HP: %lld/100", hp);
    WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    ln++;

    sc(h, Y);
    got(h, uiX, ln);
    sprintf(buf, "弹药: %lld", am);
    WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    ln++;
    sc(h, CYN);
    got(h, uiX, ln);
    sprintf(buf, "医疗: %lld", cm);
    WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    ln++;
    sc(h, r);
    got(h, uiX, ln);
    sprintf(buf, "击杀: %lld/%lld", kc, te);
    WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    ln++;

    if (armorLevel > 0) {
        sc(h, BL);
        got(h, uiX, ln);
        sprintf(buf, "护甲%d: %lld/%lld", (int)armorLevel, armorDura,
                armorMaxDura);
        WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
        ln++;
    } else {
        sc(h, GY);
        got(h, uiX, ln);
        wca(h, "无护甲");
        ln++;
    }

    if (net) {
        long long ph = 0;
        bool pl = false;
        {
            lock_guard<mutex> lock(mtx);
            ph = prH;
            pl = prL;
        }
        sc(h, PURPLE);
        got(h, uiX, ln);
        if (pl) {
            if (ph > 70) sc(h, G);
            else if (ph > 40) sc(h, Y);
            else sc(h, r);
            sprintf(buf, "队友HP: %lld", ph);
            WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
        } else {
            sc(h, GY);
            wca(h, "等待队友...");
        }
        ln++;
    }

    ln++;
    sc(h, W);
    got(h, uiX, ln);
    if (eva) {
        long long r = max(0LL, ES - (long long)duration_cast<seconds>(
                                        steady_clock::now() - evT)
                                        .count());
        sc(h, G);
        sprintf(buf, "撤离:%ds", (int)r);
        WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    } else if (hea) {
        long long r = max(0LL, HS - (long long)duration_cast<seconds>(
                                        steady_clock::now() - heT)
                                        .count());
        sc(h, CYN);
        sprintf(buf, "治疗:%ds", (int)r);
        WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    } else if (vic) {
        sc(h, Y);
        wca(h, "胜利!");
    } else {
        sc(h, W);
        wca(h, "WASD移动 K护甲");
    }
    ln++;

    if (msgBuf[0] && steady_clock::now() < me) {
        sc(h, mc);
        got(h, uiX, ln);
        sprintf(buf, "%-16s", msgBuf);
        WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    } else {
        sc(h, GY);
        got(h, uiX, ln);
        wca(h, "                ");
        msgBuf[0] = 0;
    }
    ln++;

    sc(h, Y);
    got(h, uiX, ln);
    wca(h, "方向键射击");
    ln++;
    sc(h, CYN);
    got(h, uiX, ln);
    wca(h, "E拾取 H治疗");
    ln++;
    sc(h, G);
    got(h, uiX, ln);
    wca(h, "Q撤离 P暂停");
    ln++;
    sc(h, r);
    got(h, uiX, ln);
    wca(h, "F取消操作");
    ln++;

    if (ps) {
        sc(h, r);
        got(h, 0, 0);
        wca(h, "[暂停]");
    }
}

// ========== 操作指南 ==========
void showHelp() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    sc(h, Y);
    got(h, 28, 1);
    wca(h, "+====================================+");
    sc(h, CYN);
    got(h, 30, 2);
    wca(h, "|       操 作 指 南       |");
    sc(h, Y);
    got(h, 28, 3);
    wca(h, "+====================================+");

    sc(h, W);
    got(h, 20, 5);
    wca(h, "[移动]");
    sc(h, G);
    got(h, 28, 5);
    wca(h, "WASD 键");

    sc(h, W);
    got(h, 20, 6);
    wca(h, "[射击]");
    sc(h, r);
    got(h, 28, 6);
    wca(h, "方向键 (上 下 左 右)");

    sc(h, W);
    got(h, 20, 7);
    wca(h, "[拾取]");
    sc(h, Y);
    got(h, 28, 7);
    wca(h, "E 键 (弹药/医疗包)");

    sc(h, W);
    got(h, 20, 8);
    wca(h, "[治疗]");
    sc(h, CYN);
    got(h, 28, 8);
    wca(h, "H 键 (使用医疗包)");

    sc(h, W);
    got(h, 20, 9);
    wca(h, "[护甲]");
    sc(h, BL);
    got(h, 28, 9);
    wca(h, "K 键 (穿脱护甲)");

    sc(h, W);
    got(h, 20, 10);
    wca(h, "[撤离]");
    sc(h, G);
    got(h, 28, 10);
    wca(h, "Q 键 (在撤离点G上)");

    sc(h, W);
    got(h, 20, 11);
    wca(h, "[暂停]");
    sc(h, PURPLE);
    got(h, 28, 11);
    wca(h, "P 键");

    sc(h, W);
    got(h, 20, 12);
    wca(h, "[取消]");
    sc(h, r);
    got(h, 28, 12);
    wca(h, "F 键");

    sc(h, Y);
    got(h, 24, 14);
    wca(h, "========== 图 标 说 明 ==========");

    sc(h, G);
    got(h, 20, 15);
    wca(h, " @  玩家自己");
    sc(h, BL);
    got(h, 20, 16);
    wca(h, " ♦  队友");
    sc(h, r);
    got(h, 20, 17);
    wca(h, " E  敌人 (血量55)");
    sc(h, Y);
    got(h, 20, 18);
    wca(h, " $  弹药箱");
    sc(h, CYN);
    got(h, 20, 19);
    wca(h, " +  医疗包");
    sc(h, G);
    got(h, 20, 20);
    wca(h, " G  撤离点");
    sc(h, Y);
    got(h, 20, 21);
    wca(h, " #  墙壁");
    sc(h, GY);
    got(h, 20, 22);
    wca(h, " ?  阴影(未探索)");

    sc(h, CYN);
    got(h, 15, 24);
    wca(h, "请到 luogu.com.cn/team/117828 的团队文件中");
    sc(h, CYN);
    got(h, 15, 25);
    wca(h, "或 https://github.com/yingMC1/Infinite-Shooter 下载最新版");

    sc(h, Y);
    got(h, 30, 27);
    wca(h, "按任意键返回...");
    _getch();
}

// ========== 主游戏循环 ==========
void gameLoop(long long ca, long long cm, long long &remainAm,
              long long &remainCm) {
    gameRunning = true;
    am = ca;
    hp = 100;
    px = MS / 2;
    py = MS / 2;
    eva = hea = vic = false;
    kc = 0;
    hostDisconnected = false;

    // 地图生成
    if (!net) {
        gen();
        initE();
        emT = steady_clock::now();
        msgBuf[0] = 0;
        last_px = -1;
        last_py = -1;
    } else if (isHost) {
        gen();
        initE();
        emT = steady_clock::now();
        msgBuf[0] = 0;
        last_px = -1;
        last_py = -1;
        sendFullSync();
    } else {
        // 客户端等待同步
        for (long long i = 0; i < MS; i++)
            for (long long j = 0; j < MS; j++) mp[i][j] = 0;
        es.clear();
        te = 0;
        mapSynced = false;
        msgBuf[0] = 0;
        last_px = -1;
        last_py = -1;
        auto start = steady_clock::now();
        while (!mapSynced &&
               duration_cast<seconds>(steady_clock::now() - start).count() <
                   5) {
            Sleep(50);
        }
        if (!mapSynced) show("地图同步超时，使用默认地图", r, 2000);
    }

    if (net) {
        lr = steady_clock::now();
        st = false;
        netReady = false;
        rt = thread(recvData);
        stt = thread(syncThread);
        for (long long i = 0; i < 40 && !netReady; i++) Sleep(50);
        if (!netReady) show("队友未连接", Y, 2000);
    }

    CONSOLE_CURSOR_INFO ci;
    ci.bVisible = false;
    ci.dwSize = 1;
    SetConsoleCursorInfo(bf[0], &ci);
    SetConsoleCursorInfo(bf[1], &ci);
    SetConsoleActiveScreenBuffer(bf[0]);
    cb = 1;

    while (true) {
        if (hostDisconnected) {
            vic = true;
            show("主机已断开", r, 2000);
            Sleep(2000);
            break;
        }
        auto now = steady_clock::now();
        if (hea && duration_cast<seconds>(now - heT).count() >= HS) {
            hea = false;
            hp = min(100LL, hp + 20);
            show("治疗+20", G);
            if (net) sendSync();
        }
        if (eva && !hea && duration_cast<seconds>(now - evT).count() >= ES) {
            show("撤离成功！", G, 1500);
            if (net) {
                string msg = "EVAC 2";
                sendto(sk, msg.c_str(), msg.length(), 0, (sockaddr *)&pr,
                       sizeof(pr));
            }
            Sleep(1500);
            break;
        }
        if (!vic && kc == te) vic = true;
        if (!net || isHost) enemy();
        if (hp <= 0) {
            show("阵亡", r, 1500);
            Sleep(1500);
            am = 0;
            cm = 0;
            break;
        }
        draw(bf[1 - cb], cm);
        SetConsoleActiveScreenBuffer(bf[1 - cb]);
        cb = 1 - cb;

        if (_kbhit()) {
            long long k = _getch();
            if (k == 'p' || k == 'P') {
                ps = !ps;
                continue;
            }
            if (ps) continue;
            if ((eva || hea) && (k == 'f' || k == 'F')) {
                eva = hea = false;
                show("取消", Y);
                continue;
            }
            if (k == 'k' || k == 'K') {
                toggleArmor();
                continue;
            }
            if (k == 224) {
                k = _getch();
                long long dx = 0, dy = 0;
                if (k == 72) dy = -1;
                else if (k == 80) dy = 1;
                else if (k == 75) dx = -1;
                else if (k == 77) dx = 1;
                if (dx || dy) {
                    shoot(dx, dy);
                    if (net) sendSync();
                }
                continue;
            }
            long long nx = px, ny = py;
            char low = tolower(k);
            switch (low) {
            case 'w':
                ny--;
                break;
            case 's':
                ny++;
                break;
            case 'a':
                nx--;
                break;
            case 'd':
                nx++;
                break;
            case 'h':
                if (cm > 0 && hp < 100 && !hea && !eva) {
                    hea = true;
                    heT = now;
                    cm--;
                    show("治疗中", CYN);
                    if (net) sendSync();
                } else show("无法使用", r);
                break;
            case 'e':
                if (hea || eva) {
                    show("先完成动作", Y);
                    break;
                }
                if (mp[py][px] == 2) {
                    mp[py][px] = 0;
                    long long g = fastRand(9) + 4;
                    am += g;
                    char tmp[16];
                    sprintf(tmp, "+%lld弹", g);
                    show(tmp, Y);
                    if (net) {
                        char msg[64];
                        sprintf(msg, "PICKUP %d %d 2", (int)px, (int)py);
                        sendto(sk, msg, strlen(msg), 0, (sockaddr *)&pr,
                               sizeof(pr));
                    }
                } else if (mp[py][px] == 5) {
                    mp[py][px] = 0;
                    cm++;
                    show("+1医疗包", CYN);
                    if (net) {
                        char msg[64];
                        sprintf(msg, "PICKUP %d %d 5", (int)px, (int)py);
                        sendto(sk, msg, strlen(msg), 0, (sockaddr *)&pr,
                               sizeof(pr));
                    }
                } else show("无可拾取", GY);
                break;
            case 'q':
                if (hea) {
                    show("先治疗", r);
                    break;
                }
                if (mp[py][px] == 4 && !eva) {
                    eva = true;
                    evT = now;
                    show("撤离5秒", G);
                    if (net) {
                        string msg = "EVAC 1";
                        sendto(sk, msg.c_str(), msg.length(), 0,
                               (sockaddr *)&pr, sizeof(pr));
                    }
                } else if (mp[py][px] != 4) show("不在撤离点", r);
                break;
            }
            if ((nx != px || ny != py) && (eva || hea)) {
                eva = hea = false;
                show("取消", Y);
            }
            if (nx >= 0 && nx < MS && ny >= 0 && ny < MS && mp[ny][nx] != 1 &&
                mp[ny][nx] != 3) {
                px = nx;
                py = ny;
                if (net) sendSync();
            }
        }
        Sleep(30);
    }

    gameRunning = false;
    remainAm = am;
    remainCm = cm;
    if (net) {
        st = true;
        if (rt.joinable()) rt.join();
        if (stt.joinable()) stt.join();
        closesocket(sk);
        WSACleanup();
    }
    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    system("cls");
}

// ========== 辅助 ==========
long long inputNum(long long x, long long y, const char *prompt, long long d) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    sc(h, W);
    got(h, x, y);
    WriteConsoleA(h, prompt, strlen(prompt), NULL, NULL);
    char buf[16] = {0};
    sprintf(buf, "%lld", d);
    WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
    long long pos = (long long)strlen(buf), len = (long long)strlen(prompt);
    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 13) break;
            if (ch == 8 && pos > 0) {
                buf[--pos] = 0;
                got(h, x + len, y);
                WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
                wca(h, " ");
                got(h, x + len + pos, y);
            } else if (ch >= '0' && ch <= '9' && pos < 15) {
                buf[pos++] = ch;
                buf[pos] = 0;
                got(h, x + len, y);
                WriteConsoleA(h, buf, strlen(buf), NULL, NULL);
                got(h, x + len + pos, y);
            }
        }
        Sleep(16);
    }
    return atoll(buf);
}

string getLocalIP() {
    char name[256];
    gethostname(name, sizeof(name));
    hostent *host = gethostbyname(name);
    if (!host) return "";
    for (long long i = 0; host->h_addr_list[i]; ++i) {
        in_addr addr;
        memcpy(&addr, host->h_addr_list[i], sizeof(addr));
        string ip = inet_ntoa(addr);
        if (ip != "127.0.0.1" && ip.substr(0, 3) != "169") return ip;
    }
    return "127.0.0.1";
}

// ========== 初始化 ==========
void init() {
    system("mode con cols=80 lines=30");
    system("title 无限枪洲");

    for (long long i = 0; i < 2; i++) {
        bf[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                                          NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleScreenBufferSize(bf[i], sz);
        SMALL_RECT r = {0, 0, (SHORT)(sz.X - 1), (SHORT)(sz.Y - 1)};
        SetConsoleWindowInfo(bf[i], TRUE, &r);
    }
}

// ========== 主程序 ==========
int main() {
    system("chcp 65001 >nul");
    SetConsoleOutputCP(CP_UTF8);
    init();
    srand((unsigned)time(nullptr));
    loadData();

    while (true) {
        SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
        system("cls");
        HANDLE hMain = GetStdHandle(STD_OUTPUT_HANDLE);

        char buf[64];
        sc(hMain, Y);
        got(hMain, 28, 2);
        wca(hMain, "+============================+");
        sc(hMain, CYN);
        got(hMain, 31, 3);
        wca(hMain, " 无 限 枪 洲 ");
        sc(hMain, Y);
        got(hMain, 28, 4);
        wca(hMain, "+============================+");

        sc(hMain, PURPLE);
        got(hMain, 25, 6);
        wca(hMain, "作者: yingMC");
        sc(hMain, GY);
        got(hMain, 40, 6);
        wca(hMain, "luogu.com.cn/user/1488732");

        sc(hMain, G);
        got(hMain, 33, 9);
        wca(hMain, "1. 单人游戏");
        sc(hMain, BL);
        got(hMain, 33, 11);
        wca(hMain, "2. 创建主机");
        sc(hMain, PURPLE);
        got(hMain, 33, 13);
        wca(hMain, "3. 加入游戏");
        sc(hMain, Y);
        got(hMain, 33, 15);
        wca(hMain, "4. 商店");
        sc(hMain, CYN);
        got(hMain, 33, 17);
        wca(hMain, "5. 操作指南");
        sc(hMain, r);
        got(hMain, 33, 19);
        wca(hMain, "6. 退出游戏");

        sc(hMain, W);
        got(hMain, 25, 23);
        wca(hMain, "==================================");
        sprintf(buf, "仓库:%lld弹 %lld医疗包 金钱:%lld金", sa, sm, money);
        sc(hMain, Y);
        got(hMain, 25, 24);
        WriteConsoleA(hMain, buf, strlen(buf), NULL, NULL);
        if (armorLevel > 0) {
            sprintf(buf, "护甲:%d级 耐久:%lld/%lld", (int)armorLevel, armorDura,
                    armorMaxDura);
            sc(hMain, BL);
            got(hMain, 25, 25);
            WriteConsoleA(hMain, buf, strlen(buf), NULL, NULL);
        } else {
            sc(hMain, GY);
            got(hMain, 25, 25);
            wca(hMain, "无护甲");
        }

        long long ch = 0;
        while (!ch) {
            if (_kbhit()) {
                char c = _getch();
                if (c == '1') ch = 1;
                else if (c == '2') ch = 2;
                else if (c == '3') ch = 3;
                else if (c == '4') ch = 4;
                else if (c == '5') ch = 5;
                else if (c == '6') ch = 6;
            }
            Sleep(30);
        }
        if (ch == 6) break;
        if (ch == 5) {
            showHelp();
            continue;
        }
        if (ch == 4) {
            shop(hMain);
            continue;
        }

        // ===== 网络连接 =====
        if (ch == 2 || ch == 3) {
            WSADATA wsa;
            if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
                system("cls");
                got(hMain, 35, 15);
                sc(hMain, r);
                wca(hMain, "WSA初始化失败");
                _getch();
                continue;
            }
            sk = socket(AF_INET, SOCK_DGRAM, 0);
            if (sk == INVALID_SOCKET) {
                system("cls");
                got(hMain, 35, 15);
                sc(hMain, r);
                wca(hMain, "网络初始化失败");
                _getch();
                WSACleanup();
                continue;
            }

            long long port = NET_PORT;
            sockaddr_in local;
            local.sin_family = AF_INET;
            local.sin_port = htons((u_short)port);
            local.sin_addr.s_addr = INADDR_ANY;

            if (bind(sk, (sockaddr *)&local, sizeof(local)) == SOCKET_ERROR) {
                system("cls");
                got(hMain, 35, 15);
                sc(hMain, r);
                wca(hMain, "端口绑定失败");
                _getch();
                closesocket(sk);
                WSACleanup();
                continue;
            }

            u_long mode = 1;
            ioctlsocket(sk, FIONBIO, &mode);

            if (ch == 2) {
                isHost = true;
                lip = getLocalIP();
                system("cls");
                sc(hMain, Y);
                got(hMain, 32, 10);
                wca(hMain, "+==================+");
                sc(hMain, CYN);
                got(hMain, 32, 11);
                wca(hMain, "|  主机模式  |");
                sc(hMain, Y);
                got(hMain, 32, 12);
                wca(hMain, "+==================+");
                sc(hMain, W);
                got(hMain, 30, 14);
                wca(hMain, "IP: ");
                sc(hMain, G);
                got(hMain, 34, 14);
                WriteConsoleA(hMain, lip.c_str(), lip.length(), NULL, NULL);
                sc(hMain, Y);
                got(hMain, 30, 16);
                wca(hMain, "等待连接... (30秒)");

                char buf2[64];
                sockaddr_in from;
                int fl = sizeof(from);
                bool conn = false;
                auto start = steady_clock::now();

                while (duration_cast<seconds>(steady_clock::now() - start)
                           .count() < 30) {
                    long long ret =
                        recvfrom(sk, buf2, 64, 0, (sockaddr *)&from, &fl);
                    if (ret > 0 && strcmp(buf2, "CONNECT") == 0) {
                        pr = from;
                        sendto(sk, "ACK", 3, 0, (sockaddr *)&pr, sizeof(pr));
                        sc(hMain, G);
                        got(hMain, 30, 18);
                        wca(hMain, "连接成功!");
                        conn = true;
                        break;
                    }
                    Sleep(50);
                }

                if (!conn) {
                    sc(hMain, r);
                    got(hMain, 30, 20);
                    wca(hMain, "连接超时");
                    _getch();
                    closesocket(sk);
                    WSACleanup();
                    continue;
                }
                net = true;
                netReady = true;
                prL = true;
                prH = 100;
                prX = prY = -1;
                show("主机已启动（合作模式）", G, 1500);

            } else {
                isHost = false;
                system("cls");
                sc(hMain, Y);
                got(hMain, 32, 10);
                wca(hMain, "+==================+");
                sc(hMain, CYN);
                got(hMain, 32, 11);
                wca(hMain, "|  加入游戏  |");
                sc(hMain, Y);
                got(hMain, 32, 12);
                wca(hMain, "+==================+");
                sc(hMain, W);
                got(hMain, 30, 14);
                wca(hMain, "输入主机IP:");
                char ip[32] = {0};
                long long pos = 0;
                while (_kbhit()) _getch();

                while (true) {
                    if (_kbhit()) {
                        char c = _getch();
                        if (c == 13) break;
                        if (c == 8 && pos > 0) {
                            ip[--pos] = 0;
                        } else if ((c >= '0' && c <= '9') || c == '.') {
                            if (pos < 31) {
                                ip[pos++] = c;
                                ip[pos] = 0;
                            }
                        }
                        sc(hMain, G);
                        got(hMain, 30, 15);
                        WriteConsoleA(hMain, ip, strlen(ip), NULL, NULL);
                        wca(hMain, "   ");
                    }
                    Sleep(30);
                }

                pr.sin_family = AF_INET;
                pr.sin_port = htons((u_short)port);
                inet_pton(AF_INET, ip, &pr.sin_addr);

                sc(hMain, Y);
                got(hMain, 30, 17);
                wca(hMain, "正在连接...");
                bool conn = false;

                for (long long retry = 0; retry < 15 && !conn; retry++) {
                    sendto(sk, "CONNECT", 7, 0, (sockaddr *)&pr, sizeof(pr));

                    char buf2[64];
                    sockaddr_in from;
                    int fl = sizeof(from);

                    auto start = steady_clock::now();
                    while (
                        duration_cast<milliseconds>(steady_clock::now() - start)
                            .count() < 500) {
                        long long ret =
                            recvfrom(sk, buf2, 64, 0, (sockaddr *)&from, &fl);
                        if (ret > 0 && strcmp(buf2, "ACK") == 0) {
                            conn = true;
                            pr = from;
                            sc(hMain, G);
                            got(hMain, 30, 19);
                            wca(hMain, "连接成功!");
                            break;
                        }
                        Sleep(30);
                    }
                    if (!conn) Sleep(100);
                }

                if (!conn) {
                    sc(hMain, r);
                    got(hMain, 30, 21);
                    wca(hMain, "连接失败");
                    _getch();
                    closesocket(sk);
                    WSACleanup();
                    continue;
                }
                net = true;
                netReady = true;
                prL = true;
                prH = 100;
                show("已加入游戏（合作模式）", G, 1500);
            }

            mode = 0;
            ioctlsocket(sk, FIONBIO, &mode);
            Sleep(500);

        } else {
            net = false;
            netReady = false;
            isHost = false;
        }

        // ===== 物资配置 =====
        system("cls");
        HANDLE hSetup = GetStdHandle(STD_OUTPUT_HANDLE);
        sc(hSetup, Y);
        got(hSetup, 30, 5);
        wca(hSetup, "+====================+");
        sc(hSetup, CYN);
        got(hSetup, 30, 6);
        wca(hSetup, "|  装 备 配 置  |");
        sc(hSetup, Y);
        got(hSetup, 30, 7);
        wca(hSetup, "+====================+");

        sc(hSetup, W);
        got(hSetup, 25, 9);
        wca(hSetup, "仓库:");
        sc(hSetup, Y);
        sprintf(buf, "%lld弹", sa);
        WriteConsoleA(hSetup, buf, strlen(buf), NULL, NULL);
        sc(hSetup, W);
        wca(hSetup, "  ");
        sc(hSetup, CYN);
        sprintf(buf, "%lld医疗包", sm);
        WriteConsoleA(hSetup, buf, strlen(buf), NULL, NULL);
        sc(hSetup, W);
        wca(hSetup, "  金钱:");
        sc(hSetup, Y);
        sprintf(buf, "%lld金", money);
        WriteConsoleA(hSetup, buf, strlen(buf), NULL, NULL);

        sc(hSetup, W);
        got(hSetup, 25, 12);
        wca(hSetup, "携带弹药:");
        long long ca = inputNum(25 + 10, 12, "", 0);
        if (ca < 0) ca = 0;
        if (ca > sa) ca = sa;
        sc(hSetup, W);
        got(hSetup, 25, 14);
        wca(hSetup, "携带医疗包:");
        long long cm = inputNum(25 + 12, 14, "", 0);
        if (cm < 0) cm = 0;
        if (cm > sm) cm = sm;

        sa -= ca;
        sm -= cm;
        long long remainAm = 0, remainCm = 0;
        gameLoop(ca, cm, remainAm, remainCm);
        sa += remainAm;
        sm += remainCm;
        saveData();
    }
    return 0;
}