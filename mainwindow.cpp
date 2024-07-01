#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->statusbar->showMessage("Выбрано: Ничего");



    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 800, 600);

    ui->graphicsView->setScene(scene);

    ui->centralwidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    //this->setMouseTracking(true);

    contextMenu = new QMenu(this);
    //contextMenu->addAction("Запустить анимацию", this, &MainWindow::animateSelectedShape);
    contextMenu->addAction("Переместить", this, &MainWindow::moveSelectedShape);
    contextMenu->addAction("Повернуть", this, &MainWindow::rotateSelectedShape);
    contextMenu->addAction("Изменить размер", this, &MainWindow::resizeSelectedShape);
    contextMenu->addAction("Изменить цвет пера", this, &MainWindow::changeSelectedShapePenColor);
    contextMenu->addAction("Изменить цвет кисти", this, &MainWindow::changeSelectedShapeBrushColor);
    contextMenu->addAction("Удалить", this, &MainWindow::deleteSelectedShape);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::showContextMenu(const QPoint& pos) {
    selectedShape = nullptr;
    QGraphicsItem* item = scene->itemAt(windowCoordinatesToSceneCoordinates(pos), QTransform());
    if (item && qgraphicsitem_cast<Shape*>(item)) {
        selectedShape = qgraphicsitem_cast<Shape*>(item);
    }
    if (selectedShape) {
        contextMenu->exec(mapToGlobal(pos));
    }
}

void MainWindow::on_actionNewFile_triggered(){
    scene->clear();
    scene->update();
}


void MainWindow::on_actionOpenFile_triggered(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение"), "", tr("Изображения (*.png *.jpg *.jpeg)"));
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        if (pixmap.isNull()) {
            qDebug() << "Не удалось загрузить изображение.";
            return;
        }

        QGraphicsPixmapItem *pixmapItem = scene->addPixmap(pixmap);

        QPointF sceneCenter = scene->sceneRect().center();
        QPointF pixmapCenter = pixmapItem->boundingRect().center();
        pixmapItem->setPos(sceneCenter - pixmapCenter);
    }
}


void MainWindow::on_actionSaveFile_triggered(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить как изображение"), "", tr("PNG (*.png);;JPEG (*.jpg *.jpeg)"));
    if (!fileName.isEmpty()) {
        if (fileName.endsWith(".png", Qt::CaseInsensitive)) {
            saveAsImage(fileName, "PNG");
        } else if (fileName.endsWith(".jpg", Qt::CaseInsensitive) || fileName.endsWith(".jpeg", Qt::CaseInsensitive)) {
            saveAsImage(fileName, "JPEG");
        } else {
            qDebug() << "Неподдерживаемый формат файла.";
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    adjustGraphicsViewSize();
}


void MainWindow::on_actionExit_triggered(){
    QApplication::exit();
}


void MainWindow::on_actionSelectLine_triggered() {
    currentShapeType = ShapeType::Line;
    ui->statusbar->showMessage("Выбрано: Линия");
}

void MainWindow::on_actionSelectCurvedLine_triggered() {
    currentShapeType = ShapeType::CurvedLine;
    ui->statusbar->showMessage("Выбрано: Кривая");
}

void MainWindow::on_actionSelectRectangle_triggered(){
    currentShapeType = ShapeType::Rectangle;
    ui->statusbar->showMessage("Выбрано: Прямоугольник");
}

void MainWindow::on_actionSelectCircle_triggered() {
    currentShapeType = ShapeType::Circle;
    ui->statusbar->showMessage("Выбрано: Круг");
}


void MainWindow::on_actionSelectPenColor_triggered(){
    penColor = QColorDialog::getColor(penColor, nullptr, "Меню выбора цвета пера");
}

void MainWindow::on_actionPenSettings_triggered(){
    bool ok;
    penSize = QInputDialog::getInt(this, "Настройки пера", "Выберите размер пера:", penSize, 1, 50, 1, &ok);
}


void MainWindow::on_actionSelectBrushColor_triggered(){
    brushColor = QColorDialog::getColor(brushColor, nullptr, "Меню выбора цвета кисти");
}

void MainWindow::on_actionBrushSettings_triggered(){
    QStringList brushTypes;
    brushTypes << "SolidPattern" << "Dense1Pattern" << "Dense2Pattern" << "Dense3Pattern" << "Dense4Pattern" << "Dense5Pattern" << "Dense6Pattern" << "Dense7Pattern" << "HorPattern" << "VerPattern" << "CrossPattern" << "BDiagPattern" << "FDiagPattern" << "DiagCrossPattern";
    bool ok;
    QString brushType = QInputDialog::getItem(this, tr("Настройки кисти"), tr("Выберите тип кисти:"), brushTypes, 0, false, &ok);
    if (ok) {
        brushStyle = Qt::BrushStyle(brushTypes.indexOf(brushType));
    }
}

void MainWindow::adjustGraphicsViewSize(){
    ///
    /// Вообще не понимаю как работает
    /// но рад, что хотя бы работает
    ///
    int windowWidth = width();
    int windowHeight = height();

    int menuBarHeight = ui->menubar->height();
    int toolBarHeight = ui->toolBar->height();
    int statusBarHeight = ui->statusbar->height();

    int availableHeight = windowHeight - menuBarHeight - toolBarHeight - statusBarHeight;

    double aspectRatio = 4.0 / 3.0;

    int newWidth = windowWidth;
    int newHeight = static_cast<int>(windowWidth / aspectRatio);

    if (newHeight > availableHeight) {
        newHeight = availableHeight;
        newWidth = static_cast<int>(availableHeight * aspectRatio);
    }

    int x = (windowWidth - newWidth) / 2;
    int y = 0;

    ui->graphicsView->setGeometry(x, y, newWidth, newHeight);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

QPointF MainWindow::windowCoordinatesToSceneCoordinates(const QPoint &windowPos) const{
    QPoint viewPos = ui->graphicsView->mapFrom(this, windowPos);
    QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

    return scenePos;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;

        QPointF scenePos = this->windowCoordinatesToSceneCoordinates(event->pos());

        switch(currentShapeType){
            case ShapeType::None:
                return;
            case ShapeType::Line:
                currentShape = new Line(scenePos, scenePos, QPen(penColor, penSize), QBrush(Qt::transparent));
                break;
            case ShapeType::CurvedLine:
                currentShape = new CurvedLine(scenePos, scenePos, QPen(penColor, penSize), QBrush(Qt::transparent));
                break;
            case ShapeType::Rectangle:
                currentShape = new Rectangle(scenePos, scenePos, QPen(penColor, penSize), QBrush(brushColor, brushStyle));
                break;
            case ShapeType::Circle:
                currentShape = new Circle(scenePos, scenePos, QPen(penColor, penSize), QBrush(brushColor, brushStyle));
                break;
            default:
                return;
        }

        scene->addItem(currentShape);
    }
    if (event->button() == Qt::RightButton) {
        showContextMenu(event->pos());
        return;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {

    if (isDrawing && currentShapeType != ShapeType::None) {
        QPointF scenePos = this->windowCoordinatesToSceneCoordinates(event->pos());

        currentShape->setEndPoint(scenePos);
        scene->update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
        shapes.append(currentShape);
        currentShape = nullptr;
    }
}

void MainWindow::saveAsImage(const QString &fileName, const char* format) {
    QPixmap pixmap(ui->graphicsView->viewport()->size());
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    ui->graphicsView->render(&painter);

    if (!pixmap.save(fileName, format)) {
        qDebug() << "Не удалось сохранить изображение.";
    }
}

void MainWindow::moveSelectedShape() {
    if (selectedShape) {
        QPointF currentPosition = selectedShape->getEndPoint();
        bool okX, okY;
        double newX = QInputDialog::getDouble(this, "Переместить фигуру (X)", "Введите новую координату X:", currentPosition.x(), -2147483647, 2147483647, 2, &okX);
        double newY = QInputDialog::getDouble(this, "Переместить фигуру (Y)", "Введите новую координату Y:", currentPosition.y(), -2147483647, 2147483647, 2, &okY);
        if (okX && okY) {
            QPointF newPosition(newX, newY);
            qreal dx = newPosition.x() - selectedShape->getEndPoint().x();
            qreal dy = newPosition.y() - selectedShape->getEndPoint().y();
            selectedShape->setStartPoint(selectedShape->getStartPoint() + QPointF(dx, dy));
            selectedShape->setEndPoint(newPosition);
        }
    }
}

void MainWindow::rotateSelectedShape() {
    // Реализация поворота фигуры
    if (selectedShape) {
        bool ok;
        qreal angle = QInputDialog::getDouble(this, "Повернуть фигуру", "Введите угол поворота (в градусах):", 0, -360, 360, 1, &ok);
        if (ok) {
            QTransform transform;
            transform.rotate(angle);
            selectedShape->setTransform(transform, true);
        }
    }
}

void MainWindow::resizeSelectedShape() {
    if (selectedShape) {
        QPointF currentEndPoint = selectedShape->getEndPoint();
        bool okX, okY;
        double newX = QInputDialog::getDouble(this, "Изменить размер фигуры (X)", "Введите новую координату X конца:", currentEndPoint.x(), -2147483647, 2147483647, 2, &okX);
        double newY = QInputDialog::getDouble(this, "Изменить размер фигуры (Y)", "Введите новую координату Y конца:", currentEndPoint.y(), -2147483647, 2147483647, 2, &okY);
        if (okX && okY) {
            QPointF newEndPoint(newX, newY);
            selectedShape->setEndPoint(newEndPoint);
        }
    }
}

void MainWindow::changeSelectedShapePenColor() {
    if (selectedShape) {
        QColor newColor = QColorDialog::getColor(selectedShape->getPenColor(), this, "Выберите цвет обводки");
        if (newColor.isValid()) {
            selectedShape->setPenColor(newColor);
        }
    }
}

void MainWindow::changeSelectedShapeBrushColor() {
    if (selectedShape) {
        QColor newColor = QColorDialog::getColor(selectedShape->getBrushColor(), this, "Выберите цвет заливки");
        if (newColor.isValid()) {
            selectedShape->setBrushColor(newColor);
        }
    }
}

void MainWindow::deleteSelectedShape() {
    if (selectedShape) {
        scene->removeItem(selectedShape);
        shapes.removeOne(selectedShape);
        delete selectedShape;
        selectedShape = nullptr;
    }
}

