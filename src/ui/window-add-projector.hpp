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

#include <memory>
#include <QDialog>
#include "ui_window-add-projector.h"
#include "../projector-widget.hpp"

class BetterProjectors;

class AddProjectorWindow : public QDialog {
private:
	BetterProjectors *instance;
	ProjectorWidget *previewWidget;

public:
	AddProjectorWindow(BetterProjectors *instance, QWidget *parent);

	std::unique_ptr<Ui::AddProjectorWindow> ui;

private slots:
	void radioButtonSceneToggled(bool checked);
	void radioButtonSourceToggled(bool checked);
	void onOkClicked(bool checked);
	void sourceSelectionChanged();

private:
	void updateSourceComboBox();
};
