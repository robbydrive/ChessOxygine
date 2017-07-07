#pragma once
#include "oxygine-framework.h"
#include "Chessboard.h"
#include "res.h"
using namespace oxygine;

DECLARE_SMART(GameManager, spGameManager)

class GameManager : public Actor
{
public:
    GameManager();

    void init();

private:
    spChessboard _chessboard = nullptr;
    spActor _blackZone;
    spActor _whiteZone;
    spTextField _infoField;

    void doUpdate(const UpdateState& us);
};

void game_preinit();
void game_init();
void game_update();
void game_destroy();