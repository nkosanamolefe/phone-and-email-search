#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtWidgets>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTextCursor>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create widgets
    displayText = new QTextEdit;
    loadButton = new QPushButton("Load");
    processButton = new QPushButton("Process");
    processButton->setEnabled(false);

    // Create layout
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(processButton);

    // Combine widgets and layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(displayText);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Set window properties
    setWindowTitle("Search");

    // Connect buttons to slot
    connect(loadButton, SIGNAL(clicked()), this, SLOT(on_loadButtonClicked()));
    connect(processButton, SIGNAL(clicked()), this, SLOT(processText()));
}

void MainWindow::on_loadButtonClicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, "Open file", ".", "Text files (*.txt)");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "File error", "Could not open file " + fileName, QMessageBox::Ok);
        return;
    }
    QTextStream in(&file);
    displayText->setPlainText(in.readAll());
    processButton->setEnabled(true);
    file.close();
}

void MainWindow::processText(){
    QTextDocument* document = displayText->document();
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::yellow);

    QRegularExpression emailRegex("[\\w\\.\\n]+@[\\w\\.]+");
    QRegularExpression phoneRegex(
                "(\\(\\d{3}\\)\\d{3}-\\d{4}|\\(\\d{3}\\)\\d{7}|\\d{3}-\\d{3}-\\d{4}|\\d{3}-\\d{7}|\\d{10})"
                );
    QRegularExpression combinedRegex(emailRegex.pattern() + "|" + phoneRegex.pattern());

    QTextCursor cursor(document);
    cursor.setPosition(0);

    QString plainText = document->toPlainText(); // Get the text once

    while (!cursor.isNull() && !cursor.atEnd()) {
        QRegularExpressionMatch match = combinedRegex.match(plainText, cursor.position());

        if (match.hasMatch()) {
            int start = match.capturedStart();
            int length = match.capturedLength();
            cursor.setPosition(start);
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, length);
            cursor.mergeCharFormat(highlightFormat);

            cursor.setPosition(start + length); // Move cursor past the matched part
        } else {
            cursor.movePosition(QTextCursor::NextCharacter);
        }
    }

    QMessageBox::information(this, "Done", "Checking completed");
}

MainWindow::~MainWindow()
{
}

