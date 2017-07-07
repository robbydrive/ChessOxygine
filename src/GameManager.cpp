#include <string>
#include "GameManager.h"

GameManager::GameManager()
{

}

void GameManager::init()
{
    setSize(getStage()->getSize());

    _chessboard = new Chessboard;
    _chessboard->attachTo(this);

    Vector2 infoSize = Vector2(320, 200);
    TextStyle style;
    style.vAlign = TextStyle::VALIGN_MIDDLE;
    style.hAlign = TextStyle::HALIGN_MIDDLE;
    style.color = Color::White;
    style.font = res::ui.getResFont("main");
    style.fontSize = 48;

    _infoField = initActor(new TextField,
                           arg_style = style,
                           arg_size = infoSize,
                           arg_attachTo = this);

    _blackZone = new Actor;
    Vector2 size;
    size.x = getSize().x - _chessboard->getSize().x;
    size.y = getSize().y / 2 - _infoField->getSize().y / 2;
    _blackZone->setSize(size);
    _blackZone->setX(_chessboard->getSize().x);
    _blackZone->attachTo(this);
    _infoField->setPosition(_blackZone->getPosition() + Vector2(0, _blackZone->getHeight()));

    _whiteZone = new Actor;
    _whiteZone->setSize(size);
    _whiteZone->setPosition(_infoField->getPosition() + Vector2(0, _infoField->getHeight()));
    _whiteZone->attachTo(this);
    log::messageln("Black zone position: %f %f; size: %f %f", _blackZone->getPosition().x, _blackZone->getPosition().y,
                   _blackZone->getSize().x, _blackZone->getSize().y);
    log::messageln("Info field position: %f %f; size: %f %f", _infoField->getPosition().x, _infoField->getPosition().y,
                   _infoField->getSize().x, _infoField->getSize().y);
    log::messageln("White zone position: %f %f; size: %f %f", _whiteZone->getPosition().x, _whiteZone->getPosition().y,
                   _whiteZone->getSize().x, _whiteZone->getSize().y);
    log::messageln("Chessboard position: %f %f; size: %f %f", _chessboard->getPosition().x, _chessboard->getPosition().y,
                   _chessboard->getSize().x, _chessboard->getSize().y);
}

void GameManager::doUpdate(const UpdateState &us)
{
    Actor::doUpdate(us);

    if (_chessboard.get() != nullptr)
    {
        std::string message;
        if (_chessboard->getWinner() == Winner::Nobody && _chessboard->isCheck())
            message.append("Check!\n");
        if (_chessboard->getWinner() == Winner::Nobody)
            message.append(_chessboard->isWhitesTurn() ? "White goes" : "Black goes");
        if (_chessboard->getWinner() != Winner::Nobody)
            message.append("Checkmate! " + Winner::Whites ? "White wins!" : "Black wins!");
        _infoField->setText(message);

        _blackZone->removeChildren();
        _whiteZone->removeChildren();

        int whites = 0, blacks = 0;
        auto eaten = _chessboard->getEaten();
        auto lastElement = eaten.end();
        for (auto it = eaten.begin(); it != lastElement; ++it)
        {
            if ((*it)->isWhite())
            {
                Vector2 position = Vector2(0, 0);
                if (_whiteZone->getLastChild().get() != nullptr)
                {
                    position = _whiteZone->getLastChild()->getPosition();
                    position.x += _whiteZone->getLastChild()->getWidth();
                    if (position.x + (*it)->getWidth() > _whiteZone->getSize().x)
                    {
                        position.x = 0;
                        position.y += (*it)->getSize().y;
                    }
                }
                (*it)->attachTo(_whiteZone);
                (*it)->setPosition(position);
                ++whites;
            }
            else
            {
                Vector2 position = Vector2(0, 0);
                if (_blackZone->getLastChild().get() != nullptr)
                {
                    position = _blackZone->getLastChild()->getPosition();
                    position.x += _blackZone->getLastChild()->getWidth();
                    if (position.x + (*it)->getWidth() > _blackZone->getSize().x)
                    {
                        position.x = 0;
                        position.y += (*it)->getSize().y;
                    }
                }
                (*it)->attachTo(_blackZone);
                (*it)->setPosition(position);
                ++blacks;
            }
        }
    }
}

void game_preinit() {}

//called from main.cpp
void game_init()
{
    //load xml file with resources definition
    res::load();

    //lets create our client code simple actor
    spGameManager manager = new GameManager;
    manager->setName("manager");
    manager->init();

    //and add it to Stage as child
    getStage()->addChild(manager);
}


//called each frame from main.cpp
void game_update() {}

//called each frame from main.cpp
void game_destroy()
{
    //free previously loaded resources
    res::free();
}
