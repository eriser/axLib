/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#include "axObjectLoader.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>

#include "axButton.h"
#include "axNumberBox.h"
#include "axKnob.h"
#include "axLabel.h"
#include "axStaticImage.h"

using namespace rapidxml;

axWidgetLoader::axWidgetLoader()
{
    
}

axVectorPairString axWidgetLoader::GetAttributes(const std::string& path)
{
    std::ifstream t(path);
    std::string xml_str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
    
    std::vector<char> xml_copy(xml_str.begin(), xml_str.end());
    xml_copy.push_back('\0');
    
    xml_document<char> doc;    // character type defaults to char
    doc.parse<0>(&xml_copy[0]);    // 0 means default parse flags
    
    std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
    xml_node<>* objNode = doc.first_node(0);
    
//    xml_node<>* objNode = node->first_node();
    
    axVectorPairString objData;
    
    for (xml_attribute<>* attr = objNode->first_attribute();
         attr; attr = attr->next_attribute())
    {
        objData.push_back(std::pair<std::string, std::string>
                          (attr->name(), attr->value()));
    }
    return objData;
}

axObjectLoader::axObjectLoader(axPanel* win, const std::string& path)
{
    std::ifstream t(path);
    std::string xml_str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
    
    std::vector<char> xml_copy(xml_str.begin(), xml_str.end());
    xml_copy.push_back('\0');
    
    xml_document<char> doc;    // character type defaults to char
    doc.parse<0>(&xml_copy[0]);    // 0 means default parse flags
    
    std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
    xml_node<>* node = doc.first_node(0);
    
    xml_node<>* objNode = node->first_node();
 
    while(objNode)
    {
        axVectorPairString objData;
        
        for (xml_attribute<>* attr = objNode->first_attribute();
             attr; attr = attr->next_attribute())
        {
            objData.push_back(std::pair<std::string, std::string>
                              (attr->name(), attr->value()));
        }
        
        if(objNode->name() == std::string("axToggle"))
        {
            axToggle::Builder toggleBuilder(win);
            toggleBuilder.Create(objData);
        }
        else if(objNode->name() == std::string("axButton"))
        {
            axButton::Builder builder(win);
            builder.Create(objData);
        }
        else if(objNode->name() == std::string("axNumberBox"))
        {
            axNumberBox::Builder builder(win);
            builder.Create(objData);
        }
        else if(objNode->name() == std::string("axKnob"))
        {
            axKnob::Builder builder(win);
            builder.Create(objData);
        }
        else if(objNode->name() == std::string("axLabel"))
        {
            axLabel::Builder builder(win);
            builder.Create(objData);
        }
        else if(objNode->name() == std::string("axStaticImage"))
        {
            axStaticImage::axStaticImageBuilder builder(win);
            builder.Create(objData);
        }
        
        objNode = objNode->next_sibling();
    }

    
    //    node->next_sibling()
}
