#include "DummyController.h"


void DummyController::onInit(InitEvent &evt)
{
	getObj(myname());
}

double DummyController::onAction(ActionEvent &evt)
{
	return 10.0;
}

extern "C"  Controller * createController ()
{
	return new DummyController;
}
