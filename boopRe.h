#ifndef BOOP_H
#define BOOP_H
#include "space.h"
#include "game.h"
using namespace main_savitch_14;

class Boop:public game{
    public:
        Boop();
        
        static const int board_size = 6;

        void display_status()const;//prints the board
        void board_only()const; //prints the board without the player pieces or prompt for another move
       
        void pieceAround(const std::string move_made); //moves the pieces in accordance to the rules next to the move made
        
        void restart(); //clean slate board
        
       who last_mover() const{
            if(current_player == 1) return COMPUTER;
            else return HUMAN;
       }

       int my_last_move()const{
            if(current_player == 1) return 1;
            else return 2;
       }

        //board checks
        bool no_pieces();
        bool three_in_row()const;
        bool is_game_over() const;



        game* clone() const{return new Boop(*this);} // should be all
        void compute_moves(std::queue<std::string>& moves) const;
        int evaluate() const;
        std::string get_user_move()const;
        
        //game loop
        void make_move(const std::string& input);//gets the move
        bool is_legal(const std::string& move)const;

        //move types
        void insert_piece(const string& input); // type 1
        bool insert_is_legal(const std::string& move)const; //checks for legality of inputted move

        void remove3Pieces(const string& input);
        bool remove_three_is_legal(const string& piecesRemoved)const;

        void remove_one(const string& piecesRemoved);
        bool remove_one_is_legal(const string& input)const;
        
        
        
    private:
        int move_type;
        int player1_small; //number of small pieces for p1
        int player1_large; //number of large pieces for p1
        int player2_small; //number of small pieces for p2
        int player2_large; //number of large pieces for p2
        int current_player;
        Space board[board_size][board_size];
};


#endif