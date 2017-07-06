#include "res.h"

namespace res
{
    Resources ui;

    void load()
    {
        ui.loadXML("data/res.xml");
    }

    void free()
    {
        ui.free();
    }
}
