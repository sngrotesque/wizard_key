#include <wmkc_conf.hpp>
#include <wmkc_random.c>
#include <wmkc_basic.c>
#include <wmkc_time.c>

#define DEFAULT_N_MIN 0
#define DEFAULT_N_MAX 1000
#define DEFAULT_COUNT (((DEFAULT_N_MAX - DEFAULT_N_MIN) <= 1000)?\
    (10):((DEFAULT_N_MAX - DEFAULT_N_MIN) / 10 / 2))

wmkcBool check_chcp(wmkcVoid)
{
    if(GetConsoleOutputCP() != 65001) {
        if(SetConsoleOutputCP(65001)) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

class RandomNumberGame {
    private:
        wmkc_u32 n;
        wmkc_u32 count;
        wmkc_u32 player_n;
        wmkc_u32 game_min, game_max;
    public:
        wmkcBool win;
        RandomNumberGame(wmkc_u32 min = DEFAULT_N_MIN, wmkc_u32 max = DEFAULT_N_MAX)
        : count(DEFAULT_COUNT), player_n(), win(false), game_min(min), game_max(max)
        {
            wmkcRandom_seed();
            this->n = wmkcRandom_randint(this->game_min, this->game_max);
            check_chcp();
        }

        void start()
        {
            while(this->count && !win) {
                cout << "请输入(" << this->game_min << "-" << this->game_max << "): ";
                cin >> this->player_n;

                if(this->player_n > this->game_max || this->player_n < game_min) {
                    cout << "你输入的数字超过了范围！！！" << endl;
                    continue;
                }

                if(this->player_n == this->n) {
                    this->win = true;
                    if(this->count == DEFAULT_COUNT) {
                        cout << "byd运气这么好是吧？" << endl;
                        cout << "你赢了！" << endl;
                    } else {
                        cout << "你还剩" << this->count << "次机会，但你还是赢了。" << endl;
                    }
                } else {
                    if(this->player_n > this->n) {
                        cout << "太大了！";
                    } else {
                        cout << "太小了！";
                    }
                    cout << "你还剩" << --this->count << "次机会。" << endl;
                }
            }
        }

        void show()
        {
            cout << "虽然你没有赢下这次游戏，但最终谜底揭晓：" << this->n << endl;
        }

        void final()
        {
            system("pause");
        }
};

int main()
{
    RandomNumberGame *game = wmkcNull;
    game = new RandomNumberGame();
    game->start();
    if(!game->win) {
        game->show();
    }
    game->final();
    delete game;
    return 0;
}
