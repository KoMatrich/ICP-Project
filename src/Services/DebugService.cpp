#include "services/DebugService.h"

void DebugService::setEndpoint(DebugTextEdit* d)
{
    DebugService::getInstance().setDebug(d);
}

void DebugService::printText(QString msg)
{
    if (!DebugService::getInstance().debug)
        return;

    DebugService::getInstance().debug->append(msg);
}

void DebugService::clearText()
{
    if (!DebugService::getInstance().debug)
        return;

    DebugService::getInstance().debug->clear();
}

void DebugService::setDebug(DebugTextEdit* d)
{
    this->debug = d;
}
