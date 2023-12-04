#include "NMEA0183.h"

namespace GuL
{
    namespace NMEA0183
    {

        Protocol::Protocol(char sentenceStart) : Protocol(_internalData, sentenceStart)
        {
        }

        Protocol::Protocol(NMEA0183::Data &data, char sentenceStart) : _data(data)
        {
            // TODO: Maybe check the char. NMEA0183 defines just $ and ! as start char
            _startChar = sentenceStart;

            // _sentenceParser["RMC"] = RMC();
            _sentenceTypeMap["RMC"] = SentenceType::ST_RMC;
            _sentenceTypeMap["GGA"] = SentenceType::ST_GGA; /*
             _sentenceTypeMap["GLL"] = SentenceType::GLL;
             _sentenceTypeMap["GSA"] = SentenceType::GSA;
             _sentenceTypeMap["GSV"] = SentenceType::GSV;
             _sentenceTypeMap["TXT"] = SentenceType::TXT;
             _sentenceTypeMap["VTG"] = SentenceType::VTG;*/
        }

        std::string Protocol::getParserName()
        {
            return _parserName;
        }

        uint8_t Protocol::calcChecksum(std::string const &sentence)
        {
            size_t startPos = (sentence.at(0) == _startChar) ? 1 : 0;
            std::size_t csSeparatorPos = sentence.rfind('*');
            size_t endPos = (csSeparatorPos != std::string::npos) ? csSeparatorPos : sentence.length();

            uint8_t cs = 0;

            for (auto byte : std::string(sentence.begin() + startPos, sentence.begin() + endPos))
            {
                cs ^= byte;
            }
            return cs;
        }

        bool Protocol::checkSentenceChecksum(std::string const &sentence)
        {
            std::size_t csSeparatorPos = sentence.rfind('*');
            if (csSeparatorPos == std::string::npos)
            {
                return true;
            }

            uint8_t calcCS = this->calcChecksum(sentence);
            std::string recvCS = sentence.substr(csSeparatorPos + 1);
            uint recCS = strtoul(recvCS.c_str(), 0, 16);
            return (calcCS == recCS);
        }

        std::set<Protocol::SentenceType> Protocol::parse(std::string &data, bool singleSentence)
        {
            std::set<SentenceType> parsed;
            _buffer.append(data);

            while (_buffer.size() > 1 && _buffer.find(_startChar) != std::string::npos)
            {
                // Remove starting garbage
                _buffer = _buffer.substr(_buffer.find(_startChar));
                if (!this->isSentenceComplete())
                {
                    break;
                }
                // Extract and remove next sentence from the buffer
                std::string sentence = this->popNextSentence();
                if (this->checkSentenceChecksum(sentence))
                {
                    parsed.insert(this->parseSentence(sentence));
                    if (singleSentence)
                    {
                        // We want just parse one single valid sentence per function run
                        break;
                    }
                }
            }
            return parsed;
        }

        bool Protocol::isSentenceComplete()
        {
            return (_buffer.at(0) == _startChar &&
                    (_buffer.find("\r") != std::string::npos || _buffer.find(_startChar, 1) != std::string::npos));
        }

        std::string Protocol::popNextSentence()
        {
            size_t endOfSentence = std::min(_buffer.find("\r"), _buffer.find(_startChar, 1));
            std::string sentence = _buffer.substr(0, endOfSentence);
            if (_buffer.size() > endOfSentence + 1)
            {
                _buffer = _buffer.substr(endOfSentence + 1);
            }
            else
            {
                _buffer.clear();
            }
            return sentence;
        }

        Protocol::SentenceType Protocol::parseSentence(std::string &sentence)
        {
            std::string dataset = sentence.substr(std::max((unsigned int)0, sentence.find(",") - 3), 3);
            if (_sentenceTypeMap.find(dataset) == _sentenceTypeMap.end())
            {
                return SentenceType::ST_UNKNOWN;
            }
            switch (_sentenceTypeMap.at(dataset))
            {
            case SentenceType::ST_RMC:
                _rmc.parse(sentence, _data);
                break;
            case SentenceType::ST_GGA:
                _gga.parse(sentence, _data);
                break;
            }
            return _sentenceTypeMap.at(dataset);
        }

        std::string Protocol::appendChecksum(std::string sentence)
        {
            static std::string hexLookup = "0123456789ABCDEF";
            uint8_t cs = calcChecksum(sentence);
            sentence.push_back('*');
            sentence.push_back(hexLookup.at(cs >> 4));
            sentence.push_back(hexLookup.at(cs & 0xF));
            return sentence;
        }

    } // namespace NMEA0183
} // namespace GuL
