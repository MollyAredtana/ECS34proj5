#include "Path.h"
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include "StringUtils.h"
#include <iostream>

struct CPath::SImplementation{
    // Your code goes here
    std::string DPath;
};

CPath::CPath() : DImplementation(std::make_unique< SImplementation >()){
    // Your code goes here


}

CPath::CPath(const CPath &path) : DImplementation(std::make_unique< SImplementation >()){
    // Your code goes here

    DImplementation->DPath = path.DImplementation->DPath;

    std::string answer;
    answer = DImplementation->DPath;
}

CPath::CPath(const std::string &path): DImplementation(std::make_unique< SImplementation >()) {
    // Your code goes here

    DImplementation->DPath = path;
}

CPath::~CPath(){
    // Your code goes here
//    ~DPath;


}

CPath& CPath::operator=(const CPath &path){
    // Your code goes here
    this->DImplementation->DPath = path.DImplementation->DPath;
    return *this;
}

CPath CPath::operator+(const CPath &path) const{
    // Your code goes here
    this->DImplementation->DPath + path.DImplementation->DPath;
    std::string Tstrings;
    Tstrings = DImplementation->DPath + '/' + path.DImplementation->DPath;

    return Tstrings;
}

bool CPath::operator==(const CPath &path) const{
    // Your code goes here
    std::string ToF;
    std::string FoT;
    ToF = DImplementation->DPath;
    FoT = path.DImplementation->DPath;


    if(ToF == FoT)
    {
        return true;
    }
    else{

        return false;
    }
}

bool CPath::operator!=(const CPath &path) const {
    // Your code goes here
    if(DImplementation->DPath != path.DImplementation->DPath)
    {
        return false;
    }
    else{
        return true;
    }
}

CPath CPath::Directory() const{
    // Your code goes here
    std::string obj;
    std::string result;
    std::size_t found;
    int pos = 0;
    int spot = 0;

    obj = DImplementation->DPath;

    found = obj.find('.cpp');

    if(obj.empty())
    {
        return obj;
    }
    else{
        if(found != -1)
        {
            while(obj[pos] != '/')
            {
                result += obj[pos];
                pos++;
            }

            pos = 0; // reset to 0 so I can reuse

        }
        else{
            for (int i = 0; i < obj.length() / 2; i++)
            {
                std::swap(obj[i], obj[obj.length() - i - 1]);
            }

            found = obj.find('/');
            pos = found + 1;

            while(pos < obj.size())
            {
                result += obj[pos];
                pos++;
            }

            for (int i = 0; i < result.length() / 2; i++)
            {
                std::swap(result[i], result[result.length() - i - 1]);
            }

        }
    }
    return result;



}

std::string CPath::BaseName() const{
    // Your code goes here
    std::string obj;
    std::string result;
    int pos = 0;

    obj = DImplementation->DPath;

    for (int i = 0; i < obj.length() / 2; i++)
    {
        std::swap(obj[i], obj[obj.length() - i - 1]);
    }

    if(obj.empty())
    {
        return obj;
    }
    else{
        while(obj[pos] != '/')
        {
            result += obj[pos];
            pos++;
        }

        for (int i = 0; i < result.length() / 2; i++)
        {
            std::swap(result[i], result[result.length() - i - 1]);
        }

    }

    return result;
}

std::string CPath::Extension() const{
    // Your code goes here
    std::string obj;
    std::string result;
    std::size_t found;
    std::string empty;
    int pos = 0;

    obj = DImplementation->DPath;

    for (int i = 0; i < obj.length() / 2; i++)
    {
        std::swap(obj[i], obj[obj.length() - i - 1]);
    }

    if(obj.empty())
    {
        return obj;
    }
    else {
        found = obj.find('.');

        if (found != -1) {
            while (obj[pos] != '.')
            {
                result += obj[pos];
                pos++;
            }

            if(pos == found)
            {
                result += obj[pos];

                for (int i = 0; i < result.length() / 2; i++)
                {
                    std::swap(result[i], result[result.length() - i - 1]);
                }
            }
            else{
                return empty;
            }
        }
        else
        {
            return empty;
        }
    }

    return result;
}

bool CPath::IsAbsolute() const{
    // Your code goes here
    if(DImplementation->DPath[0] == '/')
    {
        return true;
    }
    return false;
}

std::string CPath::ToString() const{
    // Your code goes here
    std::string TS;
    TS = DImplementation->DPath;
    return TS;
}

CPath::operator std::string() const {
    // Your code goes here

    std::string OP;
    OP = DImplementation->DPath;




    return OP;
}

CPath CPath::AbsolutePath() const{ // should absolute path be the whole path so I can just return the whole thing
    // Your code goes here
    CPath AP;
    std::string APS;
    std::string result;

    AP = DImplementation->DPath;
    APS = DImplementation->DPath;

    if(AP.IsAbsolute())
    {
        return APS;    // swap
    }
    else{
        result += CurrentPath();
        result += APS;// swap
    }




    // call current path
    // append current path to the absolute path which is the AP
    // and then normalize all things
    return NormalizePath(result);



}

CPath CPath::CommonPath(const CPath &path) const{
    // Your code goes here
    std::string CP;
    std::string DI;
    std::string Bname;
    std::string EX;

    DI = path.Directory();
    Bname = path.BaseName();
    EX = path.Extension();

    if(DI.length() > Bname.length() && DI.length() > EX.length())
    {
        CP = DI;
    }
    else if(Bname.length() > DI.length() && Bname.length() > EX.length())
    {
        CP = Bname;
    }
    else{
        CP = EX;
    }



    return CP;

}

CPath CPath::NormalizePath() const{ // line first normalize
    // Your code goes here
    std::string NP;
    CPath NPC;
    std::string answer;
    std::size_t found1;
    std::size_t found2;
    std::string result;

    int pos = 0;
    std::size_t spot1;
    std::size_t spot2;

    bool choice1;
    bool choice2;

    NPC = DImplementation->DPath;
    NP = DImplementation->DPath;

//    answer = NPC.Directory();
//    std::cout << "   ANSW#4 " << answer << std::endl;

    found1 = NP.find('.');
//    found2 = NP.find("..");

    spot1 = found1 - 1;
    spot2 = found2 - 1;

    if(NP[spot1] == '/')
    {
        choice1 = true;
    }

//    if(NP[spot2] == '/')
//    {
//        choice2 = true;
//    }


    if(found1 != -1)
    {
        if(choice1)
        {
            while(pos < NP.length())
            {
                if(pos < found1)
                {
                    result += NP[pos];
                    pos++;
                }
                else if(pos == found1)
                {
                    pos += 2;
                }
                else{
                    result += NP[pos];
                    pos++;
                }
            }

            pos = 0;  // so I can reuse pos

            return result;
        }
        else{
            answer += '.';
            answer += '.';
            answer += NPC.BaseName() + NPC.Extension();
//            answer = NPC.GetHome() +  NP;
//            std::cout << "  home  " << NPC.GetHome() << std::endl;
            return answer;
        }

    }
//    else if(found2 != -1)
//    {
//        std::cout << "  hihi  "<< std::endl;
//        // just testing
//        // how to do with this part
//        if(choice2)
//        {
//            while(pos < NP.length())
//            {
//                if( pos < found2)
//                {
//                    result += NP[pos];
//                    pos++;
//                }
//                else if(pos == found2)
//                {
//                    pos += 3;
//                }
//                else{
//                    result +=NP[pos];
//                    pos++;
//                }
//            }
//
//            std::size_t found2slash_sec;
//            std::size_t foundBin;
//
//            found2slash_sec = result.find('//');
//            foundBin = result.find('bin');
//
//            if(found2slash_sec != -1)
//            {
////                std::cout << " here " << std::endl;
//                std::string newResult;
//                int start = 0;
//                while(start < result.length())
//                {
//                    if(start < found2slash_sec)
//                    {
//                        newResult += result[start];
//                        start++;
//                    }
//                    else if(start == found2slash_sec)
//                    {
//                        newResult += '/';
//                        start += 2;
//                    }
//                    else{
//                        newResult += result[start];
//                        start++;
//                    }
//                }
//                return newResult;
//            }
//            else{
//                std::cout << " here " << std::endl;
//                return result;
//            }
//        }
//        else{
//            answer = NPC.GetHome() +  NP;
//            std::cout << "  home  " << NPC.GetHome() << std::endl;
//            return answer;
//        }


//    }
    else{
        return NP;
    }
//




//    std::string NP;
//    std::size_t found1;
//    std::size_t found2;
//    std::string result;
//
//    CPath NPC;
//    std::string answer;
//
//    int pos = 0;
//    std::size_t spot1;
//    std::size_t spot2;
//
//    bool choice1;
//    bool choice2;
//
//    NPC = DImplementation->DPath;
//    NP = DImplementation->DPath;
//
////    answer = NPC.Directory();
////    std::cout << "   ANSW#4 " << answer << std::endl;
//
//    found1 = NP.find('.');
//    found2 = NP.find('..');
//
//    spot1 = found1 - 1;
//    spot2 = found2 - 1;
//
//    if(NP[spot1] == '/')
//    {
//        choice1 = true;
//    }
//
//    if(NP[spot2] == '/')
//    {
//        choice2 = true;
//    }
//
//
//    if(found1 != -1)
//    {
//        if(choice1)
//        {
//            while(pos < NP.length())
//            {
//                if(pos < found1)
//                {
//                    result += NP[pos];
//                    pos++;
//                }
//                else if(pos == found1)
//                {
//                    pos += 2;
//                }
//                else{
//                    result += NP[pos];
//                    pos++;
//                }
//            }
//
//            pos = 0;  // so I can reuse pos
//
//            return result;
//        }
//        else{
//            return NP;
//        }
//
//    }
//    else if(found2 != -1)
//    {
//        // just testing
//        // how to do with this part
//        if(choice2)
//        {
//            while(pos < NP.length())
//            {
//                if( pos < found2)
//                {
//                    result += NP[pos];
//                    pos++;
//                }
//                else if(pos == found2)
//                {
//                    pos += 3;
//                }
//                else{
//                    result +=NP[pos];
//                    pos++;
//                }
//            }
//        }
//        else{
//            return NP;
//        }
//
//
//    }
//    else{
//        return NP;
//    }
////

}

CPath CPath::RelativePathTo(const CPath &path) const{ // first relative      what's the difference between relative path to and the relative path
    // Your code goes here
    CPath RPC;
    std::string RP;
    std::string result;
    std::size_t found;
    int pos = 0;

    RPC = path.DImplementation->DPath;
    RP = path.DImplementation->DPath;

//    return RPC.NormalizePath();

    if(RPC.IsAbsolute())
    {
        for (int i = 0; i < RP.length() / 2; i++)
        {
            std::swap(RP[i], RP[RP.length() - i - 1]);
        }

        found = RP.find('/');

        while(pos <= found)
        {
            result += RP[pos];
            pos++;
        }
        result += '.';

        result += '.';

        for (int i = 0; i < result.length() / 2; i++)
        {
            std::swap(result[i], result[result.length() - i - 1]);
        }

        return result;
    }
    else{
        return RPC.NormalizePath();
    }




//    std::string RP;
//    std::string result;
//    std::size_t found;
//    int pos = 0;
//
//    RP = path.DImplementation->DPath;
//
//    for (int i = 0; i < RP.length() / 2; i++)
//    {
//        std::swap(RP[i], RP[RP.length() - i - 1]);
//    }
//
//    found = RP.find('/');
//
//    while(pos <= found)
//    {
//        result += RP[pos];
//        pos++;
//    }
//
//    result += '.';
//
//    result += '.';
//
//    for (int i = 0; i < result.length() / 2; i++)
//    {
//        std::swap(result[i], result[result.length() - i - 1]);
//    }
//
//    return result;
}

// Helper function provided to get the current working directory
std::string CPath::GetCWD(){
    std::vector< char > CurrentPathName;

    CurrentPathName.resize(1024);
    while(NULL == getcwd(CurrentPathName.data(), CurrentPathName.size())){
        CurrentPathName.resize(CurrentPathName.size() * 2);
    }
    return CurrentPathName.data();
}

// Helper funciton provided to get the home directory
std::string CPath::GetHome(){
    return std::getenv("HOME");
}

CPath CPath::AbsolutePath(const CPath &path){
    // Your code goes here
    CPath AP;

    AP = path.DImplementation->DPath;

    return AP.AbsolutePath();


}

CPath CPath::CurrentPath(){
    // Your code goes here
    return GetCWD();
}

CPath CPath::CommonPath(const CPath &path1, const CPath &path2){
    // Your code goes here
    std::string CP;
    std::string Di;
    std::string Bname;
    std::string EX;

    if(path1.Directory() == path2.Directory())
    {
        Di = path1.Directory();
    }

    if(path1.BaseName() == path2.BaseName())
    {
        Bname = path1.BaseName();
    }

    if(path1.Extension() == path2.Extension())
    {
        EX = path1.Extension();
    }

    CP = Bname;

    return CP;


}

CPath CPath::ExpandUserPath(const CPath &path){
    // Your code goes here
    std::string EP;
    std::string result;
    CPath EPC;
    int pos = 0;

    EP = path.DImplementation->DPath;

    while(pos < EP.length())
    {
        if(EP[pos] == '~' && pos == 0)
        {
            result += path.GetHome();
            pos++;
        }
        else{
            return EP;
        }

        while(pos < EP.length())
        {
            result += EP[pos];
            pos++;
        }
    }
    return result;
}

CPath CPath::HomePath(){
    // Your code goes here
    CPath P;
    std::string HP;
    HP = P.GetHome();

    return HP;
}

CPath CPath::NormalizePath(const CPath &path){ // 513 second normalize
    // Your code goes here
//    CPath NP;
//
//    NP = path.DImplementation->DPath;

    return path.NormalizePath();

}

CPath CPath::RelativePath(const CPath &path, const CPath &startpath){ // second relative
    // Your code goes here
    return startpath.RelativePathTo(path);
}















//#include "Path.h"
//#include <cstdlib>
//#include <unistd.h>
//#include <vector>
//#include "StringUtils.h"
//#include <iostream>
//
//struct CPath::SImplementation{
//    // Your code goes here
//    std::string DPath;
//};
//
//CPath::CPath() : DImplementation(std::make_unique< SImplementation >()){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//
//
//}
//
//CPath::CPath(const CPath &path) : DImplementation(std::make_unique< SImplementation >()){
//    // Your code goes here
//
//    DImplementation->DPath = path.DImplementation->DPath;
//
//    std::string answer;
//    answer = DImplementation->DPath;
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    std::cout << "  DPath  " << answer << std::endl;
//}
//
//CPath::CPath(const std::string &path): DImplementation(std::make_unique< SImplementation >()) {
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//
//    DImplementation->DPath = path;
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//}
//
//CPath::~CPath(){
//    // Your code goes here
////    ~DPath;
//
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//
//}
//
//CPath& CPath::operator=(const CPath &path){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    this->DImplementation->DPath = path.DImplementation->DPath;
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    return *this;
//}
//
//CPath CPath::operator+(const CPath &path) const{
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " <<  "operator+" << std::endl;
//    this->DImplementation->DPath + path.DImplementation->DPath;
//    std::string Tstrings;
//    Tstrings = DImplementation->DPath + '/' + path.DImplementation->DPath;
//    std::cout << "    two strings plus  " << Tstrings << std::endl;
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    return Tstrings;
//}
//
//bool CPath::operator==(const CPath &path) const{
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " <<  "operator==" << std::endl;    std::string ToF;
//    std::string FoT;
//    ToF = DImplementation->DPath;
//    FoT = path.DImplementation->DPath;
//
//    std::cout << "   Tof  " << ToF << std::endl;
//    if(ToF == FoT)
//    {
//        std::cout << __LINE__ << "  @ line  " << std::endl;
//        std::cout << " true %%%%" << std::endl;
//        return true;
//    }
//    else{
//        std::cout << "   FoT  " << FoT << std::endl;
//        std::cout << "   false  &&&" << std::endl;
//        return false;
//    }
//}
//
//bool CPath::operator!=(const CPath &path) const {
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " <<  "operator!=" <<std::endl;
//    if(DImplementation->DPath != path.DImplementation->DPath)
//    {
//        std::cout << __LINE__ << "  @ line  " << std::endl;
//        return false;
//    }
//    else{
//        std::cout << __LINE__ << "  @ line  " << " operator!= end " <<std::endl;
//        return true;
//    }
//}
//
//CPath CPath::Directory() const{
//    // Your code goes here
//    std::string obj;
//    std::string result;
//    std::size_t found;
//    int pos = 0;
//    int spot = 0;
//
//    obj = DImplementation->DPath;
//
//    found = obj.find('.cpp');
//
//    if(obj.empty())
//    {
//        return obj;
//    }
//    else{
//        if(found != -1)
//        {
//            while(obj[pos] != '/')
//            {
//                result += obj[pos];
//                pos++;
//            }
//
//            pos = 0; // reset to 0 so I can reuse
//
//        }
//        else{
//            for (int i = 0; i < obj.length() / 2; i++)
//            {
//                std::swap(obj[i], obj[obj.length() - i - 1]);
//            }
//
//            found = obj.find('/');
//            pos = found + 1;
//
//            while(pos < obj.size())
//            {
//                result += obj[pos];
//                pos++;
//            }
//
//            for (int i = 0; i < result.length() / 2; i++)
//            {
//                std::swap(result[i], result[result.length() - i - 1]);
//            }
//
//        }
//    }
//    return result;
//
//
//
//}
//
//std::string CPath::BaseName() const{
//    // Your code goes here
//    std::string obj;
//    std::string result;
//    int pos = 0;
//
//    obj = DImplementation->DPath;
//
//    for (int i = 0; i < obj.length() / 2; i++)
//    {
//        std::swap(obj[i], obj[obj.length() - i - 1]);
//    }
//
//    if(obj.empty())
//    {
//        return obj;
//    }
//    else{
//        while(obj[pos] != '/')
//        {
//            result += obj[pos];
//            pos++;
//        }
//
//        for (int i = 0; i < result.length() / 2; i++)
//        {
//            std::swap(result[i], result[result.length() - i - 1]);
//        }
//
//    }
//
//    return result;
//}
//
//std::string CPath::Extension() const{
//    // Your code goes here
//    std::string obj;
//    std::string result;
//    std::size_t found;
//    std::string empty;
//    int pos = 0;
//
//    obj = DImplementation->DPath;
//
//    for (int i = 0; i < obj.length() / 2; i++)
//    {
//        std::swap(obj[i], obj[obj.length() - i - 1]);
//    }
//
//    if(obj.empty())
//    {
//        return obj;
//    }
//    else {
//        found = obj.find('.');
//
//        if (found != -1) {
//            while (obj[pos] != '.')
//            {
//                result += obj[pos];
//                pos++;
//            }
//
//            if(pos == found)
//            {
//                result += obj[pos];
//
//                for (int i = 0; i < result.length() / 2; i++)
//                {
//                    std::swap(result[i], result[result.length() - i - 1]);
//                }
//            }
//            else{
//                return empty;
//            }
//        }
//        else
//            {
//            return empty;
//        }
//    }
//
//    return result;
//}
//
//bool CPath::Is
// te() const{
//    // Your code goes here
//    if(DImplementation->DPath[0] == '/')
//    {
//        std::cout << __LINE__ << "  @ line  " << std::endl;
//        return true;
//    }
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    return false;
//}
//
//std::string CPath::ToString() const{
//    // Your code goes here
//    std::string TS;
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    TS = DImplementation->DPath;
//    std::cout << "   TS |||| " << TS;
//    return TS;
//}
//
//CPath::operator std::string() const {
//    // Your code goes here
//
//    std::string OP;
//    OP = DImplementation->DPath;
//
//    std::cout << __LINE__ << "  @ line  " << "  std::string  function end here  " << std::endl;
//
//
//
//    return OP;
//}
//
//CPath CPath::AbsolutePath() const{
//    // Your code goes here
//    std::string AP;
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//
//    AP = DImplementation->DPath;
//
//    // call current path
//    // append current path to the absolute path which is the AP
//    // and then normalize all things
//
//
//
//
//
//    std::cout << "    Absolute PAth  " << AP << std::endl;
//    return AP;
//}
//
//CPath CPath::CommonPath(const CPath &path) const{
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    std::string CP;
//    std::string DI;
//    std::string Bname;
//    std::string EX;
//
//    DI = path.Directory();
//    Bname = path.BaseName();
//    EX = path.Extension();
//
//    if(DI.length() > Bname.length() && DI.length() > EX.length())
//    {
//        CP = DI;
//    }
//    else if(Bname.length() > DI.length() && Bname.length() > EX.length())
//    {
//        CP = Bname;
//    }
//    else{
//        CP = EX;
//    }
//
//    std::cout << "  former cp  line 296 " << CP << std::endl;
//
//
//
//    return CP;
//
//}
//
//CPath CPath::NormalizePath() const{
//    // Your code goes here
//    std::string NP;
//    std::size_t found1;
//    std::size_t found2;
//    std::string result;
//
//    CPath NPC;
//    std::string answer;
//
//    int pos = 0;
//
//    NPC = DImplementation->DPath;
//    NP = DImplementation->DPath;
//
////    answer = NPC.Directory();
////    std::cout << "   ANSW#4 " << answer << std::endl;
//
//    found1 = NP.find('.');
//    found2 = NP.find('..');
//
//    if(found1 != -1)
//    {
//        while(pos < NP.length())
//        {
//            if(pos < found1)
//            {
//                result += NP[pos];
//                pos++;
//            }
//            else if(pos == found1)
//            {
//                pos += 2;
//            }
//            else{
//                result += NP[pos];
//                pos++;
//            }
//        }
//
//        pos = 0;  // so I can reuse pos
//
//        return result;
//    }
//    else if(found2 != -1)
//    {
//        // just testing
//        // how to do with this part
////        std::cout << "  Just testing  " << std::endl;
////        answer = NPC.Directory() + NPC.CurrentPath() + NPC.Extension();
////        std::cout << "  !!!!!!!!!! 这里是 ANSwer " << answer << std::endl;
//
//    }
//    else{
//        return NP;
//    }
////
//
//}
//
//CPath CPath::RelativePathTo(const CPath &path) const{
//    // Your code goes here
//    std::string RP;
//    std::string result;
//    std::size_t found;
//    int pos = 0;
//
//    RP = path.DImplementation->DPath;
//
//    for (int i = 0; i < RP.length() / 2; i++)
//    {
//        std::swap(RP[i], RP[RP.length() - i - 1]);
//    }
//
//    found = RP.find('/');
//
//    while(pos <= found)
//    {
//        result += RP[pos];
//        pos++;
//    }
//
//    result += '.';
//
//    result += '.';
//
//    for (int i = 0; i < result.length() / 2; i++)
//    {
//        std::swap(result[i], result[result.length() - i - 1]);
//    }
//
//    return result;
//}
//
//// Helper function provided to get the current working directory
//std::string CPath::GetCWD(){
//    std::vector< char > CurrentPathName;
//
//    CurrentPathName.resize(1024);
//    while(NULL == getcwd(CurrentPathName.data(), CurrentPathName.size())){
//        CurrentPathName.resize(CurrentPathName.size() * 2);
//    }
//    return CurrentPathName.data();
//}
//
//// Helper funciton provided to get the home directory
//std::string CPath::GetHome(){
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    return std::getenv("HOME");
//}
//
//CPath CPath::AbsolutePath(const CPath &path){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    CPath ABP;
//    return ABP.absolute;
//
//
//}
//
//CPath CPath::CurrentPath(){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    return GetCWD();
//}
//
//CPath CPath::CommonPath(const CPath &path1, const CPath &path2){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    std::string CP;
//    std::string Di;
//    std::string Bname;
//    std::string EX;
//
//    if(path1.Directory() == path2.Directory())
//    {
//        Di = path1.Directory();
//    }
//
//    if(path1.BaseName() == path2.BaseName())
//    {
//        Bname = path1.BaseName();
//    }
//
//    if(path1.Extension() == path2.Extension())
//    {
//        EX = path1.Extension();
//    }
//
//    CP = Bname;
//
//    std::cout << CP << "  cp  " << std::endl;
//
//    return CP;
//
//
//}
//
//CPath CPath::ExpandUserPath(const CPath &path){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    std::string EP;
//    std::string result;
//    CPath EPC;
//    int pos = 0;
//
//    EP = path.DImplementation->DPath;
//
//    while(pos < EP.length())
//    {
//        if(EP[pos] == '~')
//        {
//            result += path.GetHome();
//            pos++;
//        }
//        else{
//            result += EP[pos];
//            pos++;
//        }
//    }
//    return EP;
//}
//
//CPath CPath::HomePath(){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    CPath P;
//    std::string HP;
//    HP = P.GetHome();
//
//    return HP;
//}
//
//CPath CPath::NormalizePath(const CPath &path){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << "   call  here  " << std::endl;
//    std::string NP;
//
//    NP = path.DImplementation->DPath;
//    return NP;
//
//}
//
//CPath CPath::RelativePath(const CPath &path, const CPath &startpath){
//    // Your code goes here
//    std::cout << __LINE__ << "  @ line  " << std::endl;
//    std::string RP;
//
//    std::cout << __LINE__ << "  @ Line  " << "  @@@@@RP  " << RP << std::endl;
//
//    return RP;
//}
//
