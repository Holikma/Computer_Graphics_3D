#pragma once
#include <QtWidgets>
#include <iostream>


class Data_Structure {
	private:
		QVector<QVector3D> points;
		QVector<QVector3D*> polygons;
	public:
		Data_Structure() {};
		QVector<QVector3D> get_Points() { return points; };
		QVector<QVector3D*> get_Polygons() { return polygons; };
		void add_Point(QVector3D point) { points.push_back(point); };
		void add_Polygon(QVector3D* polygon) { polygons.push_back(polygon); };
		void Print_Data();
		void Clear_Data();

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
	void Sutherland_Hodgeman(QVector<QPoint> triangle, QColor color);
	void Generate_Cube_VTK(int length);
	void Generate_Sphere_VTK(int radius, int meridians, int parallels);
	void Generate_Object(int length,  int meridians, int parallels, int radius);
	void Visualize_Object(int distance, int vision, int zenit, int azimut, int frame);

	void Load_VTK_to_Data();
	void Perspective_Projection(double distance, double zenit, double azimut);
	void Parallel_Projection(double distance, double zenit, double azimut);
	void Wireframe_Display(double distance, int perspective, double zenit, double azimut);
	void zBuffer_Display(double distance, int perspective, double zenit, double azimut);

	void clear();

public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};