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

#include "better-projectors.hpp"
#include "ui/window-add-projector.hpp"
#include "QAction"
#include <ctime>

BetterProjectors::BetterProjectors()
{
	initialize();
}

BetterProjectors::~BetterProjectors() {}

void BetterProjectors::showProjector(const char *name, const char *dockId)
{
	const auto source = obs_get_source_by_name(name);

	const float width = 400;
	const float height = width * (9.0f / 16.0f);

	auto projector = new ProjectorDock(source, width, height);

	if (strcmp(dockId, "") != 0)
		projector->setObjectName(dockId);
	else {
		projector->setFloating(true);

		auto t = std::time(0);
		std::tm *now = std::localtime(&t);

		std::string str = std::to_string(now->tm_year) +
				  std::to_string(now->tm_mon) +
				  std::to_string(now->tm_mday) +
				  std::to_string(now->tm_hour) +
				  std::to_string(now->tm_min) +
				  std::to_string(now->tm_sec);

		projector->setObjectName(str.c_str());
	}

	projector->show();

	obs_frontend_add_dock(projector);
	docks.push_back(projector); // TODO: Remove when closed
}

void BetterProjectors::initialize()
{
	registerToolsAction();
	registerSaveCallback();
}

void BetterProjectors::registerToolsAction()
{
	const QAction *action = static_cast<QAction *>(
		obs_frontend_add_tools_menu_qaction("Better projectors"));

	const auto menu_cb = [this] {
		auto window = new AddProjectorWindow(
			this,
			static_cast<QWidget *>(obs_frontend_get_main_window()));

		window->show();
	};

	action->connect(action, &QAction::triggered, menu_cb);
}

void BetterProjectors::registerSaveCallback()
{
	obs_frontend_add_save_callback(saveCallback, this);
}

void BetterProjectors::saveCallback(obs_data_t *save_data, bool saving,
				    void *private_data)
{
	const auto instance =
		reinterpret_cast<BetterProjectors *>(private_data);

	if (saving)
		save(save_data, instance);
	else
		load(save_data, instance);
}

void BetterProjectors::save(obs_data_t *save_data, BetterProjectors *instance)
{
	auto pluginData = obs_data_create();
	auto projectorsData = obs_data_array_create();

	for (const auto &dock : instance->docks) {
		auto projector = obs_data_create();

		const char *sourceName = obs_source_get_name(dock->source);

		obs_data_set_string(projector, "source_name", sourceName);
		obs_data_set_string(projector, "dock_id",
				    dock->objectName().toLocal8Bit().data());

		obs_data_array_push_back(projectorsData, projector);

		obs_data_release(projector);
	}

	obs_data_set_array(pluginData, "projectors", projectorsData);
	obs_data_set_obj(save_data, "better-projectors", pluginData);
	obs_data_array_release(projectorsData);
	obs_data_release(pluginData);
}

void BetterProjectors::load(obs_data_t *save_data, BetterProjectors *instance)
{
	auto pluginData = obs_data_get_obj(save_data, "better-projectors");

	if (!pluginData)
		return;

	auto projectorsData = obs_data_get_array(pluginData, "projectors");

	if (!projectorsData)
		return;

	auto count = obs_data_array_count(projectorsData);

	for (size_t i = 0; i < count; i++) {
		auto projector = obs_data_array_item(projectorsData, i);

		const char *sourceName =
			obs_data_get_string(projector, "source_name");

		const char *dockId = obs_data_get_string(projector, "dock_id");

		obs_data_release(projector);

		instance->showProjector(sourceName, dockId);
	}
}
