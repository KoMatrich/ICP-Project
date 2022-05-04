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
