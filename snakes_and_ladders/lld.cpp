#include <vector>
#include <queue>
#include <map>
#include <string>
#include <bits/stdc++.h>
using namespace std;

// class Board;



class Player
{
private:
    string name;
    int position;

public:
    Player(int position, string name)
    {
        this->name = name;
        this->position = position;
    }
    int getposition()
    {
        return position;
    }
    void setposition(int new_position)
    {
        this->position = new_position;
    }
    string getname()
    {
        return this->name;
    }
};

class special
{
public:
    Board* board;
    int start;
    int end;
    special(int start, int end)
    {
        this->start = start;
        this->end = end;
        board = Board::giveboard();
    }
    virtual bool check(){};
    virtual void move(Player turnee) {};
};

class Snake : public special
{
public:
    bool check()
    {
        if (this->start < this->end)
            return false;
        else
        {
            return true;
            board->add_snake(this);
        }
    }
    void move(Player *turnee)
    {
        turnee->setposition(this->end);
    }
};

class Ladder : public special
{
public:
    bool check()
    {
        if (this->start > this->end)
            return false;
        else
        {
            return true;
            board->add_ladder(this);
        }
    }
    void move(Player *turnee)
    {
        turnee->setposition(this->end);
    }
};

class Diceroll{
    private:
    int roll;
    Board* board;
    public:
    Diceroll(){
        int roll = rand()%6 + 1;
        board = Board :: giveboard();
        board->turn(roll);
    }
};

class Board
{
private:
    static Board *chessboard;
    int size;
    int player_number;
    queue<Player*> players;
    map<int, Snake*> snakes;
    map<int, Ladder*> ladders;
    Board(int size, int player_number)
    {
        this->size = size;
        this->player_number = player_number;
        for (int i = 0; i < player_number; i++)
        {
            Player *temp = new Player(0, "player " + 'a'+i);
            this->players.push(temp);
        }
    }

public:
    bool game_continues = true;
    static Board *giveboard(int size, int players)
    {
        if (chessboard == nullptr)
            chessboard = new Board(size, players);
        return chessboard;
    }
    void add_snake(Snake *snake)
    {
        snakes[snake->start] = snake;
    }
    void add_ladder(Ladder *ladder)
    {
        ladders[ladder->start] = ladder;
    }
    Player* turn(int n)
    {
        Player* player_turn = players.front();
        players.pop();
        cout<<player_turn->getname()<<" is moving "<<n<<" spaces";
        player_turn->setposition() = player_turn->getposition() + n;
        if(snakes.find(player_turn->getposition())!=snakes.end())
            snakes[player_turn->getposition()]->move(player_turn);
        if(ladders.find(player_turn->getposition()) != ladders.end())
            ladders[player_turn->getposition()]->move(player_turn);
        if(player_turn->getposition() < this->size)
            players.push(player_turn);
        else
            cout<<player_turn->getname()<<" won "<<endl;
        if(players.empty())
            this->game_continues = false;
    }
};

Board* Board::chessboard = nullptr;

int main()
{
Board* board = Board::giveboard(20, 5);
    Snake* snake = new Snake(15, 5);
    snake->check();
    Ladder* ladder = new Ladder(5, 10);
    ladder->check();
    while(board->game_continues)
    {
        Diceroll* obj = new Diceroll();
    }
}