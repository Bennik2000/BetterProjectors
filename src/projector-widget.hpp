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

#include <QWidget>
#include <QScreen>
#include <obs.h>
#include <obs.hpp>

class ProjectorWidget : public QWidget {

private:
	obs_source_t *sourceRef = nullptr;
	OBSDisplay display;

public:
	ProjectorWidget(QWidget *parent, obs_source_t *source);
	~ProjectorWidget();

	void setSource(obs_source_t *source);
	obs_source_t *source();

	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	QPaintEngine *paintEngine() const override;

private slots:
	void visibleChanged(bool visible);
	void screenChanged(QScreen *);

private:
	void createDisplay();
	void updateSize();

	void renderCallback(uint32_t cx, uint32_t cy);

	static void qTToGSWindow(WId windowId, gs_window &gsWindow);
	static QSize getWidgetPixelSize(QWidget *widget);
};
