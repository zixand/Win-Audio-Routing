#ifndef WIDGETCONTROL_H
#define WIDGETCONTROL_H

#include "pch.h"

class WidgetControl {
public:
	WidgetControl();
	virtual ~WidgetControl();
	virtual void render();
	bool isOpen();

public:
	bool open;
};

#endif // WIDGETCONTROL_H
