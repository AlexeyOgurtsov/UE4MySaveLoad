#pragma once

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

class FMySaveArchive : public FObjectAndNameAsStringProxyArchive
{
public:
	FMySaveArchive(FArchive& InInnerArchive) :
		FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		ArIsSaveGame = true;
	}
};
