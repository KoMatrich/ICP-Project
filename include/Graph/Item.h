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
