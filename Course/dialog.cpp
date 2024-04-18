#include "dialog.h"

MyDialog::MyDialog( QWidget* parent ) : QDialog( parent )
{
	this->setWindowTitle("Додати ребро");
	this->resize(300,100);

	sbox->setValue(1);//параметри спінбокса
	sbox->setMaximum(999);
	sbox->setMinimum(-999);

	connect(sbox, &QSpinBox::textChanged,this, &MyDialog::spinBoxChanged );

	QString text = "Вага ребра";//текстове поле
	QLabel *label = new QLabel(text, this);

	QPushButton *isOriented = new QPushButton;//кнопки
	isOriented->setText("Орієнтоване");
	connect( isOriented, &QPushButton::clicked,this, &MyDialog::orientedBtn );
	QPushButton *isNotOriented = new QPushButton;
	isNotOriented->setText("Неорієнтоване");
	connect( isNotOriented, &QPushButton::clicked,this, &MyDialog::notOrientedBtn );
	QPushButton *cancel = new QPushButton;
	cancel->setText("Відміна");
	connect( cancel, &QPushButton::clicked,this, &MyDialog::cancelBtn );

	QBoxLayout* Hlayout1 = new QHBoxLayout;
	QBoxLayout* Hlayout2 = new QHBoxLayout;
	QBoxLayout* layout = new QVBoxLayout;

	Hlayout1->addWidget(label);
	Hlayout1->addWidget(sbox);
	Hlayout2->addWidget(isOriented);
	Hlayout2->addWidget(isNotOriented);
	Hlayout2->addWidget(cancel);
	layout->addLayout(Hlayout1);
	layout->addLayout(Hlayout2);
	setLayout( layout );//обєднуємо обєкти
}

int MyDialog::getInput() const //повернути значення яке ввів коритсувач
{
	return returnedValue;
}

int MyDialog::getOrientation() const //повернути режим орієнтації
{
	return this->isOriented;
}

void MyDialog::orientedBtn() //якщо натиснули "Орієнтоване"
{
	returnedValue = sbox->value();
	isOriented = 1;
	this->close();
}

void MyDialog::notOrientedBtn() //якщо натиснули "Неорієнтоване"
{
	returnedValue = sbox->value();
	isOriented = 2;
	this->close();
}

void MyDialog::cancelBtn() //якщо натиснули "Відміна"
{
	returnedValue = 0;
	isOriented = 3;
	this->close();
}

void MyDialog::spinBoxChanged()
{
	if(sbox->value()==0)
	{
		sbox->setStyleSheet("* { background-color: rgb(225,100,100); }");
	}
	else
	{
		sbox->setStyleSheet("* { background-color: rgb(255,255,255); }");
	}
}
