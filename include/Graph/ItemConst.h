#pragma once

/// @brief item line type
enum class BlockType
{
    InheritedText, Text, SepSingle, SepBold, SepDouble
};

/// @brief item line data
class Block
{
public:
    /// @brief block type
    BlockType type;
    /// @brief block data
    QString data;
    /// @brief          constructor
    /// @param type     block data
    /// @param data 
    Block(BlockType type, QString data)
        :type(type), data(data)
    {
    };
};

//general constants
const int GRID_S = 12;
const int OFFSET{ GRID_S };
const int RADIUS{ GRID_S };

const QPoint POFFSET{ OFFSET, OFFSET };
const QSize SOFFSET{ OFFSET,OFFSET };

/// @brief metric for getting text size
QFontMetrics const metric = QFontMetrics{ QApplication::font() };

//arrow settings
const int ARROW_OFFSET{ 0 };
const float ARROW_GEN_SIZE{ 20.0 };
const float ARROW_COMAGG_SIZE{ 15.0 };
const float ARROW_SEQ_SIZE{ 18.0 };
const float SELF_ARROW_WIDTH{ 60.0 };
const float SELF_ARROW_HEIGHT{ 18.0 };

//SEQ settings
/// @brief Width of activation rectangle
const int ACTIVATION_W{ GRID_S * 2 };

/// @brief Action text height
const int ACTION_H{ GRID_S };
/// @brief Real action height
const int ACTION_RH{ ACTION_H + int(ARROW_GEN_SIZE) };
/// @brief (extra) Space between columns
const int COLUMN_SPACE{ GRID_S * 2 };
/// @brief Space under header
const int HEADER_SPACE{ GRID_S * 2 };
/// @brief Height of header
const int HEADER_HEIGHT{ GRID_S * 2 };
/// @brief Steam ending extra
const int STEM_EXTRA{ GRID_S };

const int BOX_OFFSET{ GRID_S };
const QMargins BOX_OFF{ BOX_OFFSET ,BOX_OFFSET ,BOX_OFFSET ,BOX_OFFSET };

//ERD settings
/// @brief separator height
const int SEPARATOR_H{ 3 };

const int pixOf = 0;
const QMargins BOUND_OF{ pixOf,pixOf,pixOf,pixOf };

inline QLinearGradient redG(uint cont_height)
{
    QLinearGradient gradient(0, 0, 0, cont_height);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, { 255, 220, 220 });
    gradient.setColorAt(0.8, { 240, 220, 220 });
    gradient.setColorAt(1.0, { 240, 220, 220 });
    return gradient;
}

inline QLinearGradient greenG(uint cont_height)
{
    QLinearGradient gradient(0, 0, 0, cont_height);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, { 220, 255, 220 });
    gradient.setColorAt(0.8, { 220, 240, 220 });
    gradient.setColorAt(1.0, { 220, 240, 220 });
    return gradient;
}

inline QLinearGradient blueG(uint cont_height)
{
    QLinearGradient gradient(0, 0, 0, cont_height);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, { 220, 220, 255 });
    gradient.setColorAt(0.8, { 220, 220, 240 });
    gradient.setColorAt(1.0, { 220, 220, 240 });
    return gradient;
}

//#define DEBUG_DRAW

/// @brief          paints debug info
/// @param painter  painter
/// @param obj      object
/// @note           use as last thing in paint function
/// @warn           clears all formating
void drawDebug(QPainter* painter, QGraphicsObject* obj);

void inline drawPoint(QPainter* painter)
{
    painter->drawEllipse(-3, -3, 6, 6);
}
