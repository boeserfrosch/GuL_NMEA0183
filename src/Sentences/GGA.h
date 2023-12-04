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

#include <Arduino.h>
#include "SentenceType.h"

#pragma once

namespace GuL
{
    namespace NMEA0183
    {
        namespace Sentence
        {
            class GGA : public Sentence::Type
            {
            public:
                struct GGA_DATA
                {
                    float time;
                    PositionComponent latitude;
                    PositionComponent longitude;
                    GPSQuality quality;
                    uint8_t numberOfUsedSatellites;
                    float dilutionOfPrecision;
                    HeightInfo antenneHeight;
                    HeightInfo geoidalSeparation;
                    float ageOfDGPSDataset;
                    uint16_t DGPSReferenceStation;
                };

                virtual bool parse(std::string &sentence, Data &data) override;
                static std::string createNMEASentence(GGA_DATA &data, char startChar = '$');

            private:
            };
        };
    }; // namespace NMEA0183
};     // namespace GuL
