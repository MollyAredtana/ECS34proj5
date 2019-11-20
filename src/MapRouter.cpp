#include "MapRouter.h" 	  			 	 
#include <cmath>
#include <iostream
#include <fstream>



const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;

CMapRouter::CMapRouter(){
    //Constructor of the CMapRouter
    // I think there is no need to write anything for constructor 
    // since we build the vectors which can be considered as the graph in .h

    //整体的图只需要公交线路和站点

}

CMapRouter::~CMapRouter(){
    // Destructor of the CMapRouter

}

double CMapRouter::HaversineDistance(double lat1, double lon1, double lat2, double lon2){
    // Provided function to calculate the distance in miles
    auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
    // radius
	double LatRad1 = DegreesToRadians(lat1);
	double LatRad2 = DegreesToRadians(lat2);
	double LonRad1 = DegreesToRadians(lon1);
	double LonRad2 = DegreesToRadians(lon2);
	double DeltaLat = LatRad2 - LatRad1;
	double DeltaLon = LonRad2 - LonRad1;
	double DeltaLatSin = sin(DeltaLat/2);
	double DeltaLonSin = sin(DeltaLon/2);	
	double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
	const double EarthRadiusMiles = 3959.88;
	
	return 2 * EarthRadiusMiles * Computation;
}        

double CMapRouter::CalculateBearing(double lat1, double lon1,double lat2, double lon2){
    // Provided function to calculate the bearing in degrees
    // degrees
    auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
    auto RadiansToDegrees = [](double rad){return 180.0 * (rad) / M_PI;};
    double LatRad1 = DegreesToRadians(lat1);
	double LatRad2 = DegreesToRadians(lat2);
	double LonRad1 = DegreesToRadians(lon1);
	double LonRad2 = DegreesToRadians(lon2);
    double X = cos(LatRad2)*sin(LonRad2-LonRad1);
    double Y = cos(LatRad1)*sin(LatRad2)-sin(LatRad1)*cos(LatRad2)*cos(LonRad2-LonRad1);
    return RadiansToDegrees(atan2(X,Y));
}
// calculate the distance should use HaversineDistance * CalculateBearing 

bool CMapRouter::LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes){
    // Your code HERE
    // Loads the map, stops, and routes given the input streams

    // this part is for the .osm
    SXMLReader SX; // xml reader here
    std::string fileOSM = "davis.osm";

    std::ifstream s(fileOSM);
    std::pair< std::string, std::string > first;
    unsigned long NODEid;
    unsigned long LAT;
    unsigned long LON;

    if(!s.is_open())
    {
        std::cout << "  it can not be opened correctly  " << std::endl;
    }

    CXMLReader XMLReader(s);
    XMLReader.ReadEntity();

    while(!XMLReader.End())
    {
        // read entity inside and use the same way I used in proj4 main.cpp to save the 
        // vertices into a vector or array or list 
        // the id inside of the osm are all the vertices 
        // the id inside of the stops.csv 也是vertices但是 davis.osm 已经包含了
        // 但需要注意的是到一个stops才是一段path 所以一定要把 stops 和 每段路的vertices 分别存放

        // 在这个while loop里只需要存放每段路的定点
        Reader.ReadEntity(Entity);
        if(!Entity.DAttributes.empty())
        {
            first = Entity.DAttributes.front();
            NODEid = Entity.DAttributes.front().second; //  value
            // Vertices.push_back(Value); // this is the node id
            Entity.DAttributes.erase(Entity.DAttributes.begin());
            LAT = Entity.DAttributes.front().second; //  value
            // ertices.push_back(Value); // this is the node id
            Entity.DAttributes.erase(Entity.DAttributes.begin());
            LON = Entity.DAttributes.front().second; //  value
            // ertices.push_back(Value); // this is the node id
            Entity.DAttributes.erase(Entity.DAttributes.begin());

            if(Entity.empty())
            {
                MAP[NODEid] = {LAT, LON}; // I create a map here
            }

            std::pair< std::string, std::string > first; // renew it
            
        }
        NODES++; // we can get how many nodes overhere
    }

// this part is for the stop.csv
    
    std::string fileSTOP = "stops.csv";

    CCSVReader STOPReader(fileSTOP);
    
    STOPReader.ReadRow(stops); // so we can get all the stop here


// this part is for the Route.csv
    std::string fileBUS = "routes.csv";

    CCSVReader BUSReader(fileBUS);

    BUSReader.ReadRow(BUS); // so we can get all the bus here
    // but we may want to make the buffer to becomes a vector od pointers here
    // so we can use the ptr to point to the node id like ptr->ID;



}

size_t CMapRouter::NodeCount() const{
    // Your code HERE
    // Returns the number of nodes read in by the osm file

    // I will keep track of nodes inside of the LOAD Function so I can just do
    return NODES;
}

CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const{
    // Your code HERE
    // Returns the node ID of the node specified by the index of the
    // nodes in sorted order

    // after you have save the nodes in a map or hashtable you 
    // will have the indexes for each node, then you can return the node which is specified 
    // by the parameter index
}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const{
    // Your code HERE
    // Returns the location of the node that is returned by
    // GetSortedNodeIDByIndex when passed index, returns
    // std::make_pair(180.0, 360.0) on error

}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const{
    // Your code HERE
    // Returns the location of the node specified by nodeid, returns
    // std::make_pair(180.0, 360.0) on error

}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const{
    // Your code HERE
    // Returns the node ID of the stop ID specified
}

size_t CMapRouter::RouteCount() const{
    // Your code HERE
    // Returns the number of bus routes
}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const{
    // Your code HERE
    // Returns the name of the bus route specified by the index of
    // the bus routes in sorted order
}

bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops){
    // Your code HERE
    // Fills the stops vector with the stops of the bus route
    // specified by route
}

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
    // Your code HERE
    // Finds the shortest path from the src node to dest node. The
    // list of nodes visited will be filled in path. The return
    // value is the distance in miles,
    // std::numeric_limits<double>::max() is returned if no path
    // exists.

    // DFS
}

double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path){
    // Your code HERE
    // Finds the fastest path from the src node to dest node. The
    // list of nodes and mode of transit will be filled in path.
    // The return value is the time in hours,
    // std::numeric_limits<double>::max() is returned if no path
    // exists. When a bus can be taken it should be taken for as
    // long as possible. If more than one bus can be taken for the
    // same length, the one with the earliest route name in the
    // sorted route names.

    //BFS
}

bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
    // Your code HERE
    // Returns a simplified set of directions given the input path
}
