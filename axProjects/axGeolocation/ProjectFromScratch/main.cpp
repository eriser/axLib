#include "main.h"
#include "axUrlApi.h"
#include "rapidxml.hpp"
#include "axDatabase.h"

#include <unistd.h>
#include <sys/types.h>


axVectorPairString GetGeoLocationFromIP(const std::string& ip)
{
    axUrlApi* url_api = axUrlApi::GetInstance();

    std::string key = "c19950934a2aeef36fb8314ebdfb2b22df8c16f9529da84ff98945011d011cf0";
    std::string format = "xml";
    std::string geoRequest = "http://api.ipinfodb.com/v3/ip-city/?key=" +
    key + "&ip=" + ip + "&format=" + format;
    
    std::string answer = url_api->GetUrlData(geoRequest);
    
    rapidxml::xml_document<char> doc;
    doc.parse<0>(&answer[0]);
    
    rapidxml::xml_node<>* objNode = doc.first_node("Response");
    objNode = objNode->first_node();
    
    axVectorPairString objData;
    
    while(objNode)
    {
        objData.push_back(axStringPair(objNode->name(), objNode->value()));
        objNode = objNode->next_sibling();
    }
    
    return objData;
}

axFloatPoint GetCoordinates(const axVectorPairString& attributes)
{
    axFloatPoint pos(0.0, 0.0);
    
    for(auto& n : attributes)
    {
        if(n.first == "latitude")
        {
            pos.y = stod(n.second);
        }
        else if(n.first == "longitude")
        {
            pos.x = stod(n.second);
        }
    }
    
    return pos;
}


std::vector<axFloatPoint> GetVectorOfCoord(const axStringVector& ips)
{
    std::vector<axFloatPoint> points;
    
    std::string ip;
    axVectorPairString loc_info;
    
    for(auto& n : ips)
    {
        ip = n;
        loc_info = GetGeoLocationFromIP(ip);
        points.push_back(GetCoordinates(loc_info));
    }
    
    return points;
}

axStringVector GetLastWebHistory(const int& nWebHistoryToShow)
{
    std::string home_dir = getenv("HOME");
    axDatabase dataBase(home_dir + "/Library/Safari/History.db");
    axVectorPairStringVector history = dataBase.Request("SELECT * from history_items");

    axStringVector web_history;
    
    if(history.size() > nWebHistoryToShow)
    {
        int start_index = history.size() - nWebHistoryToShow - 1;
        for(int i = start_index; i < history.size(); i++)
        {
            for(auto& n : history[i])
            {
                if(n.first == "url")
                {
                    web_history.push_back(n.second);
                }
            }
        }
    }

    return web_history;
}

// traceroute -n -w 4 -q 1 www.google.com
void axMain::MainEntryPoint(axApp* app)
{
    
    axStringVector web_history = GetLastWebHistory(10);
    
    for(auto& n : web_history)
    {
        std::cout << n << std::endl;
    }
    
//    axStringVector listOfIP;
//    
//    if (fork() == 0)
//    {
//        execl("traceroute -n -w 4 -q 1 www.yahoo.com", NULL);
//        
//        close(1);
//    }
//    else
//    {
//
//    }

    // -------------------------------------------------------------------------
    std::ifstream input( "ip.txt" );
    axStringVector ip;
    
    for(std::string line; getline(input, line);)
    {
        axStringVector vec(GetVectorFromStringDelimiter(line, " "));
        if(vec.size() == 7)
        {
            std::cout << vec[3] << std::endl;
            ip.push_back(vec[3]);
        }
    }

    std::vector<axFloatPoint> points = GetVectorOfCoord(ip);
    WorldMap* worldMap = new WorldMap();
    worldMap->SetCoordinates(points);
}


WorldMap::WorldMap() :
axPanel(nullptr, axRect(0, 0, 1029, 518))
{
    _worldMap = new axImage(axApp::GetAppPath() + "ressources/images/World.png");
}

void WorldMap::SetCoordinates(const std::vector<axFloatPoint>& coord)
{
    _coordVector = coord;
    Update();
}

void WorldMap::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(1.0, 1.0, 1.0, 1.0));
    gc->DrawRectangle(rect);
    
    gc->DrawImageResize(_worldMap, axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(1.0, 0.0, 0.0, 1.0));
    
    bool first = true;
    axPoint past_pos;
    
    for(auto& n : _coordVector)
    {
        if(!(n.x == 0.0 && n.y == 0.0))
        {
            double coor_x = n.x; //-73.5878;
            double coor_y = n.y; //45.5088;
            
            coor_x /= 180.0;
            coor_y /= -90.0;
            
            axPoint pos;
            pos.x = rect.size.x * 0.5 + coor_x * (rect.size.x * 0.5 - 5);
            pos.y = rect.size.y * 0.5 + coor_y * (rect.size.y * 0.5 - 5);
            
            gc->DrawPoint(pos, 5);
            
            if(!first)
            {
                gc->DrawLine(past_pos, pos);
            }
            
            first = false;
            past_pos = pos;
        }
    }
}

