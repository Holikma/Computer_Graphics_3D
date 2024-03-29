#include   "ViewerWidget.h"

ViewerWidget::ViewerWidget(QSize imgSize, QWidget* parent): QWidget(parent){
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
	}
}
ViewerWidget::~ViewerWidget(){
	delete painter;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size){
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg){
	if (img != nullptr) {
		delete painter;
		delete img;
	}
	img = new QImage(inputImg);
	if (!img) {
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	setDataPtr();
	update();

	return true;
}
bool ViewerWidget::isEmpty(){
	if (img == nullptr) {
		return true;
	}

	if (img->size() == QSize(0, 0)) {
		return true;
	}
	return false;
}

bool ViewerWidget::changeSize(int width, int height) {
	QSize newSize(width, height);

	if (newSize != QSize(0, 0)) {
		if (img != nullptr) {
			delete painter;
			delete img;
		}

		img = new QImage(newSize, QImage::Format_ARGB32);
		if (!img) {
			return false;
		}
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
		update();
	}

	return true;
}

void ViewerWidget::setPixel(int x, int y, uchar r, uchar g, uchar b, uchar a) {
	r = r > 255 ? 255 : (r < 0 ? 0 : r);
	g = g > 255 ? 255 : (g < 0 ? 0 : g);
	b = b > 255 ? 255 : (b < 0 ? 0 : b);
	a = a > 255 ? 255 : (a < 0 ? 0 : a);

	size_t startbyte = y * img->bytesPerLine() + x * 4;
	data[startbyte] = b;
	data[startbyte + 1] = g;
	data[startbyte + 2] = r;
	data[startbyte + 3] = a;
}
void ViewerWidget::setPixel(int x, int y, double valR, double valG, double valB, double valA) {
	valR = valR > 1 ? 1 : (valR < 0 ? 0 : valR);
	valG = valG > 1 ? 1 : (valG < 0 ? 0 : valG);
	valB = valB > 1 ? 1 : (valB < 0 ? 0 : valB);
	valA = valA > 1 ? 1 : (valA < 0 ? 0 : valA);

	size_t startbyte = y * img->bytesPerLine() + x * 4;
	data[startbyte] = static_cast<uchar>(255 * valB);
	data[startbyte + 1] = static_cast<uchar>(255 * valG);
	data[startbyte + 2] = static_cast<uchar>(255 * valR);
	data[startbyte + 3] = static_cast<uchar>(255 * valA);
}
void ViewerWidget::setPixel(int x, int y, const QColor& color) {
	if (color.isValid()) {
		size_t startbyte = y * img->bytesPerLine() + x * 4;

		data[startbyte] = color.blue();
		data[startbyte + 1] = color.green();
		data[startbyte + 2] = color.red();
		data[startbyte + 3] = color.alpha();
	}
}

//Draw functions
void ViewerWidget::drawLine(QPoint start, QPoint end, QColor color, int algType) {
	painter->setPen(QPen(color));
	painter->drawLine(start, end);

	update();
}

void ViewerWidget::clear() {
	img->fill(Qt::white);
	update();
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}

void ViewerWidget::Generate(int type, int strany, int polomer, int rovnobezky, int poludniky) {
	//Function to generate vtk file for Cube
	FILE* file;
	std::string filename = "data.vtk";
	file = fopen(filename.c_str(), "w");
	if (file == NULL) {
		qDebug() << "Error opening file";
		return;
	}
	fprintf(file, "# vtk DataFile Version 3.0\n");
	fprintf(file, "vtk output\n");
	fprintf(file, "ASCII\n");
	fprintf(file, "DATASET POLYDATA\n");
	QVector<QVector3D> body;
	switch (type) {
		case 0: { //Function to generate vtk file for Cube
			fprintf(file, "POINTS %d int\n", strany);
			QVector<QVector3D> points;
			points.append(QVector3D(0, 0, 0));
			points.append(QVector3D(strany, 0, 0));
			points.append(QVector3D(0, strany, 0));
			points.append(QVector3D(0, 0, strany));
			points.append(QVector3D(0, strany, strany));
			points.append(QVector3D(strany, 0, strany));
			points.append(QVector3D(strany, strany, 0));
			points.append(QVector3D(strany, strany, strany));

			for (int i = 0; i < points.size(); i++) {
				fprintf(file, "%d %d %d\n", points[i].x(), points[i].y(), points[i].z());
				qDebug() << points[i].x() << points[i].y() << points[i].z();
			}

			// Generate lines

			fprintf(file, "LINES 12 36\n");
			for (int i = 0; i < points.size(); i++) {
				for (int j = i + 1; j < points.size(); j++) {
					if (points[i].x() == points[j].x() || points[i].y() == points[j].y() || points[i].z() == points[j].z()) {
						fprintf(file, "2 %d %d\n", i, j);
					}
				}
			}

			break;
		}
		case 1: { //Function to generate vtk file for Ball
			fprintf(file, "POINTS %d float\n", strany);
			
			break;
		}
	}
	fclose(file);

	

}