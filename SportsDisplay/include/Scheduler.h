#pragma once

#include "Datetime/datetime.h"
#include "Datetime/timespan.h"


enum Modes
{
	nfl_only,
	nascar_only,
	mixed

};

void check_for_update();
void start_main();