#pragma once
#include <set>
#include "oxygine-framework.h"
using namespace oxygine;

struct Vector2Cmp
{
    bool operator() (const Vector2& first, const Vector2& second) const
    {
        if (first.x != second.x) return first.x < second.x;
        return first.y < second.y;
    }
};

DECLARE_SMART(Piece, spPiece)
using MovesSet = std::set<Vector2, Vector2Cmp>;

enum PieceType { King, Queen, Rook, Knight, Bishop, Pawn, None };

class Piece : public Sprite
{
public:
    Piece(PieceType type, bool isWhite);

    PieceType getType() const { return _type; }
    bool isWhite() const { return _isWhite; }
    bool isOpponentsPiece(const spPiece otherPiece) const;
    const MovesSet& getPossibleMoves() const { return _possibleMoves; }
    void setPossibleMoves(const MovesSet& set) { _possibleMoves = set; }
    void resetMoves() { if (!_possibleMoves.empty()) _possibleMoves.clear(); }
private:
    PieceType _type;
    bool _isWhite;
    MovesSet _possibleMoves = {};
};

