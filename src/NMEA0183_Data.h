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

#include <string>
#include <sstream>

namespace GuL
{
    namespace NMEA0183
    {
        enum PositionSignalIntegrity : char
        {
            AUTONOMOUS = 'A',
            DIFFERENTIAL = 'D',
            ESTIMATED = 'E',
            MANUAL_INPUT = 'M',
            SIMULATED = 'S',
            DATA_NOT_VALID = 'N'
        };
        enum CompassDirection : char
        {
            N = 'N',
            E = 'E',
            S = 'S',
            W = 'W'
        };

        struct PositionComponent
        {
            uint16_t degree;
            uint8_t minutes;
            float seconds;
            CompassDirection direction;
        };

        enum PositionStatus : char
        {
            A = 'A',
            V = 'V',
            P = 'P'
        };

        enum LengthUnit : char
        {
            METER = 'M'
        };

        struct HeightInfo
        {
            float value;
            LengthUnit unit;
        };

        enum GPSQuality : uint8_t
        {
            INVALID = 0,
            GNSS_FIX = 1,
            DGPS_FIX = 2,
            PPS_FIX = 3,
            REAL_TIME_KINEMATIC = 4,
            DEAD_RECKON = 6,
            MANUAL_INPUT_MODE = 7,
            SIMULATION_MODE = 8
        };

        struct Datetime
        {
            uint8_t day;
            uint8_t month;
            uint8_t year;
            uint8_t hour;
            uint8_t minute;
            uint8_t second;
            uint16_t millisecond;
        };

        struct DGPSMetaInfo
        {
            float ageOfData;
            uint16_t referenceStation;
        };

        struct Data
        {
            Datetime datetime;
            struct
            {
                PositionComponent latitude;
                PositionComponent longitude;
                struct
                {
                    PositionStatus status = PositionStatus::V;
                    PositionSignalIntegrity signalIntegrity = PositionSignalIntegrity::DATA_NOT_VALID;
                    GPSQuality gpsQuality = GPSQuality::INVALID;
                    uint8_t numberOfUsedSatellites = 0;
                    float dilutionOrPrecision = 0;
                    HeightInfo antennaHeight;
                    HeightInfo geoidalSeparation;
                    DGPSMetaInfo dgps;

                } meta;
            } position;
            struct
            {
                float speed; // m/s
                PositionComponent track;
            } movement;
            PositionComponent mageneticVariation;
        };

        namespace Utils
        {
            std::ostream &operator<<(std::ostream &oStrStream, const PositionSignalIntegrity integrity);
            PositionSignalIntegrity PositionSignalIntegrityStringToEnum(std::string &integrityStr);
            PositionStatus statusStringToEnum(std::string status);
            CompassDirection compassDirectionStringToEnum(std::string dir);
            float positionComponentToDecimalDegree(PositionComponent pos);
            PositionComponent decimalDegreeToPositionComponent(float deg);
            float knotsToMeterPerSeconds(float knots);
            float stringToFloat(std::string str, float defaultValue = 0);
            int stringToInt(std::string str, int defaultValue = 0);
            Datetime extractTimeAndDateToDatetime(float time, uint16_t date);
            uint16_t getDDMMYYFromDatetime(Datetime &datetime);
            float getUTCTimeFromDatetime(Datetime &datetime);
        }
    } // namespace NMEA0183
} // namespace GuL
