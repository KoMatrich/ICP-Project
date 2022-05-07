#pragma once

#include "Services/Highlight.h"
#include "Syntax/Rules.h"

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
    /// @brief              sets global instance endpoint
    /// @param c            endpoint
    static void setEndpoint(QTextEdit* c);
    /// @brief              formats line
    /// @param ln           line index
    /// @param level        highlighting type
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
    /// @brief              deletes class definition from code
    /// @param start        first relevant line index
    /// @param end          last relevant line index
    static void deleteEntity(size_t start, size_t end);
    /// @brief              highlightes and jumps to class
    /// @param ln           first line of class
    static void highlightLine(size_t ln);
    /// @brief              first connection pair storing
    /// @param sel_name     name of calling class
    /// @param rule         connection type
    /// @param line         line to specify where to put the code
    static void relationRequest(QString sel_name, RuleID rule, size_t line);
    /// @brief              second connection pair storing and processing
    /// @param target       name of target class
    static void relationConnect(QString target);
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

    bool isConnecting = false;
    size_t rel_line = 0;
    QString sel_name;
    RuleID rule = RuleID::R_ERR;
};
