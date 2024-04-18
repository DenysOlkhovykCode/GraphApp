#ifndef DIALOG_H
#define DIALOG_H

#include <QString>
#include <QDialog>
#include <QSpinBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>

class MyDialog : public  QDialog
{
public:
	MyDialog( QWidget* parent = 0 );//констуктор діалогового вікна
	int getInput() const;//повернути значення яке ввів коритсувач
	int getOrientation() const;//повернути режим орієнтації
public slots:
	void orientedBtn();//якщо натиснули "Орієнтоване"
	void notOrientedBtn();//якщо натиснули "Неорієнтоване"
	void cancelBtn();//якщо натиснули "Відміна"
	void spinBoxChanged();//якщо натиснули "Відміна"
private:
	int returnedValue = 0;//значення яке ввів користувач
	int isOriented = false;//режим орієнтації
	QSpinBox *sbox = new QSpinBox;
};

#endif // DIALOG_H
