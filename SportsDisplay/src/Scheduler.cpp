#include "Scheduler.h"

// NFL start - Aug 4
// NFL end - Feb, 2nd Sunday
// NASCAR start - Feb 13
// NASCAR end - November 1st week

using namespace jed_utils;


datetime today;

void start_main()
{
	while (true)
	{
		
	}
}

void determineSportsMode()
{
	today = datetime();

	const datetime NFL_Start = datetime(today.get_year(), 8, 4);
	const datetime NASCAR_Start = datetime(today.get_year(), 2, 13);




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