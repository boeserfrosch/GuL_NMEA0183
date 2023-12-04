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

#include "GGA.h"
#include <regex>
#include <sstream>
#include <iomanip>

namespace GuL
{
    namespace NMEA0183
    {
        namespace Sentence
        {
            bool GGA::parse(std::string &sentence, NMEA0183::Data &data)
            {
                std::stringstream sstream(sentence);
                std::string part;
                std::vector<std::string> parts;
                while (std::getline(sstream, part, ','))
                {
                    parts.push_back(part);
                }

                if (parts.size() != 15)
                {
                    return false;
                }

                float time = Utils::stringToFloat(parts.at(1));
                data.datetime = Utils::extractTimeAndDateToDatetime(time, Utils::getDDMMYYFromDatetime(data.datetime));
                float lat = Utils::stringToFloat(parts.at(2));
                float lon = Utils::stringToFloat(parts.at(4));
                data.position.latitude = Utils::decimalDegreeToPositionComponent(lat);
                data.position.longitude = Utils::decimalDegreeToPositionComponent(lon);
                data.position.latitude.direction = Utils::compassDirectionStringToEnum(parts.at(3));
                data.position.longitude.direction = Utils::compassDirectionStringToEnum(parts.at(5));

                data.position.meta.gpsQuality = (GPSQuality)Utils::stringToInt(parts.at(6));
                data.position.meta.numberOfUsedSatellites = Utils::stringToInt(parts.at(7));
                data.position.meta.dilutionOrPrecision = Utils::stringToFloat(parts.at(8));
                data.position.meta.antennaHeight.value = Utils::stringToFloat(parts.at(9));
                data.position.meta.antennaHeight.unit = LengthUnit::METER; // GGA should always be with meter. TODO: Use the passed data
                data.position.meta.geoidalSeparation.value = Utils::stringToFloat(parts.at(11));
                data.position.meta.geoidalSeparation.unit = LengthUnit::METER; // GGA should always be with meter. TODO: Use the passed data
                data.position.meta.dgps.ageOfData = Utils::stringToFloat(parts.at(13));
                data.position.meta.dgps.referenceStation = Utils::stringToInt(parts.at(14)); // parts.at(14) can include the checksum separated with *, stringToInt should be able to handle this!?

                return true;
            }

            std::string GGA::createNMEASentence(GGA::GGA_DATA &data, char startChar)
            {
                std::ostringstream sentence;
                sentence
                    << startChar << "GPGGA," // 0
                    << std::fixed << std::setprecision(2)
                    << data.time << "," // 1
                    << std::fixed << std::setprecision(4)
                    << Utils::positionComponentToDecimalDegree(data.latitude) << "," << (char)data.latitude.direction << ","   // 2 + 3
                    << Utils::positionComponentToDecimalDegree(data.longitude) << "," << (char)data.longitude.direction << "," // 4 + 5
                    << data.quality << ","                                                                                     // 6
                    << (uint16_t)data.numberOfUsedSatellites << ","                                                            // 7
                    << data.dilutionOfPrecision << ","                                                                         // 8
                    << data.antenneHeight.value << "," << (char)data.antenneHeight.unit << ","                                 // 9 + 10
                    << data.geoidalSeparation.value << "," << (char)data.geoidalSeparation.unit << ","                         // 11 + 12
                    << data.ageOfDGPSDataset << "," << data.DGPSReferenceStation;                                              // 13 + 14

                return sentence.str();
            }
        }
    }
} // namespace GuL
