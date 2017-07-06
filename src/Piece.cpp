#include <string>
#include "oxygine-framework.h"
#include "Piece.h"
#include "res.h"

using namespace oxygine;

Piece::Piece(PieceType type, bool isWhite)
    : Sprite(), _type(type), _isWhite(isWhite)
{
    int row = isWhite ? 1 : 0;
    setAnimFrame(res::ui.getResAnim("pieces"), type, row);
}

bool Piece::isOpponentsPiece(const spPiece otherPiece) const
{
    if (otherPiece.get() == nullptr) return false;
    return (_isWhite && !otherPiece->isWhite()) || (!isWhite() && otherPiece->isWhite());
}
