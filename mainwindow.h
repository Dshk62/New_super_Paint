#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QVector>
#include <QDebug>
#include <QBrush>
#include <QFileDialog>
#include <QInputDialog>
#include "line.h"
#include "curvedline.h"
#include "rectangle.h"
#include "circle.h"


QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void adjustGraphicsViewSize();

    QPointF windowCoordinatesToSceneCoordinates(const QPoint &windowPos) const;

    void saveAsImage(const QString &fileName, const char* format);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;


private slots:
    void on_actionNewFile_triggered();
    void on_actionOpenFile_triggered();
    void on_actionSaveFile_triggered();
    void on_actionExit_triggered();


    void on_actionSelectLine_triggered();
    void on_actionSelectCurvedLine_triggered();
    void on_actionSelectRectangle_triggered();
    void on_actionSelectCircle_triggered();


    void on_actionSelectPenColor_triggered();
    void on_actionPenSettings_triggered();

    void on_actionSelectBrushColor_triggered();
    void on_actionBrushSettings_triggered();

    void showContextMenu(const QPoint& pos);
    void moveSelectedShape();
    void rotateSelectedShape();
    void resizeSelectedShape();
    void changeSelectedShapePenColor();
    void changeSelectedShapeBrushColor();
    void deleteSelectedShape();

private:
    Ui::MainWindow* ui;

    QGraphicsScene* scene;

    QColor penColor = Qt::black;
    int penSize = 3;

    QColor brushColor = Qt::red;
    Qt::BrushStyle brushStyle = Qt::SolidPattern;

    QVector<Shape*> shapes;

    bool isDrawing = false;
    Shape* currentShape = nullptr;

    ShapeType currentShapeType = ShapeType::None;

    Shape* selectedShape = nullptr;  // Выбранная фигура
    QMenu* contextMenu;  // Контекстное меню

};
#endif // MAINWINDOW_H
