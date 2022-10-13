#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <Move.hpp>

extern const double MMIN;
extern const double MMAX;
extern const std::vector<std::string> START_POSITION;


template<typename T>
std::vector<T> merge_vectors(vector<T>& A, vector<T>& B);

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

    std::vector<std::vector<mMove> > get_moves(int player);

    bool player_move(bool player);

};

#endif