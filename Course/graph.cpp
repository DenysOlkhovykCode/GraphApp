#include "graph.h"
//в яку вершину попали курсором
int graphStructure::hitVertex(QPoint coord)
{
	for(int i=0; i < vertices.size(); i++)
	{
		if(((coord.x() >= this->vertices[i].x()-16)
			&& (coord.y() >= this->vertices[i].y()-16))
		   && ((coord.x() <= this->vertices[i].x()+16)
			   && (coord.y() <= this->vertices[i].y()+16)))
			//якщо попали в яку-небудь вершину
		{
			return i;
		}
	}
	return noVertex;
}
//в яке ребро попали курсором
QPair<int, int> graphStructure::hitWay(QPoint coord)
{
	for (int i = 0; i < this->ways.size(); ++i)
	{
		for (int j = 0; j < this->ways.size(); ++j)
		{
			if((i!=j)&&((ways[i][j]!=0)||(ways[j][i]!=0)))
			{
				int VModifaier = 0, HModifaier = 0;
				if(abs(vertices[i].y()-vertices[j].y()) >
				   abs(vertices[i].x()-vertices[j].x()))
					//якщо по вертикалі різниця відстаней більша ніж по горизонталі
				{
					HModifaier = abs(vertices[i].y()-vertices[j].y())/10;//записуємо цю різницю
				}
				else
				{
					VModifaier = abs(vertices[i].x()-vertices[j].x())/10;//записуємо цю різницю
				}
				if(i < j)//якщо від вершини з меншим номером до вершини з більшим номером
				{
					HModifaier = HModifaier * -1;//дуга стає у верхньому положенні
					VModifaier = VModifaier * -1;
				}
				QPoint center;//рахуємо центр відрізка між двома вершинами
				center.setX((vertices[i].x() + vertices[j].x())/2);
				center.setY((vertices[i].y() + vertices[j].y())/2);
				int x0 = coord.x() , x1 = vertices[i].x(), x2 = vertices[j].x(), x3 = center.x()+HModifaier;
				int y0 = coord.y() , y1 = vertices[i].y(), y2 = vertices[j].y(), y3 = center.y()+VModifaier;
				//формула з інтернету за якою якщо точчка миші потрапляє в трикутник утворений 2 вершинами і їх центром зі зміщенням то видаляємо ребро між ними
				double v1 = (x1-x0)*(y2-y1)-(x2-x1)*(y1-y0);
				double v2 = (x2-x0)*(y3-y2)-(x3-x2)*(y2-y0);
				double v3 = (x3-x0)*(y1-y3)-(x1-x3)*(y3-y0);
				if(((v1<=0)&&(v2<=0)&&(v3<=0))||((v1>=0)&&(v2>=0)&&(v3>=0)))
				{
					return QPair(i,j);
				}
			}
		}
	}
	return QPair(-1,-1);
}

QPoint graphStructure::center(QPoint p1, QPoint p2)
{
	QPoint center;//рахуємо центр відрізка між двома вершинами
	center.setX((p1.x() + p2.x())/2);
	center.setY((p1.y() + p2.y())/2);
	return center;
}

QPolygon graphStructure::createArrowPolygon(QPoint pnt1, QPoint pnt2)
{
	float x, y;
	float f1x2 , f1y2;
	float lons, ugol;
	float ostr = 0.05;//гострота стрілки
	x = pnt2.x() - pnt1.x();
	y = pnt2.y() - pnt1.y();
	QPolygon poly;
	poly<<QPoint(pnt1.x(), pnt1.y());
	ugol = atan2(y, x);//кут нахилу
	lons = sqrt(x*x + y*y) / 100 * 55;
	f1x2 = pnt2.x() - lons * cos(ugol - ostr);
	f1y2 = pnt2.y() - lons * sin(ugol - ostr);
	poly<<QPoint(f1x2, f1y2);
	f1x2 = pnt2.x() - lons * cos(ugol + ostr);
	f1y2 = pnt2.y() - lons * sin(ugol + ostr);
	poly<<QPoint(f1x2, f1y2);
	return poly;
}
//переміщення та зупинка вершини
int graphStructure::moveVertex(QPoint coord, int vertexID)
{
	this->vertices[vertexID] = coord;
	return noVertex;
}
//додати нову вершину
void graphStructure::addVertex(QPoint coord, int vertexID)
{
	this->vertices.push_back(coord);
	this->numbers.push_back(vertexID);
	QVector<int> *temp = new QVector<int>;
	this->ways.push_back(*temp);//додаємо місце для ребер
	for (int i = 0; i < this->ways.size(); i++)
	{
		while(this->ways[i].size()!=this->vertices.size())
		{
			this->ways[i].push_back(0);
		}
	}
}
//додати нове ребро
int graphStructure::addEdge(QPoint coord, int startVertex)
{
	int i = this->hitVertex(coord);
	if(startVertex == -1)//якщо ще не виберали точок
	{
		return i;//записуємо її
	}
	else if((startVertex != i)&&(i >= 0))//якщо вже вибрали першу і вони не співпадають
	{
		MyDialog *dia = new MyDialog;
		dia->show();//відкриваємо діалогове вікно
		while(dia->exec() != QDialog::Accepted)
		{
			if(dia->getOrientation() == 1)//якщо вибрали орієнтоване ребро
			{
				ways[startVertex][i] = dia->getInput();
			}
			else if (dia->getOrientation() == 2)//якщо вибрали неорієнтоване ребро
			{
				ways[startVertex][i] = ways[i][startVertex] = dia->getInput();
			}
			break;
		}
	}
	return noVertex;//скидаємо до значення за замовчуванням
}
//видалити вершину
void graphStructure::deleteVertex(QPoint coord)
{
	int i = this->hitVertex(coord);
	if(i >= 0)
	{	vertices.remove(i);//видаляємо цю вершину
		numbers.remove(i);
		ways.remove(i);//видаляємо повязані з нею ребра
		for (int j = 0; j < ways.size(); j++)
		{
			ways[j].remove(i);
		}
	}
}
//видалити ребро
void graphStructure::deletedEdge(QPoint coord)
{
	QPair pnt = this->hitWay(coord);
	if((pnt.second!=-1)||(pnt.second!=-1))
	{
		if(this->ways[pnt.first][pnt.second] == this->ways[pnt.second][pnt.first] )
		{
			this->ways[pnt.first][pnt.second] = this->ways[pnt.second][pnt.first]= 0;
		}
		else
		{
			this->ways[pnt.first][pnt.second] = 0;
		}
	}
}
//намалювати всі вершини
void graphStructure::drawVertices(QPainter &painter, int startVertex)
{
	QString str;
	for(int i=0;i<vertices.size();i++)
	{
		painter.setPen(QPen(Qt::black, 2));
		if(startVertex==i)//якщо вершину вибрали для додавання нового ребра або як почток шляху
		{
			painter.setBrush(QBrush(QColor(199,98,122)));
		}
		else
		{
			painter.setBrush(QBrush(QColor(104,174,186)));
		}
		painter.drawEllipse(vertices[i],16,16);
		str.setNum(this->numbers[i]);
		QPoint pnt(vertices[i].x()-4-(int)log10(this->numbers[i])*5,vertices[i].y()+6);
		painter.drawText(pnt,str);//малюємо номер вершини
	}
}

void graphStructure::drawPathLenght(QPainter &painter, int finishVertex)
{
	//малюємо значення довжини шляху справа від вершини
	if(algo.bestLenght!=100000)
	{
		QString str;
		str.setNum(algo.bestLenght);
		int param =(int)log10(abs(algo.bestLenght))*5;
		QPoint pnt(vertices[finishVertex].x()-35-param,vertices[finishVertex].y()+6);
		painter.setBrush(QBrush(QColor(199,98,122)));
		painter.drawRect(vertices[finishVertex].x()-30-param-10,vertices[finishVertex].y()-10,20+param,20);
		painter.setPen(QPen(Qt::black, 2));
		painter.drawText(pnt,str);
	}
}
//намалювати всі ребра
void graphStructure::drawEdges(QPainter &painter)
{
	//малювання ребер між вершинами
	for (int i = 0; i < ways.size(); i++)
	{
		for (int j = 0; j < ways.size(); j++)
		{
			if(ways[i][j]!=0) //якщо є ребро(за умовами проекту ребро не може дорівнювати 0)
			{
				QPoint center = this->center(this->vertices[i],this->vertices[j]);//рахуємо центр відрізка між двома вершинами
				int rectSizeFactor = (int)log10(abs(ways[i][j]))*10;//коефіцієнт який впливає на розмір "комірки" для значення довжини ребра
				if(ways[i][j]<0)
					rectSizeFactor += 2;
				if(ways[i][j]==ways[j][i])//якщо ребро неорієнтоване(в обидві сторони однаковий)
				{
					if(((std::find(algo.bestpath.begin(),algo.bestpath.end(), i*10+j)!=algo.bestpath.end())
					   ||(std::find(algo.bestpath.begin(),algo.bestpath.end(), j*10+i)!=algo.bestpath.end())))
					{
						painter.setPen(QPen(QColor(199,98,122), 3));
					}
					painter.drawLine(vertices[i].x(),vertices[i].y(),vertices[j].x(),vertices[j].y());//малюємо лінію між вершинами
					painter.setPen(QPen(Qt::black, 2));
					painter.drawRect(center.x()-10,center.y()-10,20+rectSizeFactor,20);//малюємо "комірку" для значення довжини ребра
					QString str;
					str.setNum(ways[i][j]);
					painter.setBrush(QBrush(QColor(104,174,186)));
					painter.drawText(center.x()-5,center.y()+5,str);//пишемо занчення довжини ребра
				}
				else//якщо ребро орієнтоване (різне значення довжини ребра в залежності від обраного напрямку)
				{
					if((std::find(algo.bestpath.begin(),algo.bestpath.end(), i*10+j)!=algo.bestpath.end()))
					{
						painter.setPen(QPen(QColor(199,98,122), 3));
					}
					else
					{
						painter.setPen(QPen(Qt::black, 2));
					}
					int VModifaier = 0, HModifaier = 0;
					double rectModifaier = 0.5;
					int textModifaier = 0;
					if(abs(vertices[i].y()-vertices[j].y()) >
					   abs(vertices[i].x()-vertices[j].x()))
						//якщо по вертикалі різниця відстаней більша ніж по горизонталі
					{
						HModifaier = abs(vertices[i].y()-vertices[j].y())/10;//записуємо цю різницю
						textModifaier = 7;//модифікатор для зміщення "комірки" зі значенням довжини бо по вертикалі вдбувається зміщення
					}
					else
					{
						VModifaier = abs(vertices[i].x()-vertices[j].x())/10;//записуємо цю різницю
					}
					if(i < j)//якщо від вершини з меншим номером до вершини з більшим номером
					{
						HModifaier = HModifaier * -1;//дуга стає у верхньому положенні
						VModifaier = VModifaier * -1;
						rectModifaier = 1;//немає пояснення але потрібно змітити "комірку"
					}
					QPainterPath path;
					//малюємо криву від 1 вершини через точку по центру відрізка, але зміщену, до 2 вершини
					path.moveTo(vertices[i].x(),
								vertices[i].y());
					path.cubicTo(center.x()+HModifaier,
								 center.y()+VModifaier,
								 center.x()+HModifaier,
								 center.y()+VModifaier,
								 vertices[j].x(),
								 vertices[j].y());
					painter.setBrush(QBrush(Qt::NoBrush));
					painter.drawPath(path);
					//малюємо накінечник стрілки
					QPoint startArrow;
					QPoint endArrow;
					startArrow.setX(vertices[j].x());
					startArrow.setY(vertices[j].y());
					endArrow.setX(center.x() + HModifaier);
					endArrow.setY(center.y() + VModifaier);
					QPolygon poly = createArrowPolygon(startArrow,endArrow);
					painter.setPen(QPen(Qt::black, 2));
					painter.setBrush(QBrush(QColor(104,174,186)));
					painter.drawPolygon(poly);
					//маюємо "комірку" зі значенням довжини ребра
					painter.drawRect(center.x()+HModifaier/(2*rectModifaier)-5-textModifaier,
									 center.y()+VModifaier/(2*rectModifaier)-16,20+rectSizeFactor,20);
					QString str;
					str.setNum(ways[i][j]);
					painter.drawText(center.x()+HModifaier/(2*rectModifaier)-textModifaier,
									 center.y()+VModifaier/(2*rectModifaier),str);
				}
			}
		}
	}
}
//зберігання в файл
void graphStructure::saveToFile()
{
	QFile *file = new QFile("save.txt");
	if (!file->open(QIODevice::WriteOnly))//якщо не вдалося відкрити файл
	{
		qDebug("Cannot open file for saving");
	}
	QTextStream out(file);//змінна для запису тексту у файл
	out<<this->vertices.size();//переписуємо розмір масиву з вершинами
	Qt::endl(out);
	for (int i = 0; i < this->vertices.size(); i++)//переписуємо вершини
	{
		out<<this->vertices[i].x()<<" ";
		out<<this->vertices[i].y()<<" ";
		out<<this->numbers[i]<<" ";
		Qt::endl(out);
	}
	out<<this->ways.size();//переписуємо розмір масиву з ребрами
	Qt::endl(out);
	for (int i = 0; i < this->ways.size(); i++)//переписуємо ребра
	{
		for (int j = 0; j < this->ways.size(); j++)
		{
			out<<this->ways[i][j] << " ";
		}
		Qt::endl(out);
	}
}
//відновелння даних з файлу
int graphStructure::restoreFromFile()
{
	QFile *file = new QFile("save.txt");
	if (!file->open(QIODevice::ReadOnly))//якщо не вдалося відкрити файл
	{
		qDebug("Cannot open file for reading");
	}
	QTextStream in(file);//змінна для читання з файлу
	QString l1 = in.readLine();
	this->vertices.resize(l1.toInt());//перписуємо розмір масиву з вершинами
	this->numbers.resize(l1.toInt());
	int maxNumberVertex = 0;//максимальний номер вершини
	for(int i = 0; i < this->vertices.size(); i++)//переписуємо вершини
	{
		QString line = in.readLine();
		QStringList fields = line.split(" ", Qt::SkipEmptyParts);
		this->vertices[i].setX(fields.at(0).toInt());
		this->vertices[i].setY(fields.at(1).toInt());
		this->numbers[i] = fields.at(2).toInt();
		if(this->numbers[i] > maxNumberVertex)//шукаємо максимальний номер вершини
		{
			maxNumberVertex = this->numbers[i];
		}
	}
	QString l2 = in.readLine();
	this->ways.resize(l2.toInt());//перписуємо розмір масиву з ребрами
	for(int i = 0; i< this->ways.size(); i++)
	{
		this->ways[i].resize(l2.toInt());
	}
	for(int i = 0; i < this->ways.size(); i++)//переписуємо ребра
	{
		QString line = in.readLine();
		QStringList fields = line.split(" ", Qt::SkipEmptyParts);
		for(int j = 0; j < this->ways.size(); j++)
		{
			this->ways[i][j] = fields.at(j).toInt();
		}
	}
	if(maxNumberVertex>0)
	{
		return maxNumberVertex+1;
	}
	else
	{
		return 1;
	}
}
//додати нову контрольну точку для відміни
void undoGraphState::addState(graphStructure newGraph)
{
	graphs.push_back(newGraph);
}
