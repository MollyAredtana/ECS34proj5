#include <gtest/gtest.h>
#include "XMLReader.h"
// #include "XMLWriter.h"
#include <sstream>


TEST(XMLReader, Emptyfile)
{
    std::stringstream input;
    CXMLReader Reader(input);
    EXPECT_TRUE(Reader.End());


}

TEST(XMLReader, Simpletest)
{
    // " 1,2 ,  3         ,4,5\x0d\x0a";

    std::stringstream input("<ppp><other></other></ppp>");
    // std::cout << "\"I call our world Flatland\",\x0a" << std::endl;

    CXMLReader Reader(input);

    SXMLEntity Entity;

    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Reader.End());

}

TEST(XMLReader, ComplexTest)
{
    std::stringstream input("<ppp><other>hhh</other></ppp>"); // abc=\"dfg\"
    // std::cout << "\"I call our world Flatland\",\x0a" << std::endl;

    CXMLReader Reader(input);

    SXMLEntity Entity;

    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    // EXPECT_TRUE(Entity.AttributeExists(Entity.DNameData));
    // EXPECT_EQ(Entity.AttributeValue(Entity.DNameData), "dfg");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "hhh");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Reader.End());
}


TEST(XMLReader, MoreComplexTest)
{
    std::stringstream input("<ppp abc=\"dfg\"><other></other></ppp>"); // abc=\"dfg\"
    CXMLReader Reader(input);
    SXMLEntity Entity;

    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Entity.AttributeExists("abc"));
    EXPECT_EQ(Entity.AttributeValue("abc"),"dfg");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Reader.End());
}


TEST(XMLReader, MoreMoreComplexTest)
{
    std::stringstream input("<ppp abc=\"dfg\"><other>hhh</other></ppp>"); // abc=\"dfg\"
    CXMLReader Reader(input);
    SXMLEntity Entity;

    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Entity.AttributeExists("abc"));
    EXPECT_EQ(Entity.AttributeValue("abc"),"dfg");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "hhh");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Reader.End());
}


TEST(XMLReader, SuperComplexTest)
{
    std::stringstream input("<ppp abc=\"dfg\" cln=\"idol\" ><other>hhh</other></ppp>"); // abc=\"dfg\"
    CXMLReader Reader(input);
    SXMLEntity Entity;

    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Entity.AttributeExists("abc"));
    EXPECT_EQ(Entity.AttributeValue("abc"),"dfg");
    EXPECT_TRUE(Entity.AttributeExists("cln"));
    EXPECT_EQ(Entity.AttributeValue("cln"),"idol");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_FALSE(Reader.End());
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "hhh");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Reader.End());
}

TEST(XMLReader, skip1)
{
    // " 1,2 ,  3         ,4,5\x0d\x0a";

    std::stringstream input("<ppp><other></other></ppp>");
    // std::cout << "\"I call our world Flatland\",\x0a" << std::endl;

    CXMLReader Reader(input);

    SXMLEntity Entity;

    EXPECT_FALSE(Reader.ReadEntity(Entity, true));
    EXPECT_FALSE(Reader.ReadEntity(Entity, true));
    EXPECT_FALSE(Reader.ReadEntity(Entity, true));
    EXPECT_FALSE(Reader.ReadEntity(Entity, true));
    EXPECT_TRUE(Reader.End());

}

TEST(XMLReader, skip2)
{
    // " 1,2 ,  3         ,4,5\x0d\x0a";

    std::stringstream input("<ppp><other><jjj></jjj></other></ppp>");
    // std::cout << "\"I call our world Flatland\",\x0a" << std::endl;

    CXMLReader Reader(input);

    SXMLEntity Entity;

    EXPECT_FALSE(Reader.ReadEntity(Entity, true));
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "ppp");
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "other");
    EXPECT_FALSE(Reader.End());

}


// TEST(XMLWriter, Single)
// {
//     std::stringstream output;
//     CXMLWriter Writer(output);
//     SXMLEntity Test;
//     Test.DType = SXMLEntity::EType::CompleteElement;
//     Test.DNameData = "Hello Greetings = \"Hi\"";
// 	EXPECT_TRUE(Writer.WriteEntity(Test));
// 	EXPECT_EQ(output.str(), "<Hello Greetings = \"Hi\"/>");
// }

// TEST(XMLWriter, StartandEnd)
// {
//     std::stringstream output;
//     CXMLWriter Writer(output);
//     SXMLEntity Test1, Test2;
//     Test1.DType = SXMLEntity::EType::StartElement;
//     Test1.DNameData = "Hello";
//     Test2.DType = SXMLEntity::EType::EndElement;
//     Test2.DNameData = "Hello";
// 	EXPECT_TRUE(Writer.WriteEntity(Test1));
// 	EXPECT_TRUE(Writer.WriteEntity(Test2));
// 	EXPECT_EQ(output.str(), "<Hello></Hello>");
// }

// TEST(XMLWriter, Flush)
// {
//     std::stringstream output;
//     CXMLWriter Writer(output);
//     SXMLEntity Test1, Test2;
//     Test1.DType = SXMLEntity::EType::StartElement;
//     Test1.DNameData = "Hello";
//     Test2.DType = SXMLEntity::EType::EndElement;
//     Test2.DNameData = "Hello";
//     Test2.DType = SXMLEntity::EType::EndElement;
// 	EXPECT_TRUE(Writer.WriteEntity(Test1));
// 	EXPECT_TRUE(Writer.Flush());
// 	EXPECT_EQ(output.str(), "<Hello></Hello>");
// }

// TEST(XMLWriter, LargerFile)
// {
//     std::stringstream output;
//     CXMLWriter Writer(output);
//     SXMLEntity Test1, Test2, Test3, Test4, Test5, Test6, Test7;
//     Test1.DType = SXMLEntity::EType::StartElement;
//     Test1.DNameData = "Hello";
//     Test2.DType = SXMLEntity::EType::CharData;
//     Test2.DNameData = "\n ";
//     Test3.DType = SXMLEntity::EType::CompleteElement;
//     Test3.DNameData = "Greeting Hi = \"Good Morning\"";
//     Test4.DType = SXMLEntity::EType::CharData;
//     Test4.DNameData = "\n ";
//     Test5.DType = SXMLEntity::EType::CompleteElement;
//     Test5.DNameData = "Greeting Hey = \"Good Afternoon\"";
//     Test6.DType = SXMLEntity::EType::CharData;
//     Test6.DNameData = "\n";
//     Test7.DType = SXMLEntity::EType::EndElement;
//     Test7.DNameData = "Hello";
    
// 	EXPECT_TRUE(Writer.WriteEntity(Test1));
// 	EXPECT_TRUE(Writer.WriteEntity(Test2));
// 	EXPECT_TRUE(Writer.WriteEntity(Test3));
// 	EXPECT_TRUE(Writer.WriteEntity(Test4));
// 	EXPECT_TRUE(Writer.WriteEntity(Test5));
// 	EXPECT_TRUE(Writer.WriteEntity(Test6));
// 	EXPECT_TRUE(Writer.WriteEntity(Test7));
// 	EXPECT_EQ(output.str(), "<Hello>\n <Greeting Hi = \"Good Morning\"/>\n <Greeting Hey = \"Good Afternoon\"/>\n</Hello>");
// }

// TEST(XMLWriter, NestedFlush){
	
//     std::stringstream output;
//     CXMLWriter Writer(output);
//     SXMLEntity Test1, Test2, Test3, Test4, Test5, Test6, Test7;
//     Test1.DType = SXMLEntity::EType::StartElement;
//     Test1.DNameData = "Hello";
//     Test2.DType = SXMLEntity::EType::StartElement;
//     Test2.DNameData = "Hey";
//     Test3.DType = SXMLEntity::EType::StartElement;
//     Test3.DNameData = "Greetings";
	
// 	EXPECT_TRUE(Writer.WriteEntity(Test1));
// 	EXPECT_TRUE(Writer.WriteEntity(Test2));
// 	EXPECT_TRUE(Writer.WriteEntity(Test3));
// 	EXPECT_TRUE(Writer.Flush());
// 	EXPECT_EQ(output.str(), "<Hello><Hey><Greetings></Greetings></Hey></Hello>");
// }