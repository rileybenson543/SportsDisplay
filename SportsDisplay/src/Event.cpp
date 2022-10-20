#include "Event.h"




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