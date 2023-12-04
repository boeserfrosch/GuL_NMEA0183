#include <Arduino.h>
#include "NMEA0183.h"
#include "Sentences/RMC.h"

GuL::NMEA0183::Data data;
GuL::NMEA0183::Protocol nmea(data);
std::string sentence = "$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E";

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

    GuL::NMEA0183::Sentence::GGA::GGA_DATA sendData;
    sendData.time = GuL::NMEA0183::Utils::getUTCTimeFromDatetime(data.datetime);
    sendData.latitude = data.position.latitude;
    sendData.longitude = data.position.longitude;
    sendData.quality = data.position.meta.gpsQuality;
    sendData.numberOfUsedSatellites = data.position.meta.numberOfUsedSatellites;
    sendData.dilutionOfPrecision = data.position.meta.dilutionOrPrecision;
    sendData.antenneHeight = data.position.meta.antennaHeight;
    sendData.geoidalSeparation = data.position.meta.geoidalSeparation;
    sendData.ageOfDGPSDataset = data.position.meta.dgps.ageOfData;
    sendData.DGPSReferenceStation = data.position.meta.dgps.referenceStation;

    Serial.println(nmea.appendChecksum(GuL::NMEA0183::Sentence::GGA::createNMEASentence(sendData, '$')).c_str());

    delay(5000);
}