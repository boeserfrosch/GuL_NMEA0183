#include "NMEA0183_Data.h"
#include <string>

namespace GuL
{
    namespace NMEA0183
    {
        namespace Utils
        {

            std::ostream &operator<<(std::ostream &oStrStream, const PositionSignalIntegrity integrity)
            {
                switch (integrity)
                {
                case PositionSignalIntegrity::AUTONOMOUS:
                    oStrStream << "A";
                    break;
                case PositionSignalIntegrity::DIFFERENTIAL:
                    oStrStream << "D";
                    break;
                case PositionSignalIntegrity::ESTIMATED:
                    oStrStream << "E";
                    break;
                case PositionSignalIntegrity::MANUAL_INPUT:
                    oStrStream << "M";
                    break;
                case PositionSignalIntegrity::SIMULATED:
                    oStrStream << "S";
                    break;
                default:
                    oStrStream << "N";
                    break;
                }
                return oStrStream;
            }

            std::ostream &operator<<(std::ostream &oStrStream, const PositionStatus status)

            {
                switch (status)
                {
                case PositionStatus::A:
                    oStrStream << "A";
                    break;
                case PositionStatus::V:
                    oStrStream << "V";
                    break;
                case PositionStatus::P:
                    oStrStream << "P";
                    break;
                }
                return oStrStream;
            }
            PositionSignalIntegrity PositionSignalIntegrityStringToEnum(std::string &integrityStr)
            {
                char integrity = std::toupper(integrityStr[0]);
                if (integrity == 'A')
                {
                    return PositionSignalIntegrity::AUTONOMOUS;
                }
                else if (integrity == 'D')
                {
                    return PositionSignalIntegrity::DIFFERENTIAL;
                }
                else if (integrity == 'E')
                {
                    return PositionSignalIntegrity::ESTIMATED;
                }
                else if (integrity == 'M')
                {
                    return PositionSignalIntegrity::MANUAL_INPUT;
                }
                else if (integrity == 'S')
                {
                    return PositionSignalIntegrity::SIMULATED;
                }
                return PositionSignalIntegrity::DATA_NOT_VALID;
            }

            PositionStatus statusStringToEnum(std::string status)
            {
                char stat = status[0];
                switch (stat)
                {
                case 'P':
                case 'p':
                    return PositionStatus::P;
                case 'A':
                case 'a':
                    return PositionStatus::A;
                default:
                    return PositionStatus::V;
                }
            }
            CompassDirection compassDirectionStringToEnum(std::string dir)
            {
                if (dir.size() != 1)
                {
                    return CompassDirection::N;
                }
                switch (toupper(dir[0]))
                {
                case 'N':
                    return CompassDirection::N;
                case 'E':
                    return CompassDirection::E;
                case 'S':
                    return CompassDirection::S;
                default:
                    return CompassDirection::W;
                }
            }

            float positionComponentToDecimalDegree(PositionComponent pos)
            {
                return pos.degree * 100 + pos.minutes + pos.seconds / 60;
            }

            PositionComponent decimalDegreeToPositionComponent(float deg)
            {
                PositionComponent position;
                position.degree = deg / 100;
                position.minutes = ((int)deg) % 100;
                position.seconds = (deg - ((int)deg)) * 60;
                return position;
            }
            float knotsToMeterPerSeconds(float knots)
            {
                return knots * 0.5144444444;
            }
            float stringToFloat(std::string str, float defaultValue)
            {
                if (str.size() == 0)
                {
                    return defaultValue;
                }
                return std::stof(str);
            }
            int stringToInt(std::string str, int defaultValue)
            {
                if (str.size() == 0)
                {
                    return defaultValue;
                }
                return std::stoi(str);
            }

            Datetime extractTimeAndDateToDatetime(float time, uint16_t date)
            {
                Datetime datetime;
                datetime.year = date % 100;
                datetime.month = (date / 100) % 100;
                datetime.day = date / 10000;
                datetime.hour = time / 10000;
                datetime.minute = ((int)(time / 100)) % 100;
                datetime.second = ((int)time % 100);
                datetime.millisecond = (time - ((int)time)) * 1000;
                return datetime;
            }

            uint16_t getDDMMYYFromDatetime(Datetime &datetime)
            {
                return datetime.year + datetime.month * 100 + datetime.day * 10000;
            }

            float getUTCTimeFromDatetime(Datetime &datetime)
            {
                return datetime.hour * 10000 + datetime.minute * 100 + datetime.second + datetime.millisecond / 1000;
            }
        } // namespace Utils
    }     // namespace NMEA0183

} // namespace GuL
