#include <string>
#include "NewsItem.h"
using std::string_view;

NewsItem::NewsItem(string_view _headline, int64_t _relatedTeamId)
: headline(_headline), relatedTeamId(_relatedTeamId) {}