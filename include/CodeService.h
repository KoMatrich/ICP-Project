#pragma once

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
    static void formatLine(size_t ln);
    static void clearBackground();
	void setCodeWindow(QTextEdit* c);
};
