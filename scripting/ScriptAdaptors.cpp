#include "ScriptAdaptors.h"
#include "mono/metadata/object.h"
#include "mono/metadata/appdomain.h"
#include <mono/metadata/loader.h>


namespace Exodus
{

#define EX_ADD_INTERNAL_CALL(Name) mono_add_internal_call("EX_ScriptCore.InternalCalls::" #Name, Name)

	static MonoString* CppFunction()
	{
		return mono_string_new(mono_domain_get(), "This is C++ from Adaptor Class!");
	}

	void Adaptor::RegisterFunctions()
	{
		EX_ADD_INTERNAL_CALL(CppFunction);


	}
}