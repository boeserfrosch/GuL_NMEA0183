#include <Arduino.h>
#include "NMEA0183.h"
#include "Sentences/RMC.h"

GuL::NMEA0183::Data data;
GuL::NMEA0183::Protocol nmea(data);

std::string sentence = "$GPRMC,123519.00,A,4807.04,N,1151.00,E,0.00,0.00,11201,0.00,N,A*0B\r\n";
void setup()
{
    Serial.begin(9600);
}

void loop()
{
    nmea.parse(sentence);
    Serial.printf("%d.%d.20%02d %02d:%02d:%02d.%03d\n", data.datetime.day, data.datetime.month, data.datetime.year, data.datetime.hour, data.datetime.minute, data.datetime.second, data.datetime.millisecond);
    Serial.printf("%d°%d'%f'' %c\n", data.position.latitude.degree, data.position.latitude.minutes, data.position.latitude.seconds, data.position.latitude.direction);
    Serial.printf("%d°%d'%f'' %c\n", data.position.longitude.degree, data.position.longitude.minutes, data.position.longitude.seconds, data.position.longitude.direction);

    GuL::NMEA0183::Sentence::RMC::RMC_DATA sendData;
    sendData.dateDDMMYY = GuL::NMEA0183::Utils::getDDMMYYFromDatetime(data.datetime);
    sendData.latitude = data.position.latitude;
    sendData.longitude = data.position.longitude;
    sendData.mageneticVariation = data.mageneticVariation;
    sendData.signalIntegrity = data.position.meta.signalIntegrity;
    sendData.speed_knots = data.movement.speed / GuL::NMEA0183::Utils::knotsToMeterPerSeconds(1);
    sendData.status = data.position.meta.status;
    sendData.time = GuL::NMEA0183::Utils::getUTCTimeFromDatetime(data.datetime);
    sendData.track = GuL::NMEA0183::Utils::positionComponentToDecimalDegree(data.movement.track);

    Serial.println(nmea.appendChecksum(GuL::NMEA0183::Sentence::RMC::createNMEASentence(sendData, '$')).c_str());
    delay(5000);
}