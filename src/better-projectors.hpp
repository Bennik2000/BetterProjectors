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
#include <vector>
#include "projector-dock.hpp"

class BetterProjectors {

public:
	std::vector<ProjectorDock *> docks;

public:
	BetterProjectors();
	~BetterProjectors();

	void showProjector(const char *name, const char *dockId = "");

private:
	void initialize();
	void registerToolsAction();
	void registerSaveCallback();
	static void saveCallback(obs_data_t *save_data, bool saving,
				 void *data);

	static void save(obs_data_t *save_data, BetterProjectors *instance);
	static void load(obs_data_t *save_data, BetterProjectors *instance);
};
