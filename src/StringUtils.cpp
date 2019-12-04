#include "StringUtils.h" // remember to copy the header file to clion so I can run the test here
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
//#include <string> // just to make it look better
//#include <vector>// just to make it look better


namespace StringUtils {

    std::string
    Slice(const std::string &str, ssize_t start, ssize_t end) {    //       ~~~~~~~~~ don't forget to rewrite slice
        // Your code goes here
        auto SString = str;
        int strLength = str.size();
        auto empty = "";
        if (start >= 0) {  // the case that start > 0
            if (end > 0) {

                int pos = 0;
                for (int i = start; i < end; i++) {
                    SString[pos] = str[i];
                    pos++;
                }
                int rest = strLength - end;
                SString.resize(strLength - rest - start);

            } else if (end == 0) {
                if (start < strLength) {
                    int pos = 0;

                    for (int i = start; i < strLength; i++) {
                        SString[pos] = str[i];
                        pos++;
                    }
                    SString.resize(strLength - start);
                } else if (start >= strLength) {
                    SString = empty;

                }
            } else if (end < 0) {
                int pos = 0;
                int last = strLength + end;
                int rest = end;
                for (int i = start; i <= last; i++) {
                    SString[pos] = str[i];
                    pos++;
                }
                SString.resize(strLength + rest - start);
            }

        } else if (start < 0) { //  the case that start < 0
            int newstart = strLength + start;
            if (end > 0) {

                int pos = 0;
                for (int i = newstart; i < end; i++) {
                    SString[pos] = str[i];
                    pos++;
                }
                int rest = strLength - end;
                SString.resize(strLength - rest + start);

            } else if (end == 0) {
                int pos = 0;

                for (int i = newstart; i < strLength; i++) {
                    SString[pos] = str[i];
                    pos++;
                }
                SString.resize(strLength + start);
            } else if (end < 0) {
                int pos = 0;
                int last = strLength + end;
                for (int i = newstart; i <= last; i++) {
                    SString[pos] = str[i];
                    pos++;
                }
                SString.resize(last - newstart);
            }


        }


        return SString;
    }

    std::string Capitalize(const std::string &str) {
        // Your code goes here
        auto CString = str;
        int first = 0;
        int strLength = str.length();
        CString[first] = toupper(str[first]);
        for (int i = 1; i < strLength; i++) {
            CString[i] = tolower(str[i]);
        }
        return CString;

    }

    std::string Title(const std::string &str) {
        // Your code goes here
        auto TString = str;
        int first = 0;
        int count = 0;
        auto empty = str;
        int strLength = str.length();
        // find the first letter and count how many empty spaces before the first string

        if (!isspace(str[first]) && first == 0) {
            TString[first] = toupper(str[first]);
        } else if (isspace(str[first])) {
            while (isspace(str[first])) {   // title the first letter
                first++;
            }

            TString[first] = toupper(str[first]);
        }

        // add all empty spaces in
        while (count < first) {
            TString[count] = str[count];
            count++;
        }

        // check the rest
        for (int i = count + 1; i < strLength; i++) {
            // first lower the rest
            TString[i] = tolower(str[i]);

            // then find out what else need to upper

            if (isalpha(str[i]) && !isalpha(str[i - 1])) { // saw isalpha() on cplusplus.com
                TString[i] = toupper(str[i]);
            }
        }

        return TString;


    }

    std::string LStrip(const std::string &str) {
        // Your code goes here
        // delete the left space
        auto LString = str;
        int strLength = str.size();
        int count = 0;

        while (isspace(str[count])) {
            count++;
        }

        int pos = 0;

        for (int i = count; i < strLength; i++) {
            LString[pos] = str[i];
            pos++;
        }
        LString.resize(strLength - count);


//        std::cout << "Lstring       " << LString << std::endl;
        return LString;

    }

    std::string RStrip(const std::string &str) {
        // Your code goes here
        //delete the right space
        auto RString = str;
        int strLength = str.size();
        int lastspot = strLength - 1;
        int count = 0;

        while (isspace(str[lastspot])) {
            lastspot--;
            count++;
        }

        for (int i = 0; i <= lastspot; i++) {
            RString[i] = str[i];
        }

        RString.resize(strLength - count);


        return RString;
    }

    std::string Strip(const std::string &str) {
        // if I can not use public
        auto WholeString = str;
        int Lcount = 0;
        int Rcount = 0;
        int strLength = str.size();
        int lastspot = strLength - 1;

        // Let's do left first

        while (isspace(str[Lcount])) {// count how many spaces on left
            Lcount++;
        }

        while (isspace(str[lastspot])) {// count how many spaces on right
            lastspot--;
            Rcount++;
        }

        int last = strLength - Rcount;
        int pos = 0;

        WholeString.resize(strLength - Lcount - Rcount);


        for (int i = Lcount; i < last; i++) {
            WholeString[pos] = str[i];
            pos++;
        }

        return WholeString;


    }

    std::string Center(const std::string &str, int width, char fill) {
        // Your code goes here
        auto CString = str;
        int oldstrLength = str.size();
        CString.resize(width);

        int half = ((width - oldstrLength) / 2);// the mid point of the filling chars

        int restart = half + oldstrLength; // the spot position after I fill the first part of chars and the ori strings

        int pos = 0;

        for (int i = 0; i < width; i++) {
            if (i < half || i >= restart) {
                CString[i] = fill;

            } else if (half <= i < restart) {
                CString[i] = str[pos];
                pos++;
            }
        }

        return CString;


    }

    std::string LJust(const std::string &str, int width, char fill) {
        // Your code goes here
        // left just is put char on the right side
        // EX : ("Test String",19), "Test String        ");

        auto LJString = str;
        auto LcharLength = str.length();

        LJString.resize(width);

        for (int i = 0; i < width; i++) {
            if (i < LcharLength) {
                LJString[i] = str[i];
            } else {
                LJString[i] = fill;
            }
        }

        return LJString;

    }

    std::string RJust(const std::string &str, int width, char fill) {
        // Your code goes here
        // right just is put char on the left side
        // EX : ("Test String",19), "        Test String");

        auto RJString = str;
        int RcharLength = str.length();
        int count = 0;
        int startSpot = width - RcharLength;

        RJString.resize(width);

        for (int i = 0; i < width; i++) {
            if (i < startSpot) {
                RJString[i] = fill;
            } else {
                RJString[i] = str[count];
                count++;
            }
        }
//        std::cout << " This is RJString " << RJString << std::endl;
//        std::cout <<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        return RJString;

    }

    std::string
    Replace(const std::string &str, const std::string &old, const std::string &rep) {  // replace need rewrite
        // Your code goes here

        std::string REPString;
        int i = 0;
        int pos = 0;
        int count = 0;
        int ori;
        int strLen = str.length();
        int start = 0;
        int spot = 0;
        int begin = 0;
        int oldLen = old.length();

        while (i < str.length()) {
            if (str[i] == old[pos]) {
                ori = i;
                while (str[ori] == old[pos] && count < oldLen) {
                    count++;
                    ori++;
                    pos++;
                }

                if (count == oldLen) {
                    REPString += rep;
                    count = 0;
                    pos = 0;
                    i += (old.length() - 1);
                } else {
                    REPString += str[i];
                    count = 0;
                    pos = 0;
                }
            } else {
                REPString += str[i];
            }
            i++;

        }

        return REPString;
    }


    std::vector<std::string> Split(const std::string &str, const std::string &splt) {
        // Your code goes here

        std::vector<std::string> SPVect; // This will declare a empty vector of strings
        std::string word;
//        std::string chars;
//
//
        int pos = 0;
        int count = 0;
        int startIndex = 0;
        int endIndex = 0;
        if (splt == "") {
            while (pos < str.length()) {
                while (!isspace(str[pos]) && pos < str.length()) {
                    word += str[pos];
                    pos++;
                }
                if (word != " " && !word.empty()) {
                    SPVect.push_back(word);
                }

                word.clear();
                pos++;
            }

        } else {
            while ((endIndex = str.find(splt, startIndex)) < str.size()) {
                std::string val = str.substr(startIndex, endIndex - startIndex);
                SPVect.push_back(val);
                startIndex = endIndex + splt.size();

            }
            if (startIndex < str.size()) {
                std::string val = str.substr(startIndex);
                SPVect.push_back(val);
            }
        }


        return SPVect;
    }

    std::string Join(const std::string &str, const std::vector<std::string> &vect) {
        // Your code goes here
        std::string JString;

        for (auto i : vect) {
            if (i != vect.back()) {
                i += str;
            }
            JString += i;
        }

        return JString;

    }

    std::string ExpandTabs(const std::string &str, int tabsize) {
        // Your code goes here
        std::string EXString;
        std::vector<int> nums;
        int start = 0;
        int strLen = str.length();
        int count = 0;
        int spot = 0;
        int exceed = 0;

        if (tabsize == 0) {
            for (auto i : str) {
                if (!isspace(i)) {
                    EXString += i;
                }
            }
        } else {
            while (start < strLen) {
                if (!isspace(str[start])) {
                    count++;

                    if (count > tabsize) {
                        exceed++;
                    }

                    EXString += str[start];
                    start++;
                } else {
                    if (count == tabsize) {
                        for (int i = 0; i < tabsize; i++) {
                            EXString += " ";
                        }
                        count = 0;
                        start++;

                    } else if (count < tabsize) {
                        spot = tabsize - count;

                        for (int i = 0; i < spot; i++) {
                            EXString += " ";
                        }
                        count = 0;
                        start += 1;
                    } else if (count > tabsize) {
                        if (exceed < tabsize) {
                            spot = tabsize - exceed;
                        } else if (exceed > tabsize) {
                            spot = exceed % tabsize;
                        }

                        for (int i = 0; i < spot; i++) {
                            EXString += " ";
                        }
                        exceed = 0;
                        count = 0;
                        start++;
                    }
                }

            }
        }


        return EXString;
    }

    int EditDistance(const std::string &left, const std::string &right, bool ignorecase)
    {
        // Your code goes here
        int leftLen = left.length();
        int rightLen = right.length();
        int count = 0;
        int pos = 0;
        int counting = 0;


        if (ignorecase)
        {
            return 0;
        }
        else if (leftLen == rightLen)
        {
            while (pos < leftLen)
            {
                if (left[pos] != right[pos])
                {
                    counting++;
                }
                pos++;
            }

            return counting;
        }
        else{
            typedef std::vector<std::vector<int> > Thematrix;
            //  I saw the matrix creating meathod online  https://stackoverflow.com/questions/48494266/stdvector-initializations-and-typedefs
            // And the rest of code most of them are based on these  links
            // https://blog.csdn.net/chichoxian/article/details/53944188
            //https://www.geeksforgeeks.org/print-common-characters-two-strings-alphabetical-order-2/
            //https://stackoverflow.com/questions/3371090/c-vector-based-two-dimensional-array-of-objects
            //https://en.wikipedia.org/wiki/Levenshtein_distance

            Thematrix matrix(leftLen + 1);
            for (int i = 0; i <= leftLen; i++) matrix[i].resize(rightLen + 1);


            for (int i = 1; i <= leftLen; i++) // do not put anything for spot 0
            {
                matrix[i][0] = i;
            }

            for (int j = 0; j <= rightLen; j++) {
                matrix[0][j] = j;
            }


            for (int i = 1; i < leftLen; i++) {
                matrix[i][0] = matrix[i - 1][0] + 1;
            }

            for (int j = 1; j < rightLen; j++) {
                matrix[0][j] = matrix[0][j - 1] + 1;
            }


            for (int i = 1; i < leftLen + 1; i++)
            {
                for (int j = 1; j < rightLen + 1; j++)
                {
                    if (left[i - 1] == right[j - 1])
                    {
                        count = 0;
                    }
                    else{
                        count = 1;
                    }

                    const int above = matrix[i - 1][j] + 1;
                    const int left = matrix[i][j - 1] + 1;
                    const int diag = matrix[i - 1][j - 1] + count;

                    matrix[i][j] = std::min(above, std::min(left, diag));

//                matrix[i][j] = std::min(matrix[i - 1][j] + 1,matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + count);
//                if ((matrix[i - 1][j] + 1 < matrix[i][j - 1] + 1) && (matrix[i - 1][j] < matrix[i - 1][j - 1] + count)) {
//                    matrix[i][j] = matrix[i - 1][j] + 1;
//                } else if ((matrix[i][j - 1] + 1 < matrix[i - 1][j] + 1) && (matrix[i][j - 1] + 1 < matrix[i - 1][j - 1] + count)) {
//                    matrix[i][j] = matrix[i][j - 1] + 1;
//                } else if((matrix[i - 1][j - 1] + count < matrix[i][j - 1] + 1)&& (matrix[i - 1][j - 1] + count < matrix[i - 1][j] + 1)){
//                    matrix[i][j] = matrix[i - 1][j - 1] + count;
//                }
                }

            }
            return matrix[leftLen][rightLen];
        }
    }
}

//    while(i < leftLen)
//{
//    while(!isspace(left[i]) && i < leftLen)
//{
//    Lword += left[i];
//    i++;
//}
//
//if(!Lword.empty())
//{
//LWORD.push_back(Lword);
//std::cout << " left word  " << Lword << std::endl;
//}
//Lword.clear();
//i++;
//}
//
//
//while(j < rightLen)
//{
//while(!isspace(right[j]) && j < rightLen)
//{
//Rword += right[j];
//j++;
//}
//
//if(!Rword.empty())
//{
//RWORD.push_back(Rword);
//std::cout << " right word  " << Rword << std::endl;
//}
//Rword.clear();
//j++;
//}
//
//int begin = 0;
//int total = 0;
//
//
//for(auto i : LWORD)
//{
//for(auto j :RWORD)
//{
//if(i.compare(j))
//{}
//else
//{
//if(i.length() == j.length())
//{
//while(begin < i.length())
//{
//if(i[begin] != j[begin])
//{
//total++;
//}
//
//begin++;
//}
//
//EDSint = total;
//}
//else if(std::max(i, j) == i)
//{
//
//
//}
//}
//}
//}

