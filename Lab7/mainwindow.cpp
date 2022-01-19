#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    obrazek_sceny=NULL;
    Wybrany_plik.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOtworz_triggered()
{
    QString sciezka_pliku = QFileDialog::getOpenFileName(this,tr("Otworz plik"),
                                                         "",tr("JPEG (*.jpg *.jpeg);;PNG (*png)"));
    if(!sciezka_pliku.isEmpty())
    {
            Wybrany_plik=sciezka_pliku;
            image = new QImage();
            image->load(sciezka_pliku);

            obrazek = QPixmap::fromImage(*image);
            scena = new QGraphicsScene(this);
            obrazek_sceny = scena->addPixmap(obrazek);
            scena->setSceneRect(obrazek.rect());
            ui->graphicsView->setScene(scena);
            delete image;

    }
}

void MainWindow::on_actionZapisz_triggered()
{
    if(obrazek_sceny!=NULL)
    {
        QString sciezka_pliku = QFileDialog::getSaveFileName(this,tr("Zapisz plik"),
                                                       "",tr("JPEG (*.jpg *.jpeg);;PNG (*png)"));
        if(!sciezka_pliku.isEmpty())
        {
            QImage obrazek_do_zapisu = obrazek_sceny->pixmap().toImage();
            if(obrazek_do_zapisu.size().isValid()==true) obrazek_do_zapisu.save(sciezka_pliku);
        }
    }
    else
    {
        QMessageBox(QMessageBox::Information,"Blad",
                    "Brak pliku do zapisania", QMessageBox::Ok).exec();
    }
}

void MainWindow::on_actionZakoncz_triggered()
{
    QMessageBox okienko_wyjscia(QMessageBox::Warning,"Koniec Programu",
                                "Czy chcesz wyjsc z programu?", QMessageBox::Yes|QMessageBox::No);
    okienko_wyjscia.setButtonText(QMessageBox::Yes, "Tak");
    okienko_wyjscia.setButtonText(QMessageBox::No, "Nie");
    if(okienko_wyjscia.exec()==QMessageBox::Yes){
        QApplication::quit();
    }
}

void MainWindow::on_actionPokaz_triggered()
{
    if(!Wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(Wybrany_plik.toStdString().c_str());
        cv::imshow("Obrazek bazowy",obrazek_bazowy);
    }
}

void MainWindow::on_actionPodzial_na_RGB_triggered()
{
    if(!Wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(Wybrany_plik.toStdString().c_str());
        cv::Mat pokaz_blue = cv::imread(Wybrany_plik.toStdString().c_str());
        cv::Mat pokaz_red = cv::imread(Wybrany_plik.toStdString().c_str());
        cv::Mat pokaz_green = cv::imread(Wybrany_plik.toStdString().c_str());
        cv::Mat skladowa_blue[3];
        cv::Mat skladowa_red[3];
        cv::Mat skladowa_green[3];
        cv::split (obrazek_bazowy, skladowa_blue);
        cv::split (obrazek_bazowy, skladowa_red);
        cv::split (obrazek_bazowy, skladowa_green);
        skladowa_blue[1] = cv::Mat::zeros(skladowa_blue[1].size(), CV_8UC1);
        skladowa_blue[2] = cv::Mat::zeros(skladowa_blue[2].size(), CV_8UC1);
        skladowa_red[0] = cv::Mat::zeros(skladowa_red[0].size(), CV_8UC1);
        skladowa_red[1] = cv::Mat::zeros(skladowa_red[1].size(), CV_8UC1);
        skladowa_green[0] = cv::Mat::zeros(skladowa_blue[0].size(), CV_8UC1);
        skladowa_green[2] = cv::Mat::zeros(skladowa_blue[2].size(), CV_8UC1);
        cv::merge(skladowa_blue,3,pokaz_blue);
        cv::merge(skladowa_red,3,pokaz_red);
        cv::merge(skladowa_green,3,pokaz_green);
        cv::imshow("skladowa B", pokaz_blue);
        cv::imshow("skladowa R", pokaz_red);
        cv::imshow("skladowa G", pokaz_green);
    }

}

void MainWindow::on_actionPodzial_na_YCrCb_triggered()
{
    if(!Wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(Wybrany_plik.toStdString().c_str());
        cv::Mat konwersja_YCrCb;
        cv::Mat skladowe_ycrcb[3];
        cv::cvtColor(obrazek_bazowy, konwersja_YCrCb, cv::COLOR_BGR2YCrCb);
        cv::split(konwersja_YCrCb,skladowe_ycrcb);
        cv::imshow("skladowa Y", skladowe_ycrcb[0]);
        cv::imshow("skladowa Cr", skladowe_ycrcb[1]);
        cv::imshow("skladowa Cb", skladowe_ycrcb[2]);
    }
}

void MainWindow::on_actionPodzial_na_HSV_triggered()
{
    if(!Wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(Wybrany_plik.toStdString().c_str());
        cv::Mat konwersja_HSV;
        cv::Mat skladowe_hsv[3];
        cv::cvtColor(obrazek_bazowy,konwersja_HSV,cv::COLOR_BGR2HSV);
        cv::split(konwersja_HSV,skladowe_hsv);
        cv::imshow("skladowa H",skladowe_hsv[0]);
        cv::imshow("skladowa S",skladowe_hsv[1]);
        cv::imshow("skladowa V",skladowe_hsv[2]);
    }
}




void MainWindow::on_actionKontrast_i_Jasnosc_obrazu_RGB_triggered()
{
    if(!Wybrany_plik.isEmpty())
        {
            cv::Mat obrazek_bazowy = cv::imread(Wybrany_plik.toStdString().c_str());
            cv::Mat obraz_out=cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());

            double beta = ui->Jasnosc->value();
            double alfa = ui->Kontrast->value(); alfa/=100;

            a_Blue = a_Green = a_Red = alfa;
            b_Blue = b_Green = b_Red = beta;

            for(int y=0; y<obrazek_bazowy.cols; y++)
            for(int x=0; x<obrazek_bazowy.rows; x++)
            {
                obraz_out.at<cv::Vec3b>(x,y)[0] = cv::saturate_cast<uchar>(a_Blue*(obrazek_bazowy.at<cv::Vec3b>(x,y)[0])+b_Blue);
                obraz_out.at<cv::Vec3b>(x,y)[1] = cv::saturate_cast<uchar>(a_Green*(obrazek_bazowy.at<cv::Vec3b>(x,y)[1])+b_Green);
                obraz_out.at<cv::Vec3b>(x,y)[2] = cv::saturate_cast<uchar>(a_Red*(obrazek_bazowy.at<cv::Vec3b>(x,y)[2])+b_Red);
            }
            obrazek_bazowy.convertTo(obraz_out, -1, alfa, beta);
            cv::imshow("Jasność i kontrast RGB", obraz_out);
        }
}


void MainWindow::on_Jasnosc_valueChanged(int value)
{
    alfa = float(value);
    on_actionKontrast_i_Jasnosc_obrazu_RGB_triggered();
}


void MainWindow::on_Kontrast_valueChanged(int value)
{
    beta = float(value);
    on_actionKontrast_i_Jasnosc_obrazu_RGB_triggered();
}

