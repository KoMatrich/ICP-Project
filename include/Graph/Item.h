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
const int ARROW_OFFSET{ 0 };
const float ARROW_GEN_SIZE{ 20.0 };
const float ARROW_COMAGG_SIZE{ 15.0 };

const QPoint POFFSET{ OFFSET,OFFSET };

//TODO remove
#define DEBUG_DRAW
