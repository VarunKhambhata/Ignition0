/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023

 ______                          __                                __     
/\__  _\                     __ /\ \__  __                       /'__`\   
\/_/\ \/       __      ___  /\_\\ \ ,_\/\_\     ___     ___     /\ \/\ \  
   \ \ \     /'_ `\  /' _ `\\/\ \\ \ \/\/\ \   / __`\ /' _ `\   \ \ \ \ \ 
    \_\ \__ /\ \L\ \ /\ \/\ \\ \ \\ \ \_\ \ \ /\ \L\ \/\ \/\ \   \ \ \_\ \
    /\_____\\ \____ \\ \_\ \_\\ \_\\ \__\\ \_\\ \____/\ \_\ \_\   \ \____/
    \/_____/ \/___L\ \\/_/\/_/ \/_/ \/__/ \/_/ \/___/  \/_/\/_/    \/___/ 
               /\____/                                                    
               \_/__/                                                     
**/

#ifndef __Ignition0Engine__
#define __Ignition0Engine__

#include <string>
#include <Ignition0Core/Scene.h>
#include <Ignition0Core/Ignition0Environment.h>

namespace Ignition0 {
	extern void launch(std::string title, int width = 0, int height = 0);
	extern void setDetail(Detail level);
	extern void ignite(m<Scene> scene);
};

#endif