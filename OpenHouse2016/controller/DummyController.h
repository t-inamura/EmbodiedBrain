#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>
#include <sigverse/commonlib/comm/controller/Controller.h>


class DummyController : public Controller
{
	void onInit(InitEvent &evt);
	double onAction(ActionEvent &evt);
};


