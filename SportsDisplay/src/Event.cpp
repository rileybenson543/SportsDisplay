#include "Event.h"

Event::Event() {}

Event::Event(
	long _id,
	string _str_scheduledDatetime,
	string _detail,
	string _shortDetail,
	string _name,
	string _shortName,
	float _clock,
	string _displayClock,
	int _period,
	bool _completed,
	string _state,
	string _locationName,
	int _homeTeamId,
	int _awayTeamId,
	int _homeTeamScore,
	int _awayTeamScore,
	string _briefDownText,
	int _posessionTeamId,
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