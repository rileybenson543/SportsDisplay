#include "Event.h"
#include <string>
#include <chrono>

using std::string;

Event::Event() {}

Event::Event(
	int64_t _id,
	string_view _str_scheduledDatetime,
	string_view _detail,
	string_view _shortDetail,
	string_view _name,
	string_view _shortName,
	double _clock,
	string_view _displayClock,
	int64_t _period,
	bool _completed,
	string_view _state,
	string_view _locationName,
	int64_t _homeTeamId,
	int64_t _awayTeamId,
	int64_t _homeTeamScore,
	int64_t _awayTeamScore,
	string_view _briefDownText,
	int64_t _posessionTeamId,
	bool _isRedZone) 
	: id(_id), str_scheduledDatetime(_str_scheduledDatetime), detail(_detail),
		shortDetail(_shortDetail), name(_name), shortName(_shortName),
		clock(_clock), displayClock(_displayClock), period(_period),
		completed(_completed), state(_state), locationName(_locationName),
		homeTeamId(_homeTeamId), awayTeamId(_awayTeamId), homeTeamScore(_homeTeamScore),
		awayTeamScore(_awayTeamScore), briefDownText(_briefDownText),
		posessionTeamId(_posessionTeamId), isRedZone(_isRedZone)
{

}
string Event::printString() {
	auto s = string();
	string datetime_pre = string(str_scheduledDatetime);
	string datetime = convertTimeToEST(&datetime_pre);
	s = string(state) + "/"
		+ std::to_string(homeTeamId) + "/" + std::to_string(awayTeamId) + "/";
	if (state == "pre")
		s = s + datetime.substr(0,2) //convertMonthNumToString(stoi(datetime.substr(0, 2)))
		+ "/" + datetime.substr(2, 7) + "/";
	else if (state == "in")
		s = s + std::to_string(homeTeamScore) + "/" + std::to_string(awayTeamScore)
		+ "/" + std::to_string(period)
		+ "/" + string(displayClock) + "/";
	else if (state == "post")
		s = s + std::to_string(homeTeamScore) + "/" + std::to_string(awayTeamScore);
	return string(s);
}
string Event::convertTimeToEST(const string* UTCInput) {
	// 2022-12-16T01:15Z
	enum timeOfDay {
		am = 0,
		pm = 1
	};
	timeOfDay TOD;
	string year = UTCInput->substr(0, 4);
	string month = UTCInput->substr(5, 2);
	int day = stoi(UTCInput->substr(8, 2));
	int hour = stoi(UTCInput->substr(11, 2));
	string minute = UTCInput->substr(14, 2);

	// converting to EST
	// TODO: Will eventually need
	// to find a better way to do this
	constexpr int EST_OFFSET = -5;
	int EST_hour = hour + EST_OFFSET;
	if (EST_hour < 0) {
		EST_hour = EST_hour + 24;
		day--;
	}

	// Converting to 12-hour format
	if (EST_hour < 12) {
		TOD = am;
	}
	else {
		TOD = pm;
		EST_hour -= 12;
	}
	string hour_str;
	if (EST_hour < 10) 
		hour_str = "0" + std::to_string(EST_hour);
	else
		hour_str = std::to_string(EST_hour);

	return month + std::to_string(day) + hour_str 
		+ minute + std::to_string(TOD);
}


string Event::convertMonthNumToString(int monthNum) {
	switch (monthNum) {
	case 1:
		return "Jan";
	case 2:
		return "Feb";
	case 3:
		return "Mar";
	case 4:
		return "Apr";
	case 5:
		return "May";
	case 6:
		return "Jun";
	case 7:
		return "Jul";
	case 8:
		return "Aug";
	case 9:
		return "Sep";
	case 10:
		return "Oct";
	case 11:
		return "Nov";
	case 12:
		return "Dec";
	default:
		return "NaN";

	}
}