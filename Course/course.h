#ifndef KOURSE_H
#define KOURSE_H

#include <QMainWindow>
#include <QApplication>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QKeySequence>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "graph.h"

class Course : public QMainWindow
{
public:
	enum modeSwitch
	{
		moveVertex, //режим "Переміщення вершини"
		addVertex, //режим "Додати вершину"
		addEdge, //режим "Додати шлях"
		deleteObject, //режим "Видалити об'єкт"
		findPath //режим "Знайти найкоротший шлях"
	};
	Course(QWidget *parent = 0); //конструктор вікна
	~Course(); //деструктор вікна
protected:
	void paintEvent(QPaintEvent *event); //малювання вікна
	void mouseMoveEvent (QMouseEvent *event); //обробка руху миші
	void mousePressEvent(QMouseEvent *event); //обробка натиснення клавіш
	void mouseReleaseEvent(QMouseEvent *event); //відпускання кнопок миші
	void keyPressEvent(QKeyEvent * event); //обробка натичкання кнопок
public slots:
	void moveVertexMode(); //якщо натиснули на кнопку режиму "Переміщення вершини"
	void addVertexMode(); //якщо натиснули на кнопку режиму "Додати вершину"
	void addEdgeMode(); //якщо натиснули на кнопку режиму "Додати шлях"
	void deleteMode(); //якщо натиснули на кнопку режиму "Видалити об'єкт"
	void findPathMode(); //якщо натиснули на кнопку режиму "Знайти найкоротший шлях"
private:
	void Icons(); //зміна іконок на кнопках
	// -1 бо таке значення не можна отримати під час роботи програми
	int draggedVertex = -1; //номер вершини яку перемещують в режимі "Переміщення вершини"
	int startVertex = -1; //номер вершини з якої малюватимемо шлях в режимі "Додати шлях" та номер вершини для пошуку найкоротшого шляху в режимі "Знайти найкоротший шлях"
	int finishVertex = -1; //номер вершини до якої шукатимемо найкоротший шлях в режимі "Знайти найкоротший шлях"
	int vertexID = 0; //змінна для створення нових точок(якщо додаавати нові вершини то їх номери будуть лише збільшуватися)
	QPoint startGrab; //вершина з якої почалося переміщення вершини
	graphStructure graph;//всі параметри графу
	undoGraphState undoAction;//зберігання станів графу для "відміни" CTRL+Z
	//перемикання режимів
	modeSwitch mode;
	QPushButton *moveVertexBtn = new QPushButton(this); //кнопка режиму "Переміщення вершини"
	QPushButton *addVertexBtn = new QPushButton(this); //кнопка режиму "Додати вершину"
	QPushButton *addEdgeBtn = new QPushButton(this); //кнопка режиму "Додати шлях"
	QPushButton *deleteBtn = new QPushButton(this); //кнопка режиму "Видалити об'єкт"
	QPushButton *findPathBtn = new QPushButton(this); //кнопка режиму "Знайти найкоротший шлях"
	QIcon *moveIco = new QIcon("move.png"); //іконки для кнопок якщо їх не активовано
	QIcon *addIco = new QIcon("add.png");
	QIcon *wayIco = new QIcon("way.png");
	QIcon *deleteIco = new QIcon("delete.png");
	QIcon *findPathIco = new QIcon("findPath.png");
	QIcon *moveIcoSet = new QIcon("moveSet.png"); //іконки для кнопок якщо їх активовано
	QIcon *addIcoSet = new QIcon("addSet.png");
	QIcon *wayIcoSet = new QIcon("waySet.png");
	QIcon *deleteIcoSet = new QIcon("deleteSet.png");
	QIcon *findPathIcoSet = new QIcon("findPathSet.png");
};

#endif // KOURSE_H
