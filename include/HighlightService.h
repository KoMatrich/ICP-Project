#pragma once

class HighlightService
{
public:
    static HighlightService& getInstance()
    {
        static HighlightService inst;
        return inst;
    }
    static void setEnabled(bool e);
    static bool getEnabled();
protected:
    HighlightService() {}; // Prevent construction
    HighlightService(const HighlightService&) {}; // Prevent construction by copying
    HighlightService& operator=(const HighlightService&) {}; // Prevent assignment
    ~HighlightService() {}; // Prevent unwanted destruction
    bool enabled;
};
