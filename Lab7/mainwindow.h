#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double alfa, beta;
    double a_Blue, a_Green, a_Red;
    double b_Blue, b_Green, b_Red;

private slots:
    void on_actionOtworz_triggered();

    void on_actionZapisz_triggered();

    void on_actionZakoncz_triggered();

    void on_actionPokaz_triggered();

    void on_actionPodzial_na_RGB_triggered();

    void on_actionPodzial_na_YCrCb_triggered();

    void on_actionPodzial_na_HSV_triggered();

    void on_actionKontrast_i_Jasnosc_obrazu_RGB_triggered();

    void on_Jasnosc_valueChanged(int value);

    void on_Kontrast_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QImage *image;
    QPixmap obrazek;
    QGraphicsScene *scena;
    QGraphicsPixmapItem *obrazek_sceny;
    QString Wybrany_plik;
};

#endif // MAINWINDOW_H
