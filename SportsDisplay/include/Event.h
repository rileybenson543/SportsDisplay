#pragma once
#include <string>

using std::string_view, std::string;


class Event {
public:
	int64_t homeTeamId{};
	int64_t awayTeamId{};
	Event();
	Event(
		int64_t id,
		string_view str_scheduledDatetime,
		string_view detail,
		string_view shortDetail,
		string_view name,
		string_view shortName,
		double clock,
		string_view displayClock,
		int64_t period,
		bool completed,
		string_view state,
		string_view locationName,
		int64_t homeTeamId,
		int64_t awayTeamId,
		int64_t homeTeamScore,
		int64_t awayTeamScore,
		string_view briefDownText,
		int64_t posessionTeamId,
		bool isRedZone
	);
	string printString();
	string convertTimeToEST(const string* UTCInput);
	string convertMonthNumToString(int monthNum);
private:
	int64_t id{};
	string_view str_scheduledDatetime;
	string_view detail;
	string_view shortDetail;
	string_view name;
	string_view shortName;
	double clock{};
	string_view displayClock;
	int64_t period{};
	bool completed{};
	string_view state;
	string_view locationName;
	int64_t homeTeamScore{};
	int64_t awayTeamScore{};
	string_view briefDownText;
	int64_t posessionTeamId{};
	bool isRedZone{};
};