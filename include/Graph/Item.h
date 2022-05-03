#pragma once

//item data line type
enum class BlockType
{
    Text, SepSingle, SepBold, SepDouble
};

//item data line
class Block
{
public:
    Block(BlockType type, QString data)
    {
        this->type = type;
        this->data = data;
    }

    BlockType type;
    QString data;
};

const int GRID_S = 12;
const int OFFSET{ GRID_S / 2 };
const int RADIUS{ GRID_S };
const int SEPARATOR_H{ 3 };

const QMargins MARGIN(1, 1, 1, 1);

const QPoint POFFSET{ OFFSET,OFFSET };

//TODO remove
#define DEBUG_DRAW
