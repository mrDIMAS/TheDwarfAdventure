/**********************************************************************
 * The Dwarf Adventure - 2D adventure RPG with destructible world     *
 *                                                                    *
 * Copyright(C) 2016-2019 Dmitry Stepanov a.k.a mrDIMAS a.k.a v1al    *
 *                                                                    *
 * This program is free software: you can redistribute it and/or      *
 * modify it under the terms of the GNU General Public License as     *
 * published by the Free Software Foundation, either version 3 of     *
 * the License, or (at your option) any later version.                *
 *                                                                    *
 * This program is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * along with this program. If not, see https://www.gnu.org/licenses/ *
 *                                                                    *
 **********************************************************************/

#include "Precompiled.h"
#include "Exception.h"

#ifdef _WIN32
#include <Windows.h>
#include <process.h>
#include <DbgHelp.h>
#define TRACE_MAX_STACK_FRAMES 1024
#define TRACE_MAX_FUNCTION_NAME_LENGTH 1024
#else
#include <signal.h>
#endif

Exception::Exception(const std::string & message)
{
	mMessage += "Unhandled exception: " + message;
	mMessage += "\n\n\n";
	mMessage += "Call stack back trace:\n";

#ifdef _WIN32
	void *stack[TRACE_MAX_STACK_FRAMES];
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	WORD numberOfFrames = CaptureStackBackTrace(0, TRACE_MAX_STACK_FRAMES, stack, NULL);
	SYMBOL_INFO *symbol = (SYMBOL_INFO *)malloc(sizeof(SYMBOL_INFO) + (TRACE_MAX_FUNCTION_NAME_LENGTH - 1) * sizeof(TCHAR));
	symbol->MaxNameLen = TRACE_MAX_FUNCTION_NAME_LENGTH;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	DWORD displacement;
	IMAGEHLP_LINE *line = (IMAGEHLP_LINE *)malloc(sizeof(IMAGEHLP_LINE));
	line->SizeOfStruct = sizeof(IMAGEHLP_LINE);
	for (int i = 0; i < numberOfFrames; i++)
	{
		DWORD address = (DWORD)(stack[i]);
		SymFromAddr(process, address, NULL, symbol);
		if (SymGetLineFromAddr(process, address, &displacement, line))
		{
			mMessage += std::string("   ") + to_string(i) + ". " + symbol->Name + " in " + line->FileName + ": " + to_string(line->LineNumber) + '\n';
		}
		else
		{
			mMessage += std::string("   ") + to_string(i) + ". Stack frame was missing\n";
		}
	}
#else
	mMessage += "Not available";
#endif

    cerr << mMessage << endl;
    
#ifndef NDEBUG
#ifdef _MSC_VER
	__debugbreak();
#else
    raise(SIGTRAP);
#endif
#endif
}

Exception::~Exception()
{

}

const char * Exception::what() const throw()
{
	return mMessage.c_str();
}
