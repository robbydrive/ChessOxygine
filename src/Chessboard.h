#include <array>
#include <deque>
#include "oxygine-framework.h"
#include "Cell.h"
using namespace oxygine;

enum Winner { Nobody, Whites, Blacks };

DECLARE_SMART(Cell, spCell)
DECLARE_SMART(Chessboard, spChessboard)

class Chessboard : public Actor
{
public:
    Chessboard();

    bool isWhitesTurn() const { return _isWhitesTurn; }
    const std::deque<spPiece>& getEaten() const { return _eaten; }
    bool isCheck() const { return _isCheck; }
    Winner getWinner() const { return _winner; }
private:
    std::array<std::array<spCell, 8>, 8> _cells = {};
    bool _isWhitesTurn = true;
    std::deque<spPiece> _eaten;
    spCell _source = nullptr;
    spCell _target = nullptr;
    spCell _whiteKing;
    spCell _blackKing;
    bool _isCheck = false;
    Winner _winner = Winner::Nobody;

    const MovesSet& getPossibleMoves(spCell cell);
    void clickEventHandler(Event* ev);
    void move(spCell source, spCell target);
    void checkMate();
    MovesSet cellsBetween(spCell cell1, spCell cell2);

    bool isValidPosition(const Vector2& position)
    {
        return position.x >= 0 && position.x < 8 && position.y >= 0 && position.y < 8;
    }
};
