#include <Graph/DragItemPrecomp.h>

ItemData::ItemData(QString text)
{
	this->text = text;

	QRegExp separators = QRegExp("(\\-|=|_){2}");

	for (QString line : text.split("\n")) {
		QString line_tr = line.trimmed();

		if (!separators.exactMatch(line_tr)) {
			lines.append(ItemDL(ItemDLT::Text, line_tr));
			
			auto size = metric.size(Qt::TextLongestVariant, line_tr);
			
			width = qMax(width, size.width());
			line_h = qMax(line_h, size.height())+1.1;
			line_c++;
			continue;
		}

		//separators
		QChar c = *line_tr.data();
		switch (c.toLatin1()) {
		case '-':
			lines.append(ItemDL(ItemDLT::Separator, "-"));
			break;
		case '_':
			lines.append(ItemDL(ItemDLT::Separator, "_"));
			break;
		case '=':
			lines.append(ItemDL(ItemDLT::Separator, "="));
			break;
		default:
			qDebug("Unknown selector (DragItemPrecomp)");
			break;
		}

		separator_c++;
	}
}
