#include   "ViewerWidget.h"

void Data_Structure::Print_Data() {
	qDebug() << "Points:";
	for (int i = 0; i < points.size(); i++) {
		qDebug() << points[i];
	}
	qDebug() << "Polygons:";
	for (int i = 0; i < polygons.size(); i++) {
		qDebug() << polygons[i][0] << polygons[i][1] << polygons[i][2];
	}
}

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

//Custom functions
void ViewerWidget::Generate_Cube_VTK(int length) {
	length = length / 2;
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
	fprintf(file, "POINTS %d int\n", 8);
	QVector<QVector3D> points;
	//Generate points for cube with center of cube be  0 0 0 
	points.push_back(QVector3D(-length, -length, -length));
	points.push_back(QVector3D(length, -length, -length));
	points.push_back(QVector3D(length, length, -length));
	points.push_back(QVector3D(-length, length, -length));
	points.push_back(QVector3D(-length, -length, length));
	points.push_back(QVector3D(length, -length, length));
	points.push_back(QVector3D(length, length, length));
	points.push_back(QVector3D(-length, length, length));

	for (int i = 0; i < 8; i++) {
		fprintf(file, "%d %d %d\n", (int)points[i].x(), (int)points[i].y(), (int)points[i].z());
	}
	fprintf(file, "POLYGONS 12 48\n");
	fprintf(file, "3 0 1 5\n");
	fprintf(file, "3 0 5 4\n");
	fprintf(file, "3 1 2 6\n");
	fprintf(file, "3 1 5 6\n");
	fprintf(file, "3 2 3 7\n");
	fprintf(file, "3 2 7 6\n");
	fprintf(file, "3 3 0 4\n");
	fprintf(file, "3 3 4 7\n");
	fprintf(file, "3 0 1 2\n");
	fprintf(file, "3 0 2 3\n");
	fprintf(file, "3 4 5 6\n");
	fprintf(file, "3 4 7 6\n");
			
	qDebug() << "Cube generated";
	fclose(file);
}
void ViewerWidget::Generate_Sphere_VTK(int radius, int meridians, int parallels) {
	FILE* file;
	std::string filename = "data.vtk";
	file = fopen(filename.c_str(), "w");
	if (file == NULL) {
		qDebug() << "Error opening file";
		return;
	}
	QVector<QVector3D> points;
	double theta_Increment = M_PI / static_cast<double>(parallels + 1);
	double gamma_Increment = 2 * M_PI / static_cast<double>(meridians);
	double theta = -(M_PI / 2.0) + theta_Increment;
	double gamma = gamma_Increment;
	int number_of_points = parallels * meridians + 2;

	// Write VTK header
	fprintf(file, "# vtk DataFile Version 3.0\nvtk output\nASCII\nDATASET POLYDATA\nPOINTS %d float\n", number_of_points);
	fprintf(file, "0 0 %d\n", -radius);

	points.push_back(QVector3D(0, 0, -radius));

	for (int i = 1; i <= meridians; i++) {
		gamma = gamma_Increment;
		for (int j = 0; j < parallels; j++) {
			double x = radius * cos(theta) * cos(gamma);
			double y = radius * cos(theta) * sin(gamma);
			double z = radius * sin(theta);
			fprintf(file, "%f %f %f\n", x, y, z);
			points.push_back(QVector3D(x, y, z));
			gamma += gamma_Increment;
		}
		theta += theta_Increment;
	}

	points.push_back(QVector3D(0, 0, radius));
	fprintf(file, "0 0 %d\n", radius);

	// Calculate number of faces
	int faces;
	if (meridians == 1) {
		faces = parallels * (meridians + 1);
	}
	else {
		faces = 2 * parallels * meridians;
	}

	// Write polygons
	fprintf(file, "POLYGONS %d %d\n", faces, 4 * faces);

	// Write lower triangles
	for (int i = 1; i < parallels; i++) {
		fprintf(file, "3 0 %d %d\n", i, i + 1);
	}
	fprintf(file, "3 0 %d 1\n", parallels);

	for (int i = 0; i < meridians - 1; i++) {
		for (int j = 1 + i * parallels; j < parallels * (i + 1); j++) {
			fprintf(file, "3 %d %d %d\n", j, j + parallels, j + 1 + parallels);
			fprintf(file, "3 %d %d %d\n", j, j + parallels + 1, j + 1);
		}
		fprintf(file, "3 %d %d %d\n", parallels * (i + 1), parallels * (i + 2), parallels * (i + 1) + 1);
		fprintf(file, "3 %d %d %d\n", parallels * (i + 1), 1 + parallels * (i + 1), 1 + parallels * i);
	}
	// Write upper triangles
	for (int i = number_of_points - parallels - 1; i < number_of_points - 2; i++) {
		fprintf(file, "3 %d %d %d\n", number_of_points - 1, i + 1, i);
	}
	fprintf(file, "3 %d %d %d\n", number_of_points - 1, number_of_points - 1 - parallels, number_of_points - 2);

	fclose(file);
	qDebug() << "Sphere generated";
}
void ViewerWidget::Load_VTK_to_Data() {
	FILE* file;
	file = fopen("data.vtk", "r");
	QTextStream in(file);
	QString line;
	while (!line.startsWith("POINTS")) {
		line = in.readLine();
	}
	QStringList list = line.split(" ");

	int points = list[1].toInt();
	for (int i = 0; i < points; i++) {
		line = in.readLine();
		list = line.split(" ");
		QVector3D point(list[0].toFloat(), list[1].toFloat(), list[2].toFloat());
		Object_data.add_Point(point);
	}
	while (!line.startsWith("POLYGONS")) {
		line = in.readLine();
	}

	list = line.split(" ");
	int number_of_polygons = list[1].toInt();
	for (int i = 0; i < number_of_polygons; i++) {
		line = in.readLine();
		list = line.split(" ");
		QVector3D* polygon { new QVector3D[3] };
		for (int j = 0; j < 3; j++) {
			polygon[j] = Object_data.get_Points()[list[j + 1].toInt()];
		}
		Object_data.add_Polygon(polygon);
	}	
	Object_data.Print_Data();
	fclose(file);

}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}
