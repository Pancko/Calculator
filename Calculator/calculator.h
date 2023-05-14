
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpression>
#include <math.h>



QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow

{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;

private slots:
    void NumPressed();
    void MathButtonPressed();
    void EqualButton();
    void ChangeNumberSign();

    void on_m_pB_MC_clicked();
    void on_m_pB_MR_clicked();
    void on_m_pB_MS_clicked();
    void on_m_pB_MAdd_clicked();
    void on_m_pB_Point_clicked();
    void on_m_pB_Backspace_clicked();
    void on_m_pB_Clear_clicked();
    void on_m_pB_ClearAll_clicked();
};

#endif // MAINWINDOW_H
