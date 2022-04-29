#pragma once

#include "TextDebug.h"

#define VitaPrint(_text) DebugService::getInstance().printText(QString(_text))
#define VitaPrintf(_text, _format) DebugService::getInstance().printText(QString(_text)_format)
#define VF(_x) .arg(_x)

#define VitaClear() DebugService::getInstance().clearText()

class DebugService
{
private:
	static DebugService* instance;
	DebugTextEdit* debug = nullptr;
    void setDebug(DebugTextEdit* d);
protected:
    DebugService() {}; // Prevent construction
    DebugService(const DebugService&) {}; // Prevent construction by copying
    DebugService& operator=(const DebugService&) {}; // Prevent assignment
    ~DebugService() {}; // Prevent unwanted destruction
public:
	static DebugService& getInstance()
    {
        static DebugService inst;
        return inst;
    }
	static void setEndpoint(DebugTextEdit* d);
	static void printText(QString msg);
	static void clearText();
};
