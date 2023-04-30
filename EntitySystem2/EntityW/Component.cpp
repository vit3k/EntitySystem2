#include "Component.h"

namespace EntityW
{
	TypeId ScriptComponentTypeId()
	{
		return ClassTypeId<BaseComponent>::GetScriptTypeId();
	}
}
