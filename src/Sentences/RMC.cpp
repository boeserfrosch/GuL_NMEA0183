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

#include "RMC.h"
#include <regex>
#include <sstream>
#include <iomanip>

namespace GuL
{
    namespace NMEA0183
    {
        namespace Sentence
        {

            bool RMC::parse(std::string &sentence, NMEA0183::Data &data)
            {
                data.position.meta.signalIntegrity = PositionSignalIntegrity::DATA_NOT_VALID;

                std::stringstream sstream(sentence);
                std::string part;
                std::vector<std::string> parts;
                while (std::getline(sstream, part, ','))
                {
                    parts.push_back(part);
                }

                if (parts.size() != 12)
                {
                    return false;
                }

                float time = Utils::stringToFloat(parts.at(1));
                data.datetime.hour = time / 10000;
                data.datetime.minute = ((int)(time / 100)) % 100;
                data.datetime.second = ((int)time % 100);
                data.datetime.millisecond = (time - ((int)time)) * 1000;

                data.position.meta.status = Utils::statusStringToEnum(parts.at(2));

                float lat = Utils::stringToFloat(parts.at(3));
                float lon = Utils::stringToFloat(parts.at(5));
                data.position.latitude = Utils::decimalDegreeToPositionComponent(lat);
                data.position.latitude.direction = Utils::compassDirectionStringToEnum(parts.at(4));
                data.position.longitude = Utils::decimalDegreeToPositionComponent(lon);
                data.position.longitude.direction = Utils::compassDirectionStringToEnum(parts.at(6));

                try
                {
                    data.movement.speed = Utils::knotsToMeterPerSeconds(Utils::stringToFloat(parts.at(7)));
                }
                catch (std::exception())
                {
                }
                data.movement.track = Utils::decimalDegreeToPositionComponent(Utils::stringToFloat(parts.at(8)));
                data.movement.track.direction = CompassDirection::N;

                uint16_t date = Utils::stringToInt(parts.at(9));
                data.datetime.year = date % 100;
                data.datetime.month = (date / 100) % 100;
                data.datetime.day = date / 10000;

                data.mageneticVariation = Utils::decimalDegreeToPositionComponent(Utils::stringToInt(parts.at(10)));
                data.mageneticVariation.direction = Utils::compassDirectionStringToEnum(parts.at(11));
                data.position.meta.signalIntegrity = Utils::PositionSignalIntegrityStringToEnum(parts.at(12)); // in parts at 12 there can be also the checksum include, due to the characteristic of the function just the first char will viewed, so it is not problematic (at the moment) TODO: CHANGE!

                return true;
            }

            std::string RMC::createNMEASentence(RMC::RMC_DATA &data, char startChar)
            {
                std::ostringstream rmcSentence;
                rmcSentence
                    << startChar << "GPRMC,"
                    << std::fixed << std::setprecision(2)
                    << data.time << ","
                    << std::fixed << std::setprecision(4)
                    << (char)data.status << "," << Utils::positionComponentToDecimalDegree(data.latitude) << "," << (char)data.latitude.direction << ","
                    << Utils::positionComponentToDecimalDegree(data.longitude) << "," << (char)data.longitude.direction << ","
                    << data.speed_knots << ","
                    << data.track << ","
                    << data.dateDDMMYY << ","
                    << Utils::positionComponentToDecimalDegree(data.mageneticVariation) << "," << (char)data.mageneticVariation.direction << ","
                    << (char)data.signalIntegrity;
                return rmcSentence.str();
            }
        }
    }
} // namespace GuL
