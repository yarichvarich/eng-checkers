#ifndef AI_HPP
#define AI_HPP

#include <Board.hpp>

struct AI {

    bool player = 0;

    int difficulty = 13;

    AI(int p);

    double minimax(Board board, std::vector<mMove> node, bool m_player, int depth, double alpha, double beta);

    void make_move(Board& board);
};

#endif