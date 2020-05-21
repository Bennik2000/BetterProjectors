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

#include "projector-widget.hpp"
#include <QWindow>

ProjectorWidget::ProjectorWidget(QWidget *parent, obs_source_t *source)
	: QWidget(parent), source(source)
{
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_StaticContents);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	setAttribute(Qt::WA_NativeWindow);

	connect(windowHandle(), &QWindow::visibleChanged, this,
		&ProjectorWidget::visibleChanged);
	connect(windowHandle(), &QWindow::screenChanged, this,
		&ProjectorWidget::screenChanged);
}

ProjectorWidget::~ProjectorWidget() {}

void ProjectorWidget::visibleChanged(bool visible)
{
	if (!visible)
		return;

	if (!display)
		createDisplay();

	updateSize();
}

void ProjectorWidget::screenChanged(QScreen *)
{
	createDisplay();
	updateSize();
}

void ProjectorWidget::createDisplay()
{
	if (display || !windowHandle()->isExposed())
		return;

	const QSize size = getWidgetPixelSize(this);

	gs_init_data info = {};
	info.cx = size.width();
	info.cy = size.height();
	info.format = GS_BGRA;
	info.zsformat = GS_ZS_NONE;

	qTToGSWindow(winId(), info.window);

	// TODO: Destroy display??
	display = obs_display_create(&info, 0);

	const auto renderCallbackLambda = [](void *data, uint32_t cx,
					     uint32_t cy) {
		static_cast<ProjectorWidget *>(data)->renderCallback(cx, cy);
	};

	obs_display_add_draw_callback(display, renderCallbackLambda, this);
	obs_display_set_background_color(display, 0x000000);
}

void ProjectorWidget::updateSize()
{
	if (!display)
		return;

	QSize size = getWidgetPixelSize(this);
	obs_display_resize(display, size.width(), size.height());
}

void ProjectorWidget::renderCallback(uint32_t cx, uint32_t cy)
{
	if (!source || !display)
		return;

	const uint32_t sourceHeight = obs_source_get_height(source);
	const uint32_t sourceWidth = obs_source_get_width(source);

	const float scaleX = static_cast<float>(cx) / sourceWidth;
	const float scaleY = static_cast<float>(cy) / sourceHeight;
	const float scale = std::min(scaleX, scaleY);

	const float scaledSourceHeight = sourceHeight * scale;
	const float scaledSourceWidth = sourceWidth * scale;

	const float heightGap = cy - scaledSourceHeight;
	const float widthGap = cx - scaledSourceWidth;

	const QSize size = getWidgetPixelSize(this);

	gs_projection_push();
	gs_viewport_push();
	gs_matrix_push();
	gs_matrix_identity();

	gs_matrix_translate3f(widthGap / 2, heightGap / 2, 1.0f);
	gs_matrix_scale3f(scale, scale, 1.0f);

	gs_set_viewport(0, 0, size.width(), size.height());
	gs_ortho(0, size.width(), 0, size.height(), -100.0f, 100.0f);

	obs_source_video_render(source);

	gs_viewport_pop();
	gs_projection_pop();
	gs_matrix_pop();
}

void ProjectorWidget::qTToGSWindow(WId windowId, gs_window &gsWindow)
{
#ifdef _WIN32
	gsWindow.hwnd = (HWND)windowId;
#elif __APPLE__
	gsWindow.view = (id)windowId;
#else
	gsWindow.id = windowId;
	gsWindow.display = QX11Info::display();
#endif
}

QSize ProjectorWidget::getWidgetPixelSize(QWidget *widget)
{
#ifdef SUPPORTS_FRACTIONAL_SCALING
	return widget->size() * widget->devicePixelRatioF();
#else
	return widget->size() * widget->devicePixelRatio();
#endif
}

void ProjectorWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	if (display && isVisible())
		updateSize();
}

void ProjectorWidget::paintEvent(QPaintEvent *event)
{
	if (!display)
		createDisplay();
	
	QWidget::paintEvent(event);
}

QPaintEngine *ProjectorWidget::paintEngine() const
{
	return nullptr;
}
