#include "Services/Highlight.h"

void HighlightService::setEnabled(bool e)
{
    HighlightService::getInstance().enabled = e;
}

bool HighlightService::getEnabled()
{
    return HighlightService::getInstance().enabled;
}
