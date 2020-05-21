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

	setWindowTitle("Create projector");

	connect(ui->radioButtonScene, &QRadioButton::toggled, this,
		&AddProjectorWindow::radioButtonSceneToggled);

	connect(ui->radioButtonSource, &QRadioButton::toggled, this,
		&AddProjectorWindow::radioButtonSourceToggled);

	connect(ui->pushButtonOk, &QPushButton::clicked, this,
		&AddProjectorWindow::onOkClicked);

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
