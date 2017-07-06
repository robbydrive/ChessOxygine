#pragma once
#include "oxygine-framework.h"
#include "Piece.h"
using namespace oxygine;

class Cell : public ColorRectSprite
{
public:
    Cell(bool isWhite, const Vector2& position,
         const Vector2& size = { 64, 64 }, PieceType type = PieceType::None, bool isPieceWhite = true);

    const spPiece getPiece() const;
    const Vector2& getCBPosition() const;

    void setPiece(const spPiece piece);

    void resetColor();
private:
    bool _isWhite;
    spPiece _piece = nullptr;
    Vector2 _position;
};
