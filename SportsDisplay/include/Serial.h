#pragma once
/*


====================================================
****** Transportaion Protocol Standard v1.0 ********
====================================================



	=================================
	********  Ack message  *********
	=================================

	0				2				4
	+---------------+---------------+
	|   0x1111		| 	message	 	|
	+-------------------------------+
		
		Possible Values:
			"OK" - All good
			"RS" - General Resend
			"CS" - Resend, checksum failure




	=================================
	********  Base message  *********
	=================================
					
	0				2				4
	+---------------+---------------+
	| command type	| command data	|
	+-------------------------------+
	| command data					|
	+-------------------------------+
				.
				.
				.
	+-------------------------------+
	|			checksum			|
	+-------------------------------+




	=================================
	*******   News Message **********
	=================================

	0				2				4
	+---------------+---------------+
	| 	0x0010		| # of headlines|
	+-------------------------------+
	|		headline 1 length		|
	+-------------------------------+
	|		headline 1 data 		|
	+-------------------------------+
	|	headline 1 continued...		|
	+-------------------------------+
	|	headline 1 continued...		|
	+-------------------------------+
	|		headline 2 length		|
	+-------------------------------+
	|		headline 2 data			|
	+-------------------------------+
				.
				.
				.
	+-------------------------------+
	|			checksum			|
	+-------------------------------+




	=================================
	********** Single Event *********
	=================================



	0				2				4
	+---------------+---------------+
	| 	0x0011		| message len.	|
	+-------------------------------+
	|  event type	|  event data	|
	+-------------------------------+
	|			event data	 		|
	+-------------------------------+
	|			event data			|
	+-------------------------------+
					.
					.
					.
	+-------------------------------+
	|			checksum			|
	+-------------------------------+


		Event types:
			Pre - 0x0001
			In Progress - 0x0002
			Post - 0x0003

		Event data:
			Home Team ID - 1 byte char
			Away Team ID - 1 byte char
			 ** If Scheduled **
				Date - 5 bytes - MMMDD string
				Time - HHMMT string
					T: 0 - AM
					   1 - PM
			** If Current **
				Home Team Score - 1 byte int
				Away Team Score - 1 byte int
				Period - 1 byte int
				Time Remaining in period - mmss string
			** If Post **
				Home Team Score
				Away Team Score
			


	=================================
	******  Score w/ Bitmap  ********
	=================================


	0				2				4
	+---------------+---------------+
	| 	0x0020		|	home/away	|
	+-------------------------------+
	|	score		| bitmap length	|
	+-------------------------------+
	|			bitmap data 		|
	+-------------------------------+
	|			bitmap data			|
	+-------------------------------+
	|			bitmap data			|
	+-------------------------------+
				.
				.
				.
	+-------------------------------+
	|			checksum			|
	+-------------------------------+




	=================================
	*********  Score Only  **********
	=================================


	0				2				4
	+---------------+---------------+
	| 	0x0021		|	home/away	|
	+-------------------------------+
	|	score		| checksum		|
	+-------------------------------+




	=================================
	******  Nascar Positions ********
	=================================


	0				2				4
	+---------------+---------------+
	| 	0x2001		|	race state	|
	+-------------------------------+
	|  # of drivers	|	driver 1	|
	+-------------------------------+
	|			driver 1	 		|
	+-------------------------------+
	|			driver 1			|
	+-------------------------------+
	|			driver 1			|
	+-------------------------------+
	|			driver 2			|
	+-------------------------------+
				.
				.
				.
	+-------------------------------+
	|			checksum			|
	+-------------------------------+

		Race states:
			"Green":	0x0000
			"Yellow":	0x0010
			"Red":		0x0020
			"White":	0x0030
			"Checker":	0x0040


*/
#include <vector>
#include <string>

namespace Serial {

	enum class TransmissionType : short {
		ACK = 0x1111,
		NEWS = 0x10,
		SingleEvent = 0x11,
		SingleEventWithBitmap = 0x12,
		Bitmap = 0x20,
		ScoreOnly = 0x21,
		NascarPositions = 0x2001
	};
	enum ACK_Signals {
		OK = 0x4f4b
	};
	enum NascarRaceStates {
		Green =	0x0000,
		Yellow = 0x0010,
		Red =  0x0020,
		White = 0x0030,
		Checker = 0x0040
	};

	struct SerialMessage {
		TransmissionType type;
		short num_instances;
		short state;
		std::string string_data;
		std::vector<std::string> instances;
		bool sendingBitmap = false;
		std::vector<char>* bmp;
		int homeTeamId;
		int awayTeamId;
		bool operator==(const SerialMessage& other) const {
			return type == other.type && 
				num_instances == other.num_instances &&
				state == other.state &&
				string_data == other.string_data &&
				instances == other.instances &&
				bmp == other.bmp &&
				homeTeamId == other.homeTeamId &&
				awayTeamId == other.awayTeamId;
		}
	};
}
int start();
int sendMessage(Serial::SerialMessage data);
int addChecksum(char* message, int end);
int addToArray(void* itemToAdd, char* array, int start);
int addIntToArray(void* _itemToAdd, char* array, int start);