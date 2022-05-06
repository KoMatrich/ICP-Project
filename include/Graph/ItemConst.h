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
const QSize SOFFSET{ GRID_S * 2, GRID_S * 2 };

/// @brief metric for getting text size
QFontMetrics const metric = QFontMetrics{ QApplication::font() };

//arrow settings
const int ARROW_OFFSET{ 0 };
const float ARROW_GEN_SIZE{ 20.0 };
const float ARROW_COMAGG_SIZE{ 15.0 };

//SEQ settings
/// @brief Width of activation rectangle
const int ACTIVATION_W{ GRID_S * 2 };

/// @brief Action text height
const int ACTION_H{ GRID_S };
/// @brief Real action height (with space)
const int ACTION_RH{ ACTION_H + GRID_S * 2 };
/// @brief (extra) Space between columns
const int COLUMN_SPACE{ GRID_S * 2 };
/// @brief Space under header
const int HEADER_SPACE{ GRID_S * 2 };

//ERD settings
/// @brief separator height
const int SEPARATOR_H{ 3 };

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
