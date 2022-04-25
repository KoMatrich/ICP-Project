#pragma once

QT_BEGIN_NAMESPACE
class QString;
class QApplication;
QT_END_NAMESPACE

QFontMetrics const metric = QFontMetrics(QApplication::font());

//item data line type
enum class ItemDLT
{
	Text, Separator
};

//item data line
class ItemDL
{
public:
	ItemDL(ItemDLT type, QString data)
	{
		this->type = type;
		this->data = data;
	}

	ItemDLT type;
	QString data;
};

class ItemData
{
public:
	ItemData(QString text);
	QString Text() const
	{
		return text;
	}
	int Height()
	{
		return line_c * line_h + separator_c*separator_h;
	}
	int Line_h()
	{
		return line_h;
	}
	int Separator_h(){
		return separator_h;
	}
	int Width()
	{
		return width;
	}
	QSize Size()
	{
		return QSize(this->Width() , this->Height());
	}
	QVector<ItemDL> Lines()
	{
		return lines;
	}
private:
	QString text = "";

	int width = 0;

	int separator_c = 0;
	const int separator_h = 3;

	int line_h = 0;
	int line_c = 0;

	QVector<ItemDL> lines;
};