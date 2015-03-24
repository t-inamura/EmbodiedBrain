#include "SensorData.h"

std::map<std::string, std::vector<std::string> > SensorData::convertMessage2Map(const std::string &message, const std::string &pairsDelim, const std::string &keyValueDelim, const std::string &vectorDelim)
{
	std::map<std::string, std::vector<std::string> > map;
	try {
		// �܂��́u;�v��KEY:VALUE���Ƃɕ�������擾����
		std::vector<std::string> pairs;
		boost::split(pairs, message, boost::is_any_of(pairsDelim));

		for(int i = 0; i < (int)pairs.size(); i++) {
			if (pairs[i] == "") continue;

			//std::cout << pairs[i] << std::endl;
			
			// KEY:VALUE �ƂȂ��Ă��镶���񂩂� KEY �� VALUE �𒊏o���ă}�b�v�ɂ���
			std::vector<std::string> keyAndValues;
			boost::split(keyAndValues, pairs[i], boost::is_any_of(keyValueDelim));

			// �u(�v�Ɓu)�v���폜����i�����ꍇ�͉������Ȃ��j�D
			// Delete "(" and ")" from message.
			const std::string valuesStringDeletedParentheses = SensorData::deleteParenthesesFromString(keyAndValues[1]);

			// VALUE�������u,�v�ŕ�������
			std::vector<std::string> valuesVector;
			boost::split(valuesVector, valuesStringDeletedParentheses, boost::is_any_of(vectorDelim));

			map.insert( std::map<std::string, std::vector<std::string> >::value_type(keyAndValues[0], valuesVector) );
		}
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
	return map;
}

std::string SensorData::deleteParenthesesFromString(const std::string &input, const std::string &pBegin, const std::string &pEnd)
{
	// �u(�v�Ɓu)�v���폜����D
	// Delete "(" and ")" from message.
	
	// ( ��T��
	std::string tmpValuesString = input;
	const std::string::size_type pos1(tmpValuesString.find("("));
	
	// ( �������ꍇ�͓��͂̕������Ԃ��ďI������D
	if (pos1 == std::string::npos) {
		return input;
	}

	tmpValuesString.replace(pos1, 1, "");
	const std::string::size_type pos2(tmpValuesString.find(")"));
	tmpValuesString.replace(pos2, 1, "");
	const std::string valuesStringDeletedParenthesis = tmpValuesString;
	return tmpValuesString;
}