#pragma once
#include <QtWidgets>
#include <iostream>
#include <stdio.h>
#include <limits>
#include <random>

class Data_Structure {
	private:
		QVector<QVector3D> points;
		QVector<QVector3D*> polygons;
		QVector<QColor> colors;
	public:
		Data_Structure() {};
		QVector<QVector3D> get_Points() { return points; };
		QVector<QVector3D*> get_Polygons() { return polygons; };
		QVector<QColor> get_Colors() { return colors; };
		void add_Point(QVector3D point) { points.push_back(point); };
		void add_Polygon(QVector3D* polygon) { polygons.push_back(polygon); };
		void add_Color(QColor color) { colors.push_back(color); };
		void Print_Data();
		void Clear_Data();
		void Print_Points();

};

struct Edge {
	QVector3D start;
	QVector3D end;
	double m;
	int delta_y;
	double x;
	double w;
};

class Light {
	private:
		QVector3D position;
		QColor color;
		QColor ambient;
		QVector<double> POM_Diff_RGB;
		QVector<double>  POM_Refl_RGB;
		QVector<double>  POM_Amb_RGB;
	public:
		Light() {};
		Light(QVector3D pos, QColor col, QColor amb, QVector<double>  diff, QVector<double>  refl, QVector<double>  pamb) : position(pos), color(col), ambient(amb), POM_Diff_RGB(diff), POM_Refl_RGB(refl), POM_Amb_RGB(pamb) {};
		QVector3D get_Position() { return position; };
		QColor get_Color() { return color; };
		QColor get_Ambient() { return ambient; };
		QVector<double>  get_POM_Diff_RGB() { return POM_Diff_RGB; };
		QVector<double> get_POM_Refl_RGB() { return POM_Refl_RGB; };
		QVector<double>  get_POM_Amb_RGB() { return POM_Amb_RGB; };
		const void Print_Light() { qDebug() << "Position: " << position << " Color: " << color << " Ambient: " << ambient << " Diffuse: " << POM_Diff_RGB << " Reflection: " << POM_Refl_RGB << " Ambient: " << POM_Amb_RGB; };
};


class ViewerWidget :public QWidget {
	Q_OBJECT
private:
	QSize areaSize = QSize(0, 0);
	QImage* img = nullptr;
	QPainter* painter = nullptr;
	uchar* data = nullptr;

	bool drawLineActivated = false;
	QPoint drawLineBegin = QPoint(0, 0);
	Data_Structure Object_data;

public:
	ViewerWidget(QSize imgSize, QWidget* parent = Q_NULLPTR);
	~ViewerWidget();
	void resizeWidget(QSize size);
	//Image functions
	bool setImage(const QImage& inputImg);
	QImage* getImage() { return img; };
	bool isEmpty();
	bool changeSize(int width, int height);
	void setPixel(int x, int y, uchar r, uchar g, uchar b, uchar a = 255);
	void setPixel(int x, int y, double valR, double valG, double valB, double valA = 1.);
	void setPixel(int x, int y, const QColor& color);
	bool isInside(int x, int y) { return (x >= 0 && y >= 0 && x < img->width() && y < img->height()) ? true : false; }
	bool isInsidePolygon(int x, int y, QVector<QVector3D> polygon);
	//Draw functions
	void drawLine(QPoint start, QPoint end, QColor color, int algType = 0);
	void setDrawLineBegin(QPoint begin) { drawLineBegin = begin; }
	QPoint getDrawLineBegin() { return drawLineBegin; }
	void setDrawLineActivated(bool state) { drawLineActivated = state; }
	bool getDrawLineActivated() { return drawLineActivated; }
	//Get/Set functions
	uchar* getData() { return data; }
	void setDataPtr() { data = img->bits(); }
	void setPainter() { painter = new QPainter(img); }
	int getImgWidth() { return img->width(); };
	int getImgHeight() { return img->height(); };


	//Custom functions
	Data_Structure* get_Object_Data() { return &Object_data; };
	void DDALine(QPoint start, QPoint end, QColor color);
	QVector<QVector3D> Sutherland_Hodgeman(QVector<QVector3D> triangle, QColor color);
	void Scan_Line(QVector<QVector3D> polygon, QColor color);
	void Load_VTK_to_Data();
	void Generate_Cube_VTK(int length);
	void Generate_Sphere_VTK(int radius, int meridians, int parallels);
	void Generate_Object(int length,  int meridians, int parallels, int radius);
	void Visualize_Object(int distance, int vision, int zenit, int azimut, int frame);
	void Light_Object(Light light, int distance, int vision, int zenit, int azimut, int frame, int shading);
	void Print_Projected_Polygons(QVector<QVector<QVector3D>> Projected_polygons);
	double interpolateZ(int x, int y, QVector<QVector3D> polygon);
	bool isInsideTriangle(QVector<QVector3D> triangle, QVector3D P);
	bool isOnEdge(QVector<QVector3D> triangle, QVector3D P);
	QColor Barycentric(QVector<QVector3D> triangle, QVector3D P, QVector<QColor> color);
	QVector<QVector<QVector3D>> Perspective_Projection(double distance, double zenit, double azimut);
	QVector<QVector<QVector3D>> Parallel_Projection(double distance, double zenit, double azimut);
	void Wireframe_Display(double distance, int perspective, double zenit, double azimut);
	void zBuffer_Display(double distance, int perspective, double zenit, double azimut);
	void zBuffer(QVector<QVector<QVector3D>> Projected_polygons);
	void zBuffer(QVector<QVector<QVector3D>> Projected_polygons, Light bulb, int distance, int shading);
	QColor Phong_Model(QVector<QVector3D> polygon, Light bulb, QVector3D P, int distance);
	QColor Gouraud_Model(QVector<QVector3D> polygon, Light bulb, QVector3D P, int distance);
	QColor Flat_Model(QVector<QVector3D> polygon, Light bulb, QVector3D P, int distance);
	void clear();

public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};