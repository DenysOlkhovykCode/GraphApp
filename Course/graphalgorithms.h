#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H

#include <QVector>

class graphAlgorithms
{
public:
	graphAlgorithms(){}
	QVector<QVector<int>> paths;//шляхи
	QVector<int> bestpath;//накращий шлях
	int bestLenght = 100000;//довжина найкращого шляху
	void graphChange(QVector<QVector<int>> other);//переписуємо значення
	void findShortestPath(int startVertex, int finishVertex);//агоритм пошуку найкращого шляху
private:
	QVector<QVector<int>> graph;//ребра між вершинами
};

#endif // GRAPHALGORITHMS_H
