#pragma once
#include <string>

using std::string_view, std::string;


class Event {
public:
	int64_t homeTeamId{};
	int64_t awayTeamId{};
	Event();
	Event(int64_t _id, string _str_scheduledDatetime, string _detail, string _shortDetail,
	      string _name,
	      string _shortName, double _clock, string _displayClock, int64_t _period, bool _completed,
	      string _state, string _locationName, int64_t _homeTeamId, int64_t _awayTeamId,
	      int64_t _homeTeamScore,
	      int64_t _awayTeamScore, string _briefDownText, int64_t _posessionTeamId, int64_t _yardLine, bool _isRedZone);
	string printString();
	string convertTimeToEST(const string* UTCInput) const;
	string convertMonthNumToString(int monthNum);
	char determineHomeOrAwayHasPossession() const;
	string getPossessionString() const;

private:
	int64_t id{};
	string str_scheduledDatetime;
	string detail;
	string shortDetail;
	string name;
	string shortName;
	double clock{};
	string displayClock;
	int64_t period{};
	bool completed{};
	string state;
	string locationName;
	int64_t homeTeamScore{};
	int64_t awayTeamScore{};
	string briefDownText;
	int64_t posessionTeamId{};
	int64_t yardLine;
	bool isRedZone{};
};