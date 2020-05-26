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

#include <obs-frontend-api.h>
#include "window-add-projector.hpp"
#include "../projector-dock.hpp"
#include "../better-projectors.hpp"

AddProjectorWindow::AddProjectorWindow(BetterProjectors *instance,
				       QWidget *parent)
	: QDialog(parent), ui(new Ui::AddProjectorWindow), instance(instance)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	ui->setupUi(this);

	previewWidget = new ProjectorWidget(this, nullptr);

	ui->previewLayout->addWidget(previewWidget);

	previewWidget->setMinimumWidth(400);
	previewWidget->setMinimumHeight(400 * (9.0f / 16.0f));

	setWindowTitle("Create projector");

	connect(ui->radioButtonScene, &QRadioButton::toggled, this,
		&AddProjectorWindow::radioButtonSceneToggled);

	connect(ui->radioButtonSource, &QRadioButton::toggled, this,
		&AddProjectorWindow::radioButtonSourceToggled);

	connect(ui->pushButtonOk, &QPushButton::clicked, this,
		&AddProjectorWindow::onOkClicked);

	connect(ui->listWidget, &QListWidget::itemSelectionChanged, this,
		&AddProjectorWindow::sourceSelectionChanged);

	updateSourceComboBox();
}

void AddProjectorWindow::radioButtonSceneToggled(bool checked)
{
	if (checked)
		updateSourceComboBox();
}

void AddProjectorWindow::radioButtonSourceToggled(bool checked)
{
	if (checked)
		updateSourceComboBox();
}

void AddProjectorWindow::onOkClicked(bool checked)
{
	if (ui->listWidget->selectedItems().size() == 1) {
		instance->showProjector(ui->listWidget->selectedItems()[0]
						->text()
						.toLocal8Bit()
						.data());
	}

	close();
}

void AddProjectorWindow::sourceSelectionChanged()
{
	if (ui->listWidget->selectedItems().size() == 1) {
		previewWidget->setSource(ui->listWidget->selectedItems()[0]
						 ->text()
						 .toLocal8Bit()
						 .data());
	}
}

void AddProjectorWindow::updateSourceComboBox()
{
	ui->listWidget->clear();

	const auto enumSources = [](void *param, obs_source_t *source) {
		const AddProjectorWindow *window =
			reinterpret_cast<AddProjectorWindow *>(param);

		const QString name = obs_source_get_name(source);

		const enum obs_source_type type = obs_source_get_type(source);

		const bool hasVideo = (obs_source_get_output_flags(source) &
				       OBS_SOURCE_VIDEO) != 0;

		if ((type == OBS_SOURCE_TYPE_INPUT ||
		     type == OBS_SOURCE_TYPE_SCENE) &&
		    hasVideo)
			window->ui->listWidget->addItem(name);

		return true;
	};

	if (ui->radioButtonScene->isChecked())
		obs_enum_scenes(enumSources, this);
	else if (ui->radioButtonSource->isChecked())
		obs_enum_sources(enumSources, this);
}
