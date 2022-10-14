#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Config.hpp>

#include <Board.hpp>
#include <AI.hpp>
#include <Move.hpp>

#include <cmath>

enum STATES{
    SELECT_MODE,
    SELECT_COLOR,
    SELECT_DIFFICULTY,
    SELECT_MOVE,
    ANIMATION_START,
    ANIMATION_END
};

STATES GAME_STATE = STATES::SELECT_MODE;

bool ai_mode = true;
bool black_color = true;
bool player_turn = false;
//bool moves_displayed = false;

int difficulty = 5;
int max_difficulty = 5;

void select_mode();
void select_color();
void select_difficulty();
void select_move();
void animation_start(std::vector<mMove> moves);


const int width = 800;
const int height = 800;

Board board{};
AI bot{1};

std::vector<std::vector<mMove> > moves;

sf::Texture texture;
sf::Sprite buff;

std::vector<mMove> moves_to_animate;
int animation_frames = 1000;
int current_frame;
sf::Vector2f velocity;
sf::Vector2f c_pos;

void render_board(sf::RenderWindow &window, Board& board);

void render_moves(sf::RenderWindow &window);

void render_move(mMove m, sf::RenderWindow& window, char piece);

int is_in_moves(int x, int y);

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Checkers");
    // run the program as long as the window is open

    sf::Font font;

    if(!font.loadFromFile("./fonts/arial.ttf")) {
        std::cout << "cannot load a font";
    }

    sf::Text text;

    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyReleased) {
                
                if(GAME_STATE == SELECT_MODE) {

                    if(event.key.code == sf::Keyboard::Up ||
                       event.key.code == sf::Keyboard::Down)
                       ai_mode ^= 1;

                    if(event.key.code == sf::Keyboard::Enter) {
                        GAME_STATE = SELECT_COLOR;
                        event.key.code = sf::Keyboard::Space;
                    }

                }

                if(GAME_STATE == SELECT_COLOR) {
                    
                    if(event.key.code == sf::Keyboard::Up ||
                       event.key.code == sf::Keyboard::Down)
                       black_color ^= 1;

                    if(event.key.code == sf::Keyboard::Enter) {
                        GAME_STATE = SELECT_DIFFICULTY;
                        event.key.code = sf::Keyboard::Space;
                    }

                    if(event.key.code == sf::Keyboard::Escape) {
                        GAME_STATE = SELECT_MODE;
                        event.key.code = sf::Keyboard::Space;
                    }

                    bot.player = black_color;
                }

                if(GAME_STATE == SELECT_DIFFICULTY) {
                    
                    if(event.key.code == sf::Keyboard::Up)
                       difficulty = (difficulty + 1) % (max_difficulty + 1);
                    else if(event.key.code == sf::Keyboard::Down)
                        difficulty = abs(difficulty - 1) % (max_difficulty + 1);

                    if(event.key.code == sf::Keyboard::Enter) {
                        GAME_STATE = SELECT_MOVE;
                        bot.difficulty = difficulty;
                        event.key.code = sf::Keyboard::Space;
                    }

                    if(event.key.code == sf::Keyboard::Escape) {
                        GAME_STATE = SELECT_COLOR;
                        event.key.code = sf::Keyboard::Space;
                    }

                }

                if(GAME_STATE == SELECT_MOVE) {

                    if(event.key.code == sf::Keyboard::Escape) {
                        GAME_STATE = SELECT_DIFFICULTY;
                        board = Board{};
                        moves.clear();
                        event.key.code = sf::Keyboard::Space;
                    }

                }

            }

            if(GAME_STATE == SELECT_MOVE && player_turn == bot.player && ai_mode) {
                moves_to_animate = bot.make_move(board);
                std::reverse(moves_to_animate.begin(), moves_to_animate.end());
                Board temp{board};
                temp.get_state()[moves_to_animate.back().py][moves_to_animate.back().px] = 'e';

                render_board(window, temp);
                texture.create(width, height);
                moves.clear();
                texture.update(window);
                buff = sf::Sprite{texture};
                player_turn ^= 1;
                
                GAME_STATE = ANIMATION_START;
            }

            if(event.type == sf::Event::MouseButtonReleased && GAME_STATE == SELECT_MOVE) {

                if(event.mouseButton.button == sf::Mouse::Left) {

                    sf::Vector2i coords = sf::Mouse::getPosition(window);

                    int x = coords.x / (width / 8);
                    int y = coords.y / (height / 8);

                    char piece = board.get_state()[y][x];

                    if(piece != 'e') {
                        if(!player_turn && (piece == 'b' || piece == 'q')) {
                            
                            moves.clear();
                            board.moves_of_piece(moves, x, y);

                        }
                        else if(player_turn && (piece == 'w' || piece == 'v')) {

                            moves.clear();
                            board.moves_of_piece(moves, x, y);

                        }
                    }
                    else {
                        int move = is_in_moves(x, y);

                        if(move >= 0) {
                            moves_to_animate = moves[move];
                            std::reverse(moves_to_animate.begin(), moves_to_animate.end());
                            Board temp{board};
                            temp.get_state()[moves[move].front().py][moves[move].front().px] = 'e';

                            render_board(window, temp);
                            texture.create(width, height);
                            moves.clear();
                            texture.update(window);
                            buff = sf::Sprite{texture};
                            player_turn ^= 1;
                            
                            GAME_STATE = ANIMATION_START;
                        }
                    }

                }

            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);
        switch(GAME_STATE) {
            case SELECT_MODE:
            {
                std::string tittle = "SELECT MODE:";
                std::string vs_player = "versus player";
                std::string vs_AI = "versus computer";
                
                text.setFillColor(sf::Color::Green);
                text.setString(tittle);
                text.setPosition(width/2 - text.getLocalBounds().width/2, height/2 - 3*text.getLocalBounds().height);
                window.draw(text);

                if(ai_mode)
                    text.setFillColor(sf::Color::Black);
                else
                    text.setFillColor(sf::Color::Red);

                text.setString(vs_player);
                text.setPosition(width/2 - text.getLocalBounds().width/2, height/2);
                window.draw(text);

                if(!ai_mode)
                    text.setFillColor(sf::Color::Black);
                else
                    text.setFillColor(sf::Color::Red);

                text.setString(vs_AI);
                text.setPosition(width/2 - text.getLocalBounds().width/2, height/2 + 1.5*text.getLocalBounds().height);
                window.draw(text);
            }
            break;

            case SELECT_COLOR:
            {
                std::string tittle = "SELECT COLOR:";
                std::string blk = "black";
                std::string rd = "red";
                
                text.setFillColor(sf::Color::Green);
                text.setString(tittle);
                text.setPosition(width/2 - text.getLocalBounds().width/2, height/2 - 3*text.getLocalBounds().height);
                window.draw(text);

                if(!black_color)
                    text.setFillColor(sf::Color::Black);
                else
                    text.setFillColor(sf::Color::Red);

                text.setString(blk);
                text.setPosition(width/2 - text.getLocalBounds().width/2, height/2);
                window.draw(text);

                if(black_color)
                    text.setFillColor(sf::Color::Black);
                else
                    text.setFillColor(sf::Color::Red);

                text.setString(rd);
                text.setPosition(width/2 - text.getLocalBounds().width/2, height/2 + 1.5*text.getLocalBounds().height);
                window.draw(text);
            }
            break;

            case SELECT_DIFFICULTY:
            {

                std::string tittle = "SELECT DIFFICULTY:";
                std::string diff; diff += (char)(difficulty + '0');
                
                text.setFillColor(sf::Color::Green);
                text.setString(tittle);
                text.setPosition(width/2 - text.getLocalBounds().width/2, height/2 - 3*text.getLocalBounds().height);
                window.draw(text);

                text.setFillColor(sf::Color::Red);

                text.setString(diff);
                text.setPosition(width/2 - text.getLocalBounds().width/2, height/2 - 1.5*text.getLocalBounds().height);
                window.draw(text);

            }
            break;

            case SELECT_MOVE:
            {
                render_board(window, board);
            }
            break;

            case ANIMATION_START:
            {
                render_moves(window);
            }
            break;

            case ANIMATION_END:
            break;
        }
        window.display();
    }

    return 0;
}

int is_in_moves(int x, int y) {
    if(moves.empty() || board.get_state()[y][x] != 'e')
        return -1;

    
    for(int i = 0; i < moves.size(); i++) {

        for(std::vector<mMove>::iterator it = moves[i].begin(); it != moves[i].end(); it++) {

            char piece = board.get_state()[y][x];

            if(
                (x == it->x && y == it->y) ||
                (x == it->px && y == it->py)
            )   
                return i;
            else if(x == 2*it->x - it->px && y == 2*it->y - it->py && board.get_state()[it->y][it->x] != 'e')
                return i;
            }

    }

    return -1;

}

void render_board(sf::RenderWindow &window, Board& board) {
    int offset = 10;

    sf::RectangleShape rect{sf::Vector2f(width/8, height/8)};
    sf::CircleShape circle{width/16 - (float)offset}, inner_circle{width/32 - (float)offset};

    inner_circle.setFillColor(sf::Color::Green);
    circle.setOutlineThickness(5);

    sf::Vector2i ms;

    for(int y = 0; y < 8; y++) {

        for(int x = 0; x < 8; x++) {
            circle.setOutlineColor(sf::Color::Transparent);

            ms = sf::Mouse::getPosition(window);

            if((x + y) % 2 == 0)
                rect.setFillColor(sf::Color::Blue);
            else if(is_in_moves(x, y) >= 0)
                rect.setFillColor(sf::Color::Green);
            else
                rect.setFillColor(sf::Color::Cyan);

            rect.setPosition(sf::Vector2f(x * width/8, y * height / 8));

            window.draw(rect);
            
            circle.setPosition(sf::Vector2f(x * width/8 + offset, y * height/8 + offset));
            inner_circle.setPosition(sf::Vector2f(x * width/8 + width/32 + offset, y * height/8 + height/32 + offset));

            if(board.get_state()[y][x] == 'b' || board.get_state()[y][x] == 'q') {
                circle.setFillColor(sf::Color::Black);

                if( (x*width/8 + width/16 - ms.x)*(x*width/8 + width/16 - ms.x)
                  + (y*height/8 + height/16 - ms.y)*(y*height/8 + height/16 - ms.y)
                 <= (width/16 - (float)offset)*(width/16 - (float)offset))
                    circle.setOutlineColor(sf::Color::Yellow);
            }

            if(board.get_state()[y][x] == 'w' || board.get_state()[y][x] == 'v') {
                circle.setFillColor(sf::Color::Red);

                if( (x*width/8 + width/16 - ms.x)*(x*width/8 + width/16 - ms.x)
                  + (y*height/8 + height/16 - ms.y)*(y*height/8 + height/16 - ms.y)
                 <= (width/16 - (float)offset)*(width/16 - (float)offset))
                    circle.setOutlineColor(sf::Color::Yellow);
            }


            if(board.get_state()[y][x] == 'e')
                circle.setFillColor(sf::Color::Transparent);

            window.draw(circle);

            if(board.get_state()[y][x] == 'v' || board.get_state()[y][x] == 'q')
                window.draw(inner_circle);
        }

    }
}

void render_move(mMove m, sf::RenderWindow& window, char piece) {
    window.draw(buff);
    float offset = 10;
    sf::CircleShape c{width/16 - offset}, i_c{width/32 - offset};

    if(piece == 'b' || piece == 'q'){
        c.setFillColor(sf::Color::Black);
    }
    if(piece == 'w' || piece == 'v'){
        c.setFillColor(sf::Color::Red);
    }
    i_c.setFillColor(sf::Color::Green);

    sf::Vector2f a{(float)(m.x - m.px), (float)(m.y - m.py)};
    float d = sqrt(2) * (width/8);
    if(board.get_state()[m.y][m.x] != 'e')
        d*=2;
    float a_mag = 4*d / ((animation_frames)*(animation_frames)); 

    a /= sqrt(a.x*a.x + a.y*a.y);
    a *= a_mag;

    if(current_frame <= animation_frames / 2)
        velocity += a;
    else
        velocity -=a;
    c_pos += velocity;

    c.setPosition(c_pos);
    window.draw(c);
    i_c.setPosition(sf::Vector2f{width/32, height/32} + c_pos);
    if(piece == 'v' || piece == 'q')
        window.draw(i_c);

    current_frame++;
}

void render_moves(sf::RenderWindow &window) {
    float offset = 10;
    if(current_frame == 0) {
        velocity = sf::Vector2f{0,0};
        c_pos = sf::Vector2f{moves_to_animate.back().px*width/8 + offset, moves_to_animate.back().py*height/8 + offset};
    }

    board.get_state()[moves_to_animate.back().py][moves_to_animate.back().px];

    if(current_frame > animation_frames) {
        board.move(moves_to_animate.back());
        moves_to_animate.pop_back();
        current_frame = 0;
    }
    if(moves_to_animate.empty()) {
        GAME_STATE = SELECT_MOVE;
        return;
    }
    render_move(moves_to_animate.back(), window, board.get_state()[moves_to_animate.back().py][moves_to_animate.back().px]);
}