// CallGatesIA32.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

unsigned short callGate[10];

void* g_CallgateAddr = 0;

typedef struct CALL_GATE32 {
	int offset0_15 : 16;
	int selector : 16;
	union {
		struct {
			int argCount : 5;
			int zeros : 3;
			int type : 4;
			int sFlag : 1;
			int dpl : 2;
			int pFlag : 1;
		} part;
		unsigned __int16 all;
	} dummy;
	int offset16_31 : 16;
}CALL_GATE32, *PCALL_GATE32;

typedef struct CALL_GATE
{
	unsigned __int32 offset0_15 : 16;
	unsigned __int32 selector : 16;
	union {
		struct {
			unsigned __int16 ist : 3;
			unsigned __int16 ignored : 5;
			unsigned __int16 type : 5;
			unsigned __int16 dpl : 2;
			unsigned __int16 p : 1;
		} part;
		unsigned __int16 all;
	} dummy;
	unsigned __int64 offset16_63 : 48;
	unsigned __int32 reserved : 32;
}CALL_GATE, *PCALL_GATE;

typedef struct Address
{
	__int64 All;
	struct
	{
		__int64 offset0_15 : 16;
		__int64 offset16_63 : 48;
	}Fields;

};

void CreateCallGateStruct(int address , int gdtr , int GDTIndex) {


	CALL_GATE32 callgate32 = { 0 };
	callgate32.dummy.part.zeros = 0x0;
	callgate32.selector = 0x8;
	callgate32.dummy.part.type = 0xc;
	callgate32.dummy.part.sFlag = 0x0;
	callgate32.dummy.part.dpl = 0x3;
	callgate32.dummy.part.pFlag = 0x1;
	callgate32.offset0_15 = 0x0000ffff & address;
	address = address >> 16;
	callgate32.offset16_31 = 0x0000ffff & address;


	printf("eq %x+(0x8*0x%x) ", gdtr , GDTIndex);

	printf("%04x", callgate32.offset16_31);
	printf("%04x", callgate32.dummy.all);
	printf("%04x", callgate32.selector);
	printf("%04x", callgate32.offset0_15);



}


int main()
{
	// Making windbg structure

	int DispatcherAddress= 0x8176fcd4;
	int gdtr = 0x80b5b000;
	int GdtIndex = 0x64;

	CreateCallGateStruct(DispatcherAddress, gdtr, GdtIndex);

	unsigned short callGate[3];

	callGate[0] = 0x0;
	callGate[1] = 0x0;
	callGate[2] = 0x320;

	// Invoke the far call

	__asm { int 3 }
	__asm
	{
		call fword ptr[callGate];
	}
}
