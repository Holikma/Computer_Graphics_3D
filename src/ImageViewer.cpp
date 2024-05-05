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
	connect(ui->Slider_Source_X, SIGNAL(valueChanged(int)), ui->SpinBox_X, SLOT(setValue(int)));
	connect(ui->SpinBox_X, SIGNAL(valueChanged(int)), ui->Slider_Source_X, SLOT(setValue(int)));
	connect(ui->Slider_Source_Y, SIGNAL(valueChanged(int)), ui->SpinBox_Y, SLOT(setValue(int)));
	connect(ui->SpinBox_Y, SIGNAL(valueChanged(int)), ui->Slider_Source_Y, SLOT(setValue(int)));
	connect(ui->Slider_Source_Z, SIGNAL(valueChanged(int)), ui->SpinBox_Z, SLOT(setValue(int)));
	connect(ui->SpinBox_Z, SIGNAL(valueChanged(int)), ui->Slider_Source_Z, SLOT(setValue(int)));
	connect(ui->Slider_SColor_R, SIGNAL(valueChanged(int)), ui->SpinBox_SColor_R, SLOT(setValue(int)));
	connect(ui->SpinBox_SColor_R, SIGNAL(valueChanged(int)), ui->Slider_SColor_R, SLOT(setValue(int)));
	connect(ui->Slider_SColor_G, SIGNAL(valueChanged(int)), ui->SpinBox_SColor_G, SLOT(setValue(int)));
	connect(ui->SpinBox_SColor_G, SIGNAL(valueChanged(int)), ui->Slider_SColor_G, SLOT(setValue(int)));
	connect(ui->Slider_SColor_B, SIGNAL(valueChanged(int)), ui->SpinBox_SColor_B, SLOT(setValue(int)));
	connect(ui->SpinBox_SColor_B, SIGNAL(valueChanged(int)), ui->Slider_SColor_B, SLOT(setValue(int)));
	connect(ui->Slider_AColor_R, SIGNAL(valueChanged(int)), ui->SpinBox_AColor_R, SLOT(setValue(int)));
	connect(ui->SpinBox_AColor_R, SIGNAL(valueChanged(int)), ui->Slider_AColor_R, SLOT(setValue(int)));
	connect(ui->Slider_AColor_G, SIGNAL(valueChanged(int)), ui->SpinBox_AColor_G, SLOT(setValue(int)));
	connect(ui->SpinBox_AColor_G, SIGNAL(valueChanged(int)), ui->Slider_AColor_G, SLOT(setValue(int)));
	connect(ui->Slider_AColor_B, SIGNAL(valueChanged(int)), ui->SpinBox_AColor_B, SLOT(setValue(int)));
	connect(ui->SpinBox_AColor_B, SIGNAL(valueChanged(int)), ui->Slider_AColor_B, SLOT(setValue(int)));
	connect(ui->Slider_Zenit, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Azimut, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Distance, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Source_X, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Source_Y, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Source_Z, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_SColor_R, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_SColor_G, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_SColor_B, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_AColor_R, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_AColor_G, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_AColor_B, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Diff_R, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Diff_G, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Diff_B, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Refl_R, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Refl_G, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Refl_B, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Amb_R, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Amb_G, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->Slider_Amb_B, SIGNAL(valueChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->checkBoxAddLight, SIGNAL(stateChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->comboBoxVision, SIGNAL(currentIndexChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->comboBoxFrame, SIGNAL(currentIndexChanged(int)), this, SLOT(Update_visuals()));
	connect(ui->comboBoxShading, SIGNAL(currentIndexChanged(int)), this, SLOT(Update_visuals()));
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
	vW->clear();
	vW->get_Object_Data()->Clear_Data();
	vW->Generate_Cube_VTK(ui->SpinBox_Length->value());
	vW->Load_VTK_to_Data(); 
	Update_visuals();
}

void ImageViewer::on_pushButtonGenerateSphere_clicked() {
	vW->clear();
	vW->Generate_Sphere_VTK(ui->SpinBox_Radius->value(), ui->SpinBox_Meridians->value(), ui->SpinBox_Parallels->value());
	vW->get_Object_Data()->Clear_Data();
	vW->Load_VTK_to_Data();
	Update_visuals();
}
//Crate object Slots

void ImageViewer::on_Slider_Meridians_valueChanged(int value) {
	vW->Generate_Object(ui->SpinBox_Length->value(), value, ui->SpinBox_Parallels->value(), ui->SpinBox_Radius->value());
	Update_visuals();
}

void ImageViewer::on_Slider_Parallels_valueChanged(int value) {
	vW->Generate_Object(ui->SpinBox_Length->value(), ui->SpinBox_Meridians->value(), value, ui->SpinBox_Radius->value());
	Update_visuals();
}

void ImageViewer::on_Slider_Radius_valueChanged(int value) {
	vW->Generate_Object(ui->SpinBox_Length->value(), ui->SpinBox_Meridians->value(), ui->SpinBox_Parallels->value(), value);
	Update_visuals();
}

void ImageViewer::on_Slider_Length_valueChanged(int value) {
	vW->Generate_Cube_VTK(value);
	vW->Load_VTK_to_Data();
	Update_visuals();
}

//Visualize object Slots

void ImageViewer::Update_visuals() {
	vW->clear();
	vW->Visualize_Object(ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex());
	if (ui->checkBoxAddLight->isChecked() && ui->comboBoxFrame->currentIndex() == 1) {
		Light bulb(
			QVector3D(ui->SpinBox_X->value(), ui->SpinBox_Y->value(), ui->SpinBox_Z->value()),
			QColor(ui->SpinBox_SColor_R->value(), ui->SpinBox_SColor_G->value(), ui->SpinBox_SColor_B->value()),
			QColor(ui->SpinBox_AColor_R->value(), ui->SpinBox_AColor_G->value(), ui->SpinBox_AColor_B->value()),
			QVector<double>{ui->Slider_Diff_R->value() / 100., ui->Slider_Diff_G->value() / 100., ui->Slider_Diff_B->value() / 100.},
			QVector<double>{ui->Slider_Refl_R->value() / 100., ui->Slider_Refl_G->value() / 100., ui->Slider_Refl_B->value() / 100.},
			QVector<double>{ui->Slider_Amb_R->value() / 100., ui->Slider_Amb_G->value() / 100., ui->Slider_Amb_B->value() / 100.}
		);
		vW->Light_Object(bulb, ui->SpinBox_Distance->value(), ui->comboBoxVision->currentIndex(), ui->SpinBox_Zenit->value(), ui->SpinBox_Azimut->value(), ui->comboBoxFrame->currentIndex(), ui->comboBoxShading->currentIndex());
	}
}
