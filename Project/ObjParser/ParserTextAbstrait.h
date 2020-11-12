#ifndef PARSER_TEXT_ABSTRAIT
#define PARSER_TEXT_ABSTRAIT

#include <fstream>

class CParserTextAbstrait
{
public:
    CParserTextAbstrait(void);
    virtual ~CParserTextAbstrait(void);

    bool OpenFile(const std::string& Filename);
    void CloseFile(void);

    virtual void ParseFile(void) = 0;

protected:
    std::string  m_Filename;
    std::fstream m_FileStream;
    bool         m_IsOpen;
};

#endif
