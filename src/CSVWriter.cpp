#include "CSVWriter.h"
#include "StringUtils.h"
#include <string>
#include <iostream>


CCSVWriter::CCSVWriter(std::ostream &ou) : DOutput(ou)
{
}

CCSVWriter::~CCSVWriter()
{
}

bool CCSVWriter::WriteRow(const std::vector< std::string > &row)
{
	if(row.empty()){
		return false;
	}
	std::string all;
	std::string temp;
	bool Quotes = false;
	std::vector<std::string > Temp;
	
	for(int i = 0; i < row.size(); i++) {
		temp = row[i];
		if(!(temp.find('\"') == std::string::npos)){
			temp = StringUtils::Replace(temp, "\"", "\'");
			Temp.push_back(temp);
			Quotes = true;
		} else if(!(temp.find(",") == std::string::npos) || !(temp.find(' ' ) == std::string::npos)) {
			Quotes = true;
			Temp.push_back(temp);
		}
	}
	
	if(Quotes == true){
		all ="\"";
		all+=StringUtils::Join("\", \"", Temp);
		all+="\"";
	} else {
		all = StringUtils::Join(",", Temp);
	}
	
	DOutput << all;
    
	return true;
}