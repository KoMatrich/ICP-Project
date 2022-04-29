#include "CodeService.h"

void CodeService::setEndpoint(MainTextEdit* c)
{
    CodeService::getInstance()->setCodeWindow(c);
}

void CodeService::setCodeWindow(MainTextEdit* c)
{
	this->code = c;
}
