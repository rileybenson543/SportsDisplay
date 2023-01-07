#include "Scheduler.h"

#include <iostream>

#include "datetime.h"
#include "DataProcess.h"

// NFL start - Aug 4
// NFL end - Feb, 2nd Sunday
// NASCAR start - Feb 13
// NASCAR end - November 1st week
using jed_utils::timespan, jed_utils::datetime;

timespan one_week_away = timespan(-7); // 1 week buffer for getting events the
										// the week before they start
timespan one_week_ago = timespan(7);

datetime today;


void start_main()
{
	while (true)
	{
		
	}
}

SportsType determineSportsMode()
{
	bool nfl = false;
	bool nascar = false;

	today = datetime();

	SportsDates nfl_dates = get_start_end_date(SportsType::NFL,&today);
	SportsDates nascar_dates = get_start_end_date(SportsType::NASCAR, &today);

	if ((today - nfl_dates.start) >= one_week_away 
		&& (today - nfl_dates.end) <= one_week_ago)
		nfl = true;

	if ((today - nascar_dates.start) >= one_week_away
		&& (today - nascar_dates.end) <= one_week_ago)
		nascar = true;

	if (nfl && nascar) return SportsType::MIXED;
	if (nfl) return SportsType::NFL;
	if (nascar) return SportsType::NASCAR;
}



//bool every_n_seconds(int n)
//{
//	
//}


/*
raceCenterGlobals = {
'raceURL': 'https://cf.nascar.com/cacher/2020/{series}/{race_id}/weekend-feed.json',
'pointsURL' : 'https://cf.nascar.com/live/feeds/series_{series}/{race_id}/live_points.json',
'liveRaceURL' : 'https://cf.nascar.com/live/feeds/series_{series}/{race_id}/live_feed.json',
'stagePointsURL' : 'https://cf.nascar.com/cacher/2020/{series}/{race_id}/live-stage-points.json',
'snappyTVURL' : 'https://cf.nascar.com/cacher/2020/{series}/{race_id}/snappytv.json',
'keyMomentsURL' : 'https://cf.nascar.com/cacher/2020/{series}/{race_id}/lap-notes.json',
'lapTimesURL' : 'https://cf.nascar.com/cacher/2020/{series}/{race_id}/lap-times.json',
'driverRecapURL' : 'https://cf.nascar.com/cacher/2020/{series}/{race_id}/driver-recap.json',
'lapAveragesURL' : 'https://cf.nascar.com/cacher/2020/{series}/{race_id}/lap-averages.json',
'pitStopDataURL' : 'https://cf.nascar.com/cacher/live/series_{series}/{race_id}/live-pit-data.json',
'liveFlagDataURL' : 'https://cf.nascar.com/live/feeds/live-flag-data.json',
'scheduleFeedURL' : 'https://cf.nascar.com/cacher/2020/race_list_basic.json',
'weekendScheduleFeedURL' : 'https://cf.nascar.com/cacher/2020/{series}/schedule-feed.json',
}*/