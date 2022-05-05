#include "Services/History.h"

void HistoryService::setEndpoint(QTextEdit* c)
{
    HistoryService::getInstance().setCodeWindow(c);
}

void HistoryService::takeHistorySnapshot()
{
    HistoryService& instance = HistoryService::getInstance();

    // if empty, always push
    if (instance.history.empty())
    {
        instance.history.push_back(instance.code->document()->toRawText());
        return;
    }

    /// push up to max size, delete front if needed
    if (instance.history.size() > MAX_HIST_SIZE)
    {
        instance.history.erase(instance.history.begin());
    }
    instance.history.push_back(instance.code->document()->toRawText());
}

void HistoryService::restoreHistorySnapshot()
{
    HistoryService& instance = HistoryService::getInstance();
    QString snapshot;
    QString current;
    bool changed = false;
    while (!changed)
    {
        if (instance.history.empty()) {
            return;
        }

        snapshot = instance.history.back();
        current = instance.code->document()->toRawText();
        if (current != snapshot)
        {
            changed = true;
        }
        instance.history.pop_back();
        instance.code->document()->setPlainText(snapshot);
    }
    instance.history.push_back(snapshot);

}

void HistoryService::setCodeWindow(QTextEdit* c)
{
    this->code = c;
}
