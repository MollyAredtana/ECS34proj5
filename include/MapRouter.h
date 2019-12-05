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
//         using TNodeID = unsigned long;
//         using TStopID = unsigned long;
//         using TLocation = std::pair<double, double>;
//         using TPathStep = std::pair<std::string, TNodeID>;
        
//         static const TNodeID InvalidNodeID;
        
//     private:
//         using TNodeIndex = std::size_t;
// //        std::list <std::string> StopID;
// //      std::vector <TNodeID> BUSStopID;
//  //       std::list <std::string> NodeID;
//  //       std::unordered_map <TNodeID ,TStopID> NodeToStop;
//    //     std::unordered_map <std::string ,std::vector <TNodeID>> BUSLine;
//      //   std::unordered_map <TStopID ,std::vector <std::string>> ReversedBUSLine;
        
        
//         //Copied from given website
//         struct pair_hash
// 		{
// 			template <class T1, class T2>
// 			std::size_t operator() (const std::pair<T1, T2> &pair) const
// 			{
// 				return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
// 			}
// 		};



//         struct SEdge
//         {
//             TNodeIndex DOtherNodeIndex;
//             // std::unordered_map <std:: < TNodeID, TNodeID>, std::vector<std::string>> ID_to_Name;
//             double DDistance;
//             double DTime = 0.0;
//             double DMaxSpeed;
//         };

//         struct SNode
//         {       
//             TNodeID DNodeID;
//             double DLatitude;
//             double DLongitude;
//             std::list <TStopID> BusStop;
//             std::list <std::string> RouteName;
//             bool takeBUS = false;
//             std::vector <SEdge> DEdges; //Driving Edges
//             std::vector <SEdge> DWEdges; //Walking Edges
//             std::vector <SEdge> Driving_Edge;
//         };
//         // SNode TempNode;
//         std::vector <SNode> DNodes;
//         std::vector <TNodeID> Index;
//         std::unordered_map <TNodeID, TNodeIndex> DNodeIDToNodeIndex;
//         std::unordered_map <TStopID ,TNodeID> DStopIDToNodeID;
//         std::unordered_map <TNodeID ,TStopID> NodeToStop;
//         std::unordered_map <TStopID ,TNodeIndex> DStopIDToNodeIndex;
//         std::vector <std::string> DSortedRoutes;
//         std::unordered_map< std::string, std::vector < TStopID > > DRouteToStops;
//         std::unordered_map< std::pair< TNodeIndex, TNodeIndex >, std::vector< std::string >, pair_hash > DBusEdgesToRouteName;
//         std::unordered_map< std::pair< TNodeIndex, TNodeIndex >, std::vector< TNodeIndex >, pair_hash > DBusEdgesToRoutePath;
//         double Dijkstras(TNodeIndex src, TNodeIndex dest, std::vector<TNodeIndex> &path, char type);
        
                
//     public:
//         std::unordered_map <TNodeID ,std::pair <double, double>> LOC;
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
            double DDistance;
            double DTime;
            double DMaxSpeed;
            std::vector <TNodeIndex> SaveBetween; //Holds the Stops that are passed over
            int PassStops = 0; //Check if Stops are passed over
        };

        struct SNode
        {       
            TNodeID DNodeID;
            double DLatitude;
            double DLongitude;
            TStopID BusStop;
            std::vector <SEdge> DEdges; //Driving Edges
            std::vector <SEdge> DWEdges; //Walking Edges
            std::vector <SEdge> Driving_Edge;
            std::unordered_map <TNodeIndex, TNodeIndex> BusNodeToNode;
        };
        
        std::vector <SNode> NODES;
                
    public:
        std::unordered_map <TNodeID ,std::pair <double, double>> LOC;
        std::unordered_map <TNodeID, TNodeIndex> DNodeIDToNodeIndex;
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







