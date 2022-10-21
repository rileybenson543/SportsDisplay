#include <string>
using std::string_view;

class NewsItem {
public:
	NewsItem(string_view _headline, int64_t _relatedTeamId);
		
private:
	string_view headline;
	int64_t relatedTeamId;

};