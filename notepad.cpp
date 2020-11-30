#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif // QT_CONFIG(printdialog)
#include <QPrinter>
#endif // QT_CONFIG(printer)
#endif // QT_PRINTSUPPORT_LIB
#include <QFont>
#include <QFontDialog>

#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    connect(ui->actionNew, &QAction::triggered, this, &Notepad::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionSave, &QAction::triggered, this, &Notepad::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &Notepad::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &Notepad::print);
    connect(ui->actionExit, &QAction::triggered, this, &Notepad::exit);
    connect(ui->actionCopy, &QAction::triggered, this, &Notepad::copy);
    connect(ui->actionCopy, &QAction::triggered, this, &Notepad::copy);
    connect(ui->actionCut, &QAction::triggered, this, &Notepad::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &Notepad::paste);
    connect(ui->actionUndo, &QAction::triggered, this, &Notepad::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &Notepad::redo);
    connect(ui->actionFont, &QAction::triggered, this, &Notepad::selectFont);
    connect(ui->actionBold, &QAction::triggered, this, &Notepad::setFontBold);
    connect(ui->actionUnderline, &QAction::triggered, this, &Notepad::setFontUnderline);
    connect(ui->actionItalic, &QAction::triggered, this, &Notepad::setFontItalic);
    connect(ui->actionAbout, &QAction::triggered, this, &Notepad::about);

// Disable menu actions for unavailable features
#if !defined (QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
    ui->actionPrint->setEnabled(false);
#endif

#if !QT_CONFIG(clipboard)
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif
}

Notepad::~Notepad()
{
    delete ui;
}


void Notepad::newDocument()
{
    currentFile.clear();    // Clear the text buffer
    setWindowTitle(QString("New File"));
    ui->textEdit->setText(QString());
}

void Notepad::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file"); // Open dialog to select a file
    QFile file(fileName);   // Create a QFile object with the selected file name
    currentFile = fileName; // Store selected file globally

    // Attempt to open selected file as a readonly text file. If file cannot be opened, issue a warning and stop the program.
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    setWindowTitle(fileName);   // Set window title to selected file's name
    QTextStream in(&file);  // Define file textstream as instream
    QString text = in.readAll();    // File contents are copied to QString text variable
    ui->textEdit->setText(text);    // Fills the buffer of the editor with the selected file's text
    file.close();   // Close the selected file
}

void Notepad::save()
{
    QString fileName;

    // If no fileName exists from before, get one. Else, use the current fileName.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");  // Get filename
        currentFile = fileName; // Store file globally
    } else {
        fileName = currentFile; // Store file globally
    }

    QFile file(fileName);   // Create a QFile object with the file's fileName.

    // Attempt to open the current file. If file cannot be opened, issue an error message and stop the program.
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    setWindowTitle(fileName);   // Set window title to current file's name
    QTextStream out(&file); // Define file textstream as outstream
    QString text = ui->textEdit->toPlainText(); // Convert the contents of the editor buffer into plain text and save as a QString
    out << text;    // Write QString text to outstream
    file.close();   // Close the current file
}

void Notepad::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save As");   // Get fileName to save as
    QFile file(fileName);   // Create a QFile object with the file's fileName

    // Attempt to open the current file. If file cannot be opened, issue an error message and stop the program.
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    currentFile = fileName; // Store file globally
    setWindowTitle(fileName);   // Set window title to current file's name
    QTextStream out(&file); // Define file textstream as outstream
    QString text = ui->textEdit->toPlainText(); // Convert the contents of the editor buffer into plain text and save as QString
    out << text;    // Write QString text to outstream
    file.close();   // Close the current file
}

void Notepad::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printdev;  // Define QPrinter object called printdev
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printdev, this);   // Launch printer dialog box and store the selected printer in printdev
    // If user clicks on 'Cancel' and didn't select a printer, method returns
    if (dialog.exec() == QDialog::Rejected)
        return;
#endif // QT_CONFIG(printdialog)
    ui->textEdit->print(&printdev); // Actual printer command, QPrinter object as parameter
#endif // QT_CONFIG(printer)
}

void Notepad::exit()
{
    QCoreApplication::quit();
}

void Notepad::copy()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->copy();
#endif
}

void Notepad::cut()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->cut();
#endif
}

void Notepad::paste()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->paste();
#endif
}

void Notepad::undo()
{
    ui->textEdit->undo();
}

void Notepad::redo()
{
    ui->textEdit->redo();
}

void Notepad::selectFont()
{
    bool fontSelected;  // Boolean to know if a font has been selected
    QFont font = QFontDialog::getFont(&fontSelected, this);
    // If font was selected, set the font in the notepad
    if (fontSelected)
        ui->textEdit->setFont(font);
}

void Notepad::setFontBold(bool bold)
{
    bold
        ? ui->textEdit->setFontWeight(QFont::Bold)
        : ui->textEdit->setFontWeight(QFont::Normal);
}

void Notepad::setFontUnderline(bool underline)
{
    ui->textEdit->setFontUnderline(underline);
}

void Notepad::setFontItalic(bool italic)
{
    ui->textEdit->setFontItalic(italic);
}

void Notepad::about()
{
    QMessageBox::about(this, tr("About MDI"),
            tr("The <b>Notepad</b> example demonstrates how to code a basic "
               "text editor using QtWidgets."));
}
