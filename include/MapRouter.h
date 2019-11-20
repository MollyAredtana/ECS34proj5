#ifndef MAPROUTER_H 	  			 	 
#define MAPROUTER_H

#include <vector>
#include <istream>
#include <iostream>
#include <map>
#include <string>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "CSVReader.h"
#include "CSVWriter.h"

class CMapRouter{
    public:
        using TNodeID = unsigned long; // the node id in davis.osm
        using TStopID = unsigned long; // stop id in both routes.csv and stops.csv
        using TLocation = std::pair<double, double>;
        using TPathStep = std::pair<std::string, TNodeID>;
        
        static const TNodeID InvalidNodeID;
    private:
    // maybe use the map to save all the things overhere
        std::vector <unsigned long> Vertices; // vertices are the node id
        std::vector <int> stops; // edges are the distances
        std::vector <char> BUS; // kee[ track of the Bus lines 

        int NODES; // count how many nodes in the .osm
        std::map <unsigned long, std::make_pair<unsigned long, unsigned long> > MAP;

        
    public:
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
