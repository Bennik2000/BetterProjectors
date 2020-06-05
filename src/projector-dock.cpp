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

#include "projector-dock.hpp"
#include "projector-widget.hpp"
#include "platform.hpp"
#include <QGridLayout>
#include <QResizeEvent>
#include <QMenuBar>

ProjectorDock::ProjectorDock(const char *source, int width, int height,
			     bool isAlwaysOnTop)
	: QDockWidget("Projector",
		      static_cast<QWidget *>(obs_frontend_get_main_window())),
	  width(width),
	  height(height),
	  isAlwaysOnTop(isAlwaysOnTop)
{
	setFeatures(QDockWidget::AllDockWidgetFeatures);
	setWindowFlags(windowFlags() & Qt::WindowMinMaxButtonsHint);

	projectorWidget = new ProjectorWidget(this, source);
	setWidget(projectorWidget);

	connect(this, &QDockWidget::topLevelChanged, this,
		&ProjectorDock::topLevelChanged);

	wasFloating = isFloating();

	setWindowTitle(source);
	adjustSize();
}

QSize ProjectorDock::sizeHint() const
{
	return QSize(width, height);
}

void ProjectorDock::resizeToWidth()
{
	const float sourceHeight =
		obs_source_get_height(projectorWidget->source());
	const float sourceWidth =
		obs_source_get_width(projectorWidget->source());

	width = sourceWidth / sourceHeight * height;
	adjustSize();
}

void ProjectorDock::topLevelChanged(bool topLevel)
{
	if (!wasFloating && isFloating()) {
		resizeToWidth();
		SetAlwaysOnTop(this, isAlwaysOnTop);
	}

	wasFloating = isFloating();
}

void ProjectorDock::resizeEvent(QResizeEvent *event)
{
	width = event->size().width();
	height = event->size().height();
}

void ProjectorDock::closeEvent(QCloseEvent *event)
{
	if (closeCallback)
		closeCallback(this, closeCallbackParameter);

	projectorWidget->setSource(nullptr);
}

void ProjectorDock::mousePressEvent(QMouseEvent *event)
{
	QDockWidget::mousePressEvent(event);

	if (event->button() == Qt::RightButton) {

		QMenu popup(this);

		if (isFloating()) {
			QAction *alwaysOnTopButton =
				new QAction("AlwaysOnTop", this);

			alwaysOnTopButton->setCheckable(true);
			alwaysOnTopButton->setChecked(isAlwaysOnTop);

			connect(alwaysOnTopButton, &QAction::toggled, this,
				&ProjectorDock::AlwaysOnTopToggled);

			popup.addAction(alwaysOnTopButton);
		}

		popup.exec(QCursor::pos());
	}
}

void ProjectorDock::setVisible(bool visible)
{
	QDockWidget::setVisible(visible);
	SetAlwaysOnTop(this, isAlwaysOnTop);
}

void ProjectorDock::AlwaysOnTopToggled(bool alwaysOnTop)
{
	this->isAlwaysOnTop = alwaysOnTop;
	SetAlwaysOnTop(this, isAlwaysOnTop);
}

obs_source_t *ProjectorDock::getSource() const
{
	return projectorWidget->source();
}

bool ProjectorDock::getIsAlwaysOnTop() const
{
	return isAlwaysOnTop;
}

void ProjectorDock::setCloseCallback(projectorDockCallback callback,
				     void *parameter)
{
	closeCallback = callback;
	closeCallbackParameter = parameter;
}
