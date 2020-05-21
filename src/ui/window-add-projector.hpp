#pragma once

#include <memory>
#include <QDialog>
#include "ui_window-add-projector.h"

class AddProjectorWindow : public QDialog {
	

public:
	explicit AddProjectorWindow(QWidget *parent = nullptr);

	std::unique_ptr<Ui::AddProjectorWindow> ui;

private slots:
	void radioButtonSceneToggled(bool checked);
	void radioButtonSourceToggled(bool checked);
	void onOkClicked(bool checked);

	
private:
	void updateSourceComboBox();
};
