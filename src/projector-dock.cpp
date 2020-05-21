/*
Better Projectors
Copyright (C) 2020 Bennik2000 bennik.ko@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include "projector-dock.hpp"
#include "projector-widget.hpp"
#include <QGridLayout>
#include <QLabel>

ProjectorDock::ProjectorDock(obs_source_t *source)
	: QDockWidget("Projector", static_cast<QWidget *>(obs_frontend_get_main_window()))
	
{
	setFeatures(QDockWidget::AllDockWidgetFeatures);

	// TODO: When projector gets docked, Qt animates its size. This is a performance issue
	// Maybe this: https://stackoverflow.com/questions/1445011/turn-off-opaque-resizing-of-qmainwindow-qdockwidget-separator
	// 


	auto widget = new ProjectorWidget(this, source);
	//auto widget = new QLabel("test", this);


	setWidget(widget);
	show();
}
