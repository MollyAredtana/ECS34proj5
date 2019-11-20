#include "XMLReader.h"
#include <iostream>
#include <string>

void CXMLReader::START(void *UserData, const XML_Char * Name, const XML_Char ** attris)
{ 
    SXMLEntity SX;
    auto ptr = static_cast<CXMLReader *> (UserData);
    SX.DNameData = Name;
    SX.DType = SXMLEntity::EType::StartElement;

    for(int i = 0; attris[i]; i += 2)
    {
        // make pair...
        SX.SetAttribute(attris[i], attris[i + 1]);
    }
    ptr->Buffer.push_back(SX);
}

void CXMLReader::END(void *UserData, const XML_Char * Name)
{
    SXMLEntity SX;
    auto ptr = static_cast<CXMLReader *> (UserData);
    SX.DNameData = Name;
    SX.DType = SXMLEntity::EType::EndElement;
    ptr->Buffer.push_back(SX);

}

void CXMLReader::Handler(void *userData, const XML_Char *s, int len)
{

    // C++ version
    SXMLEntity SX;

    auto ptr = static_cast<CXMLReader *> (userData);
    SX.DNameData = std::string(s, len);
    SX.DType = SXMLEntity::EType::CharData;
    ptr->Buffer.push_back(SX);
}

CXMLReader::CXMLReader(std::istream &is) : input(is)
{
    Parser = XML_ParserCreate(NULL);
    XML_SetUserData(Parser, this);
    XML_SetElementHandler(Parser, START, END);
    XML_SetCharacterDataHandler(Parser, Handler);
    
}
CXMLReader::~CXMLReader()
{
    XML_ParserFree(Parser);
    
}
        
bool CXMLReader::End()
{
    // the first case is the completely empty one
    // then we don't need to set up the XML tree
    int length = 0;
    input.seekg(0, input.end);
    if(input.tellg() == 0)
    {
        return true;
    }
    input.seekg(0, input.beg);

    return !input.eof() && Buffer.empty();


}
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata)
{
    // int check;
    // std::string mark;
    if(skipcdata)
    {
        return false;
    }
    
    while(!input.eof() && Buffer.empty())
    {
        SXMLEntity cg;

        if(!skipcdata)
        {
    
            input.read(TempData, sizeof(TempData));
            XML_Parse(Parser, TempData, input.gcount(), XML_TRUE);
            if(skipcdata == true && Buffer.front().DType == SXMLEntity::EType::CharData)
            {
                Buffer.pop_front();
            }
        }
        else{
            return false;
        }
    }

    if(Buffer.empty())
    {
        return false;
    }
    SXMLEntity cp;

    cp = Buffer.front();
    entity.DNameData = cp.DNameData;
    entity.DAttributes = cp.DAttributes;
    entity.DType = cp.DType;
    Buffer.pop_front();


    return true;
}


