#include "oxygine-framework.h"
#include "Cell.h"
#include "Piece.h"

using namespace oxygine;

Cell::Cell(bool isWhite, const Vector2& position, const Vector2& size, PieceType type, bool isPieceWhite)
    : ColorRectSprite(), _isWhite(isWhite), _position(position)
{
    setColor(_isWhite ? Color::White : Color::Black);
    setSize(size);
    if (type != PieceType::None)
        _piece = new Piece(type, isPieceWhite);
}

const spPiece Cell::getPiece() const { return _piece; }

const Vector2& Cell::getCBPosition() const { return _position; }

void Cell::setPiece(const spPiece piece) { _piece = piece; }

void Cell::resetColor() { setColor(_isWhite ? Color::White : Color::Black); }
