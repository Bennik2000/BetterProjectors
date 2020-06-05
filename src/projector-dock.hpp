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
#pragma once

#include <QDockWidget>
#include <obs-frontend-api.h>
#include <obs.h>
#include "projector-widget.hpp"

class ProjectorDock;

typedef void (*projectorDockCallback)(ProjectorDock *dock, void *parameter);

class ProjectorDock : public QDockWidget {

private:
	int width;
	int height;
	bool wasFloating;
	bool isAlwaysOnTop;

	ProjectorWidget *projectorWidget;
	projectorDockCallback closeCallback = nullptr;
	void *closeCallbackParameter = nullptr;

public:
	ProjectorDock(const char *source, int width = 400, int height = 225,
		      bool isAlwaysOnTop = false);

	void resizeToWidth();

	void topLevelChanged(bool topLevel);
	obs_source_t *getSource() const;
	bool getIsAlwaysOnTop() const;

	void setCloseCallback(projectorDockCallback callback, void *parameter);

	QSize sizeHint() const override;
	void resizeEvent(QResizeEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void setVisible(bool visible) override;

private slots:
	void AlwaysOnTopToggled(bool alwaysOnTop);
};
