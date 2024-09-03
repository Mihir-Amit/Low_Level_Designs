#include <bits/stdc++.h>

using namespace std;

class Player{
    private:
    int position;
    string name;

    public:
    Player(int position, string name)
    {
        this->position = position;
        this->name = name;
    }

    int get_position()
    {
        return this->position;
    }

    string get_name()
    {
        return this->name;
    }

    void set_position(int n)
    {
        this->position = n;
    }
};

class Dice{
    private:
    int number;

    public:
    Dice(int n)
    {
        this->number = n;
    }

    int roll()
    {
        return rand()%(number*6) + 1;
    }
};

class Special{
    protected:
    int start;
    int end;
    void make(int start, int end)
    {
        this->start = start;
        this->end = end;
    }
};

class Snake:public Special{
    public:
    Snake(int pos1, int pos2){
        if(pos1 < pos2)
            cout<<"Invalid input"<<endl;
        else
            make(pos1, pos2);
    }

    int get_end()
    {
        return this->end;
    }
};

class Ladder:public Special{
    public:
    Ladder(int start, int end)
    {
        if(start > end)
            cout<<"Invalid Input"<<endl;
        else
            make(start, end);
    }
    int get_end()
    {
        return this->end;
    }
};

class Board{
    private:
    static Board* boardptr;
    queue<Player*> player_list;
    map<int, Snake*> snake_list;
    map<int, Ladder*> ladder_list;
    Dice* dice;
    int size;
    int num_players;

    Board(map<int, Snake*> snakes, map<int, Ladder*> ladders, int num_dices, int size, int num_players, vector<string> players)
    {
        this->dice = new Dice(num_dices);
        this->num_players = num_players;
        this->size = size;
        for(int i=0; i<num_players; i++)
        {
            Player* player = new Player(0, players[i]);
            this->player_list.push(player);
        }
        this->snake_list = snakes;
        this->ladder_list = ladders;
    }

    public:
    static Board* instantiate(map<int, Snake*> snakes, map<int, Ladder*> ladders, int num_dices, int size, int num_players, vector<string> players)
    {
        if(boardptr == nullptr)
        {
            boardptr = new Board(snakes, ladders, num_dices, size, num_players, players);
        }
        return boardptr;
    }

    void move()
    {
        Player* mover = this->player_list.front();
        cout<<mover->get_name()<<" moves"<<endl;
        this->player_list.pop();
        int moves = this->dice->roll();
        int old_position = mover->get_position();
        int new_position = old_position + moves;
        if(snake_list.find(new_position)!=snake_list.end())
            new_position = snake_list[new_position]->get_end();
        if(ladder_list.find(new_position)!=ladder_list.end())
            new_position = ladder_list[new_position]->get_end();
        mover->set_position(mover->get_position() + moves);
        if(mover->get_position() >= this->size)
            cout<<mover->get_name()<<" won"<<endl;
        else
            this->player_list.push(mover);
    }

    int players_left()
    {
        return player_list.size();
    }
};

Board* Board::boardptr = nullptr;

int main()
{
    vector<string> players = {"mihir", "alka", "mihir2"};
    int num_players = 3;
    int num_dice = 2;
    Snake* snake1 = new Snake(10, 5);
    Snake* snake2 = new Snake(15, 10);
    Snake* snake3 = new Snake(25, 15);
    Ladder* ladder1 = new Ladder(6, 16);
    Ladder* ladder2 = new Ladder(4, 14);
    Ladder* ladder3 = new Ladder(20, 29);
    map<int, Snake*> snakes;
    snakes[10] = snake1;
    snakes[15] = snake2;
    snakes[25] = snake3;
    map<int, Ladder*> ladders;
    ladders[6] = ladder1;
    ladders[4] = ladder2;
    ladders[20] = ladder3;
    Board* board = Board::instantiate(snakes, ladders, num_dice, 50, num_players, players);
    while(board->players_left() > 1)
    {
        board->move();
    }
}

