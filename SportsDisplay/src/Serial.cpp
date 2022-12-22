#pragma once

#include "serialib.h"
#include "Serial.h"
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "Team.h"
#include "Event.h"
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
int start()
{
    // Serial object

    // Connection to serial port
    std::cout << SERIAL_PORT << std::endl;
    

    char errorOpening = serial.openDevice(SERIAL_PORT, 57600);
    std::cout << std::to_string(errorOpening) << std::endl;
    if (errorOpening != 1) return errorOpening;
        // If connection fails, return the error code otherwise, display a success message

    const char onSignal[] = {0x01};
    const char offSignal[] = { 0x02 };


    //while (true) {
    //
    //    serial.writeBytes(onSignal, 1);
    //    Sleep(1000);
    //    serial.writeBytes(offSignal, 1);
    //    Sleep(1000);
    //
    //}

    // Close the serial device
    //serial.closeDevice();

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
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.type)),
            static_cast<const char*>(static_cast<const void*>(&data.type)) + sizeof data.type,
            std::begin(messageArray) + curr_index);
        curr_index += sizeof data.type;
        //std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&data.num_instances)),
        //    static_cast<const char*>(static_cast<const void*>(&data.num_instances)) + sizeof data.num_instances,
        //    std::begin(messageArray) + curr_index);
        //curr_index += sizeof data.num_instances;

        //for (std::string event_string : data.instances)
        //{
        //const std::string event_string = data.instances[0];

        const short messageLength = data.string_data.length();
        std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&messageLength)),
            static_cast<const char*>(static_cast<const void*>(&messageLength)) + sizeof messageLength,
            std::begin(messageArray) + curr_index);
        curr_index += sizeof messageLength;

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
    char readBuffer[250] = { 0x11,0x11,0x4f,0x4b };

    // Write the message
    serial.writeBytes(messageArray, curr_index);
    // Check for an ACK responses
    //serial.readBytes(readBuffer,4,1000);
    //if ((readBuffer[0] << 8 | readBuffer[1]) == 0x1111) {
    //    if ((readBuffer[2] << 8 | readBuffer[3]) == ACK_Signals::OK) {
    //        std::cout << "test";
    //    }
    //}
    //std::cout << "done";
    //
    return 0;
}

int addIntToArray(void* _itemToAdd, char* array, int start) {
    auto itemToAdd = (short)_itemToAdd;
    std::reverse_copy(static_cast<const char*>(static_cast<const void*>(&itemToAdd)),
        static_cast<const char*>(static_cast<const void*>(&itemToAdd)) + sizeof itemToAdd,
        array + start);
    return sizeof itemToAdd;
}

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
