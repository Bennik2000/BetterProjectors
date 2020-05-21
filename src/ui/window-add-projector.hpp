#pragma once

#include <memory>
#include <QDialog>
#include "ui_window-add-projector.h"

class BetterProjectors;

class AddProjectorWindow : public QDialog {
private:
	BetterProjectors *instance;

public:
	AddProjectorWindow(BetterProjectors *instance, QWidget *parent);

	std::unique_ptr<Ui::AddProjectorWindow> ui;

private slots:
	void radioButtonSceneToggled(bool checked);
	void radioButtonSourceToggled(bool checked);
	void onOkClicked(bool checked);

	
private:
	void updateSourceComboBox();
};
