#pragma once
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>
#include <iostream>

// Define delimiters to split string.
#define PAIRS_DELIMITER       ";" // Delimiter to make pair(s) from whole message.
#define KEYandVALUE_DELIMITER ":" // Delimter to split to KEY and VALUE from one pair.
#define VECTOR_DELIMITER      "," // Delimiter to vector(multi dimentional values) from one VALUE.

static const std::string pairsDelimDefault    = PAIRS_DELIMITER;
static const std::string keyValueDelimDefault = KEYandVALUE_DELIMITER;
static const std::string vectorDelimDefault   = VECTOR_DELIMITER;

class SensorData
{
public:
	virtual ~SensorData() {};

	///@brief Convert sensor information to string.
	///@param keyDelim Symbol to split KEY and VALUE.
	///@param recordDelim Symbol to split records. Record is pair of KEY and VALUE.
	///@param valueDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
	virtual std::string encodeSensorData(const std::string &pairsDelim = pairsDelimDefault, const std::string &keyValueDelim = keyValueDelimDefault, const std::string &vectorDelim = vectorDelimDefault) = 0;
	
	///@brief Convert message(string) to sensor data.
	///@param pairsDelim Symbol to split KEY and VALUE.
	///@param recordDelim Symbol to split records. Record is pair of KEY and VALUE.
	///@param valueDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
	virtual void decodeSensorData(const std::string &message, const std::string &pairsDelim = pairsDelimDefault, const std::string &keyValueDelim = keyValueDelimDefault, const std::string &vectorDelim = vectorDelimDefault) = 0;

	///@brief Convert message to std::map. 
	///@param keyDelim Symbol to split KEY and VALUE.
	///@param recordDelim Symbol to split whole message.
	//static std::map<std::string, std::string> convertMessage2Map(const std::string &message, const std::string &keyDelim, const std::string &recordDelim);
	static std::map<std::string, std::vector<std::string> > convertMessage2Map(const std::string &messages, const std::string &pairsDelim = pairsDelimDefault, const std::string &keyValueDelim = keyValueDelimDefault, const std::string &vectorDelim = vectorDelimDefault);

	// TODO �ł��D
	// �Z���T�[�f�[�^�� std::map<std::string, std::vector<std::string> > �ɕϊ��������̂�n���΁C����Ɋ�Â����b�Z�[�W������Ă����D�Ƃ������́D
	// �Ȃ��}�b�v�ɂ���K�v������̂��H�Ƃ��������͈ȉ��D(���)
	// ����ł� encodeSensorData() �Ń��b�Z�[�W�쐬�̂قڂ��ׂĂ�d���Ă��邪�C���b�Z�[�W�쐬�̋K���i:��,�̒u���ꏊ�Ȃǁj����������l���������Ă����K�v������D
	// SensorData�̎��̂��}�b�v�ɕϊ����������������΁C���b�Z�[�W�쐬�̋K���ɏ]���ă��b�Z�[�W���쐬����Ƃ����@�\�������������D
	// �܂�CSensorData�̎��̂��u[KEY�̕�����]��[VALUE�̕�����̃x�N�^]����Ȃ�}�b�v�v�ɕϊ����������encodeSensorData()���ɏ����C
	// ���̒���iencodeSensorData()���j�ɂ���convertMap2Message()���Ă�ő������ɗ^����΁C����ׂ����b�Z�[�W�����������D
	// �Ƃ����@�\�������������D�Ƃ������Ƃł��D
	// ���������C���^�t�F�[�X���������Ă݂܂����D
	// static std::string convertMap2Message(std::map<std::string, std::vector<std::string> > &map, const std::string &keyDelim, const std::string &recordDelim, const std::string &vectorDelim);

	///@brief Delete parentheses '(' and ')' from string.
	static std::string deleteParenthesesFromString(const std::string &input, const std::string &pBegin = "(", const std::string &pEnd = ")");
};
