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

//#include "plugin-macros.generated.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

bool obs_module_load(void)
{
	QMainWindow *main_window =
		(QMainWindow *)obs_frontend_get_main_window();

	QAction *action = (QAction *)obs_frontend_add_tools_menu_qaction("Projector");
	
	//obs_frontend_push_ui_translation(obs_module_get_string);
	//virtual_prop = new VirtualProperties(main_window);
	//obs_frontend_pop_ui_translation();

	auto menu_cb = [] {
		obs_frontend_add_dock(
			new ProjectorDock(obs_get_source_by_name("Szene")));

	};

	action->connect(action, &QAction::triggered, menu_cb);

	
	return true;
}

void obs_module_unload()
{
	blog(LOG_INFO, "plugin unloaded");
}
