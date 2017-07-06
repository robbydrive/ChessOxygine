#pragma once
#include <set>
#include "oxygine-framework.h"
using namespace oxygine;

DECLARE_SMART(Piece, spPiece)

enum PieceType { King, Queen, Rook, Knight, Bishop, Pawn, None };

class Piece : public Sprite
{
public:
    Piece(PieceType type, bool isWhite);

    PieceType getType() const { return _type; }
    bool isWhite() const { return _isWhite; }
    bool isOpponentsPiece(const spPiece otherPiece) const;
//    const std::set<Vector2>& getPossibleMoves() const;
private:
    PieceType _type;
    bool _isWhite;
    std::set<Vector2> _possibleMoves;
};

