#pragma once

#include "Services/Highlight.h"

enum class HLevel
{
    LEVEL_OK, LEVEL_WARN, LEVEL_ERROR
};

class CodeService
{
private:
    static CodeService* instance;
    QTextEdit* code = nullptr;

public:
    static CodeService* getInstance()
    {
        if (!instance)
            instance = new CodeService;
        return instance;
    }
    static void setEndpoint(QTextEdit* c);
    static void formatLine(size_t ln, HLevel level);
    static void updatePos(size_t entity_ln, size_t x_ln, int x_val, size_t y_ln, int y_val);
    static void cacheUpdatePos(size_t entity_ln, size_t x_ln, int x_val, size_t y_ln, int y_val);
    static void insertLine(size_t ln, QString text);
    static void callCachedUpdatePos();
    static void clearBackground();
    void setCodeWindow(QTextEdit* c);
    size_t new_class_line;
    size_t new_x_line;
    size_t new_y_line;
    int new_x;
    int new_y;
};
