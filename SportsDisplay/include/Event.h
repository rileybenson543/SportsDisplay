#pragma once
#include <ctime>
#include <string>

using std::string;

class Event {
public:
	Event();
	Event(
		long id,
		string str_scheduledDatetime,
		string detail,
		string shortDetail,
		string name,
		string shortName,
		float clock,
		string displayClock,
		int period,
		bool completed,
		string state,
		string locationName,
		int homeTeamId,
		int awayTeamId,
		int homeTeamScore,
		int awayTeamScore,
		string briefDownText,
		int posessionTeamId,
		bool isRedZone
	);
private:
	long id;
	string str_scheduledDatetime;
	string detail;
	string shortDetail;
	string name;
	string shortName;
	float clock;
	string displayClock;
	int period;
	bool completed;
	string state;
	string locationName;
	int homeTeamId;
	int awayTeamId;
	int homeTeamScore;
	int awayTeamScore;
	string briefDownText;
	int posessionTeamId;
	bool isRedZone;
	


};