#ifndef CSVREADER_H 	  			 	 
#define CSVREADER_H

#include <istream>
#include <iostream>
#include <string>
#include <vector>
#include <csv.h>
#include <sstream>
#include <list>



class CCSVReader{
    protected:
        // std::istream &input;
        // unsigned char c;
        char TempData[20000];
        size_t ByteReads;
        CCSVReader *c = 0; /// need to understand what is it doing here
        unsigned char start = 0;
        // then make a change
        // char * str;
        // void cb1 (void *s, size_t len, void *data);

        

        
    public:
        std::istream &input;

        CCSVReader(std::istream &in);
        ~CCSVReader();
        
        bool End() const;
        bool ReadRow(std::vector< std::string > &row);
        // std::vector <std::string > CurRow;
        int CRow;
        long unsigned columns;
        long unsigned rows;
        // std::vector <std::vector <std::string >> Buffer;
        std::vector <std::string > Buffer;
        std::list <std::vector <std::string >> Buffer2;
        csv_parser Data;
        static void cbb1(void *s, size_t len, void *data);
        static void cbb2(int c, void *data);
 
};

#endif
