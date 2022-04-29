#pragma once

#include "TextCode.h"

class CodeService
{
private:
	static CodeService* instance;
    MainTextEdit* code = nullptr;

public:
	static CodeService* getInstance()
	{
		if (!instance)
			instance = new CodeService;
		return instance;
	}
	static void setEndpoint(MainTextEdit* c);
    static void formatLine(size_t ln);
    static void clearBackground();
	void setCodeWindow(MainTextEdit* c);
};
