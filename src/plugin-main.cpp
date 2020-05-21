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

#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QApplication>
#include <QAction>

#include "projector-dock.hpp"

#include "ui/window-add-projector.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

bool obs_module_load(void)
{
	const QAction *action = static_cast<QAction *>(
		obs_frontend_add_tools_menu_qaction("Projector"));

	const auto menu_cb = [] {
		auto window = new AddProjectorWindow(
			static_cast<QWidget *>(obs_frontend_get_main_window()));

		window->show();
	};

	action->connect(action, &QAction::triggered, menu_cb);

	return true;
}

void obs_module_unload()
{
	blog(LOG_INFO, "plugin unloaded");
}
