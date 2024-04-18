#ifndef GRAPH_H
#define GRAPH_H

#include <QPoint>
#include <QVector>
#include <QList>
#include <cmath>
#include <QPainter>
#include <QPainterPath>
#include <QFile>
#include <QPair>
#include <QDebug>
#include <QTextStream>
#include "graphalgorithms.h"
#include "dialog.h"
#define noVertex -1


class graphStructure
{
public:
	graphStructure(){}
	int hitVertex(QPoint coord);//в яку вершину попали курсором
	QPair<int,int> hitWay(QPoint coord);//в яке ребро попали курсором
	QPoint center(QPoint p1, QPoint p2);
	QPolygon createArrowPolygon(QPoint pnt1,QPoint pnt2);
	int moveVertex(QPoint coord, int vertexID);//переміщення та зупинка вершини
	void addVertex(QPoint coord, int pointID);//додати нову вершину
	int addEdge(QPoint coord, int startVertex);//додати нове ребро
	void deleteVertex(QPoint coord);//видалити вершину
	void deletedEdge(QPoint coord);//видалити ребро
	void drawVertices(QPainter &painter, int startPoint);//намалювати всі вершини
	void drawPathLenght(QPainter &painter, int finishPoint);//намалювати комірку зі значенням довжини шляху біля кінцевої вершини
	void drawEdges(QPainter &painter);//намалювати всі ребра
	void saveToFile();//зберігання в файл
	int restoreFromFile();//відновелння даних з файлу
	QVector<QPoint> vertices;//координати вершин
	QVector<int> numbers;//номери вершин
	QVector<QVector<int>> ways;//значення ребер між вершинами
	graphAlgorithms algo;//алгоритм пошуку найкоротшого шляху в графі
};

class undoGraphState
{
public:
	undoGraphState(){}
	void addState(graphStructure newGraph);//додати нову контрольну точку для відміни
	QList<graphStructure> graphs;//всіконтрольні точки до який можна відмінити
};

#endif // GRAPH_H
