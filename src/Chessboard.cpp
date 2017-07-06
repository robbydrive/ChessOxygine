#include <array>
#include <deque>
#include "oxygine-framework.h"
#include "res.h"
#include "Cell.h"
#include "Chessboard.h"
using namespace oxygine;

DECLARE_SMART(Cell, spCell)

class Chessboard : public Actor
{
public:
    Chessboard() : Actor()
    {
        Vector2 pieceSize = { 64, 64 };
        // Offset for piece inside cell to achieve center position of the piece
        Vector2 pieceOffset = { 0, 0 };
        Vector2 position = getStage()->getSize() / 2;
        position.x -= pieceSize.x * 4;
        position.y += pieceSize.y * 3;
        bool isWhite = true;
        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                isWhite = !isWhite;
                spCell cell = new Cell(isWhite, Vector2(i, j), pieceSize);
                cell->setPosition(position);
                addChild(cell);

                spPiece piece = nullptr;
                if (i == 0 || i == 7)
                {
                    PieceType type;
                    if (j == 0 || j == 7)
                        type = PieceType::Rook;
                    else if (j == 1 || j == 6)
                        type = PieceType::Knight;
                    else if (j == 2 || j == 5)
                        type = PieceType::Bishop;
                    else if (j == 3)
                        type = PieceType::Queen;
                    else
                        type = PieceType::King;
                    piece = new Piece(type, i == 0);
                }
                else if (i == 1 || i == 6)
                    piece = new Piece(PieceType::Pawn, i == 1);

                if (piece.get() != nullptr)
                {
                    cell->setPiece(piece);
                    piece->attachTo(cell);
                    piece->setPosition(pieceOffset);
                }
                cell->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Chessboard::clickEventHandler));
                _cells[i][j] = cell;
                position.x += pieceSize.x;
            }
            position.x -= pieceSize.x * 8;
            position.y -= pieceSize.y;
            isWhite = !isWhite;
        }
    }

    const MovesSet& getPossibleMoves(spCell cell)
    {
        spPiece piece = cell->getPiece();
        if (piece->getPossibleMoves().size() > 0)
            return piece->getPossibleMoves();
        const Vector2& currentPosition = cell->getCBPosition();
        Vector2 targetPosition;
        spPiece targetCellPiece;
        MovesSet possibleMoves;
        if (piece->getType() == PieceType::Pawn)
        {
            //region Pawn
            if (piece->isWhite())
            {
                log::messageln("White");
                log::messageln("cPosition: %f %f", currentPosition.x, currentPosition.y);
                if (currentPosition.x != 7
                        && _cells[currentPosition.x + 1][currentPosition.y]->getPiece().get() == nullptr)
                {
                    possibleMoves.insert(currentPosition + Vector2(1, 0));
                    if (currentPosition.x == 1
                            && _cells[currentPosition.x + 2][currentPosition.y]->getPiece().get() == nullptr)
                        possibleMoves.insert(currentPosition + Vector2(2, 0));
                }
                if (currentPosition.x != 7 && currentPosition.y > 0
                        && piece->isOpponentsPiece(_cells[currentPosition.x + 1][currentPosition.y - 1]->getPiece()))
                    possibleMoves.insert(currentPosition + Vector2(1, -1));
                if (currentPosition.x != 7 && currentPosition.y < 7
                        && piece->isOpponentsPiece(_cells[currentPosition.x + 1][currentPosition.y + 1]->getPiece()))
                    possibleMoves.insert(currentPosition + Vector2(1, 1));
            }
            else
            {
                log::messageln("Black");
                if (currentPosition.x != 0
                        && _cells[currentPosition.x - 1][currentPosition.y]->getPiece().get() == nullptr)
                {
                    possibleMoves.insert(currentPosition + Vector2(-1, 0));
                    if (currentPosition.x == 6
                            && _cells[currentPosition.x - 2][currentPosition.y]->getPiece().get() == nullptr)
                        possibleMoves.insert(currentPosition + Vector2(-2, 0));
                }
                if (currentPosition.x != 0 && currentPosition.y > 0
                        && piece->isOpponentsPiece(_cells[currentPosition.x - 1][currentPosition.y - 1]->getPiece()))
                    possibleMoves.insert(currentPosition + Vector2(-1, -1));
                if (currentPosition.x != 0 && currentPosition.y < 7
                    && piece->isOpponentsPiece(_cells[currentPosition.x - 1][currentPosition.y + 1]->getPiece()))
                    possibleMoves.insert(currentPosition + Vector2(-1, 1));
            }
            //endregion
        }
        else if (piece->getType() == PieceType::Knight)
        {
            //region Knight
            targetPosition = currentPosition + Vector2(2, 1);
            if (isValidPosition(targetPosition)
                && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                    || piece->isOpponentsPiece(targetCellPiece))) {
                possibleMoves.insert(targetPosition);
            }

            targetPosition = currentPosition + Vector2(1, 2);
            if (isValidPosition(targetPosition)
                && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                    || piece->isOpponentsPiece(targetCellPiece))) {
                possibleMoves.insert(targetPosition);
            }

            targetPosition = currentPosition + Vector2(-1, 2);
            if (isValidPosition(targetPosition)
                && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                    || piece->isOpponentsPiece(targetCellPiece))) {
                possibleMoves.insert(targetPosition);
            }

            targetPosition = currentPosition + Vector2(-2, 1);
            if (isValidPosition(targetPosition)
                && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                    || piece->isOpponentsPiece(targetCellPiece))) {
                possibleMoves.insert(targetPosition);
            }

            targetPosition = currentPosition + Vector2(-1, -2);
            if (isValidPosition(targetPosition)
                && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                    || piece->isOpponentsPiece(targetCellPiece))) {
                possibleMoves.insert(targetPosition);
            }

            targetPosition = currentPosition + Vector2(-2, -1);
            if (isValidPosition(targetPosition)
                && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                    || piece->isOpponentsPiece(targetCellPiece))) {
                possibleMoves.insert(targetPosition);
            }

            targetPosition = currentPosition + Vector2(2, -1);
            if (isValidPosition(targetPosition)
                && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                    || piece->isOpponentsPiece(targetCellPiece))) {
                possibleMoves.insert(targetPosition);
            }

            targetPosition = currentPosition + Vector2(1, -2);
            if (isValidPosition(targetPosition)
                && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                    || piece->isOpponentsPiece(targetCellPiece))) {
                possibleMoves.insert(targetPosition);
            }
            //endregion
        }
        else if (piece->getType() == PieceType::King)
        {
            //region King
            for (int i = -1; i < 2; ++i)
                for (int j = -1; j < 2; ++j)
                {
                    targetPosition = currentPosition + Vector2(i, j);
                    if (isValidPosition(targetPosition)
                            && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                                || piece->isOpponentsPiece(targetCellPiece)))
                        possibleMoves.insert(targetPosition);
                }
            //endregion
        }
        else if (piece->getType() == PieceType::Rook)
        {
            //region Rook
            // Checking vertical line up
            for (int i = 1; i < 8; ++i)
            {
                targetPosition = currentPosition + Vector2(i, 0);
                if (isValidPosition(targetPosition)
                        && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                            || piece->isOpponentsPiece(targetCellPiece)))
                    possibleMoves.insert(targetPosition);
                // Stop iteration if there is an obstacle to go further
                if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr) break;
            }
            // Checking vertical line down
            for (int i = -1; i > -8; --i)
            {
                targetPosition = currentPosition + Vector2(i, 0);
                if (isValidPosition(targetPosition)
                    && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                        || piece->isOpponentsPiece(targetCellPiece)))
                    possibleMoves.insert(targetPosition);
                // Stop iteration if there is an obstacle to go further
                if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr) break;
            }
            // Checking horizontal line to the right
            for (int j = 1; j < 8; ++j)
            {
                targetPosition = currentPosition + Vector2(0, j);
                if (isValidPosition(targetPosition)
                    && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                        || piece->isOpponentsPiece(targetCellPiece)))
                    possibleMoves.insert(targetPosition);
                // Stop iteration if there is an obstacle to go further
                if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr) break;
            }
            // Checking horizontal line to the left
            for (int j = -1; j > -8; --j)
            {
                targetPosition = currentPosition + Vector2(0, j);
                if (isValidPosition(targetPosition)
                    && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                        || piece->isOpponentsPiece(targetCellPiece)))
                    possibleMoves.insert(targetPosition);
                // Stop iteration if there is an obstacle to go further
                if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr) break;
            }
            //endregion
        }
        else if (piece->getType() == PieceType::Bishop)
        {
            //region Bishop
            bool to_break = false;
            // Checking top-right diagonal
            for (int i = 1; i < 8; ++i)
            {
                for (int j = 1; j < 8; ++j)
                {
                    if (i != j) continue;
                    else
                    {
                        targetPosition = currentPosition + Vector2(i, j);
                        if (isValidPosition(targetPosition)
                            &&
                            ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                             || piece->isOpponentsPiece(targetCellPiece)))
                            possibleMoves.insert(targetPosition);
                            // Stop iteration if there is an obstacle to go further
                        if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr)
                        {
                            to_break = true;
                            break;
                        }
                    }
                }
                if (to_break) break;
            }
            to_break = false;
            // Checking bottom-right diagonal
            for (int i = -1; i > -8; --i)
            {
                for (int j = 1; j < 8; ++j)
                {
                    if (abs(i) != j) continue;
                    else
                    {
                        targetPosition = currentPosition + Vector2(i, j);
                        if (isValidPosition(targetPosition)
                            &&
                            ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                             || piece->isOpponentsPiece(targetCellPiece)))
                            possibleMoves.insert(targetPosition);
                            // Stop iteration if there is an obstacle to go further
                        if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr)
                        {
                            to_break = true;
                            break;
                        }
                    }
                }
                if (to_break) break;
            }
            to_break = false;
            // Checking bottom-left diagonal
            for (int i = -1; i > -8; --i)
            {
                for (int j = -1; j > -8; --j)
                {
                    if (i != j) continue;
                    else
                    {
                        targetPosition = currentPosition + Vector2(i, j);
                        if (isValidPosition(targetPosition)
                            &&
                            ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                             || piece->isOpponentsPiece(targetCellPiece)))
                            possibleMoves.insert(targetPosition);
                        // Stop iteration if there is an obstacle to go further
                        if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr)
                        {
                            to_break = true;
                            break;
                        }

                    }
                }
                if (to_break) break;
            }
            to_break = false;
            // Checking top-left diagonal
            for (int i = 1; i < 8; ++i)
            {
                for (int j = -1; j > -8; --j)
                {
                    if (i != abs(j)) continue;
                    else
                    {
                        targetPosition = currentPosition + Vector2(i, j);
                        if (isValidPosition(targetPosition)
                            &&
                            ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                             || piece->isOpponentsPiece(targetCellPiece)))
                            possibleMoves.insert(targetPosition);
                        // Stop iteration if there is an obstacle to go further
                        if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr)
                        {
                            to_break = true;
                            break;
                        }
                    }
                }
                if (to_break) break;
            }
            //endregion
        }
        else if (piece->getType() == PieceType::Queen)
        {
            //region Queen
            // Checking vertical line up
            for (int i = 1; i < 8; ++i)
            {
                targetPosition = currentPosition + Vector2(i, 0);
                if (isValidPosition(targetPosition)
                    && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                        || piece->isOpponentsPiece(targetCellPiece)))
                    possibleMoves.insert(targetPosition);
                // Stop iteration if there is an obstacle to go further
                if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr) break;
            }
            // Checking vertical line down
            for (int i = -1; i > -8; --i)
            {
                targetPosition = currentPosition + Vector2(i, 0);
                if (isValidPosition(targetPosition)
                    && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                        || piece->isOpponentsPiece(targetCellPiece)))
                    possibleMoves.insert(targetPosition);
                // Stop iteration if there is an obstacle to go further
                if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr) break;
            }
            // Checking horizontal line to the right
            for (int j = 1; j < 8; ++j)
            {
                targetPosition = currentPosition + Vector2(0, j);
                if (isValidPosition(targetPosition)
                    && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                        || piece->isOpponentsPiece(targetCellPiece)))
                    possibleMoves.insert(targetPosition);
                // Stop iteration if there is an obstacle to go further
                if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr) break;
            }
            // Checking horizontal line to the left
            for (int j = -1; j > -8; --j)
            {
                targetPosition = currentPosition + Vector2(0, j);
                if (isValidPosition(targetPosition)
                    && ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                        || piece->isOpponentsPiece(targetCellPiece)))
                    possibleMoves.insert(targetPosition);
                // Stop iteration if there is an obstacle to go further
                if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr) break;
            }
            bool to_break = false;
            // Checking top-right diagonal
            for (int i = 1; i < 8; ++i)
            {
                for (int j = 1; j < 8; ++j)
                {
                    if (i != j) continue;
                    else
                    {
                        targetPosition = currentPosition + Vector2(i, j);
                        if (isValidPosition(targetPosition)
                            &&
                            ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                             || piece->isOpponentsPiece(targetCellPiece)))
                            possibleMoves.insert(targetPosition);
                        // Stop iteration if there is an obstacle to go further
                        if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr)
                        {
                            to_break = true;
                            break;
                        }
                    }
                }
                if (to_break) break;
            }
            to_break = false;
            // Checking bottom-right diagonal
            for (int i = -1; i > -8; --i)
            {
                for (int j = 1; j < 8; ++j)
                {
                    if (abs(i) != j) continue;
                    else
                    {
                        targetPosition = currentPosition + Vector2(i, j);
                        if (isValidPosition(targetPosition)
                            &&
                            ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                             || piece->isOpponentsPiece(targetCellPiece)))
                            possibleMoves.insert(targetPosition);
                        // Stop iteration if there is an obstacle to go further
                        if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr)
                        {
                            to_break = true;
                            break;
                        }
                    }
                }
                if (to_break) break;
            }
            to_break = false;
            // Checking bottom-left diagonal
            for (int i = -1; i > -8; --i)
            {
                for (int j = -1; j > -8; --j)
                {
                    if (i != j) continue;
                    else
                    {
                        targetPosition = currentPosition + Vector2(i, j);
                        if (isValidPosition(targetPosition)
                            &&
                            ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                             || piece->isOpponentsPiece(targetCellPiece)))
                            possibleMoves.insert(targetPosition);
                        // Stop iteration if there is an obstacle to go further
                        if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr)
                        {
                            to_break = true;
                            break;
                        }

                    }
                }
                if (to_break) break;
            }
            to_break = false;
            // Checking top-left diagonal
            for (int i = 1; i < 8; ++i)
            {
                for (int j = -1; j > -8; --j)
                {
                    if (i != abs(j)) continue;
                    else
                    {
                        targetPosition = currentPosition + Vector2(i, j);
                        if (isValidPosition(targetPosition)
                            &&
                            ((targetCellPiece = _cells[targetPosition.x][targetPosition.y]->getPiece()).get() == nullptr
                             || piece->isOpponentsPiece(targetCellPiece)))
                            possibleMoves.insert(targetPosition);
                        // Stop iteration if there is an obstacle to go further
                        if (isValidPosition(targetPosition) && targetCellPiece.get() != nullptr)
                        {
                            to_break = true;
                            break;
                        }
                    }
                }
                if (to_break) break;
            }
            //endregion
        }
        piece->setPossibleMoves(possibleMoves);
        return piece->getPossibleMoves();
    }

private:
    std::array<std::array<spCell, 8>, 8> _cells = {};
    bool _isWhitesTurn = true;
    std::deque<spPiece> _eaten;
    spCell _source = nullptr;
    spCell _target = nullptr;

    void clickEventHandler(Event* ev)
    {
        spCell clickedCell = safeCast<Cell*>(ev->currentTarget.get());

        if (_source.get() == nullptr && clickedCell->getPiece().get() != nullptr)
        {
            // Protection from moving opponent's pieces
            if ((_isWhitesTurn && !clickedCell->getPiece()->isWhite())
                || (!_isWhitesTurn && clickedCell->getPiece()->isWhite()))
                return;

            _source = clickedCell;
            _source->setColor(Color::LawnGreen);

            auto result = getPossibleMoves(_source);
            log::messageln("Found %d potential moves", result.size());
            for (auto move : result)
            {
                log::messageln("Move: {%f %f}", move.x, move.y);
                _cells[move.x][move.y]->setColor(Color::Orange);
            }
        }
        else if (_source.get() != nullptr && _source == clickedCell)
        {
            for (auto move : getPossibleMoves(_source))
                _cells[move.x][move.y]->resetColor();

            _source->resetColor();
            if (_source->getPiece().get() != nullptr) _source->getPiece()->resetMoves();
            _source = nullptr;
        }
        else if (_source.get() != nullptr && clickedCell->getPiece().get() != nullptr
                 && clickedCell->getPiece()->isWhite() == _source->getPiece()->isWhite())
        {
            for (auto move : getPossibleMoves(_source))
                _cells[move.x][move.y]->resetColor();

            _source->resetColor();
            if (_source->getPiece().get() != nullptr) _source->getPiece()->resetMoves();
            _source = clickedCell;
            _source->setColor(Color::LawnGreen);

            for (auto move : getPossibleMoves(_source))
                _cells[move.x][move.y]->setColor(Color::Orange);
        }
        else if (_source.get() != nullptr && _source != clickedCell
                 && getPossibleMoves(_source).find(clickedCell->getCBPosition()) != getPossibleMoves(_source).end())
        {
            if (_target.get() != nullptr) _target->resetColor();
            _target = clickedCell;

            spPiece piece = _source->getPiece();
            piece->detach();

            if (_target->getPiece().get() != nullptr)
            {
                _eaten.push_front(_target->getPiece());
                _target->getPiece()->detach();
            }

            piece->attachTo(_target);
            _target->setPiece(_source->getPiece());

            for (auto move : getPossibleMoves(_source))
                _cells[move.x][move.y]->resetColor();

            if (_source->getPiece().get() != nullptr) _source->getPiece()->resetMoves();
            _source->setPiece(nullptr);
            _source->resetColor();
            _target->setColor(Color::Cyan);

            _source = nullptr;
            _isWhitesTurn = !_isWhitesTurn;
        }
    }

    bool isValidPosition(const Vector2& position)
    {
        return position.x >= 0 && position.x < 8 && position.y >= 0 && position.y < 8;
    }
};

DECLARE_SMART(Chessboard, spChessboard)

void example_preinit() {}

//called from main.cpp
void example_init()
{
    //load xml file with resources definition
    res::load();

    //lets create our client code simple actor
    spChessboard chessboard = new Chessboard;

    //and add it to Stage as child
    getStage()->addChild(chessboard);
}


//called each frame from main.cpp
void example_update()
{
}

//called each frame from main.cpp
void example_destroy()
{
    //free previously loaded resources
    res::free();
}
