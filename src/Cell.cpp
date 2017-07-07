#include "oxygine-framework.h"
#include "Cell.h"
#include "res.h"

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

void Cell::select()
{
    spSprite dot = new Sprite;
    dot->setResAnim(res::ui.getResAnim("yellow_dot"));
    dot->setScale(getSize().x / dot->getSize().x * 0.6f);
    dot->setName("dot");
    dot->attachTo(this);
    // Expremintal formula to achieve centered position
    dot->setPosition(getSize() / 2 - dot->getSize() / 4 + Vector2(8, 8));
}

void Cell::selectWithColor(Color color)
{
    setColor(color);
    setAlpha(150);
}

void Cell::unSelect()
{
    removeChild(getChild("dot"));
}

void Cell::unSelectWithColor()
{
    resetColor();
    setAlpha(_defaultAlpha);
}
