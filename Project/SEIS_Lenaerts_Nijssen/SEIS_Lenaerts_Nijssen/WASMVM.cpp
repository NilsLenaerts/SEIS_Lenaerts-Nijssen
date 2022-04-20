#include "WASMVM.h"


WASMVM::WASMVM()
{
	
}

void WASMVM::loadFile(std::string path)
{
	byteStream = Bytestream{ path };
	byteStream.seek(8); //skip webassembly magic number and version number
	InstructionSet section;
	while (!byteStream.atEnd()) {
		section = (InstructionSet)byteStream.readByte();
		switch (section)
		{
		case InstructionSet::custom:
			//custom section
			break;
		case InstructionSet::type: //verander dit naar constants voor readability
			readTypeSection();
			break;
		case InstructionSet::import:
			readImportSection();
			break;
		case InstructionSet::function:
			readFuncSection();
			break;
		case InstructionSet::table:
			//table section not neccessary
			break;
		case InstructionSet::memory:
			readMemorySection();
			break;
		case InstructionSet::global:
			//global section
			break;
		case InstructionSet::code:
			readCodeSection();
			break;
		case InstructionSet::data:
			readDataSection();
			break;
		case InstructionSet::datacount:
			readDataCountSection();
			break;
		default:
			break;
		}
	}
}

void WASMVM::readTypeSection() 
{
	byteStream.readUInt32(); //skip section size
	int typeCount = byteStream.readUInt32();
	for (int i = 0; i < typeCount; i++) {

	}
}

void WASMVM::readFuncSection()
{
}

void WASMVM::readCodeSection()
{
}

void WASMVM::readExportSection()
{
}

void WASMVM::readMemorySection()
{
}

void WASMVM::readDataSection()
{
}

void WASMVM::readDataCountSection()
{
}

void WASMVM::readImportSection()
{
}
