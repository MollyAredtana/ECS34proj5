#include "CSVReader.h"
#include <iostream>
#include <string>



void CCSVReader::cbb1(void *str, size_t len, void *reader){
    auto Reader = static_cast<CCSVReader *> (reader);
    auto Column = std::string(static_cast<const char *> (str), len);
    Reader->Buffer.push_back(Column);
}


void CCSVReader::cbb2(int ch, void *reader){
	auto Reader = static_cast<CCSVReader *> (reader);
    Reader->Buffer2.push_back(Reader->Buffer);
    Reader->Buffer.clear();	
}


CCSVReader::CCSVReader(std::istream &in) : input(in)
{
    // std::cout << __LINE__ << " @  line   " << std::endl;
    csv_init(& Data, 0);
}
CCSVReader::~ CCSVReader()
{
    // std::cout << __LINE__ << " @  line   " << std::endl;
    csv_free(& Data);
    // void *c;

    // input.clear();
    // csv_fini(& Data, cb1, cb2, start);
    
}
        
bool CCSVReader::End() const  // not sure about this one
{
    // std::cout << __LINE__ << " @  line   " << std::endl;
    int length = 0;
    input.seekg(0, input.end);
    if(input.tellg() == 0)
    {
        return true;
    }
    input.seekg(0, input.beg);

    return !input.eof() and Buffer2.empty();



}
bool CCSVReader::ReadRow(std::vector< std::string > &row) 
{


    
    // std::cout << __LINE__ << " @ line " << std::endl;

    std::string Row;
    while(!input.eof() &&  Buffer2.empty())
    {
        // std::cout << " @ " << __LINE__ << std::endl;
        input.read(TempData, sizeof(TempData));
        ByteReads = input.gcount();
        csv_parse(& Data, TempData, ByteReads, cbb1, cbb2, this);
        if(input.eof())
        {
            csv_fini(& Data, cbb1, cbb2, this);
            break;
        }
    }

    if(!Buffer2.empty())
    {
        // std::cout << "  buffer size is  " << Buffer2.size() << std::endl;
        row.clear();
        row = Buffer2.front();
        Buffer2.pop_front();

        // for(auto i : Buffer2)
        // {
        //     for(auto j : i)
        //     {                
        //         row.push_back(j);
        //     }
        // }
        // Buffer2.clear();
        return true;
    }
    else{
        // std::cout << "  empty  " << std::endl;
        return false;
    }

}