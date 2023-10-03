/**
 * Author:	Varun Khambhata
 * Created: 03.10.2023
**/

#include <Ignition0Supplement/VoidMemory0.h>

void* VoidMemory0::getMemory() 				{ return memory;													}
void  VoidMemory0::setMemory(void* memory) 	{ this->memory = memory;											}
bool  VoidMemory0::hasRefrence() 			{ return refrenceCount > 0; 										}
bool  VoidMemory0::firstRefrence() 			{ return refrenceCount == 1; 										}
bool  VoidMemory0::operator++() 			{ refrenceCount ++; return firstRefrence(); 						}
bool  VoidMemory0::operator--() 			{ refrenceCount -= refrenceCount > 0 ? 1 : 0; return hasRefrence(); }
bool  VoidMemory0::operator++(int) 			{ refrenceCount ++; return firstRefrence(); 						}
bool  VoidMemory0::operator--(int) 			{ refrenceCount -= refrenceCount > 0 ? 1 : 0; return hasRefrence(); }