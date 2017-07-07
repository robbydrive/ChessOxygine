#include "Chessboard.h"

Chessboard::Chessboard() : Actor()
{
    float side = 80.f, defaultSide = 64.f;
    Vector2 pieceSize = { side, side };
    // Offset for piece inside cell to achieve center position of the piece
    Vector2 pieceOffset = { 0, 0 };
    setSize(pieceSize * 8);
    Vector2 position = getSize() / 2;
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
                piece->setScale(side / defaultSide);
                piece->setPosition(pieceOffset);
                if (piece->getType() == PieceType::King && piece->isWhite())
                    _whiteKing = cell;
                else if (piece->getType() == PieceType::King)
                    _blackKing = cell;
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

const MovesSet& Chessboard::getPossibleMoves(spCell cell)
{
    spPiece piece = cell->getPiece();
//        if (piece->getPossibleMoves().size() > 0)
//            return piece->getPossibleMoves();
    const Vector2& currentPosition = cell->getCBPosition();
    Vector2 targetPosition;
    spPiece targetCellPiece;
    MovesSet possibleMoves;
    if (piece->getType() == PieceType::Pawn)
    {
        //region Pawn
        if (piece->isWhite())
        {
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

        // Checking possibility of castling
        if (!piece->wasMoved())
        {
            spPiece rook;
            bool noFiguresBetween = true;
            if ((rook = _cells[cell->getCBPosition().x][0]->getPiece()).get() != nullptr
                    && rook->getType() == PieceType::Rook
                    && !rook->wasMoved())
                for (int j = 1; j < 4; ++j)
                    if (_cells[cell->getCBPosition().x][j]->getPiece().get() != nullptr)
                    {
                        noFiguresBetween = false;
                        break;
                    }
                if (noFiguresBetween)
                    possibleMoves.insert(currentPosition + Vector2(0, -2));
            noFiguresBetween = true;
            if ((rook = _cells[cell->getCBPosition().x][7]->getPiece()).get() != nullptr
                    && rook->getType() == PieceType::Rook
                    && !rook->wasMoved())
                for (int j = 5; j < 7; ++j)
                    if (_cells[cell->getCBPosition().x][j]->getPiece().get() != nullptr)
                    {
                        noFiguresBetween = false;
                        break;
                    }
                if (noFiguresBetween)
                    possibleMoves.insert(currentPosition + Vector2(0, 2));
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

void Chessboard::clickEventHandler(Event* ev)
{
    // Block users interaction if checkmate
    if (_winner != Winner::Nobody) return;
    spCell clickedCell = safeCast<Cell*>(ev->currentTarget.get());

    if (_source.get() == nullptr && clickedCell->getPiece().get() != nullptr)
    {
        // Protection from moving opponent's pieces
        if ((_isWhitesTurn && !clickedCell->getPiece()->isWhite())
            || (!_isWhitesTurn && clickedCell->getPiece()->isWhite()))
            return;

        _source = clickedCell;
        _source->selectWithColor(Color::LawnGreen);

        for (auto move : getPossibleMoves(_source))
        {
            _cells[move.x][move.y]->select();
        }
    }
    else if (_source.get() != nullptr && _source == clickedCell)
    {
        for (auto move : getPossibleMoves(_source))
            _cells[move.x][move.y]->unSelect();

        _source->unSelectWithColor();
        if (_source->getPiece().get() != nullptr) _source->getPiece()->resetMoves();
        _source = nullptr;
    }
    else if (_source.get() != nullptr && clickedCell->getPiece().get() != nullptr
             && clickedCell->getPiece()->isWhite() == _source->getPiece()->isWhite())
    {
        for (auto move : getPossibleMoves(_source))
            _cells[move.x][move.y]->unSelect();

        _source->unSelectWithColor();
        if (_source->getPiece().get() != nullptr) _source->getPiece()->resetMoves();
        _source = clickedCell;
        _source->selectWithColor(Color::LawnGreen);

        for (auto move : getPossibleMoves(_source))
            _cells[move.x][move.y]->select();
    }
    else if (_source.get() != nullptr && _source != clickedCell
             && getPossibleMoves(_source).find(clickedCell->getCBPosition()) != getPossibleMoves(_source).end())
    {
        // Deleting trace to previous move
        if (_target.get() != nullptr) _target->unSelectWithColor();
        _target = clickedCell;

        // Resetting color for cells selected as possible moves
        for (auto possibleMove : getPossibleMoves(_source))
            _cells[possibleMove.x][possibleMove.y]->unSelect();

        // Resetting set of possible moves
        if (_source->getPiece().get() != nullptr) _source->getPiece()->resetMoves();

        move(_source, _target);
        _source->unSelectWithColor();
        _target->selectWithColor(Color::Cyan);

        // Checking whether there was a castling
        if (_target->getPiece()->getType() == PieceType::King
                && _target->getCBPosition() - _source->getCBPosition() == Vector2(0, 2))
        {
            spCell rookSource, rookTarget;
            rookSource = _cells[_target->getCBPosition().x][7];
            rookTarget = _cells[_target->getCBPosition().x][5];
            move(rookSource, rookTarget);
        }
        else if (_target->getPiece()->getType() == PieceType::King
                    && _target->getCBPosition() - _source->getCBPosition() == Vector2(0, -2))
        {
            spCell rookSource, rookTarget;
            rookSource = _cells[_target->getCBPosition().x][0];
            rookTarget = _cells[_target->getCBPosition().x][3];
            move(rookSource, rookTarget);
        }

        _source = nullptr;
        _isWhitesTurn = !_isWhitesTurn;
        _isCheck = false;

        // Checking for a check[mate]
        checkMate();
    }
}

void Chessboard::move(spCell source, spCell target)
{
    spPiece piece = source->getPiece();
    piece->detach();

    if (target->getPiece().get() != nullptr)
    {
        _eaten.push_front(target->getPiece());
        target->getPiece()->detach();
    }

    piece->attachTo(target);
    piece->setMoved(true);
    target->setPiece(source->getPiece());
    source->setPiece(nullptr);

    if (piece->getType() == PieceType::King && piece->isWhite())
        _whiteKing = target;
    else if (piece->getType() == PieceType::King)
        _blackKing = target;
}

void Chessboard::checkMate()
{
    bool isWhiteThreatened = false, isBlackThreatened = false;
    spCell currentCell;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if ((currentCell = _cells[i][j])->getPiece().get() != nullptr
                && currentCell->getPiece()->isWhite()
                && getPossibleMoves(currentCell).find(_blackKing->getCBPosition())
                   != getPossibleMoves(currentCell).end())
                isBlackThreatened = true;
            else if ((currentCell = _cells[i][j])->getPiece().get() != nullptr
                     && !currentCell->getPiece()->isWhite()
                     && getPossibleMoves(currentCell).find(_whiteKing->getCBPosition())
                        != getPossibleMoves(currentCell).end())
                isWhiteThreatened = true;
        }
    if (_isWhitesTurn && isBlackThreatened)
        _winner = Winner::Whites;
    else if (!_isWhitesTurn && isWhiteThreatened)
        _winner = Winner::Blacks;
    else if (_isWhitesTurn && isWhiteThreatened)
    {
        MovesSet blackMoves, threatMoves = cellsBetween(_target, _whiteKing);
        for (int i = 0; i < 8 && !_isCheck; ++i)
        {
            for (int j = 0; j < 8 && !_isCheck; ++j)
            {
                if ((currentCell = _cells[i][j])->getPiece().get() == nullptr)
                    continue;
                if (!currentCell->getPiece()->isWhite())
                {
                    blackMoves.insert(getPossibleMoves(currentCell).begin(),
                                      getPossibleMoves(currentCell).end());
                    currentCell->getPiece()->resetMoves();
                    continue;
                }
                MovesSet intersResult, possibleMoves = getPossibleMoves(currentCell);
                currentCell->getPiece()->resetMoves();
                std::set_intersection(possibleMoves.begin(), possibleMoves.end(),
                                      threatMoves.begin(), threatMoves.end(),
                                      std::inserter(intersResult, intersResult.begin()), vector2Cmp);
                // If white figure can eat a threat or cover the king from it, it is not a checkmate
                if (possibleMoves.find(_target->getCBPosition()) != possibleMoves.end()
                    || (intersResult.size() > 0 && currentCell->getPiece()->getType() != PieceType::King))
                {
                    _isCheck = true;
                    break;
                }
            }
        }
        MovesSet differResult;
        std::set_difference(getPossibleMoves(_whiteKing).begin(),
                            getPossibleMoves(_whiteKing).end(),
                            blackMoves.begin(), blackMoves.end(),
                            std::inserter(differResult, differResult.begin()), vector2Cmp);
        // If the king can go away from black figures' possible moves, it is not a checkmate
        if (!_isCheck && differResult.size() > 0)
            _isCheck = true;
        else if (!_isCheck)
            _winner = Winner::Blacks;
    }
    else if (!_isWhitesTurn && isBlackThreatened)
    {
        MovesSet whiteMoves, threatMoves = cellsBetween(_target, _blackKing);
        for (int i = 0; i < 8 && !_isCheck; ++i)
        {
            for (int j = 0; j < 8 && !_isCheck; ++j)
            {
                if ((currentCell = _cells[i][j])->getPiece().get() == nullptr)
                    continue;
                if (currentCell->getPiece()->isWhite())
                {
                    whiteMoves.insert(getPossibleMoves(currentCell).begin(),
                                      getPossibleMoves(currentCell).end());
                    currentCell->getPiece()->resetMoves();
                    continue;
                }
                MovesSet intersResult, possibleMoves = getPossibleMoves(currentCell);
                currentCell->getPiece()->resetMoves();
                std::set_intersection(possibleMoves.begin(), possibleMoves.end(),
                                      threatMoves.begin(), threatMoves.end(),
                                      std::inserter(intersResult, intersResult.begin()), vector2Cmp);
                // If black figure can eat a threat or cover the king from it, it is not a checkmate
                if (possibleMoves.find(_target->getCBPosition()) != possibleMoves.end()
                    || (intersResult.size() > 0 && currentCell->getPiece()->getType() != PieceType::King))
                {
                    _isCheck = true;
                    break;
                }
            }
        }
        MovesSet differResult;
        std::set_difference(getPossibleMoves(_blackKing).begin(),
                            getPossibleMoves(_blackKing).end(),
                            whiteMoves.begin(), whiteMoves.end(),
                            std::inserter(differResult, differResult.begin()), vector2Cmp);
        // If the king can go away from white figures' possible moves, it is not a checkmate
        if (!_isCheck && differResult.size() > 0)
            _isCheck = true;
        else if (!_isCheck)
            _winner = Winner::Whites;
    }
    if (_winner != Winner::Nobody)
        log::messageln("%s won", _winner == Winner::Whites ? "Whites" : "Blacks");
    else if (_isCheck)
        log::messageln("Check from %s", isWhiteThreatened ? "Blacks" : "Whites");
}

MovesSet Chessboard::cellsBetween(spCell cell1, spCell cell2)
{
    MovesSet result = {};
    // On the same line
    if (cell1->getCBPosition().x == cell2->getCBPosition().x)
    {
        for (int j = std::min(cell1->getCBPosition().y, cell2->getCBPosition().y) + 1;
                j < std::max(cell1->getCBPosition().y, cell2->getCBPosition().y); ++j)
            result.insert(Vector2(cell1->getCBPosition().x, j));
    }
    // In the same column
    else if (cell1->getCBPosition().y == cell2->getCBPosition().y)
    {
        for (int i = std::min(cell1->getCBPosition().x, cell2->getCBPosition().x) + 1;
             i < std::max(cell1->getCBPosition().x, cell2->getCBPosition().x); ++i)
            result.insert(Vector2(i, cell1->getCBPosition().y));
    }
    // On the diagonal
    else if (abs(cell1->getCBPosition().x - cell2->getCBPosition().x)
             == abs(cell1->getCBPosition().y - cell2->getCBPosition().y))
    {
        int incI, incJ, startI, startJ;
        incI = cell1->getCBPosition().x - cell2->getCBPosition().x > 0 ? -1 : 1;
        incJ = cell1->getCBPosition().y - cell2->getCBPosition().y > 0 ? -1 : 1;
        startI = cell1->getCBPosition().x - cell2->getCBPosition().x > 0 ?
                 cell1->getCBPosition().x - 1 : cell1->getCBPosition().x + 1;
        startJ = cell1->getCBPosition().y - cell2->getCBPosition().y > 0 ?
                 cell1->getCBPosition().y - 1 : cell1->getCBPosition().y + 1;
        for (int i = startI, j = startJ; i < std::max(cell1->getCBPosition().x, cell2->getCBPosition().x)
             && i > std::min(cell1->getCBPosition().x, cell2->getCBPosition().x)
             && j > std::min(cell1->getCBPosition().y, cell2->getCBPosition().y)
             && j < std::max(cell1->getCBPosition().y, cell2->getCBPosition().y); i += incI, j += incJ)
            result.insert(Vector2(i, j));
    }
    return result;
}
