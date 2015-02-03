#include <sigverse/Controller.h>
#include <sigverse/modelerr.h>
#include <sigverse/ControllerEvent.h>
#include <sigverse/SimObj.h>
#include <sigverse/Logger.h>
#include <sigverse/Vector3d.h>
#include <sigverse/ViewImage.h>
#include <math.h>

/*!
 * @brief Define circular constant.
 */
#define PI 3.141592

/*!
 * @brief Translate degree to radian.
 */
#define DEG2RAD(DEG) ( (PI) * (DEG) / 180.0 )

/*!
 * @brief Define Kinect service name.
 */
#define SIGSERVICE "SIGKINECT_Linkage"

static double RAD2DEG(const double RAD) {
	return RAD * 180.0 / PI;
}

static int RAD2DEG(const int RAD) {
	return (int)(RAD2DEG((double)RAD) + 0.5);
}
