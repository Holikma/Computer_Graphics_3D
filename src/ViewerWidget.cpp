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
void Data_Structure::Clear_Data() {
	points.clear();
	polygons.clear();
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
void ViewerWidget::DDALine(QPoint start, QPoint end, QColor color) {
	float dx = end.x() - start.x();
	float dy = end.y() - start.y();

	// Calculate the number of steps
	float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	// Increment for each step
	float xinc = dx / steps;
	float yinc = dy / steps;

	float x = start.x();
	float y = start.y();

	for (int i = 0; i < steps; i++) {
		// Check if coordinates are within the clipping region
		if (isInside(round(x), round(y))) {
			setPixel(round(x), round(y), color);
		}

		x += xinc;
		y += yinc;
	}
}
QVector<QVector3D> ViewerWidget::Sutherland_Hodgeman(QVector<QVector3D> triangle, QColor color) {
	QVector<QVector3D> W; // Initialize a vector to hold the vertices of the clipped polygon
	QVector<QVector3D> polygon = triangle;
	int edges[4] = { 0,0,-(img->width()-1),-(img->height() - 1)};

	for (int j = 0; j < 4; j++) {
		if (polygon.size() == 0) {
			break;
		}
		QVector3D S = polygon[polygon.size() - 1]; // Initialize S to the last vertex
		double xmin = edges[j];
		for (int i = 0; i < polygon.size(); i++) {
			QVector3D Vi = polygon[i];

			if (Vi.x() >= xmin) {
				if (S.x() >= xmin) {
					W.append(Vi);
				}
				else {
					// Calculate the intersection point P between the clipping edge and the line segment formed by S and Vi
					QVector3D P = QVector3D(xmin, S.y() + (xmin - S.x()) * (Vi.y() - S.y()) / (double)(Vi.x() - S.x()), S.z());
					W.append(P);
					W.append(Vi);
				}
			}
			else {
				if (S.x() >= xmin) {
					QVector3D P = QVector3D(xmin, S.y() + (xmin - S.x()) * (Vi.y() - S.y()) / (double)(Vi.x() - S.x()), S.z());
					W.append(P);
				}
			}
			S = Vi;
		}
		polygon = W;
		W.clear();
		// Rotate the polygon clockwise to handle the next clipping edge
		for (int k = 0; k < polygon.size(); k++) {
			QVector3D swap = polygon[k];
			polygon[k].setX(swap.y());
			polygon[k].setY(-swap.x());
		}
	}
	
	for (int i = 0; i < polygon.size() - 1; i++) {
		QPoint p1 = QPoint(polygon[i].x(), polygon[i].y());
		QPoint p2 = QPoint(polygon[i + 1].x(), polygon[i + 1].y());
		DDALine(p1, p2, color);
	}
	if (polygon.size() > 1) {
		QPoint p1 = QPoint(polygon[polygon.size() - 1].x(), polygon[polygon.size() - 1].y());
		QPoint p2 = QPoint(polygon[0].x(), polygon[0].y());
		DDALine(p1, p2, color);
	}

	update();
	return polygon;
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
	double theta_Increment = M_PI / static_cast<double>(meridians + 1);
	double gamma_Increment = 2 * M_PI / static_cast<double>(parallels);
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
	
	if (meridians != 1) {
		for (int i = 0; i < meridians - 1; i++) {
			for (int j = 1 + i * parallels; j < parallels * (i + 1); j++) {
				fprintf(file, "3 %d %d %d\n", j, j + parallels, j + 1 + parallels);
				fprintf(file, "3 %d %d %d\n", j, j + parallels + 1, j + 1);
			}
			fprintf(file, "3 %d %d %d\n", parallels * (i + 1), parallels * (i + 2), parallels * (i + 1) + 1);
			fprintf(file, "3 %d %d %d\n", parallels * (i + 1), 1 + parallels * (i + 1), 1 + parallels * i);
		}
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
	//clear data
	Object_data.Clear_Data();
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
	
	for (int i = 0; i < number_of_polygons; i++) {
		QVector<QColor> color;
		color.push_back(QColor(rand() % 256, rand() % 256, rand() % 256));
		color.push_back(QColor(rand() % 256, rand() % 256, rand() % 256));
		color.push_back(QColor(rand() % 256, rand() % 256, rand() % 256));
		Object_data.add_Color(color);
	}

	fclose(file);

}
QColor ViewerWidget::Barycentric(QVector<QVector3D> triangle, QVector3D P, QVector<QColor> color) {
	QVector3D A = triangle[0];
	QVector3D B = triangle[1];
	QVector3D C = triangle[2];

	// Calculate area of the triangle ABC
	double A_tri = qAbs((A.x() * (B.y() - C.y()) + B.x() * (C.y() - A.y()) + C.x() * (A.y() - B.y())) / 2.0);

	// Calculate areas of sub-triangles PBC, APC, and ABP
	double A0_tri = qAbs((P.x() * (B.y() - C.y()) + B.x() * (C.y() - P.y()) + C.x() * (P.y() - B.y())) / 2.0);
	double A1_tri = qAbs((A.x() * (P.y() - C.y()) + P.x() * (C.y() - A.y()) + C.x() * (A.y() - P.y())) / 2.0);
	double A2_tri = qAbs((A.x() * (B.y() - P.y()) + B.x() * (P.y() - A.y()) + P.x() * (A.y() - B.y())) / 2.0);

	// Calculate barycentric coordinates
	double lambda0 = A0_tri / A_tri;
	double lambda1 = A1_tri / A_tri;
	double lambda2 = A2_tri / A_tri;

	// Interpolate color
	double red = lambda0 * color[0].redF() + lambda1 * color[1].redF() + lambda2 * color[2].redF();
	double green = lambda0 * color[0].greenF() + lambda1 * color[1].greenF() + lambda2 * color[2].greenF();
	double blue = lambda0 * color[0].blueF() + lambda1 * color[1].blueF() + lambda2 * color[2].blueF();

	return QColor::fromRgbF(red, green, blue);
}
double ViewerWidget::interpolateZ(int x, int y, QVector<QVector3D> polygon) {
	double z = 0.0;
	int numVertices = polygon.size();
	for (int i = 0; i < numVertices; ++i) {
		int j = (i + 1) % numVertices;
		double x0 = polygon[i].x();
		double y0 = polygon[i].y();
		double x1 = polygon[j].x();
		double y1 = polygon[j].y();
		double x2 = x;
		double y2 = y;
		double lambda1 = ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2)) / ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));
		double lambda2 = ((y2 - y0) * (x1 - x2) + (x0 - x2) * (y1 - y2)) / ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));
		double lambda3 = 1 - lambda1 - lambda2;
		z += lambda1 * polygon[i].z() + lambda2 * polygon[j].z() + lambda3 * polygon[(i + 2) % numVertices].z();
	}
	return z;
}
bool ViewerWidget::isInsideTriangle(QVector<QVector3D> triangle, QVector3D P) {
	QVector3D A = triangle[0];
	QVector3D B = triangle[1];
	QVector3D C = triangle[2];

	int as_x = P.x() - A.x();
	int as_y = P.y() - A.y();

	// Urèíme, na ktorej strane priamky AB sa bod P nachádza
	bool s_ab = (B.x() - A.x()) * as_y - (B.y() - A.y()) * as_x > 0;

	// Ak sa bod P nachádza na opaènej strane priamky AC, nie je vnútri trojuholníka
	if ((C.x() - A.x()) * as_y - (C.y() - A.y()) * as_x > 0 == s_ab) return false;

	// Ak sa bod P nachádza na opaènej strane priamky BC, nie je vnútri trojuholníka
	if ((C.x() - B.x()) * (P.y() - B.y()) - (C.y() - B.y()) * (P.x() - B.x()) > 0 != s_ab) return false;
	return true;
}
bool ViewerWidget::isOnEdge(QVector<QVector3D> triangle, QVector3D P) {
	QVector3D A = triangle[0];
	QVector3D B = triangle[1];
	QVector3D C = triangle[2];

	// Check if P is on edge AB
	if (P.x() == A.x() && P.y() == A.y()) return true;
	if (P.x() == B.x() && P.y() == B.y()) return true;
	if ((P.x() - A.x()) * (B.y() - A.y()) - (P.y() - A.y()) * (B.x() - A.x()) == 0) return true;

	// Check if P is on edge BC
	if (P.x() == B.x() && P.y() == B.y()) return true;
	if (P.x() == C.x() && P.y() == C.y()) return true;
	if ((P.x() - B.x()) * (C.y() - B.y()) - (P.y() - B.y()) * (C.x() - B.x()) == 0) return true;

	// Check if P is on edge AC
	if (P.x() == A.x() && P.y() == A.y()) return true;
	if (P.x() == C.x() && P.y() == C.y()) return true;
	if ((P.x() - A.x()) * (C.y() - A.y()) - (P.y() - A.y()) * (C.x() - A.x()) == 0) return true;

	return false;
}
void ViewerWidget::Generate_Object(int length, int meridians, int parallels, int radius) {
	clear();
	Object_data.Clear_Data();
	if (meridians == 0 && parallels == 0)
		Generate_Cube_VTK(length);
	else
		Generate_Sphere_VTK(radius, meridians, parallels);
	Load_VTK_to_Data();
}
void ViewerWidget::Visualize_Object(int distance, int vision, int zenit, int azimut, int frame, Light Bulb, int shading) {
	clear();
	if (!frame) {
		Wireframe_Display(distance, vision, zenit * M_PI / 180., azimut * M_PI / 180.);
	}
	else {
		zBuffer_Display(distance, vision, zenit * M_PI / 180., azimut * M_PI / 180., Bulb, shading);
	}
}
QVector<QVector<QVector3D>> ViewerWidget::Perspective_Projection(double distance, double zenit, double azimut) {
	int center_x = img->width() / 2;
	int center_y = img->height() / 2;
	QVector<QVector3D> points = Object_data.get_Points();
	QVector<QVector3D*> polygons = Object_data.get_Polygons();
	QVector<QVector<QVector3D>> projectedPolygons;
	// Calculate camera orientation vectors
	QVector3D n = QVector3D(sin(zenit) * sin(azimut), sin(zenit) * cos(azimut), cos(zenit));
	QVector3D u = QVector3D(sin(zenit + M_PI_2) * sin(azimut), sin(zenit + M_PI_2) * cos(azimut), cos(zenit + M_PI_2));
	QVector3D v = QVector3D::crossProduct(n, u);

	for (int i = 0; i < polygons.size(); i++) {
		QVector3D* polygon = polygons[i];
		QVector<QVector3D> projectedPolygon;

		// Transform vertices to camera space
		QVector3D vertex1 = polygon[0];
		QVector3D vertex2 = polygon[1];
		QVector3D vertex3 = polygon[2];
		QVector3D p1(QVector3D::dotProduct(vertex1, v), QVector3D::dotProduct(vertex1, u), QVector3D::dotProduct(vertex1, n));
		QVector3D p2(QVector3D::dotProduct(vertex2, v), QVector3D::dotProduct(vertex2, u), QVector3D::dotProduct(vertex2, n));
		QVector3D p3(QVector3D::dotProduct(vertex3, v), QVector3D::dotProduct(vertex3, u), QVector3D::dotProduct(vertex3, n));

		// Project vertices to the image plane with z axis

		projectedPolygon.push_back(QVector3D(distance * p1.x() / (distance - p1.z()) + center_x, distance * p1.y() / (distance - p1.z()) + center_y, p1.z()));
		projectedPolygon.push_back(QVector3D(distance * p2.x() / (distance - p2.z()) + center_x, distance * p2.y() / (distance - p2.z()) + center_y, p2.z()));
		projectedPolygon.push_back(QVector3D(distance * p3.x() / (distance - p3.z()) + center_x, distance * p3.y() / (distance - p3.z()) + center_y, p3.z()));

		// Draw projected polygon
		projectedPolygon = Sutherland_Hodgeman(projectedPolygon, Qt::black);
		projectedPolygons.push_back(projectedPolygon);

	}
	update();
	return projectedPolygons;
}
QVector<QVector<QVector3D>> ViewerWidget::Parallel_Projection(double distance, double zenit, double azimut) {
	int center_x = img->width() / 2;
	int center_y = img->height() / 2;
	QVector<QVector3D> points = Object_data.get_Points();
	QVector<QVector3D*> polygons = Object_data.get_Polygons();
	QVector<QVector<QVector3D>> projectedPolygons;
	// Calculate camera orientation vectors
	QVector3D n = QVector3D(sin(zenit) * sin(azimut), sin(zenit) * cos(azimut), cos(zenit));
	QVector3D u = QVector3D(sin(zenit + M_PI_2) * sin(azimut), sin(zenit + M_PI_2) * cos(azimut), cos(zenit + M_PI_2));
	QVector3D v = QVector3D::crossProduct(n, u);

	for (int i = 0; i < polygons.size(); i++) {
		QVector3D* polygon = polygons[i];
		QVector<QVector3D> projectedPolygon;

		// Transform vertices to camera space
		
		QVector3D vertex1 = polygon[0];
		QVector3D vertex2 = polygon[1];
		QVector3D vertex3 = polygon[2];
		QVector3D p1(QVector3D::dotProduct(vertex1, v), QVector3D::dotProduct(vertex1, u), QVector3D::dotProduct(vertex1, n));
		QVector3D p2(QVector3D::dotProduct(vertex2, v), QVector3D::dotProduct(vertex2, u), QVector3D::dotProduct(vertex2, n));
		QVector3D p3(QVector3D::dotProduct(vertex3, v), QVector3D::dotProduct(vertex3, u), QVector3D::dotProduct(vertex3, n));
		projectedPolygon.push_back(QVector3D(p1.x() + center_x, p1.y() + center_y, p1.z()));
		projectedPolygon.push_back(QVector3D(p2.x() + center_x, p2.y() + center_y, p2.z()));
		projectedPolygon.push_back(QVector3D(p3.x() + center_x, p3.y() + center_y, p3.z()));


		// Draw projected polygon
		projectedPolygon = Sutherland_Hodgeman(projectedPolygon, Qt::black);
		projectedPolygons.push_back(projectedPolygon);
	}
	update();
	return projectedPolygons;
}
void ViewerWidget::Wireframe_Display(double distance, int vision, double zenit, double azimut) {
	QVector<QVector<QVector3D>> Projection;
	switch (vision) {
		case 0:
			Projection = Parallel_Projection(distance, zenit, azimut);
			break;
		case 1:
			Projection = Perspective_Projection(distance, zenit, azimut);
			break;
	}
}
void ViewerWidget::zBuffer_Display(double distance, int vision, double zenit, double azimut, Light Bulb, int shading) {
	QVector<QVector<QVector3D>> Projection;
	Light bulb;
	switch (vision) {
		case 0:
			Projection = Parallel_Projection(distance, zenit, azimut);
			zBuffer(Projection, bulb, shading);
			break;
		case 1:
			Projection = Perspective_Projection(distance, zenit, azimut);
			zBuffer(Projection, bulb, shading);
			break;
	}
}
void ViewerWidget::zBuffer(QVector<QVector<QVector3D>> projectedPolygons, Light bulb, int shading) {
	// Image dimensions
	int width = img->width();
	int height = img->height();

	// Initialize arrays for storing color and depth
	QVector<QVector<QColor>> F(width, QVector<QColor>(height, Qt::black)); // Color points
	QVector<QVector<double>> Z(width, QVector<double>(height, -std::numeric_limits<double>::infinity())); // Depth values

	// Write background color and low depth values
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			F[x][y] = Qt::white; // Background color
		}
	}
	// Iterate over each projected polygon
	for (int i = 0; i < projectedPolygons.size(); ++i) {
		const auto& polygon = projectedPolygons[i];
		QVector<QColor> polygon_colors = Object_data.get_Colors()[i];
		// Calculate bounding box of the polygon
		int minX = std::numeric_limits<int>::max();
		int maxX = std::numeric_limits<int>::min();
		int minY = std::numeric_limits<int>::max();
		int maxY = std::numeric_limits<int>::min();
		for (const auto& point : polygon) {
			int x = point.x();
			int y = point.y();
			minX = std::min(minX, x);
			maxX = std::max(maxX, x);
			minY = std::min(minY, y);
			maxY = std::max(maxY, y);
		}

		// Iterate over the bounding box
		for (int x = minX; x < maxX; ++x) {
			for (int y = minY; y < maxY; ++y) {
				// Check if the pixel is inside the polygon
				if (isInsideTriangle(polygon, QVector3D(x, y, 0))) {
					double z = interpolateZ(x, y, polygon); // Interpolate z-coordinate
					if (z > Z[x][y]) {
						Z[x][y] = z;
						F[x][y] = Barycentric(polygon, QVector3D(x, y, z), polygon_colors); // Interpolate color
					}
				}
			}
		}
	}

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			img->setPixel(x, y, F[x][y].rgb()); // Set pixel color
		}
	}
	update();
}

void ViewerWidget::Print_Projected_Polygons(QVector<QVector<QVector3D>> Projected_polygons) {
	for (int i = 0; i < Projected_polygons.size(); i++) {
		qDebug() << "Polygon " << i;
		for (int j = 0; j < Projected_polygons[i].size(); j++) {
			qDebug() << Projected_polygons[i][j];
		}
	}
}
//Slots
void ViewerWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}
