#include <AI.hpp>
#include <cmath>

double MMIN = -9999999999999999.;
double MMAX = 9999999999999999.;

double max(double a, double b) {
    return (a >= b)? a : b;
}

double min(double a, double b) {
    return (a >= b)? b : a;
}

double AI::minimax(Board board, std::vector<mMove> node, bool m_player, int depth, double alpha, double beta) {

    if (depth == difficulty) {
        double r = board.evaluate_score();
        return r;
    }

    for (std::vector<mMove>::iterator i = node.begin(); i != node.end(); i++)
        board.move(*i);

    std::vector<std::vector<mMove> > nodes = board.get_moves(m_player);
    if (m_player) {
        double b = MMIN;

        for (std::vector<std::vector<mMove> >::iterator i = nodes.begin(); i != nodes.end(); i++) {
            double v = minimax(board, *i, 0, depth + 1, alpha, beta);
            b = max(b, v);
            alpha = max(alpha, b);
            if (beta <= alpha)
                break;
        }

        for (int i = 0; i < node.size(); i++)
            board.pop_move();

        return b;
    }
    else {
        double b = MMAX;

        for (std::vector<std::vector<mMove> >::iterator i = nodes.begin(); i != nodes.end(); i++) {
            double v = minimax(board, *i, 1, depth + 1, alpha, beta);
            b = min(b, v);
            beta = min(beta, b);
            if (beta <= alpha)
                break;
        }

        for (int i = 0; i < node.size(); i++)
            board.pop_move();
        return b;
    }

}

void AI::make_move(Board& board) {
    std::vector<std::vector<mMove> > nodes = board.get_moves(player);
    if (player) {
        double best_value = MMIN; int counter = 0, right_move = 0;
        for (std::vector<std::vector<mMove> >::iterator i = nodes.begin(); i != nodes.end(); i++, counter++) {
            double value = minimax(board, *i, !player, 1, MMIN, MMAX);
            if (best_value < value) {
                best_value = value;
                right_move = counter;
            }
        }
        for (std::vector<mMove>::iterator i = nodes[right_move].begin(); i != nodes[right_move].end(); i++)
            board.move(*i);
    }
    else {
        double best_value = MMAX; int counter = 0, right_move = 0;
        for (std::vector<std::vector<mMove> >::iterator i = nodes.begin(); i != nodes.end(); i++, counter++) {
            double value = minimax(board, *i, !player, 1, MMIN, MMAX);
            if (best_value > value) {
                best_value = value;
                right_move = counter;
            }
        }
        for (std::vector<mMove>::iterator i = nodes[right_move].begin(); i != nodes[right_move].end(); i++)
            board.move(*i);
    }
}

AI::AI(int p) : player(p) {};