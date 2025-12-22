#pragma once

#include "pch.h"

class UFortEngine : public UEngine
{
public:
	DefineUOriginal(float, GetMaxTickRate, UFortEngine*)
	
public:
	DECLARE_DEFAULT_OBJECT(UFortEngine);
	DECLARE_STATIC_CLASS(UFortEngine);
};