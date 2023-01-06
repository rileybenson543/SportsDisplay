#include "serialib.h"
#include "Serial.h"
#include <vector>
#include <algorithm>
#include <string>
#include "Team.h"
#include "DataProcess.h"

#include <iostream>

#include <stdio.h>


using namespace Serial;




#if defined (_WIN32) || defined(_WIN64)
//for serial ports above "COM9", we must use this extended syntax of "\\.\COMx".
//also works for COM0 to COM9.
//https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea?redirectedfrom=MSDN#communications-resources
#define SERIAL_PORT "\\\\.\\COM6"
#endif
#if defined (__linux__) || defined(__APPLE__)
#define SERIAL_PORT "/dev/ttyUSB0"
#endif



/*!
 * \brief main  Simple example that send ASCII characters to the serial device
 * \return      0 : success
 *              <0 : an error occured
 */
serialib serial;
FILE* fp;

int start()
{

    // Serial object

    // Connection to serial port
    

    char errorOpening = serial.openDevice(SERIAL_PORT, 115200);
    if (errorOpening != 1) {
        std::cerr << "Error Connecting to " + string(SERIAL_PORT) + " with error code " + errorOpening << std::endl;
    	return errorOpening;
    }
        // If connection fails, return the error code otherwise, display a success message
    std::cout << "Successfully connected to " + string(SERIAL_PORT) << std::endl;
    
	return 0;
}


int sendMessage(SerialMessage data) {
    char messageArray[4000] = {};
    int curr_index = 0;
    switch (data.type) {

    case TransmissionType::NEWS:
    {
        // Add command code
        messageArray[1] = 0x10;
        curr_index += 2;

        // number of headlines
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.num_instances)),
            static_cast<const char*>(static_cast<const void*>(&data.num_instances)) + sizeof data.num_instances,
            std::begin(messageArray) + curr_index);
        curr_index += 2;

        // add each headline
        for (std::string s : data.instances) {
            int length = s.length();
            std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&length)),
                static_cast<const char*>(static_cast<const void*>(&length)) + sizeof length,
                std::begin(messageArray) + curr_index);
            curr_index += 4;

            const char* chars = s.c_str();
            std::copy(chars, chars + length,
                std::begin(messageArray) + curr_index);
            curr_index += length;
        }

    }
    break;
    case TransmissionType::SingleEvent:
    {
        // Tells the ESP32 what message is coming
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.type)),
            static_cast<const char*>(static_cast<const void*>(&data.type)) + sizeof data.type,
            std::begin(messageArray) + curr_index);
        curr_index += sizeof data.type;

        // Message length used to determine message end
        const short messageLength = data.string_data.length();
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&messageLength)),
            static_cast<const char*>(static_cast<const void*>(&messageLength)) + sizeof messageLength,
            std::begin(messageArray) + curr_index);
        curr_index += sizeof messageLength;

        // Actual String of data
        std::copy(data.string_data.c_str(), data.string_data.c_str() + data.string_data.size(),
            std::begin(messageArray) + curr_index);
        curr_index += data.string_data.size();
        //}


    }
    break;
    case TransmissionType::SingleEventWithBitmap:
    {
            // Tells the ESP32 what message is coming
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.type)),
            static_cast<const char*>(static_cast<const void*>(&data.type)) + sizeof data.type,
            std::begin(messageArray) + curr_index);
        curr_index += sizeof data.type;

			// Message length used to determine message end
        const short messageLength = data.string_data.length();
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&messageLength)),
            static_cast<const char*>(static_cast<const void*>(&messageLength)) + sizeof messageLength,
            std::begin(messageArray) + curr_index);
        curr_index += sizeof messageLength;

            // Actual String of data
        std::copy(data.string_data.c_str(), data.string_data.c_str() + data.string_data.size(),
            std::begin(messageArray) + curr_index);
        curr_index += data.string_data.size();
        //}


    }
    break;

    case TransmissionType::Bitmap:
    {
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.type)),
            static_cast<const char*>(static_cast<const void*>(&data.type)) + sizeof data.type,
            std::begin(messageArray) + curr_index);
        curr_index += sizeof data.type;

        std::copy(data.bmp->begin(), data.bmp->begin() + 3600, std::begin(messageArray) + curr_index);
        curr_index += 3600;
    }
    break;

    case TransmissionType::NascarPositions:
    {
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.type)),
            static_cast<const char*>(static_cast<const void*>(&data.type)) + sizeof data.type,
            std::begin(messageArray) + curr_index);
        curr_index += 2;

        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.state)),
            static_cast<const char*>(static_cast<const void*>(&data.state)) + sizeof data.state,
            std::begin(messageArray) + curr_index);
        curr_index += 2;

        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.num_instances)),
            static_cast<const char*>(static_cast<const void*>(&data.num_instances)) + sizeof data.num_instances,
            std::begin(messageArray) + curr_index);
        curr_index += 2;

        for (const std::string &driver : data.instances) {
            const char* chars = driver.c_str();
            std::copy(chars, chars + 15,
                std::begin(messageArray) + curr_index);
            curr_index += 15;
        }

    }
    break;
    }
    // Final 
    //addChecksum(messageArray, curr_index);
    //char finalArray[curr_index+1]
    char readBuffer[8] = {};

    // Write the message
	serial.writeBytes(messageArray, curr_index);
    // Check for an ACK responses
    serial.readBytes(readBuffer,2,1000);
    //if ((readBuffer[0] << 8 | readBuffer[1]) == 0x1111) {
        if ((readBuffer[0] << 8 | readBuffer[1]) == OK) {
            //sleep_for(seconds(2));
        	//std::cout << "Received ACK" << std::endl;
        }
    //}
        else {
            std::cerr << "Failed to receive ACK" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            return 1;
        }
    // TODO - process different ACK messages
    return 0;
}

//int addIntToArray(void* _itemToAdd, char* array, int start) {
//    auto itemToAdd = (short)_itemToAdd;
//    std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&itemToAdd)),
//        static_cast<const char*>(static_cast<const void*>(&itemToAdd)) + sizeof itemToAdd,
//        array + start);
//    return sizeof itemToAdd;
//}

int addToArray(const void* _itemToAdd, char* array, int start) {
    std::reverse_copy(static_cast<const char*>(static_cast<const void*>(_itemToAdd)),
        static_cast<const char*>(static_cast<const void*>(_itemToAdd)) + sizeof _itemToAdd,
        array + start);
    return sizeof _itemToAdd;
}


int addChecksum(char *message, int end) {
    int sum = 0;
    for (int i = 0; i < 4000; i++) {
        sum += message[i];
    }
    std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&sum)),
        static_cast<const char*>(static_cast<const void*>(&sum)) + sizeof sum,
        message + end);
    return 0;
}

