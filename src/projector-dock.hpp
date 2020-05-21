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

class ProjectorDock : public QDockWidget {

private:
	int width;
	int height;

public:
	obs_source_t *source;

public:
	ProjectorDock(obs_source_t *source, int width = 400, int height = 225);

	QSize sizeHint() const override;
	void closeEvent(QCloseEvent *event) override;
};
