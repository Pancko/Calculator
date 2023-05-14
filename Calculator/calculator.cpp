
#include "calculator.h"
#include "./ui_calculator.h"
#include <QDebug>

double CalculationValue = 0.0;
double Memory = 0.0;

bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool sqrtTrigger = false;
bool squareTrigger = false;
bool fractureTrigger = false;

int point = 0;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->m_lcdNumber->display(CalculationValue);

    QPushButton *numberButtons[10];
    for (int i = 0; i < 10; ++i)
    {
        QString buttonName = "m_pB_" + QString::number(i);
        numberButtons[i] = Calculator::findChild<QPushButton *>(buttonName);
        connect(numberButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->m_pB_Add,       SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->m_pB_Sub,       SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->m_pB_Times,     SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->m_pB_Divide,    SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->m_pB_Sqrt,      SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->m_pB_Square,    SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->m_pB_Fracture,  SIGNAL(released()), this, SLOT(MathButtonPressed()));

    connect(ui->m_pB_Equals,    SIGNAL(released()), this, SLOT(EqualButton()));

    connect(ui->m_pB_PlusMinus, SIGNAL(released()), this, SLOT(ChangeNumberSign()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed()
{
    QPushButton *button = (QPushButton *)sender();
    QString buttonValue = button->text();
    QString displayValue = QString::number(ui->m_lcdNumber->value(), 'g', 15);
    double dblNewValue;
    if(displayValue.contains('-'))
    {
        if (point)
        {
            dblNewValue= dblNewValue = displayValue.toDouble() * (pow(10, !point )) - buttonValue.toDouble()/(pow(10, point - 1));
        }
        else
        {
            dblNewValue= dblNewValue = displayValue.toDouble() * (pow(10, !point)) - buttonValue.toDouble()/(pow(10, point));
        }
    }
    else
    {
        dblNewValue= dblNewValue = displayValue.toDouble() * (pow(10, !point)) + buttonValue.toDouble()/(pow(10, point));
    }


    if (point > 0) point++;
    ui->m_lcdNumber->display(QString::number(dblNewValue, 'g', 15));
}

void Calculator::MathButtonPressed()
{
    Calculator::EqualButton();
    point = 0;

    QPushButton *button = (QPushButton *)sender();
    QString buttonValue = button->text();

    if(QString::compare(buttonValue, "/", Qt::CaseInsensitive) == 0)
    {
        divTrigger = true;
        ui->m_lcdNumber->display("");
    }
    else if(QString::compare(buttonValue, "*", Qt::CaseInsensitive) == 0)
    {
        multTrigger = true;
        ui->m_lcdNumber->display("");
    }
    else if(QString::compare(buttonValue, "+", Qt::CaseInsensitive) == 0)
    {
        addTrigger = true;
        ui->m_lcdNumber->display("");
    }
    else if(QString::compare(buttonValue, "-", Qt::CaseInsensitive) == 0)
    {
        subTrigger = true;
        ui->m_lcdNumber->display("");
    }
    else if(QString::compare(buttonValue, "Sqrt", Qt::CaseInsensitive) == 0)
    {
        sqrtTrigger = true;
        Calculator::EqualButton();
    }
    else if(QString::compare(buttonValue, "x^2", Qt::CaseInsensitive) == 0)
    {
        squareTrigger = true;
        Calculator::EqualButton();
    }
    else
    {
        fractureTrigger = true;
        Calculator::EqualButton();
    }
}

void Calculator::EqualButton()
{
    QString displayValue = QString::number(ui->m_lcdNumber->value(), 'g', 15);
    double dblDisplayValue = displayValue.toDouble();

    if (addTrigger || subTrigger || multTrigger || divTrigger || sqrtTrigger || squareTrigger || fractureTrigger)
    {
        if(addTrigger)
        {
            CalculationValue = CalculationValue + dblDisplayValue;
            addTrigger = false;
        }
        else if(subTrigger)
        {
            CalculationValue = CalculationValue - dblDisplayValue;
            subTrigger = false;
        }
        else if(multTrigger)
        {
            CalculationValue = CalculationValue * dblDisplayValue;
            multTrigger = false;
        }
        else if(divTrigger)
        {
            if (dblDisplayValue == 0)
            {
                CalculationValue = 0;
            }
            else
            {
                CalculationValue = CalculationValue / dblDisplayValue;
            }
            divTrigger = false;
        }
        else if(sqrtTrigger)
        {
            if(dblDisplayValue < 0)
            {
                CalculationValue = 0;
            }
            else
            {
                CalculationValue = sqrt(dblDisplayValue);
            }
            sqrtTrigger = false;
        }
        else if(squareTrigger)
        {
            CalculationValue = dblDisplayValue * dblDisplayValue;
            squareTrigger = false;
        }
        else
        {
            CalculationValue = 1 / dblDisplayValue;
            fractureTrigger = false;
        }
    }
    else CalculationValue = dblDisplayValue;
    displayValue = QString::number(CalculationValue, 'g', 15);
    while (displayValue.contains(".") && (displayValue.size() - displayValue.indexOf(".") > 4))
    {
        displayValue.chop(displayValue.size() - (displayValue.size() - displayValue.indexOf(".")));
    }

    ui->m_lcdNumber->display(displayValue);

    if (displayValue.contains("."))
    {
        point = displayValue.size() - displayValue.indexOf(".");
    }
    else
    {
        point = 0;
    }

}

void Calculator::ChangeNumberSign()
{
    QString displayValue = QString::number(ui->m_lcdNumber->value(), 'g', 15);
    QRegularExpression reg("[-]?[0-9.]*");
    QRegularExpressionMatch match = reg.match(displayValue);
    if(match.hasMatch())
    {
        double dblDisplayVal = displayValue.toDouble();
        CalculationValue = -1 * dblDisplayVal;
        if ((displayValue.contains('.')) && (displayValue.contains('-')))
        {
            point--;
        }
        if ((displayValue.contains('.')) && !(displayValue.contains('-')))
        {
            point++;
        }
        ui->m_lcdNumber->display(QString::number(CalculationValue));
    }
}


void Calculator::on_m_pB_MC_clicked()
{
    Memory = 0.0;
}


void Calculator::on_m_pB_MR_clicked()
{
    ui->m_lcdNumber->display(QString::number(Memory, 'g', 15));
}


void Calculator::on_m_pB_MS_clicked()
{
    QString displayValue = QString::number(ui->m_lcdNumber->value(), 'g', 15);
    double dblDisplayValue = displayValue.toDouble();

    Memory = dblDisplayValue;
}


void Calculator::on_m_pB_MAdd_clicked()
{
    QString displayValue = QString::number(ui->m_lcdNumber->value(), 'g', 15);
    double dblDisplayValue = displayValue.toDouble();
    Memory = Memory + dblDisplayValue;
}


void Calculator::on_m_pB_Point_clicked()
{
    QString displayValue = QString::number(ui->m_lcdNumber->value(), 'g', 15);
    if (!displayValue.contains("."))
    {
        ui->m_lcdNumber->display(displayValue + ".");
        if (displayValue.contains('-'))
        {
            point = 2;
        }
        else
        {
            point = 1;
        }

    }
}


void Calculator::on_m_pB_Backspace_clicked()
{
    QString displayValue = QString::number(ui->m_lcdNumber->value(), 'g', 15);
    if (displayValue[displayValue.size() - 1] == '.')
    {
        point = 0;
    }
    displayValue.chop(1);
    ui->m_lcdNumber->display(displayValue);
    if ((ui->m_lcdNumber->value() == 0) || (ui->m_lcdNumber->value() == 0.0)) ui->m_lcdNumber->display("0");
}


void Calculator::on_m_pB_Clear_clicked()
{
    ui->m_lcdNumber->display("0");
    point = 0;
}


void Calculator::on_m_pB_ClearAll_clicked()
{
    Memory = 0;
    ui->m_lcdNumber->display("0");
    point = 0;
}
