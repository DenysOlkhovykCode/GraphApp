#include "graphalgorithms.h"
#include <QDebug>

void graphAlgorithms::graphChange(QVector<QVector<int> > other)//переписуємо значення
{
	this->bestLenght = 100000;
	this->bestpath.clear();
	this->paths.clear();
	this->graph.resize(other.size());//робимо масиви однакового розміру
	for(int i=0;i < other.size(); i++)
	{
		this->graph[i].resize(other.size());
	}
	for(int i = 0;i < other.size();i++)//переписуємо всі значення
	{
		for(int j=0;j < other.size();j++)
		{
			if(other[i][j]!=0)
			{
				this->graph[i][j]=other[i][j];
			}
			else
			{
				this->graph[i][j] = 100000;//записуємо бескінечність щоб працював алгоритм
			}
		}
	}
}

void graphAlgorithms::findShortestPath(int startVertex, int finishVertex)//агоритм пошуку найкращого шляху
{
	for (int i = 0; i < this->graph.size(); i++)//створюємо шляхи від startPoint до всіх її сусідів
	{
		if(graph[startVertex][i]!=100000)//якщо не бескінечність значить ребро є
		{
			QVector<int> *newarr = new QVector<int>;
			newarr->push_back(startVertex);
			newarr->push_back(i);
			paths.push_back(*newarr);//додаємо нове ребро
		}
	}
	for(int repeat = 0; repeat < 2; repeat++)//повторюємо декілька разів щоб не залишилося неперевірених маршрутів
	{
		for (int i = 0; i < this->graph.size(); i++)
		{
			for (int j = 0; j < this->graph.size(); j++)
			{
				if((i!=startVertex)&&(j!=startVertex)&&(graph[i][j]!=100000))//минаємо всі шляхи від startPoint
				{
					for(int k = 0;k < paths.size();k++)
					{
						if((paths[k][paths[k].size()-1]==i)
						   &&(std::find(paths[k].begin(),paths[k].end(),j)==paths[k].end())
						   &&(paths[k][paths[k].size()-1]!=finishVertex))//якщо є ребро і ми його ще не додавали і не дійшли до кінцевої вершини
						{
							paths.push_back(paths[k]);
							paths[paths.size()-1].push_back(j);//додаємо нове ребро
							if(std::find(paths.begin(),paths.end(),paths[paths.size()-1])!=paths.end()-1)//якщо ми вже додавали таке ребро
							{
								paths.pop_back();
							}
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < this->paths.size();i++)//видаляємо ті шляхи у яких кінець не співпадає з finishVertex
	{
		if(paths[i][paths[i].size()-1]!=finishVertex)
		{
			paths.remove(i);
			i--;
		}
	}
	int bestpath = 0;//номер найкращого шляху
	for (int i = 0; i < this->paths.size(); i++)//шукаємо найкращий шлях
	{
		int lenght = 0;//довжина шляху
		for (int j = 0; j < this->paths[i].size()-1; j++)//рахуємо суму всіх ребер
		{
			lenght+=graph[paths[i][j]][paths[i][j+1]];
		}
		if(lenght < this->bestLenght)//якщо сума менша ніж найкращий шлях
		{
			this->bestLenght = lenght;//переписуємо значення
			bestpath = i;//запамятовуємо номер
		}
	}
	if(paths.size()>0)
	{
		this->bestpath.resize(this->paths[bestpath].size()-1);
		for (int i = 0; i < this->bestpath.size(); i++)//переписуємо маршрут найкращого шляху в масив
		{
			this->bestpath[i] = this->paths[bestpath][i] * 10 + this->paths[bestpath][i+1];
		}
	}
}
