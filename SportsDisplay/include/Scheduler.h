#pragma once

#include "datetime.h"
#include "DataProcess.h"

using jed_utils::datetime;

struct SportsDates
{
	datetime start;
	datetime end;
};
enum class SportsType
{
	NFL,
	NHL,
	NASCAR,
	MIXED
};

SportsType determineSportsMode();
