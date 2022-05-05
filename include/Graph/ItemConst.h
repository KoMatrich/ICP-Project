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

const int GRID_S = 12;
const int OFFSET{ GRID_S / 2 };
const int RADIUS{ GRID_S };
const int SEPARATOR_H{ 3 };
const int ARROW_OFFSET{ 0 };
const float ARROW_GEN_SIZE{ 20.0 };
const float ARROW_COMAGG_SIZE{ 15.0 };

const QPoint POFFSET{ OFFSET,OFFSET };

/// @brief metric for getting text size
QFontMetrics const metric = QFontMetrics{ QApplication::font() };

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
