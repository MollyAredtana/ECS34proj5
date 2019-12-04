#ifndef CSVWRITER_H 	  			 	 
#define CSVWRITER_H

#include <ostream>
#include <string>
#include <vector>
#include <csv.h>
#include <list>

class CCSVWriter{
    protected:
    	std::ostream &DOutput;
    	

    public:
        CCSVWriter(std::ostream &ou);
        ~CCSVWriter();

        bool WriteRow(const std::vector< std::string > &row);
        
};

#endif
