#pragma once

#include "Services/Highlight.h"

/// @brief highlighting type
enum class HLevel
{
    LEVEL_OK, LEVEL_WARN, LEVEL_ERROR
};

/// @brief code editor service
class CodeService : public Service<CodeService>
{
private:
    /// @brief endpoint
    QTextEdit* code = nullptr;
public:
    /// @brief      sets global instance endpoint
    /// @param c    endpoint
    static void setEndpoint(QTextEdit* c);
    /// @brief          formats line
    /// @param ln       line index
    /// @param level    highlighting type
    static void formatLine(size_t ln, HLevel level);
    /// @brief              updates position value
    /// @param entity_ln    entity start line index
    /// @param x_ln         x position line index
    /// @param x_val        value of x
    /// @param y_ln         y position line index
    /// @param y_val        value of y
    static void updatePos(size_t entity_ln, size_t x_ln, int x_val, size_t y_ln, int y_val);
    /// @brief              store last position of object
    /// @param entity_ln    entity start line index
    /// @param x_ln         x position line index
    /// @param x_val        value of x
    /// @param y_ln         y position line index
    /// @param y_val        value of y
    static void cacheUpdatePos(size_t entity_ln, size_t x_ln, int x_val, size_t y_ln, int y_val);
    /// @brief				restore cashed position
    static void callCachedUpdatePos();
    /// @brief              inserts line after line index
    /// @param ln           line index
    /// @param text         new line text
    static void insertLine(size_t ln, QString text);
    /// @brief              sets a flag if position updates should be accepted
    /// @param val          new flag value
    static void setPosActive(bool val);
    /// @brief              restores cashed position
    static void clearBackground();
private:
    /// @brief              non static endpoint set
    /// @param c            endpoint
    void setCodeWindow(QTextEdit* c);
    bool isPosActive = false;
    size_t new_class_line = 0;
    size_t new_x_line = 0;
    size_t new_y_line = 0;
    int new_x = 0;
    int new_y = 0;
};
