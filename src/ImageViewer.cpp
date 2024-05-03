#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget* parent): QMainWindow(parent), ui(new Ui::ImageViewerClass){
	ui->setupUi(this);
	vW = new ViewerWidget(QSize(500, 500));
	ui->scrollArea->setWidget(vW);

	ui->scrollArea->setBackgroundRole(QPalette::Dark);
	ui->scrollArea->setAlignment(Qt::AlignCenter);
	ui->scrollArea->setWidgetResizable(true);
	ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	Connect_Sliders_to_SpinBoxes();
	vW->setObjectName("ViewerWidget");
	vW->installEventFilter(this);

	globalColor = Qt::blue;
	QString style_sheet = QString("background-color: #%1;").arg(globalColor.rgba(), 0, 16);
	ui->pushButtonSetColor->setStyleSheet(style_sheet);
}

// Event filters
bool ImageViewer::eventFilter(QObject* obj, QEvent* event){
	if (obj->objectName() == "ViewerWidget") {
		return ViewerWidgetEventFilter(obj, event);
	}
	return false;
}

//ViewerWidget Events
bool ImageViewer::ViewerWidgetEventFilter(QObject* obj, QEvent* event){
	ViewerWidget* w = static_cast<ViewerWidget*>(obj);

	if (!w) {
		return false;
	}

	if (event->type() == QEvent::MouseButtonPress) {
		ViewerWidgetMouseButtonPress(w, event);
	}
	else if (event->type() == QEvent::MouseButtonRelease) {
		ViewerWidgetMouseButtonRelease(w, event);
	}
	else if (event->type() == QEvent::MouseMove) {
		ViewerWidgetMouseMove(w, event);
	}
	else if (event->type() == QEvent::Leave) {
		ViewerWidgetLeave(w, event);
	}
	else if (event->type() == QEvent::Enter) {
		ViewerWidgetEnter(w, event);
	}
	else if (event->type() == QEvent::Wheel) {
		ViewerWidgetWheel(w, event);
	}

	return QObject::eventFilter(obj, event);
}
void ImageViewer::ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
}
void ImageViewer::ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event){
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
}
void ImageViewer::ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event){
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
}
void ImageViewer::ViewerWidgetLeave(ViewerWidget* w, QEvent* event){
}
void ImageViewer::ViewerWidgetEnter(ViewerWidget* w, QEvent* event){
}
void ImageViewer::ViewerWidgetWheel(ViewerWidget* w, QEvent* event){
	QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
}

//ImageViewer Events
void ImageViewer::closeEvent(QCloseEvent* event){
	if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No))
	{
		event->accept();
	}
	else {
		event->ignore();
	}
}

//Image functions
bool ImageViewer::openImage(QString filename){
	QImage loadedImg(filename);
	if (!loadedImg.isNull()) {
		return vW->setImage(loadedImg);
	}
	return false;
}
bool ImageViewer::saveImage(QString filename){
	QFileInfo fi(filename);
	QString extension = fi.completeSuffix();

	QImage* img = vW->getImage();
	return img->save(filename, extension.toStdString().c_str());
}

//Custom functions

void ImageViewer::Connect_Sliders_to_SpinBoxes() {
	connect(ui->Slider_Meridians, SIGNAL(valueChanged(int)), ui->SpinBox_Meridians, SLOT(setValue(int)));
	connect(ui->SpinBox_Meridians, SIGNAL(valueChanged(int)), ui->Slider_Meridians, SLOT(setValue(int)));

	connect(ui->Slider_Parallels, SIGNAL(valueChanged(int)), ui->SpinBox_Parallels, SLOT(setValue(int)));
	connect(ui->SpinBox_Parallels, SIGNAL(valueChanged(int)), ui->Slider_Parallels, SLOT(setValue(int)));

	connect(ui->Slider_Radius, SIGNAL(valueChanged(int)), ui->SpinBox_Radius, SLOT(setValue(int)));
	connect(ui->SpinBox_Radius, SIGNAL(valueChanged(int)), ui->Slider_Radius, SLOT(setValue(int)));

	connect(ui->Slider_Length, SIGNAL(valueChanged(int)), ui->SpinBox_Length, SLOT(setValue(int)));
	connect(ui->SpinBox_Length, SIGNAL(valueChanged(int)), ui->Slider_Length, SLOT(setValue(int)));

	connect(ui->Slider_Zenit, SIGNAL(valueChanged(int)), ui->SpinBox_Zenit, SLOT(setValue(int)));
	connect(ui->SpinBox_Zenit, SIGNAL(valueChanged(int)), ui->Slider_Zenit, SLOT(setValue(int)));
	
	connect(ui->Slider_Azimut, SIGNAL(valueChanged(int)), ui->SpinBox_Azimut, SLOT(setValue(int)));
	connect(ui->SpinBox_Azimut, SIGNAL(valueChanged(int)), ui->Slider_Azimut, SLOT(setValue(int)));

	connect(ui->Slider_Distance, SIGNAL(valueChanged(int)), ui->SpinBox_Distance, SLOT(setValue(int)));
	connect(ui->SpinBox_Distance, SIGNAL(valueChanged(int)), ui->Slider_Distance, SLOT(setValue(int)));
}

//Slots
void ImageViewer::on_actionOpen_triggered(){
	QString folder = settings.value("folder_img_load_path", "").toString();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getOpenFileName(this, "Load image", folder, fileFilter);
	if (fileName.isEmpty()) { return; }

	QFileInfo fi(fileName);
	settings.setValue("folder_img_load_path", fi.absoluteDir().absolutePath());

	if (!openImage(fileName)) {
		msgBox.setText("Unable to open image.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
}
void ImageViewer::on_actionSave_as_triggered(){
	QString folder = settings.value("folder_img_save_path", "").toString();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getSaveFileName(this, "Save image", folder, fileFilter);
	if (!fileName.isEmpty()) {
		QFileInfo fi(fileName);
		settings.setValue("folder_img_save_path", fi.absoluteDir().absolutePath());

		if (!saveImage(fileName)) {
			msgBox.setText("Unable to save image.");
			msgBox.setIcon(QMessageBox::Warning);
		}
		else {
			msgBox.setText(QString("File %1 saved.").arg(fileName));
			msgBox.setIcon(QMessageBox::Information);
		}
		msgBox.exec();
	}
}
void ImageViewer::on_actionClear_triggered(){
	vW->clear();
}
void ImageViewer::on_actionExit_triggered(){
	this->close();
}
void ImageViewer::on_pushButtonSetColor_clicked(){
	QColor newColor = QColorDialog::getColor(globalColor, this);
	if (newColor.isValid()) {
		QString style_sheet = QString("background-color: #%1;").arg(newColor.rgba(), 0, 16);
		ui->pushButtonSetColor->setStyleSheet(style_sheet);
		globalColor = newColor;
	}
}

//Custom Slots
void ImageViewer::on_pushButtonGenerateCube_clicked() {
	vW->get_Object_Data()->Clear_Data();
	vW->Generate_Cube_VTK(ui->SpinBox_Length->value());
	vW->Load_VTK_to_Data();
	vW->Wireframe_Display(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value());
}
void ImageViewer::on_pushButtonGenerateSphere_clicked() {
	vW->Generate_Sphere_VTK(ui->SpinBox_Radius->value(), ui->SpinBox_Meridians->value(), ui->SpinBox_Parallels->value());
	vW->get_Object_Data()->Clear_Data();
	vW->Load_VTK_to_Data();
	vW->Wireframe_Display(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value());
}
//Crate object Slots


void ImageViewer::on_Slider_Meridians_valueChanged(int value) {
	vW->Generate_Object(ui->SpinBox_Length->value(), value, ui->SpinBox_Parallels->value(), ui->SpinBox_Radius->value());
	vW->Visualize_Object(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex());
}

void ImageViewer::on_Slider_Parallels_valueChanged(int value) {
	vW->Generate_Object(ui->SpinBox_Length->value(), ui->SpinBox_Meridians->value(), value, ui->SpinBox_Radius->value());
	vW->Visualize_Object(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex());
}

void ImageViewer::on_Slider_Radius_valueChanged(int value) {
	vW->Generate_Object(ui->SpinBox_Length->value(), ui->SpinBox_Meridians->value(), ui->SpinBox_Parallels->value(), value);
	vW->Visualize_Object(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex());
}

void ImageViewer::on_Slider_Length_valueChanged(int value) {
	vW->Generate_Object(value, ui->SpinBox_Meridians->value(), ui->SpinBox_Parallels->value(), ui->SpinBox_Radius->value());
	vW->Visualize_Object(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex());
}

//Visualize object Slots
void ImageViewer::on_Slider_Zenit_valueChanged(int value) {
	vW->Visualize_Object(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), value, ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex());
}

void ImageViewer::on_Slider_Azimut_valueChanged(int value) {
	vW->Visualize_Object(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), value, ui->comboBoxFrame->currentIndex());
}

void ImageViewer::on_Slider_Distance_valueChanged(int value) {
	vW->Visualize_Object(value, ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex());
}

void ImageViewer::on_comboBoxVision_currentIndexChanged(int index) {
	vW->Visualize_Object(ui->SpinBox_Distance->value(), index, ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex());
}