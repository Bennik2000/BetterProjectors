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
