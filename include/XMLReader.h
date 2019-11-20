#ifndef XMLREADER_H 	  			 	 
#define XMLREADER_H

#include "XMLEntity.h"
#include <istream>
#include <expat.h>
#include <list>

class CXMLReader{
    private:
        std::istream &input;
        SXMLEntity Se;
        std::string name;
        char * last_content;
        // CXMLReader * CCPtr; 
        XML_Parser Parser;
        int depth = 0;
        int done;    
    public:
        CXMLReader(std::istream &is);
        ~CXMLReader();
        
        bool End();
        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
        std::list <SXMLEntity> Buffer;

        std::list <std::vector <std::string > > Buffer2;
        char TempData[20000];

        static void START(void *UserData, const XML_Char * Name, const XML_Char ** attris);
        // it used to be a void ptr here ex :: void *UserData
        static void END(void *UserData, const XML_Char * Name);
        static void Handler(void *userData, const XML_Char *s, int len);

        // SXMLEntity SX;

        // void *UserData;
        // const XML_Char * Name;
        // const XML_Char ** attris;
    
        
        // int XML_Parse(XML_Parser p, const char *s, int len, int isFinal);
};

#endif
