#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QList<QPushButton*> digitButtons;
    digitButtons << ui->btn0 << ui->btn1 << ui->btn2
                 << ui->btn3 << ui->btn4 << ui->btn5
                 << ui->btn6 << ui->btn7 << ui->btn8 << ui->btn9;


    for (auto btn : digitButtons) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    }


    connect(ui->btnPlus, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    connect(ui->btnMinus, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    connect(ui->btnMultiply, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    connect(ui->btnDivide, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);


    connect(ui->btnEqual, &QPushButton::clicked, this, &MainWindow::onEqualClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDigitClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    if (waitingForNextInput) {
        ui->display->clear();
        waitingForNextInput = false;
    }

    ui->display->setText(ui->display->text() + button->text());
}


void MainWindow::onOperatorClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn)
        ui->display->setText(ui->display->text() + " " + btn->text() + " ");
}

void MainWindow::onEqualClicked()
{
    QString expr = ui->display->text();
    QStringList tokens = expr.split(' ', Qt::SkipEmptyParts);

    if (tokens.size() != 3) {
        ui->display->setText("Error");
        return;
    }

    double a = tokens[0].toDouble();
    QString op = tokens[1];
    double b = tokens[2].toDouble();
    double result = 0;

    if (op == "+") result = a + b;
    else if (op == "-") result = a - b;
    else if (op == "*") result = a * b;
    else if (op == "/") {
        if (b == 0) {
            ui->display->setText("Div by 0");
            return;
        }
        result = a / b;
    } else {
        ui->display->setText("Error");
        return;
    }

    ui->display->setText(QString::number(result));

    waitingForNextInput = true;
}


void MainWindow::onClearClicked()
{
    ui->display->clear();
    waitingForNextInput = false;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QString key = event->text();

    if (key >= "0" && key <= "9") {
        ui->display->setText(ui->display->text() + key);
    } else if (key == "+" || key == "-" || key == "*" || key == "/") {
        ui->display->setText(ui->display->text() + " " + key + " ");
    } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        onEqualClicked();
    } else if (event->key() == Qt::Key_Backspace) {
        QString text = ui->display->text();
        if (!text.isEmpty()) {
            text.chop(1);
            ui->display->setText(text);
        }
    } else if (event->key() == Qt::Key_Escape) {
        onClearClicked();
    }

    QMainWindow::keyPressEvent(event);
}
