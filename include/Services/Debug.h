#pragma once

#include "Services/Service.h"
#include "TextDebug.h"

#define VitaPrint(_text) DebugService::getInstance().printText(QString(_text))
#define VitaPrintf(_text, _format) DebugService::getInstance().printText(QString(_text)_format)
#define VF(_x) .arg(_x)

#define VitaClear() DebugService::getInstance().clearText()

/// @brief debug console service
/// used to print to debug console
class DebugService : public Service<DebugService>
{
private:
    /// @brief endpoint
    DebugTextEdit* debug = nullptr;
    /// @brief      sets debug console endpoint
    /// @param d    endpoint
    void setDebug(DebugTextEdit* d);

public:
    /// @brief      sets endpoint
    /// @param d    endpoint
    static void setEndpoint(DebugTextEdit* d);
    /// @brief      prints text in debug console
    /// @param msg  text
    static void printText(QString msg);
    /// @brief      clears debug text console
    static void clearText();
};
