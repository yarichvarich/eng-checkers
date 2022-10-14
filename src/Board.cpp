#include <Board.hpp>


std::string w = "w";//"\u2B24";
std::string b = "b";//"\u25EF";
std::string v = "v";  //"\u25A0";
std::string q = "q";//"\u25A1";

const std::vector<std::string> START_POSITION = { "ebebebeb",
								  "bebebebe",
								  "ebebebeb",
								  "eeeeeeee",
								  "eeeeeeee",
								  "wewewewe",
								  "ewewewew",
								  "wewewewe" };

template<typename T>
std::vector<T> merge_vectors(std::vector<T>& A, std::vector<T>& B) {
    std::vector<T> AB;
	AB.reserve(A.size() + B.size());
	AB.insert(AB.end(), A.begin(), A.end());
	AB.insert(AB.end(), B.begin(), B.end());
	return AB;
}

Board::Board()  {
    this->states.push_back(START_POSITION);
    this->board_score.push_back(0);
    current = 0;
}

Board::Board(const Board& b) {
    this->board_score = b.get_scores();
    this->states = b.get_field();
    this->current = b.get_current();
}

std::vector<std::vector<std::string> > Board::get_field() const {
	return states;
}

std::vector<int> Board::get_scores() const {
    return board_score;
}

int Board::get_current() const {
    return current;
}

bool Board::is_tie() {
    std::vector<std::vector<mMove> > b = get_moves(0);
    std::vector<std::vector<mMove> > w = get_moves(1);
    if(b.empty() || w.empty())
        return true;
    return false;
}

std::vector<std::string>& Board::get_state() {
    return this->states[current];
}

std::vector<std::vector<std::string> >& Board::get_states() {
	return this->states;
}

bool Board::out(int x, int y) {
    if (x < 0 || x > 7 || y < 0 || y > 7)
        return true;
    return false;
}

double Board::evaluate_score() {
    //      ..           ..     ..     ..
    //checkers number  
    int closer_to_king = 0;
    int pieces_left = 0;
    int better_king_position = 0;
    int random = rand() % 100;
    int w = 0, b = 0;

    std::vector<std::string> cur = get_state();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            if (cur[i][j] == 'b') {
                closer_to_king += -i;
                b++;
            }
            else if (cur[i][j] == 'w') {
                closer_to_king += 7 - i;
                w++;
            }
            else if (cur[i][j] == 'q') {
                int bk = 0;
                if (move(j, i, j + 1, i + 1)) {
                    bk += board_score[current] - board_score[current - 1] - 5;
                    pop_move();
                }
                if (move(j, i, j + 1, i - 1)) {
                    bk += board_score[current] - board_score[current - 1] - 5;
                    pop_move();
                }
                if (move(j, i, j - 1, i + 1)) {
                    bk += board_score[current] - board_score[current - 1] - 5;
                    pop_move();
                }
                if (move(j, i, j - 1, i - 1)) {
                    bk += board_score[current] - board_score[current - 1] - 5;
                    pop_move();
                }
                better_king_position += bk;
            }
            else if (cur[i][j] == 'v') {
                int bk = 0;
                if (move(j, i, j + 1, i + 1)) {
                    bk += board_score[current] - board_score[current - 1] + 5;
                    pop_move();
                }
                if (move(j, i, j + 1, i - 1)) {
                    bk += board_score[current] - board_score[current - 1] + 5;
                    pop_move();
                }
                if (move(j, i, j - 1, i + 1)) {
                    bk += board_score[current] - board_score[current - 1] + 5;
                    pop_move();
                }
                if (move(j, i, j - 1, i - 1)) {
                    bk += board_score[current] - board_score[current - 1] + 5;
                    pop_move();
                }
                better_king_position += bk;
            }

        }
    }

    pieces_left = 10 * (w - b);
    better_king_position;

    w = 0, b = 0;

    double r = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            r += (get_state()[i][j] == 'w') ? 5. + 0*(++w) : 0;
            r += (get_state()[i][j] == 'b') ? -5. + 0*(++b) : 0;
            r += (get_state()[i][j] == 'v') ? 10.  + 0 * (++w) : 0;
            r += (get_state()[i][j] == 'q') ? -10. + 0*(++b): 0;
        }

    if (b == 0) {
        
    }
    if (w == 0) {
        
    }

    double result = (double)r * 100000000 + (double)closer_to_king * 1000000 +
        (double)pieces_left * 10000 + (double)better_king_position * 100 + (random + 1);

    return result;
}

bool Board::move(int px, int py, int x, int y) {
    std::pair<int, int> v{ x - px, y - py };

    if (v.first * v.first + v.second * v.second != 2)
        return false;

    if (out(px, py) || out(x, y))
        return false;

    std::vector<std::string> current_state = get_state();

    //for black
    //black pawn
    if (current_state[py][px] == 'b') {
        if (v.second != 1)
            return false;
        //empty
        if (current_state[y][x] == 'e') {
            if (y == 7) {
                current_state[y][x] = 'q';
                board_score.push_back(board_score[current] - 6);
            }
            else
                current_state[y][x] = 'b';

            current_state[py][px] = 'e';
            states.push_back(current_state);
            board_score.push_back(board_score[current]);
            current++;
            return true;
        }
        //alies
        if (current_state[y][x] == 'b' ||
            current_state[y][x] == 'q')
            return false;

        //ememies
        int x1 = x + v.first, y1 = y + v.second;

        if (out(x1, y1))
            return false;

        if (current_state[y1][x1] != 'e')
            return false;
        //become king?	
        if (y1 == 7) {
            current_state[y1][x1] = 'q';
            board_score.push_back(board_score[current] - 6);
        }
        else
            current_state[y1][x1] = 'b';
        //beaten	
        if (current_state[y][x] == 'w')
            board_score.push_back(board_score[current] - 5);
        else
            board_score.push_back(board_score[current] - 10);

        current_state[y][x] = 'e';
        current_state[py][px] = 'e';
        states.push_back(current_state);
        current++;
        return true;
    }
    else if (current_state[py][px] == 'q') {
        //empty
        if (current_state[y][x] == 'e') {
            current_state[y][x] = 'q';
            current_state[py][px] = 'e';
            states.push_back(current_state);
            board_score.push_back(board_score[current]);
            current++;
            return true;
        }
        //alies
        if (current_state[y][x] == 'b' ||
            current_state[y][x] == 'q')
            return false;
        //enemies
        int x1 = x + v.first, y1 = y + v.second;
        if (out(x1, y1))
            return false;
        if (current_state[y1][x1] != 'e')
            return false;
        if (current_state[y][x] == 'w')
            board_score.push_back(board_score[current] - 5);
        else
            board_score.push_back(board_score[current] - 10);

        current_state[y1][x1] = 'q';
        current_state[y][x] = 'e';
        current_state[py][px] = 'e';
        states.push_back(current_state);
        current++;
        return true;
    }

    //for whites

    //white pawn
    else if (current_state[py][px] == 'w') {
        if (v.second != -1)
            return false;
        //empty
        if (current_state[y][x] == 'e') {
            if (y == 0) {
                current_state[y][x] = 'v';
                board_score.push_back(board_score[current] + 6);
            }
            else
                current_state[y][x] = 'w';

            current_state[py][px] = 'e';
            states.push_back(current_state);
            board_score.push_back(board_score[current]);
            current++;
            return true;
        }
        //alies
        if (current_state[y][x] == 'w' ||
            current_state[y][x] == 'v')
            return false;

        //ememies
        int x1 = x + v.first, y1 = y + v.second;

        if (out(x1, y1))
            return false;

        if (current_state[y1][x1] != 'e')
            return false;
        //become king?	
        if (y1 == 0) {
            current_state[y1][x1] = 'v';
            board_score.push_back(board_score[current] + 6);
        }
        else
            current_state[y1][x1] = 'w';
        //beaten	
        if (current_state[y][x] == 'b')
            board_score.push_back(board_score[current] + 5);
        else
            board_score.push_back(board_score[current] + 10);

        current_state[y][x] = 'e';
        current_state[py][px] = 'e';
        states.push_back(current_state);
        current++;
        return true;
    }
    else if (current_state[py][px] == 'v') {
        //empty
        if (current_state[y][x] == 'e') {
            current_state[y][x] = 'v';
            current_state[py][px] = 'e';
            states.push_back(current_state);
            board_score.push_back(board_score[current]);
            current++;
            return true;
        }
        //alies
        if (current_state[y][x] == 'w' ||
            current_state[y][x] == 'v')
            return false;
        //enemies
        int x1 = x + v.first, y1 = y + v.second;
        if (out(x1, y1))
            return false;
        if (current_state[y1][x1] != 'e')
            return false;
        if (current_state[y][x] == 'w')
            board_score.push_back(board_score[current] + 5);
        else
            board_score.push_back(board_score[current] + 10);

        current_state[y1][x1] = 'v';
        current_state[y][x] = 'e';
        current_state[py][px] = 'e';
        states.push_back(current_state);
        current++;
        return true;
    }

    return false;
}

bool Board::move(mMove dir) {
	return move(dir.px, dir.py, dir.x, dir.y);
}

void Board::pop_move() {
    if (current <= 0)
        return;
    current--;
    states.pop_back();
    board_score.pop_back();
}

int Board::get_score() {
    return this->board_score[current];
}

void Board::moves_of_piece(std::vector<std::vector<mMove> >& seq, int x, int y, std::vector<mMove> p) {

    if (out(x, y))
        return;
    if (get_state()[y][x] == 'e')
        return;
    mMove ld{ x, y, x - 1, y + 1 }, rd{ x, y, x + 1, y + 1 },
        lu{ x, y, x - 1, y - 1 }, ur{ x, y, x + 1, y - 1 };

    if (get_state()[y][x] == 'b') {
        if (move(ld)) {
            if (p.empty()) {
                std::vector<mMove> t; t.push_back(ld);
                seq.push_back(t);

                if (get_state()[ld.y][ld.x] == 'e')
                    moves_of_piece(seq, ld.x - 1, ld.y + 1, t);
            }
            else if (get_state()[ld.y][ld.x] == 'e') {
                std::vector<mMove> t = p; t.push_back(ld);
                seq.push_back(t);

                moves_of_piece(seq, ld.x - 1, ld.y + 1, t);
            }
            pop_move();
        }
        if (move(rd)) {
            if (p.empty()) {
                std::vector<mMove> t; t.push_back(rd);
                seq.push_back(t);

                if (get_state()[rd.y][rd.x] == 'e')
                    moves_of_piece(seq, rd.x + 1, rd.y + 1, t);
            }
            else if (get_state()[rd.y][rd.x] == 'e') {
                std::vector<mMove> t = p; t.push_back(rd);
                seq.push_back(t);

                moves_of_piece(seq, rd.x + 1, rd.y + 1, t);
            }
            pop_move();
        }
    }
    else if (get_state()[y][x] == 'w') {
        if (move(lu)) {
            if (p.empty()) {
                std::vector<mMove> t; t.push_back(lu);
                seq.push_back(t);

                if (get_state()[lu.y][lu.x] == 'e')
                    moves_of_piece(seq, lu.x - 1, lu.y - 1, t);
            }
            else if (get_state()[lu.y][lu.x] == 'e') {
                std::vector<mMove> t = p; t.push_back(lu);
                seq.push_back(t);

                moves_of_piece(seq, lu.x - 1, lu.y - 1, t);
            }
            pop_move();
        }
        if (move(ur)) {
            if (p.empty()) {
                std::vector<mMove> t; t.push_back(ur);
                seq.push_back(t);

                if (get_state()[ur.y][ur.x] == 'e')
                    moves_of_piece(seq, ur.x + 1, ur.y - 1, t);
            }
            else if (get_state()[ur.y][ur.x] == 'e') {
                std::vector<mMove> t = p; t.push_back(ur);
                seq.push_back(t);

                moves_of_piece(seq, ur.x + 1, ur.y - 1, t);
            }
            pop_move();
        }
    }
    else if (get_state()[y][x] == 'q' || get_state()[y][x] == 'v') {
        if (move(ld)) {
            if (p.empty()) {
                std::vector<mMove> t; t.push_back(ld);
                seq.push_back(t);

                if (get_state()[ld.y][ld.x] == 'e')
                    moves_of_piece(seq, ld.x - 1, ld.y + 1, t);
            }
            else if (get_state()[ld.y][ld.x] == 'e') {
                std::vector<mMove> t = p; t.push_back(ld);
                seq.push_back(t);

                moves_of_piece(seq, ld.x - 1, ld.y + 1, t);
            }
            pop_move();
        }
        if (move(rd)) {
            if (p.empty()) {
                std::vector<mMove> t; t.push_back(rd);
                seq.push_back(t);

                if (get_state()[rd.y][rd.x] == 'e')
                    moves_of_piece(seq, rd.x + 1, rd.y + 1, t);
            }
            else if (get_state()[rd.y][rd.x] == 'e') {
                std::vector<mMove> t = p; t.push_back(rd);
                seq.push_back(t);

                moves_of_piece(seq, rd.x + 1, rd.y + 1, t);
            }
            pop_move();
        }
        if (move(lu)) {
            if (p.empty()) {
                std::vector<mMove> t; t.push_back(lu);
                seq.push_back(t);

                if (get_state()[lu.y][lu.x] == 'e')
                    moves_of_piece(seq, lu.x - 1, lu.y - 1, t);
            }
            else if (get_state()[lu.y][lu.x] == 'e') {
                std::vector<mMove> t = p; t.push_back(lu);
                seq.push_back(t);

                moves_of_piece(seq, lu.x - 1, lu.y - 1, t);
            }
            pop_move();
        }
        if (move(ur)) {
            if (p.empty()) {
                std::vector<mMove> t; t.push_back(ur);
                seq.push_back(t);

                if (get_state()[ur.y][ur.x] == 'e')
                    moves_of_piece(seq, ur.x + 1, ur.y - 1, t);
            }
            else if (get_state()[ur.y][ur.x] == 'e') {
                std::vector<mMove> t = p; t.push_back(ur);
                seq.push_back(t);

                moves_of_piece(seq, ur.x + 1, ur.y - 1, t);
            }
            pop_move();
        }
    }
}

std::vector<std::vector<mMove> > Board::get_moves(int player) {
    std::vector<std::vector<mMove> > r;
    char s1, s2;
    if (player)
        s1 = 'w', s2 = 'v';
    else
        s1 = 'b', s2 = 'q';
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (get_state()[i][j] == s1 || get_state()[i][j] == s2) {
                std::vector<std::vector<mMove>> t;
                moves_of_piece(t, j, i);
                r = merge_vectors(r, t);
            }
    return r;
}

void Board::apply_sq(std::vector<mMove>& moves) {
    for(std::vector<mMove>::iterator i = moves.begin(); i != moves.end(); i++)
        this->move(*i);
}