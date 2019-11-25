#include "MapRouter.h" 	  			 	 
#include <cmath>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <list>
#include <algorithm> 
#include "XMLReader.h"
#include "XMLEntity.h"
#include "CSVReader.h"
//Q  how to get rid of this error line here



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

    // 在教授的做法后我们可以把stop ID也联系到 node里面

    // this part is for the .osm
    // SXMLEntity SX; // xml reader here
// this part is for stops.csv
    if(stops.bad() || routes.bad() || osm.bad())
    {
        return false;
    }
    
    CCSVReader STOPReader(stops);
    STOPReader.ReadRow(Stop);
    while(STOPReader.ReadRow(Stop))
    {
        StopID.push_back(Stop.front());
        NodeID.push_back(Stop.back()); //  these are the node Id in the stops.csv
        if(!StopID.empty()) // 如果不是空的
        {
            auto front = std::stoul(StopID.front());
            auto num = std::stoul(NodeID.front());
            // two maps doubly linked
            NodeToStop[num] = front;
            StopToNode[front] = num; // I make it become doubly linked
            StopID.pop_front();
            NodeID.pop_front();
        }
        Stop.clear();
    }

// this part is for route.csv

    CCSVReader BUSReader(routes);

    BUSReader.ReadRow(BUS);
    // int pos = 0;
    while(BUSReader.ReadRow(BUS))
    {
        std::vector <std::string> empty;
        // std::string temp;
        BusName.push_back(BUS.front());
        BusName.erase( std::unique( BusName.begin(), BusName.end() ), BusName.end() );
        auto key = BusName.back();
        auto temp = BUS.back(); // this is the stop id
        auto num = std::stoul(BUS.back());
        if(BUS.front() == key)  //还在同一个里面
        {
            BUSStopID.push_back(num);
        }
        BUSLine[key] = BUSStopID;

        // auto num = std::to_string(temp);

        if(ReversedBUSLine.find(temp) != ReversedBUSLine.end()) // found
        {
            for(auto i : ReversedBUSLine[temp])
            {
                allLines.push_back(i);
            }
        }
        ReversedBUSLine[temp] = allLines;
        allLines.clear();
        
    }


    CXMLReader XMLReader(osm);
    SXMLEntity Entity;
    // XMLReader.ReadEntity(Entity);


// still have to figure out how to 区分 vertices and edges;
    while(XMLReader.ReadEntity(Entity))
    {
        if(Entity.DNameData == "node" && Entity.DType == SXMLEntity::EType::StartElement)
        {
            // std::cout << "   ----5---  " << std::endl;
            auto ID = std::stoul(Entity.AttributeValue("id"));
            auto Latitude = std::stod(Entity.AttributeValue("lat"));
            auto Longititude = std::stod(Entity.AttributeValue("lon"));
            // std::make_pair <double, double> location;
        
            TempNode.DNodeID = ID;
            TempNode.DLatitude = Latitude;
            TempNode.DLongititude = Longititude;
            LOC[TempNode.DNodeID] = {TempNode.DLatitude, TempNode.DLongititude};
            DnodeIdToDnodeIndex[ID] = NODES.size();
            // NODES.push_back(TempNode);


            auto station = NodeToStop.find(std::stoul(Entity.AttributeValue("id")));
            unsigned long STOPinfo;
            // station  should be the value
            if(station != NodeToStop.end())// which means it is in it
            {
                STOPinfo = std::stoul(Entity.AttributeValue("id"));
                TempNode.BusStop = STOPinfo;
            }
                
            for(auto i : BUSStopID)
            {
                // auto id = std::stoul(i);
                if(STOPinfo == i)
                {
                    TempNode.allBuses = BusName;
                }
            }
            NODES.push_back(TempNode);  
            CPnode[TempNode.DNodeID] = TempNode; 
            
        }

                // in one field
        // std::cout <<NODES.size() << "sizeeeeeeeeee" << std::endl;
        
        // unsigned long cp;
        else if(Entity.DNameData == "way" && Entity.DType == SXMLEntity::EType::StartElement)
        {
            std::cout << "    wayyyyyyyyy   " << std::endl;
            save = std::stoul(Entity.AttributeValue("id"));
            mark++;
        }
        else if(Entity.DNameData == "nd" && Entity.DType == SXMLEntity::EType::StartElement && mark == 1)
        {
            OneRoad.clear();
            for(auto i : NODES)
            {
                if(i.DNodeID == std::stoul(Entity.AttributeValue("ref")))
                {
                    PrevNode.push_front(i);
                    OneRoad.push_front(i);
                }
            }
            mark--;
        }
        else if(Entity.DNameData == "nd" && Entity.DType == SXMLEntity::EType::StartElement && mark == 0)
        {
            for(auto i : NODES)
            {
                if(i.DNodeID == std::stoul(Entity.AttributeValue("ref")))
                {
                    // PrevNode.front().next.push_front(i);
                    // PrevNode.pop_front();
                    // PrevNode.push_front(i);
                    for(auto j : NODES)
                    {
                        if(j.DNodeID == PrevNode.front().DNodeID)
                        {
                            j.next.push_front(i);
                            std::cout << j.DNodeID   << "  " << j.next.front().DNodeID << "  this is each node id  " << std::endl;
                            PrevNode.pop_front();
                            PrevNode.push_front(i);
                            break;
                        }
                    }
                }
            }
        }
        else if(Entity.DNameData == "tag" && Entity.DType == SXMLEntity::EType::StartElement)
        {
            // if(Entity.AttributeValue("k") == "name")
            // {
            //     auto it = std::find(NODES.begin(), NODES.end(), OneRoad.front());
            //     while(it != NODES.end())
            //     {
            //         it->roadName = Entity.AttributeValue("v");
            //         OneRoad.pop_front();
            //         auto it = std::find(NODES.begin(), NODES.end(), OneRoad.front());
            //     }  
            // }
            if(Entity.AttributeValue("k") == "maxspeed")
            {

                for(auto i : NODES)
                {
                    for(auto j : OneRoad)
                    {
                        if(i.DNodeID == j.DNodeID)
                        {
                            i.bus = true;
                            i.BUSvelocity = std::stoi(Entity.AttributeValue("v"));
                            break;
                            // OneRoad.pop_front();
                        }
                    }
                }
                // auto it = std::find(NODES.begin(), NODES.end(), OneRoad.front());
                // while(it != NODES.end())
                // {
                //     it->bus = true;
                //     it->BUSvelocity = std::stoi(Entity.AttributeValue("v"));
                //     OneRoad.pop_front();
                //     auto it = std::find(NODES.begin(), NODES.end(), OneRoad.front());
                // }  
            }
            else if(Entity.AttributeValue("k") == "foot")
            {
                if(Entity.AttributeValue("v") == "yes")
                {
                    for(auto i : NODES)
                    {
                        for(auto j : OneRoad)
                        {
                            if(i.DNodeID == j.DNodeID)
                            {
                                i.walk = true;
                                i.WALKvelocity = 3;
                                break;
                                // OneRoad.pop_front();
                            }
                        }
                    }
                }
            }
            else if(Entity.AttributeValue("k") == "barrier")
            {
                for(auto i : NODES)
                {
                    for(auto j : OneRoad)
                    {
                        if(i.DNodeID == j.DNodeID)
                        {
                            i.bus = false;
                            // i.WALKvelocity = 3;
                            break;
                            // OneRoad.pop_front();
                        }
                    }
                }

            }
            
        }

        // while(mark && Entity.DNameData != "way")
        // {
        //     std::cout <<"  hereeeeeeee  eeee" << std::endl;
        //     std::cout << " keepgoing  @ " <<__LINE__ <<std::endl;
        //     if(Entity.DNameData == "way" && Entity.DType == SXMLEntity::EType::EndElement)
        //     {
        //         break;
        //     }
        //     // save = std::stoul(Entity.AttributeValue("id"));
        //     XMLReader.ReadEntity(Entity);// keep reading here


        //     if(Entity.DNameData == "nd")
        //     {
        //         auto ND = std::stoul(Entity.AttributeValue("ref"));
        //         auto TN = CPnode[ND];
        //         if(PrevNode.empty())
        //         {
        //             PrevNode.push_front(TN);
        //         }
        //         else
        //         {
        //             Cur = PrevNode.front();
        //             for(auto i : NODES)
        //             {
        //                 if(i.DNodeID == Cur.DNodeID)
        //                 {
        //                     i.next.push_back(TN);
        //                     OneRoad.push_front(i);
        //                 }
        //             }
        //         }
        //     }
        //     else if(Entity.DNameData == "tag")
        //     {
        //         if(Entity.AttributeValue("k") == "name")
        //         {
        //             auto ROAD_name = Entity.AttributeValue("v");
        //             eachRoad[ROAD_name] = OneRoad;
        //         }
        //         else if(Entity.AttributeValue("k") == "maxspeed")
        //         {
        //             for(auto i : NODES)
        //             {
        //                 for(auto j : OneRoad)
        //                 {
        //                     if(i.DNodeID == j.DNodeID)
        //                     {
        //                         i.BUSvelocity = std::stoi(Entity.AttributeValue("v"));
        //                         i.bus = true;
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }
        SXMLEntity Entity;
    }
    // if(Index.empty()){
    for(auto i : NODES)
    {
        Index.push_back(i.DNodeID);
    }
    std::sort(Index.begin(), Index.end());
	// }    
    if(!NODES.empty())
    {
        return true;
    }
    else{
        return false;
    }
}




size_t CMapRouter::NodeCount() const{
    // Your code HERE
    // Returns the number of nodes read in by the osm file

    // I will keep track of nodes inside of the LOAD Function so I can just do
    std::cout << " @ " <<__LINE__ <<std::endl;
    return NODES.size();
}

CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const{
    // Your code HERE
    // Returns the node ID of the node specified by the index of the
    // nodes in sorted order
    std::cout << " @ " <<__LINE__ <<std::endl;

    // after you have save the nodes in a map or hashtable you 
    // will have the indexes for each node, then you can return the node which is specified 
    // by the parameter index
	
	return Index[index];
}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const{
    // Your code HERE
    // Returns the location of the node that is returned by
    // GetSortedNodeIDByIndex when passed index, returns
    // std::make_pair(180.0, 360.0) on error
    std::cout << " @ " <<__LINE__ <<std::endl;

    // just use std::sort() to sort
    TNodeID Temp = CMapRouter::GetSortedNodeIDByIndex(index);
    TLocation Ret;
    
    
    if(index > (NODES.size() - 1) || index < 0){
    	Ret = {180.0, 360.0};
    	return Ret;
	}
    
    for(auto i : NODES){
    	if(Temp == i.DNodeID)
        {
    		Ret = {i.DLatitude, i.DLongititude};
    		continue;
		}
    	
	}
    
    
    return Ret;

}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const{
    // Your code HERE
    // Returns the location of the node specified by nodeid, returns
    // std::make_pair(180.0, 360.0) on error
    CMapRouter::TLocation location;
    
    for(auto i : NODES)
    {
        // auto stop = std::stoul(i);    // this might be the faster way
        
        if(i.DNodeID == nodeid) // then we know it is the one
        {
            
            location = std::make_pair(i.DLatitude, i.DLongititude);
            return location;
        }
    }



}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const{
    // Your code HERE
    // Returns the node ID of the stop ID specified

    // just look for it inside of my MAP

	// std::unordered_map <TStopID ,TNodeID>::const_iterator got = BusStations.find(stopid);
    // std::cout << BusStations.size() << "###########" << std::endl;
    
    // for(auto i : ReversedBusStations)
    // {
    //     std::cout << i.first << "-------" << stopid << std::endl;
    //     if(i.first == stopid)
    //     {
    //         return i.second;
    //     }
    // }
    return StopToNode.at(stopid);
	// if(got == BusStations.end()){
	// 	return 0;
	// }
	// return got->second;

    // std::cout << " getnodeID @ " <<__LINE__ <<std::endl; 
    // auto get = BusStations.find(stopid);
    // if(get != BusStations.end())
    // {
    //     return BusStations[get];
    // }

}

size_t CMapRouter::RouteCount() const{
    // Your code HERE
    // Returns the number of bus routes
    
    // it return the combining roads from one bus station to the other one
    std::cout << " @ " <<__LINE__ <<std::endl;
    return BusName.size();


}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const{
    // Your code HERE
    // Returns the name of the bus route specified by the index of
    // the bus routes in sorted order
    return BusName[index];
}

bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops){
    // Your code HERE
    // Fills the stops vector with the stops of the bus route
    // specified by route
    if(route.length() >= 2 || route.length() == 0)
    {
        return false;
    }
    stops = BUSLine[route];
    return true;
    
    
}

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
    // Your code HERE             // start node   desti node           // bunch of node id inside
    // Finds the shortest path from the src node to dest node. The
    // list of nodes visited will be filled in path. The return
    // value is the distance in miles,
    // std::numeric_limits<double>::max() is returned if no path
    // exists.

    // dik忘记咋拼了
//     Finds the fastest path from the src node to dest node. 
//     The list of nodes and mode of transit will be filled in path. 
//     The return value is the time in hours, 
//     std::numeric_limits<double>::max() is returned if no path exists. 
//     When a bus can be taken it should be taken for as long as possible.
//      If more than one bus can be taken for the same length, 
//     the one with the earliest route name in the sorted route names.




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

    // still have to do something with total time and the TPathStep path
    // std::cout << path.empty() << "   ----  " << std::endl;

    std::cout << NODES.size() << "  NODES sizeeeeee  " << std::endl;
    double lat1;
    double lon1;
    for(auto i : NODES)
    {
        if(i.DNodeID == src)
        {
            std::cout << "  @@@@@   " << std::endl;
            Iter = i;
            lat1 = i.DLatitude;
            lon1 = i.DLongititude;
            break;
        }
    }

    std::vector<bool> visited(Iter.DNodeID, false);// 最开始是false, 因为啥也没有
    Stack.push_back(Iter.DNodeID); // size 1
    // std::cout <<  Iter.DNodeID << "  " << Iter.next.front().DNodeID << " next.dnodeID  " << std::endl; 
    

    while(!Stack.empty())
    {
        std::cout << Stack.size() << "  this is the size eeee  " << std::endl;
        auto temp = Stack.front();
        Stack.pop_front();

        if(!visited[temp]) // 没visit 过所以现在要visit 了
        {
            // if(visited.size() == 1) // which means it is the first one  and we don't need to
            // calculate the distance for the statring node
            // {
            visited[temp] = true;
            // }

            std::cout << "  _________---------________- " << std::endl;
            for(auto i : Iter.next) //  let's check the time
            {
                std::cout << i.DNodeID << std::endl;
                // auto lat2 = LOC[i].first;
                // auto lon2 = LOC[i].second;
                auto lat2 = i.DLatitude;
                auto lon2 = i.DLongititude;


                distance = CMapRouter::HaversineDistance(lat1,lon1, lat2, lon2) * 
                CMapRouter::CalculateBearing(lat1,lon1, lat2, lon2);

                if(Iter.bus)
                {
                    tempTime = distance / Iter.BUSvelocity;
                }
                else{
                    tempTime = distance / Iter.WALKvelocity;
                }

                if(TimeHolder.empty())
                {
                    TimeHolder.push_front(tempTime);

                }

                if(tempTime < TimeHolder.front() && !TimeHolder.empty())
                {
                    TimeHolder.pop_front();
                    TimeHolder.push_front(tempTime);
                    Holder = Iter;
                }
                // auto strId = std::to_string(Iter.DNodeID);
                // std::string syntax = "Bus";
                // std::vector <std::string> name = ReversedBUSLine.at(strId);
                // std::string bus = name.front();
                // // std::string BUS = syntax + name;
                // auto pt = std::make_pair(bus, Iter.DNodeID);
                // path.push_back(pt);
                Iter = CPnode[Holder.DNodeID]; // keep moving it
                Stack.push_front(Iter.DNodeID);
            }
            ShortestTime.push_front(TimeHolder.front());
        }
        // std::cout << "  hereeeeee1 " << std::endl;
        // Stack.push_front(Holder.DNodeID);

    }

    for(auto i : ShortestTime)
    {
        totalTime += i;
    }

    return totalTime;
}

bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
    // Your code HERE
    // Returns a simplified set of directions given the input path
    std::string start = "Start at ";
    std::string CR = "0d";
    std::string East = "E";
    std::string West = "W";
    std::string North = "N";
    std::string South = "S";
    std::string WALK = "Walk ";
    std::string TAKEbus = "Take ";
    std::string Switch = "and get off at stop ";

    if(path.empty())
    {
        return false;
    }

    // std::string sentence1;
    // sentence1 = start + path.front().second + End;
    // while(!path.empty())
    // {

        
    // }
}
