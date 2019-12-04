#ifndef MAPROUTER_H 	  			 	 
#define MAPROUTER_H
// there is one more line Q
#include <vector>
#include <istream>
#include <iostream>
#include "XMLReader.h"
#include <map>
#include <unordered_map>
#include <string>
#include <list>

class CMapRouter{
    public:
        using TNodeID = unsigned long;
        using TStopID = unsigned long;
        using TLocation = std::pair<double, double>;
        using TPathStep = std::pair<std::string, TNodeID>;
        
        static const TNodeID InvalidNodeID;
        
    private:
        using TNodeIndex = std::size_t;
        std::list <std::string> StopID;
        std::vector <TNodeID> BUSStopID;
        std::list <std::string> NodeID;
        std::vector <std::string> BusName;
        std::unordered_map <TNodeID ,TStopID> NodeToStop;
        std::unordered_map <TStopID ,TNodeID> StopToNode;
        std::unordered_map <std::string ,std::vector <TNodeID>> BUSLine;
        std::unordered_map <TStopID ,std::vector <std::string>> ReversedBUSLine;
        std::vector <TNodeID> Index;



        struct SEdge
        {
            TNodeIndex DOtherNodeIndex;
            // std::unordered_map <std:: < TNodeID, TNodeID>, std::vector<std::string>> ID_to_Name;
            double DDistance;
            double DTime = 0.0;
            double DMaxSpeed;
        };

        struct SNode
        {       
            TNodeID DNodeID;
            double DLatitude;
            double DLongitude;
            std::list <TStopID> BusStop;
            std::list <std::string> RouteName;
            bool takeBUS = false;
            std::vector <SEdge> DEdges; //Driving Edges
            std::vector <SEdge> DWEdges; //Walking Edges
            std::vector <SEdge> Driving_Edge;
        };
        // SNode TempNode;
        std::vector <SNode> NODES;
                
    public:
        std::unordered_map <TNodeID ,std::pair <double, double>> LOC;
        std::unordered_map <TNodeID, TNodeIndex> DNodeIdToDnodeIndex;
        CMapRouter();
        ~CMapRouter();
        
        static double HaversineDistance(double lat1, double lon1, double lat2, double lon2);
        static double CalculateBearing(double lat1, double lon1,double lat2, double lon2);
        
        bool LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes);
        size_t NodeCount() const;
        TNodeID GetSortedNodeIDByIndex(size_t index) const;
        TLocation GetSortedNodeLocationByIndex(size_t index) const;
        TLocation GetNodeLocationByID(TNodeID nodeid) const;
        TNodeID GetNodeIDByStopID(TStopID stopid) const;
        size_t RouteCount() const;
        std::string GetSortedRouteNameByIndex(size_t index) const;
        bool GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops);
        
        double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path);
        double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path);
        bool GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const;
};

#endif









// #ifndef MAPROUTER_H 	  			 	 
// #define MAPROUTER_H
// // there is one more line Q
// #include <vector>
// #include <istream>
// #include <iostream>
// #include "XMLReader.h"
// #include <map>
// #include <unordered_map>
// #include <string>
// #include <list>

// class CMapRouter{
//     public:
//         using TNodeID = unsigned long; // the node id in davis.osm
//         using TStopID = unsigned long; // stop id in both routes.csv and stops.csv
//         using TLocation = std::pair<double, double>;
//         using TPathStep = std::pair<std::string, TNodeID>;
        
//         static const TNodeID InvalidNodeID;
//     private:
//     // maybe use the map to save all the things overhere
//         std::vector <TNodeID> Vertices; // vertices are the node id
//         std::vector <std::string> Stop; // edges are the distances
//         std::vector <std::string> BUS; // kee[ track of the Bus lines 
//         std::list <std::string> StopID;
//         std::vector <TNodeID> BUSStopID;
//         std::list <std::string> NodeID;
//         std::vector <std::string> BusName;
//         std::unordered_map <TNodeID ,TStopID> NodeToStop;
//         // save the node_id and the stops_id
//         std::unordered_map <TStopID ,TNodeID> StopToNode;
//         // stops_id and the node_id
//         std::unordered_map <std::string ,std::vector <TNodeID>> BUSLine;
//         // save bus line A and the stop id
//         std::unordered_map <std::string ,std::vector <std::string>> ReversedBUSLine;
//         // reversed BUSLine
//         std::unordered_map <std::string ,std::vector <std::string>> Ways; // save edges
//         std::vector <std::string>  allLines;
//         // Q
//         // what will be the number type for stop id?
//         int mark = 0;
//         bool start;
//         bool keepgoing = false;
//         TNodeID save; //  This is the road id 许多短路径组成的一个长路径
//         std::vector <TNodeID> Index;
// // this part is for the fastest path
//         double totalTime;
//         double tempTime;
//         double distance;
//         std::list <double> TimeHolder;
//         std::list <double> ShortestTime;
        
// // prof part  ===== more to 
//         using TNodeIndex = std::size_t;
//         // std::unordered_map <TNodeID ,std::pair <double, double>> LOC;
//         // so I can use TNodeID to access the location which is good for fastest path


//         struct SEdge
//         {
//             TNodeIndex DOtherNodeIndex;
//             double DDistance;
//             double DTime;
//             double DMaxSpeed;
//         };

//         struct SNode
//         {       
//             TNodeID DNodeID;
//             double DLatitude;
//             double DLongitude;
//             TStopID BusStop;
//             std::vector <SEdge> DEdges;
//             std::vector <std::string> allBuses;
//             std::list<SNode> next;
//             std::list<SNode> previousNode;
//             std::list<SNode> WalkNodes;
//             std::string roadName;
//             bool bus = true;
//             bool walk = true;
//             int WALKvelocity = 3;
//             int BUSvelocity = 25;
//             bool oneway;
//             std::string destination;

//         };
//         bool OneWay = false;
//         int maxspeed;
//         bool takeBus;
//         bool onlyWalk;
//         bool changeSpeed = false;
//         std::list <SNode> PrevNode;
//         std::vector <SNode> OneRoad;
//         std::unordered_map <std::string, std::list<SNode> > eachRoad;
//         SNode Cur;
//         SNode empty;
//         SNode TempNode;
//         SNode Iter;
//         SNode Holder;
//         std::vector <SNode> NODES; // count how many nodes in the .osm
//         // std::unordered_map <TNodeID, TNodeIndex> DnodeIdToDnodeIndex;
//         std::unordered_map <TNodeID , SNode> CPnode;
//         std::list <SNode> Stack;
//         // save node id and the lat and lon 

        
//     public:
//         std::unordered_map <TNodeID ,std::pair <double, double>> LOC;
//         std::unordered_map <TNodeID, TNodeIndex> DnodeIdToDnodeIndex;
//         CMapRouter();
//         ~CMapRouter();
        
//         static double HaversineDistance(double lat1, double lon1, double lat2, double lon2);
//         static double CalculateBearing(double lat1, double lon1,double lat2, double lon2);
        
//         bool LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes);
//         size_t NodeCount() const;
//         TNodeID GetSortedNodeIDByIndex(size_t index) const;
//         TLocation GetSortedNodeLocationByIndex(size_t index) const;
//         TLocation GetNodeLocationByID(TNodeID nodeid) const;
//         TNodeID GetNodeIDByStopID(TStopID stopid) const;
//         size_t RouteCount() const;
//         std::string GetSortedRouteNameByIndex(size_t index) const;
//         bool GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops);
        
//         double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path);
//         double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path);
//         bool GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const;
// };

// #endif
