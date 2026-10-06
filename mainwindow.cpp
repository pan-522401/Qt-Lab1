#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <math.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->display->setReadOnly(true);
    ui->display->installEventFilter(this);

    digitBTNs = {{Qt::Key_0, ui->btnNum0},
               {Qt::Key_1, ui->btnNum1},
               {Qt::Key_2, ui->btnNum2},
               {Qt::Key_3, ui->btnNum3},
               {Qt::Key_4, ui->btnNum4},
               {Qt::Key_5, ui->btnNum5},
               {Qt::Key_6, ui->btnNum6},
               {Qt::Key_7, ui->btnNum7},
               {Qt::Key_8, ui->btnNum8},
               {Qt::Key_9, ui->btnNum9},
              };

    foreach (auto btn,digitBTNs )
        connect(btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

//    connect(ui->btnNum0, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum1, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum2, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum3, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum4, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum5, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum6, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum7, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum8, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->btnNum9, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiple, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnPercentage, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));

    connect(ui->btnSign, SIGNAL(clicked()), this, SLOT(btnSignClicked()));

    operatorBTNs = {{Qt::Key_Plus, ui->btnPlus},
                   {Qt::Key_Minus, ui->btnMinus},
                   {Qt::Key_Asterisk, ui->btnMultiple},
                   {Qt::Key_Slash, ui->btnDivide},
                   {Qt::Key_Period, ui->btnPeriod},
                   {Qt::Key_Backspace, ui->btnDel},
                   {Qt::Key_Escape, ui->btnClear},
                   {Qt::Key_Delete, ui->btnClearAll},
                   {Qt::Key_Return, ui->btnEqual},
                   {Qt::Key_Enter, ui->btnEqual},
                   {Qt::Key_Equal, ui->btnEqual},
                  };

    unaryBTNs = {{Qt::Key_Percent, ui->btnPercentage},
                 {Qt::Key_F9, ui->btnSign},
                };

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result = 0;
    if(operands.size() == 2 && opcodes.size() > 0 ){
        //取操作数
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();

        //取操作符
        QString op = opcodes.front();
        opcodes.pop_front();

        if(op == "+"){
           result = operand1 + operand2;
        }else if(op == "-"){
           result = operand1 - operand2;
        }else if(op == "×"){
           result = operand1 * operand2;
        }else if(op == "÷"){
           if(operand2 == 0){//除数为0
               operands.clear();//清除残留状态，便于继续计算
               opcodes.clear();
               return QString("除数不能为零");
           }
           result = operand1 / operand2;
        }
        operands.push_back(QString::number(result));



        ui->statusbar->showMessage(QString("calculation is in progress:operands is %1, opcode is %2").arg(operands.size()).arg(
                                       opcodes.size()));
    }else
        ui->statusbar->showMessage(QString("operands is %1, opcode is %2").arg(operands.size()).arg(
                                       opcodes.size()));

    return QString::number(result);
}

void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton *>(sender())->text();

    if(digit == "0" && operand == "0")
       digit = "";

    if(operand == "0" && digit != "0")
       operand = "";

    operand += digit;

    ui->display->setText(operand);

}


void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains("."))
       operand += qobject_cast<QPushButton *>(sender())->text();
    ui->display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
    operand = operand.left(operand.length() - 1);
    ui->display->setText(operand);
}


void MainWindow::on_btnClear_clicked()
{
    operand.clear();
    operands.clear();
    opcodes.clear();
    ui->display->setText(operand);
}


void MainWindow::on_btnClearAll_clicked()
{
    operand.clear();
    operands.clear();
    opcodes.clear();
    ui->display->clear();
    ui->statusbar->showMessage(QString("operands is %1, opcode is %2").arg(operands.size()).arg(
                                   opcodes.size()));
}


void MainWindow::btnSignClicked()
{
    if(operand != ""){
       if(operand.startsWith("-"))
          operand.remove(0, 1);
       else
          operand = "-" + operand;

       ui->display->setText(operand);
    }

}


void MainWindow::btnBinaryOperatorClicked()
{
    ui->statusbar->showMessage("last operand " + operand);
    QString opcode = qobject_cast<QPushButton *>(sender())->text();
    qDebug()<<opcode;
    if(operand != ""){
       operands.push_back(operand);
       operand ="";

       opcodes.push_back(opcode);


       QString result = calculation();

       ui->display->setText(result);
    }

}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operand != ""){
       double result = operand.toDouble();
       operand ="";

       QString op = qobject_cast<QPushButton *>(sender())->text();

       if (op == "％")
           result /= 100.0;
       else if (op == "1/x")
       {
           if(result == 0){//对0取倒数无意义
               ui->display->setText("除数不能为零");
               return;
           }
           result = 1 / result;
       }
       else if (op == "x²")
           result *= result;
       else if (op == "√x")
       {
           if(result < 0){//负数不能开平方
               ui->display->setText("负数不能开方");
               return;
           }
           result = sqrt(result);
       }

       ui->display->setText(QString::number(result));

    }

}

void MainWindow::on_btnEqual_clicked()
{
    if(operand == ""){
        if(operands.size() > 0 && opcodes.size() > 0){//按了运算符但还没输入第二个操作数
            ui->display->setText("缺少第二操作数");
        }
        return;
    }

    if(operand != ""){
       operands.push_back(operand);
       operand ="";
    }

    if(operands.size() == 2 && opcodes.size() > 0){
        QString result = calculation();

        operands.clear();
        opcodes.clear();

        ui->display->setText(result);
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->display && event->type() == QEvent::KeyPress){
        if(!(static_cast<QKeyEvent *>(event)->modifiers() & Qt::ControlModifier)){
            keyPressEvent(static_cast<QKeyEvent *>(event));
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach (auto btnKey, digitBTNs.keys()){
        if (event->key() == btnKey)
            digitBTNs[btnKey]->animateClick(100);
    }

    foreach (auto btnKey, operatorBTNs.keys()){
        if (event->key() == btnKey)
            operatorBTNs[btnKey]->animateClick(100);
    }

    foreach (auto btnKey, unaryBTNs.keys()){
        if (event->key() == btnKey)
            unaryBTNs[btnKey]->animateClick(100);
    }

//    if(event->key() == Qt::Key_0)
//       ui->btnNum0->animateClick(100);
//    else if(event->key() == Qt::Key_1)
//       ui->btnNum1->animateClick(100);

    QMainWindow::keyPressEvent(event);
}

