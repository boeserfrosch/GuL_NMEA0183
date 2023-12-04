/**
 * GuL_NMEA0183
 * Copyright (c) 2023 Guido Lehne
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Developed for Arduino-ESP32, but universal usable
 * Created by Guido Lehne
 *
 */
#pragma once

#include <Arduino.h>
#include <set>
#include <map>
#include "NMEA0183_Data.h"
#include "Sentences/RMC.h"
#include "Sentences/GGA.h"
/*
#include "Sentences/GLL.h"
#include "Sentences/GSA.h"
#include "Sentences/GSV.h"
#include "Sentences/TXT.h"
#include "Sentences/VTG.h"*/
#include "NMEA0183_Data.h"

namespace GuL
{
    namespace NMEA0183
    {
        class Protocol
        {
        public:
            enum SentenceType
            {
                ST_RMC,
                ST_GGA,

                ST_UNKNOWN
            };

            typedef struct
            {
                struct
                {
                    float decimalDegree;
                    CompassDirection direction;
                } latitude;
                struct
                {
                    float decimalDegree;
                    CompassDirection direction;
                } longitude;
            } Position;

            Protocol(NMEA0183::Data &data, char sentenceStart = '$');
            Protocol(char sentenceStart = '$');
            NMEA0183::Data &getData();
            std::string getParserName();
            std::set<SentenceType> parse(std::string &data, bool singleSentence = false);
            std::string appendChecksum(std::string sentence);

        protected:
            uint8_t calcChecksum(std::string const &sentence);
            bool checkSentenceChecksum(std::string const &sentence);
            bool isSentenceComplete();
            std::string popNextSentence();
            SentenceType parseSentence(std::string &sentence);

            std::string _parserName = "NMEA0183";
            std::map<std::string, SentenceType> _sentenceTypeMap;
            Position _position;

        private:
            char _startChar;
            std::string _buffer;
            Data _internalData;
            Data &_data;
            Sentence::RMC _rmc;
            Sentence::GGA _gga;
        };

    }
} // namespace GuL
