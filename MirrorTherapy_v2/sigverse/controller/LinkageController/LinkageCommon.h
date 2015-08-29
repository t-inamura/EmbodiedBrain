/*
 * ==========================================================================================
 * @brief  Linkage controller common class.
 * @deprecated
 * @file   LinkageCommon.h
 * @date   2015/08/31
 * @author National Institute of Informatics
 * @par    1.0.0
 * ==========================================================================================
 */

#ifndef SIGVERSE_LINKAGE_COMMON_H
#define SIGVERSE_LINKAGE_COMMON_H

class LinkageCommon
{
public:
	/*
	 * Limb mode.
	 *   FOOT means that using foot in grasping.
	 */
	enum LimbModeType
	{
		HAND = 0,
		FOOT = 1,
		LimbMode_Count = (FOOT + 1)
	};

	// Limb mode string array list.
	static const std::string limbModes[LimbMode_Count];
};

const std::string LinkageCommon::limbModes[LimbMode_Count] = { "HAND", "FOOT" };

#endif SIGVERSE_LINKAGE_COMMON_H

