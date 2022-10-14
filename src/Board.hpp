#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <Move.hpp>
#include <iostream>

extern const std::vector<std::string> START_POSITION;


extern std::string w;//"\u2B24";
extern std::string b;//"\u25EF";
extern std::string v; //"\u25A0";
extern std::string q;//"\u25A1";


template<typename T>
std::vector<T> merge_vectors(std::vector<T>& A, std::vector<T>& B);

class Board {

    std::vector<std::vector<std::string> > states;

    int current = -1;

    std::vector<int> board_score;

public:

    Board();

    Board(const Board& b);

    std::vector<std::vector<std::string> > get_field() const;

    std::vector<int> get_scores() const;

    int get_current() const;

    bool is_tie();

    std::vector<std::string>& get_state();

    std::vector<std::vector<std::string>>& get_states();

    bool out(int x, int y);

    double evaluate_score();

    bool move(int px, int py, int x, int y);

    bool move(mMove dir);

    void pop_move();

    int get_score();

    void moves_of_piece(std::vector<std::vector<mMove> >& seq, int x, int y, std::vector<mMove> p = std::vector<mMove>{});

    void apply_sq(std::vector<mMove>& moves);

    std::vector<std::vector<mMove> > get_moves(int player);
};

#endif