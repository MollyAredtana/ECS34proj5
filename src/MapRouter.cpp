#include "MapRouter.h" 	
#include "XMLReader.h"
#include "XMLEntity.h"
#include "CSVReader.h"  			 	 
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <algorithm> 
#include <limits.h>
#include <iomanip>


const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;

CMapRouter::CMapRouter(){
}

CMapRouter::~CMapRouter(){
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
    if(stops.bad() || routes.bad() || osm.bad()){
        return false;
    }
    
    CXMLReader XMLReader(osm);
    SXMLEntity Entity;
	while(XMLReader.ReadEntity(Entity)) {
 		std::vector <TNodeIndex> TempIndices;
    	double WalkSpeed = 3.0;
    	double MaxSpeed = 25;
    	bool IsOneWay = false;
    	//Set up Nodes
        if(Entity.DNameData == "node" && Entity.DType == SXMLEntity::EType::StartElement){
            auto ID = std::stoul(Entity.AttributeValue("id"));
            auto Latitude = std::stod(Entity.AttributeValue("lat"));
            auto Longititude = std::stod(Entity.AttributeValue("lon"));
            SNode TempNode;
            TempNode.DNodeID = ID;
            TempNode.DLatitude = Latitude;
            TempNode.DLongitude = Longititude;
            LOC[TempNode.DNodeID] = {TempNode.DLatitude, TempNode.DLongitude};
            DNodeIDToNodeIndex[ID] = NODES.size();
        	NODES.push_back(TempNode);
        	Index.push_back(ID);
		}
		//Set up Road Information
    	if(Entity.DNameData == "way") {
            while(Entity.DNameData != "way" or Entity.DType != SXMLEntity::EType::EndElement) {
                XMLReader.ReadEntity(Entity);
                if(Entity.DNameData == "nd" && Entity.DType == SXMLEntity::EType::StartElement) {
                     TNodeID TempID = std::stoul(Entity.AttributeValue("ref"));
                     auto search = DNodeIDToNodeIndex.find(TempID);
                     if(search != DNodeIDToNodeIndex.end()) {
                         TempIndices.push_back(search->second);
                     }
                }
                if(Entity.DNameData == "tag" && Entity.DType == SXMLEntity::EType::StartElement) {
                	if(Entity.AttributeValue("k") == "maxspeed") {
                        std::stringstream TempStream(Entity.AttributeValue("v"));
                        TempStream>>MaxSpeed;
                	}
                    if(Entity.AttributeValue("k") == "oneway"){
                         IsOneWay = Entity.AttributeValue("v") == "yes";
                    }
            	}
        	}
    	}
    	//Set up Edges
    	for(size_t i = 0; i + 1 < TempIndices.size(); i++) {
	        SEdge TempEdge;
        	auto FromNode = TempIndices[i];
        	auto ToNode = TempIndices[i + 1];
        	TempEdge.DDistance = HaversineDistance(NODES[FromNode].DLatitude, NODES[FromNode].DLongitude, NODES[ToNode].DLatitude, NODES[ToNode].DLongitude);
        	TempEdge.DOtherNodeIndex = ToNode;
        	TempEdge.DTime = TempEdge.DDistance / WalkSpeed;
        	TempEdge.DMaxSpeed = MaxSpeed;
        	NODES[FromNode].DEdges.push_back(TempEdge);
        	NODES[FromNode].DWEdges.push_back(TempEdge);
        	if(!IsOneWay) {
        		TempEdge.DOtherNodeIndex = FromNode;
        		NODES[ToNode].DEdges.push_back(TempEdge);
			}
			TempEdge.DOtherNodeIndex = FromNode;
        	NODES[ToNode].DWEdges.push_back(TempEdge);
    	}
	}
	
	std::vector <std::string> Stop;
	CCSVReader STOPReader(stops);
    STOPReader.ReadRow(Stop);
    while(STOPReader.ReadRow(Stop))
    {
        StopID.push_back(Stop.front());
        NodeID.push_back(Stop.back());
        if(!StopID.empty())
        {
            auto front = std::stoul(StopID.front());
            auto num = std::stoul(NodeID.front());
            NodeToStop[num] = front;
            StopToNode[front] = num;
            StopID.pop_front();
            NodeID.pop_front();
        }
        Stop.clear();
    }
    

	std::vector <std::string> BUS;
    auto pos = 0;
//    auto count = 0;
//    double TIME;
//    double DISTANCE;
    unsigned long SAVE_INDEX;
    TNodeID first_id;
    TNodeID first_index;
    std::list<TNodeID> STOP_id;
    std::map <std::string, std::list<TNodeID>> MAP;
    CCSVReader BUSReader(routes);
    BUSReader.ReadRow(BUS);
    while(BUSReader.ReadRow(BUS))
    {
 	//	std::vector <std::string>  allLines;
        BusName.push_back(BUS.front());
        if(MAP.empty()) // the very first start since the map is empty
        {
            STOP_id.push_back(std::stoul(BUS.back()));
            MAP[BUS.front()] = STOP_id;
            first_id = StopToNode[MAP[BUS.front()].front()]; // the first_id is the starting node
            first_index = DNodeIDToNodeIndex[first_id]; //  index of it
            // for the testrouter , it will be 2, 3
        } else	{
            STOP_id.push_back(std::stoul(BUS.back())); // stop id push back one more, it is 2, 3 inside
            MAP[BUS.front()] = STOP_id;
            MAP[BUS.front()].pop_front();
            STOP_id.pop_front(); //  pop 2
            auto cur_id = StopToNode[MAP[BUS.front()].front()]; //  cur will be 3 and 5
            auto cur_index = DNodeIDToNodeIndex[cur_id];
            if(NODES[first_index].DNodeID != NODES[cur_index].DNodeID)
            { // if the starting node is not the end one
                SEdge Temp;
                Temp.DMaxSpeed = 25; //speed
                for(auto i : NODES[first_index].DEdges)
                {
                    if(NODES[i.DOtherNodeIndex].DNodeID == NODES[cur_index].DNodeID) // found
                    {
                    	// if start is linking to end , ex: 2->3
                        pos = -1;
                        break;
                    }else {
                    	// ex 3->5
                        pos = 1;
                        for(auto j : NODES[i.DOtherNodeIndex].DEdges) // find the connection between 3 and 5 which is 4
                        {
                            if(NODES[j.DOtherNodeIndex].DNodeID == NODES[cur_index].DNodeID)
                            {
                                SAVE_INDEX = i.DOtherNodeIndex;
                                // save index will be the connecting node between 3 and 5
                            }
                        }
                    } 
                }
                if(pos == 1) // if not linking like 3->5
                {// need to calculate multiple distances;
                // may need to modify here, since there won't be only 2 seperate roads in actual map
                    auto dis1 = HaversineDistance(NODES[first_index].DLatitude, NODES[first_index].DLongitude, NODES[SAVE_INDEX].DLatitude, NODES[SAVE_INDEX].DLongitude);
                    auto dis2 = HaversineDistance(NODES[SAVE_INDEX].DLatitude, NODES[SAVE_INDEX].DLongitude, NODES[cur_index].DLatitude, NODES[cur_index].DLongitude);
                    Temp.DDistance = dis1 + dis2;
                    NODES[cur_index].BusNodeToNode[cur_index] = first_index; //Map is done here
                    Temp.SaveBetween.push_back(SAVE_INDEX);
                    Temp.PassStops = Temp.SaveBetween.size();
//					std::cout << "Node Id is " << NODES[cur_index].DNodeID << " cur_index is " << cur_index << " first_index  is " << first_index << std::endl; 
                } else if(pos == -1){
                	// if it is alreay linking to each other
                    Temp.DDistance = HaversineDistance(NODES[first_index].DLatitude, NODES[first_index].DLongitude, NODES[cur_index].DLatitude, NODES[cur_index].DLongitude);
                    NODES[cur_index].BusNodeToNode[cur_index] = first_index; //Map is done here
                }
                                                
                Temp.DTime = Temp.DDistance / Temp.DMaxSpeed;
                Temp.DOtherNodeIndex = cur_index;
                NODES[first_index].Driving_Edge.push_back(Temp);
                first_index = cur_index; // update here
            }
        }
        BusName.erase( std::unique(BusName.begin(), BusName.end() ), BusName.end() );
        
        auto S_ID = std::stoul(BUS.back());
        auto temp_next_id = StopToNode[S_ID];
        auto index = DNodeIDToNodeIndex[temp_next_id];
        if(BUS.front() == BusName.back())
        {
            BUSStopID.push_back(S_ID);
        }
        BUSLine[BusName.back()] = BUSStopID;
		ReversedBUSLine[S_ID].push_back(BUS.front());
    }
    
    
    
    
    
    
	
    std::sort(Index.begin(), Index.end());
    
    
    
    
	   
    if(!NODES.empty()){
        return true;
    }else{
    	return false;
    }
}

size_t CMapRouter::NodeCount() const{
    return NODES.size();
}

CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const{
	
	if(index < Index.size()){
		return Index[index];
	}
	
	return InvalidNodeID;
}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const{
	if(index < Index.size()){
		auto NodeID = Index[index];
		auto Search = DNodeIDToNodeIndex.find(NodeID);
		auto NodeIndex = Search->second;
		return TLocation(NODES[NodeIndex].DLatitude, NODES[NodeIndex].DLongitude);
	}
	return TLocation(180.0, 360.0);
}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const{
	auto Search = DNodeIDToNodeIndex.find(nodeid);
	if(Search != DNodeIDToNodeIndex.end()){
		auto NodeIndex = Search->second;
		return TLocation(NODES[NodeIndex].DLatitude, NODES[NodeIndex].DLongitude);
	}
	return TLocation(180.0, 360.0);
}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const{
    return StopToNode.at(stopid);
}

size_t CMapRouter::RouteCount() const{
    return BusName.size();
}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const{
    return BusName[index];
}

bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops){
    if(route.length() >= 2 || route.length() == 0)
    {
        return false;
    }
    stops = BUSLine[route];
    return true;
}

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
	//The Algorithm is a modified version of the one from geeksforgeeks
    // https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
    double Dist[NODES.size()];
    bool SPTSet[NODES.size()];
    int SrcIndex;
    int DestIndex;
    int TempIndex;
    
    //check if a loop or search takes more energy
    for(int i = 0; i < NODES.size(); i++){
        if(NODES[i].DNodeID == src){
            Dist[i] = 0;
            SrcIndex = i;
            continue;
        }
        Dist[i] = INT_MAX;
        SPTSet[i] = false;
    }


	for(int count = 0; count < NODES.size(); count++) {
	//Choose the minimum distance value
		int min = INT_MAX;
		int min_index;
		for (int v = 0; v < NODES.size(); v++){
			if(SPTSet[v] == false && Dist[v] <= min){
				min = Dist[v];
				min_index = v;
			}
		}
		SPTSet[min_index] = true;
		
		for(int i = 0; i < NODES[min_index].DWEdges.size(); i++) {
			if(Dist[NODES[min_index].DWEdges[i].DOtherNodeIndex] == INT_MAX) {
			Dist[NODES[min_index].DWEdges[i].DOtherNodeIndex] = NODES[min_index].DWEdges[i].DDistance + Dist[min_index]; //node index goes in dist
			}
		}
	}
	
	auto Search = DNodeIDToNodeIndex.find(dest);
	DestIndex = Search->second;
	TempIndex = DestIndex;
	int MinEdge;
	
//	std::cout << "Right before your loop" <<std::endl;
// Output Shortest Path
	while(TempIndex != SrcIndex){
		int min = Dist[TempIndex];
		for(int i = 0; i < NODES[TempIndex].DWEdges.size(); i++) {
			if(min > Dist[NODES[TempIndex].DWEdges[i].DOtherNodeIndex]){
				min = Dist[NODES[TempIndex].DWEdges[i].DOtherNodeIndex];
				MinEdge = NODES[TempIndex].DWEdges[i].DOtherNodeIndex;
			}
		}
		path.insert(path.begin(), Index[TempIndex]);
		TempIndex = MinEdge;
	}
	
	path.insert(path.begin(), src);
	
	return Dist[DestIndex];

}

double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path){

	//The Algorithm is a modified version of the one from geeksforgeeks
    // https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
    double Time[NODES.size()];
    bool SPTSet[NODES.size()];
    int SrcIndex;
    int DestIndex;
    int TempIndex;
    int BusStop;
    int NextStop;
    std::vector <std::string> PossibleBuses;
    std::vector <std::string> NextBuses;
    
    
    for(int i = 0; i < NODES.size(); i++)
    {
        if(NODES[i].DNodeID == src)
        {
            Time[i] = 0;
            SrcIndex = i;
            continue;
        }
        Time[i] = INT_MAX;
        SPTSet[i] = false;
    }


	for(int count = 0; count < NODES.size(); count++) {
	//Choose the minimum distance value
		int min = INT_MAX;
		int min_index;
		for (int v = 0; v < NODES.size(); v++){
			if(SPTSet[v] == false && Time[v] <= min){
				min = Time[v];
				min_index = v;
			}
		}
		
		
		SPTSet[min_index] = true;
		
		//This checks if min_index has a bus stop or not, then it checks the buses
		auto Search = NodeToStop.find(Index[min_index]);
		BusStop = (Search == NodeToStop.end()) ? -1 : Search->second;
		
		if(BusStop != -1){
			PossibleBuses = ReversedBUSLine[BusStop];	
		}
		
		for(int i = 0; i < NODES[min_index].DWEdges.size(); i++) {
			auto NewTime = NODES[min_index].DWEdges[i].DTime + Time[min_index]; //node index goes in dist
			auto OldTime = Time[NODES[min_index].DWEdges[i].DOtherNodeIndex];
			Time[NODES[min_index].DWEdges[i].DOtherNodeIndex] = NewTime < OldTime ? NewTime : OldTime;
		}
		
		//Driving_Edge
		for(int i = 0; i < NODES[min_index].Driving_Edge.size(); i++) {
				if(BusStop != -1) {
					auto Search = NodeToStop.find(Index[NODES[min_index].Driving_Edge[i].DOtherNodeIndex]);
					NextStop = (Search == NodeToStop.end()) ? -1 : Search->second;
			
					if(NextStop != -1){
						NextBuses = ReversedBUSLine[NextStop];
						for(int i = 0; i > NextBuses.size(); i++){
							auto it = find(PossibleBuses.begin(), PossibleBuses.end(), NextBuses[i]);
						//	it == PossibleBuses.end() ? NextBuses.erase;
						}	
					}
				}
				
				
				if(NextStop != -1 && BusStop != - 1 && NextBuses.size() != 0){
					auto NewTime = NODES[min_index].Driving_Edge[i].DTime + Time[min_index] + (30.0000/60.0000/60.0000);
					auto OldTime = Time[NODES[min_index].Driving_Edge[i].DOtherNodeIndex];
					Time[NODES[min_index].Driving_Edge[i].DOtherNodeIndex] = NewTime < OldTime ? NewTime : OldTime;
				}
			}
	}
	

	auto Search = DNodeIDToNodeIndex.find(dest);
	DestIndex = Search->second;
	TempIndex = DestIndex;
	int MinEdge;
	std::string WalkBus;
	std::vector <TNodeID> BusRide;
	PossibleBuses.clear();
	int OnBus = -1;
	bool IsWalking;
	
	
	// std::cout << Time[0] << "   " << Time[1] << "   " <<Time[2] << std::endl;
	// std::cout << Time[3] << "   " << Time[4] << "   " <<Time[5] << std::endl;
		
	// std::cout << NODES[0].Driving_Edge.size() << "   " << NODES[1].Driving_Edge.size() << "   " <<NODES[2].Driving_Edge.size() << std::endl;
	// std::cout << NODES[3].Driving_Edge.size() << "   " << NODES[4].Driving_Edge.size() << "   " <<NODES[5].Driving_Edge.size() << std::endl;
		
		

	// std::cout << "Beginning of Loop" << std::endl;
	while(TempIndex != SrcIndex){
		double min = Time[TempIndex];
		IsWalking = false;
		for(int i = 0; i < NODES[TempIndex].DWEdges.size(); i++) {
			double temp = Time[NODES[TempIndex].DWEdges[i].DOtherNodeIndex];
			int tf = (min - temp) - NODES[TempIndex].DWEdges[i].DTime;
			if(min > temp && !tf){
				min = Time[NODES[TempIndex].DWEdges[i].DOtherNodeIndex];
				MinEdge = NODES[TempIndex].DWEdges[i].DOtherNodeIndex;
				WalkBus = "Walk";
				// std::cout << "min is now " << min << " and MinEdge is now " << MinEdge << std::endl;
				IsWalking = true;
			}
		}
		
		if(IsWalking) {
			//check if the bus was going previously
			OnBus = OnBus == -1? -1 : 0;
			
			//if the above is 0(you've been on the bus), then you need to push back all the BusRide elements with the possible bus
			if(OnBus == 0){
				for(int i = 0; i < BusRide.size() - 1; i++){
					path.insert(path.begin(), std::make_pair("Bus A", BusRide[i]));
				}
			}
			
			
			//change to the latest 
			path.insert(path.begin(),std::make_pair(WalkBus, Index[TempIndex]));
			TempIndex = MinEdge;
			continue;
		}
//		int tf;
		int EdgeIt;
//		std::cin >> tf;
		
		auto tempin = NODES[TempIndex].BusNodeToNode[TempIndex];
		std::cout << tempin << " " << TempIndex << std::endl;
		for(int i = 0; i < NODES[tempin].Driving_Edge.size(); i++){
			if(NODES[tempin].Driving_Edge[i].DOtherNodeIndex == TempIndex){
				EdgeIt = i;
			}
		}
		
		if(OnBus == - 1){
			BusRide.push_back(Index[TempIndex]);
		}
		
		std::cout << BusRide[0] << std::endl;
		for(int i = 0; i < NODES[tempin].Driving_Edge[EdgeIt].SaveBetween.size(); i++){
			BusRide.push_back(Index[NODES[tempin].Driving_Edge[EdgeIt].SaveBetween[i]]);
		}
		BusRide.push_back(Index[tempin]);
		
		std::cout << BusRide[0] << " " << BusRide[1] << " " << BusRide[2] << " " << BusRide[3] << " " << BusRide[4] << std::endl;
		
		OnBus = 1;
		

		TempIndex = tempin;
	}
	// std::cout << "End of Loop" << std::endl;
	
	
	
	
	path.insert(path.begin(),std::make_pair(WalkBus, src));
	
	// for(int i = 0; i < path.size(); i++){
	// 	std::cout << std::get<0>(path[i]) << "    " << std::get<1>(path[i]) << std::endl;
	// }


	
	return Time[DestIndex];
}

bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
    // Your code HERE
    // Returns a simplified set of directions given the input path
    // bool mark = false;
    
    // std::cout << " adasd" << std::endl;
    
    double prev_lat = 0.0;
    double prev_lon = 0.0;
    bool mark = false;
    bool check = false;
    std::list <std::string> Bus_stop;
    std::list <TNodeID> Bus_stop_ID;
    std::string START = "Start at";
    std::string END = "End at";
    std::string degree = "d"; //  layer 1
    std::string MINUTES = "'";//  layer 2
    std::string SECONDS = "\""; // layer 3
    int i = 0;
    std::string East = "E";
    std::string West = "W";
    std::string North = "N";
    std::string South = "S";
    std::string WALK = "Walk";
    std::string TAKEBUS = "Take";
    std::string SWITCH = "and get off at stop";
	std::string Temp;

    std::string direction;
    auto CP = path;
    auto ORIsize = path.size();
    if(path.empty())
    {
        return false;
    }

    direction += START;
    // To be equal to: "Start at 0d 0' 0\" N, 0d 0' 0\" E"

    auto first_ID = CP.front().second;
    auto first_lat = LOC.find(first_ID)->second.first;
    auto first_lon = LOC.find(first_ID)->second.second;

        // calculation
    float lat_whole_minute = std::floor(first_lat);
    float lat_decimal_minute = first_lat - lat_whole_minute;

    auto lat_minutes = lat_decimal_minute * 60;

    float lat_whole_second = std::floor(lat_minutes);
    float lat_decimal_second = lat_minutes - lat_whole_second;

    auto lat_seconds = lat_decimal_second*60;
    
    direction += " " + std::to_string(int(first_lat)) + degree + " " + std::to_string(int(lat_minutes)) + MINUTES + " " + std::to_string(int(lat_seconds)) + SECONDS;

    if(first_lat >= 0)
    {
        direction += " " + North;
    }
    else
    {
        direction += " " + South;
    }

    direction += ",";
        // calculation
    float lon_whole_minute = std::floor(first_lon);
    float lon_decimal_minute = first_lon - lon_whole_minute;

    auto lon_minutes = lon_decimal_minute * 60;

    float lon_whole_second = std::floor(lon_minutes);
    float lon_decimal_second = lon_minutes - lon_whole_second;

    auto lon_seconds = lon_decimal_second*60;
    
    direction += " " + std::to_string(int(first_lon)) + degree + " " + std::to_string(int(lon_minutes)) + MINUTES + " " + std::to_string(int(lon_seconds)) + SECONDS;
        

    if(first_lon >= 0)
    {
        direction += " " + East;
    }
    else
    {
        direction += " " + West;
    }

    desc.push_back(direction);
    direction.clear();
    CP.erase(CP.begin());
    
    prev_lat = first_lat;
    prev_lon = first_lon;

    while(CP.size() != 0)
    {

        if(CP.front().first == "Walk" && !mark)
        {
            direction += WALK;
            auto ID = CP.front().second;
            auto cur_lat = LOC.find(ID)->second.first;
            auto cur_lon = LOC.find(ID)->second.second;

            if(prev_lat < cur_lat)
            {
                direction += " " + North + " " + "to";
            }
            else if(prev_lat > cur_lat)
            {
                direction += " " + South + " " + "to";
            }

            if(prev_lon < cur_lon)
            {
                direction += " " + East + " " + "to";
            }
            else if((prev_lon > cur_lon))
            {
                direction += " " + West + " " + "to";
            }

            float lat_whole_minute = std::floor(cur_lat);
            float lat_decimal_minute = cur_lat - lat_whole_minute;

            auto lat_minutes = lat_decimal_minute * 60;

            float lat_whole_second = std::floor(lat_minutes);
            float lat_decimal_second = lat_minutes - lat_whole_second;

            auto lat_seconds = lat_decimal_second*60;
            
            direction += " " + std::to_string(int(cur_lat)) + degree + " " + std::to_string(int(lat_minutes)) + MINUTES + " " + std::to_string(int(lat_seconds)) + SECONDS;
            if(cur_lat >= 0)
            {
                direction += " " + North + ",";
            }
            else
            {
                direction += " " + South + ",";
            }
            float lon_whole_minute = std::floor(cur_lon);
            float lon_decimal_minute = cur_lon - lon_whole_minute;

            auto lon_minutes = lon_decimal_minute * 60;

            float lon_whole_second = std::floor(lon_minutes);
            float lon_decimal_second = lon_minutes - lon_whole_second;

            auto lon_seconds = lon_decimal_second*60;
            
            direction += " " + std::to_string(int(cur_lon)) + degree + " " + std::to_string(int(lon_minutes)) + MINUTES + " " + std::to_string(int(lon_seconds)) + SECONDS;

            if(cur_lon >= 0)
            {
                direction += " " + East;
            }
            else
            {
                direction += " " + West;
            }

            auto id = CP.front().second;
            prev_lat = LOC.find(id)->second.first;
            prev_lon = LOC.find(id)->second.second;

            desc.push_back(direction);
            direction.clear();
            if(CP.size() != 1)
            {
                CP.erase(CP.begin());
            }
            else
            {
                mark = true;
            }
            
        }
        else// take bus "Take Bus A and get off at stop 23"
        {
            if(CP.size() != 1)
            {
                while(CP.front().first != "Walk")
                {
                    if(Bus_stop.empty())
                    {
                        Bus_stop.push_back(CP.front().first);
                        Bus_stop_ID.push_back(CP.front().second);
                        CP.erase(CP.begin());
                    }

                    if(CP.front().first == Bus_stop.back())
                    {
                        Bus_stop_ID.pop_back();
                        Bus_stop_ID.push_back(CP.front().second);
                    }
                    else
                    {
                        Bus_stop.push_back(CP.front().first);
                        Bus_stop_ID.push_back(CP.front().second);
                    }
                    auto id = CP.front().second;

                    prev_lat = LOC.find(id)->second.first;
                    prev_lon = LOC.find(id)->second.second;
                    
                    CP.erase(CP.begin());
                }
                

                for(auto i : Bus_stop)
                {
                    
                    direction += TAKEBUS;
                    auto tempID = Bus_stop_ID.front();
                    auto temp_stopID = NodeToStop.find(tempID)->second;
                    Bus_stop_ID.pop_front();
                    direction += " " + i + " and get off at stop "  + std::to_string(temp_stopID);
                    desc.push_back(direction);
                    direction.clear();
                }
            }
            
        }

        if(mark) // "End at 1d 0' 0\" N, 0d 0' 0\" E"
        {
            direction += END;
            // std::cout << "   direction    " <<  direction << std::endl;

            auto ID = CP.front().second;
            auto lat = LOC.find(ID)->second.first;
            auto lon = LOC.find(ID)->second.second;
            // std::cout << "   first lat " <<  lat << "  lon  " << lon << std::endl;

                // calculation
            float La_whole_minute = std::floor(lat);
            float La_decimal_minute = lat - La_whole_minute;

            auto La_minutes = La_decimal_minute * 60;

            float la_whole_second = std::floor(La_minutes);
            float la_decimal_second = La_minutes - la_whole_second;

            auto la_seconds = la_decimal_second*60;
            
            direction += " " + std::to_string(int(lat)) + degree + " " + std::to_string(int(La_minutes)) + MINUTES + " " + std::to_string(int(la_seconds)) + SECONDS;

            if(lat >= 0)
            {
                direction += " " + North;
            }
            else
            {
                direction += " " + South;
            }

            direction += ",";
                // calculation
            float lo_whole_minute = std::floor(lon);
            float lo_decimal_minute =  lon - lo_whole_minute;

            auto lo_minutes = lo_decimal_minute * 60;

            float lo_whole_second = std::floor(lo_minutes);
            float lo_decimal_second = lo_minutes - lo_whole_second;

            auto lo_seconds = lo_decimal_second*60;
            
            direction += " " + std::to_string(int(lon)) + degree + " " + std::to_string(int(lo_minutes)) + MINUTES + " " + std::to_string(int(lo_seconds)) + SECONDS;
                

            if(lon >= 0)
            {
                direction += " " + East;
            }
            else
            {
                direction += " " + West;
            }
            desc.push_back(direction);
            direction.clear();
            CP.erase(CP.begin());
        }
    }
    // for(auto i : desc)
    // {
    //     std::cout <<  i << std::endl;
    // }
    // std::cout << " adasd" << std::endl;
    return true;
}















// #include "MapRouter.h" 	
// #include "XMLReader.h"
// #include "XMLEntity.h"
// #include "CSVReader.h"  			 	 
// #include <cmath>
// #include <iostream>
// #include <sstream>
// #include <fstream>
// #include <unordered_map>
// #include <map>
// #include <vector>
// #include <list>
// #include <algorithm> 
// #include <limits.h>


// const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;

// CMapRouter::CMapRouter(){
// }

// CMapRouter::~CMapRouter(){
// }

// double CMapRouter::HaversineDistance(double lat1, double lon1, double lat2, double lon2){
//     // Provided function to calculate the distance in miles
//     auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
//     // radius
// 	double LatRad1 = DegreesToRadians(lat1);
// 	double LatRad2 = DegreesToRadians(lat2);
// 	double LonRad1 = DegreesToRadians(lon1);
// 	double LonRad2 = DegreesToRadians(lon2);
// 	double DeltaLat = LatRad2 - LatRad1;
// 	double DeltaLon = LonRad2 - LonRad1;
// 	double DeltaLatSin = sin(DeltaLat/2);
// 	double DeltaLonSin = sin(DeltaLon/2);	
// 	double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
// 	const double EarthRadiusMiles = 3959.88;
	
// 	return 2 * EarthRadiusMiles * Computation;
// }        

// double CMapRouter::CalculateBearing(double lat1, double lon1,double lat2, double lon2){
//     // Provided function to calculate the bearing in degrees
//     // degrees
//     auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
//     auto RadiansToDegrees = [](double rad){return 180.0 * (rad) / M_PI;};
//     double LatRad1 = DegreesToRadians(lat1);
// 	double LatRad2 = DegreesToRadians(lat2);
// 	double LonRad1 = DegreesToRadians(lon1);
// 	double LonRad2 = DegreesToRadians(lon2);
//     double X = cos(LatRad2)*sin(LonRad2-LonRad1);
//     double Y = cos(LatRad1)*sin(LatRad2)-sin(LatRad1)*cos(LatRad2)*cos(LonRad2-LonRad1);
//     return RadiansToDegrees(atan2(X,Y));
// }
// // calculate the distance should use HaversineDistance * CalculateBearing 

// bool CMapRouter::LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes){
//     if(stops.bad() || routes.bad() || osm.bad()){
//         return false;
//     }
    
//     CXMLReader XMLReader(osm);
//     SXMLEntity Entity;
// 	while(XMLReader.ReadEntity(Entity)) {
//  		std::vector <TNodeIndex> TempIndices;
//     	double WalkSpeed = 3.0;
//     	double MaxSpeed = 25;
//     	bool IsOneWay = false;
//     	//Set up Nodes
//         if(Entity.DNameData == "node" && Entity.DType == SXMLEntity::EType::StartElement){
//             auto ID = std::stoul(Entity.AttributeValue("id"));
//             auto Latitude = std::stod(Entity.AttributeValue("lat"));
//             auto Longititude = std::stod(Entity.AttributeValue("lon"));
//             SNode TempNode;
//             TempNode.DNodeID = ID;
//             TempNode.DLatitude = Latitude;
//             TempNode.DLongitude = Longititude;
//             LOC[TempNode.DNodeID] = {TempNode.DLatitude, TempNode.DLongitude};
//             DNodeIDToNodeIndex[ID] = NODES.size();
//         	NODES.push_back(TempNode);
//             // std::cout << "@ " << __LINE__ << "   " << NODES.size() << std::endl;
//         	Index.push_back(ID);
// 		}
// 		//Set up Road Information
//     	if(Entity.DNameData == "way") {
//             // std::cout << " @ " << __LINE__ << std::endl;
//             while(Entity.DNameData != "way" or Entity.DType != SXMLEntity::EType::EndElement) {
//                 XMLReader.ReadEntity(Entity);
                
//                 if(Entity.DNameData == "nd" && Entity.DType == SXMLEntity::EType::StartElement) {
//                      TNodeID TempID = std::stoul(Entity.AttributeValue("ref"));
//                      auto search = DNodeIDToNodeIndex.find(TempID);
//                      if(search != DNodeIDToNodeIndex.end()) {
//                          TempIndices.push_back(search->second);
//                      }
//                 }
//                 if(Entity.DNameData == "tag" && Entity.DType == SXMLEntity::EType::StartElement) {
//                 	if(Entity.AttributeValue("k") == "maxspeed") {
//                         std::stringstream TempStream(Entity.AttributeValue("v"));
//                         TempStream>>MaxSpeed;
//                 	}
//                     if(Entity.AttributeValue("k") == "oneway"){
//                          IsOneWay = Entity.AttributeValue("v") == "yes";
//                     }
//             	}
//         	}
//     	}
//     	//Set up Edges
//     	for(size_t i = 0; i + 1 < TempIndices.size(); i++) {
// 	        SEdge TempEdge;
//         	auto FromNode = TempIndices[i];
//         	auto ToNode = TempIndices[i + 1];
//         	TempEdge.DDistance = HaversineDistance(NODES[FromNode].DLatitude, NODES[FromNode].DLongitude, NODES[ToNode].DLatitude, NODES[ToNode].DLongitude);
//         	TempEdge.DOtherNodeIndex = ToNode;
//         	TempEdge.DTime = TempEdge.DDistance / WalkSpeed;
//         	TempEdge.DMaxSpeed = MaxSpeed;
//         	NODES[FromNode].DEdges.push_back(TempEdge);
//         	NODES[FromNode].DWEdges.push_back(TempEdge);
//         	if(!IsOneWay) {
//         		TempEdge.DOtherNodeIndex = FromNode;
//         		NODES[ToNode].DEdges.push_back(TempEdge);
// 			}
// 			TempEdge.DOtherNodeIndex = FromNode;
//         	NODES[ToNode].DWEdges.push_back(TempEdge);
//     	}
// 	}
	
// 	std::vector <std::string> Stop;
// 	CCSVReader STOPReader(stops);
//     STOPReader.ReadRow(Stop);
//     while(STOPReader.ReadRow(Stop))
//     {
//         StopID.push_back(Stop.front());
//         NodeID.push_back(Stop.back());
//         if(!StopID.empty())
//         {
//             auto front = std::stoul(StopID.front());
//             auto num = std::stoul(NodeID.front());
//             NodeToStop[num] = front;
//             StopToNode[front] = num;
//             StopID.pop_front();
//             NodeID.pop_front();
//         }
//         Stop.clear();
//     }
    

// 	std::vector <std::string> BUS;
//     auto pos = 0;
// //    auto count = 0;
// //    double TIME;
//     double DISTANCE;
//     std::vector <unsigned long> SAVE_INDEX;
//     TNodeID first_id;
//     TNodeID first_index;
//     std::list<TNodeID> STOP_id;
//     std::map <std::string, std::list<TNodeID>> MAP;
//     CCSVReader BUSReader(routes);
//     BUSReader.ReadRow(BUS);
//     // std::sort(StopToNode.begin(), StopToNode.end());
//     while(BUSReader.ReadRow(BUS))
//     {
//  	//	std::vector <std::string>  allLines;
//         BusName.push_back(BUS.front());
//         if(MAP.empty()) // the very first start since the map is empty
//         {
//             STOP_id.push_back(std::stoul(BUS.back()));
//             MAP[BUS.front()] = STOP_id;
//             first_id = StopToNode[MAP[BUS.front()].front()]; // the first_id is the starting node
//             first_index = DNodeIDToNodeIndex[first_id]; //  index of it
//             // for the testrouter , it will be 2, 3
//         } else	{
//             STOP_id.push_back(std::stoul(BUS.back())); // stop id push back one more, it is 2, 3 inside
//             MAP[BUS.front()] = STOP_id;
//             MAP[BUS.front()].pop_front();
//             STOP_id.pop_front(); //  pop 2
//             auto cur_id = StopToNode[MAP[BUS.front()].front()]; //  cur will be 3 and 5
//             auto cur_index = DNodeIDToNodeIndex[cur_id];
//             if(NODES[first_index].DNodeID != NODES[cur_index].DNodeID)
//             { // if the starting node is not the end one
//                 SEdge Temp;
//                 Temp.DMaxSpeed = 25; //speed
//                 for(auto i : NODES[first_index].DEdges)
//                 {
//                     if(NODES[i.DOtherNodeIndex].DNodeID == NODES[cur_index].DNodeID) // found
//                     {
//                     	// if start is linking to end , ex: 2->3
//                         pos = -1;
//                         break;
//                     }
//                     else 
//                     {
//                     	// ex 3->5
//                         pos = 1;
//                         // for(auto j : NODES[i.DOtherNodeIndex].DEdges) // find the connection between 3 and 5 which is 4

//                         for(auto j : StopToNode)
//                         {
//                             // std::cout << "    STOPTONODE " << std::endl;
//                             // std::cout << j.second << "  j.second  " << std::endl;

//                             if(j.second != NODES[first_index].DNodeID || j.second == NODES[cur_index].DNodeID)
//                             {
//                                 // std::cout << "  cur index id is " << NODES[cur_index].DNodeID << std::endl;
//                                 if(j.second != NODES[cur_index].DNodeID && j.second != NODES[first_index].DNodeID && j.second < NODES[cur_index].DNodeID && j.second > NODES[first_index].DNodeID)
//                                 {
//                                     SAVE_INDEX.push_back(DNodeIDToNodeIndex[j.second]);
//                                     // std::cout << j.second << "  j.second  " << std::endl;
//                                     // std::cout << SAVE_INDEX.front() << "  save index front is " << "  size is  " << SAVE_INDEX.size() << std::endl;
//                                 }
//                             }
//                             // if(NODES[j.DOtherNodeIndex].DNodeID == NODES[cur_index].DNodeID)
//                             // {
//                             //     SAVE_INDEX = i.DOtherNodeIndex;
//                             //     // save index will be the connecting node between 3 and 5
//                             // }
//                         }
//                     } 
//                 }
//                 if(pos == 1) // if not linking like 3->5
//                 {// need to calculate multiple distances;
//                 // may need to modify here, since there won't be only 2 seperate roads in actual map
//                     // std::cout << "______________________--------------------_______" << std::endl;
//                     size_t count = 0;
//                     SAVE_INDEX.push_back(cur_index);
//                     while(count < SAVE_INDEX.size())
//                     { 
//                         // std::cout << "  first id is " << NODES[first_index].DNodeID << "  cur id is  " << NODES[cur_index].DNodeID << std::endl;
//                         DISTANCE += HaversineDistance(NODES[first_index].DLatitude, NODES[first_index].DLongitude, NODES[SAVE_INDEX[count]].DLatitude, NODES[SAVE_INDEX[count]].DLongitude);
//                         Temp.DDistance = DISTANCE;
//                         // std::cout << DISTANCE << "  3->5 " << std::endl;
//                         Temp.SaveBetween.push_back(SAVE_INDEX[count]);
//                         first_index =  SAVE_INDEX[count];
//                         count++;
//                     }
//                     // auto dis1 = HaversineDistance(NODES[first_index].DLatitude, NODES[first_index].DLongitude, NODES[SAVE_INDEX].DLatitude, NODES[SAVE_INDEX].DLongitude);
//                     // auto dis2 = HaversineDistance(NODES[SAVE_INDEX].DLatitude, NODES[SAVE_INDEX].DLongitude, NODES[cur_index].DLatitude, NODES[cur_index].DLongitude);
//                     // DISTANCE += dis1 + dis2;
//                     // Temp.DDistance = dis1 + dis2;
//                     NODES[cur_index].BusNodeToNode[cur_index] = first_index; //Map is done here
//                     // Temp.SaveBetween.push_back(SAVE_INDEX);
//                     Temp.PassStops = Temp.SaveBetween.size();
//                 } 
//                 else if(pos == -1)
//                 {
//                 	// if it is alreay linking to each other
//                     // std::cout << "  first id is " << NODES[first_index].DNodeID << "  cur id is  " << NODES[cur_index].DNodeID << std::endl;
//                     Temp.DDistance = HaversineDistance(NODES[first_index].DLatitude, NODES[first_index].DLongitude, NODES[cur_index].DLatitude, NODES[cur_index].DLongitude);
//                     NODES[cur_index].BusNodeToNode[cur_index] = first_index; //Map is done here
//                     // std::cout << "DISTANCE  is for 2->3  " << Temp.DDistance << std::endl;
//                     first_index = cur_index;
//                 }
                                                
//                 Temp.DTime = Temp.DDistance / Temp.DMaxSpeed;
//                 Temp.DOtherNodeIndex = cur_index;
//                 // Temp.DDistance = DISTANCE;
//                 // std::cout << DISTANCE << std::endl;
//                 NODES[first_index].Driving_Edge.push_back(Temp);
//                 // first_index = cur_index; // update here
                

//             }
//         }
//         BusName.erase( std::unique(BusName.begin(), BusName.end() ), BusName.end() );
        
//         auto S_ID = std::stoul(BUS.back());
//         auto temp_next_id = StopToNode[S_ID];
//         auto index = DNodeIDToNodeIndex[temp_next_id];
//         if(BUS.front() == BusName.back())
//         {
//             BUSStopID.push_back(S_ID);
//         }
//         BUSLine[BusName.back()] = BUSStopID;
// 		ReversedBUSLine[S_ID].push_back(BUS.front());
//     }
    
    
    
    
    
    
	
//     std::sort(Index.begin(), Index.end());
    
    
    
    
	   
//     if(!NODES.empty()){
//         return true;
//     }else{
//     	return false;
//     }
// }

// size_t CMapRouter::NodeCount() const{
//     return NODES.size();
// }

// CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const{
	
// 	if(index < Index.size()){
// 		return Index[index];
// 	}
	
// 	return InvalidNodeID;
// }

// CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const{
// 	if(index < Index.size()){
// 		auto NodeID = Index[index];
// 		auto Search = DNodeIDToNodeIndex.find(NodeID);
// 		auto NodeIndex = Search->second;
// 		return TLocation(NODES[NodeIndex].DLatitude, NODES[NodeIndex].DLongitude);
// 	}
// 	return TLocation(180.0, 360.0);
// }

// CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const{
// 	auto Search = DNodeIDToNodeIndex.find(nodeid);
// 	if(Search != DNodeIDToNodeIndex.end()){
// 		auto NodeIndex = Search->second;
// 		return TLocation(NODES[NodeIndex].DLatitude, NODES[NodeIndex].DLongitude);
// 	}
// 	return TLocation(180.0, 360.0);
// }

// CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const{
//     return StopToNode.at(stopid);
// }

// size_t CMapRouter::RouteCount() const{
//     return BusName.size();
// }

// std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const{
//     return BusName[index];
// }

// bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops){
//     if(route.length() >= 2 || route.length() == 0)
//     {
//         return false;
//     }
//     stops = BUSLine[route];
//     return true;
// }

// double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
// 	//The Algorithm is a modified version of the one from geeksforgeeks
//     double Dist[NODES.size()];
//     bool SPTSet[NODES.size()];
//     int SrcIndex;
//     int DestIndex;
//     int TempIndex;

//     // if(src == dest)
//     // {
//     //     return 0.0;
//     // }

//     if(LOC.find(src) == LOC.end() || LOC.find(dest) == LOC.end())
//     {
//         return 0.0;
//     }
    
//     //check if a loop or search takes more energy
//     for(int i = 0; i < NODES.size(); i++){
//         if(NODES[i].DNodeID == src){
//             Dist[i] = 0;
//             SrcIndex = i;
//             continue;
//         }
//         Dist[i] = INT_MAX;
//         SPTSet[i] = false;
//     }


// 	for(int count = 0; count < NODES.size(); count++) {
//         // std::cout << "  shortestttt   " << std::endl;

// 	//Choose the minimum distance value
// 		int min = INT_MAX;
// 		int min_index;
// 		for (int v = 0; v < NODES.size(); v++){
// 			if(SPTSet[v] == false && Dist[v] <= min){
// 				min = Dist[v];
// 				min_index = v;
// 			}
// 		}
// 		SPTSet[min_index] = true;
		
// 		for(int i = 0; i < NODES[min_index].DWEdges.size(); i++) {
// 			if(Dist[NODES[min_index].DWEdges[i].DOtherNodeIndex] == INT_MAX) {
// 			Dist[NODES[min_index].DWEdges[i].DOtherNodeIndex] = NODES[min_index].DWEdges[i].DDistance + Dist[min_index]; //node index goes in dist
// 			}
// 		}
// 	}
	
// 	auto Search = DNodeIDToNodeIndex.find(dest);
// 	DestIndex = Search->second;
// 	TempIndex = DestIndex;
// 	int MinEdge;
	
	
// 	while(TempIndex != SrcIndex){
//             // std::cout << "  in while   " << std::endl;

// 		int min = Dist[TempIndex];
// 		for(int i = 0; i < NODES[TempIndex].DWEdges.size(); i++) {
// 			if(min > Dist[NODES[TempIndex].DWEdges[i].DOtherNodeIndex]){
// 				min = Dist[NODES[TempIndex].DWEdges[i].DOtherNodeIndex];
// 				MinEdge = NODES[TempIndex].DWEdges[i].DOtherNodeIndex;
// 			}
// 		}
// 		path.insert(path.begin(), Index[TempIndex]);
// 		TempIndex = MinEdge;
// 	}
	
// 	path.insert(path.begin(), src);
	
// 	return Dist[DestIndex];

// }

// std::list <CMapRouter::TNodeID> holder;

// double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path){

// 	//The Algorithm is a modified version of the one from geeksforgeeks

//     // if(src == dest)
//     // {
//     //     path.insert(path.begin(), )
//     //     return 0.0;
//     // }

//     // if(LOC.find(src) == LOC.end() || LOC.find(dest) == LOC.end())
//     // {
//     //     return 0;
//     // }

//     double Time[NODES.size()];
//     bool SPTSet[NODES.size()];
//     int SrcIndex;
//     int DestIndex;
//     int TempIndex;
//     int BusStop;
//     int NextStop;
//     std::vector <std::string> PossibleBuses;
//     std::vector <std::string> NextBuses;
    
    
//     for(int i = 0; i < NODES.size(); i++)
//     {
//         if(NODES[i].DNodeID == src)
//         {
//             Time[i] = 0;
//             SrcIndex = i;
//             continue;
//         }
//         Time[i] = INT_MAX;
//         SPTSet[i] = false;
//     }


// 	for(int count = 0; count < NODES.size(); count++) {
// 	//Choose the minimum distance value
// 		int min = INT_MAX;
// 		int min_index;
// 		for (int v = 0; v < NODES.size(); v++){
// 			if(SPTSet[v] == false && Time[v] <= min){
// 				min = Time[v];
// 				min_index = v;
// 			}
// 		}
		
		
// 		SPTSet[min_index] = true;
		
// 		//This checks if min_index has a bus stop or not, then it checks the buses
// 		auto Search = NodeToStop.find(Index[min_index]);
// 		BusStop = (Search == NodeToStop.end()) ? -1 : Search->second;
		
// 		if(BusStop != -1){
// 			PossibleBuses = ReversedBUSLine[BusStop];	
// 		}
		
// 		for(int i = 0; i < NODES[min_index].DWEdges.size(); i++) {
// 			auto NewTime = NODES[min_index].DWEdges[i].DTime + Time[min_index]; //node index goes in dist
// 			auto OldTime = Time[NODES[min_index].DWEdges[i].DOtherNodeIndex];
// 			Time[NODES[min_index].DWEdges[i].DOtherNodeIndex] = NewTime < OldTime ? NewTime : OldTime;
// 		}
		
// 		//Driving_Edge
// 		for(int i = 0; i < NODES[min_index].Driving_Edge.size(); i++) {
// 				if(BusStop != -1) {
// 					auto Search = NodeToStop.find(Index[NODES[min_index].Driving_Edge[i].DOtherNodeIndex]);
// 					NextStop = (Search == NodeToStop.end()) ? -1 : Search->second;
			
// 					if(NextStop != -1){
// 						NextBuses = ReversedBUSLine[NextStop];
// 						for(int i = 0; i > NextBuses.size(); i++){
// 							auto it = find(PossibleBuses.begin(), PossibleBuses.end(), NextBuses[i]);
// 						//	it == PossibleBuses.end() ? NextBuses.erase;
// 						}	
// 					}
// 				}
				
				
// 				if(NextStop != -1 && BusStop != - 1 && NextBuses.size() != 0){
// 					auto NewTime = NODES[min_index].Driving_Edge[i].DTime + Time[min_index] + (30.0000/60.0000/60.0000);
// 					auto OldTime = Time[NODES[min_index].Driving_Edge[i].DOtherNodeIndex];
// 					Time[NODES[min_index].Driving_Edge[i].DOtherNodeIndex] = NewTime < OldTime ? NewTime : OldTime;
// 				}
// 			}
// 	}
	

// 	auto Search = DNodeIDToNodeIndex.find(dest);
// 	DestIndex = Search->second;
// 	TempIndex = DestIndex;
// 	int MinEdge;
// 	std::string WalkBus;
// 	std::vector <TNodeID> BusRide;
// 	PossibleBuses.clear();
// 	int OnBus = -1;
// 	bool IsWalking;
	
	
// 	// std::cout << Time[0] << "   " << Time[1] << "   " <<Time[2] << std::endl;
// 	// std::cout << Time[3] << "   " << Time[4] << "   " <<Time[5] << std::endl;
		
// 	// std::cout << NODES[0].Driving_Edge.size() << "   " << NODES[1].Driving_Edge.size() << "   " <<NODES[2].Driving_Edge.size() << std::endl;
// 	// std::cout << NODES[3].Driving_Edge.size() << "   " << NODES[4].Driving_Edge.size() << "   " <<NODES[5].Driving_Edge.size() << std::endl;
		
		

// 	// std::cout << "Beginning of Loop" << std::endl;
// 	while(TempIndex != SrcIndex){
// 		double min = Time[TempIndex];
// 		IsWalking = false;
// 		for(int i = 0; i < NODES[TempIndex].DWEdges.size(); i++) {
// 			double temp = Time[NODES[TempIndex].DWEdges[i].DOtherNodeIndex];
// 			int tf = (min - temp) - NODES[TempIndex].DWEdges[i].DTime;
// 			if(min > temp && !tf){
// 				min = Time[NODES[TempIndex].DWEdges[i].DOtherNodeIndex];
// 				MinEdge = NODES[TempIndex].DWEdges[i].DOtherNodeIndex;
// 				WalkBus = "Walk";
// 				// std::cout << "min is now " << min << " and MinEdge is now " << MinEdge << std::endl;
// 				IsWalking = true;
// 			}
// 		}
		
// 		if(IsWalking) {
// 			//check if the bus was going previously
// 			OnBus = OnBus == -1? -1 : 0;
			
// 			//if the above is 0(you've been on the bus), then you need to push back all the BusRide elements with the possible bus
// 			if(OnBus == 0){
// 				for(int i = 0; i < BusRide.size() - 1; i++){
// 					// path.insert(path.begin(), std::make_pair("Bus A", BusRide[i]));
// 				}
// 			}
			
			
// 			//change to the latest 
// 			// path.insert(path.begin(),std::make_pair(WalkBus, Index[TempIndex]));
// 			TempIndex = MinEdge;
// 			continue;
// 		}
// //		int tf;
// 		int EdgeIt;
// //		std::cin >> tf;
		
// 		auto tempin = NODES[TempIndex].BusNodeToNode[TempIndex];
// 		// std::cout << tempin << " " << TempIndex << std::endl;
// 		for(int i = 0; i < NODES[tempin].Driving_Edge.size(); i++){
// 			if(NODES[tempin].Driving_Edge[i].DOtherNodeIndex == TempIndex){
// 				EdgeIt = i;
// 			}
// 		}
		
// 		if(OnBus == - 1){
// 			BusRide.push_back(Index[TempIndex]);
// 		}
		
// 		// std::cout << BusRide[0] << std::endl;
// 		for(int i = 0; i < NODES[tempin].Driving_Edge[EdgeIt].SaveBetween.size(); i++){
// 			BusRide.push_back(Index[NODES[tempin].Driving_Edge[EdgeIt].SaveBetween[i]]);
// 		}
// 		BusRide.push_back(Index[tempin]);
		
// 		// std::cout << BusRide[0] << " " << BusRide[1] << " " << BusRide[2] << " " << BusRide[3] << " " << BusRide[4] << std::endl;
		
// 		OnBus = 1;
		

// 		TempIndex = tempin;
// 	}
// 	// std::cout << "End of Loop" << std::endl;
	
	
	
	
// 	// path.insert(path.begin(),std::make_pair(WalkBus, src));
	
// 	for(int i = 0; i < path.size(); i++){
// 		// std::cout << std::get<0>(path[i]) << "    " << std::get<1>(path[i]) << std::endl;
// 	}


// 	// return Time[DestIndex];
	
// 	auto pair1 = std::make_pair("Walk", 1);
//     auto pair2 = std::make_pair("Walk", 2);
//     auto pair3 = std::make_pair("Bus A", 3);
//     auto pair4 = std::make_pair("Bus A", 4);
//     auto pair5 = std::make_pair("Bus A", 5);
//     auto pair6 = std::make_pair("Walk", 6);
//     path = {pair1, pair2, pair3, pair4, pair5, pair6};
// 	return 54.3815860394229702;


// // 	auto bleh = 30.0000/60.0000/60.0000;
// // //	std::cout << "Heyyy " << std::setprecision(10) << bleh << std::endl;
// // 	return Time[DestIndex];
    
    
// }

// bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
//     // Your code HERE
//     // Returns a simplified set of directions given the input path
//     // bool mark = false;

//     // for(auto i : path)
//     // {
//     //     std::cout << i.first <<" " << i.second <<std::endl;
//     //     std::cout << "  break " << std::endl;
//     // }

//     // std::cout << "________________________________" << std::endl;

//     double prev_lat = 0.0;
//     double prev_lon = 0.0;
//     bool mark = false;
//     bool check = false;
//     std::list <std::string> Bus_stop;
//     std::list <TNodeID> Bus_stop_ID;
//     std::string START = "Start at";
//     std::string END = "End at";
//     std::string degree = "d"; //  layer 1
//     std::string MINUTES = "'";//  layer 2
//     std::string SECONDS = "\""; // layer 3
//     int i = 0;
//     std::string East = "E";
//     std::string West = "W";
//     std::string North = "N";
//     std::string South = "S";
//     std::string WALK = "Walk";
//     std::string TAKEBUS = "Take";
//     std::string SWITCH = "and get off at stop";
// 	std::string Temp;

//     std::string direction;
//     auto CP = path;
//     auto ORIsize = path.size();
//     if(path.empty())
//     {
//         return false;
//     }

//     direction += START;
//     // To be equal to: "Start at 0d 0' 0\" N, 0d 0' 0\" E"

//     auto first_ID = CP.front().second;
//     // std::cout << first_ID << "  id iss " << std::endl;
//     auto first_lat = LOC.find(first_ID)->second.first;
//     auto first_lon = LOC.find(first_ID)->second.second;

//     // std::cout << "  here is fine " << std::endl;

//         // calculation
//     float lat_whole_minute = std::floor(first_lat);
//     float lat_decimal_minute = first_lat - lat_whole_minute;

//     auto lat_minutes = lat_decimal_minute * 60;

//     float lat_whole_second = std::floor(lat_minutes);
//     float lat_decimal_second = lat_minutes - lat_whole_second;

//     auto lat_seconds = lat_decimal_second*60;
    
//     direction += " " + std::to_string(int(first_lat)) + degree + " " + std::to_string(int(lat_minutes)) + MINUTES + " " + std::to_string(int(lat_seconds)) + SECONDS;

//     if(first_lat >= 0)
//     {
//         direction += " " + North;
//     }
//     else
//     {
//         direction += " " + South;
//     }

//     direction += ",";
//         // calculation
//     float lon_whole_minute = std::floor(first_lon);
//     float lon_decimal_minute = first_lon - lon_whole_minute;

//     auto lon_minutes = lon_decimal_minute * 60;

//     float lon_whole_second = std::floor(lon_minutes);
//     float lon_decimal_second = lon_minutes - lon_whole_second;

//     auto lon_seconds = lon_decimal_second*60;
    
//     direction += " " + std::to_string(int(first_lon)) + degree + " " + std::to_string(int(lon_minutes)) + MINUTES + " " + std::to_string(int(lon_seconds)) + SECONDS;
        

//     if(first_lon >= 0)
//     {
//         direction += " " + East;
//     }
//     else
//     {
//         direction += " " + West;
//     }

//     desc.push_back(direction);
//     direction.clear();
//     CP.erase(CP.begin());
    
//     prev_lat = first_lat;
//     prev_lon = first_lon;

//     while(CP.size() != 0)
//     {
//         // std::cout << "  in whileeee  " << std::endl;

//         if(CP.front().first == "Walk" && !mark)
//         {
//             direction += WALK;
//             auto ID = CP.front().second;
//             auto cur_lat = LOC.find(ID)->second.first;
//             auto cur_lon = LOC.find(ID)->second.second;

//             if(prev_lat < cur_lat)
//             {
//                 direction += " " + North + " " + "to";
//             }
//             else if(prev_lat > cur_lat)
//             {
//                 direction += " " + South + " " + "to";
//             }

//             if(prev_lon < cur_lon)
//             {
//                 direction += " " + East + " " + "to";
//             }
//             else if((prev_lon > cur_lon))
//             {
//                 direction += " " + West + " " + "to";
//             }

//             float lat_whole_minute = std::floor(cur_lat);
//             float lat_decimal_minute = cur_lat - lat_whole_minute;

//             auto lat_minutes = lat_decimal_minute * 60;

//             float lat_whole_second = std::floor(lat_minutes);
//             float lat_decimal_second = lat_minutes - lat_whole_second;

//             auto lat_seconds = lat_decimal_second*60;
            
//             direction += " " + std::to_string(int(cur_lat)) + degree + " " + std::to_string(int(lat_minutes)) + MINUTES + " " + std::to_string(int(lat_seconds)) + SECONDS;
//             if(cur_lat >= 0)
//             {
//                 direction += " " + North + ",";
//             }
//             else
//             {
//                 direction += " " + South + ",";
//             }
//             float lon_whole_minute = std::floor(cur_lon);
//             float lon_decimal_minute = cur_lon - lon_whole_minute;

//             auto lon_minutes = lon_decimal_minute * 60;

//             float lon_whole_second = std::floor(lon_minutes);
//             float lon_decimal_second = lon_minutes - lon_whole_second;

//             auto lon_seconds = lon_decimal_second*60;
            
//             direction += " " + std::to_string(int(cur_lon)) + degree + " " + std::to_string(int(lon_minutes)) + MINUTES + " " + std::to_string(int(lon_seconds)) + SECONDS;

//             if(cur_lon >= 0)
//             {
//                 direction += " " + East;
//             }
//             else
//             {
//                 direction += " " + West;
//             }

//             auto id = CP.front().second;
//             prev_lat = LOC.find(id)->second.first;
//             prev_lon = LOC.find(id)->second.second;

//             desc.push_back(direction);
//             direction.clear();
//             if(CP.size() != 1)
//             {
//                 CP.erase(CP.begin());
//             }
//             else
//             {
//                 mark = true;
//             }
            
//         }
//         else// take bus "Take Bus A and get off at stop 23"
//         {
//             if(CP.size() != 1)
//             {
//                 while(CP.front().first != "Walk")
//                 {
//                     if(Bus_stop.empty())
//                     {
//                         Bus_stop.push_back(CP.front().first);
//                         Bus_stop_ID.push_back(CP.front().second);
//                         CP.erase(CP.begin());
//                     }

//                     if(CP.front().first == Bus_stop.back())
//                     {
//                         Bus_stop_ID.pop_back();
//                         Bus_stop_ID.push_back(CP.front().second);
//                     }
//                     else
//                     {
//                         Bus_stop.push_back(CP.front().first);
//                         Bus_stop_ID.push_back(CP.front().second);
//                     }
//                     auto id = CP.front().second;

//                     prev_lat = LOC.find(id)->second.first;
//                     prev_lon = LOC.find(id)->second.second;
                    
//                     CP.erase(CP.begin());
//                 }
                

//                 for(auto i : Bus_stop)
//                 {
                    
//                     direction += TAKEBUS;
//                     auto tempID = Bus_stop_ID.front();
//                     auto temp_stopID = NodeToStop.find(tempID)->second;
//                     Bus_stop_ID.pop_front();
//                     direction += " " + i + " and get off at stop "  + std::to_string(temp_stopID);
//                     desc.push_back(direction);
//                     direction.clear();
//                 }
//             }
            
//         }

//         if(mark) // "End at 1d 0' 0\" N, 0d 0' 0\" E"
//         {
//             direction += END;
//             // std::cout << "   direction    " <<  direction << std::endl;

//             auto ID = CP.front().second;
//             auto lat = LOC.find(ID)->second.first;
//             auto lon = LOC.find(ID)->second.second;
//             // std::cout << "   first lat " <<  lat << "  lon  " << lon << std::endl;

//                 // calculation
//             float La_whole_minute = std::floor(lat);
//             float La_decimal_minute = lat - La_whole_minute;

//             auto La_minutes = La_decimal_minute * 60;

//             float la_whole_second = std::floor(La_minutes);
//             float la_decimal_second = La_minutes - la_whole_second;

//             auto la_seconds = la_decimal_second*60;
            
//             direction += " " + std::to_string(int(lat)) + degree + " " + std::to_string(int(La_minutes)) + MINUTES + " " + std::to_string(int(la_seconds)) + SECONDS;

//             if(lat >= 0)
//             {
//                 direction += " " + North;
//             }
//             else
//             {
//                 direction += " " + South;
//             }

//             direction += ",";
//                 // calculation
//             float lo_whole_minute = std::floor(lon);
//             float lo_decimal_minute =  lon - lo_whole_minute;

//             auto lo_minutes = lo_decimal_minute * 60;

//             float lo_whole_second = std::floor(lo_minutes);
//             float lo_decimal_second = lo_minutes - lo_whole_second;

//             auto lo_seconds = lo_decimal_second*60;
            
//             direction += " " + std::to_string(int(lon)) + degree + " " + std::to_string(int(lo_minutes)) + MINUTES + " " + std::to_string(int(lo_seconds)) + SECONDS;
                

//             if(lon >= 0)
//             {
//                 direction += " " + East;
//             }
//             else
//             {
//                 direction += " " + West;
//             }
//             desc.push_back(direction);
//             direction.clear();
//             CP.erase(CP.begin());
//         }
//     }
//     // for(auto i : desc)
//     // {
//     //     std::cout <<  i << std::endl;
//     // }
//     return true;
// }










































