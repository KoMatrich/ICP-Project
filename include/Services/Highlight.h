#pragma once

class HighlightService
{
public:
    static HighlightService& getInstance()
    {
        static HighlightService inst;
        return inst;
    }
    //Highlighter* highlighter = nullptr;
    static void setEnabled(bool e);
    static bool getEnabled();
protected:
    //static void setEndpoint(Highlighter* c);
    HighlightService() {}; // Prevent construction
    HighlightService(const HighlightService&) {}; // Prevent construction by copying
    HighlightService& operator=(const HighlightService&) {}; // Prevent assignment
    ~HighlightService() {}; // Prevent unwanted destruction
    bool enabled = true;
};
