#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include "MapRouter.h"
#include "StringUtils.h"
#include "CSVReader.h"
#include <CSVWriter.h>
#include "XMLReader.h"
#include "Path.h"
#include "stdio.h"
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

CMapRouter Map;
bool NotValid = false;
bool first_load = false;
bool UP = false;
bool DOWN = false;
CMapRouter::TNodeID SRC;
CMapRouter::TNodeID DES;
std::string TIME;
std::string MILE;
std::stringstream content;
std::vector <std::string> all_Commands;
std::vector <std::string> all_Commands1;
std::vector <CMapRouter::TPathStep> Path;
std::vector <std::string> desc;
std::string input;
std::string input1;
std::string in;
std::vector <std::string> First_commands;
std::string LOADING(std::vector <std::string> First_commands);
std::string help();
std::string INPUT();
std::string PUTIN(void);
std::string NODE(std::vector <std::string> holder);
std::string fastestPATH(std::vector <std::string> holder);
std::string shortestPATH(std::vector <std::string> holder);
std::string SAVE();
std::string PRINT();
std::string Records(std::list <std::string> command); // this is for extra credit
std::string helper(std::vector <std::string> container);

std::string choice1 = "help";
std::string choice2 = "fastest";
std::string choice3 = "shortest";
std::string choice4 = "exit";
std::string choice5 = "count";
std::string choice6 = "print";
std::string choice7 = "save";
std::string choice8 = "node";

// Q 1 how to do node[0, count) solved
// Q 2 Unknown command ""ode" type help for help.
// Q 3 SAVE : what file should we save in, he mentioned the kml file, but what is it
//            how to use it?        ^^^^bug here fix it
// Q 4 PRINT : should we print it in a stringstream ? 
// Q 5 Extra Credit : 1. isn't the up arrow and down arrow are the built-in stuff?
// Q 6 --data=proj5data


std::string DataPath;
int main(int argc, char *argv[])
{
    // std::cout << "  hereee " << std::endl;
    // use kml program to load everything in the kml files
    // std::cin>>in;
    // auto in = *argv[0];
    // std::string inn;
    // inn += in;
    for(int i = 0; i < argc; i++)
    {
        auto command = StringUtils::Split(argv[i], " ");
        auto splt = StringUtils::Split(argv[i], "=");
        if(splt.front() == "--data")
        {
            DataPath = splt.back();
        }
        // std::cout << argv[i] << std::endl;

    }
    // First_commands = StringUtils::Split(inn, " ");
    // First_commands.push_back(inn);
    First_commands.push_back(DataPath);
    LOADING(First_commands);

    // auto input = INPUT();
    // std::cout << input << std::endl;
    auto arrow = "> ";
    std::string input;
    std::cout << arrow;
    std::getline(std::cin, input);

    std::vector <std::string> holder;
    holder = StringUtils::Split(input, " ");
    // std::cout << "  hodler  size  isss " << holder.size() << std::endl;
    for(auto i : holder)
    {
        all_Commands.push_back(i);
    }
    if(holder.size() > 1)
    {
        holder.erase( std::unique(holder.begin(), holder.end() ), holder.end());
    }

    while(input != "exit")
    {
        // if(holder.size() == 1)
        // {
        // std::cout << "  holder front is  " <<  holder.front() << std::endl;
        if(holder.front() != choice1 && holder.front() != choice4 && holder.front() != choice5 && holder.front() != choice6 && holder.front() != choice7 && holder.front() != choice2 && holder.front() != choice3 && holder.front() != choice8)
        {
            std::cout << "Unknown command " << "\"" << holder.front() << "\"" << " type help for help." << std::endl;           
        }
        if(holder.front() == "help" && holder.size() == 1)
        {
            help();
            // input.clear();
        }
        else if(holder.front() == "count" && holder.size() == 1)
        {
            std::cout << Map.NodeCount() << " nodes" << std::endl;

        }
        else if(holder.front() == "save" && holder.size() == 1)
        {
            // save to a file
            auto string = SAVE();
            std::cout << string << std::endl;
        }
        else if(holder.front() == "print" && holder.size() == 1)
        {
            // print out the file, probally want to use csv writer and reader
            auto string = PRINT();
            std::cout << string << std::endl;
        }
        // }
        // else
        // {
        if(holder.front() == "node") // node [0, count)
        {
            CMapRouter::TNodeID nodeID;
            if(holder.size() >= 2)
            {
                // std::cout << "  in here  " << std::endl;
                holder.erase(holder.begin());
                auto string = NODE(holder);
                std::cout << string << std::endl;
            }
            else if(holder.size() == 1)
            {
                std::cout << "Invalid node command, see help." << std::endl;
            }
            
        }

        if(holder.front() == "fastest")
        {
            if(holder.size() > 2)
            {
                holder.erase(holder.begin());
                auto string = fastestPATH(holder);
                std::cout << string << std::endl;
            }
            else
            {
                std::cout << "Invalid fastest command, see help." << std::endl;
            }
        }

        if(holder.front() == "shortest")
        {
            if(holder.size() > 2)
            {
                holder.erase(holder.begin());
                // std::cout << "sizeeeee is   " << holder.size() << std::endl;
                auto string = shortestPATH(holder);
                std::cout << string << std::endl;
            }
            else
            {
                std::cout << "Invalid shortest command, see help." << std::endl;
            }
            
        }

        auto arrow = "> ";
        std::string input;
        std::cout << arrow;
        std::getline(std::cin, input);
        holder = StringUtils::Split(input, " ");
        for(auto i : holder)
        {
            all_Commands.push_back(i);
        }
        if(holder.size() > 1)
        {
            holder.erase( std::unique(holder.begin(), holder.end() ), holder.end());
        }

    }

    // input = INPUT();


}


// std::string INPUT()
// {
//     input.clear();
//     char in;

//     auto arrow = "> ";
//     std::string input;
//     std::cout << arrow;
//     std::cin >> input;
    
    // while(std::cin >> in)
    // while(std::cin >> in && in != '^M') // extra credit here
    // { // keep inputting char and char is not equal to ENTER key
    // //     // std::cout << in;
    //     input += in;
    //     std::cout << input << std::endl;
    //     if(in == '^[OM')
    //     {
    //         break;
    //     }
        // if(input == "^[A")
        // {
        //     UP = true;
        //     auto output = helper(all_Commands);
        //     std::cout << output << std::endl;
        // }
        // else if(input == "^[B")
        // {
        //     DOWN = true;
        //     auto output = helper(all_Commands);
        //     std::cout << output << std::endl;
        // }
    // }
    // std::cout << "  input is " << input << std::endl;
    // if(!UP || !DOWN)
    // {
    //     all_Commands.push_back(input);// it will record all the commands here
    // }
    // Records(all_Commands);
    // helper(all_Commands);

//     return input;
    
// }


std::string LOADING(std::vector <std::string> First_commands)
{
    // auto DataFile = StringUtils::Split(First_commands.back(), "=");
    auto DataFile = First_commands.front();
    if(!first_load)
    {
        std::string currentPath = CPath::CurrentPath();
        // std::cout << currentPath << "  current path  " << std::endl;
        // std::string DataPath = "/proj5data/";
        std::string STOP = currentPath + "/" + DataFile +  "/"  + "stops.csv";
        std::string ROUTE = currentPath +  "/" +  DataFile +  "/" + "routes.csv";
        std::cout << ROUTE << "  ROUTE path  " << std::endl;
        std::string DAVIS = currentPath + "/" + DataFile + "/" + "davis.osm";
        std::ifstream stops(STOP);
        std::ifstream routes(ROUTE);
        std::ifstream osm(DAVIS);
        Map.LoadMapAndRoutes(osm, stops, routes);
        // std::cout << "   finished loading  " << std::endl;
        first_load = true;
    }
}

std::string help()
{
    std::cout << "findroute [--data=path | --resutls=path]" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;;
    std::string MENU = "help     Display this help menu\nexit     Exit the program\ncount    Output the number of nodes in the map\nnode     Syntax \"node [0, count)\"\n        Will output node ID and Lat/Lon for node\nfastest  Syntax \"fastest start end\"\n         Calculates the time for fastest path from start to end\nshortest Syntax \"shortest start end\"\n         Calculates the distance for the shortest path from start to end\nsave     Saves the last calculated path to file\nprint    Prints the steps for the last calculated path";
    std::cout << MENU << std::endl;
}

std::string NODE(std::vector <std::string> holder) //  think about this one later
{
    // auto id = std::stoul(holder.front());
    // int index = Map.DNodeIDToNodeIndex[id];
    int index = std::stoi(holder.front());
    // std::cout << Map.NodeCount() << "  nodes  " << std::endl;
    if(index >= Map.NodeCount() || index < 0)
    {
        return "Invalid node parameter, see help.";
    }
    else
    {
        auto temp_id = Map.GetSortedNodeIDByIndex(index);
        auto lat = Map.LOC[temp_id].first;
        auto lon = Map.LOC[temp_id].second;
        return std::string("Node ")+ std::to_string(index) + std::string(": id = ") + std::to_string(temp_id) + std::string(" is at ") + std::to_string(lat) + std::string(", ") + std::to_string(lon);

        // 模版
        // Node 10258: id = 5754883282 is at 38.543037, -121.742295
    }
    
    
}


std::string fastestPATH(std::vector <std::string> holder)
{
    auto begin = std::stoul(holder[0]);
    auto back = std::stoul(holder[1]);
    SRC = begin;
    DES = back;
    // std::vector <CMapRouter::TPathStep> Path;
    // CMapRouter Map;

    if(Map.DNodeIDToNodeIndex[begin] == Map.DNodeIDToNodeIndex[back])
    {
        // didn't found it
        // invaid node
        NotValid = true;
        return "Unable to find fastest path from " + std::to_string(begin) + " to " + std::to_string(back);
    }
    if(Map.DNodeIDToNodeIndex.find(begin) == Map.DNodeIDToNodeIndex.find(back)) // 起点终点一致
    {
        return "Fastest path takes 0.0sec";
    }
    // may have to translate them to hours and minutes and secs
    auto time = Map.FindFastestPath(begin, back, Path);
    std::string tempTime;
    float whole = int(time);
    tempTime += std::to_string(whole) + "h";
    float decimal = time - whole;
    float mins = decimal * 60; // make it to mins;
    float whole_mins = int(mins);
    tempTime += std::to_string(whole_mins) + "mins";
    float secs = (mins - whole_mins)*60;
    tempTime += std::to_string(secs) + "secs";

    TIME = tempTime;

    // TIME = std::to_string(time);
    return "Fastest path takes " + tempTime + "sec";
}

std::string shortestPATH(std::vector <std::string> holder)
{
    auto begin = std::stoul(holder[0]);
    auto back = std::stoul(holder[1]);
    // std::cout << begin << "  begin and back " << back << std::endl;
    // std::cout << "   indexxx is   begin->> " << Map.DNodeIDToNodeIndex[begin] << "   back -->>" << Map.DNodeIDToNodeIndex[back] << std::endl;

    SRC = begin;
    DES = back;
    std::vector <CMapRouter::TNodeID> Path;
    // CMapRouter Map;

    // std::cout << "   start finding  " <<  std::endl;


    
    

    if(Map.DNodeIDToNodeIndex[begin] == Map.DNodeIDToNodeIndex[back])
    {
        // std::cout << "   not found  " << std::endl;
        // didn't found it
        // invaid node
        NotValid = true;
        return "Unable to find shortest path from " + std::to_string(begin) + " to " + std::to_string(back);
    }
    else if(begin == back) // 起点终点一致
    {
        // std::cout << "   00  " << std::endl;
        return "Shortest path takes 0.0mi";
    }
    else{
    // if(holder.size() == 1 || holder.size() == 0)
    // {
    //     return "Invalid shortest command, see help.";
    // }
        auto miles = Map.FindShortestPath(begin, back, Path);
        MILE = std::to_string(miles);
        // std::cout << "   end finding  " << std::endl;
        return "Shortest path is " + std::to_string(miles) + "mi";
    }

}

bool check_open;

std::string SAVE()
{
    std::string answer;
    // std::string path = CPath::CurrentPath();
    auto open = Map.GetPathDescription(Path, desc);
    check_open = open;
    if(!open)
    {
        return "No valid path to print.";
    }
    if(open)
    {
        std::cout << "   open but it should not be opened" << std::endl;
        // for(auto i : desc)
        // {
        //     content << i << std::endl;
        // }
        // Path saved to "./results/267547406_267547406_0.000000hr.csv".    模版
        // if(!TIME.empty())
        // {
            std::string fileName = std::to_string(SRC) + "_" + std::to_string(DES) + "_" + TIME + ".csv";
            // std::ofstream file(fileName);
            auto CP = CPath::CurrentPath();
            std::string results = "./results/";
            std::string path1= CP + results + fileName;
            // const char *path = "/xwhuang/ECS34/ECS34proj5/results/" + fileName.c_str();
            std::ofstream file1(path1);
            for(auto i : desc)
            {
                file1 << i << std::endl;
                content << i << std::endl;
            }
            answer += "Path saved to " + path1;
            return answer;
        // }
        // else if(!MILE.empty())
        // {
        //     std::string fileName = std::to_string(SRC) + "_" + std::to_string(DES) + "_" + MILE + ".csv";
        //     std::ofstream file(fileName);
        //     auto CP = CPath::CurrentPath();
        //     std::string results = "./results/";
        //     std::string path2= CP + results + fileName;
        //     std::ofstream file2(path2);
        //     answer += "Path saved to " + path2;
        //     return answer;
        // }
        // // std::ofstream file(fileName);

    }
    // else
    // {
    //     std::cout << "  not be able to save 也许需要改一下syntax在这里";
    // }
    
    
}

std::string PRINT()
{
    if(!check_open)
    {
        return "No valid path to print.";
    }
    CCSVWriter Writer(content);
    std::vector <std::string> row;
    while(Writer.WriteRow(row))
    {
        std::cout << row.front() << std::endl;
        // row.pop_back();
    }
}


std::string prev;
std::string next;
auto pos = all_Commands.size();
std::string helper(std::vector <std::string> container)
{
    
    auto CP = container;
    if(input == "^[A")
    {
        pos--;
        if(pos != -1)
        {
            prev = CP[pos];
            return prev;
        }

    }
    else if(input == "^[B")
    {
        pos++;
        if(pos < all_Commands.size())
        {
            next = CP[pos];
            return next;
        }
    }

}


// void helper(std::vector <std::string> container)
// {

std::string prev1;
std::string next1;
std::string inp;
auto pos1 = all_Commands1.size();
struct termios saved_attributes;

void reset_input_mode (void)
{
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode (void)
{
    struct termios tattr;
    char *name;

    /* Make sure stdin is a terminal. */
    if (!isatty (STDIN_FILENO))
    {
        fprintf (stderr, "Not a terminal.\n");
        exit (EXIT_FAILURE);
    }

    /* Save the terminal attributes so we can restore them later. */
    tcgetattr (STDIN_FILENO, &saved_attributes);
    atexit (reset_input_mode);

    /* Set the funny terminal modes. */
    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}


// put this part into a different output function
// and call this function inside of the other function
// int main(void)

std::string PUTIN(void)
{
    bool up;
    bool down;
    char c;

    set_input_mode();

    while (1) // might need to move this one up to main 
    {
        read (STDIN_FILENO, &c, 1);
        input1 += c;
        if (c == '\004')  
        {
            break;
        }        
        else if (c == '^[A') 
        {
            down = false;
            pos1--;
            if(pos1 != -1)
            {
                up = true;
                prev1 = all_Commands1[pos1];
                // return prev1;
                for(auto i : prev1)
                {
                    putchar(i);
                }
            }
            fflush(STDIN_FILENO);
        } 
        else if(c == '^[B')  
        {
            up = false;
            // go down
            pos1++;
            if(pos1 < all_Commands.size())
            {
                down = true;
                next1 = all_Commands1[pos1];
                // return next1;
                for(auto i : next1)
                {
                    putchar(i);
                }
            }
            fflush(STDIN_FILENO);
        }   
        else if(c == '^[OM')
        {
            // enter
            // keep doing things
            // return command;
            if(up)
            {
                return prev1;
            }
            else if(down)
            {
                return next1;
            }
        }
        else
        {
            putchar(c);
            if(c != '^[OM' || c != '^[A' ||  c != '^[B')
            {
                inp += c;
                // may need a global vector to keep track of everything here
            }
        }
    }
    all_Commands1.push_back(input1);

    // return inp;

    return EXIT_SUCCESS;
}
// }
// std::string Records(std::list <std::string> command)
// {
//     while(// hit upped arrow)
//     {
//         auto last_command =
//     }


// }
























// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <list>
// #include <algorithm>
// #include "MapRouter.h"
// #include "StringUtils.h"
// #include "CSVReader.h"
// #include <CSVWriter.h>
// #include "XMLReader.h"
// #include "Path.h"
// #include "stdio.h"
// #include <stdlib.h>
// #include <termios.h>
// #include <unistd.h>

// CMapRouter Map;
// bool NotValid = false;
// bool first_load = false;
// bool UP = false;
// bool DOWN = false;
// CMapRouter::TNodeID SRC;
// CMapRouter::TNodeID DES;
// std::string TIME;
// std::string MILE;
// std::stringstream content;
// std::vector <std::string> all_Commands;
// std::vector <std::string> all_Commands1;
// std::vector <CMapRouter::TPathStep> Path;
// std::vector <std::string> desc;
// std::string input;
// std::string input1;
// std::string in;
// std::vector <std::string> First_commands;
// std::string LOADING(std::vector <std::string> First_commands);
// std::string help();
// std::string INPUT();
// std::string PUTIN(void);
// std::string NODE(std::vector <std::string> holder);
// std::string fastestPATH(std::vector <std::string> holder);
// std::string shortestPATH(std::vector <std::string> holder);
// std::string SAVE();
// std::string PRINT();
// std::string Records(std::list <std::string> command); // this is for extra credit
// std::string helper(std::vector <std::string> container);

// std::string choice1 = "help";
// std::string choice2 = "fastest";
// std::string choice3 = "shortest";
// std::string choice4 = "exit";
// std::string choice5 = "count";
// std::string choice6 = "print";
// std::string choice7 = "save";
// std::string choice8 = "node";

// // Q 1 how to do node[0, count) solved
// // Q 2 Unknown command ""ode" type help for help.
// // Q 3 SAVE : what file should we save in, he mentioned the kml file, but what is it
// //            how to use it?        ^^^^bug here fix it
// // Q 4 PRINT : should we print it in a stringstream ? 
// // Q 5 Extra Credit : 1. isn't the up arrow and down arrow are the built-in stuff?
// // Q 6 --data=proj5data


// struct termios saved_attributes;
// void reset_input_mode (void)
// {
//     tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
// }

// void set_input_mode (void)
// {
//     struct termios tattr;
//     char *name;

//     /* Make sure stdin is a terminal. */
//     if (!isatty (STDIN_FILENO))
//     {
//         fprintf (stderr, "Not a terminal.\n");
//         exit (EXIT_FAILURE);
//     }

//     /* Save the terminal attributes so we can restore them later. */
//     tcgetattr (STDIN_FILENO, &saved_attributes);
//     atexit (reset_input_mode);

//     /* Set the funny terminal modes. */
//     tcgetattr (STDIN_FILENO, &tattr);
//     tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
//     tattr.c_cc[VMIN] = 1;
//     tattr.c_cc[VTIME] = 0;
//     tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
// }


// std::string DataPath;
// // std::vector <char> container;
// // std::vector <std::string> holder1;
// // std::vector <std::string> vec;
// // std::string new_input;
// int main(int argc, char *argv[])
// {
//     // std::cout << "  hereee " << std::endl;
//     // use kml program to load everything in the kml files
//     // std::cin>>in;
//     // auto in = *argv[0];
//     // std::string inn;
//     // inn += in;
//     for(int i = 0; i < argc; i++)
//     {
//         auto command = StringUtils::Split(argv[i], " ");
//         auto splt = StringUtils::Split(argv[i], "=");
//         if(splt.front() == "--data")
//         {
//             DataPath = splt.back();
//         }
//         // std::cout << argv[i] << std::endl;

//     }
//     // First_commands = StringUtils::Split(inn, " ");
//     // First_commands.push_back(inn);
//     First_commands.push_back(DataPath);
//     LOADING(First_commands);




//     // auto input = INPUT();
//     // std::cout << input << std::endl;
//     auto arrow = "> ";
//     std::string input;
//     // char inn[20];
//     std::cout << arrow;


//     // PUTIN();


//     std::getline(std::cin, input);

//     std::vector <std::string> holder;
    
//     holder = StringUtils::Split(input, " ");
//     for(auto i : holder)
//     {
//         all_Commands.push_back(i);
//     }
//     if(holder.size() > 1)
//     {
//         holder.erase( std::unique(holder.begin(), holder.end() ), holder.end());
//     }


//     // bool up;
//     // // bool down;
//     // char c;

//     // set_input_mode();

//     // while(1)
//     // {
//     //     read(STDIN_FILENO, &c, 1);
//     //     new_input += c;
//     //     holder1[0] = new_input;
//     //     if(c == '^[E')
//     //     {
//     //         break;
//     //     }
//     //     putchar(c);
//     // }


//     // while(1)
//     // while(input != "exit")
//     while(holder.front() != "exit")
//     {
//         // std::cout << "  in hereee " << std::endl;
//         // read(STDIN_FILENO, &c, 1);
//         // std::cout << c;
//         // putchar(c);
//         // new_input += c;
//         // holder1[0] = new_input;
//         // container.push_back(c);
//         // if(holder.size() == 1)
//         // {
//         // if(c != 'help' && c != 'count' && c != 'node' && c != 'fastest' && c != 'shortest' && c != 'save' && c != 'print' && c != 'exit')
//         // std::cout << "  holder front is  " <<  holder.front() << std::endl;
//         if(holder.front() != choice1 && holder.front() != choice4 && holder.front() != choice5 && holder.front() != choice6 && holder.front() != choice7 && holder.front() != choice2 && holder.front() != choice3 && holder.front() != choice8)
//         {
//             std::cout << "Unknown command " << "\"" << holder.front() << "\"" << " type help for help." << std::endl;           
//         }
//         // if(c == 'help')
//         if(holder.front() == "help" && holder.size() == 1)
//         {
//             help();
//             // input.clear();
//         }
//         // else if(c == 'count')
//         else if(holder.front() == "count" && holder.size() == 1)
//         {
//             std::cout << Map.NodeCount() << " nodes" << std::endl;

//         }
//         // else if(c == 'save')
//         else if(holder.front() == "save" && holder.size() == 1)
//         {
//             // save to a file
//             SAVE();
//         }
//         // else if(c == 'print')
//         else if(holder.front() == "print" && holder.size() == 1)
//         {
//             // print out the file, probally want to use csv writer and reader
//             PRINT();
//         }
//         // }
//         // else
//         // {
//         // if(c == 'node')
//         if(holder.front() == "node") // node [0, count)
//         {
//             // CMapRouter::TNodeID nodeID;
//             // if(container.size() >= 2)
//             if(holder.size() >= 2)
//             {
//                 // container.erase(container.begin());
//                 // std::vector <std::string> vec;
//                 // for(auto i : container)
//                 // {
//                 //     // std::to_string(i);
//                 //     vec.push_back(std::to_string(i));
//                 // }
//                 // auto string = NODE(vec);
//                 holder.erase(holder.begin());
//                 auto string = NODE(holder);
//                 std::cout << string << std::endl;
//             }
//             // else if(container.size()  == 1)
//             else if(holder.size() == 1)
//             {
//                 std::cout << "Invalid node command, see help." << std::endl;
//             }
            
//         }
//         // if(container.front() == 'fastest')
//         if(holder.front() == "fastest")
//         {
//             // if(container.size() == 3)
//             if(holder.size() == 3)
//             {
//                 // container.erase(container.begin());
//                 // for(auto i : container)
//                 // {
//                 //     // std::to_string(i);
//                 //     vec.push_back(std::to_string(i));
//                 // }
//                 // fastestPATH(vec);
//                 holder.erase(holder.begin());
//                 fastestPATH(holder);
//             }
//             else
//             {
//                 std::cout << "Invalid fastest command, see help." << std::endl;
//             }
//         }

//         // if(container.front() == 'shortest')
//         if(holder.front() == "shortest")
//         {
//             // if(container.size() > 2)
//             if(holder.size() > 2)
//             {
//                 // container.erase(container.begin());
//                 // for(auto i : container)
//                 // {
//                 //     // std::to_string(i);
//                 //     vec.push_back(std::to_string(i));
//                 // }
//                 // shortestPATH(vec);
//                 holder.erase(holder.begin());
//                 std::cout << "sizeeeee is   " << holder.size() << std::endl;
//                 shortestPATH(holder);
//             }
//             else
//             {
//                 std::cout << "Invalid shortest command, see help." << std::endl;
//             }
            
//         }

//         auto arrow = "> ";
//         // std::string input;
//         // input.clear();
//         // char inn[20];
//         std::cout << arrow;
//         // vec.clear();

//         // PUTIN();


//         std::getline(std::cin, input);
//         // std::cin.getline(inn, sizeof(inn));
//         // input += inn;
//         holder = StringUtils::Split(input, " ");
//         for(auto i : holder)
//         {
//             all_Commands.push_back(i);
//         }
//         if(holder.size() > 1)
//         {
//             holder.erase( std::unique(holder.begin(), holder.end() ), holder.end());
//         }

//     }

//     // input = INPUT();


// }


// // std::string INPUT()
// // {
// //     input.clear();
// //     char in;

// //     auto arrow = "> ";
// //     std::string input;
// //     std::cout << arrow;
// //     std::cin >> input;
    
//     // while(std::cin >> in)
//     // while(std::cin >> in && in != '^M') // extra credit here
//     // { // keep inputting char and char is not equal to ENTER key
//     // //     // std::cout << in;
//     //     input += in;
//     //     std::cout << input << std::endl;
//     //     if(in == '^[OM')
//     //     {
//     //         break;
//     //     }
//         // if(input == "^[A")
//         // {
//         //     UP = true;
//         //     auto output = helper(all_Commands);
//         //     std::cout << output << std::endl;
//         // }
//         // else if(input == "^[B")
//         // {
//         //     DOWN = true;
//         //     auto output = helper(all_Commands);
//         //     std::cout << output << std::endl;
//         // }
//     // }
//     // std::cout << "  input is " << input << std::endl;
//     // if(!UP || !DOWN)
//     // {
//     //     all_Commands.push_back(input);// it will record all the commands here
//     // }
//     // Records(all_Commands);
//     // helper(all_Commands);

// //     return input;
    
// // }


// std::string LOADING(std::vector <std::string> First_commands)
// {
//     // auto DataFile = StringUtils::Split(First_commands.back(), "=");
//     auto DataFile = First_commands.front();
//     if(!first_load)
//     {
//         std::string currentPath = CPath::CurrentPath();
//         // std::cout << currentPath << "  current path  " << std::endl;
//         // std::string DataPath = "/proj5data/";
//         std::string STOP = currentPath + "/" + DataFile +  "/"  + "stops.csv";
//         std::string ROUTE = currentPath +  "/" +  DataFile +  "/" + "routes.csv";
//         std::cout << ROUTE << "  ROUTE path  " << std::endl;
//         std::string DAVIS = currentPath + "/" + DataFile + "/" + "davis.osm";
//         std::ifstream stops(STOP);
//         std::ifstream routes(ROUTE);
//         std::ifstream osm(DAVIS);
//         Map.LoadMapAndRoutes(osm, stops, routes);
//         std::cout << "   finished loading  " << std::endl;
//         first_load = true;
//     }
// }

// std::string help()
// {
//     std::cout << "findroute [--data=path | --resutls=path]" << std::endl;
//     std::cout << "------------------------------------------------------------------------" << std::endl;;
//     std::string MENU = "help     Display this help menu\nexit     Exit the program\ncount    Output the number of nodes in the map\nnode     Syntax \"node [0, count)\"\n        Will output node ID and Lat/Lon for node\nfastest  Syntax \"fastest start end\"\n         Calculates the time for fastest path from start to end\nshortest Syntax \"shortest start end\"\n         Calculates the distance for the shortest path from start to end\nsave     Saves the last calculated path to file\nprint    Prints the steps for the last calculated path";
//     std::cout << MENU << std::endl;
//     // return MENU;
//     // return "help     Display this help menu\nexit     Exit the program\ncount    Output the number of nodes in the map\nnode     Syntax \"node [0, count)\"\n        Will output node ID and Lat/Lon for node\nfastest  Syntax \"fastest start end\"\n         Calculates the time for fastest path from start to end\nshortest Syntax \"shortest start end\"\n         Calculates the distance for the shortest path from start to end\nsave     Saves the last calculated path to file\nprint    Prints the steps for the last calculated path";
// }

// std::string NODE(std::vector <std::string> holder) //  think about this one later
// {
//     // auto id = std::stoul(holder.front());
//     int index = std::stoi(holder.front());
//     // std::cout << "  get sorteddd  " << Map.GetSortedNodeIDByIndex(10) << std::endl;
//     // std::cout << Map.NodeCount() << "  nodes  " << std::endl;
//     if(index >= Map.NodeCount() || index < 0)
//     {
//         return "Invalid node parameter, see help.";
//     }
//     else
//     {
//         // std::cout << "  is in here " << std::endl;
//         auto temp_id = Map.GetSortedNodeIDByIndex(index);
//         auto lat = Map.LOC[temp_id].first;
//         auto lon = Map.LOC[temp_id].second;
//         return std::string("Node ")+ std::to_string(index) + std::string(": id = ") + std::to_string(temp_id) + std::string(" is at ") + std::to_string(lat) + std::string(", ") + std::to_string(lon);


//         // Node 10258: id = 5754883282 is at 38.543037, -121.742295
//     }
    
    
// }


// std::string fastestPATH(std::vector <std::string> holder)
// {
//     auto begin = std::stoul(holder[0]);
//     auto back = std::stoul(holder[1]);
//     SRC = begin;
//     DES = back;
//     // std::vector <CMapRouter::TPathStep> Path;
//     // CMapRouter Map;
//     auto time = Map.FindFastestPath(begin, back, Path);

//     if(Map.DNodeIDToNodeIndex.find(begin) == Map.DNodeIDToNodeIndex.end() || Map.DNodeIDToNodeIndex.find(back) == Map.DNodeIDToNodeIndex.end())
//     {
//         // didn't found it
//         // invaid node
//         NotValid = true;
//         return "Unable to find fastest path from " + holder.front() + " to " + holder.back();
//     }
//     if(Map.DNodeIDToNodeIndex.find(begin) == Map.DNodeIDToNodeIndex.find(back)) // 起点终点一致
//     {
//         return "Fastest path takes 0.0sec";
//     }
//     // if(holder.size() == 1 || holder.size() == 0)
//     // {
//     //     return "Invalid fastest command, see help.";
//     // }
//     // may have to translate them to hours and minutes and secs
//     std::string tempTime;
//     float whole = int(time);
//     tempTime += std::to_string(whole) + "h";
//     float decimal = time - whole;
//     float mins = decimal * 60; // make it to mins;
//     float whole_mins = int(mins);
//     tempTime += std::to_string(whole_mins) + "mins";
//     float secs = (mins - whole_mins)*60;
//     tempTime += std::to_string(secs) + "secs";

//     // TIME = std::to_string(time);
//     return "Fastest path takes " + tempTime + "sec";
// }

// std::string shortestPATH(std::vector <std::string> holder)
// {
//     auto begin = std::stoul(holder[0]);
//     auto back = std::stoul(holder[1]);
//     std::cout << begin << "  begin and back " << back << std::endl;
//     std::cout << "   indexxx is   begin->> " << Map.DNodeIDToNodeIndex[begin] << "   back -->>" << Map.DNodeIDToNodeIndex[back] << std::endl;

//     SRC = begin;
//     DES = back;
//     std::vector <CMapRouter::TNodeID> Path;
//     // CMapRouter Map;
//     auto miles = Map.FindShortestPath(begin, back, Path);

//     std::cout << "   start finding  " << std::endl;

//     if(Map.DNodeIDToNodeIndex.find(begin) == Map.DNodeIDToNodeIndex.end() || Map.DNodeIDToNodeIndex.find(back) == Map.DNodeIDToNodeIndex.end())
//     {
//         std::cout << "   not found  " << std::endl;
//         // didn't found it
//         // invaid node
//         NotValid = true;
//         std::cout << "Unable to find shortest path from " + holder.front() + " to " + holder.back() << std::endl;
//     }
//     else if(Map.DNodeIDToNodeIndex.find(begin) == Map.DNodeIDToNodeIndex.find(back)) // 起点终点一致
//     {
//         std::cout << "   00  " << std::endl;
//         return "Shortest path takes 0.0mi";
//     }
//     // if(holder.size() == 1 || holder.size() == 0)
//     // {
//     //     return "Invalid shortest command, see help.";
//     // }
//     MILE = std::to_string(miles);
//     std::cout << "   end finding  " << std::endl;
//     return "Shortest path is " + std::to_string(miles) + "mi";

// }

// std::string SAVE()
// {
//     std::string answer;
//     // std::string path = CPath::CurrentPath();
//     auto open = Map.GetPathDescription(Path, desc);
//     if(NotValid)
//     {
//         return "No valid path to print.";
//     }
//     if(open)
//     {
//         std::cout << "   open but it should not be opened" << std::endl;
//         for(auto i : desc)
//         {
//             content << i << std::endl;
//         }
//         // Path saved to "./results/267547406_267547406_0.000000hr.csv".    模版
//         if(!TIME.empty())
//         {
//             std::string fileName = std::to_string(SRC) + "_" + std::to_string(DES) + "_" + TIME + ".csv";
//             // std::ofstream file(fileName);
//             auto CP = CPath::CurrentPath();
//             std::string results = "./results/";
//             std::string path1= CP + results + fileName;
//             // const char *path = "/xwhuang/ECS34/ECS34proj5/results/" + fileName.c_str();
//             std::ofstream file1(path1);
//             answer += "Path saved to " + path1;
//             return answer;
//         }
//         else if(!MILE.empty())
//         {
//             std::string fileName = std::to_string(SRC) + "_" + std::to_string(DES) + "_" + MILE + ".csv";
//             std::ofstream file(fileName);
//             auto CP = CPath::CurrentPath();
//             std::string results = "./results/";
//             std::string path2= CP + results + fileName;
//             std::ofstream file2(path2);
//             answer += "Path saved to " + path2;
//             return answer;
//         }
//         // std::ofstream file(fileName);

//     }
//     else
//     {
//         std::cout << "  not be able to save 也许需要改一下syntax在这里";
//     }
    
// }

// std::string PRINT()
// {
//     CCSVWriter Writer(content);
//     std::vector <std::string> row;
//     while(Writer.WriteRow(row))
//     {
//         std::cout << row.front() << std::endl;
//         row.pop_back();
//     }
// }


// std::string prev;
// std::string next;
// auto pos = all_Commands.size();
// std::string helper(std::vector <std::string> container)
// {
    
//     auto CP = container;
//     if(input == "^[A")
//     {
//         pos--;
//         if(pos != -1)
//         {
//             prev = CP[pos];
//             return prev;
//         }

//     }
//     else if(input == "^[B")
//     {
//         pos++;
//         if(pos < all_Commands.size())
//         {
//             next = CP[pos];
//             return next;
//         }
//     }

// }


// // void helper(std::vector <std::string> container)
// // {

// // std::string prev1;
// // std::string next1;
// // std::string inp;
// // auto pos1 = all_Commands1.size();
// // struct termios saved_attributes;

// // void reset_input_mode (void)
// // {
// //     tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
// // }

// // void set_input_mode (void)
// // {
// //     struct termios tattr;
// //     char *name;

// //     /* Make sure stdin is a terminal. */
// //     if (!isatty (STDIN_FILENO))
// //     {
// //         fprintf (stderr, "Not a terminal.\n");
// //         exit (EXIT_FAILURE);
// //     }

// //     /* Save the terminal attributes so we can restore them later. */
// //     tcgetattr (STDIN_FILENO, &saved_attributes);
// //     atexit (reset_input_mode);

// //     /* Set the funny terminal modes. */
// //     tcgetattr (STDIN_FILENO, &tattr);
// //     tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
// //     tattr.c_cc[VMIN] = 1;
// //     tattr.c_cc[VTIME] = 0;
// //     tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
// // }


// // // put this part into a different output function
// // // and call this function inside of the other function
// // // int main(void)

// // std::string PUTIN(void)
// // {
// //     bool up;
// //     bool down;
// //     char c;

// //     set_input_mode();

// //     while (c != '^[OM') // might need to move this one up to main 
// //     {
// //         read(STDIN_FILENO, &c, 1);
// //         input1 += c;
// //         if (c == '^[A') 
// //         {
// //             down = false;
// //             // how do you go up and down?
// //             // since you have to have one container saving all the things here, 
// //             // this one is only for putting chars
// //             // probally this one is for left and right
// //             // go up
// //             pos1--;
// //             if(pos1 != -1)
// //             {
// //                 up = true;
// //                 prev1 = all_Commands1[pos1];
// //                 // return prev1;
// //                 for(auto i : prev1)
// //                 {
// //                     putchar(i);
// //                 }
// //             }
// //             fflush(STDIN_FILENO);
// //         } 
// //         else if(c == '^[B')  
// //         {
// //             up = false;
// //             // go down
// //             pos1++;
// //             if(pos1 < all_Commands.size())
// //             {
// //                 down = true;
// //                 next1 = all_Commands1[pos1];
// //                 // return next1;
// //                 for(auto i : next1)
// //                 {
// //                     putchar(i);
// //                 }
// //             }
// //             fflush(STDIN_FILENO);
// //         }   
// //         else if(c == '^[OM')
// //         {
// //             // enter
// //             // keep doing things
// //             // return command;
// //             if(up)
// //             {
// //                 return prev1;
// //             }
// //             else if(down)
// //             {
// //                 return next1;
// //             }
// //             break;
// //         }
// //         else
// //         {
// //             putchar(c);
// //             if(c != '^[OM' || c != '^[A' ||  c != '^[B')
// //             {
// //                 inp += c;
// //                 // may need a global vector to keep track of everything here
// //             }
// //         }
// //     }
// //     all_Commands1.push_back(input1);

// //     // return inp;

// //     return EXIT_SUCCESS;
// // }
// // // }
// // // std::string Records(std::list <std::string> command)
// // // {
// // //     while(// hit upped arrow)
// // //     {
// // //         auto last_command =
// // //     }


// // // }