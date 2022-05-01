#pragma once

enum class HLevel {
    LEVEL_OK, LEVEL_WARN, LEVEL_ERROR
};

class CodeService
{
private:
	static CodeService* instance;
    QTextEdit* code = nullptr;

public:
	static CodeService* getInstance()
	{
		if (!instance)
			instance = new CodeService;
		return instance;
	}
	static void setEndpoint(QTextEdit* c);
    static void formatLine(size_t ln, HLevel level);
    static void insertLine(size_t ln, QString text);
    static void clearBackground();
	void setCodeWindow(QTextEdit* c);
};
