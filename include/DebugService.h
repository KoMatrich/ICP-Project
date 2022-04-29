#pragma once

#include "TextDebug.h"

#define VitaPrint(_text) DebugService::getInstance()->printText(QString(_text))
#define VitaPrintf(_text, _format) DebugService::getInstance()->printText(QString(_text)_format)
#define VF(_x) .arg(_x)

#define VitaClear() DebugService::getInstance()->clearText()

class DebugService
{
private:
	static DebugService* instance;
	DebugTextEdit* debug = nullptr;
    void setDebug(DebugTextEdit* d);

public:
	static DebugService* getInstance()
	{
		if (!instance)
			instance = new DebugService;
		return instance;
	}
	static void setEndpoint(DebugTextEdit* d);
	static void printText(QString msg);
	static void clearText();
};
