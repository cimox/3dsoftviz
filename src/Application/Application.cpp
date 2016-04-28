﻿#include "Application/Application.h"

#include <QDebug>

#ifdef Q_OS_LINUX
	#include <X11/Xlib.h>
#endif

namespace App {

Application::Application(int &argc, char **argv) : QApplication( argc, argv ) {
#if defined(Q_OS_LINUX)
	this->x11Connected = false;
	XInitThreads();
	//XSelectInput(QX11Info::display(), DefaultRootWindow(QX11Info::display()), SubstructureNotifyMask);
#endif
}

Application::~Application() {
}

#ifdef Q_OS_LINUX

bool Application::x11EventFilter(XEvent *event) {
	//qDebug() << "Event Type = " << this->event->type;
	if ( this->x11Connected && event->type == ClientMessage ){
		XEvent xev = *event;
		// uncomment below to get error
		//emit x11PassEvent( xev );
		//emit testPass();
	}
    return false;
}

void Application::x11InitConnection( LibMouse3d::Mouse3dDevice *device ){
	QObject::connect(this, SIGNAL(x11PassEvent( XEvent& )), device, SLOT(x11TranslateEvent( XEvent )));
	this->x11Connected = true;
}

#endif

} //App