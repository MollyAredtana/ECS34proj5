#include <gtest/gtest.h>
#include "CSVReader.h"
// #include "CSVWriter.h"
#include <sstream>


TEST(CSVReader, Emptyfile)
{
    std::stringstream input;
    CCSVReader Reader(input);
    std::vector<std::string> Row;

    EXPECT_FALSE(Reader.ReadRow(Row));

    EXPECT_TRUE(Reader.End());


}

TEST(CSVReader, SingleRow1)
{

    std::stringstream input("\"I call our world Flatland\",\x0a");
    CCSVReader Reader(input);
    std::vector<std::string> Row;

    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), 2);
    if(5 <= Row.size())
    {
        EXPECT_EQ(Row[0], "I call our world Flatland");
        EXPECT_EQ(Row[1], ",");
    }
    EXPECT_TRUE(Reader.End());

}


TEST(CSVReader, SingleRow2)
{

    std::stringstream input(",,,,,\x0a");
    CCSVReader Reader(input);
    std::vector<std::string> Row;

    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), 6);
    if(0 <= Row.size())
    {
        EXPECT_EQ(Row[0], "");
        EXPECT_EQ(Row[1], "");
       
    }
    EXPECT_TRUE(Reader.End());

}


TEST(CSVReader, MultipleRows)
{

    std::stringstream input("\"I call our world Flatland,\x0a"
                       "not because we call it so,\x0a"
                       "but to make its nature clearer\x0a"
                       "to you, my happy readers,\x0a"
                       "who are privileged to live in Space.\"");
    CCSVReader Reader(input);
    std::vector<std::string> Row;

    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), 1);
    EXPECT_EQ(Row[0], "I call our world Flatland,\nnot because we call it so,\nbut to make its nature clearer\nto you, my happy readers,\nwho are privileged to live in Space.");

    EXPECT_TRUE(Reader.End());

}

TEST(CSVReader, longnumbers)
{
    

    std::stringstream input("12345678901234567890123456789012");
    CCSVReader Reader(input);
    std::vector<std::string> Row;

    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), 1);

    if(0 <= Row.size())
    {
        EXPECT_EQ(Row[0], "12345678901234567890123456789012");
       
    }

    EXPECT_TRUE(Reader.End());

}

TEST(CSVReader, weirdnumbers)
{
    
    // std::cout << " a\0b\0c " << std::endl;

    std::stringstream input(" a\0b\0c ");
    CCSVReader Reader(input);
    std::vector<std::string> Row;

    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), 1);

    if(0 <= Row.size())
    {
        EXPECT_EQ(Row[0], "a");
       
    }

    EXPECT_TRUE(Reader.End());

}



// TEST(CSVWriter, Emptyfile)
// {
//     std::stringstream output;
//     CCSVWriter Writer(output);
//     std::vector<std::string> Row;
//     EXPECT_FALSE(Writer.WriteRow(Row));
// }


// TEST(CSVWriter, SingleRow)
// {
//     std::stringstream output;
//     CCSVWriter Writer(output);
//     std::vector<std::string> Row = {"1, 2, 3,\x0a\x0d\x0a  \"4\", \x0d\"\""};    
//     EXPECT_TRUE(Writer.WriteRow(Row));
//     // "1,2,3,4,5"
// }


// TEST(CSVWriter, MultipleRows1)
// {
//     std::stringstream output;
//     CCSVWriter Writer(output);
//     // confusion here :  can the writer read the numbers like 5 instad of "5"?
//     std::vector<std::string> Row = {"I call our world Flatland,\x0a",
//                      "tot because we call it so,\x0a",
//                      "but to make its nature clearer\x0a",
//                      "to you, my happy readers,\x0a",
//                      "who are privileged to live in Space."};
//     EXPECT_TRUE(Writer.WriteRow(Row));
// }

// TEST(CSVWriter, MultipleRows2)
// {
//     std::stringstream output;
//     CCSVWriter Writer(output);
//     std::vector<std::string> Row = {"Hello  ", "\"Whatever\"", "Heya,a"};
//     EXPECT_TRUE(Writer.WriteRow(Row));
// }