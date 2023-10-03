/**
 * Author:	Varun Khambhata
 * Created: 03.10.2023
**/

#ifndef __VOIDMEMORY0__
#define __VOIDMEMORY0__

class VoidMemory0 {
private:
	void* memory;
	unsigned int refrenceCount = 0;

public:
	void* getMemory();
	void  setMemory(void* memory);
	bool  hasRefrence();
	bool  firstRefrence();
	bool  operator++();
	bool  operator--();
	bool  operator++(int);
	bool  operator--(int);	
};

#endif
